
#include <libTimestamp/HighTimeCounter_POSIX.h>
#include <libCommon/CMyException.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <algorithm>

namespace _timestamp_ {

	HighTimeCounter::HighTimeCounter( void )
		: mTimeVal( 0 )
	{
		reset();
	}

	HighTimeCounter::~HighTimeCounter( void )
	{

	}

	bool HighTimeCounter::setOption( const _string_type & key, const void * val )
	{
		return false;
	}


	//-------------------------------------------------------------------------
	void HighTimeCounter::reset()
	{
		struct timeval curTime;
		if ( !gettimeofday(&curTime, NULL) )
		{
			mTimeVal = Int64(curTime.tv_sec) * 1000000L + curTime.tv_usec;
		}
	}

	//-------------------------------------------------------------------------
	unsigned long HighTimeCounter::getMilliseconds()
	{
		struct timeval curTime;
		Int64 newTicks = -1;
		if ( !gettimeofday(&curTime, NULL) )
		{
			newTicks = (Int64(curTime.tv_sec) * 1000000L + curTime.tv_usec - mTimeVal) / 1000;
		}

		return newTicks;
	}

	//-------------------------------------------------------------------------
	unsigned long HighTimeCounter::getMicroseconds()
	{
		struct timeval curTime;
		Int64 newMicro = -1;
		if ( !gettimeofday(&curTime, NULL) )
		{
			newMicro = (Int64(curTime.tv_sec) * 1000000L + curTime.tv_usec - mTimeVal) / 1000;
		}

		return newMicro;
	}

	UInt64 HighTimeCounter::get( void ) const
	{
		return mTimeVal;
	}

} // namespace Common
