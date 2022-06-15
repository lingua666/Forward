
#ifndef __PACKET_ROUTE_20160108165656_1452243416_H__
#define __PACKET_ROUTE_20160108165656_1452243416_H__

#include "PAddressHead.h"

struct tagRoute_INFO
{
	tagAddress_HEAD	_hAddr;
	UInt32	_uHop;			//ÌøÊý
	char	_Data[1];
};

class RouteHead
{
public:
	RouteHead( void )
	{

	}

	~RouteHead( void )
	{

	}

	RouteHead( const RouteHead& t1 ) 
	{
		_Head = t1._Head;
	}

	RouteHead& operator=( const RouteHead& t1 )
	{
		_Head = t1._Head;
		return *this;
	}

	void	Fill( UInt8 u8Flag, NETHANDLE Src, NETHANDLE Dst, UInt32 uHop )
	{
		_Head.Fill(u8Flag, Src, Dst);
		_Head.Append((char*)&uHop, sizeof(uHop));
	}

	void	Append( const char* szData, UInt32 uSize )
	{
		_Head.Append(szData, uSize);
	}

	_string_type	RemoveAddress( void )
	{
		const _string_type s = _Head.GetString();
		if(s.size() <= 0)
			return "";

		const int iLen = HeadSize();
		return _string_type(iLen, s.size() - iLen);
	}

	void	Swap( void )
	{
		_Head.Swap();
	}

	tagRoute_INFO* Get( void ) const
	{
		const _string_type& s = _Head.GetString();
		if( s.size() <= 0 )
			return NULL;

		return (tagRoute_INFO*)_Head.GetString().c_str();
	}

	const _string_type&	GetString( void ) const
	{
		_Head.GetString();
	}

	int	Parse( const char* c_szData, UInt32 u32Size )
	{
		if( u32Size < HeadSize() )
			return -1;

		return _Head.Parse(c_szData, u32Size);
	}

	inline UInt8	HeadSize( void ) const
	{
		return _Head.HeadSize() + sizeof(UInt32);
	}

private:
	PAddressHead	_Head;
};

/*!
* @class  Packet_Route
* @brief   Packet_Route
* @{
*/

class Packet_Route
{
public:
	/*!
	* @function   Packet_Route
	* @brief    
	* @return  
	*/
	Packet_Route( NETHANDLE Address );

	/*!
	* @function   ~Packet_Route
	* @brief    
	* @return  
	*/
	~Packet_Route( void );

	Packet_Route( const Packet_Route& t1 ) 
	{
		_Head = t1._Head;
		_sData = t1._sData;
	}

	Packet_Route& operator=( const Packet_Route& t1 )
	{
		_Head = t1._Head;
		_sData = t1._sData;
		return *this;
	}

	void	Fill( NETHANDLE Dst, UInt32 uHop );

	PAddressHead* GetHeadObj( void );

	tagAddress_HEAD*	GetRouteInfo( void );

	tagAddress_HEAD*	GetHeadInfo( void );

	int	AppendHead( const char* c_szData, UInt32 uSize );

	int	AppendData( const char* c_szData, UInt32 uSize );

	int	Parse( const char* c_szData, UInt32 uInSize );

	_string_type	GetString( void );

	const _string_type& GetData( void ) const;

private:
	PAddressHead	_Head;
	_string_type	_sData;
	NETHANDLE	_Address;
};
/** @} end Packet_Route */

#endif//__PACKET_ROUTE_20160108165656_1452243416_H__
