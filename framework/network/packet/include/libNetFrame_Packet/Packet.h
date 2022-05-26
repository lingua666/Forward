
#ifndef __PACKET_20151109101248_1447035168_H__
#define __PACKET_20151109101248_1447035168_H__

#include "PHead.h"
#include "IPack.h"

#define		MAX_PACKET_PAYLOAD				(1024 * 1024 - 8)
#define		P00L_MAX_BUFFER_SIZE			(MAX_PACKET_PAYLOAD + 8)

#pragma pack(push, 1)
struct tagProcPack_INFO
{
	UInt8				_u8Status;		//接收包的状态
	UInt32				_uOffset;		//缓冲区偏移量
	UInt32				_uPayload;		//包的有效负载，即数据大小
	char*				_pHead;			//头缓冲区
	char*				_pData;			//数据缓冲区
};

#pragma pack(pop)

typedef	function20_handle<void (const PKBuf_ptr&)>	PKFunction;


/*!
* @class  Packet
* @brief   Packet
* @{
*/

template<typename H>
class Packet : public	IPack
{
public:
	enum
	{
		enPHEAD_STATUS_NULL,
		enPHEAD_STATUS_WAIT_HEAD,
		enPHEAD_STATUS_WAIT_DATA
	} enProcStatus;

public:
	/*!
	* @function   Packet
	* @brief    
	* @return  
	*/
	Packet( UInt32 uSendSize, UInt32 ReservedSize )
		: _uSendSize( __min(uSendSize, GetMaxPayload()) )
		, _ReservedSize( ReservedSize )
		, _MemPool( GetPoolSize() )
		, _ListPool( sizeof(PKBList) )
		, _szMaxBuf( NULL )
	{
		_tagData._uOffset = 0;
		_tagData._uPayload = 0;
		_tagData._u8Status = enPHEAD_STATUS_WAIT_HEAD;
		_tagData._pHead = reinterpret_cast<char*>(MemoryAllocator::Alloc(GetHeadSize()));
		_tagData._pData = NULL;
	}

	/*!
	* @function   ~Packet
	* @brief    
	* @return  
	*/
	~Packet( void )
	{
		Release();
	}

	void Release( void )
	{
		if ( _tagData._pHead != NULL )
		{
			MemoryAllocator::Free(_tagData._pHead);
			_tagData._pHead = NULL;
		}

		//如果最大缓冲区未释放，则释放内存
		if( _szMaxBuf != NULL )
		{
			Singleton< MemPool_macro<P00L_MAX_BUFFER_SIZE> >::instance()->Free(_szMaxBuf);
			_szMaxBuf = NULL;
		}
	}

	UInt32	GetHeadSize( void ) const
	{
		return _Head.size() + sizeof(tagPKBuf) - 1;
	}

	UInt32	GetPayload( const char* c_szHead ) const
	{
		return *(UInt32*)(c_szHead + _Head.size());
	}

	UInt32	GetSendSize( void ) const
	{
		return _uSendSize;
	}

	UInt32	GetPoolSize( void ) const
	{
		return _uSendSize + _ReservedSize + GetHeadSize() + 1;
	}

	UInt32	GetMaxPayload( void ) const
	{
		return MAX_PACKET_PAYLOAD;
	}

	void SetFunction( const PKFunction& Func )
	{
		_Func = Func;
	}

	int	Package( const char* c_InData, UInt32 uInSize, char* c_OutData,
				UInt32 uOutSize )
	{
		const UInt32 uLen = _Head.GetHead().size();
		tagPKBuf* pBuf = reinterpret_cast<tagPKBuf*>(c_OutData + uLen);
		memcpy(c_OutData, _Head.GetHead().c_str(), uLen);
		pBuf->_uPayload = uInSize;
		memcpy(pBuf->_Data, c_InData, uInSize);
		return uInSize + uLen + sizeof(pBuf->_uPayload);
	}

