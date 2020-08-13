// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libIOCP_Net/acceptor.h>
#include <libIOCP_Net/connector.h>
#include <libIOCP_Net/TcpSocket.h>
#include <libFoundation/modafx.h>

_iocp_net_::_tcp_::io_service	g_io_service;


/*#include <winsock2.h>
#include <ws2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")	// Á´½Óµ½WS2_32.lib*/

void ConnectHandle( tagIOData_INFO* p )
{

}


void AsyncConnect( void )
{
	g_io_service.open();
	_iocp_net_::_tcp_::connector	con(g_io_service);
	con.listen();
	con.set_function(function20_bind_Ex(ConnectHandle));

	_thread_::ThreadWrap	g_thread( function20_bind(&_iocp_net_::_tcp_::io_service::run, &g_io_service) );
	g_thread.wait_run();

	con.async_connect("192.168.3.102",60000);

	std::cin.get();
}

int main(int argc, char* argv[])
{
	int nCount = 0;
	_string_type sHost = GetIPByDomain("www.baidu.com", &nCount);

	AsyncConnect();

	/*SOCKET Sock = APIWSACreate(SOCK_STREAM,-1,-1);

	int buflen = 8 * 1024;
	if ( APIWSASetSockopt( Sock, SOL_SOCKET,SO_SNDBUF, (const char*)&buflen,sizeof(int)) == SOCKET_ERROR )
	{
		APIWSAClose( Sock );
		return INVALID_SOCKET;
	}

	APIWSAClose(Sock);

	if ( APIWSASetSockopt( Sock, SOL_SOCKET,SO_SNDBUF, (const char*)&buflen,sizeof(int)) == SOCKET_ERROR )
	{
		APIWSAClose( Sock );
		return INVALID_SOCKET;
	}*/

	std::cin.get();
	return 0;
}