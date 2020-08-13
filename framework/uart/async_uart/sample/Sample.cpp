
#include <dllFramework_AsyncUarts/Exports.h>
#include <libFramework_AsyncUarts/AsyncUarts.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

char g_RevszBuf[1024] = {0};
int  g_Offset = 0;
_async_::_uarts_::AsyncUarts	Async;
int g_Inc = 0;
void ReadFun(THandle Handle, const char* szData, UInt32 uSize)
{
	g_Inc += uSize;
	printf("ReadFun(%d,%d):%s\r\n", uSize, g_Inc, szData);
	if( Handle != NULL && uSize > 0 )
	{
		if( g_Offset + uSize > 1024 )
		{
			g_Offset = 0;
		}
		else
		{
			memcpy(&g_RevszBuf[g_Offset], szData, uSize);
			g_Offset += uSize;
			if( szData[uSize - 1] == '#' )
			{
				g_RevszBuf[g_Offset] = 0;
				//Async.Send(Handle, g_RevszBuf, g_Offset);
				MAsyncUart_SentData(Handle, g_RevszBuf, g_Offset);
				g_Offset = 0;
			}
		}
	}
	//Async.Close(Handle);
}

void _CALLTYPE MAsyncUart_RecData( void* pUser, THandle Handle,
	const char* szData, UInt32 uSize )
{
	//MAsyncUart_SentData(Handle, c_szData, u32Size);
	g_Inc += uSize;
	printf("ReadFun(%d,%d):%s\r\n", uSize, g_Inc, szData);
	if( Handle != NULL && uSize > 0 )
	{
		if( g_Offset + uSize > 1024 )
		{
			g_Offset = 0;
		}
		else
		{
			memcpy(&g_RevszBuf[g_Offset], szData, uSize);
			g_Offset += uSize;
			if( szData[uSize - 1] == '#' )
			{
				g_RevszBuf[g_Offset] = 0;
				//Async.Send(Handle, g_RevszBuf, g_Offset);
				MAsyncUart_SentData(Handle, g_RevszBuf, g_Offset);
				g_Offset = 0;
			}
		}
	}
	//Async.Close(Handle);
}

void WriteFun(THandle Handle, UInt32 uSize )
{
	printf("WriteFun(%d)\r\n", uSize);
	//Async.Close(Handle);
	/*char sz[] = {"www.baidu.con#"};
	Async.Send(Handle, sz, sizeof(sz));*/
}

void DestroyFun(THandle Handle)
{
	printf("DestroyFun\r\n");
	//Async.Close(Handle);
}

int main(int argc, char* argv[])
{
	THandle Handle;
	MAsyncUart_Init();
	//Async.Init(3);

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	char szCom[] = "COM1";
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	char szCom[] = "/dev/ttySP1";
#endif
	
	while(true)
	{
		Handle = MAsyncUart_Open(1, 9600);//Async.Open(szCom, 9600);
		if( Handle == -1 )
		{
			printf("Open Uart failed!!\r\n");
		}
		else
		{
			printf("Open Uart:%s success!!\r\n", szCom);
			//Async.SetCallBack(Handle, ReadFun, DestroyFun, WriteFun);
			
			MAsyncUart_SetRecData(Handle, MAsyncUart_RecData, NULL);
			char sz[] = {"www.baidu.con#"};
			MAsyncUart_SentData(Handle, sz, sizeof(sz) - 1);//Async.Send(Handle, sz, sizeof(sz));
		}
		std::cin.get();
		int iTm = rand() % 2000 + 1000;
		Sleep(iTm);
		MAsyncUart_Close(Handle);
		//Async.Close(Handle);
	}

	std::cin.get();
	//Async.Close(Handle);
	MAsyncUart_Close(Handle);
	return 0;
}

