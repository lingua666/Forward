// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_FtpSvr/FtpServer.h>

int main(int argc, char* argv[])
{
	_ftp_::_ftp_string sIP = "192.168.1.164";
	UInt16 uPort = 6666;
	_ftp_::FtpServer	FtpSvr;
	FtpSvr.Init(sIP.c_str(), uPort, 40000, 50000, "F:");
	FtpSvr.NewSession(sIP, "admin", "admin", false);
	if (FtpSvr.Run() == -1)
	{
		printf("Ftp Server(%s:%d) Run Failed!!\r\n", sIP.c_str(), uPort);
	}
	else
	{
		printf("Ftp Server(%s:%d) Run Success!!\r\n", sIP.c_str(), uPort);
	}

	std::cin.get();
	return 0;
}