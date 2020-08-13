
#ifndef __SMARTPTR_20150808113239_1439004759_H__
#define __SMARTPTR_20150808113239_1439004759_H__

#include <libSmart_ptr/SmartPtr_Impl.h>

namespace	_smart_ptr_{

	template<typename T> class weak_ptr;
	template<typename T> class enable_shared_from_this;
	template<typename T> class SmartPtr;


	template<typename T>
	class SmartPtr
	{
	public:
		template <typename Y> friend class weak_ptr;

		typedef SmartPtr	self_type;
		
		static	void FreeSmartPtr( void* p )
		{
			delete reinterpret_cast<T*>(p);
		}
	
	public:
		SmartPtr( void )
			: _Impl( NULL, true )
		{

		}

		SmartPtr( int t1 )
			: _Impl( NULL, true )
		{

		}

		explicit SmartPtr( T* t1, bool isAuto = true )
			: _Impl(t1, isAuto)
		{
			sp_enable_shared_from_this(this,t1);
		}

		SmartPtr( T* t1, const SMTFunction& Function )
			: _Impl(t1, Function)
		{
			sp_enable_shared_from_this(this,t1);
		}

		SmartPtr( const self_type& t1 )
			: _Impl(t1._Impl)
		{
		
		}

		template<typename Y>
		SmartPtr( weak_ptr<Y> const& t1 )
			: _Impl(t1._Impl)
		{
			
		}

		~SmartPtr( void )
		{
			reset();
		}

		void reset( void )
		{
			_Impl.reset( &FreeSmartPtr );
		}

		template<typename Y>
		self_type& operator=( weak_ptr<Y> const& t1 )
		{
			reset();
			_Impl = t1._Impl;
			return *this;
		}

		self_type& operator=( const self_type& t1 )
		{
			reset();
			_Impl = t1._Impl;
			return *this;
		}

		operator bool ()
		{
			return !_Impl ? false : true;
		}

		bool operator!()
		{ 
			return !_Impl;
		}

		operator bool () const
		{
			return !_Impl ? false : true;
		}

		bool operator!() const
		{ 
			return !_Impl;
		}

		T& operator*() const
		{
			return *(get());
		}

		T*	operator-> ( void ) const 
		{
			return get();
		}

		T* get()
		{
			return reinterpret_cast<T*>(_Impl.get());
		}

		T* get() const
		{
			return reinterpret_cast<T*>(_Impl.get());
		}

		UInt32 use_count( void ) 
		{
			return _Impl.use_count();
		}

		UInt32 use_count( void ) const
		{
			return _Impl.use_count();
		}

	private:
		SmartPtr_Impl	_Impl;
	};

#define _shared_ptr	SmartPtr

	template< class X, class T > 
	inline void sp_enable_shared_from_this( SmartPtr<X> const * ppx,
		enable_shared_from_this< T > const * pe )
	{
		if( pe != 0 )
		{
			pe->_internal_accept_owner( ppx );
		}
	}

	inline void sp_enable_shared_from_this( ... )
	{

	}

} // namespace	_smart_ptr_

#endif//__SMARTPTR_20150808113239_1439004759_H__
