
#ifndef __ENABLE_SHARED_FROM_THIS_V20_20160128154200_1453966920_H__
#define __ENABLE_SHARED_FROM_THIS_V20_20160128154200_1453966920_H__

#include <libSmart_ptr_v20/weak_ptr.hpp>

namespace	_smart_ptr_v20_{

	/*!
* @class  enable_shared_from_this
* @brief   enable_shared_from_this
* @{
*/
template<typename T>
class enable_shared_from_this
{
public:
	/*!
	* @function   enable_shared_from_this
	* @brief    
	* @return  
	*/
	enable_shared_from_this( void )
	{

	}

	/*!
	* @function   ~enable_shared_from_this
	* @brief    
	* @return  
	*/
	~enable_shared_from_this( void )
	{

	}

	_shared_ptr<T> shared_from_this( void )
	{
		_shared_ptr<T> p( weak_this_ );
		return p;
	}

	_shared_ptr<T const> shared_from_this( void ) const
	{
		_shared_ptr<T const> p( weak_this_ );
		return p;
	}
public: // actually private, but avoids compiler template friendship issues

	// Note: invoked automatically by shared_ptr; do not call
	template<class X>
	void _internal_accept_owner( _shared_ptr<X> const * ppx ) const
	{
		if( weak_this_.expired() )
		{
			weak_this_ = _shared_ptr<T>( *ppx );
		}
	}
private:
	mutable weak_ptr<T> weak_this_;
};
/** @} end enable_shared_from_this */

} //_smart_ptr


#endif//__ENABLE_SHARED_FROM_THIS_20160128154200_1453966920_H__
