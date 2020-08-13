
#include <libNetFrame_RawServer/RawServer.h>

#define SOCKET_STREAM_COUNT		10000

namespace	_server_{

	RawServer::RawServer( void )
		: _Pool( sizeof(RawSession) )
		, _Accept()
		, _Worker(function20_bind(&RawServer::WorkThread, reinterpret_cast<void*>(this)))
		, _Message(function20_bind(&RawServer::MessageThread, reinterpret_cast<void*>(this)))
		, _Count( 0 )
	{

	}

	RawServer::~RawServer( void )
	{
		Release();
	}

	void RawServer::Release( void )
	{
		Stop();
		_Worker.interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Worker.join();
		_Message.interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Message.join();
		_QuitQueue.clear();
		_MQueue.clear();
		_Pool.Release();
	}

	void	RawServer::Stop( void )
	{
		_Accept.Stop();
		CloseAll();
	}

	void	RawServer::CloseAll( void )
	{
		while(_Map.size() > 0 )
		{
			Close(_Map.begin()->first);
		}

		_Map.clear();
	}

	int	RawServer::Close( NETHANDLE Node )
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

	void	RawServer::CloseSession( RawSession_sptr ptr )
	{
		ptr->Close(); //关闭连接
		_QuitLock.Lock();
		_QuitQueue.push_back(ptr); //压入退出队列
		_QuitLock.UnLock();
	}

	const RawSession_sptr	RawServer::FindSession( NETHANDLE Node )
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

	void	RawServer::FreeSession( void* pSession )
	{
		_Pool.FreeObj(reinterpret_cast<RawSession*>(pSession));
	}

	void	RawServer::Run( UInt32 u32Num )
	{
		_Accept.Run(u32Num);
	}

	void	RawServer::RunLoop( UInt32 u32Num )
	{
		_Accept.RunLoop(u32Num);
	}

	int	RawServer::Send( NETHANDLE Node, const char* c_pData, UInt32 u32Size )
	{
		RawSession_sptr sptr = FindSession(Node);
		if( sptr )
			return sptr->Send(c_pData, u32Size);
		return -1;
	}

	void	RawServer::HandleAccept( const _SOCKET_::HSOCKET& Socket )
	{
		if( _Count < SOCKET_STREAM_COUNT )
		{
			RawSession* Session = _Pool.AllocObj<RawSession>( sizeof(tagRawIdentity) );
			if( Session->Init((io_service&)_Accept.GetIoServer(),Socket) != 1 )
			{
				int iError = error_code::GetLastError();
				printf("RawSession::HandleAccept is Error:%d\r\n",iError);
				_Pool.FreeObj(Session);
				APIWSAClose(Socket);
				return ;
			}

			NETHANDLE	Node = Session->GetStream()->GetSocketInfo().remote_node;
			if( FindSession(Node) )
			{
				printf("RawSession::HandleAccept Node is exsit\r\n");
				_Pool.FreeObj(Session);
				return ;
			}

			_Lock.Lock();
			_Map.insert(std::make_pair(Node, 
				RawSession_sptr(Session,
				function20_bind_Ex(&RawServer::FreeSession,this))));
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

	void	RawServer::HandleRecv( RawSession* pSession,
									const StreamBuf_ptr& Buf_ptr )
	{
		_buf_hdr*	pHdr = Buf_ptr.get();
		SetCmdIdentity(Buf_ptr.get(), pSession);

		pSession->AddUseRef();
		_ReadLock.Lock();
		_MQueue.push_back(Buf_ptr);
		_ReadLock.UnLock();
	}

	void	RawServer::HandleClose( RawSession* pSession )
	{
		pSession->CloseNotify(pSession->GetRemoteNode(), pSession->GetStream()->GetSocketInfo().remote_ip.c_str(), 
							pSession->GetStream()->GetSocketInfo().remote_port);
		Close( pSession->GetStream()->GetSocketInfo().remote_node );
	}

	void	RawServer::HandleDestroy( const Raw_HDestroy& hDestroy, NETHANDLE Node )
	{
		if( hDestroy )
			hDestroy(Node);
	}

	void	RawServer::HandleExcept( RawSession* pSession, const char* strError )
	{
		pSession->CloseNotify(pSession->GetRemoteNode(), pSession->GetStream()->GetSocketInfo().remote_ip.c_str(),
			pSession->GetStream()->GetSocketInfo().remote_port);
		Close( pSession->GetStream()->GetSocketInfo().remote_node );
	}

	void	RawServer::WorkThread( void* pParamter )
	{
		RawServer* Server = reinterpret_cast<RawServer*>(pParamter);
		while(true)
		{
			try
			{
				ThreadWrap::interruption_point();
				if( Server->_MQueue.size() > 0 )
				{
					Server->_ReadLock.Lock();
					StreamBuf_ptr ptr = Server->_MQueue.front();
					Server->_MQueue.pop_front();
					Server->_ReadLock.UnLock();
					RawSession*	pSession = Server->GetCmdIdentity(ptr.get());

					//增加结束符，防止打印字符串出现乱码
					ptr->data[ptr->payload] = 0;
					pSession->RecvNotify(pSession->GetRemoteNode(),
						StreamBuf::GetData(ptr.get()),
						StreamBuf::GetSize(ptr.get()));

					ptr.reset();

					//更新心跳
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
		Server->_MQueue.clear();
	}

	void	RawServer::MessageThread( void* pParamter )
	{
		RawServer* Server = reinterpret_cast<RawServer*>(pParamter);
		while(true)
		{
			try
			{
				ThreadWrap::interruption_point();

				//扫描退出队列
				if( Server->_QuitQueue.size() > 0 )
				{
					Server->_QuitLock.Lock();
					if( Server->_QuitQueue.front()->isDestory() )
					{
						Server->_QuitQueue.pop_front();
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

} //namespace	_server_