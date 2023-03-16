#include "jkwait.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>

#ifdef WIN32
#include <process.h>
#include <wtypes.h>

#pragma comment(lib, "winmm.lib")
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/select.h>
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DEFAULT_RATE 10
#define DEF_SYSTEM_CPUNUM 2

static int rate_limit = DEFAULT_RATE;
static int status = UNRUN;
static BOOL _stop = FALSE;

static int64_t proc_pre_cpu_time;
static int64_t sys_pre_cpu_time;
static uint32_t core_num;

#ifdef WIN32
static HANDLE th_handle;
static HANDLE wait;

static HANDLE hprocess;
#else
static pthread_t th_handle;
static pthread_attr_t th_attr;
static struct sched_param sp;

static pthread_rwlockattr_t lk_attr;
static pthread_rwlock_t wait;
#endif

static uint32_t get_cpu_core_num()
{
#ifdef WIN32
	SYSTEM_INFO info;
	memset(&info, 0, sizeof(info));
	GetSystemInfo(&info);
	printf("dwNumberOfProcessors:%d\n", info.dwNumberOfProcessors);
	return info.dwNumberOfProcessors;
#else
	uint32_t ret = get_nprocs();
	return (ret <= 0) ? DEF_SYSTEM_CPUNUM : ret;
#endif
};

#ifdef WIN32
static int64_t get_sys_cpu_time()
{
	timeBeginPeriod(1);
	int64_t sys_tm = timeGetTime(); // 毫秒级
	timeEndPeriod(1);

	return sys_tm;
}

static int64_t get_pro_cpu_time()
{
	BOOL bRetCode = FALSE;

	FILETIME CreateTime, ExitTime, KernelTime, UserTime;
	LARGE_INTEGER lgKernelTime;
	LARGE_INTEGER lgUserTime;
	LARGE_INTEGER lgCurTime;

	bRetCode = GetProcessTimes(hprocess, &CreateTime, &ExitTime, &KernelTime, &UserTime);
	if (bRetCode)
	{
		lgKernelTime.HighPart = KernelTime.dwHighDateTime;
		lgKernelTime.LowPart = KernelTime.dwLowDateTime;
		lgUserTime.HighPart = UserTime.dwHighDateTime;
		lgUserTime.LowPart = UserTime.dwLowDateTime;
		lgCurTime.QuadPart = (lgKernelTime.QuadPart + lgUserTime.QuadPart);

		return lgCurTime.QuadPart / 10000; // 100纳秒级,除10000是为了精确到毫秒级（和系统timeGetTime保持精度一致）
	}

	return -1;
}
#else
static int64_t get_sys_cpu_time()
{
	int64_t cpu_time = 0;

	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	cpu_time = ts.tv_sec * 1000l * 1000l * 1000l + ts.tv_nsec;

	return cpu_time;
}

static int64_t get_pro_cpu_time()
{
	int64_t cpu_time = 0;

	struct timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

	cpu_time = ts.tv_sec * 1000l * 1000l * 1000l + ts.tv_nsec;

	return cpu_time;
}
#endif

static int64_t calculate_cpu_rate(int64_t *now_sys_cpu_time, int64_t *now_proc_cpu_time)
{
	int64_t proc_cpu_use = 0;
	int64_t sys_cpu_use = 0;
	int64_t rate = 0;

	*now_sys_cpu_time = get_sys_cpu_time();
	if (0 > now_sys_cpu_time)
	{
		status = UNRUN;
		return -1;
	}

	*now_proc_cpu_time = get_pro_cpu_time();
	if (0 > now_proc_cpu_time)
	{
		status = UNRUN;
		return -1;
	}

	proc_cpu_use = *now_proc_cpu_time - proc_pre_cpu_time;
	sys_cpu_use = *now_sys_cpu_time - sys_pre_cpu_time;

#ifdef WIN32
	rate = (100 * proc_cpu_use) / sys_cpu_use; // windows已经将核数计算在内
#else
	// rate = (100 * core_num * proc_cpu_use) / sys_cpu_use;
	rate = (100 * proc_cpu_use) / sys_cpu_use;
#endif

	return rate;
}

static uint32_t get_check_interval()
{
#ifdef WIN32
#define MS 1
#else
#define MS 1000 // linux uslee精度微秒
#endif

	if (core_num < 8)
		return 50 * MS;
	else if (core_num < 16)
		return 25 * MS;
	else if (core_num < 32)
		return 10 * MS;
	else if (core_num < 64)
		return 5 * MS;
	else
		return 2 * MS;
}

static int32_t lock()
{
#ifdef WIN32
	return ResetEvent(wait);
#else
	return pthread_rwlock_wrlock(&wait);
#endif
}

static int32_t unlock()
{
#ifdef WIN32
	return SetEvent(wait);
#else
	return pthread_rwlock_unlock(&wait);
#endif
}

static BOOL try_lock(BOOL islocked, int64_t rate)
{
	// 如果前面检查超过rate_limit，这里保险起见，必须降到rate_limit 的1/2以下才重置cpu时间
	if (islocked && (rate_limit > (1.5f * rate)))
	{
		// printf("unlock, time:%llu\n", time(0));
		unlock();
		islocked = FALSE;
	}
	else if (!islocked && rate >= rate_limit)
	{
		// printf("locked, time:%llu\n", time(0));
		lock();
		islocked = TRUE;
	}

	return islocked;
}

