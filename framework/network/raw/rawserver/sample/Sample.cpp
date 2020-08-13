
#include <libNetFrame_RawServer/RawServer.h>
#include <libFoundation/modafx.h>

#include <iostream>

long g_AcceptNum = 0;

_server_::RawServer*	GetServer( void )
{
	static	_server_::RawServer	Server;
	return &Server;
}

void RecvBackFun( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	GetServer()->Send(Node, pData, u32Size);
}

void CloseBackFun( const _string_type& sIP, UInt16 uPort, NETHANDLE Node )
{
	if( g_AcceptNum  <= 0 )
	{
		int i = 0;
	}

	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum,sIP.c_str(), uPort);
}

void AcceptBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	GetServer()->SetRecvHandle(Node, function20_bind_Ex(RecvBackFun));
	GetServer()->SetCloseHandle(Node, function20_bind(CloseBackFun, GetIPv4ToString(Node), GetPort(Node), _function_::_1));
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

/*
_string_type GetHostIP( void )
{
	char ip[20] = {0};
	struct hostent *phostinfo = gethostbyname("");  
	char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
	strcpy(ip, p);  
	return _string_type(ip);
}*/

int main(int argc, char* argv[])
{
	int iPort = 60000;
	if( GetServer()->Listen(iPort, function20_bind_Ex(AcceptBackFun)) )
	{
		int iThreadNum = get_processor_number() * 2 + 2;
		GetServer()->Run( iThreadNum );
		printf("=====================================================\r\n");
		printf("Command Serever is Runing,\r\nHost:%s, "
			"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),iPort, iThreadNum);
		printf("=====================================================\r\n");
	}

	while(true)
	{
		
	};
}