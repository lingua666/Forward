
#ifndef __PADDRESS_HEAD_20160108165847_1452243527_H__
#define __PADDRESS_HEAD_20160108165847_1452243527_H__

#include "External.h"

#pragma pack(push,1)
struct tagAddress_INFO
{
	NETHANDLE _Src;//源地址
	NETHANDLE _Dst;//目标地址
};

struct tagHeadLen
{
	UInt16	_uFlag:4;		//包类型    0: 普通包  1:地址信息包
	UInt16	_uSize:12;
};

struct tagAddress_HEAD
{
	tagHeadLen	_Len;
	tagAddress_INFO	_Info;
	char		_Data[1];
};
#pragma pack(pop)

enum	AddressFlag
{
	ENUM_PACKET_GENERAL_FLAG_DEF = 0,
	ENUM_PACKET_ADDRESS_FLAG_DEF
};

/*!
* @class  Packet_Address
* @brief   Packet_Address
* @{
*/

class PAddressHead
{
public:
	/*!
	* @function   Packet_Address
	* @brief    
	* @return  
	*/
	PAddressHead( void );

	/*!
	* @function   ~Packet_Address
	* @brief    
	* @return  
	*/
	~PAddressHead( void );

	PAddressHead( const PAddressHead& t1 ) 
	{
		_sHead = t1._sHead;
	}

	PAddressHead& operator=( const PAddressHead& t1 )
	{
		_sHead = t1._sHead;
		return *this;
	}

	void	Fill( UInt8 u8Flag, NETHANDLE Src, NETHANDLE Dst );

	void	Append( const char* szData, UInt16 u16Size );

	_string_type	RemoveAddress( void );

	void	Swap( void );

	tagAddress_HEAD* Get( void ) const;

	const _string_type&	GetString( void ) const;

	int	Parse( const char* c_szData, UInt32 u32Size );

	inline UInt8	HeadSize( void ) const
	{
		return sizeof(tagAddress_HEAD) - 1;
	}

private:
	_string_type	_sHead;
};
/** @} end Packet_Address */

#endif//__PACKET_ADDRESS_20160108165847_1452243527_H__
