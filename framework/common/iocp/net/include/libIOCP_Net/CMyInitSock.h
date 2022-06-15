#ifndef  __CUSTOM_CMYINITSOCK_HEAD_DEF__
#define  __CUSTOM_CMYINITSOCK_HEAD_DEF__

#include <libCommon/os/Platform.h>
#include <libCommon/os/winsock2i.h>
#include <libFunction_v20/modafx.h>
#include <libMessage/modafx.h>
#include <ws2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")	// 链接到WS2_32.lib
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#include <stdio.h>
#include <stdlib.h>

#define IS_INVALID_ZERO(lpPoint)    \
	if(lpPoint == NULL) { return 0; }

#define IS_INVALID_NONE(lpPoint)    \
	if(lpPoint == NULL) { return ; }

#define SOCKET_INVALID(socket)\
		{\
           if(socket == INVALID_SOCKET) \
				return INVALID_SOCKET; \
		}\

#define SOCKET_INVALID_ZERO(socket)\
		{\
		if(socket == INVALID_SOCKET) \
			return 0; \
		}\

//uTimeOut: 单位毫秒
inline bool CheckHostOnline(const _string_type& sIP, unsigned short uPort, unsigned int uTimeOut)
{
	SOCKADDR_IN clientsock_in = { 0 };
	timeval tm;
	fd_set set;
	unsigned long ul = 1;
	int error = -1;
	bool ret = false;
	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET)
		return ret;

	ioctlsocket(sockfd, FIONBIO, &ul); //设置为非阻塞模式  

	clientsock_in.sin_family = AF_INET;
	clientsock_in.sin_addr.S_un.S_addr = inet_addr(sIP.c_str());
	clientsock_in.sin_port = htons(uPort);

	if (connect(sockfd, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR)) == -1)
	{
		tm.tv_sec = uTimeOut / 1000;
		tm.tv_usec = uTimeOut % 1000;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		socklen_t len = sizeof(error);
		if (select(sockfd, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t *)&len);
			if (error == 0)
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = true;
	}

	::closesocket(sockfd);
	return ret;
}


//uTimeOut: 单位毫秒
typedef function20_handle<void(bool)>		HFNSocketResult;
static void CheckHostOnline_V20_Impl(SOCKET sockfd, const HFNSocketResult& hCheck)
{
	bool ret = false;
	timeval tm;
	fd_set set;
	int error = -1;

	tm.tv_sec = 0;
	tm.tv_usec = 0;

	FD_ZERO(&set);
	FD_SET(sockfd, &set);
	socklen_t len = sizeof(error);
	if (select(sockfd, NULL, &set, NULL, &tm) > 0)
	{
		getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t *)&len);
		if (error == 0)
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}

	::closesocket(sockfd);

	if (hCheck)
		hCheck(ret);
}

inline int CheckHostOnline_V20(const _string_type& sIP, unsigned short uPort, unsigned int uTimeOut,
						TimerEvent* pTimerEv, const HFNSocketResult& hCheck)
{
	SOCKADDR_IN clientsock_in = { 0 };
	fd_set set;
	unsigned long ul = 1;
	int iRet = -1;
	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET)
		return iRet;

	ioctlsocket(sockfd, FIONBIO, &ul); //设置为非阻塞模式  
	
	clientsock_in.sin_family = AF_INET;
	clientsock_in.sin_addr.S_un.S_addr = inet_addr(sIP.c_str());
	clientsock_in.sin_port = htons(uPort);
	
	if (connect(sockfd, (SOCKADDR*)&clientsock_in, sizeof(SOCKADDR)) == -1)
	{
		pTimerEv->push_back(function20_bind(&CheckHostOnline_V20_Impl, sockfd, hCheck), uTimeOut);
		iRet = 0;
	}
	else
	{
		iRet = 1;
		::closesocket(sockfd);
	}
	
	return iRet;
}


inline _string_type GetHostIP( void )
{
	char ip[20] = {0};
	struct hostent *phostinfo = gethostbyname("");
	char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
	strcpy(ip, p);  
	return _string_type(ip);
}

inline _string_type GetIPByDomain( const _string_type& Domain, int* nCount )
{
	struct hostent *he;
	he = gethostbyname(Domain.c_str());
	if ( he )
	{
		struct       sockaddr_in   sAddr;

		*nCount = 0;
		while ( he->h_addr_list[*nCount] )
		{
			(*nCount) ++;
		}

		if( *nCount > 0 )
		{
			memcpy(&sAddr.sin_addr.s_addr, he->h_addr_list[0], he->h_length);
			return inet_ntoa(sAddr.sin_addr);
		}
	}

	return "";
}

inline _string_type	GetMacFromIP(const _string_type& sDstIP)
{
	DWORD dwRetVal = 0;
	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;       /* default for src ip */
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhysAddrLen = 6;  /* default to length of six bytes */

	char *SrcIpString = NULL;
	BYTE *bPhysAddr = NULL;
	_string_type sDstMac;

	DestIp = inet_addr(sDstIP.c_str());
	memset(&MacAddr, 0xff, sizeof(MacAddr));
	dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhysAddrLen);

	if (dwRetVal == NO_ERROR) 
	{
		bPhysAddr = (BYTE *)& MacAddr;
		if (PhysAddrLen) 
		{
			for (unsigned int i = 0; i < (int)PhysAddrLen; i++) 
			{
				sDstMac += _string_type::HexToStr((char*)&bPhysAddr[i], 1);
			}
		}
		else
		{
			sDstMac = "000000000000";
		}

		return sDstMac;
	}
	else 
	{
		printf("Error: SendArp failed with error: %d", dwRetVal);
		switch (dwRetVal) 
		{
		case ERROR_GEN_FAILURE:
			break;
		case ERROR_INVALID_PARAMETER:
			break;
		case ERROR_INVALID_USER_BUFFER:
			break;
		case ERROR_BAD_NET_NAME:
			break;
		case ERROR_BUFFER_OVERFLOW:
			break;
		case ERROR_NOT_FOUND:
			break;
		default:
			break;
		}
	}

	return "";
}

inline bool Ping(const _string_type& sDst)
{
	return GetMacFromIP(sDst) != "" ? true : false;
}

namespace _iocp_net_{

	class CMyInitSock	
	{
	public:
		enum  SOCKTYPE_enum
		{
			SOCKET_TCP_TYPE = 1,
			SOCKET_UDP_TYPE,
		};

		enum
		{
			CLIENT_ENUM,	//客户端
			SERVER_ENUM,	//服务器端
		};
	public:
		CMyInitSock(unsigned char minorVer = 2, unsigned char majorVer = 2)
		{
			// 初始化WS2_32.dll
			WSADATA wsaData;
			unsigned short sockVersion = MAKEWORD(minorVer, majorVer);
			if(::WSAStartup(sockVersion, &wsaData) != 0)
			{
				MessageBoxA(NULL,"初始话SOCKET库失败","ERROR",MB_OK);
				exit(0);
			}
		}

		~CMyInitSock()
		{	
			::WSACleanup();	
		}
	};

	static CMyInitSock	_CMyInitSock201512012254;

} // _iocp_net_

#endif