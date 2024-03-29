﻿#include "jkwait.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <time.h>

#ifdef WIN32
#include <process.h>
#include <wtypes.h>

#pragma comment( lib,"winmm.lib" )
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/select.h>
#endif


#define DEFAULT_RATE                    10
#define DEF_SYSTEM_CPUNUM               2

namespace jk {

	class jkwait
	{
	private:
		jkwait();
		~jkwait();

		int init();

		void run_thread();
		uint32_t get_cpu_core_num();
		int64_t get_sys_cpu_time();
		int64_t get_pro_cpu_time();

		int64_t calculate_cpu_rate(int64_t& now_sys_cpu_time, int64_t& now_proc_cpu_time);
		bool try_lock(bool islocked, int64_t rate);
		int32_t lock();
		int32_t unlock();

		void check_sleep();
		uint32_t get_check_interval();
	public:
		void try_wait();
		int set_limit(int rate);
		int status_();
		int start();
		void stop();

		void check_rate();
	public:
		static jkwait w;
	private:

		int rate_limit;
		int status;
		bool _stop;

		int64_t proc_pre_cpu_time;
		int64_t sys_pre_cpu_time;
		uint32_t core_num;

#ifdef WIN32
		HANDLE th_handle;
		HANDLE wait;

		HANDLE hprocess;
#else
		pthread_t th_handle;
		pthread_attr_t th_attr;
		struct sched_param sp;

		pthread_rwlockattr_t lk_attr;
		pthread_rwlock_t wait;
#endif
	};

	jkwait jkwait::w;

	jkwait::jkwait()
		: rate_limit(DEFAULT_RATE)
		, _stop(false)
		, status(UNRUN)
	{
	}


	jkwait::~jkwait()
	{
		stop();
	}

	int jkwait::init()
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

	void jkwait::try_wait()
	{
#ifdef WIN32
		WaitForSingleObject(wait, INFINITE);
#else
		pthread_rwlock_rdlock(&wait);
		pthread_rwlock_unlock(&wait);
#endif
	}

	int jkwait::set_limit(int rate)
	{
		if (rate < 2)
			return -1;

		rate_limit = rate;

		return rate_limit;
	}

	int jkwait::status_()
	{
		return status;
	}


	int jkwait::start()
	{
		stop();

		if (0 == init())
			run_thread();

		return status;
	}


	void jkwait::stop()
	{
		_stop = true;

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
		status = UNRUN;
	}

#ifdef WIN32
	unsigned int __stdcall thread_cb(void* arg)
#else
	void* thread_cb(void* arg)
#endif
	{
		jkwait::w.check_rate();

		return NULL;
	}

	void jkwait::run_thread()
	{
		int ret = 0;
		_stop = false;

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
				ret = pthread_create(&th_handle, NULL, thread_cb, NULL);    // 有些机器禁止线程提优先级，这里再尝试使用默认优先级创建线程

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

	void jkwait::check_rate()
	{
		bool islocked = false;
		int64_t now_sys_cpu_time = 0;
		int64_t now_proc_cpu_time = 0;
		int64_t rate = 0;
		printf("into check_rate,stop:%d, status:%d, cpu_num:%d, sleep_interval:%u\n", _stop, status, core_num, get_check_interval());

		while (!_stop)
		{
			check_sleep();

			if (_stop)
				break;

			rate = calculate_cpu_rate(now_sys_cpu_time, now_proc_cpu_time);
			if (0 > rate)
				break;

			//printf("rate:%lld, rate_limit:%d, cpu_num:%u\n", rate, rate_limit, core_num);
			islocked = try_lock(islocked, rate);
			if (false == islocked)
			{
				proc_pre_cpu_time = now_proc_cpu_time;
				sys_pre_cpu_time = now_sys_cpu_time;
			}
		}

		if (islocked)
		{
			unlock();
			islocked = false;
		}

		printf("leave check_rate,stop:%d, status:%d\n", _stop, status);
	}

	uint32_t jkwait::get_check_interval()
	{
#ifdef WIN32
#define MS 1
#else
#define MS 1000   // linux uslee精度微秒
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

	int64_t jkwait::calculate_cpu_rate(int64_t& now_sys_cpu_time, int64_t& now_proc_cpu_time)
	{
		int64_t proc_cpu_use = 0;
		int64_t sys_cpu_use = 0;
		int64_t rate = 0;

		now_sys_cpu_time = get_sys_cpu_time();
		if (0 > now_sys_cpu_time)
		{
			status = UNRUN;
			return -1;
		}

		now_proc_cpu_time = get_pro_cpu_time();
		if (0 > now_proc_cpu_time)
		{
			status = UNRUN;
			return -1;
		}

		proc_cpu_use = now_proc_cpu_time - proc_pre_cpu_time;
		sys_cpu_use = now_sys_cpu_time - sys_pre_cpu_time;

#ifdef WIN32
		rate = (100 * proc_cpu_use) / sys_cpu_use;  // windows已经将核数计算在内
#else
		// rate = (100 * core_num * proc_cpu_use) / sys_cpu_use;
		rate = (100 * proc_cpu_use) / sys_cpu_use;
#endif

		return rate;
	}

	bool jkwait::try_lock(bool islocked, int64_t rate)
	{
		if (islocked && ( double(rate_limit) > double(1.5) * double(rate))) // 如果前面检查超过rate_limit，这里保险起见，必须降到rate_limit 的1/2以下才重置cpu时间
		{
			//printf("unlock, time:%llu\n", time(0));
			unlock();
			islocked = false;
		}
		else if (!islocked && rate >= rate_limit)
		{
			//printf("locked, time:%llu\n", time(0));
			lock();
			islocked = true;
		}

		return islocked;
	}

	int32_t jkwait::lock()
	{
#ifdef WIN32
		return ResetEvent(wait);
#else
		return pthread_rwlock_wrlock(&wait);
#endif
	}

	int32_t jkwait::unlock()
	{
#ifdef WIN32
		return SetEvent(wait);
#else
		return pthread_rwlock_unlock(&wait);
#endif
	}

	void jkwait::check_sleep()
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

	uint32_t jkwait::get_cpu_core_num()
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
	int64_t jkwait::get_sys_cpu_time()
	{
		timeBeginPeriod(1);
		int64_t sys_tm = timeGetTime(); // 毫秒级
		timeEndPeriod(1);

		return sys_tm;
	}

	int64_t jkwait::get_pro_cpu_time()
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
	int64_t jkwait::get_sys_cpu_time()
	{
		int64_t cpu_time = 0;

		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		cpu_time = ts.tv_sec * 1000l * 1000l * 1000l + ts.tv_nsec;

		return cpu_time;
	}

	int64_t jkwait::get_pro_cpu_time()
	{
		int64_t cpu_time = 0;

		struct timespec ts;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

		cpu_time = ts.tv_sec * 1000l * 1000l * 1000l + ts.tv_nsec;

		return cpu_time;
	}
#endif

	int set_limit(int rate)
	{
		return jkwait::w.set_limit(rate);
	}

	int wait_status()
	{
		return jkwait::w.status_();
	}


	void stop()
	{
		jkwait::w.stop();
	}

	int start()
	{
		return jkwait::w.start();
	}

	void try_wait()
	{
		jkwait::w.try_wait();
	}

}