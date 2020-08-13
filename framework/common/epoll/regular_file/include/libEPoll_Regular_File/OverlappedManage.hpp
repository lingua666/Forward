
#ifndef __OVERLAPPEDMANAGE_REGULAR_20151202110154_1449025314_H__
#define __OVERLAPPEDMANAGE_REGULAR_20151202110154_1449025314_H__

#include "External.h"
#include "CAIO.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_regular_file_{

		struct tagIOData_INFO;
		struct tagOverlapped;

		typedef	function_v20<void (tagIOData_INFO*)> HFNComplete;
		typedef	function_v10<void (tagOverlapped*)> function1_handle;
		
		enum IO_OPERATION_TYPE
		{
			IO_OP_READ,
			IO_OP_WRITE,
			IO_OP_CLOSE,
			IO_OP_WAIT,
			IO_OP_USER
		};

	#pragma pack(push,1)  
		struct WSABUF
		{
			UInt32	len;	/* the length of the buffer */
			char	*buf;	/* the pointer to the buffer */
		};

		struct WSAOVERLAPPED 
		{
			UInt32	uEvent;
			UInt32	uOP;
			UInt64	uOffset;
			struct iocb Aiocb;
		};

		struct tagWSAOverlapped
		{
			WSABUF				wsaBuf;
			WSAOVERLAPPED		wsaOverlapped;
		};
	#pragma pack(pop)

		//ÖØµþÀ©Õ¹
		struct tagOverlapped
		{
			tagWSAOverlapped		_SWSAOverlapp;
			union {
				HANDLE				_Handle;
			}_ADR;
			long			_Ref;
			long			_ListRef;
			HFNComplete		_hfnComplete;
			function1_handle	_Free_Overlapp;

			static int AddRef( tagOverlapped* p )
			{
				return CASAddAndFetch(&p->_Ref);
			}

			static int SubRef( tagOverlapped* p )
			{
				return CASSubAndFetch(&p->_Ref);
			}
		} ;

		struct tagIOData_INFO
		{
			Int32	_ibytes_transferred;
			tagOverlapped*	_pOverlapped;
		};

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
				memset(&(p->_SWSAOverlapp.wsaOverlapped.Aiocb), 0, sizeof(struct iocb));
				p->_ADR._Handle = INVALID_HANDLE_VALUE;
				p->_ListRef = 0;
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
		};

	} //_iocp_net_

#endif

#endif//__OVERLAPPEDMANAGE_20151202110154_1449025314_H__
