
#ifndef __ERROR_20170510105605_1438860785_H__
#define __ERROR_20170510105605_1438860785_H__

#include "Platform.h"
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <libCommon/os/windowsi.h>
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	#include <errno.h>
#endif

class _sys_error_
{
public:
	static _sys_error_* GetInstance( void )
	{
		static _sys_error_	s_error;
		return &s_error;
	}

	static Int32 GetLastError( void )
	{
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		return ::GetLastError();
#elif defined( PLATFORM_OS_FAMILY_UNIX )
		return errno;
#endif
	}

public:
	_sys_error_( void )
		: _u32Error(0)
	{

	}

	_sys_error_( UInt32 u32Error )
		: _u32Error(u32Error)
	{

	}

	~_sys_error_( void )
	{

	}

	_string_type message( void ) const
	{
		return _string_type::NumberToStr(_u32Error);
	}

	inline void set( UInt32 u32Error )
	{
		_u32Error = u32Error;
	}

	inline UInt16 get( void ) const
	{
		return _u32Error;
	}

	operator bool ()
	{
		return _u32Error != 0;
	}

	bool operator!()
	{ 
		return _u32Error == 0;
	}

	operator bool () const
	{
		return _u32Error != 0;
	}

	bool operator!() const
	{ 
		return _u32Error == 0;
	}
private:
	UInt32	_u32Error;
};

#endif