	const PKBuf_ptr	Package(const char* c_InData, UInt32 uInSize,
		UInt32 uInPayload)
	{
		const UInt32 uLen = _Head.GetHead().size();
		tagPKBuf* p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());
		if (p != NULL)
		{
			tagPKBuf* pBuf = reinterpret_cast<tagPKBuf*>(p->_Data + uLen);
			memcpy(p->_Data, _Head.GetHead().c_str(), uLen);
			pBuf->_uPayload = uInPayload;
			memcpy(&pBuf->_Data, c_InData, uInSize);
			p->_uPayload = uInSize + uLen + sizeof(pBuf->_uPayload);
			return PKBuf_ptr(p, false);
		}
		return PKBuf_ptr();
	}

	const PKBuf_ptr	Package( const char* c_InData, UInt32 uInSize )
	{
		return Package(c_InData, uInSize, uInSize);
	}

	const PKBuf_ptr	Package( void* pAddtion, UInt8 u8AddSize,
						const char* c_InData, UInt32 uInSize )
	{
		const UInt32 uLen = _Head.GetHead().size();
		tagPKBuf* p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());
		if( p != NULL )
		{
			tagPKBuf* pBuf = reinterpret_cast<tagPKBuf*>(p->_Data + uLen);
			memcpy(p->_Data, _Head.GetHead().c_str(), uLen);
			pBuf->_uPayload = uInSize + u8AddSize;
			memcpy(pBuf->_Data, pAddtion, u8AddSize);
			memcpy(&pBuf->_Data[u8AddSize], c_InData, uInSize);
			p->_uPayload = pBuf->_uPayload + uLen + sizeof(pBuf->_uPayload);
			return PKBuf_ptr(p, false);
		}
		return PKBuf_ptr();
	}

	const PKBList_ptr	BigPackage( const char* c_InData, UInt32 uInSize )
	{
		UInt32	uInLen = 0, uInOffset = 0;
		tagPKBuf* pBuf = NULL;
		PKBList* pList = _ListPool.AllocObj<PKBList>();
		if( pList == NULL )
			return PKBList_ptr();

		//第一个包
		uInLen = __min(uInSize, GetSendSize());
		pList->push_back( Package(c_InData, uInLen, uInSize) );
		uInSize -= uInLen;
		uInOffset += uInLen;

		//后续包
		tagPKBuf* p = NULL;
		while( uInSize > 0 )
		{
			p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());

			uInLen = __min(uInSize, GetSendSize());
			memcpy(p->_Data, &c_InData[uInOffset], uInLen);
			p->_uPayload = uInLen;
			pList->push_back( PKBuf_ptr(p, false) );

			uInSize -= uInLen;
			uInOffset += uInLen;
		};

		return PKBList_ptr(pList, 
			function20_bind(&MemPool_type::FreeMemObj<PKBList>, &_ListPool, _foundation_::_1));
	}

	const PKBList_ptr	BigPackage( void* pAddtion, UInt8 u8AddSize,
						const char* c_InData, UInt32 uInSize )
	{
		UInt32	uInLen = 0, uInOffset = 0;
		tagPKBuf* pBuf = NULL;
		PKBList* pList = _ListPool.AllocObj<PKBList>();
		if( pList == NULL )
			return PKBList_ptr();

		PKBuf_ptr sptr = Package((char*)pAddtion, u8AddSize, uInSize + u8AddSize);
		uInLen = __min(uInSize, GetSendSize() - sptr->_uPayload);
		memcpy( &sptr->_Data[sptr->_uPayload], c_InData, uInLen );
		sptr->_uPayload += uInLen;
		pList->push_back(sptr);

		uInSize -= uInLen;
		uInOffset += uInLen;

		tagPKBuf* p = NULL;
		while( uInSize > 0 )
		{
			p = reinterpret_cast<tagPKBuf*>(_MemPool.Alloc());

			uInLen = __min(uInSize, GetSendSize());
			memcpy(p->_Data, &c_InData[uInOffset], uInLen);
			p->_uPayload = uInLen;
			pList->push_back( PKBuf_ptr(p,false) );
			uInSize -= uInLen;
			uInOffset += uInLen;
		} ;
		
		return PKBList_ptr(pList, 
			function20_bind(&MemPool_type::FreeMemObj<PKBList>, &_ListPool, _foundation_::_1));
	}

	int	Parse( const char* c_szInData, UInt32 uInSize )
	{
		if( !_Func || c_szInData == NULL )
			return -1;

		switch(_tagData._u8Status)
		{
		case enPHEAD_STATUS_WAIT_HEAD:
__gt_head__:
			{			
				do 
				{
					const UInt8 u8Length = __min(GetHeadSize() - _tagData._uOffset,
												uInSize);
					//保存头部信息

					//test
					if( _tagData._uOffset + u8Length > GetHeadSize() )
					{
						printf("Packet::Parse ERROR\r\n");
						_tagData._uOffset = 0;
						return -1;
					}
					//test

					memcpy(&_tagData._pHead[_tagData._uOffset], c_szInData, u8Length);
					_tagData._uOffset += u8Length;
					c_szInData += u8Length;
					uInSize -= u8Length;

					if( _tagData._uOffset == GetHeadSize() )
					{
						if( _Head.isValid(_tagData._pHead) == 0 )
						{
							_tagData._uPayload = GetPayload(_tagData._pHead);
							if( _tagData._uPayload > P00L_MAX_BUFFER_SIZE )
							{
								printf("Packet::Parse Payload:%d > %d Not Allow!!\r\n", _tagData._uPayload, P00L_MAX_BUFFER_SIZE);
								return -1;
							}

							_tagData._uOffset = 0;
							if( _tagData._uPayload > GetSendSize() )
							{
								_tagData._pData = \
									reinterpret_cast<char*>\
									(\
										Singleton< MemPool_macro<P00L_MAX_BUFFER_SIZE> >::instance()->Alloc()\
									);

								//设置最大缓冲区备用地址，以备连接中断时，
								//数据包未接收完，无法释放内存空间问题
								_szMaxBuf = _tagData._pData;
							}
							else
							{
								_tagData._pData = reinterpret_cast<char*>(_MemPool.Alloc());
							}
							tagPKBuf* p = reinterpret_cast<tagPKBuf*>(_tagData._pData);
							p->_uPayload = _tagData._uPayload;

							if( uInSize >= p->_uPayload )
							{//完整一个包
								memcpy(p->_Data, c_szInData, p->_uPayload);
								uInSize -= p->_uPayload;
								c_szInData += p->_uPayload;

								//通知上层
								_Func(PKBuf_ptr(p, function20_bind_Ex(&MemPool_type::Free, &_MemPool)));

								if( uInSize < 0 )
									printf("CommandStreamCtrl::HandleRecv Payload error!!\r\n");
							}
							else
							{
								if( uInSize > 0 )
								{
									memcpy(p->_Data, c_szInData, uInSize);
									_tagData._uOffset = uInSize;
								}
								_tagData._u8Status = enPHEAD_STATUS_WAIT_DATA;
								break;
							}
						}
						else
						{//整个包丢弃
							printf("Packet format is error data(%d):%s!!!\r\n", uInSize, c_szInData);
							_tagData._uOffset = 0;
							uInSize = 0;
						}
					}
				} while ( uInSize > 0 );
			}
			break;
		case enPHEAD_STATUS_WAIT_DATA:
			{
				const UInt32 u32Length = __min(_tagData._uPayload - _tagData._uOffset,
					uInSize);
				tagPKBuf* p = reinterpret_cast<tagPKBuf*>(_tagData._pData);
				memcpy(&p->_Data[_tagData._uOffset], c_szInData, u32Length);
				uInSize -= u32Length;
				c_szInData += u32Length;
				_tagData._uOffset += u32Length;
				if( _tagData._uOffset >= _tagData._uPayload )
				{
					if( _tagData._uOffset > _tagData._uPayload)
					{
						printf("Recvice Data Is Error!!\r\n");
					}

					_tagData._uOffset = 0;
					_tagData._u8Status = enPHEAD_STATUS_WAIT_HEAD;

					//通知上层
					if( _tagData._uPayload > GetSendSize() )
					{
						//接收完毕，清空缓冲区备用地址
						_szMaxBuf = NULL;

						_Func( PKBuf_ptr(p,\
							function20_bind_Ex(&MemPool_macro<P00L_MAX_BUFFER_SIZE>::Free,\
								Singleton< MemPool_macro<P00L_MAX_BUFFER_SIZE> >::instance())));
					}
					else
					{
						_Func(PKBuf_ptr(p, function20_bind_Ex(&MemPool_type::Free, &_MemPool)));
					}
				}

				if( uInSize > 0 )
				{
					goto __gt_head__;
				}
			}

			break;
		}

		return 1;
	}

	void	FreeBuf( const char*  p )
	{
		_MemPool.Free( (void*)(p - (sizeof(tagPKBuf) - 1)) ) ;
	}

private:
	const UInt32 _ReservedSize;
	UInt32		_uSendSize;
	char*		_szMaxBuf;

	MemPool_type		_MemPool;
	MemPool_type		_ListPool;
	H					_Head;
	PKFunction			_Func;
	tagProcPack_INFO	_tagData;
};

/** @} end Packet */

#endif//__PACKET_20151109101248_1447035168_H__
