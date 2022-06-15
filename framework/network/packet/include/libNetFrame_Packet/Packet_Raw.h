
#ifndef __PACKET_RAW_20151212170529_1449911129_H__
#define __PACKET_RAW_20151212170529_1449911129_H__

#include "IPack.h"

#define	PACKET_RAW_MAX_PAYLOAD		1460

/*!
* @class  Packet_Raw
* @brief   Packet_Raw
* @{
*/

class Packet_Raw : public	IPack
{
public:
	typedef _io_net_::StreamBuf_ptr	StreamBuf_ptr;
	typedef	function20_handle<void (const StreamBuf_ptr&)>	PKFunction;

public:
	/*!
	* @function   Packet_Raw
	* @brief    
	* @return  
	*/
	Packet_Raw( UInt16 uSendSize, UInt16 ReservedSize )
		: _uSendSize( uSendSize )
		, _ReservedSize( ReservedSize )
		, _MemPool( GetPoolSize() )
		, _ListPool( sizeof(PKBList) )
	{

	}

	/*!
	* @function   ~Packet_Raw
	* @brief    
	* @return  
	*/
	~Packet_Raw( void )
	{
		Release();
	}

	void	Release( void ) 
	{

	}

	UInt16	GetHeadSize( void ) const
	{
		return sizeof(tagPKBuf) - 1;
	}

	UInt16	GetSendSize( void ) const
	{
		return _uSendSize;
	}

	void	SetFunction( const PKFunction& Function ) 
	{
		_Function = Function;
	}

	const PKBuf_ptr	Package( const char* c_InData, UInt16 u16InSize,
							UInt16 u16InPayload )
	{
		tagPKBuf* p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());
		if( p != NULL )
		{
			p->_uPayload = u16InPayload;
			memcpy(p->_Data, c_InData, u16InSize);
			return PKBuf_ptr(p, false);
		}
		return PKBuf_ptr();
	}

	const PKBuf_ptr	Package( const char* c_InData, UInt16 u16InSize ) 
	{
		return Package(c_InData, u16InSize, u16InSize);
	}

	const PKBuf_ptr	Package( void* pAddtion, UInt8 u8AddSize, 
							const char* c_InData, UInt16 u16InSize ) 
	{
		tagPKBuf* p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());
		p->_uPayload = u8AddSize + u16InSize ;
		memcpy(&p->_Data, pAddtion, u8AddSize);
		memcpy(&p->_Data[u8AddSize], c_InData, u16InSize);
		return PKBuf_ptr(p, false);
	}

	const PKBList_ptr	BigPackage( const char* c_InData, UInt16 u16InSize )
	{
		UInt16	u16InLen = 0, u16InOffset = 0;
		PKBList* pList = _ListPool.AllocObj<PKBList>();
		if( pList == NULL )
			return PKBList_ptr();

		//第一个包
		u16InLen = __min(u16InSize, GetSendSize());
		pList->push_back( Package(c_InData, u16InLen) );
		u16InSize -= u16InLen;
		u16InOffset += u16InLen;

		//后续包
		tagPKBuf* p = NULL;
		while( u16InSize > 0 )
		{
			p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());

			u16InLen = __min(u16InSize, GetSendSize());
			memcpy(p->_Data, &c_InData[u16InOffset], u16InLen);
			p->_uPayload = u16InLen;
			pList->push_back( PKBuf_ptr(p, false) );

			u16InSize -= u16InLen;
			u16InOffset += u16InLen;
		};

		return PKBList_ptr(pList, 
			function20_bind(&MemPool_type::FreeMemObj<PKBList>, &_ListPool, _foundation_::_1));
	}

	const PKBList_ptr	BigPackage( void* pAddtion, UInt8 u8AddSize,
									const char* c_InData, UInt16 u16InSize ) 
	{
		UInt16	u16InLen = 0, u16InOffset = 0;
		tagPKBuf* pBuf = NULL;
		PKBList* pList = _ListPool.AllocObj<PKBList>();
		if( pList == NULL )
			return PKBList_ptr();

		PKBuf_ptr sptr = Package((char*)pAddtion, u8AddSize);
		u16InLen = __min(u16InSize, GetSendSize() - sptr->_uPayload);
		memcpy( &sptr->_Data[sptr->_uPayload], c_InData, u16InLen );
		sptr->_uPayload += u16InLen;
		pList->push_back(sptr);

		u16InSize -= u16InLen;
		u16InOffset += u16InLen;

		tagPKBuf* p = NULL;
		while( u16InSize > 0 )
		{
			p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());

			u16InLen = __min(u16InSize, GetSendSize());
			memcpy(p->_Data, &c_InData[u16InOffset], u16InLen);
			p->_uPayload = u16InLen;
			pList->push_back( PKBuf_ptr(p,false) );
			u16InSize -= u16InLen;
			u16InOffset += u16InLen;
		} ;

		return PKBList_ptr(pList, 
			function20_bind(&MemPool_type::FreeMemObj<PKBList>, &_ListPool, _foundation_::_1));
	}

	int	Parse( const char* c_szInData, UInt16 u16InSize,
				const StreamBuf_ptr& Buf_ptr ) 
	{
		_Function(Buf_ptr);
		return 1;
	}

	UInt32	GetPoolSize( void ) const
	{
		return _uSendSize + _ReservedSize + sizeof(tagPKBuf);
	}

	void	FreeBuf( const char*  p )
	{
		_MemPool.Free( (void*)(p - GetHeadSize()) ) ;
	}

private:
	const UInt16 _uSendSize;
	const UInt16 _ReservedSize;

	MemPool_type		_MemPool;
	MemPool_type		_ListPool;
	PKFunction			_Function;
};
/** @} end Packet_Raw */

#endif//__PACKET_RAW_20151212170529_1449911129_H__
