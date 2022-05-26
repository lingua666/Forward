
#include <dllNetFrame_CmdServer/Exports.h>
#include <libNetFrame_CmdServer/CommandServer.h>
#include <libFoundation/modafx.h>

#include <iostream>

long g_AcceptNum = 0;

/*void _CALLTYPE RecvBackFun( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	MCmd_S_SentData(Node, pData, u32Size);
}

void _CALLTYPE CloseBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	if( g_AcceptNum  <= 0 )
	{
		int i = 0;
	}

	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum,c_szIP, uPort);
}

void _CALLTYPE AcceptBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	MCmd_S_SetClientRecData(Node, RecvBackFun);
	MCmd_S_SetClientDisConnect(Node, CloseBackFun);
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

/ *
_string_type GetHostIP( void )
{
	char ip[20] = {0};
	struct hostent *phostinfo = gethostbyname("");
	char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
	strcpy(ip, p);  
	return _string_type(ip);
}* /


int main(int argc, char* argv[])
{
	char szPort[10] = {0};
	UInt16 uPort = 8080;
	while(true)
	{
		if( MCmd_S_Listen(uPort, AcceptBackFun) )
		{
			printf("=====================================================\r\n");
			printf("Command Serever is Runing,\r\nHost:%s, "
				"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),uPort, get_processor_number() * 2 + 2);
			printf("=====================================================\r\n");
		}

		

		std::cin.get();
		MCmd_S_Free();
		MCmd_S_Close();
	};
}*/


_server_::CommandServer*	GetServer( void )
{
	static	_server_::CommandServer	Server;
	return &Server;
}

void RecvBackFun( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	//printf("Recv Data(%d):%s\r\n",u32Size, pData);
	GetServer()->Send(Node, pData, u32Size);
}

void CloseBackFun( NETHANDLE Node )
{
	if( g_AcceptNum  <= 0 )
	{
		int i = 0;
	}

	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum);
}

void AcceptBackFun(const char* c_szIP, UInt16 uPort, NETHANDLE Node)
{
	GetServer()->SetRecvHandle(Node, function20_bind_Ex(RecvBackFun));
	GetServer()->SetCloseHandle(Node, function20_bind_Ex(CloseBackFun));
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

/*_string_type GetHostIP( void )
{
	char ip[20] = {0};
	struct hostent *phostinfo = gethostbyname("");  
	char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
	strcpy(ip, p);  
	return _string_type(ip);
}*/

int main(int argc, char* argv[])
{
	char* cs_IP = "192.168.1.246";
	int iPort = 60000;

	if( GetServer()->Listen(iPort, function20_bind_Ex(AcceptBackFun), cs_IP) )
	{
		int iThreadNum = get_processor_number() * 2 + 2;
		GetServer()->Run( iThreadNum );
		printf("=====================================================\r\n");
		printf("Command Serever is Runing,\r\nHost:%s, "
			"Listen Port:%d\r\n%d Thread is Runing\r\n", cs_IP, iPort, iThreadNum);
		printf("=====================================================\r\n");
	}

	while(true)
	{
		
	};
}