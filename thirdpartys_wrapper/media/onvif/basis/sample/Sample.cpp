#include <libThirdWrap_Onvif/ONVIF_INTERFACE.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>

int Onvif_DiscoveryCallBack( const tagOnvif_DiscoveryInfo* pDiscoveryInfo, void* pUser )
{
	if( pDiscoveryInfo != NULL )
	{
		printf("/**************************************************/\r\n");
		printf("pDiscoveryInfo->Device_t:%s\r\n", pDiscoveryInfo->Device_t);
		printf("pDiscoveryInfo->EP_Address:%s\r\n", pDiscoveryInfo->EP_Address);
		printf("pDiscoveryInfo->IP:%s\r\n", pDiscoveryInfo->IP);
		printf("pDiscoveryInfo->Manufacturer:%s\r\n", pDiscoveryInfo->Manufacturer);
		printf("pDiscoveryInfo->Service_Addr:%s\r\n", pDiscoveryInfo->Service_Addr);
		printf("pDiscoveryInfo->UUID:%s\r\n", pDiscoveryInfo->UUID);
		//if( !strcmp(pDiscoveryInfo->IP, "192.168.8.166") )
		{
			ONVIF_INTERFACE	Inter;
			if( Inter.Init(pDiscoveryInfo->Service_Addr, "admin", "abcd-1234") == 1 )
			{
				printf("MediaUri:%s\r\n",Inter.GetMediaUri());

				tagOnvif_TimeAndDate Data = {0};
				Inter.GetSystemTimeAndDate(&Data);
				int i = 0;
			}
		}
		printf("/**************************************************/\r\n");
	}

	return 1;
}

int main()  
{
	while (true)
	{
		/*_sip_::Sip_Request_Register	Reg;
		Reg.Init({ "192.168.8.120",5060 }, 1);
		Reg.SetVia({ "192.168.8.110",8080 }, "xxBranchyy");
		Reg.SetFromTo("10000", ".sip.d.", "xxTagyy");
		Reg.SetCallID("xxxxCallIDyyyy");

		_sip_::_string_big_t str_big = Reg.Generate();

		Reg.SetToken("admin", ".sip.d.", "xxxNonceyyy", "xxxResponseyyy");
		//Reg.SetVia({ "192.168.8.111",8080 }, "xxBranchyy");
		str_big = Reg.Generate();*/
	}

	while(true)
	{
		//rtsp://admin:abcd-1234@192.168.8.166:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1
		ONVIF_ClientDiscovery_C(3, Onvif_DiscoveryCallBack, NULL, "192.168.8.102");

		Sleep(1);
	}

	getchar();
    return 0;  
}
