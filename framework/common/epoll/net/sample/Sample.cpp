// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libEPoll_Net/acceptor.h>
#include <libEPoll_Net/connector.h>
#include <libEPoll_Net/TcpSocket.h>
#include <libFoundation/modafx.h>

_epoll_net_::io_service	g_io_service;
_epoll_net_::acceptor	g_apt;
_epoll_net_::TcpSocket*	gp_Socket;
int g_inc = 0;

/*#include <winsock2.h>
#include <ws2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")	// Á´½Óµ½WS2_32.lib*/

char g_szBuf[1500] = {0};
char g_szWrite[1500] = {0};

void SendHandle( _epoll_net_::tagIOData_INFO* pInfo )
{
	if( pInfo->_ibytes_transferred == -1)
	{
		printf("SendHandle(%d): error code:%d\r\n", pInfo->_ibytes_transferred, errno);
	}
	
	printf("SendHandle(%d)\r\n", pInfo->_ibytes_transferred);
}

void RecvHandle( _epoll_net_::tagIOData_INFO* pInfo )
{
	printf("RecvHandle(%d):%s error code:%d\r\n", pInfo->_ibytes_transferred, pInfo->_pOverlapped->_SWSAOverlapp.wsaBuf.buf, errno);
	if( pInfo->_ibytes_transferred > 0 )
	{
		_epoll_net_::WSABUF	wBuf;
		wBuf.buf = g_szWrite;
		wBuf.len = pInfo->_ibytes_transferred;
		printf("pInfo->_pOverlapped->_SWSAOverlapp.wsaBuf.buf:%p, pInfo->_pOverlapped->_SWSAOverlapp.wsaBuf.len:%d, wBuf.len:%d\r\n",
				pInfo->_pOverlapped->_SWSAOverlapp.wsaBuf.buf, pInfo->_pOverlapped->_SWSAOverlapp.wsaBuf.len, wBuf.len);
		memcpy(wBuf.buf, pInfo->_pOverlapped->_SWSAOverlapp.wsaBuf.buf, wBuf.len);
		gp_Socket->async_write(wBuf, SendHandle);
		gp_Socket->async_read(pInfo->_pOverlapped);
	}
	else
	{
		if( pInfo->_ibytes_transferred == -1 && errno == EAGAIN )
		{
			gp_Socket->async_read(pInfo->_pOverlapped);
		}
		else
		{
			g_inc --;
			if( gp_Socket != NULL )
			{
				gp_Socket->close();
				//delete gp_Socket;
			}
		}
			
		
		
		//gp_Socket = new _epoll_net_::TcpSocket(g_io_service);
	}
}


void AcceptHandle( _epoll_net_::tagIOData_INFO* p )
{
	g_inc ++;
	printf("AcceptHandle g_inc:%d\r\n", g_inc);
	int iRet = gp_Socket->Bind(p->_pOverlapped->_ADR._Sock);
	const _epoll_net_::tagSOCKET_INFO* pInfo = gp_Socket->GetInfo();
	printf("gp_Socket->Bind:%d,_szRemote:%s, _u16RemotePort:%d, _szLocal:%s, _u16LocalPort:%d\r\n",
		iRet, pInfo->_szRemote, pInfo->_u16RemotePort, pInfo->_szLocal, pInfo->_u16LocalPort);
	_epoll_net_::WSABUF wBuf;
	wBuf.buf = g_szBuf;
	wBuf.len = sizeof(g_szBuf);
	gp_Socket->async_read(wBuf, RecvHandle);
	g_apt.async_accept(p->_pOverlapped);
	//test
	/*printf("close begin\r\n");
	Sleep(5000);
	printf("close end\r\n");
	//gp_Socket->close();
	
	g_io_service.close();*/
	//test
}

void ConnectHandle( _epoll_net_::tagIOData_INFO* p )
{

}

void AsyncAccept( void )
{
	g_io_service.open();
	g_apt.attach(g_io_service);
	gp_Socket = new _epoll_net_::TcpSocket(g_io_service);

	if( g_apt.bind(60000) == -1 )
	{
		printf("bind failed!!\r\n");
	}

	g_apt.listen();
	g_apt.set_function(function20_bind_Ex(AcceptHandle));
	g_apt.async_accept();
	_thread_::ThreadWrap	g_thread1( function20_bind(&_epoll_net_::io_service::run, &g_io_service) );
	_thread_::ThreadWrap	g_thread2( function20_bind(&_epoll_net_::io_service::run, &g_io_service) );
	_thread_::ThreadWrap	g_thread3( function20_bind(&_epoll_net_::io_service::run, &g_io_service) );
	g_thread1.wait_run();
	g_thread2.wait_run();
	g_thread3.wait_run();

	printf("AsyncAccept is run \r\n");
	while(true)
	{
		Sleep(10);
	}

	std::cin.get();
	exit(0);
}

void AsyncConnect( void )
{
	g_io_service.open();
	_epoll_net_::connector	con(g_io_service);
	con.listen();
	con.set_function(function20_bind_Ex(ConnectHandle));

	_thread_::ThreadWrap	g_thread( function20_bind(&_epoll_net_::io_service::run, &g_io_service) );
	g_thread.wait_run();

	con.async_connect("192.168.3.102",60000);

	std::cin.get();
	exit(0);
}

int main(int argc, char* argv[])
{
	AsyncAccept();
	//AsyncConnect();

	std::cin.get();
	return 0;
}