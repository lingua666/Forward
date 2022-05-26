// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_NtpClients/Ntp.h>

int HandleTime(Int64 newTime)
{
	printf("HandleTime(%lld)\r\n", newTime);
	return 0;//同步到系统
}

int main(int argc, char* argv[])
{
	GetUDPStreamInstance()->Init(1,1);
	while (true)
	{
		NtpClients::self_sptr Client = NtpClients::Alloc();
		Client->Init(HandleTime);

		Client->Open("time.windows.com");

		Client->SendNtp();
		Sleep(1000);

		Int64 newTime = 0;
		Client->SendNtp(&newTime);
		printf("main SendNtp(%lld)\r\n", newTime);

		Client->SendNtp();
		Sleep(1000);

		newTime = 0;
		Client->SendNtp(&newTime);
		printf("main SendNtp(%lld)\r\n", newTime);

		Client->Close();
	}

	while (true)
	{
		Sleep(1000);
	};

	return 0;
}