
#include <libNetFrame_AsyncRawClients/RawClients.h>

namespace	_client_{

	namespace	_async_{

		RawClients::RawClients( void )
			: _Pool( sizeof(RawSession) )
			, _Worker(function20_bind(&RawClients::WorkThread, reinterpret_cast<void*>(this)))
			, _Message(function20_bind(&RawClients::MessageThread, reinterpret_cast<void*>(this)))
			, _Count( 0 )
		{
			
		}

		RawClients::~RawClients( void )
		{
			Release();
		}

		void RawClients::Release( void )
		{
			_Worker.interrupt(ThreadData::en_INTERRUPTED_EXIT);
			_Worker.join();
			_Message.interrupt(ThreadData::en_INTERRUPTED_EXIT);
			_Message.join();
			_QuitQueue.clear();
			_MQueue.clear();
			CloseAll();
			_AsynConnector.Stop();
		}

		void	RawClients::Init( UInt8 uThreadNum )
		{
			_AsynConnector.Listen();
			_AsynConnector.Run(uThreadNum);
		}

		void	RawClients::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		int RawClients::Connect( const char* c_szIP, UInt16 uPort )
		{
			if( !_AsynConnector.is_open() ) 
				return -1;

			return _AsynConnector.Connect(c_szIP, uPort);
		}

		int RawClients::Connect( const char* c_szIP, UInt16 uPort,
							const Raw_HConnect& hConnect )
		{
			if( !_AsynConnector.is_open() )
				return -1;

			return _AsynConnector.Connect(c_szIP, uPort,
				function20_bind(&RawClients::HandleConnect, this,
				hConnect, _foundation_::_1) );
		}

		int RawClients::Close( NETHANDLE Node )
		{
			_Lock.Lock();
			HashMap_type::iterator iter = _Map.find(Node);
			if( iter != _Map.end() )
			{
				RawSession_sptr sptr = iter->second;
				_Map.erase(iter);
				_Lock.UnLock();
				CloseSession(sptr); //关闭会话
				return 1;
			}
			else
				_Lock.UnLock();

			return -1;
		}

		void	RawClients::CloseSession( const RawSession_sptr& ptr )
		{
			ptr->Close();	//关闭连接
			_QuitLock.Lock();
			_QuitQueue.push_back(ptr); //压入退出队列
			_QuitLock.UnLock();
		}

		const RawSession_sptr	RawClients::FindSession( NETHANDLE Node )
		{
			_Lock.Lock();
			HashMap_type::iterator iter = _Map.find(Node);
			if( iter != _Map.end() )
			{
				RawSession_sptr sptr = iter->second;
				_Lock.UnLock();
				return sptr;
			}
			_Lock.UnLock();
			return RawSession_sptr();
		}

		void	RawClients::FreeSession( void* pSession )
		{
			_Pool.FreeObj(reinterpret_cast<RawSession*>(pSession));
		}

		Int32	RawClients::Send( NETHANDLE Node, const char* c_pData, UInt32 u32Size )
		{
			RawSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(c_pData, u32Size);
			return -1;
		}

		void	RawClients::HandleConnect( const Raw_HConnect& hConnect,
						const _SOCKET_::HSOCKET& Socket )
		{
			if( Socket != INVALID_SOCKET )
			{
				RawSession* Session = _Pool.AllocObj<RawSession>( sizeof(tagRawIdentity) );
				if( Session->Init((io_service&)_AsynConnector.GetIoServer(), Socket) == -1 )
				{//SOCKET已经无效
					int iError = error_code::GetLastError();
					printf("RawClients::Connect is Error:%d\r\n",iError);
					_Pool.FreeObj(Session);
					APIWSAClose(Socket);
					goto gt_error ;
				}

				NETHANDLE Node = Session->GetLocalNode();

				if( FindSession(Node) )
				{
					printf("RawClients::Connect Node is exsit\r\n");
					_Pool.FreeObj(Session);
					goto gt_error;
				}

				_Lock.Lock();
				_Map.insert(std::make_pair(Node,
					RawSession_sptr(Session, 
						function20_bind_Ex(&RawClients::FreeSession, this))) );
				_Lock.UnLock();

				if( hConnect )
				{
					hConnect(Node);
				}

				CASAddAndFetch(&_Count);
				EVENT_REGISTER(Session, this); // 所有动作执行完毕后才开始注册事件，防止在事件注册后链接突然断开，引起后续动作无法正常完成，程序出现异常
				
				if( 1 != Session->Run() )
				{
					Session->Close();
					return ; 
				}
			}
			else
			{
gt_error:
				if( hConnect )
				{
					hConnect(-1);
				}
			}
		}

		void	RawClients::HandleRecv( RawSession* pSession, const StreamBuf_ptr& Buf_ptr )
		{
			_buf_hdr*	pHdr = Buf_ptr.get();

			SetCmdIdentity(pHdr, pSession);

			pSession->AddUseRef();
			_ReadLock.Lock();
			_MQueue.push_back(Buf_ptr);
			_ReadLock.UnLock();
		}

		void	RawClients::HandleClose( RawSession* pSession )
		{
			_string_type  sIP = pSession->GetStream()->GetSocketInfo().local_ip;
			pSession->CloseNotify(pSession->GetLocalNode(), sIP.c_str(), pSession->GetStream()->GetSocketInfo().local_port);
			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	RawClients::HandleDestroy( const Raw_HDestroy& hDestroy, NETHANDLE Node )
		{
			if( hDestroy )
				hDestroy(Node);
		}

		void	RawClients::HandleExcept( RawSession* pSession, const char* strError )
		{
			_string_type  sIP = pSession->GetStream()->GetSocketInfo().local_ip;
			pSession->CloseNotify(pSession->GetLocalNode(), sIP.c_str(), pSession->GetStream()->GetSocketInfo().local_port);
			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	RawClients::WorkThread( void* pParamter )
		{
			RawClients* Clients = reinterpret_cast<RawClients*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();
					if( Clients->_MQueue.size() > 0 )
					{
						Clients->_ReadLock.Lock();
						StreamBuf_ptr ptr = Clients->_MQueue.front();
						Clients->_MQueue.pop_front();
						Clients->_ReadLock.UnLock();
						RawSession*	pSession = Clients->GetCmdIdentity(ptr.get());

						//增加结束符，防止打印字符串出现乱码
						ptr->data[ptr->payload] = 0;
						pSession->RecvNotify(pSession->GetLocalNode(),
							StreamBuf::GetData(ptr.get()) ,
							StreamBuf::GetSize(ptr.get()) );

						ptr.reset();
						pSession->SubUseRef();
					}
					else
					{//没有数据休眠
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
			//清除队列中数据
			Clients->_MQueue.clear();
		}

		void	RawClients::MessageThread( void* pParamter )
		{
			RawClients* Clients = reinterpret_cast<RawClients*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();
					//扫描退出队列
					if( Clients->_QuitQueue.size() > 0 )
					{
						Clients->_QuitLock.Lock();
						if( Clients->_QuitQueue.front()->isDestory() )
						{
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

	} //namespace _async_

} //namespace	_client_
