// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libFunction/CMyFunction.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libTimestamp/Timestamp.h>
#include <libSmart_ptr/Shared_ptr.h>
#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <libFoundation/modafx.h>

#pragma pack(push, 4)
struct DataTest
{
	UInt32	mf;
	UInt32	size;
	std::string s;
	long	y;
};
#pragma pack(pop)


class CTest //, public TR1::enable_shared_from_this<CTest>
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
	}

	void Add123( void )
	{
		//g_P = shared_from_this();
		//shared(shared_from_this());
		//printf("CTest Add:%d\r\n",g_P.use_count());
	}

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

	DataTest* More123( char* p, DataTest* t2, int t3 )
	{
		//printf("CTest More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);	
		return t2;
	}

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

	void TestMony_sptr( int t1, int t2, boost::shared_ptr<CTest> sptr )
	{
		return ;
	}

	void TestFunc(const char* p, const int& num, const std::string& str)
	{
	
	}

	int iu;
};



#define		DEBUG_TYPE0 0
#define		DEBUG_TYPE1 1

#define		PARAMTER_MACRO		&CTest::More,&Test,10,10,10//&Testfunc::te4,&TestFunc,10,20,'4',std::string("123456789")//
//#define		PARAMTER_MACRO		&CTest::More,&Test,10,10,10

#define		DEC_TYPE			int,int,int//int,int,std::string//
#define		PARAMTER_1			&CTest::More,&Test//&CTest::Test,&Test//
#define		VALUE_1				10,10,10//10,10,std::string("123456")//

