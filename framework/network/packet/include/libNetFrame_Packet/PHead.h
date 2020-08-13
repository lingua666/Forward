
#ifndef __PHEAD_20151109101053_1447035053_H__
#define __PHEAD_20151109101053_1447035053_H__

#include "External.h"

/*!
* @class  PHead
* @brief   PHead
* @{
*/

class PHead
{
public:
	/*!
	* @function   PHead
	* @brief    
	* @return  
	*/
	PHead( void );

	/*!
	* @function   ~PHead
	* @brief    
	* @return  
	*/
	~PHead( void );

	const _string_type& GetHead( void ) const
	{
		return _sHead;
	}

	int	isValid( const char* c_szData ) const
	{
		return memcmp( _sHead.c_str(), c_szData, _sHead.size());
	}

	inline const UInt8	size( void ) const
	{
		return _sHead.size();
	}

protected:
	void Release( void );

private:
	_string_type		_sHead;
};
/** @} end PHead */

#endif//__PHEAD_20151109101053_1447035053_H__
