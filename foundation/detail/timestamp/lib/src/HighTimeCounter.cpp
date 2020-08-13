
#include <libTimestamp/HighTimeCounter.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)

#include "HighTimeCounter_POSIX.cxx"

#elif defined(PLATFORM_OS_FAMILY_WINDOWS)

#include "HighTimeCounter_WIN32.cxx"

#endif

