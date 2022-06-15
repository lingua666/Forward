// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_RtspClients/RtspClients.h>
#include <dllThirdWrap_FFmpeg/Exports.h>
#include <libThirdWrap_FFmpeg/DecoderManager.h>
#include <dllFrameWork_RtspClients/Exports.h>

/////////////////////////////////////test/////////////////////////////////////////////////////

struct tagRtspClientsPlayINFO
{
	UInt16 uPort;
	THandle hDec;
	bool isRealTime;
	THandle hWnd;
	THandle SessionHandle;
};

CLock	Lock;
char g_SessionID[100] = {0};

void ExceptNotify(const char* c_szSessionID, Int32 iErrCode )
{

}

void SdpNotify(const char* c_szSessionID, RtspClient_Impl::tagSdpInfo* pSdp)
{

}

void SuccessNotify(const char* c_szSessionID)
{
	Lock.Lock();
	strcpy(g_SessionID, c_szSessionID);
	Lock.UnLock();
}

THandle g_hDec = NULL;

//FILE* g_h264fp = fopen("D:\\UUIY.H264", "wb");

void RtpDataNotify(const char* c_szSessionID, const char* c_szData, UInt32 uLen)
{
	//printf("RtpDataNotify(%d)\r\n", uLen);

	//test
	/*fwrite(c_szData, uLen, 1, g_h264fp);
	fflush(g_h264fp);*/
	//test

	if( g_hDec == NULL )
	{
		g_hDec = MDecoder_Parse(c_szData, uLen);
		if( g_hDec != NULL )
		{
			MDecoder_Set(g_hDec, GetConsoleWindow(), false);
		}
	}
	else
	{
		MDecoder_Decode(g_hDec, c_szData, uLen);
	}
}

void Lib_Test( void )
{
	MDecoder_Video_Init(GetConsoleWindow());
	RtspClients Clients;
	Clients.Init();
	while(true)
	{
		printf("1111\r\n");
		_string_type s = Clients.Open("rtsp://admin:abcd-1234@192.168.8.166:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
			RtspPacket::ENUM_UDP_TYPE, 8002, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);

		//ÍâÍøRTSP²âÊÔ
		/*_string_type s = Clients.Open("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov",
			RtspPacket::ENUM_UDP_TYPE, 8002, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);*/
		Clients.Play(s.c_str());
		while( strcmp(g_SessionID, "") == 0) Sleep(10);

		Sleep(200000000);
		printf("2222\r\n");
		Lock.Lock();
		Clients.Close(g_SessionID);
		memset(g_SessionID, 0, sizeof(g_SessionID));
		Lock.UnLock();
		Sleep(10000);
		printf("3333\r\n");
	}

	/*Clients.Open("rtsp://admin:abcd-1234@192.168.8.22:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
		8000, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);

	Clients.Open("rtsp://192.168.8.30:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream",
		61142, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);*/

	std::cin.get();
}

THandle g_SessionHandle = NULL;
int g_iCount = 0;

void _CALLTYPE SdpNotify_Dll( void* pUser, THandle Handle, RtspClient_Impl::tagSdpInfo* pSdp );
void _CALLTYPE SuccessNotify_Dll( void* pUser, THandle Handle );
void _CALLTYPE RtpDataNotify_Dll( void* pUser, THandle Handle, const char* c_szData, int uLen );

void _CALLTYPE ExceptNotify_Dll( void* pUser, THandle Handle, Int32 iErrCode )
{
	//while( g_SessionHandle != NULL) Sleep(10);

	THandle* phDec = (THandle*)pUser;
	MRtspClients_Close(Handle);
	MDecoder_Close(*phDec);
	if(phDec != NULL)
	{
		*phDec = NULL;
		delete phDec;
	}

	g_iCount ++;

	//test
	/*printf("ExceptNotify_Dll\r\n");
	//test

	phDec = new THandle(); 

	//ÍâÍøRTSP²âÊÔ
	THandle Handle1 = MRtspClients_Open("rtsp://admin:abcd-1234@192.168.8.166:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
		RtspPacket::ENUM_TCP_TYPE, 10002);
	MRtspClients_SetResultEvent(Handle1, ExceptNotify_Dll, phDec, SuccessNotify_Dll, phDec);
	MRtspClients_SetDataEvent(Handle1, RtpDataNotify_Dll, phDec);

	MRtspClients_Play(Handle1);*/
}

void _CALLTYPE SdpNotify_Dll( void* pUser, THandle Handle, RtspClient_Impl::tagSdpInfo* pSdp )
{

}

void _CALLTYPE SuccessNotify_Dll( void* pUser, THandle Handle )
{
	Lock.Lock();
	g_SessionHandle = Handle;
	Lock.UnLock();
}

bool g_isRealTime201811151716 = false;

