
#ifndef __MYSTREAMSERVER_V20_20150924143440_2443076480_H__
#define __MYSTREAMSERVER_V20_20150924143440_2443076480_H__

#include <libNetFrame_StreamServer_V20/External.h>

namespace	_server_{
	
	namespace	_tcp_{

		using namespace _session_::_tcp_;
		using namespace _server_::_tcp_;

		typedef NETHANDLE							_HASH;
		typedef _io_net_::_tcp_::NetAcceptor_V20	NetAcceptor_V20;

		/*!
		* @class  StreamServer_V20
		* @brief   StreamServer_V20
		* @{
		*/

		class StreamServer_V20
		{
		public:
			typedef struct tagListen_INFO
			{
				socket_type::HSOCKET Sock;
				NETHANDLE			hNet;
				CLock				Lock;
				Container_HashMap_type<NETHANDLE, socket_type::HSOCKET> Sessions;
			};

			typedef	function20_handle<void (NETHANDLE, const char*, UInt16, NETHANDLE)>	Stream_HAccept;
			typedef	Stream_HClose												Stream_HDestroy;

			typedef	ThreadWrap	Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;
			typedef LinkList_type<StreamBuf_ptr>		LList;
			typedef Container_HashMap_type<_HASH, StreamSession_sptr>				HashMap_type;

			typedef SmartPTR<tagListen_INFO>	tagListen_INFO_sptr;
			static tagListen_INFO_sptr Alloc(void)
			{
				static MemPool_type s_Pool(sizeof(tagListen_INFO));
				return tagListen_INFO_sptr(s_Pool.AllocObj<tagListen_INFO>(),
					function20_bind_Ex(&MemPool_type::FreeObj<tagListen_INFO>, &s_Pool));
			}

			typedef Container_HashMap_type<NETHANDLE, tagListen_INFO_sptr>			ListenMap_type;

			struct __M_ALIGNED_PACKED__ tagStreamIdentity
			{
				StreamSession*	_Identity;
			};

			static	void	MessageThread( void* pParamter );

			static	void	WorkThread( void* pParamter );

		public:
			/*!
			* @function   StreamServer_V20
			* @brief    
			* @return  
			*/
			StreamServer_V20( void );

			/*!
			* @function   ~StreamServer_V20
			* @brief    
			* @return  
			*/
			~StreamServer_V20( void );

			int Init(UInt8 uIOThreadNum = 0, UInt8 uWorkerThreads = 0, bool isSequence = false);

			void Release( void );

			void StopAll(void);

			Int32	Send( NETHANDLE Node, UInt32 MediaType,
						const char* c_pData, UInt32 uSize );

			Int32	Send( NETHANDLE Node, const char* c_pData, UInt32 uSize );

			int	Stop(NETHANDLE hNet);

			int	Close(NETHANDLE hNet);

			int AddSessionToListen(NETHANDLE hListNet, NETHANDLE hNet, const socket_type::HSOCKET& hSocket);

			int RemoveSessionToListen(NETHANDLE hListNet, NETHANDLE hNet);

			//单位毫秒
			void	SetSleepStep(UInt32 uWorkMS, UInt32 uDestroyMS);

			NETHANDLE Listen( UInt16 u16Port, const Stream_HAccept& Handle, const char* c_szIP = NULL)
			{
				try
				{
					socket_type::HSOCKET Sock = _Accept.Open(u16Port, c_szIP);
					if(Sock != INVALID_SOCKET)
					{
						_string_type sIP = GetHostIP();
						NETHANDLE hNet = GetEndPointFromSIPv4(sIP.c_str(), u16Port);

						_Accept.SetAcceptHandle(function20_bind(&StreamServer_V20::HandleAccept, this,
							hNet, _function_::_1));
						_AcceptHandle = Handle;

						tagListen_INFO_sptr sptr = StreamServer_V20::Alloc();
						sptr->hNet = hNet;
						sptr->Sock = Sock;
						_ListenLock.Lock();
						_ListenMap.insert(std::make_pair(hNet, sptr));
						_ListenLock.UnLock();
						return hNet;
					}
				}
				catch (std::exception& e)
				{
					printf("%s\r\n", e.what());
					//std::cerr << e.what() << std::endl;
				}
			
				return -1;
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

				ptr->SetDestroyHandle( function20_bind(&StreamServer_V20::HandleDestroy, this,
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

			UInt32	GetSessionCount(NETHANDLE hListenNet);

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(StreamSession)
				EVENT_REGISTER_READ(&StreamServer_V20::HandleRecv)
				EVENT_REGISTER_CLOSE(&StreamServer_V20::HandleClose)
				EVENT_REGISTER_EXCEPT(&StreamServer_V20::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

		protected:
			void	CloseAll( void );
			void	CloseSession( StreamSession_sptr ptr );
			const StreamSession_sptr	FindSession( NETHANDLE Node );
			void	FreeSession(NETHANDLE hListenNet, NETHANDLE hNet, void* pSession);

			void	HandleAccept(NETHANDLE hListenNet, const _SOCKET_::HSOCKET& Socket );
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
			ListenMap_type	_ListenMap;
			ThreadList		_Workers;
			ThreadWrap		_Message;
			MemPool_type	_Pool;
			NetAcceptor_V20	_Accept;
			CLock			_ReadLock;
			CLock			_QuitLock;
			CLock			_Lock;
			CLock			_ListenLock;
			Stream_HAccept	_AcceptHandle;
			bool			_isSequence;
			UInt32 			_uWorkMS;
			UInt32 			_uDestroyMS;
		};
		/** @} end StreamServer_V20 */

	}	//namespace	_tcp_

} // _server_

inline _server_::_tcp_::StreamServer_V20* GetStreamSvrInstance_V20( void )
{
	static _server_::_tcp_::StreamServer_V20* pServer = Singleton<_server_::_tcp_::StreamServer_V20>::instance();
	return pServer;
}

#endif//__MYFILECLIENT_20150924143440_1443076480_H__
