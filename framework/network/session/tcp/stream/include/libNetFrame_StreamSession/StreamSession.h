
#ifndef __STREAMSESSION_20150917152437_2442474677_H__
#define __STREAMSESSION_20150917152437_2442474677_H__

#include <libNetFrame_StreamSession/StreamCtrl.h>
#include <libNetFrame_StreamSession/External.h>

namespace	_session_{

	namespace	_tcp_{

		typedef	function20_handle<void (NETHANDLE, const StreamBuf_ptr&, long)>	Stream_HRecv;
		typedef	function20_handle<void (NETHANDLE)>							Stream_HClose;
		typedef	function20_handle<void (void)>								Stream_HDestroy;
		typedef	function20_handle<void (NETHANDLE , const char*)>			Stream_HExcept;

		/*!
		* @class  StreamSession
		* @brief  StreamSession
		* @{
		*/

		class StreamSession
		{
		public:
			typedef LinkList_type<StreamBuf_ptr>		LList;

		public:
			/*!
			* @function   StreamSession
			* @brief    
			* @return  
			*/
			StreamSession( const UInt32 uHeadSpace )
				: _iUseRef(0)
				, _StreamCtrl( REVBUF_SIZE, uHeadSpace )
			{

			}

			/*!
			* @function   ~StreamSession
			* @brief    
			* @return  
			*/
			~StreamSession( void )
			{
				Close();
				if( _DestroyHandle )
				{
					_DestroyHandle();
				}
			}

			void Close( void )
			{
				if( _Stream_ptr )
					_Stream_ptr->Close();
			}

			int	Init( io_service& ioService, const _SOCKET_::HSOCKET& Socket )
			{
				_Stream_ptr = SockStream::Create(&ioService, 
					_StreamCtrl.GetBufSize(),
					REVBUF_SIZE);

				if( !_Stream_ptr || _Stream_ptr->Attach(Socket) != 1 )
					return -1;

				_Stream_ptr->Init();

				EVENT_REGISTER(_Stream_ptr.get(),&_StreamCtrl);
				EVENT_REGISTER(_Stream_ptr.get(),this);
				EVENT_REGISTER(&_StreamCtrl,this);
				EVENT_EXEC_ATTACH(_Stream_ptr.get(),&_StreamCtrl);
				return 1;
			}

			int Send( UInt32 CommandType, const char* c_pData, UInt32 uSize )
			{
				return _StreamCtrl.Send(&CommandType, sizeof(CommandType),
					c_pData, uSize);
			}

			int Send( const char* c_pData, UInt32 uSize )
			{
				return _StreamCtrl.Send(c_pData, uSize);
			}

			void	HandleRecv( const StreamBuf_ptr& Buf_ptr )
			{
				EVENT_EXEC_READ(this, Buf_ptr);
			}

			void	HandleClose( const STREAM_HANDLE& Stream )
			{
				EVENT_EXEC_CLOSE(this);
			}

			void	HandleExcept( const STREAM_HANDLE& Stream, const char* strError )
			{
				EVENT_EXEC_EXCEPT(this, strError);
			}

			NETHANDLE GetRemoteNode( void ) const 
			{
				return _Stream_ptr->GetSocketInfo().remote_node;
			}

			NETHANDLE GetLocalNode( void ) const 
			{
				return _Stream_ptr->GetSocketInfo().local_node;
			}

			int	Run( void )
			{
				if( !_Stream_ptr )
					return -1;

				return _Stream_ptr->Run();
			}

			inline bool	isDestory( void ) const
			{
				return _Stream_ptr.use_count() == 1 && GetUseRef() == 0 ? true : false;
			}

			inline bool is_valid( void ) const
			{
				return _Stream_ptr.use_count() != 1;
			}

			inline void AddUseRef( void )
			{
				CASAddAndFetch(&_iUseRef);
			}

			inline void SubUseRef( void )
			{
				CASSubAndFetch(&_iUseRef);
			}

			inline int	GetUseRef( void ) const 
			{
				return _iUseRef;
			}

			const STREAM_HANDLE	GetStream( void ) const 
			{
				return _Stream_ptr;
			}

			void push(const StreamBuf_ptr& ptr)
			{
				_DQueue.push_back(ptr);
			}

			StreamBuf_ptr pop_front(void)
			{
				return _DQueue.pop_front();
			}

			LList::size_type size(void)
			{
				return _DQueue.size();
			}

			void Lock()
			{
				_Lock.Lock();
			}

			void UnLock()
			{
				_Lock.UnLock();
			}

			bool TryLock()
			{
				return _Lock.TryLock();
			}

			void	SetCloseHandle( const Stream_HClose& Handle )
			{
				_CloseHandle = Handle;
			}

			void	SetDestroyHandle( const Stream_HDestroy& hDestroy )
			{
				_DestroyHandle = hDestroy;
			}

			void	SetExceptHandle( const Stream_HExcept& Handle )
			{
				_ExceptHandle = Handle;
			}

			void	SetRecvHandle( const Stream_HRecv& Handle )
			{
				_RecvHandle = Handle;
			}

			void	SetSendCompleteNotify( const HSendComplete& Handle )
			{
				_StreamCtrl.Set_SendCompleteNotify(Handle);
			}

			void	RecvNotify( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr, long lRemain )
			{
				if( _RecvHandle )
					_RecvHandle(Node, Buf_ptr, lRemain);
			}

			void	CloseNotify( NETHANDLE Node, const char* c_szIP, UInt16 uPort )
			{
				if( _CloseHandle )
					_CloseHandle(Node);
			}

			void	ExceptNotify( NETHANDLE Node, const char* strError )
			{
				if( _ExceptHandle )
					_ExceptHandle(Node, strError);
			} 

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(StreamCtrl)
				EVENT_REGISTER_READ(&StreamSession::HandleRecv)
				EVENT_REGISTER_LIST_END()

			EVENT_REGISTER_LIST_BEGIN(SockStream)
				EVENT_REGISTER_CLOSE(&StreamSession::HandleClose)
				EVENT_REGISTER_EXCEPT(&StreamSession::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()
				PERMISSION_READ(StreamSession*, const StreamBuf_ptr&)
				PERMISSION_EXCEPT(StreamSession*, const char* )
				PERMISSION_CLOSE(StreamSession* )
			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/

		private:
			long				_iUseRef;
			STREAM_HANDLE		_Stream_ptr;
			StreamCtrl			_StreamCtrl;
			Stream_HExcept		_ExceptHandle;
			Stream_HRecv		_RecvHandle;
			Stream_HClose		_CloseHandle;
			Stream_HDestroy		_DestroyHandle;
			CLock				_Lock;
			LList				_DQueue;
		};
		/** @} end StreamSession */

	} //namespace	_tcp_

} // client

#endif//__MEDIASESSION_20150917152437_1442474677_H__
