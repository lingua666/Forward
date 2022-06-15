
#include <libNetFrame_Packet/PAddressHead.h>

PAddressHead::PAddressHead( void )
{
	_sHead.resize(HeadSize());
}

PAddressHead::~PAddressHead( void )
{

}

void	PAddressHead::Fill( UInt8 u8Flag, NETHANDLE Src, NETHANDLE Dst )
{
	tagAddress_HEAD* pHead = (tagAddress_HEAD*)_sHead.c_str();
	pHead->_Info._Src = Src;
	pHead->_Info._Dst = Dst;
	pHead->_Len._uFlag = u8Flag;
	pHead->_Len._uSize = 0;
}

void	PAddressHead::Append( const char* szData, UInt16 u16Size )
{
	tagAddress_HEAD* pHead = (tagAddress_HEAD*)_sHead.c_str();
	pHead->_Len._uSize += u16Size;
	_sHead.append(szData, u16Size);
}

_string_type	PAddressHead::RemoveAddress( void )
{
	const int iLen = HeadSize();
	return _string_type(iLen, _sHead.size() - iLen);
}

void	PAddressHead::Swap( void )
{
	if( _sHead.size() >= HeadSize() )
	{
		tagAddress_HEAD* pHead = (tagAddress_HEAD*)_sHead.c_str();
		NETHANDLE Node = pHead->_Info._Src;
		pHead->_Info._Src = pHead->_Info._Dst;
		pHead->_Info._Dst = Node;
	}
}

tagAddress_HEAD* PAddressHead::Get( void ) const
{
	if( _sHead.size() <= 0 )
		return NULL;
	return (tagAddress_HEAD*)_sHead.c_str();
}

const _string_type&	PAddressHead::GetString( void ) const
{
	return _sHead;
}

int	PAddressHead::Parse( const char* c_szData, UInt32 u32Size )
{
	if( u32Size < HeadSize() )
		return -1;

	UInt16 u16Size = ((tagAddress_HEAD*)c_szData)->_Len._uSize + HeadSize();
	_sHead = _string_type(c_szData, u16Size);
	return u16Size;
}
