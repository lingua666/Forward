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
	/*if( Buf_ptr->payload > 0 )
	{
		g_pApp->Send(Node, pSrc, Buf_ptr->data, Buf_ptr->payload);

		Buf_ptr->data[Buf_ptr->payload] = 0;
	}
	printf("recv data(%d):%s\r\n", Buf_ptr->payload, Buf_ptr->data);*/

	//printf("recv data(%d)\r\n", Buf_ptr->payload);

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
	const int iPort = 11000;//_file_::RWIni::ReadInt("SERVER","Port",s.c_str());

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

			/*struct sockaddr_in g_DestAddr201801191632 = {0};
			g_DestAddr201801191632.sin_port			= htons( 60000 );
			g_DestAddr201801191632.sin_addr.s_addr	= inet_addr( "192.168.8.43" );
			g_DestAddr201801191632.sin_family			= AF_INET;
			char czD[1024] = {0};
			memset(czD, '1', sizeof(czD));
			App.Send(Node, &g_DestAddr201801191632, czD, sizeof(czD));

			srand(GetTickCount());
			Sleep(rand() % 3000);*/
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

void _CALLTYPE DllRecvBackFun(void* pUser, NETHANDLE Node, struct sockaddr_in* pSrc, const char* c_szData, UInt32 u32Size)
{
	MUdpStream_SentData(Node, pSrc, c_szData, u32Size);
}

void _CALLTYPE DllCloseBackFun(void* pUser, UInt16 uPort, NETHANDLE Node)
{
	printf("Client Close(%d)\r\n", uPort);
}

void DllClientTest(void)
{
	MUdpStream_Init(0, 0, true);

	int Port = 60000;
	NETHANDLE Node = MUdpStream_Open(Port);
	if (Node != -1)
	{
		MUdpStream_SetRecData(Node, DllRecvBackFun, NULL);
		MUdpStream_SetClose(Node, DllCloseBackFun, NULL);

		/*struct sockaddr_in g_DestAddr201801191632 = {0};
		g_DestAddr201801191632.sin_port			= htons( 60000 );
		g_DestAddr201801191632.sin_addr.s_addr	= inet_addr( "192.168.8.43" );
		g_DestAddr201801191632.sin_family			= AF_INET;
		char czD[1024] = {0};
		memset(czD, '1', sizeof(czD));
		App.Send(Node, &g_DestAddr201801191632, czD, sizeof(czD));

		srand(GetTickCount());
		Sleep(rand() % 3000);*/
		Sleep(10000000);
		MUdpStream_Close(Node);
	}

	MUdpStream_Free();
}


#define Performance_START	60001
#define Performance_COUNT	5000

void PerformanceTestClient(void)
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

	NETHANDLE Nodes[10000] = { 0 };
	memset(Nodes, 0, sizeof(Nodes));

	_server_::_udp_::StreamSession_sptr Session_sptr[10000];

	struct sockaddr_in DstAddr[10000] = {0};
	std::map<NETHANDLE, int>	NetMap;

	//打开
	int iCount = Performance_COUNT;
	int iStart = 50001;
	int iDstStart = Performance_START;
	for (int i = 0; i < iCount; i++)
	{
		Nodes[i] = App.Open(iStart + i);
		if (Nodes[i] != -1)
		{
			NetMap.insert(std::make_pair(Nodes[i], 0));
			App.SetRecvHandle(Nodes[i], RecvBackFun);
			App.SetDestroyHandle(Nodes[i], function20_bind(CloseBackFun, GetHostIP().c_str(), iPort, _function_::_1));
			printf("Open Port:%d Success\r\n", iStart + i);

			//Session_sptr[i] = App.FindSession(Nodes[i]);

			DstAddr[i].sin_port = htons(iDstStart + i);
			DstAddr[i].sin_addr.s_addr = inet_addr("192.168.1.58");
			DstAddr[i].sin_family = AF_INET;
		}
		else
		{
			printf("Open Port:%d Failed!\r\n", iStart + i);
		}
	}

	CLock Lock;

	Timestamp_type	Tm;
	Timestamp_type	time;

	char* szData = { "From: <sip:D01022011271002@202.105.12.106:40001>; tag=65ec4bd7-f4fb-4c63-b743-2ca08bae98aaTo: <sip:D01022011271002@202.105.12.106:40001>From: <sip:D01022011271001@202.105.12.106:40001>; tag=32e1b0b6-aeac-4cbd-a380-41e38f4acda5" };
	int iLen = strlen(szData);

	int iSendCount = 0;

	//发送
	/*while (true)
	{
		if (time.isElapsed(1000000))
		{
			printf("elapsed(%d) :%lld\r\n", iCount, Timestamp_type().epochMicroseconds() - time.epochMicroseconds());
			iSendCount = 0;
			Sleep(1000);
			time.update();
		}
		
		Tm.update();
		Lock.Lock();
		for (int i = 0; i < iCount; i++)
		{
			if (Nodes[i] != -1)
			{
				App.Send(Nodes[i], &DstAddr[i], szData, iLen, 10);
				//Session_sptr[0]->Send(&DstAddr[i], szData, strlen(szData));
				iSendCount += iLen;
			}
		}
		Lock.UnLock();
	}*/

	while (true)
	{
		Tm.update();
		Lock.Lock();
		for (int i = 0; i < iCount; i++)
		{
			if (Nodes[i] != -1)
			{
				App.Send(Nodes[i], &DstAddr[i], szData, iLen, 0);
				//Session_sptr[0]->Send(&DstAddr[i], szData, strlen(szData));
			}
		}
		Lock.UnLock();
		printf("elapsed(%d) :%lld\r\n", iCount, Timestamp_type().epochMicroseconds() - time.epochMicroseconds());
		//Sleep(10);
		time.update();

		_string_type se = "12345678901234567895478965741258963";

		int iNumber = 0;
		Tm.update();
		Lock.Lock();
		for (std::map<NETHANDLE, int>::iterator iter = NetMap.begin(); iter != NetMap.end(); iter++)
		{
			if (se != "")
			{
				App.Send(iter->first, &DstAddr[iNumber], szData, iLen, 0);
				//Session_sptr[0]->Send(&DstAddr[i], szData, strlen(szData));
			}
			iNumber ++;
		}
		Lock.UnLock();
		printf("map elapsed(%d) :%lld\r\n", iCount, Timestamp_type().epochMicroseconds() - time.epochMicroseconds());

		time.update();
	}

	std::cin.get();
}

void PerformanceTestServer(void)
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

	NETHANDLE Nodes[10000] = { 0 };
	memset(Nodes, 0, sizeof(Nodes));

	struct sockaddr_in DstAddr[10000] = { 0 };

	//打开
	int iCount = Performance_COUNT;
	int iStart = Performance_START;
	for (int i = 0; i < iCount; i++)
	{
		Nodes[i] = App.Open(iStart + i);
		if (Nodes[i] != -1)
		{
			App.SetRecvHandle(Nodes[i], RecvBackFun);
			App.SetDestroyHandle(Nodes[i], function20_bind(CloseBackFun, GetHostIP().c_str(), iPort, _function_::_1));
			printf("Open Port:%d Success\r\n", iStart + i);
		}
		else
		{
			printf("Open Port:%d Failed!\r\n", iStart + i);
		}
	}

	std::cin.get();
}

int main(int argc, char* argv[])
{
	PerformanceTestClient();
	//PerformanceTestServer();
	//DllTest();
	//ClientTest();
	//ServerTest();

	while(true)
	{
		Sleep(10);
	};
}