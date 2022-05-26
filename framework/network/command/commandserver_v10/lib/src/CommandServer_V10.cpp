
#include <libNetFrame_CmdServer_V10/CommandServer_V10.h>
#include <libNetFrame_CmdServer_V10/Heart.h>

#define SOCKET_STREAM_COUNT		10000

namespace	_server_{

	CommandServer_V10::CommandServer_V10( void )
		: _Pool( sizeof(CmdSession) )
		, _Message(function20_bind(&CommandServer_V10::MessageThread, reinterpret_cast<void*>(this)))
		, _Count( 0 )
		, _uWorkMS(1)
		, _uDestroyMS(1)
	{
		
	}

	CommandServer_V10::~CommandServer_V10( void )
	{
		Release();
	}

	void CommandServer_V10::Init( UInt8 uWorkerThreads )
	{
		((io_service&)_Accept.GetIoServer()).Init();

		if( _Workers.size() <= 0 )
		{
			uWorkerThreads = (uWorkerThreads == 0 ? 1 : uWorkerThreads);
			for(std::size_t i = 0; i < uWorkerThreads; ++ i)
			{
				Thread_ptr thread( new Thread_type( function20_bind(&CommandServer_V10::WorkThread, this) ) );
				_Workers.push_back(thread);
				thread->wait_run();
			}
		}
	}

