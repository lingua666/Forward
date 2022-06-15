
#include <dllNetFrame_CmdServer_V10/Exports.h>
#include <libNetFrame_CmdServer_V10/CommandServer_V10.h>
#include <libFoundation/modafx.h>

#include <iostream>

long g_AcceptNum = 0;

void _CALLTYPE RecvBackFun( NETHANDLE Node, const char* pData, int u32Size )
{
	MCmd_SV10_SentData(Node, pData, u32Size);
}

void _CALLTYPE CloseBackFun( const char* c_szIP, int uPort, NETHANDLE Node )
{
	if( g_AcceptNum  <= 0 )
	{
		int i = 0;
	}

	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

void _CALLTYPE AcceptBackFun( const char* c_szIP, int uPort, NETHANDLE Node )
{
	MCmd_SV10_SetClientRecData(Node, RecvBackFun);
	MCmd_SV10_SetClientDisConnect(Node, CloseBackFun);
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

struct testobj
{

	UInt32 i;
};

struct testobj1
{

};

int main(int argc, char* argv[])
{
	//UInt16 uPort = 18000;	

	_string_type s = _file_::RWIni::GetCurrentPath();
	s += _file_::CMyDirectory::GetSplit();
	s += "Config";
	s += _file_::CMyDirectory::GetSplit();
	s += "Config.ini";
	UInt16 uPort = 60000;// _file_::RWIni::ReadInt("SERVER", "Port", s.c_str());

	MCmd_SV10_Init(3);
	while(true)
	{
		if( MCmd_SV10_Listen(uPort, AcceptBackFun) )
		{
			printf("=====================================================\r\n");
			printf("Command Server is Runing,\r\nHost:%s, "
				"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),uPort, get_processor_number() * 2 + 2);
			printf("=====================================================\r\n");
		}

		std::cin.get();
		MCmd_SV10_Close();
	};
}

/*
_server_::CmdServer*	GetServer( void )
{
	static	_server_::CmdServer	Server;
	return &Server;
}

void RecvBackFun( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	//printf("Recv Data(%d):%s\r\n",u32Size, pData);
	GetServer()->Send(Node, pData, u32Size);
}

void CloseBackFun( NETHANDLE Node, const char* c_szIP, UInt16 uPort )
{
	if( g_AcceptNum  <= 0 )
	{
		int i = 0;
	}

	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum,c_szIP, uPort);
}

void AcceptBackFun( NETHANDLE Node, const char* c_szIP, UInt16 uPort )
{
	GetServer()->SetRecvHandle(Node, function20_bind_Ex(RecvBackFun));
	GetServer()->SetCloseHandle(Node, function20_bind_Ex(CloseBackFun));
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

_string_type GetHostIP( void )
{
	char ip[20] = {0};
	struct hostent *phostinfo = gethostbyname("");  
	char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
	strcpy(ip, p);  
	return _string_type(ip);
}

int main(int argc, char* argv[])
{
	if( GetServer()->Listen(8080, function20_bind_Ex(AcceptBackFun)) )
	{
		int iThreadNum = get_processor_number() * 2 + 2;
		GetServer()->Run( iThreadNum );
		printf("=====================================================\r\n");
		printf("Command Serever is Runing,\r\nHost:%s, "
			"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),8080, iThreadNum);
		printf("=====================================================\r\n");
	}

	while(true)
	{
		
	};
}*/