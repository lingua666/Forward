
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <libThirdWrap_RtspClient/RTSPClientManager.h>
//#include <libThirdWrap_RtspClient/RTSPClient.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	RTSPClientManager	Manager;
	Manager.Init();

	while( true )
	{
		MyRTSPClient* p = Manager.Open("rtsp://admin:abcd-1234@192.168.8.166:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1");
		/*int irand = rand() % 3000;
		Sleep(irand);
		MyRTSPClient::ShutdownStream(p);*/
		Sleep(100000000);
	}

	getchar();
    return 0;
}



