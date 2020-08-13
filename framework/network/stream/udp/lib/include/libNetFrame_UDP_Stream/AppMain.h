
#ifndef __UDP_MYSTREAM_20150924143440_2443076480_H__
#define __UDP_MYSTREAM_20150924143440_2443076480_H__

#include <libNetFrame_UDP_Stream/External.h>

namespace	_server_{

	namespace	_udp_{

		using namespace _session_::_udp_;
		typedef _session_::_udp_::_scoket_info	_scoket_info;
		typedef	_session_::_udp_::StreamBuf_ptr	StreamBuf_ptr;
		typedef _session_::_udp_::StreamBuf		StreamBuf;
		typedef _io_net_::_udp_::io_service		io_service;
		typedef NETHANDLE						_HASH;

		/*!
		* @class  AppMain
		* @brief   AppMain
		* @{
		*/

		class AppMain
		{
		public:
			typedef	_session_::_udp_::Stream_HClose									Stream_HDestroy;
			typedef _session_::_udp_::Stream_HClose									Stream_HClose;
			typedef _session_::_udp_::Stream_HExcept								Stream_HExcept;
			typedef _session_::_udp_::HSendComplete									HSendComplete;
			typedef _session_::_udp_::Stream_HRecv									Stream_HRecv;

			typedef	ThreadWrap	Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;
			typedef LinkList_type<StreamBuf_ptr>		LList;
			typedef Container_HashMap_type<_HASH, StreamSession_sptr>				HashMap_type;

			static	void	MessageThread( void* pParamter );
			static	void	WorkThread( void* pParamter );
		public:
			/*!
			* @function   AppMain
			* @brief    
			* @return  
			*/
			AppMain( void );

			/*!
			* @function   ~AppMain
			* @brief    
			* @return  
			*/
			~AppMain( void );

			int Init( UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0, bool isSequence = false );

			void Release( void );

			Int32	Send( NETHANDLE Node, sockaddr_in* pDest, UInt32 MediaType,
				const char* c_pData, UInt16 u16Size );

			Int32	Send( NETHANDLE Node, sockaddr_in* pDest,
				const char* c_pData, UInt16 u16Size );

			void	Stop( void );
			int		Close( NETHANDLE Node );

			NETHANDLE	Open( UInt16 u16Port );

			int	SetCloseHandle( NETHANDLE Node, const Stream_HClose& Handle )
			{
				StreamSession_sptr ptr = FindSession(Node);
				if( !ptr )
				{
					return -1;
				}

				ptr->SetCloseHandle(Handle);
				return 1;
			}

			int	SetDestroyHandle( NETHANDLE Node, const Stream_HDestroy& hDestroy )
			{
				StreamSession_sptr ptr = FindSession(Node);
				if( !ptr )
				{
					return -1;
				}

				ptr->SetDestroyHandle( function20_bind(&AppMain::HandleDestroy, this,
					hDestroy, Node) );
				return 1;
			}

			int	SetExceptHandle( NETHANDLE Node, const Stream_HExcept& Handle )
			{
				StreamSession_sptr ptr = FindSession(Node);
				if( !ptr )
				{
					return -1;
				}

				ptr->SetExceptHandle(Handle);
				return 1;
			}

			int	SetRecvHandle( NETHANDLE Node, const Stream_HRecv& Handle )
			{
				StreamSession_sptr ptr = FindSession(Node);
				if( !ptr )
				{
					return -1;
				}

				ptr->SetRecvHandle(Handle);
				return 1;
			}

			int	SetSendCompleteNotify( NETHANDLE Node, const HSendComplete& Handle )
			{
				StreamSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetSendCompleteNotify(Handle);
				return 1;
			}

			const _scoket_info&	GetSocketInfo( NETHANDLE Node )
			{
				StreamSession_sptr sptr = FindSession(Node);
				if( sptr )
					return sptr->GetStream()->GetSocketInfo();
				return _scoket_info();
			}

			UInt32	GetCount( void ) const
			{
				return _Count;
			}

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(StreamSession)
			EVENT_REGISTER_READ(&AppMain::HandleRecv)
			EVENT_REGISTER_CLOSE(&AppMain::HandleClose)
			EVENT_REGISTER_EXCEPT(&AppMain::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

		protected:
			void	Run( UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0, bool isSequence = false );
			void	RunLoop( UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0, bool isSequence = false );

			void	CloseAll( void );
			void	CloseSession( StreamSession_sptr ptr );
			const StreamSession_sptr	FindSession( NETHANDLE Node );
			void	FreeSession( void* pSession );

			void	HandleRecv( StreamSession* pSession, sockaddr_in* pDest,
				const StreamBuf_ptr& Buf_ptr );

			void	HandleClose( StreamSession* pSession );
			void	HandleDestroy( const Stream_HDestroy& hDestroy, NETHANDLE Node );
			void	HandleExcept( StreamSession* pSession, const char* strError );

			void SetStreamIdentity( const StreamBuf_ptr& Buf_ptr, StreamSession* pSession )
			{
				((tagStreamIdentity*)(Buf_ptr->data + Buf_ptr->payload))->_Identity = pSession;
			}

			StreamSession* GetStreamIdentity( const StreamBuf_ptr& Buf_ptr ) const
			{
				return ((tagStreamIdentity*)(Buf_ptr->data + Buf_ptr->payload))->_Identity;
			}

			void SetStreamSrcAddr( const StreamBuf_ptr& Buf_ptr, struct sockaddr_in* pSrc )
			{				
				memcpy((void*)(Buf_ptr->data + Buf_ptr->payload + sizeof(tagStreamIdentity)), (void*)pSrc, sizeof(struct sockaddr_in));
			}

			struct sockaddr_in* GetStreamSrcAddr( const StreamBuf_ptr& Buf_ptr ) const
			{	
				return (struct sockaddr_in*)(Buf_ptr->data + Buf_ptr->payload + sizeof(tagStreamIdentity));
			}

		private:
			LinkList_type<StreamSession_sptr,CNoneLock>		_QuitQueue;
			LList			_MQueue;
			long			_Count;
			HashMap_type	_Map;
			ThreadList		_Workers;
			ThreadList		_IOProcs;
			ThreadWrap		_Message;
			MemPool_type	_Pool;
			CLock			_ReadLock;
			CLock			_QuitLock;
			CLock			_Lock;
			bool			_isSequence;
			io_service		_ioService;
		};
		/** @} end AppMain */

	} // namespace	_udp_

} //

inline _server_::_udp_::AppMain* GetUDPStreamInstance( void )
{
	static _server_::_udp_::AppMain* pServer = Singleton<_server_::_udp_::AppMain>::instance();
	return pServer;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
