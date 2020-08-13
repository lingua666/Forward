
#ifndef __HIGHTIMECOUNTER_20160418173317_1438860785_H__
#define __HIGHTIMECOUNTER_20160418173317_1438860785_H__

#include <libCommon/modafx.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)

#include <libTimestamp/HighTimeCounter_POSIX.h>

#elif defined(PLATFORM_OS_FAMILY_WINDOWS)

#include <libTimestamp/HighTimeCounter_WIN32.h>

#endif


#endif // __HIGHTIMECOUNTER_20160418173317_1438860785_H__
