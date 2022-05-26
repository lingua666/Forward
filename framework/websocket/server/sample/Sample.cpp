// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_WebsocketSvr/WebsocketServer.h>


void HandleRecv(UInt32 uType, const _websocket_::_server_::WebsocketServer::DataSession_sptr& sptr,
	const char* c_szData, UInt64 uSize)
{
	//test
	printf("HandleRecv(%lld): %s\r\n", uSize, c_szData);
	//test

	sptr->SendText(c_szData, uSize);
}

void HandleDestroy(UInt32 uType, const _websocket_::_server_::WebsocketServer::DataSession_sptr& sptr,
	const char* c_szData, UInt64 uSize)
{
	//test
	printf("HandleDestroy()\r\n");
	//test
}

void HandleOpen(UInt32 uType, const _websocket_::_server_::WebsocketServer::DataSession_sptr& sptr,
	const char* c_szData, UInt64 uSize)
{
	sptr->AddConsumer(_websocket_::_server_::DataSession::EN_EVENT_RECV,
				function20_bind_Ex(HandleRecv));

	sptr->AddConsumer(_websocket_::_server_::DataSession::EN_EVENT_CLOSE,
		function20_bind_Ex(HandleDestroy));
}


void HandleDestroyApp(UInt32 uType, const _websocket_::_server_::WebsocketServer::DataSession_sptr& sptr,
	const char* c_szData, UInt64 uSize)
{
	//test
	printf("HandleDestroyApp()\r\n");
	//test
}


int main(int argc, char* argv[])
{
	_websocket_::_server_::WebsocketServer	App;
	UInt16 uPort = 60000;

	App.Init();
	App.AddConsumer(_websocket_::_server_::DataSession::EN_EVENT_OPEN,
		function20_bind_Ex(HandleOpen));

	App.AddConsumer(_websocket_::_server_::DataSession::EN_EVENT_CLOSE,
		function20_bind_Ex(HandleDestroyApp));

	if (App.Run(uPort) == -1)
	{
		printf("main Run() Port:%d Failed!!\r\n", uPort);
	}
	else
	{
		printf("main Run() Port:%d Success!!\r\n", uPort);
	}

	std::cin.get();
	return 0;
}