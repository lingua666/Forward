
#ifndef __SP_COUNTED_IMPL_20150808113239_1439004759_H__
#define __SP_COUNTED_IMPL_20150808113239_1439004759_H__

#include <libSmart_ptr_v20/sp_counted_base.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libMempool/MemPool.h>
#include <libDesignMode/Singleton.hpp>

namespace	_smart_ptr_v20_{

	template<class X> class sp_counted_impl_p;
	template<class X> class sp_counted_impl_null;
	template<class X, class D> class sp_counted_impl_pd;
	template<typename T> struct sp_alloc_mem;

	typedef	function20_handle< void (void*) >	SMTFunction;

	template<class X> class sp_counted_impl_null: public sp_counted_base
	{
	private:

		sp_counted_impl_null( sp_counted_impl_null const & );
		sp_counted_impl_null & operator= ( sp_counted_impl_null const & );

	public:
		explicit sp_counted_impl_null( X * px )
		{

		}

		void dispose() // nothrow
		{
			
		}

		void destroy()
		{
			sp_alloc_mem<X>::Free(this);
		}
	};

	template<class X> class sp_counted_impl_p: public sp_counted_base
	{
	private:
		X * _px;

		sp_counted_impl_p( sp_counted_impl_p const & );
		sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

	public:
		explicit sp_counted_impl_p( X * px ): _px( px )
		{

		}

		void dispose() // nothrow
		{
			if( _px != NULL )
				delete _px;
		}

		void destroy()
		{
			sp_alloc_mem<X>::Free(this);
		}
	};

	template<class X, class D> class sp_counted_impl_pd: public sp_counted_base
	{
	private:

		X * _px; // copy constructor must not throw
		D _hfnDel; // copy constructor must not throw

		sp_counted_impl_pd( sp_counted_impl_pd const & );
		sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );

	public:

		// pre: d(p) must not throw

		sp_counted_impl_pd( X* p, D& d ): _px( p ), _hfnDel( d )
		{

		}

		virtual void dispose() // nothrow
		{
			_hfnDel( _px );
		}

		virtual void destroy()
		{
			sp_alloc_mem<X>::Free(this);
		}
	};

	template<typename T>
	struct sp_alloc_mem
	{
		/*static MemPool_type* GetSmartHdrPool( void )
		{
			/ *static MemPool_type	_Pool20160726( sizeof(sp_counted_impl_pd<T, SMTFunction>) );
			return &_Pool20160726;* /
			return Singleton<MemPool_type>::instance( sizeof(sp_counted_impl_pd<T, SMTFunction>) );
		}*/

		static sp_counted_base* Alloc( T* ptr, bool isAuto )
		{
			return Singleton<MemPool_type>::instance( sizeof(sp_counted_impl_pd<T, SMTFunction>) )->template AllocObj< sp_counted_impl_null<T> >(ptr);
		}

		static sp_counted_base* Alloc( T* ptr )
		{
			return Singleton<MemPool_type>::instance( sizeof(sp_counted_impl_pd<T, SMTFunction>) )->template AllocObj< sp_counted_impl_p<T> >(ptr);
		}

		template<typename D>
		static sp_counted_base* Alloc( T* ptr, D& Function )
		{
			return Singleton<MemPool_type>::instance( sizeof(sp_counted_impl_pd<T, SMTFunction>) )->template AllocObj< sp_counted_impl_pd<T, D> >(ptr, Function);
		}

		template<typename D>
		static void	Free( D* s )
		{
			Singleton<MemPool_type>::instance( sizeof(sp_counted_impl_pd<T, SMTFunction>) )->FreeObj(s);
		}

		/*static void	Free( sp_counted_base* s )
		{
			Singleton<MemPool_type>::instance( sizeof(sp_counted_impl_pd<T, SMTFunction>) )->FreeObj(s);
		}*/
	};

}

#endif
