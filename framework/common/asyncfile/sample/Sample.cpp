
#include <libFrameWork_AsyncFiles/AsyncFiles.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

LOG_OBJ_DECL(SampeApps_CBC_StreamServer)

_async_::_files_::AsyncFiles	Async;
UInt64	g_uFileSize = 0;
THandle	hWrite;
THandle hRead;
void ReadFun(THandle Handle, const char* szData, UInt32 uSize)
{
	g_uFileSize += uSize;
	LOG_Print_Info(SampeApps_CBC_StreamServer, "ReadFun:%d,%lld\r\n", uSize, g_uFileSize);
	if( uSize == 0 )
	{
		LOG_Print_Info(SampeApps_CBC_StreamServer, "Read File Finish:%d\r\n", Async.Tell(Handle));
		Async.Close(Handle);
	}
	else
	{
		Async.Write(hWrite, szData, uSize);
		Async.Read(hRead, 1470);
	}
}

void WriteFun(THandle Handle, UInt32 uSize )
{
	LOG_Print_Info(SampeApps_CBC_StreamServer, "WriteFun(%d)\r\n", uSize);
}

int	g_FileCloseInc = 0;

void DestroyFun(THandle Handle)
{
	//if(Handle == hWrite)

	g_FileCloseInc ++;
	if (Handle != hWrite)
	{
		LOG_Print_Info(SampeApps_CBC_StreamServer, "Close Read\r\n");
		Async.Close(hWrite);
	}
	else
	{
		LOG_Print_Info(SampeApps_CBC_StreamServer, "Close Write\r\n");
	}
	
	LOG_Print_Info(SampeApps_CBC_StreamServer, "DestroyFun(%d)\r\n", g_FileCloseInc);
}

void Test(void)
{
	Async.Init();
	
	while(true)
	{
		g_FileCloseInc = 0;
		printf("Test begin!!!\r\n");
		for (int i = 0; i < 1; i++)
		{
			hRead = Async.Open("/home/nkdl/test/test.cap", _async_::_files_::File_impl::enMode_Read);
			if (hRead == -1)
			{
				LOG_Print_Info(SampeApps_CBC_StreamServer, "打开文件失败!!!\r\n");
			}

			hWrite = Async.Open("/home/nkdl/test/out1.cap", _async_::_files_::File_impl::enMode_Write);
			if (hWrite == -1)
			{
				LOG_Print_Info(SampeApps_CBC_StreamServer, "打开文件失败!!!\r\n");
			}
			else
			{
				Async.SetCallBack(hWrite, ReadFun, WriteFun, DestroyFun);

				Async.SetCallBack(hRead, ReadFun, WriteFun, DestroyFun);
				if (Async.Read(hRead, 1470) != 1)
				{
					Async.Close(hRead);
					LOG_Print_Info(SampeApps_CBC_StreamServer, "Async.Read Failed!!!\r\n");
				}
			}

		}

	int it = 1000 + rand() % 10000;
	Sleep(it);
	Async.Close(hRead);
	//Async.Close(hWrite);
	LOG_Print_Info(SampeApps_CBC_StreamServer, "Test end!!!\r\n");
	while(g_FileCloseInc != 2)
		Sleep(10);
};

	std::cin.get();
}

void ReadFunTest(THandle Handle, const char* szData, UInt32 uSize)
{
	LOG_Print_Info(SampeApps_CBC_StreamServer, "ReadFun(%lld):%d\r\n", Handle, uSize);
	printf("ReadFun:%d\r\n", uSize);
	if( uSize == 0 )
	{
		//LOG_Print_Info(SampeApps_CBC_StreamServer, "Read File Finish:%d\r\n", Async.Tell(Handle));
		Async.Close(Handle);
	}
	else
	{
		Async.Read(Handle, 1470);
	}
}

void DestroyFunTest(THandle Handle)
{
	printf("DestroyFunTest\r\n");
}


