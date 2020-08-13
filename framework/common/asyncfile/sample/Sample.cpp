
#include <libFramework_AsyncFiles/AsyncFiles.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

_async_::_files_::AsyncFiles	Async;
THandle	hWrite;
THandle hRead;
void ReadFun(THandle Handle, const char* szData, UInt32 uSize)
{
	//printf("ReadFun:%s\r\n", szData);
	if( uSize == 0 )
	{
		printf("Read File Finish\r\n");
	}
	else
	{
		Async.Write(hWrite, szData, uSize);
		Async.Read(hRead, 1470);
	}
}

void WriteFun(THandle Handle, UInt32 uSize )
{
	//printf("WriteFun\r\n");
}

void DestroyFun(THandle Handle)
{
	printf("DestroyFun\r\n");
	Async.Close(Handle);
}


int main(int argc, char* argv[])
{
	Async.Init();
	//while(true)
	{
		_file_::CMyFile::DeleteFile("d:\\111.txt");
		FILE* fp = fopen("d:\\111.txt","wb");
		if( fp != NULL )
		{
			fwrite("111122223333344445556667778889999", strlen("111122223333344445556667778889999"), 1, fp);
			fclose(fp);
		}
		hRead = Async.Open("g:\\TestDir\\123.exe", _async_::_files_::File_impl::enMode_Read);
		if( hRead == -1 )
		{
			printf("打开文件失败!!!\r\n");
		}

		hWrite = Async.Open("g:\\RecrodDir\\HK_123456789_0\\HK_123456789_1\\1111.txt", _async_::_files_::File_impl::enMode_Write);
		if( hWrite == -1 )
		{
			printf("打开文件失败!!!\r\n");
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

