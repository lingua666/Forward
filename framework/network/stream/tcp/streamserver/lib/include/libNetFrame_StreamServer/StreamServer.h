
#ifndef __MYSTREAMSERVER_20150924143440_2443076480_H__
#define __MYSTREAMSERVER_20150924143440_2443076480_H__

#include <libNetFrame_StreamServer/External.h>

namespace	_server_{
	
	namespace	_tcp_{

		using namespace _session_::_tcp_;
		using namespace _server_::_tcp_;

		typedef NETHANDLE						_HASH;
		typedef _io_net_::_tcp_::NetAcceptor	NetAcceptor;

		/*!
		* @class  StreamServer
		* @brief   StreamServer
		* @{
		*/

		class StreamServer
		{
		public:
			typedef	function20_handle<void (const char*, UInt16, NETHANDLE)>	Stream_HAccept;
			typedef	Stream_HClose												Stream_HDestroy;

			typedef	ThreadWrap	Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;
			typedef LinkList_type<StreamBuf_ptr>		LList;
			typedef Container_HashMap_type<_HASH, StreamSession_sptr>				HashMap_type;

			static	void	MessageThread( void* pParamter );
			static	void	WorkThread( void* pParamter );
		public:
			/*!
			* @function   StreamServer
			* @brief    
			* @return  
			*/
			StreamServer( void );

			/*!
			* @function   ~StreamServer
			* @brief    
			* @return  
			*/
			~StreamServer( void );

			void Init(UInt8 uWorkerThreads = 0);

			void Release( void );

			Int32	Send( NETHANDLE Node, UInt32 MediaType,
						const char* c_pData, UInt16 u16Size );

			Int32	Send( NETHANDLE Node, const char* c_pData, UInt16 u16Size );

			void	Stop( void );
			void	Run( UInt8 uIOThreadNum = 0,  bool isSequence = false );
			void	RunLoop( UInt8 uIOThreadNum = 0, bool isSequence = false );
			int	Close( NETHANDLE Node );

			template<typename H>
			void	Post( H handle )
			{
				_Accept.Post(handle);
			}

			bool	Listen( UInt16 u16Port, const Stream_HAccept& Handle )
			{
				try
				{
					if( _Accept.Listen(u16Port) )
					{
						EVENT_REGISTER(&_Accept, this);
						_AcceptHandle = Handle;
						return true;
					}
				}
				catch (std::exception& e)
				{
					printf("%s\r\n", e.what());
					//std::cerr << e.what() << std::endl;
				}
			
				return false;
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

				ptr->SetDestroyHandle( function20_bind(&StreamServer::HandleDestroy, this,
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

			bool is_open( void ) const
			{
				return _Accept.is_open();
			}

			UInt32	GetCount( void ) const
			{
				return _Count;
			}

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(NetAcceptor)
				EVENT_REGISTER_ACCEPT(&StreamServer::HandleAccept)
			EVENT_REGISTER_LIST_END()

			EVENT_REGISTER_LIST_BEGIN(StreamSession)
				EVENT_REGISTER_READ(&StreamServer::HandleRecv)
				EVENT_REGISTER_CLOSE(&StreamServer::HandleClose)
				EVENT_REGISTER_EXCEPT(&StreamServer::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

		protected:
			void	CloseAll( void );
			void	CloseSession( StreamSession_sptr ptr );
			const StreamSession_sptr	FindSession( NETHANDLE Node );
			void	FreeSession( void* pSession );

			void	HandleAccept( const _SOCKET_::HSOCKET& Socket );
			void	HandleRecv( StreamSession* pSession, const StreamBuf_ptr& Buf_ptr );
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
			LinkList_type<StreamSession_sptr,CNoneLock>		_QuitQueue;
			LList			_MQueue;
			long			_Count;
			HashMap_type	_Map;
			ThreadList		_Workers;
			ThreadWrap		_Message;
			MemPool_type	_Pool;
			NetAcceptor		_Accept;
			CLock			_ReadLock;
			CLock			_QuitLock;
			CLock			_Lock;
			Stream_HAccept	_AcceptHandle;
			bool			_isSequence;
		};
		/** @} end StreamServer */

	}	//namespace	_tcp_

} // _server_

inline _server_::_tcp_::StreamServer* GetStreamSvrInstance( void )
{
	static _server_::_tcp_::StreamServer* pServer = Singleton<_server_::_tcp_::StreamServer>::instance();
	return pServer;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
