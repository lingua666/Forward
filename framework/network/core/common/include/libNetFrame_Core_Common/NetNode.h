
#ifndef __NETNODE_20150820161317_1440058397_H__
#define __NETNODE_20150820161317_1440058397_H__

#include <libFoundation/modafx.h>

typedef Int64	NETNODE;		// NET ENDPOINT
typedef Int64	NETHANDLE;

// ����ָ����IP�Ͷ˿�ת���ɽڵ�
NETNODE _CALLTYPE GetEndPointFromSIPv4( const char* c_szIP, unsigned short usPort );

// ����ָ����IP�Ͷ˿�ת���ɽڵ�
NETNODE _CALLTYPE GetEndPointFromUIPv4( unsigned long uIP, unsigned short usPort );

// ��ָ���Ľڵ��л�ȡ�˿���Ϣ
uint16_t _CALLTYPE GetPort( NETNODE ep );

// ��ָ���Ľڵ��л�ȡIP��Ϣ
uint32_t _CALLTYPE GetIPv4ToInt( NETNODE ep );

//��ȡIP��ַ
_string_type _CALLTYPE GetIPv4ToString( NETNODE ep );

class XorNetNode
{
	inline static	NETNODE	GetXor( NETNODE hPrev, NETNODE hNext )
	{
		return (NETNODE)( ((NETNODE)hPrev) ^ ((NETNODE)hNext));
	}

	inline static	NETNODE	GetXorToPrev( NETNODE hXor, NETNODE hNext )
	{
		return (NETNODE)( ((NETNODE)hXor) ^ ((NETNODE)hNext));
	}

	inline static	NETNODE	GetXorToNext( NETNODE hXor, NETNODE hPrev )
	{
		return (NETNODE)( ((NETNODE)hXor) ^ ((NETNODE)hPrev));
	}
};

#endif//__NETNODE_20150820161317_1440058397_H__
