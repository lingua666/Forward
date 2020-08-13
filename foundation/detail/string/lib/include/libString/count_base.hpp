
#ifndef __STRING_COUNT_BASE_20160418173307_1438860785_H__
#define __STRING_COUNT_BASE_20160418173307_1438860785_H__

#include <libBase/modafx.h>
#include <stdlib.h>
#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
	#include <string>
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	//Linuxœ¬÷ß≥÷C++11
	#pragma GCC diagnostic error "-std=c++11"
	
	#include <cstring>
	#include <utility>
	#include <algorithm>
	#include <iostream>
#endif

#if !defined(__min)
	#define __min(X,Y) ((X) < (Y) ? (X) : (Y))
#endif

#if !defined(__max)
	#define __max(X,Y) ((X) < (Y) ? (Y) : (X))
#endif

#if defined(_WIN64) || defined(__LP64__)
	#define	STRING_OBJ_SIZE				24
#else
	#define	STRING_OBJ_SIZE				16
#endif

#define STRING_RESERVED_BUF_SIZE		16
#define STRING_BUF_SIZE					16

typedef	unsigned int	UInt32;
typedef	int				Int32;

namespace	_string_{

	class sp_counted_base
	{
	protected:
		long _use_count;        // #shared
		char* _px;

		sp_counted_base( sp_counted_base const & );
		sp_counted_base & operator= ( sp_counted_base const & );

	public:
		sp_counted_base( char* px ): _use_count( 1 ), _px( px )
		{
		}

		virtual bool add_ref_copy( sp_counted_base* pi ) = 0;

		virtual void release() = 0; // nothrow

		long use_count() const // nothrow
		{
			return static_cast<long const volatile &>( _use_count );
		}

		char*	get( void ) const
		{
			return _px;
		}
	};

} //namespace	_foundation_

#endif // __MODAFX_20160418173305_1438860785_H__
