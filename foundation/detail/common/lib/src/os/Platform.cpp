
#include <libCommon/os/Platform.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)
#include "Platform_POSIX.cxx"
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
#include "Platform_WIN32.cxx"
#endif


