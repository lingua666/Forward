
#ifndef __COMMANDSESSION_20150917152415_1442474655_H__
#define __COMMANDSESSION_20150917152415_1442474655_H__

#include <libNetFrame_CmdSession/CommandStreamCtrl.h>
#include <libNetFrame_CmdSession/External.h>

namespace	_session_{

	namespace	_tcp_{

#pragma pack(push, 1)
		struct _session_hdr
		{
			UInt8	_uPriority : 4;
			UInt8	_uType : 4;
		};
#pragma pack(pop)

		#define SESSION_HEART_TYPE_DEF	(0x0F)

		typedef void*	SESSIONHANDLE;

		typedef	function20_handle<void (NETHANDLE, const char*, UInt32)>	Comamand_HRecv;
		typedef	function20_handle<void (NETHANDLE)>						Comamand_HClose;
		typedef	function20_handle<void (void)>							Comamand_HDestroy;
		typedef	function20_handle<void (NETHANDLE, const char*)>		Comamand_HExcept;

		/*!
		* @class  CommandSession
		* @brief   CommandSession
		* @{
		*/

		template<typename T>
		class CommandSession
		{
		public:

			/*!
			* @function   CommandSession
			* @brief    
			* @return  
			*/
			CommandSession( const UInt32 uHeadSpace )
				: _iUseRef(0)
				, _StreamCtrl( REVBUF_SIZE, uHeadSpace )
			{
		
			}

			/*!
			* @function   ~CommandSession
			* @brief    
			* @return  
			*/
			~CommandSession( void )
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

			int Send( T* pAddtion, const char* c_pData, UInt32 uSize )
			{
				return _StreamCtrl.Send(pAddtion, sizeof(T),
									c_pData, uSize);
			}

			int Send( const char* c_pData, UInt32 uSize )
			{
				return _StreamCtrl.Send(c_pData, uSize);
			}

			void	HandleRecv( const CmdDBuf_ptr& Buf_ptr )
			{
				EVENT_TEMPLATE_EXEC_READ(this, Buf_ptr);
			}

			void	HandleClose( const STREAM_HANDLE& Stream )
			{
				EVENT_TEMPLATE_EXEC_CLOSE(this);
			}

			void	HandleExcept( const STREAM_HANDLE& Stream, const char* strError )
			{
				EVENT_TEMPLATE_EXEC_EXCEPT(this, strError);
			}

			void HeartTimeOut( void )
			{
				Close();
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

			void	SetCloseHandle( const Comamand_HClose& Handle )
			{
				_CloseHandle = Handle;
			}

			void	SetDestroyHandle( const Comamand_HDestroy& hDestroy )
			{
				_DestroyHandle = hDestroy;
			}

			void	SetExceptHandle( const Comamand_HExcept& Handle )
			{
				_ExceptHandle = Handle;
			}

			void	SetRecvHandle( const Comamand_HRecv& Handle )
			{
				_RecvHandle = Handle;
			}

			void	SetSendCompleteNotify( const HSendComplete& Handle )
			{
				_StreamCtrl.Set_SendCompleteNotify(Handle);
			}

			void	RecvNotify( NETHANDLE Node, const char* c_szData, UInt32 u32Size )
			{
				if( _RecvHandle )
					_RecvHandle(Node, c_szData, u32Size);
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
			EVENT_REGISTER_LIST_BEGIN(CommandStreamCtrl)
				EVENT_REGISTER_READ(&CommandSession::HandleRecv)
			EVENT_REGISTER_LIST_END()

			EVENT_REGISTER_LIST_BEGIN(SockStream)
				EVENT_REGISTER_CLOSE(&CommandSession::HandleClose)
				EVENT_REGISTER_EXCEPT(&CommandSession::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()
				PERMISSION_READ(CommandSession*, const CmdDBuf_ptr&)
				PERMISSION_EXCEPT(CommandSession*, const char*)
				PERMISSION_CLOSE(CommandSession*)
			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/

		private:
			long				_iUseRef;
			STREAM_HANDLE		_Stream_ptr;
			CommandStreamCtrl	_StreamCtrl;

			Comamand_HExcept		_ExceptHandle;
			Comamand_HRecv			_RecvHandle;
			Comamand_HClose			_CloseHandle;
			Comamand_HDestroy		_DestroyHandle;
		};
		/** @} end CommandSession */

	} //namespace	_tcp_

}	// namespace	_client_

#endif//__COMMANDSESSION_20150917152415_1442474655_H__
