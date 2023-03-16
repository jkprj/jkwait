#include "jkwait.h"
#include <stdio.h>

#ifdef WIN32
#include <time.h>
#include <process.h>
#include <wtypes.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif // WIN32


long long calculate();

#ifdef WIN32
unsigned int __stdcall thread__xxx_cb(void* arg)
#else
void* thread__xxx_cb(void* arg)
#endif
{

	while (1)
	{
		calculate();
		JK_TRY_WAIT
	}
}

int main()
{
	JK_WAIT_START
#ifdef WIN32
#else
	setbuf(stdout, NULL);

	pthread_t th_handle;
#endif

	printf("proc start\n");

	for (int i = 0; i < 8; i++)
	{
#ifdef WIN32
		_beginthreadex(NULL, 0, thread__xxx_cb, NULL, 0, NULL);
#else
		pthread_create(&th_handle, NULL, thread__xxx_cb, NULL);
#endif
	}

	// 等待24小时
#ifdef WIN32
	Sleep(24*3600*1000);
#else
	sleep(24*3600);
#endif
	
	JK_STOP
}

long long calculate()
{
	long long i = time(0);

	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++; i++;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000; i = (long long)i % 10000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000; i = (long long)i % 1000000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000; i = (long long)i % 100000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000; i = (long long)i % 10000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000; i = (long long)i % 1000;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;
	i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2; i *= 2; i /= 3; i *= 4; i /= 2;

	return i;
}