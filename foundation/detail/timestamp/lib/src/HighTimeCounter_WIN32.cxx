
#include <libTimestamp/HighTimeCounter_WIN32.h>
#include <libCommon/os/UnWindows.h>
#include <libCommon/CMyException.h>
#include <algorithm>


namespace _timestamp_ {

	HighTimeCounter::HighTimeCounter( void )
		: mTimerMask( 0 )
	{
		reset();
	}

	HighTimeCounter::~HighTimeCounter( void )
	{

	}

	bool HighTimeCounter::setOption( const _string_type & key, const void * val )
	{
		if ( key == "QueryAffinityMask" )
		{
			// Telling timer what core to use for a timer read
			DWORD newTimerMask = * static_cast < const DWORD * > ( val );

			// Get the current process core mask
			DWORD_PTR procMask;
			DWORD_PTR sysMask;
			GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

			// If new mask is 0, then set to default behavior, otherwise check
			// to make sure new timer core mask overlaps with process core mask
			// and that new timer core mask is a power of 2 (i.e. a single core)
			if( ( newTimerMask == 0 ) ||
				( ( ( newTimerMask & procMask ) != 0 ) && isPO2( newTimerMask ) ) )
			{
				mTimerMask = newTimerMask;
				return true;
			}
		}

		return false;
	}


	//-------------------------------------------------------------------------
	void HighTimeCounter::reset()
	{
		// Get the current process core mask
		DWORD_PTR procMask = 0;
		DWORD_PTR sysMask = 0;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// If procMask is 0, consider there is only one core available
		// (using 0 as procMask will cause an infinite loop below)
		if (procMask == 0)
			procMask = 1;

		// Find the lowest core that this process uses
		if( mTimerMask == 0 )
		{
			mTimerMask = 1;
			while( ( mTimerMask & procMask ) == 0 )
			{
				mTimerMask <<= 1;
			}
		}

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Get the constant frequency
		QueryPerformanceFrequency(&mFrequency);

		// Query the timer
		QueryPerformanceCounter(&mStartTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		mStartTick = GetTickCount();

		mLastTime = 0;

		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
		epoch.LowPart  = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.LowPart  = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;

		mTimeVal = ts.QuadPart/10;
	}

	//-------------------------------------------------------------------------
	unsigned long HighTimeCounter::getMilliseconds()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		// scale by 1000 for milliseconds
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;

			// Re-calculate milliseconds
			newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
		}

		// Record last time for adjust
		mLastTime = newTime;

		return newTicks;
	}

	//-------------------------------------------------------------------------
	unsigned long HighTimeCounter::getMicroseconds()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		// get milliseconds to check against GetTickCount
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;
		}

		// Record last time for adjust
		mLastTime = newTime;

		// scale by 1000000 for microseconds
		unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

		return newMicro;
	}

	UInt64 HighTimeCounter::get( void ) const
	{
		return mTimeVal;
	}

} // namespace Common
