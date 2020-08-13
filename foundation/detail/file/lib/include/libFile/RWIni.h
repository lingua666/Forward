
#ifndef __RWINI_20151031005854_1446224334_H__
#define __RWINI_20151031005854_1446224334_H__


#include <libFile/IniParserWrap.h>

namespace	_file_{

/*!
* @class  RWIni
* @brief   RWIni
* @{
*/

struct RWIni
{
	static	_string_type GetCurrentPath( void );

	static	void Write( const _string_type& sSegName,const _string_type& sKeyName,
						const _string_type& sValue, const _string_type& sFileName );

	static _string_type Read( const _string_type& sSegName, const _string_type& sKeyName,
					const _string_type& sFileName );

	static int ReadInt( const _string_type& sSegName, const _string_type& sKeyName,
							const _string_type& sFileName );

};
/** @} end RWIni */

} // Foundation


#endif//__RWINI_20151031005854_1446224334_H__
