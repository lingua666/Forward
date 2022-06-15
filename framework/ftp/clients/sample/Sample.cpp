// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_FtpClients/FtpClients.h>

void HandleSubscribe(const _ftp_::FtpClients::FtpClient_Impl_sptr& sptr, int StatusCode)
{

}

void HandleSubFiles(const _ftp_::FtpClients::FtpClient_Impl_sptr& sptr, const _ftp_::FtpClients::FileList_tpye& Files)
{

}

int main(int argc, char* argv[])
{
	_ftp_::FtpClients Clients;
	Clients.Init(function20_bind_Ex(HandleSubscribe), function20_bind_Ex(HandleSubFiles));

	while (true)
	{
		_ftp_::FtpClients::FtpClient_Impl_sptr	Client_sptr = Clients.Open("192.168.1.164", 21, "admin", "admin", true);
		//Sleep(1000000);
		//Sleep(rand()%2000);
		Client_sptr->GetFiles("/");
		Sleep(1000000);
		Clients.Close(Client_sptr);
	}

	_ftp_::FtpClients::FtpClient_Impl_sptr	Client_sptr = Clients.Open("192.168.1.164", 21, "admin", "admin", true);
	while (true)
	{
		Sleep(10);
		Client_sptr->GetFiles("/");
		//Client_sptr->DownLoad("/Tengxun_FoxitReader_CHS_8.0.4.815.exe", "F:\\test\\dir\\Tengxun_FoxitReader_CHS_8.0.4.815.exe");
		//Client_sptr->UpLoad("C:\\Users\\2020\\Downloads\\Compressed\\jiandanftpserver\\jyFTPServer\\Config.ini", "/");
		//Client_sptr->RemoveDir("/ftp_");
		//Sleep(2000);
	}
	Clients.Close(Client_sptr);

	std::cin.get();
	Clients.Release();
	return 0;
}