
#include <libIOCP_Net/WSASocket.h>

SOCKET APIWSACreate( int iSocketType, int iRevSize, int iSendSize, const char* c_szIP )
{
	SOCKET sock = WSASocket( AF_INET
		, iSocketType
		, IPPROTO_IP
		, NULL
		, NULL
		, WSA_FLAG_OVERLAPPED );

	if ( sock == INVALID_SOCKET )
		return INVALID_SOCKET;

	int buflen = iSendSize;
	// 设置SOCKET的发送缓存
	if( buflen >= 0)
	{
		if ( APIWSASetSockopt( sock, SOL_SOCKET,SO_SNDBUF, (const char*)&buflen,sizeof(int)) == SOCKET_ERROR )
		{
			APIWSAClose( sock );
			return INVALID_SOCKET;
		}
	}

	buflen = iRevSize;
	// 设置SOCKET的接收缓存
	if(buflen >= 0)
	{
		if ( APIWSASetSockopt( sock, SOL_SOCKET,SO_RCVBUF, (const char*)&buflen,sizeof(int)) == SOCKET_ERROR )
		{
			APIWSAClose( sock );
			return INVALID_SOCKET;
		}
	}

	if( iSocketType == SOCK_STREAM )
	{//TCP
		// 关闭 TCP 的 Nagle 算法
		int opt = 1;
		if( SOCK_STREAM == iSocketType && 
			APIWSASetSockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(opt)) == SOCKET_ERROR )
		{
			APIWSAClose( sock );
			return INVALID_SOCKET;
		}

		// 可重用
		/*int i_val = 1;
		if( APIWSASetSockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i_val, sizeof(i_val)) == SOCKET_ERROR)
		{
			APIWSAClose( sock );
			return INVALID_SOCKET;
		}*/

		//立即关闭
		LINGER linger = {1,0}; 
		if( APIWSASetSockopt(sock, SOL_SOCKET, SO_LINGER, 
			(char *)&linger, sizeof(linger)) == SOCKET_ERROR )
		{
			APIWSAClose( sock );
			return INVALID_SOCKET;
		}
	}
	else
	{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
		//UDP Socket接收时收到，
		//由于上一次Send操作向一个地址发送，但是那个地址没有Socket监听（例如对等体崩溃），
		//那么ICMP控制协议会向我们发送一个Port Unreachable错误，
		//但是Winsock把这个错误转化为Connection Reset，在你下一次调用读操作的时候，引发10054异常
		//以下设置解决上述问题
		DWORD	dwBytesReturned = 0;  
		BOOL	bNewBehavior = FALSE;  
		DWORD	status;  

		// disable  new behavior using  
		// IOCTL: SIO_UDP_CONNRESET  
		status = WSAIoctl(sock, SIO_UDP_CONNRESET,  
			&bNewBehavior, sizeof(bNewBehavior),  
			NULL, 0, &dwBytesReturned,  
			NULL, NULL);  

		if (SOCKET_ERROR == status)  
		{
			DWORD dwErr = WSAGetLastError();
			if (WSAEWOULDBLOCK == dwErr)  
			{  
				// nothing to do  
				APIWSAClose( sock );
				return INVALID_SOCKET; 
			}  
			else  
			{  
				printf("WSAIoctl(SIO_UDP_CONNRESET) Error: %d\n", dwErr);  
				APIWSAClose( sock );
				return INVALID_SOCKET;
			}  
		}

#endif
	}

	if (c_szIP != NULL)
	{
		APIWSABind(sock, 0, c_szIP);
	}

	return sock;
}

BOOL   APIWSAClose( SOCKET Sock )
{
	if ( Sock != INVALID_SOCKET )
	{
		::shutdown( Sock, SD_BOTH );
		::closesocket( Sock );
		return TRUE;
	}

	return FALSE;
}

int APIWSABind( SOCKET Sock, u_short usPort, const char* c_szIP  )
{
	SOCKET_INVALID(Sock);

	int nRet = 0;
	sockaddr_in local;

	if(usPort > 0)
		local.sin_port				= htons( usPort );

	if(c_szIP != NULL)
		local.sin_addr.S_un.S_addr	= inet_addr( c_szIP );
	else
		local.sin_addr.S_un.S_addr	= htonl(INADDR_ANY); 
	local.sin_family			= AF_INET;

	nRet = ::bind( Sock, (sockaddr*)&local, sizeof(local) ); 
	return nRet;
}

