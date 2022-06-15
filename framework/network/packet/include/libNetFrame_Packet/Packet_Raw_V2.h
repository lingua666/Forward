
#ifndef __PACKET_RAW_V2_20151212170529_1449911129_H__
#define __PACKET_RAW_V2_20151212170529_1449911129_H__

#include "IPack.h"

#define	PACKET_RAW_V2_MAX_PAYLOAD		1460

/*!
* @class  Packet_Raw
* @brief   Packet_Raw
* @{
*/

class Packet_Raw_V2 : public	IPack
{
public:
	typedef _io_net_::StreamBuf_ptr	StreamBuf_ptr;
	typedef	function20_handle<void (void*, const StreamBuf_ptr&)>	PKFunction;
	typedef tagPKBuf_V2			tagBuf;
	typedef	PKSmartPtr<tagBuf>	tagBuf_sptr;
	typedef	PLList< tagBuf_sptr, CNoneLock >	BufList;
	typedef	PKSmartPtr<BufList>					BufList_sptr;
public:
	/*!
	* @function   Packet_Raw
	* @brief    
	* @return  
	*/
	Packet_Raw_V2( UInt32 uHeadSize, UInt32 uSendSize, UInt32 ReservedSize )
		: _uHeadSize( uHeadSize )
		, _uSendSize( uSendSize )
		, _uReservedSize( ReservedSize )
		, _MemPool( GetPoolSize() )
		, _ListPool( sizeof(BufList) )
	{

	}

	/*!
	* @function   ~Packet_Raw
	* @brief    
	* @return  
	*/
	~Packet_Raw_V2( void )
	{
		Release();
	}

	void	Release( void ) 
	{

	}

	UInt32	GetHeadSize( void ) const
	{
		return sizeof(tagBuf) - 1;
	}

	UInt32	GetSendSize( void ) const
	{
		return _uSendSize;
	}

	void	SetFunction( const PKFunction& Function ) 
	{
		_Function = Function;
	}

	const tagBuf_sptr	Package( const char* c_InData, UInt32 uInSize,
		UInt32 uInPayload, void* pHead = NULL,
		UInt32 uHeadSize = 0 )
	{
		tagBuf* p = reinterpret_cast<tagBuf*>(_MemPool.Alloc());
		if( p != NULL )
		{
			p->_uPayload = uInPayload;
			memcpy(p->_Data, c_InData, uInSize);

			//head
			if( pHead != NULL && uHeadSize > 0 )
			{
				uHeadSize = __min(uHeadSize, _uHeadSize);
				p->_pHead = &p->_Data[uInPayload + _uReservedSize];
				memcpy(p->_pHead, pHead, uHeadSize);
			}
			return tagBuf_sptr(p, false);
		}
		return tagBuf_sptr();
	}

	const tagBuf_sptr	Package( const char* c_InData, UInt32 uInSize,
						void* pHead = NULL, UInt32 uHeadSize = 0 )
	{
		return Package(c_InData, uInSize, uInSize, pHead, uHeadSize);
	}

	const tagBuf_sptr	Package( void* pAddtion, UInt8 u8AddSize, 
							const char* c_InData, UInt32 uInSize,
							void* pHead = NULL, UInt32 uHeadSize = 0 )
	{
		tagBuf* p = reinterpret_cast<tagBuf*>(_MemPool.Alloc());
		p->_uPayload = u8AddSize + uInSize ;
		memcpy(&p->_Data, pAddtion, u8AddSize);
		memcpy(&p->_Data[u8AddSize], c_InData, uInSize);

		//head
		if( pHead != NULL && uHeadSize > 0 )
		{
			uHeadSize = __min(uHeadSize, _uHeadSize);
			p->_pHead = &p->_Data[p->_uPayload + _uReservedSize];
			memcpy(p->_pHead, pHead, uHeadSize);
		}

		return tagBuf_sptr(p, false);
	}

