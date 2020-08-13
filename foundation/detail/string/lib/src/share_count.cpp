
#include <libString/share_count.h>
#include <libString/count_base.hpp>

namespace	_string_{

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

#ifdef __cplusplus
extern "C" {
#endif

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
	
#ifdef  __cplusplus
}
#endif


	class sp_counted_impl_p : public sp_counted_base
	{
	private:
		sp_counted_impl_p( sp_counted_impl_p const & );
		sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

	public:
		static sp_counted_base* GetInstance( UInt32 uSize )
		{
			void* p = MemoryAllocator::Alloc( sizeof(sp_counted_impl_p) + uSize + 1 );
			if(p == NULL) return NULL;
			return new (p) sp_counted_impl_p( reinterpret_cast<char*>(p) + sizeof(sp_counted_impl_p) );
		}

	public:
		sp_counted_impl_p( char* px ): sp_counted_base( px )
		{

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
				MemoryAllocator::Free(_px - sizeof(sp_counted_impl_p));
			}
		}
	};

	class sp_counted_impl_buf : public sp_counted_base
	{
	private:
		sp_counted_impl_buf( sp_counted_impl_buf const & );
		sp_counted_impl_buf & operator= ( sp_counted_impl_buf const & );

	public:
		static sp_counted_base* GetInstance( void* pBuf )
		{
			return new(pBuf) sp_counted_impl_buf( reinterpret_cast<char*>(pBuf) + STRING_OBJ_SIZE );
		}

	public:
		sp_counted_impl_buf( char* px ): sp_counted_base( px )
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
	};

#undef CASCompare
#undef CASFetchAdd
#undef CASAddFetch
#undef CASFetchSub
#undef CASSubFetch

	shared_count::shared_count( void ): _pi(0) // nothrow
	{
	}

	shared_count::shared_count( UInt32 uSize )
	{
		_pi = sp_counted_impl_p::GetInstance( uSize );
	}

	shared_count::shared_count( void* pBuf )
	{
		_pi = sp_counted_impl_buf::GetInstance( pBuf );
	}

	shared_count::~shared_count() // nothrow
	{
		if( _pi != 0 ) _pi->release();
	}

	shared_count::shared_count( shared_count const & r ): _pi(r._pi) // nothrow
	{
		if( _pi != 0 ) _pi->add_ref_copy(_pi);
	}

	shared_count&	shared_count::operator=( const shared_count& r )
	{
		if( r._pi != _pi )
		{
			if( _pi != 0 ) _pi->release();
			if( r._pi != 0 )
			{
				if( r._pi->add_ref_copy(_pi) )
					_pi = r._pi;
			}
		}

		return *this;
	}

#if defined(_SUPPORTED_CXX11_)
	shared_count&	shared_count::operator=( const shared_count&& r )
	{
		if( r._pi != _pi )
		{
			if( _pi != 0 ) _pi->release();
			if( r._pi != 0 ) 
			{
				if( r._pi->add_ref_copy(_pi) )
					_pi = r._pi;
			}
		}

		return *this;
	}
#endif

	void	shared_count::swap( shared_count & r )
	{
		sp_counted_base * tmp = r._pi;
		r._pi = _pi;
		_pi = tmp;
	}

} //namespace	_foundation_