
#ifndef __SP_COUNTED_BASE_20150808113239_1439004759_H__
#define __SP_COUNTED_BASE_20150808113239_1439004759_H__

#if defined(PLATFORM_OS_FAMILY_UNIX)
	#include <pthread.h>
	#define CASCompare		__sync_bool_compare_and_swap
	#define	CASFetchAdd		__sync_fetch_and_add
	#define	CASAddFetch		__sync_add_and_fetch
	#define	CASFetchSub		__sync_fetch_and_sub
	#define	CASSubFetch		__sync_sub_and_fetch
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <libCommon/os/windowsi.h>
	#include <intrin.h>
	#define CASCompare		_InterlockedCompareExchange
	#define	CASFetchAdd		_InterlockedExchangeAdd
	#define CASAddFetch     _InterlockedIncrement
	#define	CASFetchSub		_InterlockedExchangeAdd
	#define CASSubFetch     _InterlockedDecrement
#endif

namespace	_smart_ptr_v20_{

	inline bool CASCompareExchange( long* Destination, long oldval,
		long newval)
	{
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
		//sched_yield()   只适合Liunx 功能为让出CPU执行权
		return CASCompare(Destination, oldval, newval);//sched_yield();
#else 
		return CASCompare(Destination, newval, oldval) == oldval ?  true : false;
#endif
	}

	inline const long CASFetchAndAdd( long* Destination )
	{
		return CASFetchAdd(Destination, 1);
	}

	inline const long CASAddAndFetch( long* Destination )
	{
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
		return CASAddFetch(Destination, 1);
#else
		return CASAddFetch(Destination);
#endif
	}

	inline const long CASFetchAndSub( long* Destination )
	{
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
		//sched_yield()   只适合Liunx 功能为让出CPU执行权
		return CASFetchSub(Destination, 1);
#else
		return CASFetchSub(Destination, -1);
#endif

	}

	inline const long CASSubAndFetch( long* Destination )
	{
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
		//sched_yield()   只适合Liunx 功能为让出CPU执行权
		return CASSubFetch(Destination, 1);
#else
		return CASSubFetch(Destination);
#endif
	}

	class sp_counted_base
	{
	private:
		sp_counted_base( sp_counted_base const & );
		sp_counted_base & operator= ( sp_counted_base const & );

		long _use_count;        // #shared
		long _weak_count;       // #weak + (#shared != 0)

	public:
		sp_counted_base(): _use_count( 1 ), _weak_count( 1 )
		{
		}

		virtual void dispose() = 0; // nothrow

		// destroy() is called when weak_count_ drops to zero.

		virtual void destroy() = 0; // nothrow

		void add_ref_copy()
		{
			CASAddAndFetch( &_use_count );
		}

		void release() // nothrow
		{
			if( CASSubAndFetch( &_use_count ) == 0 )
			{
				dispose();
				weak_release();
			}
		}

		void weak_add_ref() // nothrow
		{
			CASAddAndFetch( &_weak_count );
		}

		void weak_release() // nothrow
		{
			if( CASSubAndFetch( &_weak_count ) == 0 )
			{
				destroy();
			}
		}

		long use_count() const // nothrow
		{
			return static_cast<long const volatile &>( _use_count );
		}
	};
}

#undef CASCompare
#undef CASFetchAdd
#undef CASAddFetch
#undef CASFetchSub
#undef CASSubFetch

#endif
