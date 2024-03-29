
#include <libNetFrame_StreamServer_V20/StreamServer_V20.h>

#define SOCKET_STREAM_COUNT		10000

namespace	_server_{

	namespace	_tcp_{

		StreamServer_V20::StreamServer_V20( void )
			: _Pool( sizeof(StreamSession) )
			, _Message(function20_bind(&StreamServer_V20::MessageThread,
						reinterpret_cast<void*>(this)))
			, _Count( 0 )
			, _isSequence( false )
			, _uWorkMS(1)
			, _uDestroyMS(1)
		{

		}

		StreamServer_V20::~StreamServer_V20( void )
		{
			Release();
		}

		int StreamServer_V20::Init(UInt8 uIOThreadNum, UInt8 uWorkerThreads, bool isSequence)
		{
			((io_service&)_Accept.GetIoServer()).Init();

			if (_Accept.Init(uIOThreadNum == 0 ? get_processor_number() * 2 + 2 : uIOThreadNum) == -1)
				return -1;

			if (_Workers.size() <= 0)
			{
				uWorkerThreads = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
				for (std::size_t i = 0; i < uWorkerThreads; ++i)
				{
					Thread_ptr thread(new Thread_type(function20_bind(&StreamServer_V20::WorkThread, this)));
					_Workers.push_back(thread);
					thread->wait_run();
				}
			}

			_isSequence = isSequence;

			return 1;
		}

