
#ifndef __SMARTPTR_V20_20150808113239_1439004759_H__
#define __SMARTPTR_V20_20150808113239_1439004759_H__

#include <libSmart_ptr_v20/sp_counted_base.hpp>
#include <libSmart_ptr_v20/sp_counted_impl.hpp>

namespace	_smart_ptr_v20_{

	template<typename T> class weak_ptr;
	template<typename T> class enable_shared_from_this;

	template<typename T>
	class SmartPtr
	{
	public:
		template <typename Y> friend class weak_ptr;
		typedef SmartPtr	self_type;
		typedef	T&			elem_ref;
		typedef	T*			elem_ptr;

	private:
		elem_ptr		  _px;
		sp_counted_base * _pi;
	
	public:
		SmartPtr( void )
			: _pi( 0 )
			, _px( 0 )
		{

		}

		explicit SmartPtr( T* t1 )
		{
			_pi = sp_alloc_mem<T>::Alloc(t1);
			_px = t1;
			sp_enable_shared_from_this(this,t1);
		}

		SmartPtr( T* t1, bool isAuto )
		{
			_pi = sp_alloc_mem<T>::Alloc(t1, isAuto);
			_px = t1;
			sp_enable_shared_from_this(this,t1);
		}

		template<typename D>
		SmartPtr( T* t1, D Function )
		{
			_pi = sp_alloc_mem<T>::Alloc(t1, Function);
			_px = t1;
			sp_enable_shared_from_this(this,t1);
		}

		SmartPtr( const self_type& t1 )
			: _pi(t1._pi) // nothrow
			, _px(t1._px)
		{
			if( _pi != 0 ) _pi->add_ref_copy();
		}

		template<typename Y>
		SmartPtr( weak_ptr<Y> const& t1 )
			: _pi(t1._pi) // nothrow
			, _px(t1._px)
		{
			if( _pi != 0 ) _pi->add_ref_copy();
		}

		~SmartPtr() // nothrow
		{
			if( _pi != 0 ) _pi->release();
		}

		template<typename Y>
		self_type& operator=( weak_ptr<Y> const& t1 )
		{
			sp_counted_base * tmp = t1._pi;

			if( tmp != _pi )
			{
				_px = t1._px;
				if( tmp != 0 ) tmp->add_ref_copy();
				if( _pi != 0 ) _pi->release();
				_pi = tmp;
			}

			return *this;
		}

		self_type& operator=( const self_type& t1 )
		{
			sp_counted_base * tmp = t1._pi;

			if( tmp != _pi )
			{
				_px = t1._px;
				if( tmp != 0 ) tmp->add_ref_copy();
				if( _pi != 0 ) _pi->release();
				_pi = tmp;
			}

			return *this;
		}

		bool empty() const // nothrow
		{
			return _pi == 0;
		}

		operator bool ()
		{
			return !empty();
		}

		bool operator!()
		{ 
			return empty();
		}

		operator bool () const
		{
			return !empty();
		}

		bool operator!() const
		{ 
			return empty();
		}

		elem_ref operator*() const
		{
			return *(get());
		}

		elem_ptr	operator-> ( void ) const 
		{
			return get();
		}

		bool operator==( const self_type& t1 )
		{
			return this->get() == t1.get();
		}

		bool operator!=( const self_type& t1 )
		{
			return this->get() != t1.get();
		}

		elem_ptr get() const
		{
			return _px;
		}

		UInt32 use_count( void ) const
		{
			return _pi != 0 ? _pi->use_count(): 0;
		}

		void reset( void )
		{
			if( _pi != 0 ) 
			{
				_pi->release();
				_pi = 0;
			}
		}
	};

#define _shared_ptr	SmartPtr

	template< class X, class T > 
	inline void sp_enable_shared_from_this( _shared_ptr<X> const * ppx,
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
