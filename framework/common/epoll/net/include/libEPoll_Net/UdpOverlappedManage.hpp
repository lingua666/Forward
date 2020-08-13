
#ifndef __UDP_OVERLAPPEDMANAGE_20151202110154_1449025314_H__
#define __UDP_OVERLAPPEDMANAGE_20151202110154_1449025314_H__

#include "WSASocket.h"
#include "External.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{

		namespace _udp_{

			struct tagOverlapped;
			typedef	function_v10<void (tagOverlapped*)> function1_handle;

			typedef	function_v20<int (SOCKET, char*, unsigned long, struct sockaddr_in*)> HFNPerform;
			typedef HFNIOData	HFNComplete;
			//typedef	function_v20<void (tagIOData_INFO*)> HFNComplete;
		
			enum IO_OPERATION_TYPE
			{
				IO_OP_READ,
				IO_OP_WRITE,
				IO_OP_ACCEPT,
				IO_OP_CONNECT,
				IO_OP_CLOSE,
				IO_OP_WAIT,
				IO_OP_USER
			};

		#pragma pack(push,1)  

			struct WSAOVERLAPPED 
			{
				UInt32	uEvent;
				UInt32	uOP;
			};

			struct tagWSAOverlapped
			{
				WSABUF				wsaBuf;
				WSAOVERLAPPED		wsaOverlapped;
			};

		#pragma pack(pop)

			//重叠扩展
			struct tagOverlapped
			{
				tagWSAOverlapped		_SWSAOverlapp;
				union {
					SOCKET				_Sock;
				}_ADR;
				long			_Ref;
				long			_ProcRef;
				long			_RealSize;
				HFNPerform		_hfnPerform;
				HFNComplete		_hfnComplete;
				function1_handle	_Free_Overlapp;
				// struct sockaddr_in指针，直接声明为sockaddr_in在IC2000上运行会出现内存对齐问题
				void*				_pAddr;  

				static int AddRef( tagOverlapped* p )
				{
					return CASAddAndFetch(&p->_Ref);
				}

				static int SubRef( tagOverlapped* p )
				{
					return CASSubAndFetch(&p->_Ref);
				}
			} ;

			/*!
			* @class  OverlappedManage
			* @brief   OverlappedManage
			* @{
			*/
			template<int N = 0>
			class OverlappedManage
			{
			public:
				/*!
				* @function   OverlappedManage
				* @brief    
				* @return  
				*/
				OverlappedManage( void )
					: _Pool(sizeof(tagOverlapped) + N)
					, _AddrPool(sizeof(struct sockaddr_in))
				{
			
				}

				/*!
				* @function   ~OverlappedManage
				* @brief    
				* @return  
				*/
				~OverlappedManage( void )
				{

				}

				tagOverlapped* Alloc( void )
				{
					tagOverlapped* p = reinterpret_cast<tagOverlapped*>(_Pool.AllocObj<tagOverlapped>());
					p->_ADR._Sock = INVALID_SOCKET;
					p->_RealSize = -1;
					p->_ProcRef = 0;
										
					p->_pAddr = _AddrPool.Alloc();

					p->_Ref = 1;
					p->_SWSAOverlapp.wsaBuf.len = N;
					if( N != 0 )
					{
						p->_SWSAOverlapp.wsaBuf.buf = (char*)p + sizeof(tagOverlapped);
					}
					else
					{
						p->_SWSAOverlapp.wsaBuf.buf = NULL;
						p->_SWSAOverlapp.wsaBuf.len = 0;
					}
			
					return p;
				}

				void Free( tagOverlapped* pOverlapped )
				{
					if( pOverlapped != NULL )
					{
						if( pOverlapped->_hfnComplete )
						{
							pOverlapped->_hfnComplete.reset();
						}

						if( pOverlapped->_hfnPerform )
						{
							pOverlapped->_hfnPerform.reset();
						}
						
						if( pOverlapped->_pAddr != NULL )
						{
							_AddrPool.Free(pOverlapped->_pAddr);
						}

						_Pool.FreeObj(pOverlapped);
					}
				}

				void SetBuf( tagOverlapped* pOverlapped, char* c_szData, UInt32 u32Size )
				{
					pOverlapped->_SWSAOverlapp.wsaBuf.buf = c_szData;
					pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
				}

				void GetBuf( tagOverlapped* pOverlapped, WSABUF& out_wBuf ) const
				{
					out_wBuf.buf = pOverlapped->_SWSAOverlapp.wsaBuf.buf;
					out_wBuf.len = pOverlapped->_SWSAOverlapp.wsaBuf.len;
				}

			private:
				MemPool_type	_Pool;
				MemPool_type	_AddrPool;
			};

		} //namespace _tcp_

	} //_iocp_net_

#endif

#endif//__OVERLAPPEDMANAGE_20151202110154_1449025314_H__
