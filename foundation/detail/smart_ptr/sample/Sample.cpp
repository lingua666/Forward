// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <libTimestamp/Timestamp.h>
#include <libSmart_ptr/SmartList.h>
#include <libCommon/CASAtomic.h>

#define SmartPtr	_smart_ptr_::SmartPtr

#define FOR_NUMBER	100000000

class CTest //: public TR1::enable_shared_from_this<CTest>
{
public:
	CTest( void )
	{
		//printf("CTest constructor\r\n");
	}

	~CTest( void )
	{
		//printf("CTest destructor\r\n");
	}

	virtual void Add( void )
	{
		//g_P = shared_from_this();
		//shared(shared_from_this());
		//printf("CTest Add:%d\r\n",g_P.use_count());
	}

	void Add123( void )
	{
		//g_P = shared_from_this();
		//shared(shared_from_this());
		//printf("CTest Add:%d\r\n",g_P.use_count());
	}

	/*void invoke( _myfunction::_anyfunc::AnyFunction<void (void)>* MyAny )
	{
		//*MyAny = _myfunction::_anyfunc::bind(&CTest::sss,shared_from_this());
	}*/

	void	sss( void )
	{
		printf("sss\r\n");
	}

	int yst( void )
	{
		return 500;
	}

	void More( int t1, int t2, int t3 )
	{
		//printf("CTest More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);	
		return ;
	}

	/*DataTest* More123( char* p, DataTest* t2, int t3 )
	{
		//printf("CTest More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);	
		return t2;
	}*/

	void More1234( int t1, char* p, int t2, int t3 )
	{
		//printf("CTest More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);	
		//return t2;
	}

	void Test( int t1, int t2, std::string text )
	{
		return ;
	}

	void TestMony( int t1, std::string text1, std::string text2, std::string text3 )
	{
		return ;
	}

	void TestMony_sptr( int t1, boost::shared_ptr<int> sptr )
	{
		return ;
	}

	void TestFunc(const char* p, const int& num, const std::string& str)
	{
	
	}

	int iu;
};

void SharePtrTest( const boost::shared_ptr<CTest>& ppy )
{
	boost::shared_ptr<CTest> p1 = ppy; 
}

void SmartPtrTest( const SmartPtr<CTest>& ppy )
{
	SmartPtr<CTest> p1 = ppy; 
}

void PoolTest( void* p )
{
	int i = 0;
	i ++;
}

//static MemPool_type	g_Pool;

CTest* GetTestObj()
{
	//return g_Pool.AllocObj<CTest>();
	return NULL;
}

void FreeTestObj( void* p )
{
	//g_Pool.FreeObj(reinterpret_cast<CTest*>(p));
	//delete reinterpret_cast<CTest*>(p);
}

void MySmart_ptr_Test_obj( void )
{
	int iNumou = FOR_NUMBER;
	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		SmartPtr<CTest>	pp(new CTest);
		//SmartPtr<CTest>	pp(GetTestObj(), function20_bind_Ex(FreeTestObj));
	}
	printf("MySmart_ptr_Test_obj time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void boostSmart_ptr_Test_obj( void )
{
	int iNumou = FOR_NUMBER;
	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		boost::shared_ptr<CTest> pp(new CTest);
		//boost::shared_ptr<CTest> pp(GetTestObj(), FreeTestObj);
	}
	printf("boostSmart_ptr_Test_obj time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void MySmart_ptr_Test_eva( void )
{
	int iNumou = FOR_NUMBER;
	SmartPtr<CTest>	pp(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		SmartPtr<CTest> pp1 = pp;
	}
	printf("MySmart_ptr_Test_eva1 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void boostSmart_ptr_Test_eva( void )
{
	int iNumou = FOR_NUMBER;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		boost::shared_ptr<CTest>	ppy1 = ppy;
	}
	printf("boostSmart_ptr_Test_eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void MySmart_ptr_Test_eva2( void )
{
	int iNumou = FOR_NUMBER;
	SmartPtr<CTest>	pp(new CTest);

	SmartPtr<CTest> pp1;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		pp1 = pp;
	}
	printf("MySmart_ptr_Test_eva2 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void boostSmart_ptr_Test_eva2( void )
{
	int iNumou = FOR_NUMBER;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::shared_ptr<CTest>	ppy1;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		ppy1 = ppy;
	}
	printf("boostSmart_ptr_Test_eva2 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void MySmart_ptr_Test_paramter( void )
{
	int iNumou = FOR_NUMBER;
	SmartPtr<CTest>	pp(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		SmartPtrTest(pp);
	}
	printf("MySmart_ptr_Test_paramter time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void boostSmart_ptr_Test_paramter( void )
{
	int iNumou = FOR_NUMBER;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		SharePtrTest(ppy);
	}

	printf("boostSmart_ptr_Test_paramter time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void MySmart_ptr_Test_Function( void )
{
	int iNumou = FOR_NUMBER;
	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		SmartPtr<CTest>	pp(new CTest, function20_bind_Ex(FreeTestObj));
	}
	printf("MySmart_ptr_Test_Function time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void boostSmart_ptr_Test_Function( void )
{
	int iNumou = FOR_NUMBER;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < iNumou; i ++)
	{
		boost::shared_ptr<CTest> pp(new CTest, FreeTestObj);
	}

	printf("boostSmart_ptr_Test_Function time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

int main(int argc, char* argv[])
{
	/*while(true)
	{
		_smart_ptr::weak_ptr<CTest>	pst;
		SmartPtr<CTest> Smt = pst;
		{
			SmartPtr<CTest> Smart;
			{
				SmartPtr<CTest> po;
				{
					SmartPtr<CTest>	pp(new CTest );
					po = pp;
					Smart = po;
				}
			}
			Smart = SmartPtr<CTest>(new CTest);
			int iNum = Smart.use_count();
			pst = Smart;
			iNum = Smart.use_count();
			Smt = pst;
			iNum = Smt.use_count();
			int i = 0;
		}
	}*/

	{
		_smart_ptr_::SMartList<int, int>		yslw;
	}

	while(true)
	{
		boost::shared_ptr<CTest>	ppy(new CTest);
		{
			SmartPtr<CTest>	pp(new CTest, function20_bind_Ex(PoolTest) );
			{
				SmartPtr<CTest> po = pp;
				int i;
			}
		}

		int i = 0, iNum = 5;
		printf("obj begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
			MySmart_ptr_Test_obj();
		}

		for( i = 0; i < iNum; i ++)
		{
			boostSmart_ptr_Test_obj();
		}

		printf("eva begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
			MySmart_ptr_Test_eva();
		}

		for( i = 0; i < iNum; i ++)
		{
			boostSmart_ptr_Test_eva();
		}

		printf("eva2 begin\r\n");
		for( i = 0; i < iNum; i ++)
		{
			MySmart_ptr_Test_eva2();
		}

		for( i = 0; i < iNum; i ++)
		{
			boostSmart_ptr_Test_eva2();
		}

		printf("MySmart_ptr_Test_paramter\r\n");
		for( i = 0; i < iNum; i ++)
		{
			MySmart_ptr_Test_paramter();
		}

		for( i = 0; i < iNum; i ++)
		{
			boostSmart_ptr_Test_paramter();
		}

		printf("MySmart_ptr_Test_Function\r\n");
		for( i = 0; i < iNum; i ++)
		{
			MySmart_ptr_Test_Function();
		}

		for( i = 0; i < iNum; i ++)
		{
			boostSmart_ptr_Test_Function();
		}

		Sleep(10);
	}

	std::cin.get();
	return 0;
}