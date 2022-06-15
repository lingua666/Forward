
#include <libNetFrame_SyncCmdClients/CommandClients.h>
#include <libNetFrame_SyncCmdClients/Heart.h>

namespace	_client_{

	namespace _sync_{

		CommandClients::CommandClients( void )
			: _Pool( sizeof(CmdSession) )
			, _Worker(function20_bind(&CommandClients::WorkThread, reinterpret_cast<void*>(this)))
			, _Message(function20_bind(&CommandClients::MessageThread, reinterpret_cast<void*>(this)))
			, _Count( 0 )
		{

		}

		CommandClients::~CommandClients( void )
		{
			Release();
		}

		void CommandClients::Release( void )
		{
			_Worker.interrupt(ThreadData::en_INTERRUPTED_EXIT);
			_Worker.join();
			_Message.interrupt(ThreadData::en_INTERRUPTED_EXIT);
			_Message.join();
			_QuitQueue.clear();
			_MQueue.clear();
			//Heart::Release();
			CloseAll();
			Heart::Clear();
			_Count = 0;
			//_Pool.Release();
		}

		void	CommandClients::Init( UInt8 uThreadNum )
		{
			NetConnector::Init(uThreadNum);
		}

		void	CommandClients::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		NETHANDLE CommandClients::Connect( const char* c_szIP, UInt16 uPort )
		{
			_SOCKET_::HSOCKET Socket = NetConnector::Connect(c_szIP, uPort);
			if( Socket == INVALID_SOCKET )
				return -1;

			CmdSession* Session = _Pool.AllocObj<CmdSession>( sizeof(tagCmdIdentity) );
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
			CmdSession_sptr sptr(Session, 
				function20_bind_Ex(&CommandClients::FreeSession, this));

			_Map.insert(std::make_pair(Node,sptr));
			_Lock.UnLock();

			CASAddAndFetch(&_Count);

			Heart::Attach(Node, sptr);
			EVENT_REGISTER(Session, this); // 所有动作执行完毕后才开始注册事件，防止在事件注册后链接突然断开，引起后续动作无法正常完成，程序出现异常

			if( 1 != Session->Run() )
			{
				Session->Close();
				return -1; 
			}

			return Node;
		}

		int CommandClients::Close( NETHANDLE Node )
		{
			_Lock.Lock();
			HashMap_type::iterator iter = _Map.find(Node);
			if( iter != _Map.end() )
			{
				CmdSession_sptr sptr = iter->second;
				_Map.erase(iter);
				_Lock.UnLock();
				Heart::Stop(Node); //从列表中清除会话心跳信息
				CloseSession(sptr); //关闭会话
				return 1;
			}
			else
				_Lock.UnLock();

			return -1;
		}

		void	CommandClients::CloseSession( const CmdSession_sptr& ptr )
		{
			ptr->Close();	//关闭连接
			_QuitLock.Lock();
			_QuitQueue.push_back(ptr); //压入退出队列
			_QuitLock.UnLock();
		}

		const CmdSession_sptr	CommandClients::FindSession( NETHANDLE Node )
		{
			_Lock.Lock();
			HashMap_type::iterator iter = _Map.find(Node);
			if( iter != _Map.end() )
			{
				CmdSession_sptr sptr = iter->second;
				_Lock.UnLock();
				return sptr;
			}
			_Lock.UnLock();
			return CmdSession_sptr();
		}

		void	CommandClients::FreeSession( void* pSession )
		{
			_Pool.FreeObj(reinterpret_cast<CmdSession*>(pSession));
		}

		int	CommandClients::SendError( NETHANDLE Node, const char* c_pData, UInt32 uSize )
		{
			CmdSession_sptr sptr = FindSession(Node);
			if( sptr )
			{
				_session_hdr Hdr;
				Hdr._uPriority = 4;
				Hdr._uType = 0;
				return sptr->Send(&Hdr, c_pData, uSize);
			}

			return -1;
		}

