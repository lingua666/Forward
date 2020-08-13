
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
uint16_t _CALLTYPE GetPort( NETNODE ep )
{
	return (uint16_t)( ep & 0xFFFF );
}

// ��ָ���Ľڵ��л�ȡIP��Ϣ
uint32_t _CALLTYPE GetIPv4ToInt( NETNODE ep )
{
	return (uint32_t)(ep >> 31);
}

//��ȡIP��ַ
_string_type _CALLTYPE GetIPv4ToString( NETNODE ep )
{
	in_addr InAddr = {0};

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	InAddr.S_un.S_addr = GetIPv4ToInt(ep);
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	InAddr.s_addr = GetIPv4ToInt(ep);
#endif

	return inet_ntoa(InAddr);
}
