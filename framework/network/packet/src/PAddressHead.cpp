
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

void	PAddressHead::Append( const char* szData, UInt32 uSize )
{
	tagAddress_HEAD* pHead = (tagAddress_HEAD*)_sHead.c_str();
	pHead->_Len._uSize += uSize;
	_sHead.append(szData, uSize);
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

int	PAddressHead::Parse( const char* c_szData, UInt32 uInSize )
{
	if(uInSize < HeadSize() )
		return -1;

	UInt32 uSize = ((tagAddress_HEAD*)c_szData)->_Len._uSize + HeadSize();
	_sHead = _string_type(c_szData, uSize);
	return uSize;
}
