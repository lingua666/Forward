
#ifndef __UDP_OVERLAPPEDMANAGE_20171122154954_1449025314_H__
#define __UDP_OVERLAPPEDMANAGE_20171122154954_1449025314_H__

#include "WSASocket.h"
#include "External.h"

namespace _iocp_net_{

	namespace _udp_{

	#pragma pack(push,1)

		struct tagOverlapped;
		typedef	function_v10<void (tagOverlapped*)> function1_handle;

		//ÖØµþÀ©Õ¹
		struct tagOverlapped
		{
			SWSAOverlapped		_SWSAOverlapp;
			union {
				SOCKET			_Sock;
			}_ADR;
			long				_Ref;
			HFNIOData			_Func;
			function1_handle	_Free_Overlapp;
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

	#pragma pack(pop)

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
				p->_Ref = 1;
				p->_SWSAOverlapp.wsaBuf.len = N;

				p->_pAddr = _AddrPool.Alloc();

				if( N != 0 )
				{
					p->_SWSAOverlapp.wsaBuf.buf = (char*)p + sizeof(tagOverlapped);
				}
				else
				{
					p->_SWSAOverlapp.wsaBuf.buf = NULL;
					p->_SWSAOverlapp.wsaBuf.len = 0;
				}
			
				memset(&p->_SWSAOverlapp.wsaOverlapped, 0, sizeof(p->_SWSAOverlapp.wsaOverlapped));
				return p;
			}

			void Free( tagOverlapped* pOverlapped )
			{
				if( pOverlapped != NULL )
				{
					if( pOverlapped->_Func )
					{
						pOverlapped->_Func.reset();
					}

					if( pOverlapped->_pAddr != NULL )
					{
						_AddrPool.Free(pOverlapped->_pAddr);
					}

					_Pool.FreeObj(pOverlapped);
				}
			}

			SWSAOverlapped* GetOverlapped( tagOverlapped* pOverlapped ) const
			{
				return &pOverlapped->_SWSAOverlapp;
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

#endif//__OVERLAPPEDMANAGE_20151202110154_1449025314_H__
