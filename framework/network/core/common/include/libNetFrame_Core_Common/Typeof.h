
#ifndef __TYPEOF_20150721154433_1437464673_H__
#define __TYPEOF_20150721154433_1437464673_H__

#include <libFoundation/modafx.h>
#include <typeinfo>

typedef std::type_info	type_info;

struct ITypeof
{
	virtual const  type_info & type()  const = 0;
};

/*!
* @class  Typeof
* @brief   Typeof
* @{
*/

template<typename ValueType>
class Typeof : public ITypeof
{
public:
	/*!
	* @function   Typeof
	* @brief    
	* @return  
	*/
	Typeof( void )
	{

	}

	/*!
	* @function   ~Typeof
	* @brief    
	* @return  
	*/
	~Typeof( void )
	{

	}

	const  type_info & type( void )  const
	{ 
		return  typeid ( ValueType );
	}

};
/** @} end Typeof */

#endif//__TYPEOF_20150721154433_1437464673_H__
