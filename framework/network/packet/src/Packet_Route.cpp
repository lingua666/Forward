
#include <libNetFrame_Packet/Packet_Route.h>

Packet_Route::Packet_Route( NETHANDLE Address )
						: _Address(Address)
{

}

Packet_Route::~Packet_Route( void )
{

}

PAddressHead*	Packet_Route::GetHeadObj( void )
{
	return &_Head;
}

tagAddress_HEAD*	Packet_Route::GetRouteInfo( void ) 
{
	tagAddress_HEAD* pHead = _Head.Get();
	if( pHead->_Len._uFlag != 1 )
		return NULL;
	return pHead;
}

tagAddress_HEAD*	Packet_Route::GetHeadInfo( void )
{
	tagAddress_HEAD* pHead = _Head.Get();
	if( pHead->_Len._uFlag != 1 )
		return pHead;

	if( pHead->_Len._uSize == 0 )
		return NULL;

	return (tagAddress_HEAD*)pHead->_Data;
}

void	Packet_Route::Fill( NETHANDLE Dst, UInt16 uHop )
{
	_Head.Fill(1, _Address, Dst);
	_Head.Append((char*)&uHop, sizeof(uHop));
}

int	Packet_Route::AppendHead( const char* c_szData, UInt16 u16Size )
{
	_Head.Append(c_szData, u16Size);
	return 1;
}

int	Packet_Route::AppendData( const char* c_szData, UInt16 u16Size )
{
	_sData.append(c_szData, u16Size);
	return 1;
}

int	Packet_Route::Parse( const char* c_szData, UInt32 u32Size )
{
	if( _Head.HeadSize() > u32Size )
		return -1;

	int iOffset = _Head.Parse(c_szData, u32Size);
	if( iOffset < u32Size )
		_sData.append(c_szData[iOffset], u32Size - iOffset);

	return 1;
}

_string_type	Packet_Route::GetString( void )
{
	return _Head.GetString() + _sData;
}

const _string_type& Packet_Route::GetData( void ) const
{
	return _sData;
}
