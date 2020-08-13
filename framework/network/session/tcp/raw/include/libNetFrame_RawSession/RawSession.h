
#ifndef __RAWSESSION_20150917152415_1442474115_H__
#define __RAWSESSION_20150917152415_1442474115_H__

#include <libNetFrame_RawSession/RawStreamCtrl.h>
#include <libNetFrame_RawSession/External.h>

namespace	_session_{

	namespace	_tcp_{

		typedef void*	SESSIONHANDLE;

		typedef	function20_handle<void (NETHANDLE , const char*, UInt32)>	Raw_HRecv;
		typedef	function20_handle<void (NETHANDLE)>						Raw_HClose;
		typedef	function20_handle<void (void)>							Raw_HDestroy;
		typedef	function20_handle<void (NETHANDLE , const char*)>		Raw_HExcept;

		class	RawSession;

		/*!
		* @class  RawSession
		* @brief   RawSession
		* @{
		*/

		class RawSession
		{
		public:
	#pragma pack(push, 1)
			struct _session_hdr
			{
				UInt32	type;
			};
	#pragma pack(pop)

			static UInt32	GetHdrSize( void )
			{
				return sizeof(_session_hdr);
			}

			const	_session_hdr*	GetHdr( char* pData )
			{
				reinterpret_cast<_session_hdr*>(pData);
			}

		public:

			/*!
			* @function   RawSession
			* @brief    
			* @return  
			*/
			RawSession( const UInt32 uHeadSpace );

			/*!
			* @function   ~RawSession
			* @brief    
			* @return  
			*/
			~RawSession( void );

			int	Init( io_service& ioService, const _SOCKET_::HSOCKET& Socket );

			void Close( void );

			int Send( const char* c_pData, UInt32 u32Size );

			void	HandleRecv( const STREAM_HANDLE& Stream, const StreamBuf_ptr& Buf_ptr );
			void	HandleClose( const STREAM_HANDLE& Stream );
			void	HandleExcept( const STREAM_HANDLE& Stream, const char* strError );

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
				
				_Stream_ptr->Run();
				return 1;
			}

			inline bool	isDestory( void ) const
			{
				return _Stream_ptr.use_count() == 1 && GetUseRef() == 0 ? true : false;
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

			void	SetCloseHandle( const Raw_HClose& Handle )
			{
				_CloseHandle = Handle;
			}

			void	SetDestroyHandle( const Raw_HDestroy& hDestroy )
			{
				_DestroyHandle = hDestroy;
			}

			void	SetExceptHandle( const Raw_HExcept& Handle )
			{
				_ExceptHandle = Handle;
			}

			void	SetRecvHandle( const Raw_HRecv& Handle )
			{
				_RecvHandle = Handle;
			}

			void	RecvNotify( NETHANDLE Node, const char* pData, UInt32 u32Size )
			{
				if( _RecvHandle )
					_RecvHandle(Node, pData, u32Size);
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
			EVENT_REGISTER_LIST_BEGIN(RawStreamCtrl)
				EVENT_REGISTER_READ(&RawSession::HandleRecv)
			EVENT_REGISTER_LIST_END()

			EVENT_REGISTER_LIST_BEGIN(SockStream)
				EVENT_REGISTER_CLOSE(&RawSession::HandleClose)
				EVENT_REGISTER_EXCEPT(&RawSession::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()
				PERMISSION_READ(RawSession*, const StreamBuf_ptr&)
				PERMISSION_EXCEPT(RawSession*, const char*)
				PERMISSION_CLOSE(RawSession*)
			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/
		private:
			long				_iUseRef;
			NETHANDLE				_Node;
			STREAM_HANDLE		_Stream_ptr;
			RawStreamCtrl		_StreamCtrl;

			Raw_HExcept			_ExceptHandle;
			Raw_HRecv			_RecvHandle;
			Raw_HClose			_CloseHandle;
			Raw_HDestroy		_DestroyHandle;
		};
		/** @} end RawSession */

	} //namespace	_tcp_

} //namespace	_server_

#endif//__RAWSESSION_20150917152415_1442474655_H__
