
#include <libNetFrame_StreamServer/StreamServer.h>

#define SOCKET_STREAM_COUNT		10000

namespace	_server_{

	namespace	_tcp_{

		StreamServer::StreamServer( void )
			: _Pool( sizeof(StreamSession) )
			, _Message(function20_bind(&StreamServer::MessageThread,
						reinterpret_cast<void*>(this)))
			, _Count( 0 )
			, _isSequence( false )
		{

		}

		StreamServer::~StreamServer( void )
		{
			Release();
		}

		void StreamServer::Init(UInt8 uWorkerThreads)
		{
			if (_Workers.size() <= 0)
			{
				uWorkerThreads = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
				for (std::size_t i = 0; i < uWorkerThreads; ++i)
				{
					Thread_ptr thread(new Thread_type(function20_bind(&StreamServer::WorkThread, this)));
					_Workers.push_back(thread);
					thread->wait_run();
				}
			}
		}

		void StreamServer::Release( void )
		{
			if( _Accept.is_open() )
			{
				Stop();

				for(std::size_t i = 0; i < _Workers.size(); ++ i)
				{
					_Workers[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
					_Workers[i]->join();
				}
				_Workers.clear();

				_Message.interrupt(ThreadData::en_INTERRUPTED_EXIT);
				_Message.join();
				_QuitQueue.clear();
				_Pool.Release();
			}
		}

		void	StreamServer::Stop( void )
		{
			_Accept.Stop();
			CloseAll();
		}

		void	StreamServer::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		int	StreamServer::Close( NETHANDLE Node )
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

		void	StreamServer::CloseSession( StreamSession_sptr ptr )
		{
			ptr->Close();	//关闭连接
			_QuitLock.Lock();
			_QuitQueue.push_back(ptr); //压入退出队列
			_QuitLock.UnLock();
		}

		const StreamSession_sptr	StreamServer::FindSession( NETHANDLE Node )
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

		void	StreamServer::FreeSession( void* pSession )
		{
			_Pool.FreeObj(reinterpret_cast<StreamSession*>(pSession));
		}

		void	StreamServer::Run( UInt8 uIOThreadNum, bool isSequence )
		{
			_isSequence = isSequence;
			_Accept.Run(uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum);
		}

		void	StreamServer::RunLoop( UInt8 uIOThreadNum, bool isSequence )
		{
			_isSequence = isSequence;
			_Accept.RunLoop(uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum);
		}

		void	StreamServer::WorkThread( void* pParamter )
		{
			StreamServer* Server = reinterpret_cast<StreamServer*>(pParamter);
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
							if( !Server->_isSequence )
							{
								Server->_ReadLock.UnLock();
							}

							//增加结束符，防止打印字符串出现乱码
							ptr->data[ptr->payload] = 0;
							pSession->RecvNotify(pSession->GetRemoteNode(), ptr, pSession->GetUseRef() - 1);
							if( Server->_isSequence )
							{
								Server->_ReadLock.UnLock();
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

		Int32	StreamServer::Send( NETHANDLE Node, UInt32 MediaType,
								const char* c_pData, UInt16 u16Size )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(MediaType, c_pData, u16Size);

			return -1;
		}

		Int32	StreamServer::Send( NETHANDLE Node, const char* c_pData, UInt16 u16Size )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(c_pData, u16Size);
			return -1;
		}

		void	StreamServer::HandleAccept( const _SOCKET_::HSOCKET& Socket )
		{
			if( _Count < SOCKET_STREAM_COUNT )
			{
				StreamSession* Session = _Pool.AllocObj<StreamSession>( sizeof(tagStreamIdentity) );
				if( Session->Init((io_service&)_Accept.GetIoServer(),Socket) != 1 )
				{
					int iError = error_code::GetLastError();
					printf("StreamServer::HandleAccept is Error:%d\r\n",iError);
					_Pool.FreeObj(Session);
					APIWSAClose(Socket);
					return ;
				}

				NETHANDLE	Node = Session->GetRemoteNode();
				if( FindSession(Node) )
				{
					printf("StreamServer::HandleAccept Node is exsit\r\n");
					_Pool.FreeObj(Session);
					return ;
				}

				_Lock.Lock();
				StreamSession_sptr sptr(Session,
					function20_bind_Ex(&StreamServer::FreeSession,this));
				_Map.insert(std::make_pair(Node, sptr));
				_Lock.UnLock();
				CASAddAndFetch(&_Count);

				if( _AcceptHandle )
				{
					_AcceptHandle( Session->GetStream()->GetSocketInfo().remote_ip.c_str(),
						Session->GetStream()->GetSocketInfo().remote_port,
						Node );
				}
			
				EVENT_REGISTER(Session,this);

				if( 1 != Session->Run() )
				{
					Session->Close();
					return ;
				}
			}
			else
			{
				APIWSAClose(Socket);
			}
		}

		void	StreamServer::HandleRecv( StreamSession* pSession,
										const StreamBuf_ptr& Buf_ptr )
		{
			/*Buf_ptr->data[Buf_ptr->payload] = 0;
			pSession->RecvNotify(pSession->GetRemoteNode(), Buf_ptr);*/

			SetStreamIdentity(Buf_ptr, pSession);

			pSession->AddUseRef();
			_MQueue.push_back(Buf_ptr);
		}

		void	StreamServer::HandleClose( StreamSession* pSession )
		{
			pSession->CloseNotify(pSession->GetRemoteNode(),
				pSession->GetStream()->GetSocketInfo().remote_ip.c_str(), 
				pSession->GetStream()->GetSocketInfo().remote_port);

			Close( pSession->GetStream()->GetSocketInfo().remote_node );
		}

		void	StreamServer::HandleDestroy( const Stream_HDestroy& hDestroy, NETHANDLE Node )
		{
			if( hDestroy )
				hDestroy(Node);
		}

		void	StreamServer::HandleExcept( StreamSession* pSession, const char* strError )
		{
			pSession->CloseNotify(pSession->GetRemoteNode(), 
				pSession->GetStream()->GetSocketInfo().remote_ip.c_str(),
				pSession->GetStream()->GetSocketInfo().remote_port);

			Close( pSession->GetStream()->GetSocketInfo().remote_node );
		}

		void	StreamServer::MessageThread( void* pParamter )
		{
			StreamServer* pServer = reinterpret_cast<StreamServer*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();

					//扫描退出队列
					if( pServer->_QuitQueue.size() > 0 )
					{
						pServer->_QuitLock.Lock();
						if( pServer->_QuitQueue.size() > 0 )
						{
							StreamSession_sptr sptr = pServer->_QuitQueue.front();
							if( sptr->isDestory() )
							{
								pServer->_QuitQueue.pop();
								pServer->_QuitLock.UnLock();
								CASSubAndFetch(&pServer->_Count);
							}
							else
							{
								pServer->_QuitLock.UnLock();
							}
						}
						else
						{
							pServer->_QuitLock.UnLock();
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

	} //namespace	_tcp_

} // _server_