static void check_sleep()
{
#ifdef WIN32
	timeBeginPeriod(1);
	Sleep(get_check_interval());
	timeEndPeriod(1);
#else
	static struct timeval delay = {0, 0};
	delay.tv_usec = get_check_interval();
	select(0, NULL, NULL, NULL, &delay);
	// usleep(get_check_interval());
#endif
}

static void check_rate()
{
	BOOL islocked = FALSE;
	int64_t now_sys_cpu_time = 0;
	int64_t now_proc_cpu_time = 0;
	int64_t rate = 0;
	printf("into check_rate,stop:%d, status:%d, cpu_num:%d, sleep_interval:%u\n", _stop, status, core_num, get_check_interval());

	while (!_stop)
	{
		check_sleep();

		if (_stop)
			break;

		rate = calculate_cpu_rate(&now_sys_cpu_time, &now_proc_cpu_time);
		if (0 > rate)
			break;

		// printf("rate:%lld, rate_limit:%d, cpu_num:%u\n", rate, rate_limit, core_num);
		islocked = try_lock(islocked, rate);
		if (FALSE == islocked)
		{
			proc_pre_cpu_time = now_proc_cpu_time;
			sys_pre_cpu_time = now_sys_cpu_time;
		}
	}

	if (islocked)
	{
		unlock();
		islocked = islocked;
	}

	printf("leave check_rate,stop:%d, status:%d\n", _stop, status);
}

#ifdef WIN32
static unsigned int __stdcall thread_cb(void *arg)
{
	check_rate();

	return 0;
}
#else
static void *thread_cb(void *arg)
{
	check_rate();

	return NULL;
}
#endif

static void run_thread()
{
	int ret = 0;

	for (int i = 0; i < 3; i++)
	{
#ifdef WIN32
		th_handle = (HANDLE)_beginthreadex(NULL, 0, thread_cb, NULL, 0, NULL);
		if (NULL != th_handle)
		{
			SetThreadPriority(th_handle, THREAD_PRIORITY_TIME_CRITICAL);
			break;
		}
#else
		ret = pthread_create(&th_handle, &th_attr, thread_cb, NULL);
		if (EPERM == ret)
			ret = pthread_create(&th_handle, NULL, thread_cb, NULL); // 有些机器禁止线程提优先级，这里再尝试使用默认优先级创建线程

		if (0 == ret)
			break;

		printf("create thread fail, errno:%d, ret:%d\n", errno, ret);
#endif

		// 等待1s
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	status = (0 == ret) ? RUNNING : UNRUN;
}

static int init()
{
#ifdef WIN32
	th_handle = NULL;
	wait = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (NULL == wait)
	{
		printf("CreateEvent fail, errno:%d\n", errno);
		return -1;
	}

	hprocess = GetCurrentProcess();
#else
	th_handle = 0;

	// 设置读写锁写优先
	// PTHREAD_RWLOCK_PREFER_READER_NP(默认设置) 读者优先，可能会导致写者饥饿情况
	// PTHREAD_RWLOCK_PREFER_WRITER_NP 写者优先，目前有 BUG，导致表现行为和 PTHREAD_RWLOCK_PREFER_READER_NP 一致
	// PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP 写者优先，但写者不能递归加锁
	pthread_rwlockattr_init(&lk_attr);
	pthread_rwlockattr_setkind_np(&lk_attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
	pthread_rwlock_init(&wait, &lk_attr);

	// 线程优先级
	pthread_attr_init(&th_attr);
	pthread_attr_getschedparam(&th_attr, &sp);
	pthread_attr_setschedpolicy(&th_attr, SCHED_RR);
	sp.sched_priority = sched_get_priority_max(SCHED_RR);
	pthread_attr_setschedparam(&th_attr, &sp);
	pthread_attr_setinheritsched(&th_attr, PTHREAD_EXPLICIT_SCHED);
#endif

	core_num = get_cpu_core_num();
	printf("core_num:%d\n", core_num);
	sys_pre_cpu_time = get_sys_cpu_time();
	if (sys_pre_cpu_time < 0)
		return -1;

	proc_pre_cpu_time = get_pro_cpu_time();
	if (proc_pre_cpu_time < 0)
		return -1;

	return 0;
}

int set_limit(int rate)
{
	if (rate < 2)
		return -1;

	rate_limit = rate;

	return rate_limit;
}

int wait_status()
{
	return status;
}

int start()
{
	stop();

	_stop = FALSE;

	if (0 == init())
		run_thread();

	return status;
}

void try_wait()
{
#ifdef WIN32
	WaitForSingleObject(wait, INFINITE);
#else
	pthread_rwlock_rdlock(&wait);
	pthread_rwlock_unlock(&wait);
#endif
}

void stop()
{
	_stop = TRUE;

#ifdef WIN32
	if (NULL != wait)
		CloseHandle(wait);

	if (NULL != th_handle)
	{
		WaitForSingleObject(th_handle, INFINITE);
		CloseHandle(th_handle);
	}
#else
	if (0 != th_handle)
		pthread_join(th_handle, NULL);

	pthread_rwlock_destroy(&wait);
	pthread_rwlockattr_destroy(&lk_attr);
#endif
}
