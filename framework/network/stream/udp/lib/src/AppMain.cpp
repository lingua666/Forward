
#include <libNetFrame_UDP_Stream/AppMain.h>

namespace	_server_{

	namespace	_udp_{

		AppMain::AppMain( void )
			: _Pool( sizeof(StreamSession) )
			, _Message(function20_bind(&AppMain::MessageThread,
					reinterpret_cast<void*>(this)))
			, _Count( 0 )
			, _isSequence( false )
		{

		}

		AppMain::~AppMain( void )
		{
			Release();
		}

		int AppMain::Init( UInt8 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence )
		{
			if( _ioService.open() == -1 )
			{
				return -1;
			}

			Run(uIOThreadNum, uProcThreadNum, isSequence);
			return 1;
		}

		void AppMain::Release( void )
		{
			Stop();
			if( _ioService.is_open() )
			{
				_ioService.stop();
				_ioService.close();
				for(std::size_t i = 0; i < _IOProcs.size(); ++ i)
				{
					_IOProcs[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
					_IOProcs[i]->join();
				}
				_IOProcs.clear();

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

		void	AppMain::Stop( void )
		{
			CloseAll();
		}

		void	AppMain::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		NETHANDLE	AppMain::Open( UInt16 u16Port )
		{
			_SOCKET_::HSOCKET hSock = _SOCKET_(_ioService).Open(u16Port);
			if( hSock == INVALID_SOCKET )
			{
				return -1;
			}

			StreamSession* Session = _Pool.AllocObj<StreamSession>( sizeof(tagStreamIdentity) + sizeof(struct sockaddr_in) );
			if( Session->Init(_ioService, hSock) != 1 )
			{
				int iError = error_code::GetLastError();
				printf("AppMain::Open is Error:%d\r\n",iError);
				_Pool.FreeObj(Session);
				APIWSAClose(hSock);
				return -1;
			}

			NETHANDLE	Node = Session->GetLocalNode();
			if( FindSession(Node) )
			{
				printf("AppMain::Open Node is exsit\r\n");
				_Pool.FreeObj(Session);
				return -1;
			}

			_Lock.Lock();
			StreamSession_sptr sptr(Session,
				function20_bind_Ex(&AppMain::FreeSession,this));
			_Map.insert(std::make_pair(Node, sptr));
			_Lock.UnLock();
			CASAddAndFetch(&_Count);

			EVENT_REGISTER(Session,this);

			if( 1 != Session->Run() )
			{
				Session->Close();
				return -1;
			}

			return Node;
		}

		int	AppMain::Close( NETHANDLE Node )
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

		void	AppMain::CloseSession( StreamSession_sptr ptr )
		{
			ptr->Close();	//关闭连接
			_QuitLock.Lock();
			_QuitQueue.push_back(ptr); //压入退出队列
			_QuitLock.UnLock();
		}

		const StreamSession_sptr	AppMain::FindSession( NETHANDLE Node )
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

		void	AppMain::FreeSession( void* pSession )
		{
			_Pool.FreeObj(reinterpret_cast<StreamSession*>(pSession));
		}

		void	AppMain::Run( UInt8 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence )
		{
			if( _IOProcs.size() <= 0 )
			{
				uIOThreadNum = (uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum);
				for(std::size_t i = 0; i < uIOThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type(
						function20_bind(&io_service::run, &_ioService)) );
					_IOProcs.push_back(thread);
					thread->wait_run();
				}
			}
			
			if( _Workers.size() <= 0 )
			{
				uProcThreadNum = (uProcThreadNum == 0 ? 1 : uProcThreadNum);
				_isSequence = isSequence;
				for(std::size_t i = 0; i < uProcThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type( function20_bind(&AppMain::WorkThread, this) ) );
					_Workers.push_back(thread);
					thread->wait_run();
				}
			}
		}

		void	AppMain::RunLoop( UInt8 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence )
		{
			if( _IOProcs.size() <= 0 )
			{
				for(std::size_t i = 0; i < uIOThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type(
						function20_bind(&io_service::run, &_ioService)) );
					_IOProcs.push_back(thread);
					thread->wait_run();
				}
			}

			if( _Workers.size() <= 0 )
			{
				_isSequence = isSequence;
				for(std::size_t i = 0; i < uProcThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type( function20_bind(&AppMain::WorkThread, this) ) );
					_Workers.push_back(thread);
					thread->wait_run();
				}
			}
		}

		void	AppMain::HandleRecv( StreamSession* pSession,
									sockaddr_in* pSrc,
									const StreamBuf_ptr& Buf_ptr )
		{
			SetStreamIdentity(Buf_ptr, pSession);
			SetStreamSrcAddr(Buf_ptr, pSrc);

			pSession->AddUseRef();
			_ReadLock.Lock();
			_MQueue.push_back(Buf_ptr);
			_ReadLock.UnLock();
		}

		void	AppMain::WorkThread( void* pParamter )
		{
			AppMain* Server = reinterpret_cast<AppMain*>(pParamter);
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
							pSession->RecvNotify(pSession->GetLocalNode(), Server->GetStreamSrcAddr(ptr), ptr);
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

		Int32	AppMain::Send( NETHANDLE Node, sockaddr_in* pDest, UInt32 MediaType,
								const char* c_pData, UInt16 u16Size )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(pDest, MediaType, c_pData, u16Size);

			return -1;
		}

		Int32	AppMain::Send( NETHANDLE Node, sockaddr_in* pDest, const char* c_pData, UInt16 u16Size )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(pDest, c_pData, u16Size);
			return -1;
		}

		void	AppMain::HandleClose( StreamSession* pSession )
		{
			pSession->CloseNotify(pSession->GetLocalNode(),
				pSession->GetStream()->GetSocketInfo().local_ip.c_str(), 
				pSession->GetStream()->GetSocketInfo().local_port);

			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	AppMain::HandleDestroy( const Stream_HDestroy& hDestroy, NETHANDLE Node )
		{
			if( hDestroy )
				hDestroy(Node);
		}

		void	AppMain::HandleExcept( StreamSession* pSession, const char* strError )
		{
			pSession->CloseNotify(pSession->GetRemoteNode(), 
				pSession->GetStream()->GetSocketInfo().local_ip.c_str(),
				pSession->GetStream()->GetSocketInfo().local_port);

			Close( pSession->GetStream()->GetSocketInfo().local_node );
		}

		void	AppMain::MessageThread( void* pParamter )
		{
			AppMain* pServer = reinterpret_cast<AppMain*>(pParamter);
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

	} //namespace	_udp_

} // client

