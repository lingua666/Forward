
#include <libCommon/os/Platform_WIN32.h>
#include <libCommon/os/Types.h>
#include <sys/timeb.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

	static inline uint64_t win32_get_clockfreq(void)
	{
		static bool have_clockfreq = false;
		static LARGE_INTEGER clock_freq;

		if (!have_clockfreq) {
			QueryPerformanceFrequency(&clock_freq);
			have_clockfreq = true;
		}

		return clock_freq.QuadPart;
	}

	static uint64_t win32_gettime_ns(void)
	{
		LARGE_INTEGER current_time;
		double time_val;

		QueryPerformanceCounter(&current_time);
		time_val = (double)current_time.QuadPart;
		time_val *= 1000000000.0;
		time_val /= (double)win32_get_clockfreq();

		return (uint64_t)time_val;
	}

	static bool win32_sleepto_ns(uint64_t time_target)
	{
		uint64_t t = win32_gettime_ns();
		uint32_t milliseconds;

		if (t >= time_target)
			return false;

		milliseconds = (uint32_t)((time_target - t)/1000000);
		if (milliseconds > 1)
			Sleep(milliseconds-1);

		for (;;) 
		{
			t = win32_gettime_ns();
			if (t >= time_target)
				return true;

			Sleep(0);
		}
	}

	void usleep( long micro_seconds )
	{
		uint64_t t = win32_gettime_ns() + uint64_t(micro_seconds) * 1000;
		win32_sleepto_ns(t);
	}

	static LONG initializeLock_gettimeofday = 0;

	int gettimeofday(struct timeval_t* tp, int* ) 
	{
		static LARGE_INTEGER tickFrequency, epochOffset;

		static bool isInitialized = false;

		LARGE_INTEGER tickNow;

		QueryPerformanceCounter(&tickNow);

		if (!isInitialized) 
		{
			if(1 == InterlockedIncrement(&initializeLock_gettimeofday)) 
			{
				// For our first call, use "ftime()", so that we get a time with a proper epoch.
				// For subsequent calls, use "QueryPerformanceCount()", because it's more fine-grain.
				struct timeb tb;
				ftime(&tb);
				tp->tv_sec = tb.time;
				tp->tv_usec = 1000*tb.millitm;

				// Also get our counter frequency:
				QueryPerformanceFrequency(&tickFrequency);   
				// compute an offset to add to subsequent counter times, so we get a proper epoch:
				epochOffset.QuadPart
					= tp->tv_sec * tickFrequency.QuadPart + (tp->tv_usec * tickFrequency.QuadPart) / 1000000L - tickNow.QuadPart;

				// next caller can use ticks for time calculation
				isInitialized = true; 
				return 0;
			}
			else 
			{
				InterlockedDecrement(&initializeLock_gettimeofday);
				// wait until first caller has initialized static values
				while(!isInitialized)
				{
					Sleep(1);
				}
			}
		}

		// adjust our tick count so that we get a proper epoch:
		tickNow.QuadPart += epochOffset.QuadPart;

		tp->tv_sec =  (long)(tickNow.QuadPart / tickFrequency.QuadPart);
		tp->tv_usec = (long)(((tickNow.QuadPart % tickFrequency.QuadPart) * 1000000L) / tickFrequency.QuadPart);

		return 0;
	}


#endif