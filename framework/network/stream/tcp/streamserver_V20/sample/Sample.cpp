// MediaServerSample.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libNetFrame_StreamServer_V20/StreamServer_V20.h>

typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;

long g_AcceptNum = 0;
_server_::_tcp_::StreamServer_V20	g_Server;

void RecvBackFun( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr, long lRemain )
{
	//printf("Recv Data(%d):%s\r\n",Buf_ptr->payload, Buf_ptr->data);
	printf("Recv Data(%d):%s\r\n", Buf_ptr->payload, _string_type::HexToStr(Buf_ptr->data, Buf_ptr->payload).c_str());
}

void CloseBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

void AcceptBackFun( NETHANDLE hListen, const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	g_Server.SetRecvHandle(Node, function20_bind_Ex(RecvBackFun));
	g_Server.SetCloseHandle(Node, function20_bind(CloseBackFun, c_szIP, uPort,
												_function_::_1));
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#include <signal.h>
void ExitHande(int n)
{ 
	if( n == SIGINT ) 
	{
		printf("\n你按下了ctrl+c");
		signal(SIGINT, ExitHande);
		g_Server.Release();
		return;
	} 

	if( n==SIGBREAK )
	{
		printf("\n你按下了ctrl+break, 或点击了关闭按钮，现在退出程序\n");
		g_Server.Release();
		exit(0);
	}
}
#endif

int main(int argc, char* argv[])
{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	signal(SIGINT, ExitHande);  /*注册ctrl+c信号捕获函数*/
	signal(SIGBREAK, ExitHande); /*注册ctrl+break信号捕获函数*/
#endif

	//g_Server.Init(3);

	_string_type s = _file_::RWIni::GetCurrentPath();
	s.append("\\Config\\Stream_Config.ini");
	char* c_szIP = "192.168.1.58";
	const int iPort = 21000;//_file_::RWIni::ReadInt("SERVER","Port",s.c_str());

	g_Server.Init();

	while (true)
	{
		printf("Run\r\n");
		NETHANDLE hNet = g_Server.Listen(iPort, function20_bind_Ex(AcceptBackFun), c_szIP);
		if (hNet != -1)
		{
			int iThreadNum = get_processor_number();
			//g_Server.Run( __max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2) );
			printf("=====================================================\r\n");
			printf("Server is Runing,\r\nHost:%s, "
				"Listen Port:%d\r\n%d Thread is Runing\r\n", c_szIP, iPort, iThreadNum);
			printf("=====================================================\r\n");
		}
		Sleep(1000000000);
		
		g_Server.Stop(hNet);
		printf("Stop\r\n");
		Sleep(1000);
	}

	while(true)
	{
		Sleep(10);
	};
}