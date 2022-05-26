// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_WebsocketClients/WebsocketClients.h>

int main(int argc, char* argv[])
{
	_websocket_::WebsocketClients	App;
	const char* szIP = "192.168.1.164";
	UInt16 uPort = 60000;

	App.Init();
	if (App.Open(szIP, uPort) == -1)
	{
		printf("main Open() szIP:%s, Port:%d Failed!!\r\n", szIP, uPort);
	}
	else
	{
		printf("main Open() szIP:%s, Port:%d Success!!\r\n", szIP, uPort);
	}

	std::cin.get();
	return 0;
}