	void CommandServer_V10::Release( void )
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
			_MQueue.clear();
			Heart::Clear();
			_Count = 0;
			/*Heart::Release();
			_Pool.Release();*/
		}
	}

	void	CommandServer_V10::Stop( void )
	{
		_Accept.Stop();
		CloseAll();
	}

	void	CommandServer_V10::CloseAll( void )
	{
		while(_Map.size() > 0)
		{
			Close(_Map.begin()->first);
		}

		_Map.clear();
	}

	int	CommandServer_V10::Close( NETHANDLE Node )
	{
		_Lock.Lock();
		HashMap_type::iterator iter = _Map.find(Node);
		if( iter != _Map.end() )
		{
			CmdSession_sptr sptr = iter->second;
			_Map.erase(iter);
			_Lock.UnLock();
			Heart::Stop(Node);//从列表中清除会话心跳信息
			CloseSession(sptr); //关闭会话
			return 1;
		}
		else
			_Lock.UnLock();

		return -1;
	}

	void	CommandServer_V10::CloseSession( CmdSession_sptr ptr )
	{
		ptr->Close(); //关闭连接
		_QuitLock.Lock();
		_QuitQueue.push_back(ptr); //压入退出队列
		_QuitLock.UnLock();
	}

	const CmdSession_sptr	CommandServer_V10::FindSession( NETHANDLE Node )
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

	void	CommandServer_V10::FreeSession( void* pSession )
	{
		_Pool.FreeObj(reinterpret_cast<CmdSession*>(pSession));
	}

	void	CommandServer_V10::Run( UInt8 uThreadNum )
	{
		_Accept.Run(uThreadNum == 0 ? get_processor_number() * 2 + 2 : uThreadNum);
	}

	void	CommandServer_V10::RunLoop( UInt8 uThreadNum )
	{
		_Accept.RunLoop(uThreadNum == 0 ? get_processor_number() * 2 + 2 : uThreadNum);
	}

	int	CommandServer_V10::SendError( NETHANDLE Node, const char* c_pData, UInt32 uSize )
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

	int	CommandServer_V10::Send( NETHANDLE Node, const char* c_pData, UInt32 uSize )
	{
		CmdSession_sptr sptr = FindSession(Node);
		if( sptr )
		{
			_session_hdr Hdr = {0};
			return sptr->Send(&Hdr, c_pData, uSize);
		}
		return -1;
	}

	//单位毫秒
	void	CommandServer_V10::SetSleepStep(UInt32 uWorkMS, UInt32 uDestroyMS)
	{
		_uWorkMS = uWorkMS;
		_uDestroyMS = uDestroyMS;
	}

	void	CommandServer_V10::HandleAccept( const _SOCKET_::HSOCKET& Socket )
	{
		if( _Count < SOCKET_STREAM_COUNT )
		{
			CmdSession* Session = _Pool.AllocObj<CmdSession>( sizeof(tagCmdIdentity) );
			if( Session->Init((io_service&)_Accept.GetIoServer(),Socket) != 1 )
			{
				int iError = error_code::GetLastError();
				printf("CommandServer_V10::HandleAccept is Error:%d\r\n",iError);
				_Pool.FreeObj(Session);
				APIWSAClose(Socket);
				return ;
			}

			NETHANDLE	Node = Session->GetStream()->GetSocketInfo().remote_node;
			if( FindSession(Node) )
			{
				printf("CommandServer_V10::HandleAccept Node is exsit\r\n");
				_Pool.FreeObj(Session);
				return ;
			}

			//STREAM_HANDLE  sptr = Session->GetStream();//引用，保证即使客户端退出也能够执行完以下代码才释放回话资源

			_Lock.Lock();
			CmdSession_sptr sptr(Session,
				function20_bind_Ex(&CommandServer_V10::FreeSession,this));

			_Map.insert(std::make_pair(Node, sptr));
			_Lock.UnLock();

			CASAddAndFetch(&_Count);

			if( _AcceptHandle )
			{
				_AcceptHandle( Session->GetStream()->GetSocketInfo().remote_ip.c_str(),
					Session->GetStream()->GetSocketInfo().remote_port, Node );
			}

			Heart::Attach(Node, sptr);
			EVENT_REGISTER(Session,this);		// 所有动作执行完毕后才开始注册事件，防止在事件注册后链接突然断开，引起后续动作无法正常完成，程序出现异常

			if( 1 != Session->Run() )
			{
				Heart::Stop(Node);
				Session->Close();
				return; 
			}
		}
		else
		{
			APIWSAClose(Socket);
		}
	}

	void	CommandServer_V10::HandleRecv( CmdSession* pSession,
									const CmdDBuf_ptr& Buf_ptr )
	{
		_session_hdr* p = reinterpret_cast<_session_hdr*>(Buf_ptr->_Data);
		if( p->_uType == SESSION_HEART_TYPE_DEF)
		{//是否心跳
			Heart::Update(pSession->GetRemoteNode());
			Heart::Send(pSession);
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

	void	CommandServer_V10::HandleClose( CmdSession* pSession )
	{
		pSession->CloseNotify(pSession->GetRemoteNode(), pSession->GetStream()->GetSocketInfo().remote_ip.c_str(), 
							pSession->GetStream()->GetSocketInfo().remote_port);
		Close( pSession->GetStream()->GetSocketInfo().remote_node );
	}

	void	CommandServer_V10::HandleDestroy( const Comamand_HDestroy& hDestroy, NETHANDLE Node )
	{
		if( hDestroy )
			hDestroy(Node);
	}

	void	CommandServer_V10::HandleExcept( CmdSession* pSession, const char* strError )
	{
		pSession->CloseNotify(pSession->GetRemoteNode(), pSession->GetStream()->GetSocketInfo().remote_ip.c_str(),
			pSession->GetStream()->GetSocketInfo().remote_port);
		Close( pSession->GetStream()->GetSocketInfo().remote_node );
	}

	void	CommandServer_V10::WorkThread( void* pParamter )
	{
		CommandServer_V10* Server = reinterpret_cast<CommandServer_V10*>(pParamter);
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
						CmdDBuf_ptr ptr = Server->_MQueue.front();
						Server->_MQueue.pop_front();
						Server->_ReadLock.UnLock();
						CmdSession*	pSession = Server->GetCmdIdentity(ptr);

						//增加结束符，防止打印字符串出现乱码
						ptr->_Data[ptr->_uPayload] = 0;
						pSession->RecvNotify(pSession->GetRemoteNode(),
											ptr->_Data + sizeof(_session_hdr),
											ptr->_uPayload - sizeof(_session_hdr));

						ptr.reset();

						//更新心跳
						/*if( pSession->is_valid() )
						{
							Heart::Update(pSession->GetRemoteNode());
						}*/
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

	void	CommandServer_V10::MessageThread( void* pParamter )
	{
		CommandServer_V10* Server = reinterpret_cast<CommandServer_V10*>(pParamter);
		while(true)
		{
			try
			{
				ThreadWrap::interruption_point();
				Heart::CheckHeart();

				//扫描退出队列
				if( Server->_QuitQueue.size() > 0 )
				{
					Server->_QuitLock.Lock();
					if( Server->_QuitQueue.size() > 0 )
					{
						CmdSession_sptr sptr = Server->_QuitQueue.front();
						if( sptr->isDestory() )
						{
							Heart::Detach(sptr->GetRemoteNode());//从列表中清除会话心跳信息
							Server->_QuitQueue.pop();
							Server->_QuitLock.UnLock();
							CASSubAndFetch(&Server->_Count);
						}
						else
						{
							Server->_QuitLock.UnLock();
						}
					}
					else
					{
						Server->_QuitLock.UnLock();
					}
				}
				else
				{
					Sleep(Server->_uDestroyMS);
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

} //namespace	_server_