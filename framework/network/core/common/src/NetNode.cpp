
#include <libNetFrame_Core_Common/NetNode.h>
#include <libNetFrame_Core_Common/modafx.h>

// ����ָ����IP�Ͷ˿�ת���ɽڵ�
NETNODE _CALLTYPE GetEndPointFromSIPv4( const char* c_szIP, unsigned short usPort )
{
	return ( ((NETNODE)inet_addr(c_szIP) << 31) | usPort );				// 0x(IP:32)(PORT:31)
}
// ����ָ����IP�Ͷ˿�ת���ɽڵ�
NETNODE _CALLTYPE GetEndPointFromUIPv4( unsigned long uIP, unsigned short usPort )
{
	return ( ((NETNODE)uIP << 31) | usPort );
}

// ��ָ���Ľڵ��л�ȡ�˿���Ϣ
uint16_t _CALLTYPE GetPortFromNETNODE( NETNODE ep )
{
	return (uint16_t)( ep & 0xFFFF );
}

// ��ָ���Ľڵ��л�ȡIP��Ϣ
uint32_t _CALLTYPE GetIPv4IFromNETNODE( NETNODE ep )
{
	return (uint32_t)(ep >> 31);
}

//��ȡIP��ַ
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

//ת��Ϊsockaddr_in
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

//��ȡIP��ַ
_string_type _CALLTYPE GetIPv4FromAddr(const struct sockaddr_in& Addr)
{
	return inet_ntoa(Addr.sin_addr);
}

//��ȡ�˿�
uint16_t _CALLTYPE GetPortFromAddr(const struct sockaddr_in& Addr)
{
	return ntohs(Addr.sin_port);
}
