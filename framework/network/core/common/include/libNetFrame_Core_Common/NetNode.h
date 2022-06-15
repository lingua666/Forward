
#ifndef __NETNODE_20150820161317_1440058397_H__
#define __NETNODE_20150820161317_1440058397_H__

#include <libFoundation/modafx.h>

typedef Int64	NETNODE;		// NET ENDPOINT
typedef Int64	NETHANDLE;

// 根据指定的IP和端口转换成节点
NETNODE _CALLTYPE GetEndPointFromSIPv4( const char* c_szIP, unsigned short usPort );

// 根据指定的IP和端口转换成节点
NETNODE _CALLTYPE GetEndPointFromUIPv4( unsigned long uIP, unsigned short usPort );

// 从指定的节点中获取端口信息
uint16_t _CALLTYPE GetPort( NETNODE ep );

// 从指定的节点中获取IP信息
uint32_t _CALLTYPE GetIPv4ToInt( NETNODE ep );

//获取IP地址
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
