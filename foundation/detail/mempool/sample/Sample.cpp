// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <libLock/CASSpinLock.h>
#include <libTimestamp/Timestamp.h>
#include <libMempool/MemPool.h>

#define USE_BOOST	
#define USE_MY	

#define POOL_SIZE	1024
#define _NUM_		600 * 1500

//#define MyPool	MemPool_macro<>
#define MyPool	MemPool_macro<0, CNoneLock>


void MyPoolTest_Obj( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		MyPool my_pol1(POOL_SIZE);
	}

	printf("MemPool_macro MyPoolTest_Obj time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_Obj( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		boost::pool<> bst_pol1(POOL_SIZE);
	}

	printf("boost::pool BoostPoolTest_Obj time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void MyPoolTest_Obj_oneAlloc( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		MyPool my_pol1(POOL_SIZE);
		p[i] =  (char*)my_pol1.Alloc();
	}

	printf("MemPool_macro MyPoolTest_Obj_oneAlloc time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_Obj_oneAlloc( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		boost::pool<> bst_pol1(POOL_SIZE);
		p[i] =  (char*)bst_pol1.malloc();
	}

	printf("boost::pool MyPoolTest_Obj_oneAlloc time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void MyPoolTest_Alloc( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
	}

	printf("MemPool_macro MyPoolTest_Alloc time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_Alloc( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
	}

	printf("boost::pool BoostPoolTest_Alloc time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void MyPoolTest_Free( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
	}

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		my_pol.Free(p[i]);
	}

	printf("MemPool_macro MyPoolTest_Free time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_Free( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
	}

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		bst_pol.free(p[i]);
	}

	printf("boost::pool BoostPoolTest_Free time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void MyPoolTest_AllocAndFree( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		my_pol.Free(p[i]);
	}

	printf("MemPool_macro MyPoolTest_AllocAndFree time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_AllocAndFree( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		bst_pol.free(p[i]);
	}

	printf("boost::pool BoostPoolTest_AllocAndFree time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void MyPoolTest_AllocFree( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
		my_pol.Free(p[i]);
	}

	printf("MemPool_macro MyPoolTest_AllocFree time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_AllocFree( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
		bst_pol.free(p[i]);
	}

	printf("boost::pool BoostPoolTest_AllocFree time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void MyPoolTest_AllocAllocFree( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		my_pol.Free(p[i]);
	}

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		my_pol.Free(p[i]);
	}

	printf("MemPool_macro MyPoolTest_AllocAllocFree time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}

void BoostPoolTest_AllocAllocFree( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		bst_pol.free(p[i]);
	}

	_timestamp_::Timestamp	time;

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		bst_pol.free(p[i]);
	}

	printf("boost::pool BoostPoolTest_AllocAllocFree time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}


void MyPoolTest( void )
{
	int MemSize = sizeof(MyPool);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	MyPool my_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	/*for(i = 0 ; i < _NUM_; i ++)
	{
		MyPool my_pol1(POOL_SIZE);
		p[i] =  (char*)my_pol1.Alloc();
		my_pol1.Free(p[i]);
	}*/

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)my_pol.Alloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		my_pol.Free(p[i]);
	}

	printf("MemPool_macro  time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;

	//pol.Release();
}

void BoostPoolTest( void )
{
	int BoosSize = sizeof(boost::pool<>);

	char data[POOL_SIZE] = {0};
	char** p = new char*[_NUM_];
	int i = 0;
	boost::pool<>	bst_pol(POOL_SIZE);

	_timestamp_::Timestamp	time;

	/*for(i = 0 ; i < _NUM_; i ++)
	{
		boost::pool<> bst_pol1(POOL_SIZE);
		p[i] =  (char*)bst_pol1.malloc();
		bst_pol1.free(p[i]);
	}*/

	for(i = 0 ; i < _NUM_; i ++)
	{
		p[i] =  (char*)bst_pol.malloc();
	}

	for(i = 0 ; i < _NUM_; i ++)
	{
		bst_pol.free(p[i]);
	}

	printf("boost::pool  time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);

	if( p != NULL )
		delete p;
}


void MemoryTest( void )
{
	int iuwe = (int)0x5584b0 ^ (int)0x558930;
	int woei = (int)0xd80 ^ (int)0x5584b0;
	
	while(true)
	{
		/*MyPoolTest();
		BoostPoolTest();*/

		int iNum = 10;
		int i = 0;
		printf("obj begin \r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_Obj();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_Obj();
#endif
		}

		printf("Obj_oneAlloc begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_Obj_oneAlloc();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_Obj_oneAlloc();
#endif
		}

		printf("Alloc begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_Alloc();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_Alloc();
#endif
		}

		printf("Free begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_Free();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_Free();
#endif
		}

		printf("AllocAndFree begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_AllocAndFree();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_AllocAndFree();
#endif
		}

		printf("AllocFree begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_AllocFree();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_AllocFree();
#endif
		}

		printf("AllocAllocFree begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_MY)
			MyPoolTest_AllocAllocFree();
#endif
		}

		for( i = 0; i < iNum; i ++)
		{
#if defined(USE_BOOST)
			BoostPoolTest_AllocAllocFree();
#endif
		}

		//Sleep(1000);
		break;
	}
}


int main(int argc, char* argv[])
{
	MemoryTest();

	std::cin.get();
	return 0;
}