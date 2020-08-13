
#ifndef __WEAK_PTR_20160128142449_1453962289_H__
#define __WEAK_PTR_20160128142449_1453962289_H__

#include <libSmart_ptr/SmartPtr.h>

namespace	_smart_ptr_{

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

public:
	/*!
	* @function   weak_ptr
	* @brief    
	* @return  
	*/
	weak_ptr( void )
		: _Impl( NULL, true )
	{

	}

	weak_ptr( weak_ptr const& r )
	{
		_Impl.Clone(r._Impl);
		AddRef();
	}

	weak_ptr& operator=( weak_ptr const& r )
	{
		_Impl.Clone(r._Impl);
		AddRef();
		return *this;
	}

	template<typename Y>
	weak_ptr( _shared_ptr<Y> const& r )
	{
		_Impl.Clone(r._Impl);
		AddRef();
	}

	template<class Y>
	weak_ptr & operator=( _shared_ptr<Y> const& r )
	{
		_Impl.Clone(r._Impl);
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
		if( _Impl )
			_Impl.ReleaseHdr();
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

	inline long AddRef( void )
	{
		return _Impl.AddWeakRef();
	}

	_shared_ptr<T> lock( void ) const
	{
		if( _Impl.use_count() > 0)
			return _shared_ptr<T>(*this);

		return _shared_ptr<T>();
	}

	UInt32 use_count( void ) const 
	{
		return _Impl.weak_count();
	}

protected:
	bool empty( void ) const
	{
		return !_Impl || _Impl.use_count() <= 0;
	}

private:
	SmartPtr_Impl	_Impl;
};
/** @} end weak_ptr */

} //_smart_ptr

#endif//__WEAK_PTR_20160128142449_1453962289_H__
