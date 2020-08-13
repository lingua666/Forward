
#ifndef __HIGHTIMECOUNTER_POSIX_20160418173317_1438860785_H__
#define __HIGHTIMECOUNTER_POSIX_20160418173317_1438860785_H__

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

	private:
		Int64			mTimeVal;
	};

}


#endif // __HIGHTIMECOUNTER_20160418173317_1438860785_H__
