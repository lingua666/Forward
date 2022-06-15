// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libThirdWrap_Onvif_Camera/Onvif_Device.h>

void CALLBACK RealData (UInt8* pData, UInt32 uSize )
{

}

int main(int argc, char* argv[])
{
	Onvif_Device	g_Onvif_Device;
	g_Onvif_Device.Connect("http://192.168.8.23/onvif/device_service",
						"admin", "tranwit1234");

	/*g_Onvif_Device.SetRealDataCallBack(RealData);
	g_Onvif_Device.Start_Preview(GetConsoleWindow());*/
	g_Onvif_Device.PTZControl(PTZ_PAN_LEFT, 0.5);
	Sleep(500);
	g_Onvif_Device.PTZControl_Stop();

	std::cin.get();
	return 0;
}