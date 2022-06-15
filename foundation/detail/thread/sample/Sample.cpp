// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libThread/ThreadWrap.h>
#include <dllThread/Exports.h>

struct tagTest
{
	char s1[50];
	char s2[50];
};

bool FTest( void )
{
	return true;
}

bool TTest( void )
{
	return true;
}

void TTestVoid( void )
{
	printf("TTestVoid begin\r\n");
	Sleep(1000);
	/*while( true )
	{
		try
		{
			ThreadWrap::interruption_point();
			//Sleep(5000);
		}
		catch (const thread_interrupted& e)
		{
			switch( e.get_code() )
			{
			case ThreadData::en_INTERRUPTED_EXIT:	//Ïß³ÌÍË³ö
				printf(" TTestVoid thread_interrupted\r\n");
				goto gt_exit;
				break;
			}
			break;
		}
	}
gt_exit:*/
	printf("TTestVoid end\r\n");
}

int main(int argc, char* argv[])
{
	//test
	while (true)
	{
		ThreadWrap	ter1;
		ter1.interrupt(ThreadData::en_INTERRUPTED_EXIT);
		ter1.join();
	};
	//test

	while(true)
	{
		ThreadWrap	ter(function20_bind_Ex(TTestVoid));
		printf("start\r\n");
		//Sleep(1000);
		ter.join();
	}

	int i = 0;//sizeof(THandle);

	if( i > 0 ? TTest() : FTest())
	{
		int j = 0;
		j ++;
	}

	//char* p = GetInt();
	//char* p = GetString();

	char* p = GetMemory();

	ParseMemory(p,NULL);

	int iTimer = 0, iNum = 100000000;
	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		ThreadWrap::interruption_point();
	}
	printf("ThreadWrap interruption_point timer:%d\r\n", GetTickCount() - iTimer);

	std::cin.get();
	return 0;
}