int APIWSAListen( SOCKET Sock, u_int ListenNum, LPFN_ACCEPTEX& lpfnAcceptEx,
				LPFN_GETACCEPTEXSOCKADDRS& lpfnGetAcceptExSockAddrs )
{
	SOCKET_INVALID(Sock);

	int nRet = listen( Sock, ListenNum );
	if(nRet != SOCKET_ERROR)
	{
		// AcceptEx 和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
		GUID GuidAcceptEx = WSAID_ACCEPTEX;  
		GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

		// 使用AcceptEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
		// 所以需要额外获取一下函数的指针，
		// 获取AcceptEx函数指针
		DWORD dwBytes = 0;  
		if(SOCKET_ERROR == WSAIoctl(
			Sock, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidAcceptEx, 
			sizeof(GuidAcceptEx), 
			&lpfnAcceptEx, 
			sizeof(lpfnAcceptEx), 
			&dwBytes, 
			NULL, 
			NULL))  
		{  

			return SOCKET_ERROR;  
		}  

		// 获取GetAcceptExSockAddrs函数指针，也是同理
		if(SOCKET_ERROR == WSAIoctl(
			Sock, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidGetAcceptExSockAddrs,
			sizeof(GuidGetAcceptExSockAddrs), 
			&lpfnGetAcceptExSockAddrs, 
			sizeof(lpfnGetAcceptExSockAddrs),   
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			return SOCKET_ERROR; 
		}  
	}
	
	return nRet;
}

int APIWSAConnect( SOCKET Sock, const char* c_szIP, u_short usPort )
{
	SOCKET_INVALID(Sock);

	SOCKADDR_IN addr;
	addr.sin_family				= AF_INET;
	addr.sin_addr.S_un.S_addr	= inet_addr( c_szIP );
	addr.sin_port				= htons( usPort );

	int nRet = ::connect( Sock, (sockaddr *) &addr, sizeof(addr) );
	return nRet;
}

int APIWSAConnectExInit( SOCKET Sock, LPFN_CONNECTEX& lpfnConnectEx )
{
	SOCKET_INVALID(Sock);

	// 以下的绑定很重要，也是容易漏掉的。（如果少了绑定，在 ConnextEx 时将得到错误代码：10022 — 提供了一个无效的参数

	sockaddr_in local_addr ;
	ZeroMemory (&local_addr , sizeof (sockaddr_in ));
	local_addr .sin_family = AF_INET ;
	int nRet = ::bind (Sock , (sockaddr *)(&local_addr ), sizeof (sockaddr_in ));

	if(nRet != SOCKET_ERROR)
	{
		// ConnectEx的GUID，用于导出函数指针
		GUID GuidConnectEx = WSAID_CONNECTEX;

		// 使用ConnectEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
		// 所以需要额外获取一下函数的指针，
		// 获取ConnectEx函数指针
		DWORD dwBytes = 0;  
		if(SOCKET_ERROR == WSAIoctl(
			Sock, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidConnectEx, 
			sizeof(GuidConnectEx), 
			&lpfnConnectEx, 
			sizeof(lpfnConnectEx), 
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			return SOCKET_ERROR;  
		}
	}

	return nRet;
}

int APIWSAConnectEx( LPFN_CONNECTEX& lpfnConnectEx, 
					SOCKET	Sock, sockaddr_in& Addr,
					SWSAOverlapped* pSWSAOverlapped,
					unsigned short usBufSize )
{
	int nLen = sizeof (Addr);
	PVOID lpSendBuffer = NULL;
	DWORD dwSendDataLength = 0;
	DWORD dwBytesSent = 0;

	sockaddr_in local_addr ;
	ZeroMemory (&local_addr , sizeof (sockaddr_in ));
	local_addr .sin_family = AF_INET ;
	int irt = ::bind (Sock , (sockaddr *)(&local_addr), sizeof (sockaddr_in));

	if ( FALSE == lpfnConnectEx (Sock ,
					(sockaddr *)&Addr ,  // [in] 对方地址
					nLen ,               // [in] 对方地址长度
					lpSendBuffer ,       // [in] 连接后要发送的内容，这里不用
					dwSendDataLength ,   // [in] 发送内容的字节数 ，这里不用
					&dwBytesSent ,       // [out] 发送了多少个字节，这里不用
			(OVERLAPPED *)&pSWSAOverlapped->wsaOverlapped ) )      // 返回值处理
	{
		if(WSA_IO_PENDING != WSAGetLastError())
		{  
			printf("APIWSAConnectEx Is Fail,ErrorCode:%d",WSAGetLastError());
			return FALSE;
		}
	}
	
	return TRUE;
}

SOCKET APIWSAAccept( SOCKET Sock, sockaddr* pAddr )
{
	SOCKET_INVALID(Sock);

	int addrlen = sizeof(sockaddr_in);
	return ::accept( Sock, pAddr, &addrlen);
}

BOOL	APIWSAacceptEx( LPFN_ACCEPTEX& lpfnAcceptEx, 
						SOCKET	ListenSock, SOCKET& NewSock,
						SWSAOverlapped* pSWSAOverlapped, unsigned short usBufSize )
{
	if(lpfnAcceptEx != NULL && pSWSAOverlapped != NULL)
	{//必须创建一个新SOCKET，才能完成接收
		NewSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  
		if( INVALID_SOCKET == NewSock )
		{  
			return FALSE;  
		}

		// 投递AcceptEx
		DWORD dwBytes = 0;
		if(FALSE == lpfnAcceptEx( ListenSock, NewSock, pSWSAOverlapped->wsaBuf.buf, 
					usBufSize, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
					&dwBytes, &pSWSAOverlapped->wsaOverlapped)) 
		{  
			if(WSA_IO_PENDING != WSAGetLastError())
			{  
				printf("WSAacceptEx Is Fail,ErrorCode:%d",WSAGetLastError());
				return FALSE;  
			}
		}
	}

	return TRUE;
}

BOOL APIWSAGetacceptAddr( LPFN_GETACCEPTEXSOCKADDRS& lpfnGetAcceptExSockAddrs,
					SWSAOverlapped* pSWSAOverlapped, sockaddr** lpLocaladdr, 
					sockaddr** lpRemoteaddr )
{
	if(lpfnGetAcceptExSockAddrs == NULL || pSWSAOverlapped == NULL
		|| pSWSAOverlapped->wsaBuf.buf == NULL)
		return FALSE;

	int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);
	lpfnGetAcceptExSockAddrs(pSWSAOverlapped->wsaBuf.buf, 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, lpLocaladdr, 
			&localLen, lpRemoteaddr, &remoteLen);  

	return TRUE;
}

int APIWSASend( SOCKET Sock, const char* c_szDataBuf, u_long ulDataSize,
						WSAOVERLAPPED* pOverlapped )
{
	SOCKET_INVALID(Sock);

	WSABUF wsabuf;
	DWORD  sendbyte = 0;
	wsabuf.buf = (char*)c_szDataBuf;
	wsabuf.len = ulDataSize;
	if( ::WSASend( Sock
		, &wsabuf
		, 1
		, &sendbyte
		, 0
		, pOverlapped
		, NULL ) == 0 || WSAGetLastError() == WSA_IO_PENDING )
	{
		return 1;
	}

	return SOCKET_ERROR;
}

int APIWSARecv( SOCKET Sock, char* szDataBuf, u_long ulDataSize,
						WSAOVERLAPPED* pOverlapped )
{
	SOCKET_INVALID(Sock);

	DWORD dwFlags = 0,revbyte = 0;
	WSABUF wsaBuf;
	wsaBuf.buf = szDataBuf;
	wsaBuf.len = ulDataSize;

	if( ::WSARecv( Sock
		, &wsaBuf
		, 1
		, &revbyte
		, &dwFlags
		, pOverlapped
		, NULL ) == 0 || WSAGetLastError() == WSA_IO_PENDING )
	{
		return 0;
	}

	return SOCKET_ERROR;
}

int g_nWSALen = sizeof(sockaddr);//sizeof(sockaddr_in);

int APIWSASendto( SOCKET Sock, const char* c_szDataBuf, u_long ulDataSize,
								 sockaddr* pAddr ,WSAOVERLAPPED* pOverlapped )
{
	SOCKET_INVALID(Sock);

	WSABUF wsabuf;
	DWORD  sendbyte = 0;
	wsabuf.buf = (char*)c_szDataBuf;
	wsabuf.len = ulDataSize;
	if( ::WSASendTo( Sock
		, &wsabuf
		, 1
		, &sendbyte
		, 0
		, pAddr
		, g_nWSALen
		, pOverlapped
		, NULL ) == 0 || WSAGetLastError() == WSA_IO_PENDING )
	{
		return 0;
	}

	return SOCKET_ERROR;
}

int APIWSARecvFrom( SOCKET Sock, char* szDataBuf, u_long ulDataSize,
					sockaddr* pAddr,WSAOVERLAPPED* pOverlapped )
{
	SOCKET_INVALID(Sock);
	DWORD dwFlags = 0,revbyte = 0;
	WSABUF wsaBuf;
	wsaBuf.buf = szDataBuf;
	wsaBuf.len = ulDataSize;

	if( ::WSARecvFrom( Sock
		, &wsaBuf
		, 1
		, &revbyte
		, &dwFlags
		, pAddr
		, &g_nWSALen
		, pOverlapped
		, NULL ) == 0 || WSAGetLastError() == WSA_IO_PENDING )
	{
		return 0;
	}

	return SOCKET_ERROR;
}

int  APIWSASetSockopt( SOCKET Sock, int Level, int Optname, 
							   const char* c_szOptval, int Optlen )
{
	SOCKET_INVALID(Sock);
	
	//设置SOCKET参数
	int nRet = ::setsockopt( Sock, Level, Optname, c_szOptval, Optlen );
	if(nRet == SOCKET_ERROR )
		return SOCKET_ERROR;

	return TRUE;
}

BOOL  APIGetWSASocketLocalIP( SOCKET Sock, char* szIP, u_int uiSize )
{
	if ( Sock == INVALID_SOCKET && szIP != NULL )
		return FALSE;

	SOCKADDR_IN addr = {0};
	int len = sizeof(addr);
	if( ::getsockname( Sock, (sockaddr*)&addr, &len) == SOCKET_ERROR )
		return FALSE;
	
	char* IP = inet_ntoa(addr.sin_addr);
	len = __min(strlen(IP),uiSize);
	memcpy(szIP,IP,len);
	return TRUE;
}

BOOL  APIGetWSASocketLocalPort( SOCKET Sock, u_short& usPort )
{
	if ( Sock == INVALID_SOCKET )
		return FALSE;

	SOCKADDR_IN addr = {0};
	int len = sizeof(addr);
	if( ::getsockname( Sock, (sockaddr*)&addr, &len) == SOCKET_ERROR )
		return FALSE;

	usPort = ntohs(addr.sin_port);
	return TRUE;
}

BOOL  APIGetWSASocketRemoteIP( SOCKET Sock, char* szIP, u_short usSize )
{
	if ( Sock == INVALID_SOCKET && szIP != NULL )
		return FALSE;

	SOCKADDR_IN addr = {0};
	int len = sizeof(addr);
	if( ::getpeername( Sock, (sockaddr*)&addr, &len) == SOCKET_ERROR )
		return FALSE;

	char* IP = inet_ntoa(addr.sin_addr);
	len = __min(strlen(IP),usSize);
	memcpy(szIP,IP,len);

	return TRUE;
}

BOOL  APIGetWSASocketRemotePort( SOCKET Sock, u_short& usPort )
{
	if ( Sock == INVALID_SOCKET )
		return FALSE;

	SOCKADDR_IN addr = {0};
	int len = sizeof(addr);
	if( ::getpeername( Sock, (sockaddr*)&addr, &len) == SOCKET_ERROR )
		return FALSE;
	
	usPort = ntohs(addr.sin_port);
	return TRUE;
}

BOOL APIWSASetKeepalive( SOCKET Sock,int iTime,int iTimernum )
{
	BOOL bKeepAlive = TRUE;  
	int nRet = ::setsockopt(Sock, SOL_SOCKET, SO_KEEPALIVE,   
		(char*)&bKeepAlive, sizeof(bKeepAlive));  
	if (nRet == SOCKET_ERROR)  
	{  
		return FALSE;  
	}  
	// set KeepAlive parameter   
	tagWSAtcp_keepalive alive_in;  
	tagWSAtcp_keepalive alive_out;  
	alive_in.keepalivetime    = iTimernum;  // 0.5s   
	alive_in.keepaliveinterval  = iTime; //1s   
	alive_in.onoff                       = TRUE;  
	unsigned long ulBytesReturn = 0;  
	nRet = ::WSAIoctl(Sock, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),  
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
	if (nRet == SOCKET_ERROR)  
	{  
		return FALSE;  
	}

	return TRUE;
}