
#ifndef __MYSTREAMCLIENT_20150924143440_1443076480_H__
#define __MYSTREAMCLIENT_20150924143440_1443076480_H__

#include <libNetFrame_AsyncStreamClients/External.h>

namespace	_client_{

	namespace	_async_{

		using namespace _session_::_tcp_;

		typedef NETHANDLE							_HASH;
		typedef _io_net_::_tcp_::NetAsyncConnector	NetAsyncConnector;

		/*!
		* @class  StreamClients
		* @brief   StreamClients
		* @{
		*/

		class StreamClients
		{
		public:
			typedef	function20_handle<void (NETHANDLE)>			Stream_HConnect;
			typedef	Stream_HClose								Stream_HDestroy;

			typedef	ThreadWrap	Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;

			typedef LinkList_type<StreamBuf_ptr>		LList;
			typedef Container_HashMap_type<_HASH, StreamSession_sptr>		HashMap_type;
			static	void	MessageThread( void* pParamter );
			static	void	WorkThread( void* pParamter );
		public:
			/*!
			* @function   MyMeidaClient
			* @brief    
			* @return  
			*/
			StreamClients( void );

			/*!
			* @function   ~MyMeidaClient
			* @brief    
			* @return  
			*/
			~StreamClients( void );

			void Release( void );

			void Init( UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0, bool isSequence = false );

			int Connect( const char* c_szIP, UInt16 uPort );

			int Connect( const char* c_szIP, UInt16 uPort,
					const Stream_HConnect& hConnect );

			int Close( NETHANDLE Node );
			
			Int32	Send( NETHANDLE Node, const char* c_pData, UInt16 u32Size );

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(StreamSession)
				EVENT_REGISTER_READ(&StreamClients::HandleRecv)
				EVENT_REGISTER_CLOSE(&StreamClients::HandleClose)
				EVENT_REGISTER_EXCEPT(&StreamClients::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

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

				ptr->SetDestroyHandle( function20_bind(&StreamClients::HandleDestroy, this,
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

			bool is_open( void )
			{
				return _AsynConnector.is_open();
			}

		protected:
			void  CloseAll( void );
			const StreamSession_sptr	FindSession( NETHANDLE Node );
			void	FreeSession( void* pSession );
			void	CloseSession( const StreamSession_sptr& ptr );

			void	HandleConnect( const Stream_HConnect& hConnect,
				const _SOCKET_::HSOCKET& Socket );

			void	HandleRecv( StreamSession* pSession,
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

		private:
			LinkList_type<StreamSession_sptr,CNoneLock>	_QuitQueue;
			LList			_MQueue;
			NetAsyncConnector	_AsynConnector;
			long			_Count;
			HashMap_type	_Map;
			ThreadList		_Workers;
			ThreadWrap		_Message;
			MemPool_type	_Pool;
			CLock			_ReadLock;
			CLock			_QuitLock;
			CLock			_Lock;
			bool			_isSequence;
		};
		/** @} end MyMeidaClient */

	} // namespace	_async_

} // namespace client_

inline _client_::_async_::StreamClients* GetAsynStreamClientsInstance( void )
{
	static _client_::_async_::StreamClients* pClients = Singleton<_client_::_async_::StreamClients>::instance();
	return pClients;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