void _CALLTYPE RtpDataNotify_Dll( void* pUser, THandle Handle, const char* c_szData, int uLen )
{
	THandle* phDec = (THandle*)pUser;
	if( *phDec == NULL )
	{
		*phDec = MDecoder_Parse(c_szData, uLen);
		if( *phDec != NULL )
		{
			MDecoder_Set(*phDec, GetConsoleWindow(), g_isRealTime201811151716);
		}
	}
	else
	{
		MDecoder_Decode(*phDec, c_szData, uLen);
	}
}

void Dll_Test( void )
{
	MDecoder_Video_Init();
	MRtspClients_Init(2, 2, 2, 6);

	int iCount = 0;
	int iPort = 8000;

	while(true)
	{
		printf("1111\r\n");
		/*Clients.Open("rtsp://admin:abcd-1234@192.168.8.43:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
			RtspPacket::ENUM_UDP_TYPE, 8002, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);*/

		THandle Handle = MRtspClients_Open("rtsp://admin:tranwit1234@192.168.8.22:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
			RtspPacket::ENUM_TCP_TYPE, 9008);

		/*THandle Handle = MRtspClients_Open("rtsp://admin:abcd-1234@192.168.8.166:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
			RtspPacket::ENUM_UDP_TYPE, iPort);*/
		/*THandle Handle = MRtspClients_Open("rtsp://admin:tranwit1234@192.168.8.21:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
			RtspPacket::ENUM_TCP_TYPE, 9008);*/
		/*if( Handle == NULL )
			continue;*/

		iPort ++;
		g_isRealTime201811151716 = true;

		//ÍâÍøRTSP²âÊÔ
		/*THandle Handle = MRtspClients_Open("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov",
			RtspPacket::ENUM_UDP_TYPE, 19002);
		g_isRealTime201811151716 = false;*/
		THandle* phDec = new THandle();
		MRtspClients_SetResultEvent(Handle, ExceptNotify_Dll, phDec, SuccessNotify_Dll, phDec);
		MRtspClients_SetDataEvent(Handle, RtpDataNotify_Dll, phDec);

		if( MRtspClients_Play(Handle) == -1 )
		{
			MRtspClients_Close(Handle);
			if( phDec != NULL )
				delete phDec;
			continue;
		}
		//while( g_SessionHandle == NULL) Sleep(10);

		Sleep(500 + rand()%5000);
		Sleep(10000000);
		printf("2222\r\n");
		Lock.Lock();
		MRtspClients_Close(Handle);
		//g_SessionHandle = NULL;
		Lock.UnLock();
		printf("3333\r\n");

		iCount ++;
		if( iCount > 1000 )
		{
			printf("Sleep()\r\n");
			Sleep(5000);
			if(iCount != g_iCount)
			{
				int i = 0;
				Sleep(5000);
			}

			iCount = 0;
			g_iCount = 0;

			//Sleep(6000000);
		}
	}

	/*Clients.Open("rtsp://admin:abcd-1234@192.168.8.22:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
		8000, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);

	Clients.Open("rtsp://192.168.8.30:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream",
		61142, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);*/
	std::cin.get();
}

void _CALLTYPE RtpDataNotify_Ex_Impl( tagRtspClientsPlayINFO* pInfo,
	const char* c_szData, UInt32 uLen )
{
	if( pInfo->hDec == NULL )
	{
		pInfo->hDec = MDecoder_Parse(c_szData, uLen);
		if( pInfo->hDec != NULL )
		{
			MDecoder_Set(pInfo->hDec, (HWND)pInfo->hWnd, pInfo->isRealTime);
		}
	}
	else
	{
		MDecoder_Decode(pInfo->hDec, c_szData, uLen);
	}
}

void _CALLTYPE RtpDataNotify_Dll_Ex( void* pUser, THandle Handle, const char* c_szData, int uLen )
{
	RtpDataNotify_Ex_Impl((tagRtspClientsPlayINFO*)pUser, c_szData, uLen);
}

void Dll_Ex_Test( void )
{
	MDecoder_Video_Init();
	MRtspClients_Init(2, 2, 2, 6);
	
	//ÍâÍøRTSP²âÊÔ
	tagRtspClientsPlayINFO* pPlayINFO1 = new tagRtspClientsPlayINFO();
	memset(pPlayINFO1, 0, sizeof(tagRtspClientsPlayINFO));
	pPlayINFO1->isRealTime = false;
	pPlayINFO1->hWnd = (THandle)GetConsoleWindow();
	THandle Handle = MRtspClients_Open("rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov",
			RtspPacket::ENUM_TCP_TYPE, 9002);
	MRtspClients_SetResultEvent(Handle, ExceptNotify_Dll, pPlayINFO1, SuccessNotify_Dll, pPlayINFO1);
	MRtspClients_SetDataEvent(Handle, RtpDataNotify_Dll_Ex, pPlayINFO1);

	MRtspClients_Play(Handle);

	std::cin.get();
}


int main(int argc, char* argv[])
{
	//Dll_Ex_Test();
	Dll_Test();
	//Lib_Test();
	return 0;
}