//MyFunction
void	MyFunction_bind( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::bind(PARAMTER_1);
	}
	printf("MyFunction_bind time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_bind( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_bind(PARAMTER_1,_function_::_1,_function_::_2,_function_::_3);
	}
	printf("MyFunctionV20_bind time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_bind( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::bind(PARAMTER_1,_1,_2,_3); // 无参数，无返回值
	}
	printf("boostFunction_bind time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_bindAndEva( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM = _function_::bind(PARAMTER_1);
	}
	printf("MyFunction_bindAndEva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_bindAndEva( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (DEC_TYPE)> MyDelgM = function20_bind(PARAMTER_1,_function_::_1,_function_::_2,_function_::_3);
	}
	printf("MyFunctionV20_bindAndEva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_bindAndEva( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (DEC_TYPE)> f1 = boost::bind(PARAMTER_1,_1,_2,_3); // 无参数，无返回值
	}
	printf("boostFunction_bindAndEva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	_function_::CMyFunction<void (DEC_TYPE)>	ms(PARAMTER_1);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM = ms;
	}
	printf("MyFunction_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	function20_handle<void (DEC_TYPE)> ms = function20_bind(PARAMTER_1,_function_::_1,_function_::_2,_function_::_3);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (DEC_TYPE)> MyDelgM = ms;
	}
	printf("MyFunctionV20_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::function<void (DEC_TYPE)> ff1 = boost::bind(PARAMTER_1,_1,_2,_3); // 无参数，无返回值

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (DEC_TYPE)> f1 = ff1;
	}
	printf("boostFunction_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_void( void )
{
	/*CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM(PARAMTER_1);
	}
	printf("MyFunction_Construction time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);*/
}

void	MyFunctionV20_void( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (void)> MyDelgM = function20_bind(PARAMTER_1,10,10,10);
	}
	printf("MyFunctionV20_void time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_void( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (void)> f1 = boost::bind(PARAMTER_1,10,10,10);
	}
	printf("boostFunction_void time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}


void	MyFunction_void_invoke( void )
{
	/*CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM(PARAMTER_1);
	}
	printf("MyFunction_Construction time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);*/
}

void	MyFunctionV20_void_invoke( void )
{
	CTest	Test;
	int num = 100000000;
	function20_handle<void (void)> MyDelgM = function20_bind(PARAMTER_1,10,10,10);
	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM();
	}
	printf("MyFunctionV20_void_invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_void_invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::function<void (void)> f1 = boost::bind(PARAMTER_1,10,10,10);
	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1();
	}
	printf("boostFunction_void_invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_ConstructionAndBind( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM(_function_::bind(PARAMTER_1));
	}
	printf("MyFunction_ConstructionAndBind time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_ConstructionAndBind( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (DEC_TYPE)> MyDelgM(function20_bind(PARAMTER_1,_function_::_1,_function_::_2,_function_::_3));
	}
	printf("MyFunctionV20_ConstructionAndBind time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_ConstructionAndBind( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (DEC_TYPE)> f1(boost::bind(PARAMTER_1,_1,_2,_3));
	}
	printf("boostFunction_ConstructionAndBind time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	_function_::CMyFunction<void (DEC_TYPE)> MyDelgM(_function_::bind(PARAMTER_1));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,10);
	}
	printf("MyFunction_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	function20_handle<void (DEC_TYPE)> MyDelgM(function20_bind(PARAMTER_1,_function_::_1,_function_::_2,_function_::_3));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,10);
	}
	printf("MyFunctionV20_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::function<void (DEC_TYPE)> f1(boost::bind(PARAMTER_1,_1,_2,_3));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1(10,10,10);
	}
	printf("boostFunction_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM(_function_::bind(&CTest::More,ppy));
	}
	printf("MyFunction_Share_ptr time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (DEC_TYPE)> MyDelgM(function20_bind(&CTest::More,ppy,_function_::_1,_function_::_2,_function_::_3));
	}
	printf("MyFunctionV20_Share_ptr time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (DEC_TYPE)> f1(boost::bind(&CTest::More,ppy,_1,_2,_3));
	}
	printf("boostFunction_Share_ptr time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM = _function_::bind(&CTest::More,ppy);
	}
	printf("MyFunction_Share_ptr_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (DEC_TYPE)> MyDelgM = function20_bind(&CTest::More,ppy,_function_::_1,_function_::_2,_function_::_3);
	}
	printf("MyFunctionV20_Share_ptr_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (DEC_TYPE)> f1 = boost::bind(&CTest::More,ppy,_1,_2,_3);
	}
	printf("boostFunction_Share_ptr_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_EvaAndConst( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (DEC_TYPE)> MyDelgM = _function_::bind(&CTest::More,ppy);
		_function_::CMyFunction<void (DEC_TYPE)>& MyDelgM1 = MyDelgM;
	}
	printf("MyFunction_Share_ptr_EvaAndConst time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_EvaAndConst( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (DEC_TYPE)> MyDelgM = function20_bind(&CTest::More,ppy,_function_::_1,_function_::_2,_function_::_3);
		const function20_handle<void (DEC_TYPE)>& MyDelgM1 = MyDelgM;
	}
	printf("MyFunctionV20_Share_ptr_EvaAndConst time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_EvaAndConst( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (DEC_TYPE)> f1 = boost::bind(&CTest::More,ppy,_1,_2,_3);
		const boost::function<void (DEC_TYPE)>& f11 = f1;
	}
	printf("boostFunction_Share_ptr_EvaAndConst time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	_function_::CMyFunction<void (DEC_TYPE)> MyDelgM = _function_::bind(&CTest::More,ppy);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,10);
	}
	printf("MyFunction_Share_ptr_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	function20_handle<void (DEC_TYPE)> MyDelgM(function20_bind(&CTest::More,ppy,_function_::_1,_function_::_2,_function_::_3));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,10);
	}
	printf("MyFunctionV20_Share_ptr_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::function<void (DEC_TYPE)> f1 = boost::bind(&CTest::More,ppy,_1,_2,_3);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1(10,10,10);
	}
	printf("boostFunction_Share_ptr_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_paramter( void )
{
	CTest	Test;
	int num = 100000000;

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (int,int,boost::shared_ptr<CTest>)> MyDelgM(_function_::bind(&CTest::TestMony_sptr,&Test));
	}
	printf("MyFunction_Share_ptr_paramter time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_paramter( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (int,int,boost::shared_ptr<CTest>)> MyDelgM(function20_bind(&CTest::TestMony_sptr,&Test,
										_function_::_1,_function_::_2,_function_::_3));
	}
	printf("MyFunctionV20_Share_ptr_paramter time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_paramter( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (int,int,boost::shared_ptr<CTest>)> f1(boost::bind(&CTest::TestMony_sptr,&Test,_1,_2,_3));
	}
	printf("boostFunction_Share_ptr_paramter time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_paramter_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (int,int,boost::shared_ptr<CTest>)> MyDelgM = _function_::bind(&CTest::TestMony_sptr,&Test);
	}
	printf("MyFunction_Share_ptr_paramter_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_paramter_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (int,int,boost::shared_ptr<CTest>)> MyDelgM = function20_bind(&CTest::TestMony_sptr,&Test,
										_function_::_1,_function_::_2,_function_::_3);
	}
	printf("MyFunctionV20_Share_ptr_paramter_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_paramter_Eva( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (int,int,boost::shared_ptr<CTest>)> f1 = boost::bind(&CTest::TestMony_sptr,&Test,_1,_2,_3);
	}
	printf("boostFunction_Share_ptr_paramter_Eva time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_paramter_EvaAndConst( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (int,int,boost::shared_ptr<CTest>)> MyDelgM = _function_::bind(&CTest::TestMony_sptr,&Test);
		_function_::CMyFunction<void (int,int,boost::shared_ptr<CTest>)>& MyDelgM1 = MyDelgM;
	}
	printf("MyFunction_Share_ptr_paramter_EvaAndConst time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_paramter_EvaAndConst( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (int,int,boost::shared_ptr<CTest>)> MyDelgM = function20_bind(&CTest::TestMony_sptr,&Test,
									_function_::_1,_function_::_2,_function_::_3);
		const function20_handle<void (int,int,boost::shared_ptr<CTest>)>& MyDelgM1 = MyDelgM;
	}
	printf("MyFunctionV20_Share_ptr_EvaAndConst time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_paramter_EvaAndConst( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (int,int,boost::shared_ptr<CTest>)> f1 = boost::bind(&CTest::TestMony_sptr,&Test,_1,_2,_3);
		const boost::function<void (int,int,boost::shared_ptr<CTest>)>& f11 = f1;
	}
	printf("boostFunction_Share_ptr_paramter_EvaAndConst time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_Share_ptr_paramter_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	_function_::CMyFunction<void (int,int,boost::shared_ptr<CTest>)> MyDelgM = _function_::bind(&CTest::TestMony_sptr,&Test);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,ppy);
	}
	printf("MyFunction_Share_ptr_paramter_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_Share_ptr_paramter_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	function20_handle<void (int,int,boost::shared_ptr<CTest>)> MyDelgM = function20_bind(&CTest::TestMony_sptr,&Test,
											_function_::_1,_function_::_2,_function_::_3);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,ppy);
	}
	printf("MyFunctionV20_Share_ptr_paramter_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_paramter_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::function<void (int,int,boost::shared_ptr<CTest>)> f1 = boost::bind(&CTest::TestMony_sptr,&Test,_1,_2,_3);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1(10,10,ppy);
	}
	printf("boostFunction_Share_ptr_paramter_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_string( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		_function_::CMyFunction<void (int,int,std::string)> MyDelgM(_function_::bind(&CTest::Test,&Test));
	}
	printf("MyFunction_string time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_string( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		function20_handle<void (int,int,std::string)> MyDelgM(function20_bind(&CTest::Test,&Test,
								_function_::_1,_function_::_2,_function_::_3));
	}
	printf("MyFunctionV20_string time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_string( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		boost::function<void (int,int,std::string)> f1(boost::bind(&CTest::Test,&Test,_1,_2,_3));
	}
	printf("boostFunction_string time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunction_string_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	_function_::CMyFunction<void (int,int,std::string)> MyDelgM(_function_::bind(&CTest::Test,&Test));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,"123456");
	}
	printf("MyFunction_string_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	MyFunctionV20_string_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	function20_handle<void (int,int,std::string)> MyDelgM(function20_bind(&CTest::Test,&Test,
								_function_::_1,_function_::_2,_function_::_3));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM(10,10,"123456");
	}
	printf("MyFunctionV20_string_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_string_Invoke( void )
{
	CTest	Test;
	int num = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::function<void (int,int,std::string)> f1(boost::bind(&CTest::Test,&Test,_1,_2,_3));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1(10,10,"123456");
	}
	printf("boostFunction_string_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

int main(int argc, char* argv[])
{	
	int i = 0, iNum = 5;

	{
		const char* StrC1 = "0123456789 abcdef";
		const char* StrC2 = "@#$%&^0123456789 abcdef";

		_foundation_::ScanChar Scan;
		Scan.AddNumberCharacter();
		Scan.AddLetterCharacter();

		do
		{
			char c = Scan.GetChar((char**)&StrC1);
			char c1 = Scan.GetChar((char**)&StrC2, true);

			int i = 0;

		} while (true);


		do
		{
			_foundation_::_string_t s = Scan.GetString((char**)&StrC2);

			s = Scan.GetString((char**)&StrC2);
			int i = 0;
		} while (true);
	}

	printf("bind begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_bind();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_bind();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_bind();
	}

	printf("bindAndEva begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_bindAndEva();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_bindAndEva();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_bindAndEva();
	}

	printf("Eva begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Eva();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Eva();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Eva();
	}


	printf("void begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_void();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_void();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_void();
	}

	printf("void_invoke begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_void_invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_void_invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_void_invoke();
	}

	printf("ConstructionAndBind begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_ConstructionAndBind();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_ConstructionAndBind();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_ConstructionAndBind();
	}
	
	printf("Invoke begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Invoke();
	}

	printf("Share_ptr begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr();
	}

	printf("Share_ptr_Eva begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_Eva();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_Eva();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_Eva();
	}

	printf("Share_ptr_EvaAndConst begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_EvaAndConst();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_EvaAndConst();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_EvaAndConst();
	}

	printf("Share_ptr_Invoke begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_Invoke();
	}

	printf("Share_ptr_paramter begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_paramter();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_paramter();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_paramter();
	}

	printf("Share_ptr_paramter_Eva begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_paramter_Eva();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_paramter_Eva();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_paramter_Eva();
	}

	printf("Share_ptr_paramter_EvaAndConst begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_paramter_EvaAndConst();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_paramter_EvaAndConst();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_paramter_EvaAndConst();
	}

	printf("Share_ptr_paramter_Invoke begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_Share_ptr_paramter_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_paramter_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_paramter_Invoke();
	}

	printf("string begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_string();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_string();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_string();
	}

	printf("string_Invoke begin\r\n");
	for( i = 0; i < iNum; i ++)
	{
		MyFunction_string_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_string_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_string_Invoke();
	}

	std::cin.get();
	return 0;
}