		void StreamServer_V20::Release( void )
		{
			if( _Accept.is_open() )
			{
				_Accept.Release();
				StopAll();
				
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

		void StreamServer_V20::StopAll(void)
		{
			while (_ListenMap.size())
			{
				Stop(_ListenMap.begin()->first);
			}
		}

		int StreamServer_V20::Stop(NETHANDLE hNet)
		{
			int iRet = -1;
			tagListen_INFO_sptr sptr;
			ListenMap_type::iterator iter;
			_Lock.Lock();
			iter = _ListenMap.find(hNet);
			if (iter != _ListenMap.end())
				sptr = iter->second;
			_Lock.UnLock();

			if (sptr)
			{
				while (sptr->Sessions.size() > 0)
				{
					socket_type::CloseSocket(sptr->Sessions.begin()->second);
					RemoveSessionToListen(sptr->hNet, sptr->Sessions.begin()->first);
				}

				_Accept.Close(sptr->Sock);
				_Lock.Lock();
				_ListenMap.erase(sptr->hNet);
				_Lock.UnLock();
				iRet = 1;
			}

			return iRet;
		}

		void	StreamServer_V20::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		int	StreamServer_V20::Close( NETHANDLE Node )
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

		void	StreamServer_V20::CloseSession( StreamSession_sptr ptr )
		{
			ptr->Close();	//关闭连接
			_QuitLock.Lock();
			_QuitQueue.push_back(ptr); //压入退出队列
			_QuitLock.UnLock();
		}

		const StreamSession_sptr	StreamServer_V20::FindSession( NETHANDLE Node )
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

		void	StreamServer_V20::FreeSession( NETHANDLE hListenNet,
			NETHANDLE hNet, void* pSession )
		{
			_Pool.FreeObj(reinterpret_cast<StreamSession*>(pSession));
			RemoveSessionToListen(hListenNet, hNet);
		}

		void	StreamServer_V20::WorkThread( void* pParamter )
		{
			StreamServer_V20* Server = reinterpret_cast<StreamServer_V20*>(pParamter);
			StreamSession*	pSession = NULL;
			bool			isLock = false;

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
							if (Server->_isSequence)
							{
								pSession->push(ptr);
								isLock = pSession->TryLock();
							}
							Server->_ReadLock.UnLock();

							if (Server->_isSequence)
							{
								if (!isLock)
								{
									pSession->Lock();
									isLock = true;
								}

								ptr = pSession->pop_front();
							}

							//增加结束符，防止打印字符串出现乱码
							ptr->data[ptr->payload] = 0;
							pSession->RecvNotify(pSession->GetRemoteNode(), ptr, pSession->GetUseRef() - 1);
							if (Server->_isSequence)
							{
								if (isLock)
								{
									pSession->UnLock();
								}
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
						Sleep(Server->_uWorkMS);
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

		Int32	StreamServer_V20::Send( NETHANDLE Node, UInt32 MediaType,
								const char* c_pData, UInt32 uSize )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(MediaType, c_pData, uSize);

			return -1;
		}

		Int32	StreamServer_V20::Send( NETHANDLE Node, const char* c_pData, UInt32 uSize )
		{
			StreamSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->Send(c_pData, uSize);
			return -1;
		}

		//单位毫秒
		void	StreamServer_V20::SetSleepStep(UInt32 uWorkMS, UInt32 uDestroyMS)
		{
			_uWorkMS = uWorkMS;
			_uDestroyMS = uDestroyMS;
		}

		void	StreamServer_V20::HandleAccept(NETHANDLE hListenNet,
			const _SOCKET_::HSOCKET& Socket)
		{
			if( _Count < SOCKET_STREAM_COUNT )
			{
				StreamSession* Session = _Pool.AllocObj<StreamSession>( sizeof(tagStreamIdentity) );
				if( Session->Init((io_service&)_Accept.GetIoServer(),Socket) != 1 )
				{
					int iError = error_code::GetLastError();
					printf("StreamServer_V20::HandleAccept is Error:%d\r\n",iError);
					_Pool.FreeObj(Session);
					APIWSAClose(Socket);
					return ;
				}

				NETHANDLE	Node = Session->GetRemoteNode();
				if( FindSession(Node) )
				{
					printf("StreamServer_V20::HandleAccept Node is exsit\r\n");
					_Pool.FreeObj(Session);
					return ;
				}

				_Lock.Lock();
				StreamSession_sptr sptr(Session,
					function20_bind(&StreamServer_V20::FreeSession,this,
						hListenNet, Node, _function_::_1));
				_Map.insert(std::make_pair(Node, sptr));
				_Lock.UnLock();
				CASAddAndFetch(&_Count);

				if( _AcceptHandle )
				{
					_AcceptHandle(hListenNet, Session->GetStream()->GetSocketInfo().remote_ip.c_str(),
						Session->GetStream()->GetSocketInfo().remote_port,
						Node );
				}
			
				EVENT_REGISTER(Session,this);

				if( 1 != Session->Run() )
				{
					Session->Close();
					return ;
				}

				AddSessionToListen(hListenNet, Node, Socket);
			}
			else
			{
				APIWSAClose(Socket);
			}
		}

		int StreamServer_V20::AddSessionToListen(NETHANDLE hListNet, NETHANDLE hNet, const socket_type::HSOCKET& hSocket)
		{
			int iRet = -1;
			ListenMap_type::iterator iter;
			_ListenLock.Lock();
			iter = _ListenMap.find(hListNet);
			if (iter != _ListenMap.end())
			{
				iter->second->Sessions.insert(std::make_pair(hNet, hSocket));
				iRet = 1;
			}
			_ListenLock.UnLock();

			return iRet;
		}

		int StreamServer_V20::RemoveSessionToListen(NETHANDLE hListNet, NETHANDLE hNet)
		{
			int iRet = -1;
			ListenMap_type::iterator iter;
			_ListenLock.Lock();
			iter = _ListenMap.find(hListNet);
			if (iter != _ListenMap.end())
			{
				iter->second->Sessions.erase(hNet);
				iRet = 1;
			}
			_ListenLock.UnLock();

			return iRet;
		}

		void	StreamServer_V20::HandleRecv( StreamSession* pSession,
										const StreamBuf_ptr& Buf_ptr )
		{
			/*Buf_ptr->data[Buf_ptr->payload] = 0;
			pSession->RecvNotify(pSession->GetRemoteNode(), Buf_ptr);*/

			SetStreamIdentity(Buf_ptr, pSession);

			pSession->AddUseRef();
			_MQueue.push_back(Buf_ptr);
		}

		void	StreamServer_V20::HandleClose( StreamSession* pSession )
		{
			pSession->CloseNotify(pSession->GetRemoteNode(),
				pSession->GetStream()->GetSocketInfo().remote_ip.c_str(), 
				pSession->GetStream()->GetSocketInfo().remote_port);

			Close( pSession->GetStream()->GetSocketInfo().remote_node );
		}

		void	StreamServer_V20::HandleDestroy( const Stream_HDestroy& hDestroy, NETHANDLE Node )
		{
			if( hDestroy )
				hDestroy(Node);
		}

		void	StreamServer_V20::HandleExcept( StreamSession* pSession, const char* strError )
		{
			pSession->CloseNotify(pSession->GetRemoteNode(), 
				pSession->GetStream()->GetSocketInfo().remote_ip.c_str(),
				pSession->GetStream()->GetSocketInfo().remote_port);

			Close( pSession->GetStream()->GetSocketInfo().remote_node );
		}

		void	StreamServer_V20::MessageThread( void* pParamter )
		{
			StreamServer_V20* pServer = reinterpret_cast<StreamServer_V20*>(pParamter);
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
						Sleep(pServer->_uDestroyMS);
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

		UInt32	StreamServer_V20::GetSessionCount(NETHANDLE hListenNet)
		{
			UInt32 uCount = 0;
			tagListen_INFO_sptr sptr;
			ListenMap_type::iterator iter;

			_ListenLock.Lock();
			iter = _ListenMap.find(hListenNet);
			if (iter != _ListenMap.end())
				sptr = iter->second;
			_ListenLock.UnLock();

			if (sptr)
			{
				uCount = sptr->Sessions.size();
			}

			return uCount;
		}

	} //namespace	_tcp_

} // _server_

