// MediaServerSample.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libNetFrame_UDP_Stream/AppMain.h>
#include <dllNetFrame_UDP_Stream/Exports.h>

_server_::_udp_::AppMain* g_pApp = NULL;
long g_AcceptNum = 0;

void RecvBackFun( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr )
{
	/*printf("Recv Data(%d):%s\r\n",StreamBuf::GetSize(Buf_ptr.get()),
		StreamBuf::GetData(Buf_ptr.get()));*/

	//printf("Recv Data(%d):%s\r\n",u32Size, pData);
	//GetServer()->Post(boost::bind(PostRecv, Node, Buf_ptr));
	//GetServer()->Send(Node, pData, u32Size);

	//printf("Send Data: %d\r\n",StreamBuf::GetSize(Buf_ptr.get()));
	if( Buf_ptr->payload > 0 )
	{
		g_pApp->Send(Node, pSrc, Buf_ptr->data, Buf_ptr->payload);

		Buf_ptr->data[Buf_ptr->payload] = 0;
	}
	printf("recv data(%d):%s\r\n", Buf_ptr->payload, Buf_ptr->data);

	/*g_Packet.Parse(Buf_ptr->data, Buf_ptr->payload);*/
}

void CloseBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#include <signal.h>
void ExitHande(int n)
{ 
	if( n == SIGINT ) 
	{
		printf("\n你按下了ctrl+c");
		signal(SIGINT, ExitHande);
		g_pApp->Release();
		return;
	} 

	if( n==SIGBREAK )
	{
		printf("\n你按下了ctrl+break, 或点击了关闭按钮，现在退出程序\n");
		g_pApp->Release();
		exit(0);
	}
}
#endif

void DllTest( void )
{
	MEmergency_Telephone_Init(60000, "192.168.8.43", 10000);
	MEmergency_Telephone_Call();
	Sleep(1000);
	MEmergency_Telephone_Hangup();
	std::cin.get();
}

void ClientTest( void )
{
	_server_::_udp_::AppMain	App;
	g_pApp = &App;

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	signal(SIGINT, ExitHande);  /*注册ctrl+c信号捕获函数*/
	signal(SIGBREAK, ExitHande); /*注册ctrl+break信号捕获函数*/
#endif

	_string_type s = _file_::RWIni::GetCurrentPath();
	s.append("\\Config\\Stream_Config.ini");
	const int iPort = 10000;//_file_::RWIni::ReadInt("SERVER","Port",s.c_str());

	int iThreadNum = get_processor_number();
	App.Init(/*__max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2)*/);

	while(true)
	{
		NETHANDLE Node =  App.Open(iPort);
		if( Node != -1 )
		{
			App.SetRecvHandle(Node, RecvBackFun);
			App.SetDestroyHandle(Node,  function20_bind(CloseBackFun, GetHostIP().c_str(), iPort, _function_::_1));
			printf("=====================================================\r\n");
			printf("Server is Runing,\r\nHost:%s, "
				"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),iPort, iThreadNum);
			printf("=====================================================\r\n");

			struct sockaddr_in g_DestAddr201801191632 = {0};
			g_DestAddr201801191632.sin_port			= htons( 60000 );
			g_DestAddr201801191632.sin_addr.s_addr	= inet_addr( "192.168.8.43" );
			g_DestAddr201801191632.sin_family			= AF_INET;
			char czD[1024] = {0};
			memset(czD, '1', sizeof(czD));
			App.Send(Node, &g_DestAddr201801191632, czD, sizeof(czD));

			srand(GetTickCount());
			Sleep(rand() % 3000);
			Sleep(10000000);
			App.Close(Node);
		}
	}

	std::cin.get();
}

void ServerTest( void )
{
	_server_::_udp_::AppMain	App;
	g_pApp = &App;

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	signal(SIGINT, ExitHande);  /*注册ctrl+c信号捕获函数*/
	signal(SIGBREAK, ExitHande); /*注册ctrl+break信号捕获函数*/
#endif

	_string_type s = _file_::RWIni::GetCurrentPath();
	s.append("\\Config\\Stream_Config.ini");
	const int iPort = 60000;//_file_::RWIni::ReadInt("SERVER","Port",s.c_str());

	int iThreadNum = get_processor_number();
	App.Init(__max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2));

	NETHANDLE Node =  App.Open(iPort);
	if( Node != -1 )
	{
		App.SetRecvHandle(Node, RecvBackFun);
		App.SetDestroyHandle(Node,  function20_bind(CloseBackFun, GetHostIP().c_str(), iPort, _function_::_1));
		printf("=====================================================\r\n");
		printf("Server is Runing,\r\nHost:%s, "
			"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),iPort, iThreadNum);
		printf("=====================================================\r\n");
	}

	std::cin.get();
}

int main(int argc, char* argv[])
{
	//DllTest();
	ClientTest();
	//ServerTest();

	while(true)
	{
		Sleep(10);
	};
}