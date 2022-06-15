#ifndef  __CUSTOM_CMYINITSOCK_HEAD_DEF__
#define  __CUSTOM_CMYINITSOCK_HEAD_DEF__

#include "External.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )
	#include <string.h>
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
	#include <net/ethernet.h>
	#include <netpacket/packet.h>
	#include <netinet/if_ether.h>

	#define INVALID_SOCKET	(-1)
	#define SOCKET_ERROR	(-1)
	#define ARP_PACKET_LEN	sizeof(struct ether_header) + sizeof(struct ether_arp)

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
	
	//uTimeOut: 单位毫秒
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
			
		ioctl(sockfd, FIONBIO, &ul); //设置为非阻塞模式  
	
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
	
	
	//uTimeOut: 单位毫秒
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
			
		ioctl(sockfd, FIONBIO, &ul); //设置为非阻塞模式  
	
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

	inline _string_type	GetMacFromIP(const _string_type& sDstIP)
	{
		int sock = -1;
		char packet[ARP_PACKET_LEN] = {0};
		struct in_addr targetIP, srcIP;
		struct sockaddr_ll device;
		struct ether_header * eth = (struct ether_header *) packet;
		struct ether_arp * arp = (struct ether_arp *) (packet + sizeof(struct ether_header));

		_string_type sDstMac;

		//Socket open
		sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
		if (sock < 0)
		{
			perror("socket");
			return "";
		}

		struct timeval tv_out;
		tv_out.tv_sec = 5;//等待5秒
		tv_out.tv_usec = 0;
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)) == -1)
		{
			perror("setsockopt SO_RCVTIMEO");
			close(sock);
			return "";
		}

		//搜索网卡
		char NetBuffer[1024] = { 0 };
		struct ifconf ifc;
		//初始化ifconf
		ifc.ifc_len = 1024;
		ifc.ifc_buf = NetBuffer;

		if (ioctl(sock, SIOCGIFCONF, (char *)&ifc) < 0)
		{
			perror("ioctl-conf:");
			close(sock);
			return "";
		}

		_string_type sfr_name;
		struct ifreq * ifr = (struct ifreq*)NetBuffer;
		for (int i = 0; i < ifc.ifc_len / sizeof(struct ifreq); i++)
		{
			//网卡flags值
			if (ioctl(sock, SIOCGIFFLAGS, ifr) >= 0)
			{
				if (IFF_UP & ifr->ifr_flags)
				{//启动
					if (IFF_RUNNING & ifr->ifr_flags && IFF_BROADCAST & ifr->ifr_flags)
					{
						//网卡名
						sfr_name = ifr->ifr_name;
						break;
					}
				}
				else /*if(IFF_DOWN & ifr->ifr_flags)*/
				{//禁用
					
				}
			}
			else
			{
				perror("ioctl SIOCGIFFLAGS error");
			}

			ifr++;
		}

		//get my interface mac address
		struct ifreq req;
		memset(&req, 0, sizeof(req));

		//选择网卡
		strncpy(req.ifr_name, sfr_name.c_str(), IF_NAMESIZE - 1);
		if (ioctl(sock, SIOCGIFHWADDR, &req) < 0)
		{
			perror("ioctl SIOCGIFHWADDR");
			close(sock);
			return "";
		}
		
		for (int i = 0; i < 6; i++)
		{
			arp->arp_sha[i] = req.ifr_hwaddr.sa_data[i];
		}

		//获取接口IP地址
		if (ioctl(sock, SIOCGIFADDR, &req) < 0)
		{
			perror("ioctl SIOCGIFADDR");
			close(sock);
			return "";
		}
		srcIP.s_addr = ((struct sockaddr_in*)&(req.ifr_addr))->sin_addr.s_addr;
		//printf("IP addr:%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));

		//Source Protocol Address : ARP Packet
		/*sscanf(argv[2], "%d.%d.%d.%d", (int *)&arp->arp_spa[0],
			(int *)&arp->arp_spa[1],
			(int *)&arp->arp_spa[2],
			(int *)&arp->arp_spa[3]);

		//Target Protocol Address : ARP Packet
		sscanf(argv[3], "%d.%d.%d.%d.", (int *)&arp->arp_tpa[0],
			(int *)&arp->arp_tpa[1],
			(int *)&arp->arp_tpa[2],
			(int *)&arp->arp_tpa[3]);*/

		//Ethernet Packet
		memset(eth->ether_dhost, 0xff, ETH_ALEN); //destination address
		memcpy(eth->ether_shost, arp->arp_sha, ETH_ALEN); //source address
		eth->ether_type = htons(ETH_P_ARP);    //type
											   //ARP Packet
		arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);  //Format of hardware address
		arp->ea_hdr.ar_pro = htons(ETH_P_IP);   //Format of protocol address.
		arp->ea_hdr.ar_hln = ETH_ALEN;    //Length of hardware address.
		arp->ea_hdr.ar_pln = 4;      //Length of protocol address.
		arp->ea_hdr.ar_op = htons(ARPOP_REQUEST);  //ARP operation : REQUEST

		//填充发送端的MAC和IP地址
		memcpy(arp->arp_sha, arp->arp_sha, ETH_ALEN);	//发送端的MAC
		memcpy(&arp->arp_spa, &srcIP, 4);				//发送端IP地址

		//填充目的端的IP地址，MAC地址不用管
		inet_pton(AF_INET, sDstIP.c_str(), &targetIP);
		memset(&arp->arp_tha, 0x00, ETHER_ADDR_LEN);	//目的端的MAC地址
		memcpy(arp->arp_tpa, &targetIP, 4);				//目的端的IP地址

		memset(&device, 0, sizeof(device));
		device.sll_ifindex = req.ifr_ifindex;// if_nametoindex(argv[1]);  //Interface number 
		device.sll_family = AF_PACKET;
		memcpy(device.sll_addr, arp->arp_sha, ETH_ALEN); //Physical layer address
		device.sll_halen = htons(ETH_ALEN);    //Length of address
		sendto(sock, packet, ARP_PACKET_LEN, 0, (struct sockaddr *) &device, sizeof(device));

		//test
		/*for (int i = 0; i < ARP_PACKET_LEN; i++)
		{
			printf("%02x", (unsigned char)packet[i]);
		}
		printf("\r\n");*/
		//test

		struct sockaddr_ll fromaddr;
		fromaddr.sll_family = PF_PACKET;
		fromaddr.sll_protocol = htons(ETH_P_ARP);
		fromaddr.sll_hatype = ARPHRD_ETHER;
		fromaddr.sll_pkttype = PACKET_HOST;
		fromaddr.sll_halen = ETH_ALEN;
		memcpy(fromaddr.sll_addr, arp->arp_sha, ETH_ALEN);
		bind(sock, (struct sockaddr*)&fromaddr, sizeof(struct sockaddr));

		char buffer[ARP_PACKET_LEN] = { 0 };
		Timestamp_type	Time;
		int iStep = 2;

		do
		{
			memset(buffer, 0, ARP_PACKET_LEN);
			int nr = recvfrom(sock, buffer, ARP_PACKET_LEN, 0, NULL, NULL);
			if (nr > 0)
			{
				UInt64 uValue = Timestamp_type().epochMicroseconds() - Time.epochMicroseconds();

				if (uValue > 5000000)
				{//5秒
					//printf("TimeOut\r\n");
					break;
				}

				if (uValue / 1000000 == iStep)
				{
					sendto(sock, packet, ARP_PACKET_LEN, 0, (struct sockaddr *) &device, sizeof(device));
					iStep += 2;
				}

				struct ethhdr *hdr = (struct ethhdr*)buffer;
				struct ether_arp * recv_arp = (struct ether_arp*)(buffer + 14);

				//test
				/*printf("recvfrom n:%d\r\n", nr);

				for (int i = 0; i < nr; i++)
				{
					printf("%02x", (unsigned char)buffer[i]);
				}
				printf("\r\n");

				printf("arp->arp_tpa:");
				for (int i = 0; i < sizeof(arp->arp_tpa); i++)
				{
					printf("%02x", (unsigned char)arp->arp_tpa[i]);
				}
				printf("\r\n");

				printf("recv_arp->arp_spa:");
				for (int i = 0; i < sizeof(recv_arp->arp_spa); i++)
				{
					printf("%02x", (unsigned char)recv_arp->arp_spa[i]);
				}
				printf("\r\n");*/
				//test

				if (memcmp(arp->arp_tpa, recv_arp->arp_spa, sizeof(arp->arp_tpa)) == 0)
				{
					for (int i = 0; i < ETH_ALEN; i++)
					{
						sDstMac += _string_type::HexToStr((char*)&hdr->h_source[i], 1);
					}

					//test
					//printf("sDstMac:%s\r\n", sDstMac.c_str());
					//test

					break;
				}
			}
			else
			{
				if (nr < 0)
				{
					if (errno == EWOULDBLOCK)
					{// 超时
						//printf("EWOULDBLOCK 超时\r\n");
					}
				}
			}

		} while (true);
		
		close(sock);
		sock = -1;
		return sDstMac;
	}

	inline bool Ping(const _string_type& sDst)
	{
		return GetMacFromIP(sDst) != "" ? true : false;
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
				CLIENT_ENUM,	//客户端
				SERVER_ENUM,	//服务器端
			};
		public:
			CMyInitSock(unsigned char minorVer = 2, unsigned char majorVer = 2)
			{}

			~CMyInitSock()
			{}
		};

		static CMyInitSock	_CMyInitSock201512012254;

	} // _iocp_net_

#endif

#endif