		int	CommandClients::Send( NETHANDLE Node, const char* c_pData, UInt32 uSize )
		{
			CmdSession_sptr sptr = FindSession(Node);
			if( sptr )
			{
				_session_hdr Hdr = {0};
				return sptr->Send(&Hdr, c_pData, uSize);
			}
			return -1;
		}

		void	CommandClients::HandleRecv( CmdSession* pSession, const CmdDBuf_ptr& Buf_ptr )
		{
			_session_hdr* p = reinterpret_cast<_session_hdr*>(Buf_ptr->_Data);
			if( p->_uType == SESSION_HEART_TYPE_DEF)
			{//是否心跳
				Heart::Update(pSession->GetLocalNode());
			}
			else
			{
				SetCmdIdentity(Buf_ptr, pSession);
				pSession->AddUseRef();
				_ReadLock.Lock();
				_MQueue.push_back(Buf_ptr, p->_uPriority);
				_ReadLock.UnLock();
			}
		}

		void	CommandClients::HandleClose( CmdSession* pSession )
		{
			_string_type  sIP = pSession->GetStream()->GetSocketInfo().local_ip;
			pSession->CloseNotify(pSession->GetLocalNode(), sIP.c_str(), pSession->GetStream()->GetSocketInfo().local_port);
			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	CommandClients::HandleDestroy( const Comamand_HDestroy& hDestroy, NETHANDLE Node )
		{
			if( hDestroy )
				hDestroy(Node);
		}

		void	CommandClients::HandleExcept( CmdSession* pSession, const char* strError )
		{
			_string_type  sIP = pSession->GetStream()->GetSocketInfo().local_ip;
			pSession->CloseNotify(pSession->GetLocalNode(), sIP.c_str(), pSession->GetStream()->GetSocketInfo().local_port);
			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	CommandClients::WorkThread( void* pParamter )
		{
			CommandClients* Clients = reinterpret_cast<CommandClients*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();
					if( Clients->_MQueue.size() > 0 )
					{
						Clients->_ReadLock.Lock();
						if( Clients->_MQueue.size() > 0 )
						{
							CmdDBuf_ptr ptr = Clients->_MQueue.front();
							Clients->_MQueue.pop_front();
							Clients->_ReadLock.UnLock();

							CmdSession*	pSession = Clients->GetCmdIdentity(ptr);

							//增加结束符，防止打印字符串出现乱码
							ptr->_Data[ptr->_uPayload] = 0;
							pSession->RecvNotify(pSession->GetLocalNode(),
								ptr->_Data + sizeof(_session_hdr),
								ptr->_uPayload - sizeof(_session_hdr));

							ptr.reset();

							//更新心跳
							/*if( pSession->is_valid() )
							{
								Heart::Update(pSession->GetLocalNode());
							}*/
							pSession->SubUseRef();
						}
						else
						{
							Clients->_ReadLock.UnLock();
						}
					}
					else
					{//没有数据休眠
#if defined(LOW_POWER_FLAG_DEL)
						Sleep(10);
#else
						Sleep(1);
#endif
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
			//清除队列中数据
			Clients->_MQueue.clear();
		}

		void	CommandClients::MessageThread( void* pParamter )
		{
			CommandClients* Clients = reinterpret_cast<CommandClients*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();
					Heart::CheckHeart();

					//扫描退出队列
					if( Clients->_QuitQueue.size() > 0 )
					{
						Clients->_QuitLock.Lock();
						if( Clients->_QuitQueue.size() > 0 )
						{
							if( Clients->_QuitQueue.front()->isDestory() )
							{
								Heart::Detach(Clients->_QuitQueue.front()->GetLocalNode()); //从列表中清除会话心跳信息
								Clients->_QuitQueue.pop_front();
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
#if defined(LOW_POWER_FLAG_DEL)
						Sleep(1000);
#else
						Sleep(1);
#endif
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

	} //namespace _sync_

} //namespace	_client_
