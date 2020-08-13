
#include <libCommon/os/Platform.h>


#if defined(PLATFORM_OS_FAMILY_UNIX)
	#include <pthread.h>
	#define CASCompare		__sync_bool_compare_and_swap
	#define	CASFetchAdd		__sync_fetch_and_add
	#define	CASAddFetch		__sync_add_and_fetch
	#define	CASFetchSub		__sync_fetch_and_sub
	#define	CASSubFetch		__sync_sub_and_fetch
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <windows.h>
	#include <intrin.h>
	#define CASCompare		_InterlockedCompareExchange
	#define	CASFetchAdd		_InterlockedExchangeAdd
	#define CASAddFetch     _InterlockedIncrement
	#define	CASFetchSub		_InterlockedExchangeAdd
	#define CASSubFetch     _InterlockedDecrement
#endif

	bool	CASCompareExchange( long* Destination, long oldval,
		long newval )
	{
#if defined(PLATFORM_OS_FAMILY_UNIX)
		//sched_yield()   只适合Liunx 功能为让出CPU执行权
		return CASCompare(Destination, oldval, newval);//sched_yield();
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
		return CASCompare(Destination, newval, oldval) == oldval ?  true : false;
#endif
	}

	const long	CASFetchAndAdd( long* Destination )
	{
		return CASFetchAdd(Destination, 1);
	}

	const long	CASAddAndFetch( long* Destination )
	{
#if defined(PLATFORM_OS_FAMILY_UNIX)
		return CASAddFetch(Destination, 1);
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
		return CASAddFetch(Destination);
#endif
	}

	const long	CASFetchAndSub( long* Destination )
	{
#if defined(PLATFORM_OS_FAMILY_UNIX)
		//sched_yield()   只适合Liunx 功能为让出CPU执行权
		return CASFetchSub(Destination, 1);
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
		return CASFetchSub(Destination, -1);
#endif

	}

	const long	CASSubAndFetch( long* Destination )
	{
#if defined(PLATFORM_OS_FAMILY_UNIX)
		//sched_yield()   只适合Liunx 功能为让出CPU执行权
		return CASSubFetch(Destination, 1);
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
		return CASSubFetch(Destination);
#endif
	}

#undef CASCompare
#undef CASFetchAdd
#undef CASAddFetch
#undef CASFetchSub
#undef CASSubFetch

