
#include <libEPoll_Net/WSASocket.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	SOCKET APIWSACreate( int iSocketType, int iRevSize, int iSendSize, const char* c_szIP )
	{
		SOCKET sock = socket( AF_INET, iSocketType, 0);

		if ( sock == INVALID_SOCKET )
			return INVALID_SOCKET;

		//设置异步操作
		if( APIWSASetnonBlocking(sock) == INVALID_SOCKET )
		{
			APIWSAClose( sock );
			return INVALID_SOCKET;
		}

		int buflen = iSendSize;

		// 设置SOCKET的发送缓存
		if( buflen >= 0 )
		{
			if ( APIWSASetSockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char*)&buflen, sizeof(int) ) == SOCKET_ERROR )
			{
				APIWSAClose( sock );
				return INVALID_SOCKET;
			}
		}

		buflen = iRevSize;
		// 设置SOCKET的接收缓存
		if( buflen >= 0 )
		{
			if ( APIWSASetSockopt( sock, SOL_SOCKET,SO_RCVBUF, (const char*)&buflen,sizeof(int)) == SOCKET_ERROR )
			{
				APIWSAClose( sock );
				return INVALID_SOCKET;
			}
		}
		
		// 关闭 TCP 的 Nagle 算法
		if( SOCK_STREAM == iSocketType )
		{
			int opt = 1;
			if( APIWSASetSockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(opt)) == SOCKET_ERROR )
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
			struct linger liner = {1,0}; 
			if( APIWSASetSockopt(sock, SOL_SOCKET, SO_LINGER, 
				(char *)&liner, sizeof(liner)) == SOCKET_ERROR )
			{
				APIWSAClose( sock );
				return INVALID_SOCKET;
			}
		}

		if (c_szIP != NULL)
		{
			APIWSABind(sock, 0, c_szIP);
		}

		return sock;
	}

	int APIWSASetnonBlocking( SOCKET Sock )
	{
		int opts;
		if( Sock == INVALID_SOCKET )
			return - 1;

		opts = fcntl(Sock, F_GETFL);
		if ( opts < 0 )
		{
			perror("fcntl(sock,GETFL)");
			return SOCKET_ERROR;
		}

		opts = opts | O_NONBLOCK;
		if ( fcntl(Sock, F_SETFL, opts) < 0 )
		{
			perror("fcntl(sock,SETFL,opts)");
			return SOCKET_ERROR;
		}

		return 1;
	}
	
	int	APIWSAShutDown( SOCKET Sock )
	{
		if ( Sock == INVALID_SOCKET )
		{
			return SOCKET_ERROR;
		}
		shutdown( Sock, SD_BOTH );
	}

	int   APIWSAClose( SOCKET Sock )
	{
		if ( Sock == INVALID_SOCKET )
		{
			return SOCKET_ERROR;
		}

		shutdown( Sock, SD_BOTH );
		close( Sock );
		return 1;
	}

	int APIWSABind( SOCKET Sock, unsigned short usPort, const char* c_szIP  )
	{
		SOCKET_INVALID(Sock);

		int nRet = 0;
		struct sockaddr_in local = {0};
		if(usPort > 0)
			local.sin_port				= htons( usPort );

		if(c_szIP != NULL)
			local.sin_addr.s_addr	= inet_addr( c_szIP );
		else
			local.sin_addr.s_addr	= htonl(INADDR_ANY); 
		local.sin_family			= AF_INET;

		if( bind( Sock, (struct sockaddr*)&local, sizeof(local) ) < 0 )
		{
			return SOCKET_ERROR;
		}

		return 1;
	}

	int APIWSAListen( SOCKET Sock, unsigned short ListenNum )
	{
		SOCKET_INVALID(Sock);
		if( listen( Sock, ListenNum ) < 0 )
		{
			return SOCKET_ERROR;
		}

		return 1;
	}

	int APIWSAConnect( SOCKET Sock, const char* c_szIP, unsigned short usPort )
	{
		SOCKET_INVALID(Sock);

		struct sockaddr_in addr = {0};
		addr.sin_family				= AF_INET;
		addr.sin_addr.s_addr		= inet_addr( c_szIP );
		addr.sin_port				= htons( usPort );

		if( connect( Sock, (struct sockaddr*)&addr, sizeof(addr) ) < 0 )
		{
			return SOCKET_ERROR;
		}

		return 1;
	}

	SOCKET APIWSAAccept( SOCKET Sock, struct sockaddr_in* pAddr )
	{
		SOCKET_INVALID(Sock);

		socklen_t addrlen = sizeof(struct sockaddr_in);
		SOCKET s = accept(Sock, (struct sockaddr*)pAddr, &addrlen);
		if( s < 0 )
		{
			return SOCKET_ERROR;
		}

		return s;
	}

	int APIWSASend( SOCKET Sock, const char* c_szDataBuf, unsigned long ulDataSize )
	{
		SOCKET_INVALID(Sock);
		
		//如果尝试send到一个disconnected socket上，就会让底层抛出一个SIGPIPE信号，所以忽略该信号
		int iRet = send( Sock, c_szDataBuf, ulDataSize, MSG_DONTWAIT | MSG_NOSIGNAL ); 
		if( iRet < 0 )
		{
			return SOCKET_ERROR;
		}

		return iRet;
	}

	int APIWSARecv( SOCKET Sock, char* szDataBuf, unsigned long ulDataSize )
	{
		SOCKET_INVALID(Sock);

		int iRet = recv( Sock, szDataBuf, ulDataSize, MSG_DONTWAIT | MSG_NOSIGNAL);
		if( iRet < 0 )
		{
			return SOCKET_ERROR;
		}

		return iRet;
	}

	int g_nWSALen = sizeof(struct sockaddr_in);

	int APIWSASendto( SOCKET Sock, const char* c_szDataBuf,
		unsigned long ulDataSize, struct sockaddr_in* pAddr )
	{
		SOCKET_INVALID(Sock);
		int iRet = sendto( Sock, c_szDataBuf, ulDataSize, 0, (struct sockaddr*)pAddr, g_nWSALen);
		if( iRet < 0 ) 
		{
			return SOCKET_ERROR;
		}

		return iRet;
	}

	int APIWSARecvFrom( SOCKET Sock, char* szDataBuf,
		unsigned long ulDataSize, struct sockaddr_in* pAddr )
	{
		SOCKET_INVALID(Sock);

		socklen_t AddLen = sizeof(struct sockaddr_in);
		int iRet = recvfrom( Sock, szDataBuf, ulDataSize, 0, (struct sockaddr*)pAddr, &AddLen );
		if( iRet < 0 )
		{
			return SOCKET_ERROR;
		}

		return iRet;
	}

	int	APIWSASetSockopt( SOCKET Sock, int Level, int Optname, 
						const char* c_szOptval, int Optlen )
	{
		SOCKET_INVALID(Sock);
	
		//设置SOCKET参数
		int nRet = ::setsockopt( Sock, Level, Optname, c_szOptval, Optlen );
		if(nRet == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		return 1;
	}

	int	APIGetWSASocketLocalIP( SOCKET Sock, char* szIP, unsigned long uiSize )
	{
		if ( Sock == INVALID_SOCKET && szIP != NULL )
			return SOCKET_ERROR;

		struct sockaddr_in addr = {0};
		socklen_t len = sizeof(addr);
		if( getsockname( Sock, (struct sockaddr*)&addr, &len) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}
	
		char* IP = inet_ntoa(addr.sin_addr);
		len = __min(strlen(IP),uiSize);
		memcpy(szIP,IP,len);
		return 1;
	}

	int	APIGetWSASocketLocalPort( SOCKET Sock, unsigned short& usPort )
	{
		if ( Sock == INVALID_SOCKET )
		{
			return SOCKET_ERROR;
		}

		struct sockaddr_in addr = {0};
		socklen_t len = sizeof(addr);
		if( getsockname( Sock, (struct sockaddr*)&addr, &len) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		usPort = ntohs(addr.sin_port);
		return 1;
	}

	int	APIGetWSASocketRemoteIP( SOCKET Sock, char* szIP, unsigned short usSize )
	{
		if ( Sock == INVALID_SOCKET && szIP != NULL )
		{
			return SOCKET_ERROR;
		}

		struct sockaddr_in addr = {0};
		socklen_t len = sizeof(addr);
		if( getpeername( Sock, (struct sockaddr*)&addr, &len) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		char* IP = inet_ntoa(addr.sin_addr);
		len = __min(strlen(IP),usSize);
		memcpy(szIP,IP,len);

		return 1;
	}

	int	APIGetWSASocketRemotePort( SOCKET Sock, unsigned short& usPort )
	{
		if ( Sock == INVALID_SOCKET )
		{
			return SOCKET_ERROR;
		}

		struct sockaddr_in addr = {0};
		socklen_t len = sizeof(addr);
		if( getpeername( Sock, (struct sockaddr*)&addr, &len) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}
	
		usPort = ntohs(addr.sin_port);
		return 1;
	}

	int	APIWSASetKeepalive( SOCKET Sock, int iTime, int iTimerNum )
	{
		int val = 1;
		//开启keepalive机制
		if( APIWSASetSockopt(Sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(val)) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		/* Default settings are more or less garbage, with the keepalive time
		 * set to 7200 by default on Linux. Modify settings to make the feature
		 * actually useful. */

		/* Send first probe after interval. */
		val = iTime;
		if( APIWSASetSockopt(Sock, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&val, sizeof(val)) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		/* Send next probes after the specified interval. Note that we set the
		 * delay as interval / 3, as we send three probes before detecting
		 * an error (see the next setsockopt call). */
		val = iTime / iTimerNum;
		if (val == 0)
			val = 1;

		if ( APIWSASetSockopt(Sock, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&val, sizeof(val)) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		/* Consider the socket in error state after three we send three ACK
		 * probes without getting a reply. */
		val = iTimerNum;
		if( APIWSASetSockopt(Sock, IPPROTO_TCP, TCP_KEEPCNT, (char*)&val, sizeof(val)) == SOCKET_ERROR )
		{
			return SOCKET_ERROR;
		}

		return 1;
	}

#endif
