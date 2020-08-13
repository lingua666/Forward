// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#include <libSmart_ptr/Shared_ptr.h>
#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>

#include <libFunction/CMyFunction.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libTimestamp/Timestamp.h>

#define NUM_DEF 100000000


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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;

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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;

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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::function<void (int,int,boost::shared_ptr<CTest>)> f1 = boost::bind(&CTest::TestMony_sptr,&Test,_1,_2,_3);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1(10,10,ppy);
	}
	printf("boostFunction_Share_ptr_paramter_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}


void	MyFunctionV20_Share_ptr_void_Invoke( void )
{
	CTest	Test;
	int num = NUM_DEF;
	boost::shared_ptr<CTest>	ppy(new CTest);
	function20_handle<void (void)> MyDelgM = function20_bind(&CTest::TestMony_sptr,&Test,
		10,10,ppy);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		MyDelgM();
	}
	printf("MyFunctionV20_Share_ptr_void_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

void	boostFunction_Share_ptr_void_Invoke( void )
{
	CTest	Test;
	int num = NUM_DEF;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::function<void (void)> f1 = boost::bind(&CTest::TestMony_sptr,&Test,10,10,ppy);

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1();
	}
	printf("boostFunction_Share_ptr_void_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}


void	MyFunction_string( void )
{
	CTest	Test;
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
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
	int num = NUM_DEF;
	boost::shared_ptr<CTest>	ppy(new CTest);
	boost::function<void (int,int,std::string)> f1(boost::bind(&CTest::Test,&Test,_1,_2,_3));

	_timestamp_::Timestamp	time;
	for(int i = 0; i < num; i ++)
	{
		f1(10,10,"123456");
	}
	printf("boostFunction_string_Invoke time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
}

#include <boost/regex.hpp>

void RegexTest( void )
{
	/*std::string str("abcd");
	boost::regex reg( "a\\w*d" );

	if (regex_match(str, reg))
	{
		std::cout << str << " is match" << std::endl;
	}
	else
	{
	  std::cout << str << " is not match" << std::endl;
	}*/
}


#define BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/front.hpp>

/*template< class T = void> struct vector0 { typedef v_iter<vector0<>, boost::mpl::int_<0> > begin; typedef mpl::int_<1> item0; typedef mpl::int_<1> item1; typedef mpl::int_<1> item2; typedef mpl::int_<1> item3; typedef mpl::int_<1> item4; typedef mpl::int_<1> item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 > struct vector1 { typedef v_iter<vector1< P0>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef mpl::int_<1> item1; typedef mpl::int_<1> item2; typedef mpl::int_<1> item3; typedef mpl::int_<1> item4; typedef mpl::int_<1> item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 > struct vector2 { typedef v_iter<vector2< P0 , P1>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef mpl::int_<1> item2; typedef mpl::int_<1> item3; typedef mpl::int_<1> item4; typedef mpl::int_<1> item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 > struct vector3 { typedef v_iter<vector3< P0 , P1 , P2>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef mpl::int_<1> item3; typedef mpl::int_<1> item4; typedef mpl::int_<1> item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 , class P3 > struct vector4 { typedef v_iter<vector4< P0 , P1 , P2 , P3>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef mpl::int_<1> item4; typedef mpl::int_<1> item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 , class P3 , class P4 > struct vector5 { typedef v_iter<vector5< P0 , P1 , P2 , P3 , P4>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef mpl::int_<1> item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 > struct vector6 { typedef v_iter<vector6< P0 , P1 , P2 , P3 , P4 , P5>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef mpl::int_<1> item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 > struct vector7 { typedef v_iter<vector7< P0 , P1 , P2 , P3 , P4 , P5 , P6>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef P6 item6; typedef mpl::int_<1> item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 > struct vector8 { typedef v_iter<vector8< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef P6 item6; typedef P7 item7; typedef mpl::int_<1> item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 > struct vector9 { typedef v_iter<vector9< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8>, boost::mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef P6 item6; typedef P7 item7; typedef P8 item8; typedef mpl::int_<1> item9; typedef mpl::int_<1> item10; typedef mpl::int_<1> item11; typedef mpl::int_<1> item12; typedef mpl::int_<1> item13; typedef mpl::int_<1> item14; typedef mpl::int_<1> item15; typedef mpl::int_<1> item16; typedef mpl::int_<1> item17; typedef mpl::int_<1> item18; typedef mpl::int_<1> item19; typedef mpl::int_<1> item20; typedef mpl::int_<1> item21; typedef mpl::int_<1> item22; typedef mpl::int_<1> item23; typedef mpl::int_<1> item24; typedef mpl::int_<1> item25; typedef mpl::int_<1> item26; typedef mpl::int_<1> item27; typedef mpl::int_<1> item28; typedef mpl::int_<1> item29; typedef mpl::int_<1> item30; typedef mpl::int_<1> item31; typedef mpl::int_<1> item32; typedef mpl::int_<1> item33; typedef mpl::int_<1> item34; typedef mpl::int_<1> item35; typedef mpl::int_<1> item36; typedef mpl::int_<1> item37; typedef mpl::int_<1> item38; typedef mpl::int_<1> item39; typedef mpl::int_<1> item40; typedef mpl::int_<1> item41; typedef mpl::int_<1> item42; typedef mpl::int_<1> item43; typedef mpl::int_<1> item44; typedef mpl::int_<1> item45; typedef mpl::int_<1> item46; typedef mpl::int_<1> item47; typedef mpl::int_<1> item48; typedef mpl::int_<1> item49; typedef mpl::int_<1> item50; typedef mpl::int_<1> item51; typedef mpl::int_<1> item52; typedef mpl::int_<1> item53; typedef mpl::int_<1> item54; typedef mpl::int_<1> item55; typedef mpl::int_<1> item56; typedef mpl::int_<1> item57; typedef mpl::int_<1> item58; typedef mpl::int_<1> item59; typedef mpl::int_<1> item60; typedef mpl::int_<1> item61; typedef mpl::int_<1> item62; typedef mpl::int_<1> item63; typedef mpl::int_<1> item64; typedef mpl::int_<1> item65; typedef mpl::int_<1> item66; typedef mpl::int_<1> item67; typedef mpl::int_<1> item68; typedef mpl::int_<1> item69; typedef mpl::int_<1> item70; typedef mpl::int_<1> item71; typedef mpl::int_<1> item72; typedef mpl::int_<1> item73; typedef mpl::int_<1> item74; typedef mpl::int_<1> item75; typedef mpl::int_<1> item76; typedef mpl::int_<1> item77; typedef mpl::int_<1> item78; typedef mpl::int_<1> item79; typedef mpl::int_<1> item80; typedef mpl::int_<1> item81; typedef mpl::int_<1> item82; typedef mpl::int_<1> item83; typedef mpl::int_<1> item84; typedef mpl::int_<1> item85; typedef mpl::int_<1> item86; typedef mpl::int_<1> item87; typedef mpl::int_<1> item88; typedef mpl::int_<1> item89; typedef mpl::int_<1> item90; typedef mpl::int_<1> item91; typedef mpl::int_<1> item92; typedef mpl::int_<1> item93; typedef mpl::int_<1> item94; typedef mpl::int_<1> item95; typedef mpl::int_<1> item96; typedef mpl::int_<1> item97; typedef mpl::int_<1> item98; typedef mpl::int_<1> item99; };*/

template<typename Class,typename Type,Type Class::*PtrToMember>
struct TemTest
{
	const Type& operator()(const Class& x) const
	{
		return x.*PtrToMember;
	}
};

using namespace boost;
using namespace boost::multi_index;
using namespace std;
struct Employee{
	int id;
	string name;
	int age;

	Employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}

	friend std::ostream& operator<<(std::ostream& os,const Employee& e)
	{
		os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
		return os;
	}
};

struct id{};	// 索引-学号
struct name{};  // 索引-姓名
struct age{};	// 索引-年龄

typedef multi_index_container<
	Employee,
	indexed_by<
	ordered_unique<tag<id>, member<Employee, int, &Employee::id> >,
	ordered_non_unique<tag<name>,member<Employee, string, &Employee::name> >,
	ordered_non_unique<tag<age>,member<Employee, int, &Employee::age> >
	>
> EmployeeContainer;

typedef EmployeeContainer::nth_index<0>::type IdIndex;
typedef EmployeeContainer::nth_index<1>::type NameIndex;
typedef EmployeeContainer::nth_index<2>::type AgeIndex;



/*at_c
_mpl_nttp_long
mpl::long_
mpl::void_
false_*/

class Conver
{

public:
	int b;
	operator int(){return b;} //注：这里是把函数的实现直接写出来
};

//test
namespace slw{

	template<int N,typename Value,typename IndexSpecifierList>
	struct nth_layer
	{
		BOOST_STATIC_CONSTANT(int,length=mpl::size<IndexSpecifierList>::value);

		typedef typename  mpl::if_c<
			N==length,
			int,
			float
		>::type type;

		typedef typename mpl::apply2<
			boost::multi_index::detail::index_applier,
			mpl::at_c<IndexSpecifierList,N>,
			nth_layer<N+1,Value,IndexSpecifierList>
		> type123;
	};

	 

	template<typename Value,typename IndexSpecifierList>
	struct multi_index_base_type:nth_layer<0,Value,IndexSpecifierList>
	{
		
	};
}
//test

#define NUM1_DEC	10000000//30000000//100000000

struct Employee_test{
	int id;
	std::string name;
	std::string  age;

	Employee_test(int id_,std::string name_,std::string age_):id(id_),name(name_),age(age_){}
};

struct id_test{};	// 索引-学号
struct name_test{};  // 索引-姓名
struct age_test{};	// 索引-年龄

#define multi_index_container_t boost::multi_index::multi_index_container
#define vector_t				mpl::vector	
#define ordered_unique_t		boost::multi_index::ordered_unique
#define ordered_non_unique_t	boost::multi_index::ordered_non_unique
#define member_t				boost::multi_index::member	
#define tag_t					boost::multi_index::tag

typedef multi_index_container_t<
	Employee_test,
	vector_t<
	ordered_unique_t<tag_t<id_test>, member_t<Employee_test, int, &Employee_test::id> >,
	ordered_non_unique_t<tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> >,
	ordered_non_unique_t<tag_t<age_test>,member_t<Employee_test, std::string, &Employee_test::age> >
	>
> EmployeeContainer_test;

void MapTest( void )
{
	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t<tag_t<id_test>, member_t<Employee_test, int, &Employee_test::id> > >
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			con.insert(Employee_test(i,"Joe1",""));
		}
		printf("multi_index_container<int,int> insert timer:%d\r\n", GetTickCount() - iTimer);

		/*iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("map<int,int> find timer:%d\r\n", GetTickCount() - iTimer);*/
	}

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t<tag_t<id_test>, member_t<Employee_test, int, &Employee_test::id> >,
			ordered_non_unique_t<tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> > >
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;
		char sz[100] = {0};

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			con.insert(Employee_test(i,sz,""));
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container insert timer:%d\r\n", msecond);

		test_multi_index::index< id_test >::type& indexOID = con.get< id_test >();
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			test_multi_index::index<id_test>::type::iterator it2 = indexOID.find(i);
			if( it2 == indexOID.end() )
			{
				printf("Key:%d Not Found!!\r\n", i);
			}
		}
		printf("multi_index_container<int,string> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t< tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> > >
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			con.insert(Employee_test(i,sz,""));
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<string,int> insert timer:%d\r\n", msecond);

		test_multi_index::index< name_test >::type& indexOfName = con.get< name_test >();
		msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			test_multi_index::index<name_test>::type::iterator it2 = indexOfName.find(sz);
			if( it2 == indexOfName.end() )
			{
				printf("Key:%s Not Found!!\r\n", sz);
			}
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<string,int> find timer:%d\r\n", msecond);
	}

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t< tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> >,
			ordered_non_unique_t< tag_t<age_test>,member_t<Employee_test, std::string, &Employee_test::age> >>
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			con.insert(Employee_test(i,Uid,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<std::string,std::string> insert timer:%d\r\n", msecond);

		msecond = 0;
		test_multi_index::index< name_test >::type& indexOfName = con.get< name_test >();
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			test_multi_index::index<name_test>::type::iterator it2 = indexOfName.find(Uid);
			if( it2 == indexOfName.end() )
			{
				printf("Key:%s Not Found!!\r\n", sz);
			}
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<std::string,std::string> find timer:%d\r\n", msecond);
	}

	printf("end\r\n");
}

void multi_index_container_Test( void )
{
	 typedef boost::mpl::eval_if_c<
		true,
		boost::mpl::identity< int >,
		boost::mpl::identity< long >
	>::type type;

	 printf("T:%s\n",typeid(type).name());

	printf("T:%s\n",typeid(EmployeeContainer::node_type).name());
	printf("T:%s\n",typeid(EmployeeContainer::value_type).name());

	typedef boost::multi_index::detail::multi_index_node_type< Employee,
		indexed_by<
		ordered_unique<tag<id>, member<Employee, int, &Employee::id> >,
		ordered_non_unique<tag<name>,member<Employee, string, &Employee::name> >,
		ordered_non_unique<tag<age>,member<Employee, int, &Employee::age> >
		>, EmployeeContainer::allocator_type > multi_index_node_type_test;
	printf("T:%s\n",typeid(multi_index_node_type_test::type).name());

	int i8 = 10;
	boost::reference_wrapper<int> sd(i8);

	tag<id> de1;
	tag<id>::type de;

	typedef mpl::vector<float,double,long double> floats11;
	typedef mpl::push_back<floats11,int>::type types11;
	//typedef mpl::push_front<mpl::vector<double>,int>::type types12;

	//mpl::apply<types11,mpl::long_<3>> lsj;

	/*types12 sw;
	printf("T:%s",typeid(sw).name());*/

	std::tr1::is_same< mpl::at_c<types11,3>::type, int >;

	//printf("T:%s",typeid(mpl::at_c<types11,3>).name());
	printf("T:%s",typeid(mpl::apply<types11,mpl::long_<3>>).name());

	mpl::vector<int,int> sv;
	floats11 sw19;
	types11 iu12;

	mpl::int_<0> ku;

	typedef mpl::push_back<EmployeeContainer::index_type_list,int>::type types99;
	types99::type ls87;
	printf("T:%s",typeid(ls87).name());

	printf("T:%s",typeid(iu12).name());

	typedef slw::multi_index_base_type<Employee,
		indexed_by<
		ordered_unique<tag<id>, member<Employee, int, &Employee::id> >,
		ordered_non_unique<tag<name>,member<Employee, string, &Employee::name> >,
		ordered_non_unique<tag<age>,member<Employee, int, &Employee::age> >
		>>::type lwj;
	printf("T:%s",typeid(lwj).name());

	lwj d23;

	indexed_by<
		ordered_unique<tag<id>, member<Employee, int, &Employee::id> >,
	ordered_non_unique<tag<name>,member<Employee, string, &Employee::name>>> poi;

	int y = mpl::size<indexed_by<
		ordered_unique<tag<id>, member<Employee, int, &Employee::id> >>
		>::value;

	typedef mpl::vector<> floats;
	typedef mpl::push_back<floats,int>::type types;

	//mpl::back<types>::type;
	//mpl::front<types1>::type;
	bool bl = std::tr1::is_same< mpl::empty<types>::type, mpl::false_ >::value;
	bool bl1 = mpl::empty<types>::value;

	std::cout << typeid(mpl::back<types>::type).name() << std::endl; 
	std::cout << typeid(mpl::front<types>::type).name() << std::endl; 
	std::cout << typeid(mpl::at_c<types,0>::type).name() << std::endl;  
	std::cout << typeid(mpl::at_c<types,1>::type).name() << std::endl;  
	/*std::cout << typeid(mpl::at_c<types,2>::type).name() << std::endl;  
	std::cout << typeid(mpl::at_c<types,3>::type).name() << std::endl;*/

	Employee Ttw(110,"Joe",31);
	TemTest<Employee, int, &Employee::id> uuw;
	int i = uuw(Ttw);

	EmployeeContainer::node_type ls3;

	EmployeeContainer con;
	con.insert(Employee(0,"Joe",31));
	con.insert(Employee(1,"Joe",31));
	con.insert(Employee(2,"Robert",27));
	con.insert(Employee(3,"John",40));

	//con.clear();

	EmployeeContainer::index<id>::type& indexOfIndex = con.get<id>();
	indexOfIndex.erase(0);

	// 用名字作为索引
	EmployeeContainer::index<name>::type& indexOfName = con.get<name>();

	EmployeeContainer::index< name >::type::iterator it321 = indexOfName.find("John");
	if( it321 != indexOfName.end() )  
	{
		bool ls = indexOfName.replace(it321, Employee(3,"Eohn",40));
		int i = 0;

		for( it321 = indexOfName.begin(); it321 != indexOfName.end(); ++ it321 )
		{
			printf("%d,%s,%d\n",it321->id,it321->name.c_str(),it321->age) ;
		}
	}

	indexOfName.erase("Joe");
	 
	// 查找名叫李四的人
	EmployeeContainer::index<name>::type::iterator it = indexOfName.find("Joe");


	EmployeeContainer::index<id>::type& indexOfid = con.get<id>();
	EmployeeContainer::index<id>::type::iterator itid = indexOfid.find(0);

	// 找到了？
	if( it != indexOfName.end() )
	{
		// it就是一个Student序列的迭代器，现在你可以
		// 像普通迭代器一样操作它了，比如cout << *it
		cout << it->id << endl;
		cout << it->name << endl;
		cout << it->age << endl;
	}

	IdIndex& ids = con.get<0>();
	copy(ids.begin(),ids.end(), ostream_iterator<Employee>(cout));
	cout << endl;

	NameIndex& names = con.get<1>();
	copy(names.begin(), names.end(), ostream_iterator<Employee>(cout));
	cout << endl;

	names.erase(names.begin());

	AgeIndex& ages = con.get<2>();
	copy(ages.begin(), ages.end(), ostream_iterator<Employee>(cout));
	cout << endl;
}


namespace _test_{

	template<int N> struct the_counter;

	template<typename T,int N = 5>
	struct encode_counter
	{
		__if_exists(the_counter<N + 256>)
		{
			static const unsigned count = encode_counter<T,N + 257>::count;
		}
		__if_not_exists(the_counter<N + 256>)
		{
			__if_exists(the_counter<N + 64>)
			{
				static const unsigned count = encode_counter<T,N + 65>::count;
			}
			__if_not_exists(the_counter<N + 64>)
			{
				__if_exists(the_counter<N + 16>)
				{
					static const unsigned count = encode_counter<T,N + 17>::count;
				}
				__if_not_exists(the_counter<N + 16>)
				{
					__if_exists(the_counter<N + 4>)
					{
						static const unsigned count = encode_counter<T,N + 5>::count;
					}
					__if_not_exists(the_counter<N + 4>)
					{
						__if_exists(the_counter<N>)
						{
							static const unsigned count = encode_counter<T,N + 1>::count;
						}
						__if_not_exists(the_counter<N>)
						{
							static const unsigned count = N;
							typedef the_counter<N> type;
						}
					}
				}
			}
		}
	};

	# define BOOST_TYPEOF_INDEX(T) (encode_counter<T>::count)

	struct msvc_extract_type_default_param {};

	template<typename ID, typename T = msvc_extract_type_default_param>
	struct msvc_extract_type;

	template<typename ID>
	struct msvc_extract_type<ID, msvc_extract_type_default_param> {
		template<bool>
		struct id2type_impl;

		typedef id2type_impl<true> id2type;
	};

	template<typename ID, typename T>
	struct msvc_extract_type : msvc_extract_type<ID,msvc_extract_type_default_param>
	{
		template<>
		struct id2type_impl<true>  //VC8.0 specific bugfeature
		{
			typedef T type;
		};
		template<bool>
		struct id2type_impl;

		typedef id2type_impl<true> id2type;
	};

	template<typename T, typename ID>
	struct msvc_register_type : msvc_extract_type<ID, T>
	{
	};

	template<int ID>
	struct msvc_typeid_wrapper {
		typedef typename msvc_extract_type<boost::mpl::int_<ID> >::id2type id2type;
		typedef typename id2type::type type;
	};
	//Workaround for ETI-bug for VC6 and VC7
	template<>
	struct msvc_typeid_wrapper<1> {
		typedef msvc_typeid_wrapper<1> type;
	};
	//Workaround for ETI-bug for VC7.1
	template<>
	struct msvc_typeid_wrapper<4> {
		typedef msvc_typeid_wrapper<4> type;
	};

	//Tie it all together
	template<typename T>
	struct encode_type
	{
		static const unsigned value = BOOST_TYPEOF_INDEX(T);
		typedef typename msvc_register_type< T,boost::mpl::int_<value> >::id2type type;
	};

	template<class T>
	struct sizer
	{
		typedef char(*type)[encode_type<T>::value];
	};

template <bool B, class T = void>
struct disable_if_c {
	typedef T type;
};

template <class T>
struct disable_if_c<true, T> {};

template <class Cond, class T = void> 
struct disable_if : public disable_if_c<Cond::value, T> {};

template <bool B, class T = void>
struct enable_if_c {
	typedef T type;
};

template <class T>
struct enable_if_c<false, T> {};

template <class Cond, class T = void> 
struct enable_if : public enable_if_c<Cond::value, T> {};

/*template<typename T> typename disable_if<
            typename is_function<T>::type,
            typename sizer<T>::type>::type encode_start(T const&);

template<typename T> typename enable_if<
            typename is_function<T>::type,
            typename sizer<T>::type>::type encode_start(T&);*/

template<typename T> 
typename sizer<T>::type encode_start(T const&);

template<typename T> 
typename sizer<T>::type encode_start(T&); 

}

/*# define BOOST_TYPEOF(expr) \
	boost::type_of::msvc_typeid_wrapper<sizeof(*boost::type_of::encode_start(expr))>::type*/

# define BOOST_TYPEOF_TEST(expr) \
	_test_::msvc_typeid_wrapper<sizeof(*_test_::encode_start(expr))>::type

struct st_lwj
{
	st_lwj( void )
	{

	}

	int y;
};

struct st_lwj1
{
	st_lwj1( void )
	{

	}

	int y;
};


template<typename T>
struct encode_type_12
{
	static const unsigned value = sizeof(T);
	
	encode_type_12( void ) : _k(value)
	{}

	~encode_type_12( void )
	{}

	int _k;
};

int main(int argc, char* argv[])
{
	_string_type slw = "123445";
	slw.update_size(3);

	_string_type slwww  = slw;

	st_lwj ooo;
	st_lwj1 yyy;


	encode_type_12<st_lwj> lw;
	encode_type_12<st_lwj1> lw2;

	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(ooo)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(yyy)).name());

	char iu;
	int yw = 0;
	TemTest<Employee, int, &Employee::id> uuw;

	//int slw = sizeof(boost::mpl::int_<int>);
	yw = sizeof(*boost::type_of::encode_start(uuw));

	printf("typeid:%s\r\n", typeid(*boost::type_of::encode_start(uuw)).name());

	printf("typeid:%s\r\n", typeid(boost::type_of::encode_start(uuw)).name());

	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF(iu)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF(yw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF(ooo)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF(yyy)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF(uuw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(uuw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(iu)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(uuw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(iu)).name());
	printf("typeid:%s\r\n", typeid(BOOST_TYPEOF_TEST(yw)).name());

	/*MapTest();
	while(true)
	{
		Sleep(1000);
	}*/

	multi_index_container_Test();

	RegexTest();

	int i = 0, iNum = 5;

	printf("Share_ptr_void_Invoke\r\n"); 

	for( i = 0; i < iNum; i ++)
	{
		boostFunction_Share_ptr_void_Invoke();
	}

	for( i = 0; i < iNum; i ++)
	{
		MyFunctionV20_Share_ptr_void_Invoke();
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