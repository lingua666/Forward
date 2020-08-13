
#ifndef __WEAK_V20_PTR_20160128142449_1453962289_H__
#define __WEAK_V20_PTR_20160128142449_1453962289_H__

#include "SmartPtr.hpp"

namespace	_smart_ptr_v20_{

/*!
* @class  weak_ptr
* @brief   weak_ptr
* @{
*/

template<typename T>
class weak_ptr
{
public:
	template <typename Y> friend class SmartPtr;
	typedef	weak_ptr	self_type;
	typedef	T&			elem_ref;
	typedef	T*			elem_ptr;

private:
	elem_ptr		  _px;
	sp_counted_base * _pi;

public:
	/*!
	* @function   weak_ptr
	* @brief    
	* @return  
	*/
	weak_ptr( void )
		: _pi( NULL )
		, _px( NULL )
	{

	}

	weak_ptr( weak_ptr const& r )
		: _pi( r._pi )
		, _px( r._px )
	{
		AddRef();
	}

	weak_ptr& operator=( weak_ptr const& r )
	{
		_pi = r._pi;
		_px = r._px;
		AddRef();
		return *this;
	}

	template<typename Y>
	weak_ptr( _shared_ptr<Y> const& r )
		: _pi( r._pi )
		, _px( r._px )
	{
		AddRef();
	}

	template<class Y>
	weak_ptr & operator=( _shared_ptr<Y> const& r )
	{
		_pi = r._pi;
		_px = r._px;
		AddRef();
		return *this;
	}

	/*!
	* @function   ~weak_ptr
	* @brief    
	* @return  
	*/
	~weak_ptr( void )
	{
		if( _pi != NULL )
			_pi->weak_release();
	}

	operator bool ( void )
	{
		return !this->empty();
	}

	bool operator!( void )
	{ 
		return this->empty();
	}

	operator bool ( void ) const
	{
		return !this->empty();
	}

	bool operator!( void ) const
	{ 
		return this->empty();
	}

	bool expired( void ) const
	{
		return this->empty();
	}

	inline void AddRef( void )
	{
		if( _pi != NULL )
			return _pi->weak_add_ref();
	}

	_shared_ptr<T> lock( void ) const
	{
		if( !empty() && _pi->use_count() > 0)
			return _shared_ptr<T>(*this);

		return _shared_ptr<T>();
	}

protected:
	bool empty( void ) const
	{
		return _pi == NULL || _pi->use_count() <= 0;
	}
};
/** @} end weak_ptr */

} //_smart_ptr

#endif//__WEAK_PTR_20160128142449_1453962289_H__
