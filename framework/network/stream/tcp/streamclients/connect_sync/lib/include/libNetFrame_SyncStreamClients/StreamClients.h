
#ifndef __MYMEDIACLIENT_20150924143440_1443076480_H__
#define __MYMEDIACLIENT_20150924143440_1443076480_H__

#include <libNetFrame_SyncStreamClients/External.h>

namespace	_client_{

	namespace	_sync_{

		using namespace _session_::_tcp_;

		typedef NETHANDLE						_HASH;
		typedef _io_net_::_tcp_::NetConnector	NetConnector;

		typedef	Stream_HClose	Media_HClose;

		/*!
		* @class  MyMeidaClient
		* @brief   MyMeidaClient
		* @{
		*/

		class StreamClients
		{
		public:
			typedef	Stream_HClose						Stream_HDestroy;

			typedef Container_HashMap_type<_HASH, StreamSession_sptr>	HashMap_type;
			static	void	MessageThread( void* pParamter );
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

			void Init( UInt8 uThreadNum = 1 );

			NETHANDLE Connect( const char* c_szIP, UInt16 uPort );

			int Close( NETHANDLE Node );

			Int32	Send( NETHANDLE Node, const char* c_pData, UInt32 uSize );

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(StreamSession)
				EVENT_REGISTER_READ(&StreamClients::HandleRecv)
				EVENT_REGISTER_CLOSE(&StreamClients::HandleClose)
				EVENT_REGISTER_EXCEPT(&StreamClients::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			template<typename H>
			void Post( H handle )
			{
				NetConnector::Post(handle);
			}

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

		protected:
			void  CloseAll( void );
			const StreamSession_sptr	FindSession( NETHANDLE Node );
			void	FreeSession( void* pSession );
			void	CloseSession( const StreamSession_sptr& ptr );

			void	HandleRecv( StreamSession* pSession,
				const StreamBuf_ptr& Buf_ptr );
			void	HandleClose( StreamSession* pSession );
			void	HandleDestroy( const Stream_HDestroy& hDestroy, NETHANDLE Node );
			void	HandleExcept( StreamSession* pSession, const char* strError );

		private:
			LinkList_type<StreamSession_sptr,CNoneLock>				_QuitQueue;
			long			_Count;
			HashMap_type	_Map;
			CLock			_QuitLock;
			ThreadWrap		_Message;
			MemPool_type	_Pool;
			CLock			_Lock;
		};
		/** @} end MyMeidaClient */

	} //namespace	_sync_

} //namespace _client_

typedef	_client_::_sync_::StreamClients	syncStreamClients_type;

inline syncStreamClients_type* GetSyncStreamClientsInstance( void )
{
	static syncStreamClients_type* pClients = Singleton<syncStreamClients_type>::instance();
	return pClients;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
