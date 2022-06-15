
#include <libNetFrame_AsyncStreamClients/StreamClients.h>

namespace	_client_{

	namespace	_async_{

		StreamClients::StreamClients( void )
			: _Pool( sizeof(StreamSession) )
			, _Message(function20_bind(&StreamClients::MessageThread,
									reinterpret_cast<void*>(this)))
			, _Count( 0 )
			, _isSequence( false )
		{
			
		}

		StreamClients::~StreamClients( void )
		{
			Release();
		}

		void	StreamClients::Release( void )
		{
			if( _AsynConnector.is_open() )
			{
				CloseAll();
				_AsynConnector.Stop();

				for(std::size_t i = 0; i < _Workers.size(); ++ i)
				{
					_Workers[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
					_Workers[i]->join();
				}
				_Workers.clear();

				_Message.interrupt(ThreadData::en_INTERRUPTED_EXIT);
				_Message.join();
				_QuitQueue.clear();
			}
		}

		void	StreamClients::Init( UInt8 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence )
		{
			_AsynConnector.Listen();
			_AsynConnector.Run(uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum);
			uProcThreadNum = (uProcThreadNum == 0 ? 1 : uProcThreadNum);
			if( _Workers.size() <= 0 )
			{
				_isSequence = isSequence;
				for(std::size_t i = 0; i < uProcThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type( function20_bind(&StreamClients::WorkThread, this) ) );
					_Workers.push_back(thread);
					thread->wait_run();
				}
			}
		}

		void	StreamClients::WorkThread( void* pParamter )
		{
			StreamClients* Server = reinterpret_cast<StreamClients*>(pParamter);
			StreamSession*	pSession = NULL;
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();
					if( Server->_MQueue.size() > 0 )
					{
						Server->_ReadLock.Lock();
						if( Server->_MQueue.size() > 0 )
						{
							StreamBuf_ptr ptr = Server->_MQueue.front();
							Server->_MQueue.pop();
							pSession = Server->GetStreamIdentity(ptr);
							if( Server->_isSequence )
							{
								pSession->Lock();
							}
							Server->_ReadLock.UnLock();	

							//增加结束符，防止打印字符串出现乱码
							ptr->data[ptr->payload] = 0;
							pSession->RecvNotify(pSession->GetLocalNode(), ptr, pSession->GetUseRef() - 1);
							if( Server->_isSequence )
							{
								pSession->UnLock();
							}

							ptr.reset();
							pSession->SubUseRef();
						}
						else
						{
							Server->_ReadLock.UnLock();	
						}
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
			Server->_MQueue.clear();
		}

		void	StreamClients::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		int StreamClients::Connect( const char* c_szIP, UInt16 uPort )
		{
			if( !_AsynConnector.is_open() ) 
				return -1;

			return _AsynConnector.Connect(c_szIP, uPort);
		}

		int StreamClients::Connect( const char* c_szIP, UInt16 uPort,
					const Stream_HConnect& hConnect )
		{
			if( !_AsynConnector.is_open() )
				return -1;

			return _AsynConnector.Connect(c_szIP, uPort,
				function20_bind(&StreamClients::HandleConnect, this,
				hConnect, _foundation_::_1) );
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

		void	StreamClients::HandleConnect( const Stream_HConnect& hConnect,
										const _SOCKET_::HSOCKET& Socket )
		{
			if( Socket != INVALID_SOCKET )
			{
				StreamSession* Session = _Pool.AllocObj<StreamSession>( sizeof(tagStreamIdentity) );
				if( Session->Init((io_service&)_AsynConnector.GetIoServer(), Socket) == -1 )
				{//SOCKET已经无效
					int iError = error_code::GetLastError();
					printf("StreamClients::Connect is Error:%d\r\n",iError);
					_Pool.FreeObj(Session);
					APIWSAClose(Socket);
					goto gt_error ;
				}

				NETHANDLE Node = Session->GetLocalNode();

				if( FindSession(Node) )
				{
					printf("StreamClients::Connect Node is exsit\r\n");
					_Pool.FreeObj(Session);
					goto gt_error;
				}

				_Lock.Lock();
				StreamSession_sptr sptr(Session, 
					function20_bind_Ex(&StreamClients::FreeSession, this));

				_Map.insert( std::make_pair(Node,sptr) );
				_Lock.UnLock();

				CASAddAndFetch(&_Count);

				if( hConnect )
				{
					hConnect(Node);
				}

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

		void	StreamClients::HandleRecv( StreamSession* pSession,
			const StreamBuf_ptr& Buf_ptr )
		{
			/*Buf_ptr->data[Buf_ptr->payload] = 0;
			pSession->RecvNotify(pSession->GetLocalNode(), Buf_ptr);*/

			SetStreamIdentity(Buf_ptr, pSession);

			pSession->AddUseRef();
			//_ReadLock.Lock();
			_MQueue.push_back(Buf_ptr);
			//_ReadLock.UnLock();
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
							StreamSession_sptr sptr = Clients->_QuitQueue.front();
							if( sptr->isDestory() )
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

	} //namespace	_async_

} //namespace _client_

