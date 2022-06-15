
#include <libNetFrame_Core_Common/NetNode.h>
#include <libNetFrame_Core_Common/modafx.h>

// 根据指定的IP和端口转换成节点
NETNODE _CALLTYPE GetEndPointFromSIPv4( const char* c_szIP, unsigned short usPort )
{
	return ( ((NETNODE)inet_addr(c_szIP) << 31) | usPort );				// 0x(IP:32)(PORT:31)
}
// 根据指定的IP和端口转换成节点
NETNODE _CALLTYPE GetEndPointFromUIPv4( unsigned long uIP, unsigned short usPort )
{
	return ( ((NETNODE)uIP << 31) | usPort );
}

// 从指定的节点中获取端口信息
uint16_t _CALLTYPE GetPortFromNETNODE( NETNODE ep )
{
	return (uint16_t)( ep & 0xFFFF );
}

// 从指定的节点中获取IP信息
uint32_t _CALLTYPE GetIPv4IFromNETNODE( NETNODE ep )
{
	return (uint32_t)(ep >> 31);
}

//获取IP地址
_string_type _CALLTYPE GetIPv4SFromNETNODE( NETNODE ep )
{
	in_addr InAddr = {0};

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	InAddr.S_un.S_addr = GetIPv4IFromNETNODE(ep);
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	InAddr.s_addr = GetIPv4IFromNETNODE(ep);
#endif

	return inet_ntoa(InAddr);
}

//转换为sockaddr_in
int _CALLTYPE GetSockAddrInFormIPPort(const char* c_szIP, UInt16 uPort, struct sockaddr_in* pAddr)
{
	if (pAddr == NULL)
		return -1;

	struct sockaddr_in Addr = { 0 };
	Addr.sin_port = htons(uPort);
	Addr.sin_addr.s_addr = inet_addr(c_szIP);
	Addr.sin_family = AF_INET;
	memcpy(pAddr, &Addr, sizeof(struct sockaddr_in));
	return 1;
}

//获取IP地址
_string_type _CALLTYPE GetIPv4FromAddr(const struct sockaddr_in& Addr)
{
	return inet_ntoa(Addr.sin_addr);
}

//获取端口
uint16_t _CALLTYPE GetPortFromAddr(const struct sockaddr_in& Addr)
{
	return ntohs(Addr.sin_port);
}