int main(int argc, char* argv[])
{
	MLog_SetAllLevel(0);
	LOG_Print_SetLog(SampeApps_CBC_StreamServer, MLog_GetAllLevel());

	/*Async.Init(1);
	while(true)
	{
		for(int i = 0; i < 10000; i ++)
		{
			THandle Read = Async.Open("/home/nkdl/test/test.cap", _async_::_files_::File_impl::enMode_Read);
			printf("open(%lld)\r\n", Read);
			if(Read != -1)
			{
				Async.SetCallBack(Read, ReadFunTest, _async_::_files_::AsyncFiles::HFNWrite(), DestroyFunTest);
				if( Async.Read(Read, 1470) != 1 )
				{
					printf("failed\r\n");
					Async.Close(Read);
				}
				//Sleep(1000);
				Async.Close(Read);
				//Sleep(1000000);
			}
		}
		
		printf("wait\r\n");
		Sleep(10000);
	}*/
	
	Test();

	Async.Init();
	//while(true)
	{
		/*_file_::CMyFile::DeleteFile("d:\\111.txt");
		FILE* fp = fopen("d:\\111.txt","wb");
		if( fp != NULL )
		{
			fwrite("111122223333344445556667778889999", strlen("111122223333344445556667778889999"), 1, fp);
			fclose(fp);
		}*/
		//hRead = Async.Open("F:\\Tengxun_FoxitReader_CHS_8.0.4.815.exe", _async_::_files_::File_impl::enMode_Read);
		hRead = Async.Open("D:\\南控工作.txt", _async_::_files_::File_impl::enMode_Read);
		if( hRead == -1 )
		{
			printf("打开文件失败!!!\r\n");
		}

		int y = Async.Tell(hRead);

		Async.Seek(hRead, 16*4);

		int yy = Async.Tell(hRead);

		//hWrite = Async.Open("F:\\ftp - 副本\\Tengxun_FoxitReader_CHS_8.0.4.815.exe", _async_::_files_::File_impl::enMode_Write);
		hWrite = Async.Open("D:\\1.txt", _async_::_files_::File_impl::enMode_Write);
		if( hWrite == -1 )
		{
			LOG_Print_Info(SampeApps_CBC_StreamServer, "打开文件失败!!!\r\n");
		}
		else
		{
			Async.SetCallBack(hWrite, ReadFun, WriteFun, DestroyFun);

			Async.SetCallBack(hRead, ReadFun, WriteFun, DestroyFun);
			if( Async.Read(hRead, 1470) != 1 )
			{
				Async.Close(hRead);
			}
		}

	}

	LOG_Print_Info(SampeApps_CBC_StreamServer, "Finish!!!\r\n");

	std::cin.get();
	return 0;
}

/*_async_::_files_::AsyncFiles	Async;
void ReadFun(THandle Handle, const char* szData, UInt32 uSize)
{
	printf("ReadFun:%s\r\n", szData);
	Async.Close(Handle);
}

void WriteFun(THandle Handle, UInt32 uSize )
{
	printf("WriteFun\r\n");
	Async.Close(Handle);
}

void DestroyFun(THandle Handle)
{
	printf("DestroyFun\r\n");
	Async.Close(Handle);
}

int main(int argc, char* argv[])
{
	Async.Init();
	while(true)
	{
		_file_::CMyFile::DeleteFile("d:\\111.txt");
		FILE* fp = fopen("d:\\111.txt","wb");
		if( fp != NULL )
		{
			fwrite("111122223333344445556667778889999", strlen("111122223333344445556667778889999"), 1, fp);
			fclose(fp);
		}
		_async_::_files_::AsyncFiles::THandle Handle = Async.Open("d:\\111.txt", _async_::_files_::File_impl::enMode_ReadWrite);
		if( Handle == -1 )
		{
			printf("打开文件失败!!!\r\n");
		}
		else
		{
			Async.SetCallBack(Handle, ReadFun, WriteFun, DestroyFun);
			if( Async.Read(Handle, 1024) != 1 )
			{
				Async.Close(Handle);
			}

			if( Async.Write(Handle, "88888888", strlen("88888888")) != 1 )
			{
				Async.Close(Handle);
			}
		}

		_async_::_files_::AsyncFiles::THandle Handle1 = Async.Open("d:\\222.txt", _async_::_files_::File_impl::enMode_Write);
		if( Handle1 == -1 )
		{
			printf("打开文件失败!!!\r\n");
		}
		else
		{
			Async.SetCallBack(Handle1, ReadFun, WriteFun, DestroyFun);
			if( Async.Write(Handle1, "88888888", strlen("88888888")) != 1 )
			{
				Async.Close(Handle1);
			}
		}
		Async.Close(Handle1);
		Async.Close(Handle);
		Sleep(10);
	}

	std::cin.get();
	return 0;
}*/

