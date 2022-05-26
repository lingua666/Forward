
#include <libCommon/os/Platform_POSIX.h>
#include <libCommon/os/Types.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)

	#include <unistd.h>
	#include <sys/time.h>

	unsigned long GetTickCount( void )
	{
		/*struct timeval tv;
		if (gettimeofday(&tv, 0) != 0)
			return 0;

		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);*/

		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);

	}

	//注：在win10 WSL(即win10子系统)中运行，NT内核上模拟UNIX系统时钟非常棘手, 不能使用nanosleep，
	//具体原因解决方式参考:https://blog.csdn.net/fangye945a/article/details/107161467
	//POSIX.1-2001已将usleep标注为废弃，POSIX.1-2008已删除usleep，应当使用nanosleep替代usleep

	//毫秒
	void millisleep(uint32_t milliseconds) 
	{
		struct timespec ts = {milliseconds / 1000, (milliseconds % 1000) * 1000000};
		while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
	}

	//微妙
	void microsleep(uint32_t microseconds)
	{
		struct timespec ts = { microseconds / 1000000, (microseconds % 1000000) * 1000};
		while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
	}

	//基于select的毫秒级封装
	void selectsleep(int milliseconds) 
	{
		struct timeval timeout = {milliseconds / 1000, (milliseconds * 1000)};
		while (true) 
		{
			(void)select(0, NULL, NULL, NULL, &timeout);
			if (timeout.tv_sec <= 0 && timeout.tv_usec <= 0)
				break;
		}
	}

	void	Sleep( int millisecond )
	{
		//usleep(millisecond * 1000);
		//millisleep(millisecond);
		selectsleep(millisecond);
	}

#endif
