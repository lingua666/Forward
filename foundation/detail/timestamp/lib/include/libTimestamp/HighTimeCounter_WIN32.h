
#ifndef __HIGHTIMECOUNTER_WIN32_20160418173317_1438860785_H__
#define __HIGHTIMECOUNTER_WIN32_20160418173317_1438860785_H__

#include <libCommon/modafx.h>
#include <ctime>

namespace _timestamp_ {

	class HighTimeCounter
	{
	public:

		HighTimeCounter( void );

		~HighTimeCounter( void );

		bool setOption( const _string_type & key, const void * val );

		//-------------------------------------------------------------------------
		void reset();

		//-------------------------------------------------------------------------
		unsigned long getMilliseconds();

		//-------------------------------------------------------------------------
		unsigned long getMicroseconds();

		UInt64 get( void ) const;

	protected:
		template<typename T>
		static bool isPO2(T n)
		{
			return (n & (n-1)) == 0;
		}

	private:
		DWORD			mStartTick;
		LONGLONG		mLastTime;
		LARGE_INTEGER	mStartTime;
		LARGE_INTEGER	mFrequency;
		DWORD_PTR		mTimerMask;
		Int64			mTimeVal;
	};

}


#endif // __HIGHTIMECOUNTER_20160418173317_1438860785_H__
