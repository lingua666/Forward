#ifndef  __CUSTOM_CMYINITSOCK_HEAD_DEF__
#define  __CUSTOM_CMYINITSOCK_HEAD_DEF__

#include "External.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )
	#include <net/if.h>
	#include <sys/ioctl.h> 
	#include <netdb.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/epoll.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>

	#define INVALID_SOCKET	(-1)
	#define SOCKET_ERROR	(-1)

	#define IS_INVALID_ZERO(lpPoint)    \
		if(lpPoint == NULL) { return 0; }

	#define IS_INVALID_NONE(lpPoint)    \
		if(lpPoint == NULL) { return ; }

	#define SOCKET_INVALID(socket)\
			{\
			   if(socket == INVALID_SOCKET) \
					return INVALID_SOCKET; \
			}

	#define SOCKET_INVALID_ZERO(socket)\
			{\
			if(socket == INVALID_SOCKET) \
				return 0; \
			}
	
	//uTimeOut: ��λ����
	inline bool CheckHostOnline(const _string_type& sIP, unsigned short uPort, unsigned int uTimeOut)
	{
		struct sockaddr_in clientsock_in = {0};
		timeval tm;
		fd_set set;
		bool ret = false;
		unsigned long ul = 1;
		int error = -1;
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)
		{	
			return ret;
		}
			
		ioctl(sockfd, FIONBIO, &ul); //����Ϊ������ģʽ  
	
		clientsock_in.sin_family = AF_INET;
		clientsock_in.sin_addr.s_addr = inet_addr(sIP.c_str());
		clientsock_in.sin_port = htons(uPort);
			
		if (connect(sockfd, (struct sockaddr *)&clientsock_in, sizeof(clientsock_in)) == -1)
		{
			tm.tv_sec = uTimeOut / 1000;
			tm.tv_usec = uTimeOut % 1000;
			FD_ZERO(&set);
			FD_SET(sockfd, &set);
			socklen_t len = sizeof(error);
			if (select(sockfd+1, NULL, &set, NULL, &tm) > 0)
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
	
		close(sockfd);
		return ret;
	}
	
	
	//uTimeOut: ��λ����
	typedef function20_handle<void(bool)>		HFNSocketResult;
	static void CheckHostOnline_V20_Impl(int sockfd, const HFNSocketResult& hCheck)
	{
			timeval tm;
			fd_set set;
			int error = -1;
			bool ret = false;
			
			tm.tv_sec = 0;
			tm.tv_usec = 0;
			
			FD_ZERO(&set);
			FD_SET(sockfd, &set);
			socklen_t len = sizeof(error);
			if (select(sockfd+1, NULL, &set, NULL, &tm) > 0)
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
			
			close(sockfd);
			
			if (hCheck)
				hCheck(ret);
	}

	inline int CheckHostOnline_V20(const _string_type& sIP, unsigned short uPort, unsigned int uTimeOut,
						TimerEvent* pTimerEv, const HFNSocketResult& hCheck)
	{
		struct sockaddr_in clientsock_in = {0};
	
		int iRet = -1;
		unsigned long ul = 1;
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)
		{	
			return iRet;
		}
			
		ioctl(sockfd, FIONBIO, &ul); //����Ϊ������ģʽ  
	
		clientsock_in.sin_family = AF_INET;
		clientsock_in.sin_addr.s_addr = inet_addr(sIP.c_str());
		clientsock_in.sin_port = htons(uPort);
			
		if (connect(sockfd, (struct sockaddr *)&clientsock_in, sizeof(clientsock_in)) == -1)
		{
			pTimerEv->push_back(function20_bind(&CheckHostOnline_V20_Impl, sockfd, hCheck), uTimeOut);
			iRet = 0;
		}
		else
		{
			iRet = 1;
			close(sockfd);
		}
	
		return iRet;
	}
	
	inline _string_type GetHostIP( void )
	{
		int  MAXINTERFACES = 16;
		char *ip = NULL;
		int fd, intrface, retn = 0;
		struct ifreq buf[MAXINTERFACES]; ///if.h
		struct ifconf ifc; ///if.h

		if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) //socket.h
		{
			ifc.ifc_len = sizeof buf;
			ifc.ifc_buf = (caddr_t) buf;
			if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) //ioctl.h
			{
				intrface = ifc.ifc_len / sizeof (struct ifreq);

				while( intrface-- > 0 )
				{
					if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
					{
						ip=(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));//types
						break;
					}
				}
			}
			close (fd);
		}
		return _string_type(ip);
	}

	inline _string_type GetIPByDomain( const _string_type& Domain, int* nCount )
	{
		struct hostent *he;
		he = gethostbyname(Domain.c_str());
		if (he)
		{
			struct       sockaddr_in   sAddr;

			*nCount = 0;
			while (he->h_addr_list[*nCount])
			{
				(*nCount)++;
			}

			if (*nCount > 0)
			{
				memcpy(&sAddr.sin_addr.s_addr, he->h_addr_list[0], he->h_length);
				return inet_ntoa(sAddr.sin_addr);
			}
		}

		return "";
	}

	namespace _epoll_net_{

		typedef	int			HANDLE;
		typedef UInt32		DWORD;
		#define	INFINITE	(-1)

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
				CLIENT_ENUM,	//�ͻ���
				SERVER_ENUM,	//��������
			};
		public:
			CMyInitSock(unsigned char minorVer = 2, unsigned char majorVer = 2)
			{
				//test
				LOG_Print_SetLog(libEPoll_Net, 0)
				//test
			}

			~CMyInitSock()
			{	
			
			}
		};

		static CMyInitSock	_CMyInitSock201512012254;

	} // _iocp_net_

#endif

#endif