	const BufList_sptr	BigPackage( const char* c_InData, UInt32 uInSize,
		void* pHead = NULL, UInt32 uHeadSize = 0 )
	{
		UInt32	uInLen = 0, uInOffset = 0;
		BufList* pList = _ListPool.AllocObj<BufList>();
		if( pList == NULL )
			return BufList_sptr();

		//第一个包
		uInLen = __min(uInSize, GetSendSize());
		pList->push_back( Package(c_InData, uInLen, pHead, uHeadSize) );
		uInSize -= uInLen;
		uInOffset += uInLen;

		//后续包
		tagBuf* p = NULL;
		while( uInSize > 0 )
		{
			p = reinterpret_cast<tagBuf*>(_MemPool.Alloc());

			uInLen = __min(uInSize, GetSendSize());
			memcpy(p->_Data, &c_InData[uInOffset], uInLen);
			p->_uPayload = uInLen;
			
			//head
			if( pHead != NULL && uHeadSize > 0 )
			{
				uHeadSize = __min(uHeadSize, _uHeadSize);
				p->_pHead = &p->_Data[p->_uPayload + _uReservedSize];
				memcpy(p->_pHead, pHead, uHeadSize);
			}

			pList->push_back( tagBuf_sptr(p, false) );

			uInSize -= uInLen;
			uInOffset += uInLen;
		};

		return BufList_sptr(pList, 
			function20_bind(&MemPool_type::FreeMemObj<BufList>, &_ListPool, _foundation_::_1));
	}

	const BufList_sptr	BigPackage( void* pAddtion, UInt8 u8AddSize,
									const char* c_InData, UInt32 uInSize,
									void* pHead = NULL, UInt32 uHeadSize = 0 )
	{
		UInt32	uInLen = 0, uInOffset = 0;
		tagBuf* pBuf = NULL;
		BufList* pList = _ListPool.AllocObj<BufList>();
		if( pList == NULL )
			return BufList_sptr();

		tagBuf_sptr sptr = Package((char*)pAddtion, u8AddSize);
		uInLen = __min(uInSize, GetSendSize() - sptr->_uPayload);
		memcpy( &sptr->_Data[sptr->_uPayload], c_InData, uInLen );
		sptr->_uPayload += uInLen;

		//head
		if( pHead != NULL && uHeadSize > 0 )
		{
			uHeadSize = __min(uHeadSize, _uHeadSize);
			sptr->_pHead = &sptr->_Data[sptr->_uPayload + _uReservedSize];
			memcpy(sptr->_pHead, pHead, uHeadSize);
		}

		pList->push_back(sptr);

		uInSize -= uInLen;
		uInOffset += uInLen;

		tagBuf* p = NULL;
		while( uInSize > 0 )
		{
			p = reinterpret_cast<tagBuf*>(_MemPool.Alloc());

			uInLen = __min(uInSize, GetSendSize());
			memcpy(p->_Data, &c_InData[uInOffset], uInLen);
			p->_uPayload = uInLen;
			pList->push_back( tagBuf_sptr(p,false) );
			uInSize -= uInLen;
			uInOffset += uInLen;
		} ;

		return BufList_sptr(pList, 
			function20_bind(&MemPool_type::FreeMemObj<BufList>, &_ListPool, _foundation_::_1));
	}

	int	Parse( void* pUser, const char* c_szInData, UInt32 uInSize,
			const StreamBuf_ptr& Buf_ptr ) 
	{
		_Function(pUser, Buf_ptr);
		return 1;
	}

	UInt32	GetPoolSize( void ) const
	{
		return _uHeadSize + _uSendSize + _uReservedSize + sizeof(tagBuf);
	}

	void	FreeBuf( const char*  p )
	{
		_MemPool.Free( (void*)(p - GetHeadSize()) ) ;
	}

private:
	const UInt32 _uHeadSize;
	const UInt32 _uSendSize;
	const UInt32 _uReservedSize;

	MemPool_type		_MemPool;
	MemPool_type		_ListPool;
	PKFunction			_Function;
};
/** @} end Packet_Raw */

#endif//__PACKET_RAW_20151212170529_1449911129_H__
