
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

void	Packet_Route::Fill( NETHANDLE Dst, UInt32 uHop )
{
	_Head.Fill(1, _Address, Dst);
	_Head.Append((char*)&uHop, sizeof(uHop));
}

int	Packet_Route::AppendHead( const char* c_szData, UInt32 uSize )
{
	_Head.Append(c_szData, uSize);
	return 1;
}

int	Packet_Route::AppendData( const char* c_szData, UInt32 uSize )
{
	_sData.append(c_szData, uSize);
	return 1;
}

int	Packet_Route::Parse( const char* c_szData, UInt32 uInSize )
{
	if( _Head.HeadSize() > uInSize)
		return -1;

	int iOffset = _Head.Parse(c_szData, uInSize);
	if( iOffset < uInSize)
		_sData.append(c_szData[iOffset], uInSize - iOffset);

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
