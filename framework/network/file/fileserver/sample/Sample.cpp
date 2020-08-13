// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libNetFrame_FileServer/FileServer.h>

typedef _session_::StreamBuf		StreamBuf;
typedef _session_::StreamBuf_ptr	StreamBuf_ptr;

long g_AcceptNum = 0;

_server_::FileServer*	GetServer( void )
{
	static	_server_::FileServer	Server;
	return &Server;
}

void PostRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{
	int is = StreamBuf::GetSize(Buf_ptr.get());
	printf("Send Data: %d\r\n",StreamBuf::GetSize(Buf_ptr.get()));
	GetServer()->Send(Node, StreamBuf::GetData(Buf_ptr.get()),
		StreamBuf::GetSize(Buf_ptr.get()));
}

void RecvBackFun( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{
	/*printf("Recv Data(%d):%s\r\n",StreamBuf::GetSize(Buf_ptr.get()),
		StreamBuf::GetData(Buf_ptr.get()));*/

	//printf("Recv Data(%d):%s\r\n",u32Size, pData);
	//GetServer()->Post(boost::bind(PostRecv, Node, Buf_ptr));
	//GetServer()->Send(Node, pData, u32Size);

	//printf("Send Data: %d\r\n",StreamBuf::GetSize(Buf_ptr.get()));
	GetServer()->Send(Node, StreamBuf::GetData(Buf_ptr.get()),
						StreamBuf::GetSize(Buf_ptr.get()));
}

void CloseBackFun( _string_type sIP, UInt16 uPort, NETHANDLE Node )
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
	GetServer()->SetCloseHandle(Node, function20_bind(CloseBackFun,
									GetIPv4ToString(Node),
									GetPort(Node), _function_::_1));
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


#include <signal.h>
void ExitHande(int n)
{ 
	if(n==SIGINT) 
	{
		printf("\n你按下了ctrl+c");
		signal(SIGINT, ExitHande);
		GetServer()->Stop();
		return;
	} 

	if(n==SIGBREAK)
	{
		printf("\n你按下了ctrl+break,现在退出程序\n");
		GetServer()->Stop();
		exit(0);
	}
}

int main(int argc, char* argv[])
{
	signal(SIGINT, ExitHande);  /*注册ctrl+c信号捕获函数*/
	signal(SIGBREAK, ExitHande); /*注册ctrl+break信号捕获函数*/

	const int iPort = 11000;
	if( GetServer()->Listen(iPort, function20_bind_Ex(AcceptBackFun)) )
	{
		int iThreadNum = get_processor_number() * 2;
		GetServer()->Run( iThreadNum );
		printf("=====================================================\r\n");
		printf("Media Serever is Runing,\r\nHost:%s, "
			"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),iPort, iThreadNum);
		printf("=====================================================\r\n");
	}

	std::cin.get();
	return 0;
}