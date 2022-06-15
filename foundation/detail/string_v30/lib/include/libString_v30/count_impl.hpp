
#ifndef __STRING_V30_COUNT_IMPL_20160418173307_1438860785_H__
#define __STRING_V30_COUNT_IMPL_20160418173307_1438860785_H__

#include "count_base.hpp"
#include <libFunction_v20/CMyFunction.hpp>

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
	#include <pthread.h>
	#define CASCompare		__sync_bool_compare_and_swap
	#define	CASFetchAdd		__sync_fetch_and_add
	#define	CASAddFetch		__sync_add_and_fetch
	#define	CASFetchSub		__sync_fetch_and_sub
	#define	CASSubFetch		__sync_sub_and_fetch
#elif defined(_WIN32) || defined(_WIN64)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <intrin.h>
	#define CASCompare		_InterlockedCompareExchange
	#define	CASFetchAdd		_InterlockedExchangeAdd
	#define CASAddFetch     _InterlockedIncrement
	#define	CASFetchSub		_InterlockedExchangeAdd
	#define CASSubFetch     _InterlockedDecrement
#endif

namespace	_string_v30_ {

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

	template<typename Allocate>
	class sp_counted_impl_p : public sp_counted_base
	{
	private:
		static Allocate* GetPoolInstance( void )
		{
			static	Allocate	sPool;
			return &sPool;
		}

		typedef typename function20_handle<void (void)>	HFNFree;
		HFNFree	_hFree;
		long	_buffer_size;

	private:
		sp_counted_impl_p( sp_counted_impl_p const & );
		sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

	public:
		static sp_counted_base* GetInstance( UInt32 uSize )
		{
			HFNFree	hFree;
			long buffer_size = 0;
			void* p = NULL;
			if( GetPoolInstance()->GetLength() < (uSize + sizeof(sp_counted_impl_p)) )
			{
				buffer_size = uSize;
				p = MemoryAllocator::Alloc(sizeof(sp_counted_impl_p) + uSize + 1);
				hFree = function20_bind(&MemoryAllocator::Free, p);
			}
			else
			{
				buffer_size = GetPoolInstance()->GetLength() - sizeof(sp_counted_impl_p) - 1;
				p = GetPoolInstance()->Alloc();
				hFree = function20_bind(&Allocate::Free, GetPoolInstance(), p);
			}
			
			return new (p) sp_counted_impl_p( reinterpret_cast<char*>(p) + sizeof(sp_counted_impl_p), buffer_size, hFree);
		}

	public:
		sp_counted_impl_p( char* px, long buffer_size, const HFNFree& hFree )
			: sp_counted_base( px )
			, _buffer_size(buffer_size)
		{
			_hFree = hFree;
		}

		bool add_ref_copy( sp_counted_base* pi )
		{
			CASAddAndFetch( &_use_count );
			return true;
		}

		void release() // nothrow
		{
			if( CASSubAndFetch( &_use_count ) == 0 )
			{
				if (_hFree)
					_hFree();
			}
		}

		long get_buffer_size(void) const
		{
			return _buffer_size;
		}
	};

	class sp_counted_impl_buf : public sp_counted_base
	{
	private:
		long	_buffer_size;

	private:
		sp_counted_impl_buf( sp_counted_impl_buf const & );
		sp_counted_impl_buf & operator= ( sp_counted_impl_buf const & );

	public:
		static sp_counted_base* GetInstance( void* pBuf, long BufSize )
		{
			return new(pBuf) sp_counted_impl_buf( reinterpret_cast<char*>(pBuf) + STRING_OBJ_SIZE, BufSize);
		}

		static char* GetBuffer( char* pBuf )
		{
			return pBuf + STRING_OBJ_SIZE;
		}

	public:
		sp_counted_impl_buf( char* px, long BufSize)
			: sp_counted_base( px )
			, _buffer_size( BufSize )
		{

		}

		bool add_ref_copy( sp_counted_base* pi )
		{
			memcpy(pi->get(), _px, STRING_BUF_SIZE + 1);
			return false;
		}

		void release() // nothrow
		{

		}

		long get_buffer_size(void) const
		{
			return _buffer_size;
		}
	};

} //namespace	_foundation_

#undef CASCompare
#undef CASFetchAdd
#undef CASAddFetch
#undef CASFetchSub
#undef CASSubFetch


#endif // __MODAFX_20160418173305_1438860785_H__
