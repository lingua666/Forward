// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libIOCP_File/FileIO.h>
#include <libFoundation/modafx.h>

_iocp_file_::io_service	g_io_service;
char g_szBuf[4000] = {0};

void ReadData(_iocp_file_::tagIOData_INFO* Data)
{

}

void WriteData(_iocp_file_::tagIOData_INFO* Data)
{

}


int main(int argc, char* argv[])
{
	/*HANDLE hSrcFile = CreateFile("D:\\111_mo - 副本.h264",//"D:\\111_mo.h264",
				GENERIC_READ, FILE_SHARE_READ, 
				NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, 
				NULL);  

	if(hSrcFile == INVALID_HANDLE_VALUE)
	{

	}*/

	HANDLE hDstFile = CreateFile("D:\\111.txt",
				GENERIC_WRITE, 0, 
				NULL, CREATE_ALWAYS,
				FILE_FLAG_OVERLAPPED, NULL);
	if( hDstFile == INVALID_HANDLE_VALUE )
	{
		printf("创建文件失败!!!\r\n");
	}

	g_io_service.open();
	_thread_::ThreadWrap thd(function20_bind(&_iocp_file_::io_service::run, &g_io_service));
	thd.wait_run();

	_iocp_file_::FileIO	File(g_io_service);
	//File.Bind(hSrcFile);
	File.Bind(hDstFile);


	File.async_write("1234567890123456789012345678901234567890",
		strlen("1234567890123456789012345678901234567890"), WriteData);

//	File.async_read(g_szBuf, 1024, ReadData);

	std::cin.get();
	CloseHandle(hDstFile);
	std::cin.get();
	return 0;
}