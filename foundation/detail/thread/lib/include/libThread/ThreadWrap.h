
#ifndef __THREADWRAP_20151007194953_1444218593_H__
#define __THREADWRAP_20151007194953_1444218593_H__

#include <libCommon/modafx.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <libThread/ThreadWrap_Win32.h>
#elif defined(PLATFORM_OS_FAMILY_UNIX)
	#include <libThread/ThreadWrap_UNIX.h>
#endif

typedef	_thread_::ThreadWrap			ThreadWrap;
typedef	_thread_::ThreadData			ThreadData;
typedef	_thread_::thread_interrupted	thread_interrupted;

#endif//__THREADWRAP_20151007194953_1444218593_H__
