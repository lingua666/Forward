
#include <libNetFrame_SyncStreamClients/StreamClients.h>

namespace	_client_{

	namespace	_sync_{

		StreamClients::StreamClients( void )
			: _Pool( sizeof(StreamSession) )
			, _Message(function20_bind(&StreamClients::MessageThread,
			reinterpret_cast<void*>(this)))
			, _Count( 0 )
		{
			Init(get_processor_number() * 2 + 2);
		}

		StreamClients::~StreamClients( void )
		{
			Release();
		}

		void	StreamClients::Release( void )
		{
			CloseAll();

			_Message.interrupt(ThreadData::en_INTERRUPTED_EXIT);
			_Message.join();
			_QuitQueue.clear();
		}

		void	StreamClients::Init( UInt8 uThreadNum )
		{
			NetConnector::Init(uThreadNum);
		}

		void	StreamClients::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		NETHANDLE StreamClients::Connect( const char* c_szIP, UInt16 uPort )
		{
			_SOCKET_::HSOCKET Socket = NetConnector::Connect(c_szIP, uPort);
			if( Socket == INVALID_SOCKET )
				return -1;

			StreamSession* Session = _Pool.AllocObj<StreamSession>(0);
			if( Session->Init(NetConnector::GetIoServer(), Socket) == -1 )
			{//SOCKET已经无效
				int iError = error_code::GetLastError();
				printf("CommandClients::HandleConnect is Error:%d\r\n",iError);
				_Pool.FreeObj(Session);
				APIWSAClose(Socket);
				return -1;
			}

			NETHANDLE Node = Session->GetLocalNode();

			if( FindSession(Node) )
			{
				printf("CommandClients::HandleAccept Node is exsit\r\n");
				_Pool.FreeObj(Session);
				return -1;
			}

			_Lock.Lock();
			_Map.insert(std::make_pair(Node,
				StreamSession_sptr(Session, 
				function20_bind_Ex(&StreamClients::FreeSession, this))) );
			_Lock.UnLock();

			CASAddAndFetch(&_Count);
			EVENT_REGISTER(Session, this); // 所有动作执行完毕后才开始注册事件，防止在事件注册后链接突然断开，引起后续动作无法正常完成，程序出现异常

			if( 1 != Session->Run() )
			{
				Session->Close();
				return -1; 
			}

			return Node;
		}

		int StreamClients::Close( NETHANDLE Node )
		{
			_Lock.Lock();
			HashMap_type::iterator iter = _Map.find(Node);
			if( iter != _Map.end() )
			{
				StreamSession_sptr sptr = iter->second;
				_Map.erase(iter);
				_Lock.UnLock();
				CloseSession(sptr); //关闭会话
				return 1;
			}
			else
				_Lock.UnLock();

			return -1;
		}

		void	StreamClients::CloseSession( const StreamSession_sptr& ptr )
		{
			ptr->Close();	//关闭连接
			_QuitLock.Lock();
			_QuitQueue.push_back(ptr); //压入退出队列
			_QuitLock.UnLock();
		}

		const StreamSession_sptr	StreamClients::FindSession( NETHANDLE Node )
		{
			_Lock.Lock();
			HashMap_type::iterator iter = _Map.find(Node);
			if( iter != _Map.end() )
			{
				StreamSession_sptr sptr = iter->second;
				_Lock.UnLock();
				return sptr;
			}
			_Lock.UnLock();
			return StreamSession_sptr();
		}

		void	StreamClients::FreeSession( void* pSession )
		{
			_Pool.FreeObj(reinterpret_cast<StreamSession*>(pSession));
		}

		Int32	StreamClients::Send( NETHANDLE Node, const char* c_pData, UInt16 uSize )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(c_pData, uSize);
			return -1;
		}

		void	StreamClients::HandleRecv( StreamSession* pSession,
			const StreamBuf_ptr& Buf_ptr )
		{
			StreamBuf::GetData(Buf_ptr.get())[StreamBuf::GetSize(Buf_ptr.get())] = 0;
			pSession->RecvNotify(pSession->GetLocalNode(), Buf_ptr, pSession->GetUseRef() - 1);
		}

		void	StreamClients::HandleClose( StreamSession* pSession )
		{
			_string_type  sIP = pSession->GetStream()->GetSocketInfo().local_ip;
			pSession->CloseNotify(pSession->GetLocalNode(), sIP.c_str(),
				pSession->GetStream()->GetSocketInfo().local_port);
			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	StreamClients::HandleDestroy( const Stream_HDestroy& hDestroy, NETHANDLE Node )
		{
			if( hDestroy )
				hDestroy(Node);
		}

		void	StreamClients::HandleExcept( StreamSession* pSession, const char* strError )
		{
			_string_type  sIP = pSession->GetStream()->GetSocketInfo().local_ip;
			pSession->CloseNotify(pSession->GetLocalNode(), 
				sIP.c_str(), pSession->GetStream()->GetSocketInfo().local_port);
			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	StreamClients::MessageThread( void* pParamter )
		{
			StreamClients* Clients = reinterpret_cast<StreamClients*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();

					//扫描退出队列
					if( Clients->_QuitQueue.size() > 0 )
					{
						Clients->_QuitLock.Lock();
						if( Clients->_QuitQueue.size() > 0 )
						{
							if( Clients->_QuitQueue.front()->isDestory() )
							{
								Clients->_QuitQueue.pop();
								Clients->_QuitLock.UnLock();
								CASSubAndFetch(&Clients->_Count);
							}
							else
							{
								Clients->_QuitLock.UnLock();
							}
						}
						else
						{
							Clients->_QuitLock.UnLock();
						}
					}
					else
					{
						Sleep(1);
					}
				}
				catch (const thread_interrupted& e)
				{
					switch( e.get_code() )
					{
					case ThreadData::en_INTERRUPTED_EXIT:	//线程退出
						goto exit;
						break;
					}
					break;
				}
			}
exit:
			return ;
		}

	} //namespace	_sync_

} //namespace _client_

