
#ifndef __PACKET_ADDRESS_20160109102049_1452306049_H__
#define __PACKET_ADDRESS_20160109102049_1452306049_H__

#include "PAddressHead.h"

/*!
* @class  Packet_Address
* @brief   Packet_Address
* @{
*/

template<typename H>
class Packet_Address
{
public:
	/*!
	* @function   Packet_Address
	* @brief    
	* @return  
	*/
	Packet_Address( void )
	{

	}

	/*!
	* @function   ~Packet_Address
	* @brief    
	* @return  
	*/
	~Packet_Address( void )
	{

	}

	Packet_Address( const Packet_Address& t1 ) 
	{
		_Head = t1._Head;
		_sData = t1._sData;
	}

	Packet_Address& operator=( const Packet_Address& t1 )
	{
		_Head = t1._Head;
		_sData = t1._sData;
		return *this;
	}

	void	Fill( UInt8 u8Flag, NETHANDLE Src, NETHANDLE Dst )
	{
		_Head.Fill(u8Flag, Src, Dst);
	}

	void	Fill( const H& Head )
	{
		_Head = Head;
	}

	int	AppendHead( const char* c_szData, UInt16 u16Size )
	{
		_Head.Append(c_szData, u16Size);
		return 1;
	}

	int	AppendData( const char* c_szData, UInt16 u16Size )
	{
		_sData.append(c_szData, u16Size);
		return 1;
	}

	int	Parse( const char* c_szData, UInt32 u32Size )
	{
		if( _Head.HeadSize() > u32Size )
			return -1;

		int iOffset = _Head.Parse(c_szData, u32Size);
		if( iOffset < u32Size )
			_sData.append(&c_szData[iOffset], u32Size - iOffset);

		return 1;
	}

	_string_type	GetString( void ) const
	{
		return _Head.GetString() + _sData;
	}

	const H& GetHead( void ) const
	{
		return _Head;
	}

	const _string_type& GetData( void ) const
	{
		return _sData;
	}

	UInt8	GetFlag( void ) const
	{
		return GetHead().Get()->_Len._uFlag;
	}

private:
	H	_Head;
	_string_type	_sData;
};

typedef Packet_Address<PAddressHead>	AddrPacket;
/** @} end Packet_Address */

#endif//__PACKET_ADDRESS_20160109102049_1452306049_H__
