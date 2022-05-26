// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include "EventTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <Foundation/LinkList.h>
#include <Foundation/Queue.hpp>
#include <Foundation/MemPool.h>
#include <Foundation/Logger.h>
#include <Foundation/SmartPtr.h>
#include <Foundation/CMyDeque.hpp>
#include <Foundation/MemPool_MY.hpp>
#include <Foundation/ThreadWrap.h>
#include <Foundation/enable_shared_from_this.hpp>
#include <Foundation/uuid/UUIDGenerator.h>
#include <Foundation/platform/CMyDirectory.h>
#include <Foundation/platform/Environment.h>
#include <Foundation/platform/CMyException.h>
#include <Foundation/platform/macro/Macro_for.hpp>
#include <Foundation/platform/CASLock.h>
#include <Foundation/platform/Lock_WIN32.h>
#include <Foundation/platform/CASAtomic.h>
#include <Foundation/platform/Shared_ptr.h>
#include <Foundation/platform/CMyFunction.hpp>
#include <Foundation/platform/CMyFunction_v20.hpp>
#include <Foundation/platform/FunctionImpl.h>
#include <Foundation/platform/list_av_v20.hpp>
#include <Foundation/export/SmartFunction.h>
#include <Foundation/export/DllExport.h>
#include <math.h>

#include "PriorityQueue.h"

using namespace std;
using namespace _uuid_;
using namespace _smart_ptr_;
using namespace _foundation_::_function_;


enum ENVENT1_MASK
{
	READ_MASK = (1 << 0)
};

#include <vector>

template <typename T>
struct	obj
{
	template <typename A>
	inline	static void Construction( void* buf )
	{
		std::vector<int>	ls();
	}
};

#define TIMER_DEF  50000

class CTest;
static	boost::shared_ptr<CTest>	g_P;

template <typename T>
static	void shared (const T& p)
{
	//g_P = p;
}

void Add23( void )
{
	printf("void Add23\r\n");
}

class	CTestBase
{
public:
	virtual	~CTestBase( void )
	{}

	virtual	void Add( void ) = 0;
	virtual	void More( int t1, int t2, int t3 ) = 0;
	virtual	void Test( int t1, int t2, std::string text ) = 0;
private:

};

class CTest1 : public CTestBase
{
public:
	CTest1( void )
	{

	}

	CTest1( const CTest1& t1 )
	{
		int i = 0; 
		i ++;
	}

	CTest1( const CTest1* t1 )
	{
		int i = 0; 
		i ++;
	}

	~CTest1( void )
	{
		int i = 0;
	}

	int ly(int y1,int y2,int y3)
	{
		return 100;
	}

	void Add( void )
	{
		printf("CTest1 Add\r\n");
	}

	void More( int t1, int t2, int t3 )
	{
		printf("CTest1 More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);
		return ;
	}

	void Test( int t1, int t2, std::string text )
	{
		return ;
	}
};

class CTest2 : public CTest1
{
public:
	CTest2( void )
	{

	}

	~CTest2( void )
	{

	}

	void Add( void )
	{
		printf("CTest2 Add\r\n");
	}

	void More( int t1, int t2, int t3 )
	{
		//printf("CTest2 More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);
		return ;
	}

	void MoreTest( int t1, int t2, int t3 )
	{
		return ;
	}

	void Test( int t1, int t2, std::string text )
	{
		return ;
	}
};

class CTest3 : public CTest1//, public TR1::enable_shared_from_this<CTest3>
{
public:
	CTest3( void )
	{

	}

	~CTest3( void )
	{
		printf("~CTest3\r\n");
	}

	void Add( void )
	{
		printf("CTest3 Add\r\n");
	}

	void More( int t1, int t2, int t3 )
	{
		printf("CTest3 More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);
		return ;
	}

	void Test( int t1, int t2, std::string text )
	{
		return ;
	}
};

class CTest4 : public CTest3
{
public:
	CTest4( void )
	{

	}

	~CTest4( void )
	{

	}
};

#pragma pack(push, 4)
struct DataTest
{
	UInt32	mf;
	UInt32	size;
	std::string s;
	long	y;
};
#pragma pack(pop)


class CTest : public CTestBase//, public TR1::enable_shared_from_this<CTest>
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
		printf("CTest Add:%d\r\n",g_P.use_count());
	}

	void Add123( void )
	{
		//g_P = shared_from_this();
		//shared(shared_from_this());
		//printf("CTest Add:%d\r\n",g_P.use_count());
	}

	void invoke( _myfunction::_anyfunc::AnyFunction<void (void)>* MyAny )
	{
		//*MyAny = _myfunction::_anyfunc::bind(&CTest::sss,shared_from_this());
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

	void TestMony_sptr( int t1, boost::shared_ptr<int> sptr )
	{
		return ;
	}

	void TestFunc(const char* p, const int& num, const std::string& str)
	{
	
	}

	int iu;
};

class BaseTest
{
public:
	BaseTest( void )
	{

	}

	~BaseTest( void )
	{

	}

	virtual void Add123( void )
	{

	}

	virtual  void More( int t1, int t2, int t3 )
	{

	}
};

class BaseTest2
{
public:
	BaseTest2( void )
	{

	}

	~BaseTest2( void )
	{

	}

	virtual void Add1( void )
	{

	}

	virtual void Add2( void )
	{

	}

	virtual void Add( void )
	{

	}

	virtual  void More( int t1, int t2, int t3 )
	{

	}

	int o;
	int o3;
	int o4;
};

class CTest12 : virtual public BaseTest, virtual public BaseTest2//CTest//TR1::enable_shared_from_this<CTest>
{
public:
	CTest12( void )
	{
		//printf("CTest constructor\r\n");
	}

	~CTest12( void )
	{
		//printf("CTest destructor\r\n");
	}

	void Add123( void )
	{
		//g_P = shared_from_this();
		//shared(shared_from_this());
		//printf("CTest Add:%d\r\n",g_P.use_count());
	}

	void Add( void )
	{
		//g_P = shared_from_this();
		//shared(shared_from_this());
		//printf("CTest Add:%d\r\n",g_P.use_count());
	}

	void More( int t1, int t2, int t3 )
	{

	}
};

class Testfunc
{
public:
	Testfunc( void )
	{

	}

	~Testfunc( void )
	{
		int n = 0;
		n ++;
		printf("~Testfunc\r\n");
	}

	Testfunc( const Testfunc& t1 )
	{
		int n = 0;
		n ++;
	}

	Testfunc& operator=( const Testfunc& t1 )
	{
		return *this;
	}

	void	te0( void )
	{
		printf("Testfunc te0:\r\n");
	}

	void	te1( int y)
	{
		printf("Testfunc te1:y=%d\r\n",y);
	}

	void	te2( int y, int x)
	{
		printf("Testfunc te2:y=%d,x:%d\r\n",y,x);
	}

	void	te3( int y, int x, char z )
	{
		printf("Testfunc te3:y=%d,x:%d,z:%c\r\n",y,x,z);
	}

	void	te4( int y, int x, char z, std::string m )
	{
		//printf("Testfunc te4:y=%d,x:%d,z:%c,m:%s\r\n",y,x,z,m.c_str());
	}

	void More( int t1, int t2, int t3 )
	{
		printf("Testfunc More,t1:%d,t2:%d,t3:%d\r\n",t1,t2,t3);
		return ;
	}

	void Test( int t1, int t2, std::string text )
	{
		return ;
	}

protected:
private:
};

void	te0( void )
{
	printf("te0:\r\n");
}

void	te1( int y)
{
	printf("te1:y=%d\r\n",y);
}

void	te2( int y, int x)
{
	printf("te2:y=%d,x:%d\r\n",y,x);
}

void	te3( int y, int x, char z )
{
	printf("te3:y=%d,x:%d,z:%c\r\n",y,x,z);
}

void	te4( int y, int x, char z, std::string m )
{
	printf("te4:y=%d,x:%d,z:%c,m:%s\r\n",y,x,z,m.c_str());
}

void	te11121(int x,std::string m)
{
	printf("te11121\r\n");
}

const Testfunc& TBind()
{
	return Testfunc();
}

template<typename T>
void TempTest(  const T* pthis )
{
	int n ; 
	n = 0;
}

template<typename C>
void TempTest( C& const pthis )
{
	int n ; 
	n = 0;
}

#define		DEBUG_TYPE0 0
#define		DEBUG_TYPE1 1

#define		PARAMTER_MACRO		&CTest::More,&Test,10,10,10//&Testfunc::te4,&TestFunc,10,20,'4',std::string("123456789")//
//#define		PARAMTER_MACRO		&CTest::More,&Test,10,10,10

#define		DEC_TYPE			int,int,int//int,int,std::string//
#define		PARAMTER_1			&CTest::More,&Test//&CTest::Test,&Test//
#define		VALUE_1				10,10,10//10,10,std::string("123456")//

#include <boost/function.hpp>
#include <boost/bind.hpp>



/*template<typename T,typename FUNC>
inline list_av_2<FUNC,const T*>	voidbind(FUNC Function, const T* pthis)
{
return list_av_2<FUNC,const T*>(Function,pthis);
}*/

template<typename T,typename FUNC>
inline list_av_2<FUNC,const T*>	voidbind(FUNC Function, const T* pthis)
{
	return list_av_2<FUNC,const T*>(Function,pthis);
}

template<typename T>
void scTest( T& K)
{

}

template<typename T>
void TestOne( _myfunction::CMyFunction<T> *ptr )
{
	int i = 0; 
	i ++;
	std::string str = typeid(T).name();
	int uu;
}


/*template<typename R,typename T>
class InvokeHandle_Obj
{
public:
InvokeHandle_Obj(const T& Fun) : _pFun(Fun){}
~InvokeHandle_Obj( void ) {}
inline R operator()(void* buf,const int& a1, const int& a2, const int& a3)
{  
return _pFun(buf,a1,a2,a3);
}
private:
const T&	_pFun;
};*/

template<typename R,typename T,typename A1, typename A2, typename A3>
struct InvokeHandle_Obj
{
	typedef	R (*InvokeHandle)(void*, const A1&, const A2&, const A3&);
	static const InvokeHandle& GetHandle( void )
	{
		return _myfunction::CMyInvokeHandle<T,R (A1,A2,A3)>::Invoke;
	}
};

/*typedef std::type_info	type_info;

struct ITypeof
{
virtual const  type_info & type( void )  const = 0;
};

template<typename ValueType>
struct Typeof : public ITypeof
{
Typeof( void )
{}

~Typeof( void )
{}

const  type_info & type( void )  const
{
return  typeid ( ValueType );
}
};

class Event_Handle
{
public:
#define ATTACH1_BODY_MACRO(n, p1)\
new(reinterpret_cast<void*>(_Type)) Typeof< _myfunction::CMyFunction< void (MACRO_ARGLIST_DEF(n)) > >();\
new(reinterpret_cast<void*>(_handle)) _myfunction::CMyFunction< void (MACRO_ARGLIST_DEF(n)) >(p1);

#define ATTACH2_BODY_MACRO(n, p1, p2)\
new(reinterpret_cast<void*>(_Type)) Typeof< _myfunction::CMyFunction< void (MACRO_ARGLIST_DEF(n)) > >();\
new(reinterpret_cast<void*>(_handle)) _myfunction::CMyFunction< void (MACRO_ARGLIST_DEF(n)) >(p1, p2);

#define EVENT_ATTACH_DECLAR(n)\
template<typename T PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
inline void Attach( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), T* pthis )\
{\
ATTACH2_BODY_MACRO(n, pfnFun, pthis)\
}\
\
template<MACRO_TARGLIST_DEF(n)>\
inline void Attach( void (*pfnFun)(MACRO_ARGLIST_DEF(n)) )\
{\
ATTACH1_BODY_MACRO(n, pfnFun)\
}\
template<MACRO_TARGLIST_DEF(n)>\
inline void Invoke( MACRO_TPARAMLIST_DEF(n) )\
{\
_myfunction::CMyFunction< void (MACRO_ARGLIST_DEF(n)) >* ptr = any_cast< _myfunction::CMyFunction< void (MACRO_ARGLIST_DEF(n)) > >();\
if( ptr != NULL )\
(*ptr)(MACRO_PARAMLIST_DEF(n));\
}

public:
Event_Handle( void )
{
memset(_Type, 0, sizeof(_Type));
memset(_handle, 0, sizeof(_handle));
}

Event_Handle( const Event_Handle& ev )
{
memcpy(_Type, ev._Type, sizeof(_Type));
memcpy(_handle, ev._handle, sizeof(_handle));
}

Event_Handle& operator=( const Event_Handle& ev )
{
memcpy(_Type, ev._Type, sizeof(_Type));
memcpy(_handle, ev._handle, sizeof(_handle));
return *this;
}

bool	Register( const Event_Handle& handle, ENVENT1_MASK mask )
{
return true;
}

~Event_Handle( void )
{

}

bool Detach( void )
{
memset(_Type, 0, sizeof(_Type));
memset(_handle, 0, sizeof(_handle));
}

template<typename T>
inline void Attach( void (T::*pfnFun)(), T* pthis )
{
ATTACH2_BODY_MACRO(0, pfnFun, pthis)
}

inline void Attach( void (*pfnFun)( void ) )
{
ATTACH1_BODY_MACRO(0, pfnFun)
}

inline void Invoke( void )
{
_myfunction::CMyFunction< void (void) >* ptr = any_cast< _myfunction::CMyFunction< void (void) > >();
if( ptr != NULL )
(*ptr)();
}

EVENT_ATTACH_DECLAR(1)
EVENT_ATTACH_DECLAR(2)
EVENT_ATTACH_DECLAR(3)
EVENT_ATTACH_DECLAR(4)
EVENT_ATTACH_DECLAR(5)
EVENT_ATTACH_DECLAR(6)
EVENT_ATTACH_DECLAR(7)

protected:
template<typename ValueType>
ValueType* any_cast( void )
{
try
{
if( &((reinterpret_cast<ITypeof*>(_Type))->type()) == &(typeid(ValueType)))
return (reinterpret_cast<ValueType*>(_handle));
else
throw("error");
}
catch(...)
{//error
printf("error\r\n");
//error_code = -1;
}

return NULL;
}

private:
char	_Type[10];
char	_handle[50];
};*/

#define	DECL_int(name)\
	int i##name##1;\
	int i##name##2;\
	int i##name##3;\
	int i##name##4;\
	int i##name##5;

#define	DECL_char(name)\
	char c##name##1;\
	char c##name##2;\
	char c##name##3;\
	char c##name##4;\
	char c##name##5;

#define	DECL_string(name)\
	char str##name##1[100];\
	char str##name##2[100];\
	char str##name##3[100];\
	char str##name##4[100];\
	char str##name##5[100];

struct STTestF
{
	DECL_int(iNa)
	DECL_int(iNe)
	DECL_char(cNa)
	DECL_char(cNe)
	DECL_string(strNa)
	DECL_string(strNe)
};

#pragma pack(push, 4)
struct DataB
{
	UInt32	mf:1;
	UInt32	size:31;
};
#pragma pack(pop)

void ViretuTest( IntPtr* function_to_bind )
{
	_functionaddress_::MicrosoftVirtualMFP	mfp;
	memcpy(&mfp, function_to_bind, sizeof(_functionaddress_::MicrosoftVirtualMFP));
	int i = 0;
}

struct SimplifyMemFuncTest
{  
	template <typename X, typename XFuncType, typename GenericMemFuncType>  
	inline static _functionaddress_::GenericClass *Convert(X *pthis, XFuncType& function_to_bind,   
		GenericMemFuncType& bound_func) 
	{  
		union 
		{  
			XFuncType func;  
			_functionaddress_::GenericClass* (X::*ProbeFunc)();  
			_functionaddress_::MicrosoftVirtualMFP s;  
		} u;  
		
		u.func = function_to_bind;
		//memcpy(u.func, (char*)function_to_bind, sizeof(u.func) );
		bound_func = u.s.codeptr;
		union 
		{  
			_functionaddress_::GenericVirtualClass::ProbePtrType virtfunc;
			_functionaddress_::MicrosoftVirtualMFP s;
		} u2;

		u2.virtfunc = &_functionaddress_::GenericVirtualClass::GetThis;
		u.s.codeptr = u2.s.codeptr;
		return (pthis->*u.ProbeFunc)();
	}  
};

struct SimplifyMemFuncTest88
{  
	template <typename X>  
	inline static _functionaddress_::GenericClass *Convert(X *pthis, IntPtr* function_to_bind,   
		IntPtr* bound_func) 
	{  
		union 
		{  
			char func[16]; 
			_functionaddress_::GenericClass* (X::*ProbeFunc)();
			_functionaddress_::MicrosoftVirtualMFP s;
		} u; 

		memcpy(u.func, (char*)function_to_bind, sizeof(u.func) );
		*bound_func = *(IntPtr*)&u.s.codeptr;
		union 
		{  
			_functionaddress_::GenericVirtualClass::ProbePtrType virtfunc;
			_functionaddress_::MicrosoftVirtualMFP s;
		} u2;


		/*int size = sizeof(u.ProbeFunc);

		typedef	_functionaddress_::GenericClass* (_functionaddress_::GenericVirtualClass::*ProbeFunc11)();  
		int size1 = sizeof(ProbeFunc11);*/
		if( u.s.vtable_index <= 0 )
		{
			return reinterpret_cast<_functionaddress_::GenericClass *>(pthis + u.s.delta); 
		}
		else
		{
			struct VBTable
			{
				int offset;
				int vbptr_delta;
			}; 

			 //= reinterpret_cast<>((reinterpret_cast<char*>(pthis) + u.s.delta));

			VBTable* poe21 = (VBTable*)*(reinterpret_cast<IntPtr*>(pthis) + u.s.vtable_index) + u.s.delta;

			char* vbptr = reinterpret_cast<char*>(reinterpret_cast<IntPtr*>(pthis) + u.s.vtable_index);
			//_functionaddress_::GenericClass * ptr = reinterpret_cast<_functionaddress_::GenericClass *>(vbptr + (*reinterpret_cast<VBTable**>(vbptr))->vbptr_delta);
			//return reinterpret_cast<_functionaddress_::GenericClass *>(vbptr + (*reinterpret_cast<VBTable**>(vbptr))->vbptr_delta);

			//VBTable* poe21 = (VBTable*)*(reinterpret_cast<IntPtr*>(pthis) + u.s.vtable_index) + u.s.delta;
			//return reinterpret_cast<_functionaddress_::GenericClass *>(reinterpret_cast<char*>(pthis) + 48);


			//VCMFP* poi = reinterpret_cast<VCMFP *>( *(pthis + u.s.delta + u.s.vtable_index - 1)  );
			int i = 0;
			u2.virtfunc = &_functionaddress_::GenericVirtualClass::GetThis;
			u.s.codeptr = u2.s.codeptr;
			return (pthis->*u.ProbeFunc)();
		}

		return NULL;
	}  
};

void MyFunctionTest();

void SmartFunctionTest( void )
{
	CTest	Test;
	CTest1	Test1;
	CTest2	Test2;
	CTest3	Test3;

	int num = 100000000;
	int time = 0;
	time = GetTickCount();
	for(int i = 0; i < num; i ++)
	{
		_myfunction::CMyFunction<void (DEC_TYPE)>	ms(PARAMTER_1);
		//ms(10,20,30);
	}
	time = GetTickCount() - time;
	printf("_myfunction::CMyFunction:%d,size:%d\r\n",time,sizeof(boost::function<void (int,int,int)>));

	time = GetTickCount();
	for(int i = 0; i < num; i ++)
	{
		/*IFunction<void (DEC_TYPE)>* ptr = reinterpret_cast< IFunction<void (DEC_TYPE)>* >(Dll_Function_Get3());
		ptr->Bind(PARAMTER_1);
		TSmartPtr< IFunction<void (DEC_TYPE)> >	_TSmartPtr(ptr, &Dll_Function_Free3);
		_TSmartPtr.get()->Invoke(VALUE_1);*/

		FunctionImpl<void (DEC_TYPE)> funimpl(PARAMTER_1);
		//FunctionImpl<void (DEC_TYPE)> funimpl1 = funimpl;
		//funimpl(VALUE_1);
	}
	time = GetTickCount() - time;
	printf("TSmartPtr.IFunction:%d,size:%d\r\n",time,sizeof(boost::function<void (int,int,int)>));

	while(true)
	{

	SmartFunction<void (void)> MyDelg1(&CTest1::Add,&Test1);
	MyDelg1();
	SmartFunction<void (void)> MyDelg2(&CTest2::Add,&Test2);
	MyDelg2();
	SmartFunction<void (void)> MyDelg3(&CTest3::Add,&Test3);
	MyDelg3();

	SmartFunction<void (void)> MyDelg4(Add23);
	MyDelg4();

	SmartFunction<void (int,int,int)> MyDelgM1(&CTest1::More,&Test1);
	MyDelgM1(10,10,10);

	SmartFunction<void (int,int,int)> MyDelgM2(&CTest2::More,&Test2);
	MyDelgM2(10,10,10);

	SmartFunction<void (int,int,int)> MyDelgM3(&CTest3::More,&Test3);
	MyDelgM3(10,10,10);

	boost::shared_ptr<CTest3>	pp(new CTest3);

	CTestBase* slye = &Test;
	CTestBase* slye11 = reinterpret_cast<CTestBase*>(const_cast<CTest*>(&Test));
	CTest	test;
	time = GetTickCount();
	for(int i = 0; i < num; i ++)
	{
		test.More(10,10,10); // 调用 foo.methodA();
	}
	time = GetTickCount() - time;
	printf("Test.More:%d,size:%d\r\n",time,sizeof(boost::function<void (int,int,int)>));

	//直接引用
	{
#if DEBUG_TYPE0 == 0
		SmartFunction<void (DEC_TYPE)> MyDelgM(PARAMTER_1);
#endif
		time = GetTickCount();
		for(int i = 0; i < num; i ++)
		{
#if DEBUG_TYPE0 == 1
			SmartFunction<void (DEC_TYPE)> MyDelgM(PARAMTER_1);
#endif
#if DEBUG_TYPE1 == 1
			MyDelgM(VALUE_1);
#endif
		}
		time = GetTickCount() - time;
		printf("SmartFunction  const& :%d,size:%d\r\n",time,sizeof(SmartFunction<int (int,int,int)>));
	}

	//直接引用
	{
		SmartFunction<void (DEC_TYPE)>	ms(PARAMTER_1);
#if DEBUG_TYPE0 == 0
		SmartFunction<void (DEC_TYPE)> MyDelgM = ms;
#endif
		time = GetTickCount();
		for(int i = 0; i < num; i ++)
		{
#if DEBUG_TYPE0 == 1
			SmartFunction<void (DEC_TYPE)> MyDelgM = ms;
#endif
#if DEBUG_TYPE1 == 1
			MyDelgM(VALUE_1);
#endif
		}
		time = GetTickCount() - time;
		printf("SmartFunction  operator= :%d,size:%d\r\n",time,sizeof(SmartFunction<int (int,int,int)>));
	}
	};
}

void	MyFunctionTest( void )
{
	int s = sizeof(void (*)());
	int is = sizeof(&_functionaddress_::GenericVirtualClass::GetThis);
	int lse = sizeof(&CTest12::Add123);
	int ls8e3 = sizeof(&CTest3::Add);

	CTest12	Test12;
	//Test12.Add123();
	{
		typedef void (_functionaddress_::GenericClass::*TestMore)( int t1, int t2, int t3 );
		typedef void (CTest12::*CMore)( int t1, int t2, int t3 );
		TestMore piw;
		CMore More12 =  &CTest12::More;

		IFunction<void (DEC_TYPE)>* pfunc123 = reinterpret_cast<IFunction<void (DEC_TYPE)>*>(Dll_Function_Get3());
		pfunc123->Bind(&CTest12::More, &Test12);
		pfunc123->Invoke(10,20,30);

		CTest Test;
		
		IFunction<void (const char*, const int&, const std::string&)>* pfunc1234 = reinterpret_cast<IFunction<void (const char*, const int&, const std::string&)>*>(Dll_Function_Get3());
		pfunc1234->Bind(&CTest::TestFunc, &Test);
		pfunc1234->Invoke("2q34",20,"3432423");

		{
			SmartFunction<void (DEC_TYPE)> pfunc(&CTest12::More, &Test12);
			pfunc(10,20,30);
		}

	}

	/*for(int i = 0; i < 15; i ++)
	{
	char uyiu[100] = {0};
	void* vftable11 = (void*)(*((IntPtr*)classtable + i));
	memcpy(uyiu, vftable11, sizeof(uyiu));
	int n =0;
	}*/

	_functionaddress_::GenericVirtualClass::ProbePtrType pte = &_functionaddress_::GenericVirtualClass::GetThis;

	typedef void (CTest12::*fnAdd123)( void );
	fnAdd123 Add123 = &CTest12::Add123;
	ViretuTest((IntPtr*)&pte);
	ViretuTest((IntPtr*)&Add123);
	{
		typedef void (_functionaddress_::GenericClass::*fnAdd)( void );
		fnAdd	pAdd;

		CTest12 ls;
		//_functionaddress_::GenericClass* ptr = SimplifyMemFuncTest::Convert(&ls,&CTest12::Add123,pAdd);
		_functionaddress_::GenericClass* ptr = SimplifyMemFuncTest88::Convert(&ls,(IntPtr*)&Add123,(IntPtr*)&pAdd);
		(ptr->*pAdd)();
		/*_myfunction::CMyFunction<void (void)> MyDelgM(&CTest12::Add123,&ls);
		MyDelgM();*/
	}

	char* p = NULL;
	char** p0 = (&p);
	int len = sizeof(DataB);
	//stDataBuf* pu = reinterpret_cast<stDataBuf*>(malloc(1470 + sizeof(UInt32)));
	//const char* p879 = pu->pData;
	int num98 = 100000000;

	CTest2 Test28;
	int time98 = 0;

	long m_lock = 0; 
	int y = InterlockedCompareExchange( &m_lock, 1 , 0);
	int x = InterlockedCompareExchange( &m_lock, 0 , 1);

	/*Event_Handle event;
	event.Attach(&CTest2::MoreTest,&Test28);
	time98 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	event.Invoke(100,20,30);
	//Test28.MoreTest(100,20,30);
	printf("time:%d",GetTickCount() - time98);*/

	/*while(true)
	{
	while(true)
	{
	int time9890 = 0;

	TestEvent1	ev1;
	TestEvent2	ev22;
	TestEvent3	ev3;

	EVENT_EXEC_ATTACH(&ev22, &ev3)
	ev3.ExecEvent();

	time9890 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	{
	ev3.RegisterEvent(&ev22);
	ev3.ExecBind(&ev1);
	}
	printf("time1:%d\r\n",GetTickCount() - time9890);


	//EVENT_REGISTER(&ev1,&ev2);
	ev1.TestFunc0();
	ev1.TestFunc1(10);
	ev1.TestFunc2(10,20);
	time9890 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	ev1.TestFunc3(10,20,30);
	printf("time1:%d\r\n",GetTickCount() - time9890);
	Sleep(100);
	}

	while(true) Sleep(1000);

	TestEvent2	ev2;
	_event_::Event_Handle event;
	event.Attach(&TestEvent2::TestFunc3,&ev2);

	time98 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	event.Invoke< FUNC_HANDLE< void (int,int,int) > >(10,20,50);
	printf("time1:%d\r\n",GetTickCount() - time98);

	_myfunction::CMyFunction<void (int,int,int)> MyDelg121(&CTest2::MoreTest,&Test28);
	time98 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	{
	//printf("111111");
	MyDelg121(100,20,30);
	//printf("22222");
	}
	//Event.Notify(100,20,30);
	printf("time2:%d\r\n",GetTickCount() - time98);

	boost::function<void (int,int,int)> f1 = boost::bind(&CTest2::MoreTest,&Test28,_1,_2,_3);
	time98 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	f1(100,20,30);
	printf("time3:%d\r\n",GetTickCount() - time98);

	CTest1* p21 = &Test28;
	time98 = GetTickCount();
	for(int i = 0; i < num98; i ++)
	p21->More(100,20,30);
	printf("time4:%d\r\n",GetTickCount() - time98);

	Sleep(2000);
	}*/

	scTest("1111111");

	while(true)
	{

		//int len = sizeof(InvokeHandle_Obj<void,CTest>);

		CTest	Test;
		CTest1	Test1;
		CTest2	Test2;
		CTest3	Test3;

		{
			//while(true)
			{
				IFunction<void (DEC_TYPE)>* pfunc123 = reinterpret_cast<IFunction<void (DEC_TYPE)>*>(Dll_Function_Get3());
				pfunc123->Bind(PARAMTER_1);
				pfunc123->Invoke(10,20,30);

				/*IFunction<void (void)>* pfunc15 = reinterpret_cast<IFunction<void (void)>*>(Dll_Function_Get33());
				pfunc15->Bind(&CTest::Add,&Test);
				pfunc15->Invoke();*/
				DataTest	DTest;
				DTest.mf = 10;
				DTest.size = 20;
				DTest.s = "FunctionTest";
				DTest.y = 100;
				IFunction<DataTest* (char*, DataTest*, int)>* pfunc456 = reinterpret_cast<IFunction<DataTest* (char*, DataTest*, int)>*>(Dll_Function_Get3());
				//pfunc456->Clone(pfunc123);
				pfunc456->Bind(&CTest::More123,&Test);
				DataTest*	pDTest = pfunc456->Invoke("1232",&DTest,30);

				Dll_Function_Free3(pfunc123);
				Dll_Function_Free3(pfunc456);

			};

			typedef void (_functionaddress_::GenericClass::*TestMore)( int t1, int t2, int t3 );
			typedef void (CTest::*CMore)( int t1, int t2, int t3 );
			TestMore piw;
			/*CMore More12 =  &CTest::More;
			_functionaddress_::GenericClass* ptre2 = _functionaddress_::GeneraldutyFunc<sizeof(CMore)>::Convert((char*)&Test, (IntPtr*)&More12 , (IntPtr*)&piw, ConvertImpl<CTest>::Convert());
			(ptre2->*piw)(10,20,30);*/

			int ns = sizeof(_myfunction::CMyFunction<void (void)>);
			int l2 = sizeof(_myfunction::CMyFunction<void (long,char,std::string)>);
			int i = 0;
		}

		{
			_myfunction::CMyFunction<void (DEC_TYPE)>	ms(PARAMTER_1);
			_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = ms;
			MyDelgM(VALUE_1);

			/*_myfunction::CMyFunction<void (int,int,std::string)>	any1 = _myfunction::bind(&CTest::Test,&Test);
			any1(10,10,std::string("11111"));
			any1 = _myfunction::bind(&CTest::Test,&Test);
			any1(10,10,std::string("11111"));*/
		}

		/*{
		TR1::shared_ptr<CTest>	ppy(new CTest);
		_myfunction::CMyFunction<void (DEC_TYPE)>	ms(PARAMTER_1);
		_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM;
		_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM_s;
		_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM_y;
		while(true)
		{
		MyDelgM = ms;
		_myfunction::CMyFunction<void (DEC_TYPE)> MyD = _myfunction::bind(&CTest::More,ppy);
		MyDelgM_s = MyD;
		_myfunction::CMyFunction<void (DEC_TYPE)> MyD1 = _myfunction::bind(PARAMTER_1);
		MyDelgM_y = _myfunction::bind(&CTest::More,ppy);
		Sleep(10);
		}
		}*/

		/*list_av_2<void (CTest::*)(int,int,std::string),const CTest*>ls = voidbind(&CTest::Test,&Test);
		_myfunction::CMyFunction<void (int,int,std::string)> My(ls._a1,ls._a2);
		My(10,10,std::string("1112221121"));*/

		_myfunction::CMyFunction<void (int,int,std::string)> My(&CTest::Test,&Test);
		_myfunction::CMyFunction<void (int,int,std::string)> MyDel = My;
		/*{
		_myfunction::CMyFunction<void (int,int,std::string)> MyDelgM = _myfunction::listbind(&CTest::Test,&Test);
		}*/

		int num = 100000000;
		int time = 0;

		/*TempTest(&Test);
		TempTest(Test);*/

		//= bind
		/*{
		_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = _myfunction::bind(PARAMTER_1);
		_myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>	InvokeHandle(PARAMTER_1);

		typedef	void (*_pInvokeFun)(void*,const int&,const int&,const int&);
		_pInvokeFun pfun = InvokeHandle_Obj<void,CTest,DEC_TYPE>::GetHandle();//_myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>::Invoke;//
		char	szbuf[40] = {0};
		new (reinterpret_cast<void*>(szbuf)) _myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>(PARAMTER_1) ;

		void* point = &InvokeHandle;
		list_av_3<DEC_TYPE> ls(VALUE_1);

		//InvokeHandle_Obj<void,_pInvokeFun>	obj(_myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>::Invoke);

		#if DEBUG_TYPE0 == 0
		voidbind(&CTest::Test,&Test);
		#endif
		TR1::shared_ptr<CTest>	ppy(new CTest);
		time = GetTickCount();
		for(int i = 0; i < num; i ++)
		{
		#if DEBUG_TYPE0 == 1
		//voidbind(&CTest::Test,&Test);
		//list_av_2<void (CTest::*)(int,int,std::string),const CTest*>ls = _myfunction::listbind(PARAMTER_1);
		_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = _myfunction::bind(PARAMTER_1);
		#endif
		#if DEBUG_TYPE1 == 1
		MyDelgM(VALUE_1);
		//InvokeHandle(10,10,10);
		//_myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>::Invoke(szbuf,VALUE_1);
		//pfun(szbuf,VALUE_1);
		//obj(szbuf,VALUE_1);
		//(*reinterpret_cast<_myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>*>(point))(VALUE_1);
		//_myfunction::CMyInvokeHandle<CTest,void (DEC_TYPE)>::Invoke_av(point,ls);
		#endif
		}
		time = GetTickCount() - time;
		printf("_myfunction::CMyFunction  voidbind= :%d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}*/

		//= bind
		{
#if DEBUG_TYPE0 == 0
			_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = _myfunction::bind(PARAMTER_1);
#endif
			boost::shared_ptr<CTest>	ppy(new CTest);
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = _myfunction::bind(PARAMTER_1);
#endif
#if DEBUG_TYPE1 == 1
				//MyDelgM.Invoke(VALUE_1);
#endif
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction  bind= :%d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}

		//_myfunction::CMyFunction<void (void)> MyDelg(&CTest::Add,&Test);
		//MyDelg();

		{
			_myfunction::CMyFunction<void (int,int,int)> MyDelgM ;
			{
				boost::shared_ptr<CTest>	ppy(new CTest);
				//while(true)
				{
					MyDelgM = _myfunction::bind(&CTest::More,ppy);
					MyDelgM(10,10,10);
				}
			}
			int i = 0;
			i ++;
		}

		_myfunction::CMyFunction<void (void)> MyDelg1(&CTest1::Add,&Test1);
		MyDelg1();

		_myfunction::CMyFunction<void (void)> MyDelg2(&CTest2::Add,&Test2);
		MyDelg2();

		_myfunction::CMyFunction<void (void)> MyDelg3(&CTest3::Add,&Test3);
		MyDelg3();

		_myfunction::CMyFunction<void (void)> MyDelg4(Add23);
		MyDelg4();

		_myfunction::CMyFunction<void (int,int,int)> MyDelg5 = _myfunction::bind(&CTest1::More,&Test1);
		MyDelg5(10,10,10);

		_myfunction::CMyFunction<void (int,int,int)> MyDelgM1(&CTest1::More,&Test1);
		MyDelgM1(10,10,10);

		_myfunction::CMyFunction<void (int,int,int)> MyDelgM2(&CTest2::More,&Test2);
		MyDelgM2(10,10,10);

		_myfunction::CMyFunction<void (int,int,int)> MyDelgM3(&CTest3::More,&Test3);
		MyDelgM3(10,10,10);

		boost::shared_ptr<CTest3>	pp(new CTest3);

		CTestBase* slye = &Test;
		CTestBase* slye11 = reinterpret_cast<CTestBase*>(const_cast<CTest*>(&Test));
		CTest	test;
		time = GetTickCount();
		for(int i = 0; i < num; i ++)
		{
			test.More(10,10,10); // 调用 foo.methodA();
		}
		time = GetTickCount() - time;
		printf("Test.More:%d,size:%d\r\n",time,sizeof(boost::function<void (int,int,int)>));

		//test
		{
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
				_myfunction::bind(&CTest::More,&Test);
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction  const& :%d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}
		//test

		//直接引用
		{
#if DEBUG_TYPE0 == 0
			_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM(PARAMTER_1);
#endif
			boost::shared_ptr<CTest>	ppy(new CTest);
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM(PARAMTER_1);
#endif
#if DEBUG_TYPE1 == 1
				MyDelgM(VALUE_1);
#endif
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction  const& :%d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}

		//直接引用
		{
			_myfunction::CMyFunction<void (DEC_TYPE)>	ms(PARAMTER_1);
#if DEBUG_TYPE0 == 0
			_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = ms;
#endif
			boost::shared_ptr<CTest>	ppy(new CTest);
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = ms;
#endif
#if DEBUG_TYPE1 == 1
				MyDelgM(VALUE_1);
#endif
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction  operator= :%d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}

		//= bind
		{
#if DEBUG_TYPE0 == 0
			_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = _myfunction::bind(PARAMTER_1);
#endif
			boost::shared_ptr<CTest>	ppy(new CTest);
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM = _myfunction::bind(PARAMTER_1);
#endif
#if DEBUG_TYPE1 == 1
				MyDelgM(VALUE_1);
#endif
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction  bind= :%d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}

		//(bind)
		{
#if DEBUG_TYPE0 == 0
			_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM(_myfunction::bind(PARAMTER_1));
#endif
			boost::shared_ptr<CTest>	ppy(new CTest);
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				_myfunction::CMyFunction<void (DEC_TYPE)> MyDelgM(_myfunction::bind(PARAMTER_1));
#endif
#if DEBUG_TYPE1 == 1
				MyDelgM(VALUE_1);
#endif
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction: bind() %d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}

		//shared_ptr
		{
			//_myfunction::CMyFunction<void (int,int,int)> MyDelgM = _myfunction::bind(PARAMTER_1);
			//_myfunction::_anyfunc::AnyFunction<void (int,int,int)> MyDelgM = _myfunction::_anyfunc::bind(&CTest::More,&Test,10,10,10);
			boost::shared_ptr<CTest>	ppy(new CTest);
#if DEBUG_TYPE0 == 0
			_myfunction::CMyFunction<void (int,int,int)> MyDelgM(_myfunction::bind(&CTest::More,ppy));
#endif
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
				//_myfunction::CMyFunction<void (int,int,int)> MyDelgM = _myfunction::bind(&CTest::More,&Test);
				//_myfunction::CMyFunction<void (int,int,int)> MyDelgM = _myfunction::bind(&CTest::More,ppy);
#if DEBUG_TYPE0 == 1
				_myfunction::CMyFunction<void (int,int,int)> MyDelgM(_myfunction::bind(&CTest::More,ppy));
#endif
#if DEBUG_TYPE1 == 1
				MyDelgM(10,10,10);
#endif
				//MyDelgM();
			}
			time = GetTickCount() - time;
			printf("_myfunction::CMyFunction: shared_ptr %d,size:%d\r\n",time,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
		}

		//boost =
		{
			boost::shared_ptr<CTest>	ppyb(new CTest);
			boost::function<void (DEC_TYPE)> f11 = boost::bind(PARAMTER_1,_1,_2,_3); // 无参数，无返回值
#if DEBUG_TYPE0 == 0
			boost::function<void (DEC_TYPE)> f1 = f11; // 无参数，无返回值
#endif
			//boost::function<void ()> f1 = boost::bind(&CTest::Test, &Test,10,10,10); // 无参数，无返回值
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				boost::function<void (DEC_TYPE)> f1 = f11; // 无参数，无返回值
#endif
#if DEBUG_TYPE1 == 1
				f1(VALUE_1); // 调用 foo.methodA();
#endif
				//boost::function<void (int,int,int)> f1 = boost::bind(&CTest::Test, &Test,_1,_2,_3); // 无参数，无返回值
				//f1();
			}
			time = GetTickCount() - time;
			printf("boost::bind operator= :%d,size:%d\r\n",time,sizeof(boost::function<int (int,int,int)>));
		}

		//boost bind=
		{
			boost::shared_ptr<CTest>	ppyb(new CTest);
#if DEBUG_TYPE0 == 0
			boost::function<void (DEC_TYPE)> f1 = boost::bind(PARAMTER_1,_1,_2,_3); // 无参数，无返回值
#endif
			//boost::function<void ()> f1 = boost::bind(&CTest::More, &Test,10,10,10); // 无参数，无返回值
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				boost::function<void (DEC_TYPE)> f1 = boost::bind(PARAMTER_1,_1,_2,_3); // 无参数，无返回值
#endif
#if DEBUG_TYPE1 == 1
				f1(VALUE_1); // 调用 foo.methodA();
#endif
				//boost::function<void (int,int,int)> f1 = boost::bind(&CTest::Test, &Test,_1,_2,_3); // 无参数，无返回值
				//f1();
			}
			time = GetTickCount() - time;
			printf("boost::bind= %d,size:%d\r\n",time,sizeof(boost::function<int (int,int,int)>));
		}

		//boost (bind)
		{
			boost::shared_ptr<CTest>	ppyb(new CTest);
#if DEBUG_TYPE0 == 0
			boost::function<void (DEC_TYPE)> f1(boost::bind(PARAMTER_1,_1,_2,_3)); // 无参数，无返回值
#endif
			//boost::function<void ()> f1 = boost::bind(PARAMTER_1,10,10,10); // 无参数，无返回值
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				boost::function<void (DEC_TYPE)> f1(boost::bind(PARAMTER_1,_1,_2,_3)); // 无参数，无返回值
#endif
#if DEBUG_TYPE1 == 1
				f1(VALUE_1); // 调用 foo.methodA();
#endif
				//boost::function<void (int,int,int)> f1 = boost::bind(&CTest::More, &Test,_1,_2,_3); // 无参数，无返回值
				//f1();
			}
			time = GetTickCount() - time;
			printf("boost::bind() %d,size:%d\r\n",time,sizeof(boost::function<int (int,int,int)>));
		}

		//boost shared_ptr
		{
			boost::shared_ptr<CTest>	ppyb(new CTest);
#if DEBUG_TYPE0 == 0
			boost::function<void (int,int,int)> f1 = boost::bind(&CTest::More, ppyb,_1,_2,_3); // 无参数，无返回值
#endif
			//boost::function<void ()> f1 = boost::bind(&CTest::Test, &Test,10,10,10); // 无参数，无返回值
			time = GetTickCount();
			for(int i = 0; i < num; i ++)
			{
#if DEBUG_TYPE0 == 1
				boost::function<void (int,int,int)> f1 = boost::bind(&CTest::More, ppyb,_1,_2,_3); // 无参数，无返回值
#endif
#if DEBUG_TYPE1 == 1
				f1(10,10,10); // 调用 foo.methodA();
#endif
				//boost::function<void (int,int,int)> f1 = boost::bind(&CTest::Test, &Test,_1,_2,_3); // 无参数，无返回值
				//f1();
			}
			time = GetTickCount() - time;
			printf("boost::bind: shared_ptr%d,size:%d\r\n",time,sizeof(boost::function<int (int,int,int)>));
		}

		/*
		CMyDelegate<void,void> MyDelg(&Test,&CTest::Add);
		MyDelg();

		CMyDelegate<void,void> MyDelg1(&Test1,&CTest1::Add);
		MyDelg1();

		CMyDelegate<void,void> MyDelg2(&Test2,&CTest2::Add);
		MyDelg2();

		CMyDelegate<void,void> MyDelg3(&Test3,&CTest3::Add);
		MyDelg3();*/

		Sleep(100);
	}
}


char hextoi( char hexch )
{
	if((hexch>='0')&&(hexch<='9'))
	{
		return hexch - '0';
	}
	else if((hexch>='A')&&(hexch<='F'))
	{
		return hexch - 'A' + 10;
	}
	else if((hexch>='a')&&(hexch<='f'))
	{
		return hexch - 'a' + 10;
	}

	return -1;
}

char x2c( const char * hex )
{
	char digit;
	digit = hextoi( hex[0] );
	digit = digit << 4;
	digit = digit + hextoi( hex[1] );
	return digit;
}

void DecodeHtml( const char * body, int body_len, char * gb_body, int gb_body_len )
{
	char sc[3] = "";
	for( int i = 0, j = 0; i < body_len && j < gb_body_len; i++, j++ )
	{
		if( i + 2 < body_len && body[i] == '%' )
		{
			sc[0] = body[++i];
			sc[1] = body[++i];
			sc[2] = '\0';
			gb_body[j] = x2c( sc );
		}
		else
		{
			gb_body[j] = body[i];
		}
	}
}

string GBKToUTF8(const char* strGBK)  
{  
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);  
	wchar_t* wstr = new wchar_t[len+1];  
	memset(wstr, 0, len+1);  
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);  
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);  
	char* str = new char[len+1];  
	memset(str, 0, len+1);  
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);  
	string strTemp = str;  
	if(wstr) delete[] wstr;  
	if(str) delete[] str;  
	return strTemp;  
}

string UTF8ToGBK(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len+1];
	memset(wszGBK, 0, len*2+2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len+1];
	memset(szGBK, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if(wszGBK) delete[] wszGBK;
	if(szGBK) delete[] szGBK;
	return strTemp;
}

void DecodeHtmlUTF8ToGB2312( string & body )
{
	if( body.find("%" , 0) == string::npos )
	{
		return;
	}

	char gb_body[1024] = "";
	DecodeHtml( body.c_str( ), body.length( ), gb_body, sizeof( gb_body ) - 1 );

	char buf[1024] = "";
	body = UTF8ToGBK(gb_body);

	/*if( '\0' == buf[ 0 ] )
	{
	body = gb_body;
	}
	else
	{
	body = buf;
	}*/
}

/**
* GBK->UNICODE
*/
void OnConvertText( std::string& data ) 
{ 
	//1.转为UNICODE
	int wc = MultiByteToWideChar(CP_ACP,0,data.c_str(),data.size(),0,0);

	wchar_t* wUnicode = new wchar_t[wc+1];
	wUnicode[wc] = 0;

	MultiByteToWideChar(CP_ACP,0,data.c_str(),data.size(),wUnicode,wc);

	//2.UNICODE->ANSI
	wc = WideCharToMultiByte(CP_UTF8,0,wUnicode,wcslen(wUnicode),0,0,NULL,NULL);
	char* sUnicode = new char[wc+1];
	sUnicode[wc] = 0;

	WideCharToMultiByte(CP_UTF8,0,wUnicode,wcslen(wUnicode),sUnicode,wc,NULL,NULL);

	if(sUnicode)
		delete[] sUnicode;
}

/**
* UNICODE->GBK
*/
void OnReverseText( std::string& data ) 
{
	int wc = MultiByteToWideChar(CP_UTF8,0,data.c_str(),data.size(),0,0);

	wchar_t* sUnicode = new wchar_t[wc+1];
	sUnicode[wc] = 0;

	MultiByteToWideChar(CP_UTF8,0,data.c_str(),data.size(),sUnicode,wc);
	if(sUnicode)
		delete[] sUnicode;
}

/*const std::string& str_Replace( string&	str, const string& old_value,
const string& new_value )
{
while(true) 
{   
string::size_type   pos(0);   
if( (pos=str.find(old_value))!=string::npos )   
{
str.replace(pos,old_value.length(),new_value); 
}
else   
{
break;
}
}   
return   str;  
}*/


void NetControlTest( void )
{
	/*char soi[20] = "\r\nweibojl3348";
	char sz33[] = "苹果";//{0x90,0xff,0x00};
	std::string	ssz("\r\ntest\r\n\r\n",10);
	ssz.append("2014689\0weiboji876",strlen("20146890weiboji876"));
	ssz.append(sz33);
	ssz.append("testsetest");
	std::string _title21 = FindKey(ssz,"weibo",sz33);

	int size = ssz.size();
	size_t npos = ssz.find("\r\n");
	if( npos != std::string::npos)
	{
		//str_Replace(ssz,"\0","&");
		std::string ny = ssz.substr(npos,100);
		int n = ny.size();
		n ++;
	}

	char buf1[] = {"params=%7B%22photos%22%3A%5B%5D%2C%22content%22%3A%22%3Cp+dir%3Dltr%3E%26%2320160%3B%26%2320040%3B%26%2321457%3B%26%2320102%3B%26%2321834%3B%26%2365311%3B%3C%5C%2Fp%3E%5Cn%22%2C%22title%22%3A%22%E5%8F%91%E4%BA%86%E5%95%8A%22%2C%22forumid%22%3A%22zz2323%22%2C%22gid%22%3A%2202ffff109110113876719d491aafe39dbd47bfe386d8ae%22%2C%22origin%22%3A%22android%22%2C%22app_token%22%3A%22dhnEfN2HtVQpP8NYDxp%5C%2F4AKXT1eFSY1bZuKggtn7%5C%2Fjk%3D%5Cn%22%2C%22passport%22%3A%22zkl_ok%40163.com%22%2C%22ct%22%3A1418867584505%7D&"};
	std::string cookie(buf1);
	std::string username = FindKey(cookie,"%22content%22%3A%22%3Cp+dir%3Dltr%3E","%3C%5C%2Fp");

	char sz[] = "%B3%7B%9C%AF%B3";//"%E6%A2%A6%E8%AF%BA%E6%98%9F%E7%95%94";//"%E4%B8%80%E4%B8%AA%E8%8B%B9%E6%9E%9C%E7%94%B7%E5%AD%A9";//"mp=%E6%B5%8B%E8%AF%95test2014; pin=%E6%B5%8B%E8%AF%95test2014;";
	std::string  styy = sz;
	DecodeHtmlUTF8ToGB2312(styy);

	char utf[100] = {0};

	FILE* fp = fopen("D:\\1.txt","rb");
	if(fp == NULL)
		return ;

	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	fseek(fp,0,SEEK_SET);
	char* buf = (char*)malloc(len);
	fread(buf,len,1,fp);
	std::string  st = buf;
	DecodeHtmlUTF8ToGB2312(st);

	char sou[] = {0x53,0x62,0x70,0xbd,0x68,0xec};
	std::string	syeiw = "";//sou;
	OnConvertText(syeiw);
	std::string styuie = "卢炽森";
	OnReverseText(styuie);

	//DecodeHtmlUTF8ToGB2312(styuie);


	char sz1[] = {"\\\"infor\\\":"};

	std::string _title = FindKey(st,"\"title\":",",");
	std::string _body = FindKey(st,"\\\"infor\\\":\\\"","\\\"}");


	char szb[100] = "测试2014";//"发天天新";//"卢炽森";//"杨昆";//"珠海";
	std::string name = GBKToUTF8(szb);
	name = UTF8ToGBK(name.c_str());

	//珠海
	utf[0] = 0xE7;
	utf[1] = 0x8F;
	utf[2] = 0xA0;
	utf[3] = 0xE6;
	utf[4] = 0xB5;
	utf[5] = 0xB7;
	std::string usenname = UTF8ToGBK(utf);

	int i = 0; 
	i ++;*/
}

union objbuffer21
{
	char data[40];
	char	buf;
};

// XorNode Class
template<typename ElemType>
class XorNode {
public:
	ElemType data;
	XorNode<ElemType> *xorPtr;
	XorNode(ElemType data):data(data) { }
};

unsigned long xor_func(unsigned long a, unsigned long b) {
	return ((unsigned long)(a) ^ (unsigned long)(b));    
}

void MemoryTest( void )
{
	int iuwe = (int)0x5584b0 ^ (int)0x558930;
	int woei = (int)0xd80 ^ (int)0x5584b0;

#define POOL_SIZE	1024
#define _NUM_		1000 * 50

	int BoosSize = sizeof(boost::pool<>);
	int MemSize = sizeof(MemPool_macro<>);
	while(true)
	{
		boost::pool<>	bst_pol(POOL_SIZE);
		MemPool_macro<> my_pol(POOL_SIZE);

		char data[POOL_SIZE] = {0};
		char* p[_NUM_] = {0};
		int i = 0, time = 0;
		/*while(true)
		{
		MemPool_macro	pol22(POOL_SIZE);
		for(i = 0 ; i < _NUM_; i ++)
		{
		p[i] =  (char*)pol22.Alloc();
		}

		for(i = 0 ; i < _NUM_; i ++)
		{
		pol22.Free(p[i]);
		}

		Sleep(100);
		}*/

		/*for(i = 0 ; i < _NUM_; i ++)
		{
		p[i] =  (char*)pol.Alloc();
		}

		for(i = 0 ; i < _NUM_; i ++)
		{
		pol.Free(p[i]);
		}*/

		time = GetTickCount();
		for(i = 0 ; i < _NUM_; i ++)
		{
			//MemPool_macro	pol1(POOL_SIZE);
			p[i] =  (char*)my_pol.Alloc();
			//memset(p[i],i,POOL_SIZE);
		}

		for(i = 0 ; i < _NUM_; i ++)
		{
			my_pol.Free(p[i]);
		}

		printf("MemPool_macro  time:%d\r\n", GetTickCount() - time);
		//pol.Release();

		//CMyMemPool	MyMemPool(POOL_SIZE);
		//CBoostMemPool MyMemPool(POOL_SIZE);

		/*for(i = 0 ; i < _NUM_; i ++)
		{
		p[i] =  (char*)MyMemPool.Alloc( POOL_SIZE );
		}

		for(i = 0 ; i < _NUM_; i ++)
		{
		MyMemPool.Free(p[i]);
		}*/

		time = GetTickCount();
		for(i = 0 ; i < _NUM_; i ++)
		{
			//CBoostMemPool MyMemPool1(POOL_SIZE);
			p[i] =  (char*)bst_pol.malloc();
			//memset(p[i],i,POOL_SIZE);
		}

		for(i = 0 ; i < _NUM_; i ++)
		{
			bst_pol.free(p[i]);
		}

		printf("boost::pool time:%d\r\n", GetTickCount() - time);


		/*time = GetTickCount();
		for(i = 0 ; i < _NUM_; i ++)
		{
		p[i] =  (char*)malloc(POOL_SIZE);
		memset(p[i],i,POOL_SIZE);
		}

		for(i = 0 ; i < _NUM_; i ++)
		{
		free(p[i]);
		}
		printf("sys::sys  time:%d\r\n", GetTickCount() - time);*/

		Sleep(1000);
	}
}

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


/*#define __VA_NARG__(...) \
(__VA_NARG_((_0, ## __VA_ARGS__, __RSEQ_N())) - 1) 
#define __VA_NARG_(args_list) \
__VA_ARG_N args_list 
#define __VA_ARG_N( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
_61,_62,_63,N,...) N 
#define __RSEQ_N() \
63, 62, 61, 60,                         \
59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
9,  8,  7,  6,  5,  4,  3,  2,  1,  0 */


#define DEFINE_FILTERING_KEYS_AUX_GET_FIELD(r, data, i, field) \
#field#data#i

long g_Count = 0;
long g_inc = 10000000;
void ThreadTest()
{
	for(int i = 0; i < g_inc; i ++)
	{
		g_Count ++;
	}
}

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

struct  FreeTest
{
	FreeTest( void )
	{

	}

	~FreeTest( void )
	{

	}

	void Free( void* p )
	{
		delete p;
	}
};

template<typename T, typename P1,typename P2>
struct TemTestBase
{
	virtual ~TemTestBase( void )
	{

	}

	virtual void Test( P1 num, P2 lon ) = 0;
};

template<typename T,typename P1,typename P2>
struct TemTest : public TemTestBase<T,P1,P2>
{
	TemTest( void )
		: i(100)
		, n(200)
	{

	}

	~TemTest( void )
	{

	}

	void Test( P1 num, P2 lon )
	{
		printf("typeid(T).name() :%s, P1:%s, P2:%s", typeid(T).name(), typeid(P1).name(),
			typeid(P2).name());

		int yi = 0;
	}

	int i;
	int n;
};

struct FunctionNice
{
	template<typename T, typename C>
	void Nice(void* p )
	{
		int p1 = 0;
		int c2 = 0;
	}
};

template<typename T>
struct FunctionTest
{
	void Invoke( void* p )
	{
		T* PTR = reinterpret_cast<T*>(p);
		int i = 0;
		i ++;
		
		P* ptr = reinterpret_cast<P*>(this);
		ptr->Add();
	}

	void Invoke123( void* pFun )
	{
		typedef void (FunctionTest<T>::*pfnFunc)(void*);
		union 
		{  
			pfnFunc func;  
			_functionaddress_::MicrosoftVirtualMFP s;
		} u;

		u.func = NULL;

		typedef void (*pfnFunc2)(void*);
		union 
		{  
			pfnFunc2 func;
			_functionaddress_::MicrosoftVirtualMFP s;
		} u1;
		
		
		u1.func = reinterpret_cast<pfnFunc2>(*reinterpret_cast<IntPtr*>(pFun));
		u.s.codeptr = u1.s.codeptr;
		(this->*u.func)(NULL);
	}
};

template<typename T>
void TemTesetFunc( T p )
{
	printf("T:%s",typeid(T).name());
	int n = sizeof(T);
	int i;
};

void QueueTest( void )
{
	const int iNum = 50000000;
	char	szwer[1024];
	memset(szwer, '1', sizeof(szwer));
	_foundation_::Queue< const char*, MemPool_macro<0,CNoneLock>, CNoneLock >	yc;
	std::deque< const char* > yc1;
	MemPool_macro<>	pool( sizeof(const char*) );
	/*SmartPtr<CTest>	szwer(new CTest);
	_foundation_::Queue< SmartPtr<CTest>, MemPool_macro<0,CNoneLock>, CNoneLock >	yc;
	std::deque< SmartPtr<CTest> > yc1;*/
	while(true)
	{
		int iTimer = 0;
		UInt32 _u32Size;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			yc.push_back(szwer);
		}
		printf("_foundation_::Queue push timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			if( !yc.empty() )
			{
				//SmartPtr<CTest> p = yc.front();
				const char* p = yc.front();
				yc.pop_front();
				//const char* p = yc.pop();
				//SmartPtr<CTest> p = yc.pop();
			}
		}
		printf("_foundation_::Queue pop timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			yc1.push_back(szwer);
		}
		printf("deque push timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			//SmartPtr<CTest> p = yc1.front();
			const char* p = yc1.front();
			yc1.pop_front();
		}
		printf("deque pop timer:%d\r\n", GetTickCount() - iTimer);
	}	
}

template<typename T>
 void initclone( void* p, T Value )
 {
	new(p) T(Value);
 }

void PtrTest( void )
{
	int iNumou = 100000000;
	boost::shared_ptr<CTest>	ppy(new CTest);
	{
		SmartPtr<CTest>	pp(new CTest, PoolTest );
		{
			SmartPtr<CTest> po = pp;
			int i;
		}
	}

	LinkList< SmartPtr<CTest3> >	y;
	/*while(true)
	{
		{
			_myfunction::CMyFunction<void (int,int,int)> MyDelgM;
			{
				SmartPtr<CTest3>	p(new CTest3);
				y.push_back(p);
			}
			SmartPtr<CTest3>	pp = y.pop_front();
		}

		//y.pop();
	}*/

	while(true)
	{
		int time1 = 0;
		/*for(int i = 0; i < iNumou; i ++)
		{
		SmartPtr<CTest>	pp(new CTest);
		}*/

		time1 = GetTickCount();
		for(int i = 0; i < iNumou; i ++)
		{
			//TR1::shared_ptr<CTest>	ppy(new CTest);
			//SharePtrTest(ppy);
			boost::shared_ptr<CTest>	ppy1 = ppy;
		}
		printf("SharePtr time:%d\r\n",GetTickCount() - time1);

		SmartPtr<CTest>	pp(new CTest);
		time1 = GetTickCount();
		for(int i = 0; i < iNumou; i ++)
		{
			//SmartPtr<CTest>	pp(new CTest);
			//SmartPtrTest(pp);
			SmartPtr<CTest> pp1 = pp;
		}
		printf("SmartPtr time:%d\r\n",GetTickCount() - time1);
		Sleep(2000);
	}


	{
		typedef void (CTest::*pfnFunc)(void*);

		union 
		{  
			pfnFunc func;  
			_functionaddress_::MicrosoftVirtualMFP s;
		} u;

		u.func = NULL;

		typedef void (FunctionTest<int>::*pfnFunc2)(void*);
		union 
		{  
			pfnFunc2 func;  
			_functionaddress_::MicrosoftVirtualMFP s;
		} u1;
		

		u1.func = &FunctionTest<int>::Invoke123;
		u.s.codeptr = u1.s.codeptr;

		typedef void (FunctionNice::*TestFunc12)(void*);
		TestFunc12 kw23 = reinterpret_cast<TestFunc12>(&FunctionNice::Nice<int,CTest>);
		void* pFun = reinterpret_cast<void*>(&kw23);

		CTest Test;
		CTest* pTest = &Test;
		(pTest->*u.func)(pFun);
	}

	DataTest	tre;
	tre.mf = 10;
	tre.s = "Function";
	tre.size = 100;
	tre.y  = 50;

	/*BindImpl<DataTest* (void*)>	poi;
	BindImpl<DataTest* (int)>*	poi223 = reinterpret_cast<BindImpl<DataTest* (int)>*>(&poi);
	poi223->InvokeEx<int>(10);
	IFunction<void* (void*)>* poiow = reinterpret_cast< IFunction<void* (void*)>* >(&poi);
	TemTesetFuc(poiow->Invoke(&tre));

	TemTesetFuc(&_myfunction::CMyFunction<void (int,int,int)>::Bind<CTest>);*/

	{
		CTest ytest;
		typedef void (CTest::*MoreFunc)(int,int,int);
		_myfunction::CMyFunction<void (int,int,int)> my1;
		_myfunction::CMyFunction<void (MoreFunc,const CTest*)> MyDelgM(&_myfunction::CMyFunction<void (int,int,int)>::Bind<CTest>,&my1);
		MyDelgM(&CTest::More,&ytest);
		my1(10,20,30);

/*
					IFunction<void (DEC_TYPE)>* pfunc123 = reinterpret_cast<IFunction<void (DEC_TYPE)>*>(Dll_Function_Get());
			pfunc123->Bind(PARAMTER_1);*/

		
	}

	/*IFunction<void* (void*)>* pfunc = reinterpret_cast<IFunction<void* (void*)>*>(BindImpl<DataTest>::get());
	TemTesetFuc( pfunc->Invoke(reinterpret_cast<void*>(&tre)) );*/

	TemTest<long, int, int> iuw;
	TemTestBase< int, long, long >* iuw8787 = reinterpret_cast<TemTestBase< int, long, long >*>(&iuw);
	iuw8787->Test(1,2);
	//{
		CTest Test22222;
		SmartFunction<void (void)> Smart(&CTest::Add,&Test22222);
		int Num = 10000000;
		/*while(true)
		{
			int time1 = GetTickCount();
			for(int i = 0; i < Num; i ++)
			{
				//printf("Num:%d\r\n",i);
				//SmartFunction<void (void)> Smart11(&CTest::Add,&Test);
				Smart();
			}
			printf("SmartFunction time:%d\r\n",GetTickCount() - time1);

			Sleep(100);
		}*/

		TSmartPtr<CTest>	pp(NULL);
		CTest* p = pp.get();
	//};

	FreeTest	Test;
	//while(true)
	{
		TSmartPtr<CTest>	pp(new CTest, NULL );
		TSmartPtr<CTest>	pp2(new CTest, NULL );
		pp2 = pp;
		{
			TSmartPtr<CTest>* pp = 0;
		}
		//ISmartPtr<CTest> iu = p;
		{
			TSmartPtr<CTest> po = pp;
			int i;
		}
		Sleep(10);
	};
}

void LockTest( void )
{
	int iNumou = 10000000;
	while(true)
	{
		int time1 = 0;
		CLock	Lock;
		time1 = GetTickCount();
		for(int i = 0; i < iNumou; i ++)
		{
			Lock.Lock();
			Lock.UnLock();
		}
		printf("CLock time:%d\r\n",GetTickCount() - time1);
		CLock_Win32	Lock_win;
		time1 = GetTickCount();
		for(int i = 0; i < iNumou; i ++)
		{
			Lock_win.Lock();
			Lock_win.UnLock();
		}
		printf("CLock_Win32 time:%d\r\n",GetTickCount() - time1);
		Sleep(2000);
	}
}

template <int N = 0>
struct IntTemTest
{  
	IntTemTest( void )
	{
		n = N;
	}

	~IntTemTest( void )
	{

	}

	int n;
}; 

/*template <>  
struct IntTemTest<0>
{  
IntTemTest( void )
{
n = 20;
}

~IntTemTest( void )
{

}

int n;
};*/ 

void DllTest( void )
{
	printf("uuid:%s\r\n",Dll_UUID_Get()->toString().c_str());

	{
		UInt64 ui = Dll_Timestamp_Get()->utcTime();
	}

	int num = 10000000;
	while(true)
	{
		int time = 0;
		time = GetTickCount();
		for(int i = 0; i < num; i ++)
		{
			TSmartPtr<ILock> SMPtr = Dll_Lock_Get();
			{
				TSmartPtr<ILock> SMPtr1 = SMPtr;
			}
		}

		printf("TSmartPtr<ILock> %d\r\n", GetTickCount() - time);

		time = GetTickCount();
		for(int i = 0; i < num; i ++)
		{
			/*TSmartPtr<IMemPool> SMPtr = Dll_MemPool_Get(100);
			{
				TSmartPtr<IMemPool> SMPtr1 = SMPtr;
			}*/
		}

		printf("TSmartPtr<IMemPool> %d\r\n", GetTickCount() - time);

		Sleep(10);
	}
}

int switch_magic(void) {
	static int i, state = 0;
	switch (state) {
case 0: /* start of function */
	for (i = 0; i < 10; i++) {
		state = 1; /* so we will come back to "case 1" */
		return i;
case 1:; /* resume control straight after the return */
	}
	}

	return -1;
}

/*namespace	StreamTest{*/

#define		STREAM_ADDRESS_DATA_BIT	1	
#pragma pack(push, 1)
	struct  _pre_hdr
	{
		UInt16	pre_size;		//预留的空间
		char	pre_data[1];	//预留的数据内容地址
	};

	struct _buf_hdr
	{
		UInt32		payload : 31;	//数据大小
		UInt32		is_reserve : 1; //是否存在预留数据区
		char		data[1];		//数据内容地址
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct _commandstream_hdr
	{
		UInt16			head;	//头信息	0xAA,0xBB
		UInt8			MF;		//分片标志位
		_buf_hdr		hdr;	//负载信息
	};
#pragma pack(pop)

	const UInt8	g_commandstream_hdr_size = sizeof(_commandstream_hdr);

#pragma pack(push, 1)
	struct _commandstream_head_buf
	{
		char	buf[g_commandstream_hdr_size];
		UInt8	offset;
	};

	struct _commandstream_buf
	{
		_commandstream_head_buf	head_buf;
		UInt32 offset;
		_buf_hdr* hdr;
	};

#pragma pack(pop)

	typedef _buf_hdr	_stream_hdr;

	const UInt16 g_commandhead = 0xAABB;

	struct	CommandStreamHdr
	{
		inline static UInt16	DataOffset( void )
		{
			return sizeof(_commandstream_hdr) - STREAM_ADDRESS_DATA_BIT;
		}

		inline static void SetMF( _commandstream_hdr* pDest, bool isMF = false )
		{
			pDest->MF = isMF;
		}

		inline static bool GetMF( _commandstream_hdr* pDest)
		{
			return pDest->MF;
		}

		inline static const UInt32	GetSize( _commandstream_hdr* pDest )
		{
			return pDest->hdr.payload;
		}

		inline static void	SetSize( _commandstream_hdr* pDest, UInt32 u32Size )
		{
			pDest->hdr.payload = u32Size;
		}

		inline static UInt16	GetHead( _commandstream_hdr* pDest )
		{
			return pDest->head;
		}

		inline static void	SetHead( _commandstream_hdr* pDest, UInt16 u16Head )
		{
			pDest->head = u16Head;
		}

		inline static char*	GetData( _commandstream_hdr* pDest )
		{
			return (char*)&pDest->hdr.data;
			//return reinterpret_cast<char*>(pDest) + sizeof(_commandstream_hdr);
		}

		inline static const UInt32	GetPayloadSize( _commandstream_hdr* pDest )
		{
			return pDest->hdr.payload + DataOffset();
		}

		inline static const UInt32	GetSizeToData( const char* pData )
		{
			return GetHdrToData(pData)->hdr.payload;
		}

		inline static bool GetMFToData( const char* pData ) 
		{
			return GetHdrToData(pData)->MF;
		}

		inline static bool SetMFToData( const char* pData, bool isMF = false )
		{
			GetHdrToData(pData)->MF = isMF;
		}

		inline static _commandstream_hdr*	GetHdrToData( const char* pData ) 
		{
			return ((_commandstream_hdr*)(pData - DataOffset()));
		}

		inline static UInt16	GetHdrSize( void ) 
		{
			return DataOffset();//sizeof(_commandstream_hdr);
		}
	};

	struct StreamBuf
	{
		inline static UInt16	ReserveDataOffset( void )
		{
			return sizeof(_pre_hdr) - STREAM_ADDRESS_DATA_BIT;
		}

		inline static UInt16	DataOffset( void )
		{
			return sizeof(_buf_hdr) - STREAM_ADDRESS_DATA_BIT;
		}

		inline static char*	GetData( _buf_hdr* pDest )
		{
			return (char*)&pDest->data;
			//return reinterpret_cast<char*>(pDest) + sizeof(_buf_hdr);
		}

		inline static	UInt32	GetSize( _buf_hdr* pDest )
		{
			return pDest->payload;
		}

		inline static	void	SetSize( _buf_hdr* pDest, UInt32 u32Size )
		{
			pDest->payload = u32Size;
		}

		inline static	void	SetReserveSize( _buf_hdr* pDest, UInt16 u16Size )
		{
			if( u16Size == 0 )
			{
				pDest->is_reserve = false;
				return ;
			}

			pDest->is_reserve = true;
			GetPreHdr( pDest )->pre_size = u16Size;
		}

		inline static	void	ClearReserveFlag( _buf_hdr* pDest )
		{
			pDest->is_reserve = false;
		}

		inline static	UInt32	GetSizeToData( const char* pData ) 
		{
			return ((_buf_hdr*)(pData - DataOffset()))->payload;
			//return ((_buf_hdr*)(pData - sizeof(_buf_hdr)))->payload;
		}

		inline static	void	SetSizeToData( const char* pData, UInt32 u32Size )
		{
			((_buf_hdr*)(pData - DataOffset()))->payload = u32Size;
			//((_buf_hdr*)(pData - sizeof(_buf_hdr)))->payload = u32Size;
		}

		inline static	_buf_hdr*	GetHeadToData( const char* pData )
		{
			return (_buf_hdr*)(pData - DataOffset());
			//return (_buf_hdr*)(pData - sizeof(_buf_hdr));
		}

		inline static	_pre_hdr*	GetPreHdr( _buf_hdr* pDest )
		{
			return pDest->is_reserve ? (_pre_hdr*)((char*)pDest + DataOffset() + pDest->payload) : NULL;
		}

		inline static	_pre_hdr*	GetPreHdrToData( const char* pData )
		{
			_buf_hdr* pDest = GetHeadToData(pData);
			return pDest->is_reserve ? GetPreHdr(pDest) : NULL;
		}
	};

	typedef SmartPTR<_buf_hdr>	  StreamBuf_ptr;

	enum
	{
		enSTATUS_NULL,
		enSTATUS_WAIT_HEAD,
		enSTATUS_WAIT_SLICING,
		enSTATUS_WAIT_DATA
	} enRecvStatus;

	int _Status = enSTATUS_WAIT_HEAD;
	_commandstream_buf	_CommandBuf = {0};
	MemPool_type		_Pool( 1400 + sizeof(_commandstream_hdr) );

	void	HandleRecv( const StreamBuf_ptr& Buf_ptr )
	{
		const UInt16 u16HdrSize = CommandStreamHdr::GetHdrSize();
		Int32	i32Payload = Buf_ptr->payload;
		char*	pData = StreamBuf::GetData(Buf_ptr.get());

		switch(_Status)
		{
		case enSTATUS_WAIT_HEAD:
__gt_head__:
			{			
				do 
				{
					const UInt8 u8Length = __min(u16HdrSize - _CommandBuf.head_buf.offset,
						i32Payload);

					//保存头部信息
					memcpy(&_CommandBuf.head_buf.buf[_CommandBuf.head_buf.offset], pData, u8Length);
					_CommandBuf.head_buf.offset += u8Length;
					pData += u8Length;
					i32Payload -= u8Length;

					if( _CommandBuf.head_buf.offset >= u16HdrSize )
					{
						_commandstream_hdr* CommandHdr = reinterpret_cast<_commandstream_hdr*>(_CommandBuf.head_buf.buf);
						if( CommandHdr->head == g_commandhead )
						{
							_CommandBuf.hdr = reinterpret_cast<_stream_hdr*>(_Pool.Alloc());
							_CommandBuf.hdr->payload = CommandHdr->hdr.payload;
							if( i32Payload >= _CommandBuf.hdr->payload )
							{//完整一个包
								memcpy( StreamBuf::GetData(_CommandBuf.hdr), pData, _CommandBuf.hdr->payload );
								/*EVENT_EXEC_READ(Stream, 
									StreamBuf_ptr(_CommandBuf.hdr, &MemPool_type::Free, &_Pool));*/

								i32Payload -= _CommandBuf.hdr->payload;
								pData += _CommandBuf.hdr->payload;
								_CommandBuf.head_buf.offset = 0;

							
								char iudata[1024] = {0};
								memcpy(iudata, StreamBuf::GetData(_CommandBuf.hdr), _CommandBuf.hdr->payload);

								memset(&_CommandBuf, 0, sizeof(_CommandBuf));
								if( i32Payload < 0 )
									printf("CommandStreamCtrl::HandleRecv Payload error!!\r\n");
							}
							else
							{
								if( i32Payload > 0 )
								{
									memcpy(&(StreamBuf::GetData(_CommandBuf.hdr)[_CommandBuf.offset]), pData, i32Payload);
									_CommandBuf.offset = i32Payload;
								}
								_Status = enSTATUS_WAIT_DATA;
								break;
							}
						}
						else
						{//整个包丢弃
							printf("Packet format is error!!!\r\n");
							_CommandBuf.head_buf.offset = 0;
							i32Payload = 0;
						}
					}
				} while ( i32Payload > 0 );
			}
			break;
		case enSTATUS_WAIT_SLICING://分片待实现

			break;
		case enSTATUS_WAIT_DATA:
			{
				const UInt32 u32Length = __min(_CommandBuf.hdr->payload - _CommandBuf.offset,
					i32Payload);
				memcpy(&(StreamBuf::GetData(_CommandBuf.hdr)[_CommandBuf.offset]), pData, u32Length);
				i32Payload -= u32Length;
				pData += u32Length;
				_CommandBuf.offset += u32Length;
				if( _CommandBuf.offset >= _CommandBuf.hdr->payload )
				{
					if( _CommandBuf.offset > _CommandBuf.hdr->payload)
					{
						printf("Recvice Data Is Error!!\r\n");
					}

					_CommandBuf.head_buf.offset = 0;
					_CommandBuf.offset = 0;
					_Status = enSTATUS_WAIT_HEAD;

					char iudata[1024] = {0};
					memcpy(iudata, StreamBuf::GetData(_CommandBuf.hdr), _CommandBuf.hdr->payload);
					memset(&_CommandBuf, 0, sizeof(_CommandBuf));
					/*EVENT_EXEC_READ(Stream, 
						StreamBuf_ptr(_CommandBuf.hdr, &MemPool_type::Free, &_Pool));*/
				}

				if( i32Payload > 0 )
				{
					goto __gt_head__;
				}
			}

			break;
		}
	}
//}

void MessageQueueTest( void )
{
	PriorityQueue<char*>	que;
	char sz980[5][100] = {{"000000"},{"111111"},{"222222"},{"333333"},{"444444"}};
	int	iNumou = 50000000;
	while(true)
	{
		const int pro = 0;//rand() * GetTickCount() % 5;
		int time1 = GetTickCount();
		for(int i = 0; i < iNumou; i ++)
		{
			//const int pro = rand() * GetTickCount() % 5;
			que.push(sz980[pro],pro);
			//printf("pro:%d\r\n",pro);
		}
		printf("time1 push:%d\r\n",GetTickCount() - time1);

		time1 = GetTickCount();
		for(int i = 0; i < iNumou; i ++)
		{
			if( !que.empty() )
			{
				char* ptr = que.front();
				que.pop_front();
				//printf("ptr:%s\r\n", ptr);
			}
			//int n = 0;
		}

		printf("time1 pop:%d\r\n",GetTickCount() - time1);
		Sleep(2000);
	}
}

void ThreadWrapTest( void* p )
{
	while(true)
	{
		try
		{
			ThreadWrap::interruption_point();
			Sleep(10);
		}
		catch (const thread_interrupted& e)
		{
			int i = e.get_code();
			printf("i:%d\r\n",i);
			break;
		}
	}
}

/*#define STREAM_ADDRESS_DATA_BIT		1

#pragma pack(push, 1)
struct  _pre_hdr
{
	UInt16	pre_size;		//预留的空间
	char	pre_data[1];	//预留的数据内容地址
};

struct _buf_hdr
{
	UInt32		payload : 31;	//数据大小
	bool		is_reserve : 1; //是否存在预留数据区
	char		data[1];		//数据内容地址
};
#pragma pack(pop)


struct StreamBuf
{
	inline static UInt16	DataOffset( void )
	{
		return sizeof(_buf_hdr) - STREAM_ADDRESS_DATA_BIT;
	}

	inline static char* GetData( _buf_hdr* pDest )
	{
		return (char*)&pDest->data;
		//return reinterpret_cast<char*>(pDest) + sizeof(_buf_hdr);
	}

	inline static	UInt32 GetSize( _buf_hdr* pDest )
	{
		return pDest->payload;
	}

	inline static	void SetSize( _buf_hdr* pDest, UInt32 u32Size )
	{
		pDest->payload = u32Size;
	}

	inline static	UInt32 GetSizeToData( const char* pData ) 
	{
		return ((_buf_hdr*)(pData - DataOffset()))->payload;
		//return ((_buf_hdr*)(pData - sizeof(_buf_hdr)))->payload;
	}

	inline static	void SetSizeToData( const char* pData, UInt32 u32Size )
	{
		((_buf_hdr*)(pData - DataOffset()))->payload = u32Size;
		//((_buf_hdr*)(pData - sizeof(_buf_hdr)))->payload = u32Size;
	}

	inline static	_buf_hdr* GetHeadToData( const char* pData )
	{
		return (_buf_hdr*)(pData - DataOffset());
		//return (_buf_hdr*)(pData - sizeof(_buf_hdr));
	}

	inline static	_pre_hdr*	GetPreHdr( _buf_hdr* pDest )
	{
		int ns = DataOffset() + pDest->payload;
		char* ps = (char*)pDest + ns;
		return pDest->is_reserve ? (_pre_hdr*)((char*)pDest + DataOffset() + pDest->payload) : NULL;
	}

	inline static	_pre_hdr*	GetPreHdrToData( const char* pData )
	{
		_buf_hdr* pDest = GetHeadToData(pData);
		return pDest->is_reserve ? GetPreHdr(pDest) : NULL;
	}
};*/

void TestBoostFunction( int iTest, char* sz, int y, int n )
{
	int i = 0;
}

//using namespace StreamTest;

#include <Foundation/platform/Arg.hpp>
#define	Arg	_foundation_::_function_::Arg

Arg<1> TetArg1 ( void )
{
	return _foundation_::_1;
}

Arg<2> TetArg2 ( void )
{
	return _foundation_::_2;
}


# define LIST_AV_ARGS_DECLARE123(r, data, i, field)\
	void operator[] (Arg<i>) const\
{ printf("i:%d\r\n",i); }\
	void operator[] (Arg<i> (*) ()) const\
{ printf("i:%d_%d\r\n",i); }

# define LIST_AV_ARGS_DECLARE1(r, data, i, field)\
	PP_CAT(A, MACRO_NUMBER_INC_##i) operator[] (Arg<i>) const\
{ return PP_CAT(_a, MACRO_NUMBER_INC_##i); }\
	PP_CAT(A, MACRO_NUMBER_INC_##i) operator[] (Arg<i> (*) ()) const\
{ return PP_CAT(_a, MACRO_NUMBER_INC_##i); }		
#undef Arg	


template<typename T>
class TestArg\
{\
public:\
	TestArg( void )\
	{}\
	~TestArg( void )\
	{}\
	SEQ_FOR_EACH_I(LIST_AV_ARGS_DECLARE123,_,VARIADIC_TO_SEQ(p1,p2,p3))\
	T	_j;
};

template<typename T = void>
struct	list_av1_0;

template<>
struct	list_av1_0<>
{
	explicit list_av1_0( void ) {}
	inline const int getnum( void ) const { return 0; }
};

#define	LIST_ARG_MACRO1(n,m)\
	template<MACRO_TARGLIST_DEF(n)>\
struct	list_av1_##n : public	list_av1_##m<MACRO_ARGLIST_DEF(m)>\
{\
	typedef void (_functionaddress_::GenericClass::*GenericMemFn)(MACRO_ARGLIST_DEF(n));\
	list_av1_##n( void ) {}\
	explicit list_av1_##n( MACRO_TPARAMLIST_DEF(n) )\
	: list_av1_##m<MACRO_ARGLIST_DEF(m)>(MACRO_PARAMLIST_DEF(m))\
	,_a##n( a##n ) \
{}\
	SEQ_FOR_EACH_I( LIST_AV_ARGS_DECLARE1, _, VARIADIC_TO_SEQ( MACRO_PARAMLIST_DEF(n) ) )\
	inline const int getnum( void ) const { return n; }\
	A##n _a##n;\
};

LIST_ARG_MACRO1(1,0)
LIST_ARG_MACRO1(2,1)
LIST_ARG_MACRO1(3,2)
LIST_ARG_MACRO1(4,3)
LIST_ARG_MACRO1(5,4)
LIST_ARG_MACRO1(6,5)
LIST_ARG_MACRO1(7,6)
LIST_ARG_MACRO1(8,7)
LIST_ARG_MACRO1(9,8)
LIST_ARG_MACRO1(10,9)
LIST_ARG_MACRO1(11,10)
LIST_ARG_MACRO1(12,11)

class X;
union function_buffer123
{
	// For pointers to function objects
	mutable void* obj_ptr;

	// For pointers to std::type_info objects
	/*struct type_t {
		// (get_functor_type_tag, check_functor_type_tag).
		const void* type;

		// Whether the type is const-qualified.
		bool const_qualified;
		// Whether the type is volatile-qualified.
		bool volatile_qualified;
	} type;*/

	// For function pointers of all kinds
	mutable void (*func_ptr)();

	// For bound member pointers
	struct bound_memfunc_ptr_t {
		void (X::*memfunc_ptr)(int);
		void* obj_ptr;
	} bound_memfunc_ptr;

	// For references to function objects. We explicitly keep
	// track of the cv-qualifiers on the object referenced.
	struct obj_ref_t {
		mutable void* obj_ptr;
		bool is_const_qualified;
		bool is_volatile_qualified;
	} obj_ref;

	// To relax aliasing constraints
	mutable char data;
};

struct Function_v20
{
	Function_v20( void )
	{

	}

	~Function_v20( void )
	{

	}

	int	CMyFunction_v20Test( int l, int t, const char* ptr, int w )
	//int CMyFunction_v20Test( int l )
	{
		return 1;
	}

	void	NoneMyFunction_v20Test( int i1, int i2 )
	{
		return ;
	}
	void	sptrMyFunction_v20Test( int i1, int i2, boost::shared_ptr<int> sptr )
	{
		return ;
	}

	void	MyFunction_v20Test( void )
	{
		return ;
	}
};

int	g_CMyFunction_v20Test( int l, int t, char* ptr, int w )
//int CMyFunction_v20Test( int l )
{
	return 1;
}

void	g_NoneMyFunction_v20Test( int i1, int i2 )
{
	return ;
}

void	g_sptrMyFunction_v20Test( int i1, int i2, boost::shared_ptr<int> sptr )
{
	return ;
}

void	g_MyFunction_v20Test( void )
{
	return ;
}

void	g_StringMyFunction_v20Test( int, const SmartPTR<int>& s1 )
{
	return ;
}

void Test_vo23( int i )
{
	
}

void Function_v20Test( void )
{
	boost::shared_ptr<int>	sptr(new int);
	CTest	Test;
	{
		Function_v20	vis;
		boost::function<void (char*,int)>  bswa1 = boost::bind(&Function_v20::CMyFunction_v20Test,&vis,
			10, 200, _1, _2);

		//test
		function20_bind_Ex(&Function_v20::MyFunction_v20Test,&vis)();
		function20_bind_Ex(&Function_v20::CMyFunction_v20Test,&vis)(10,200,"123",20);
		function20_bind_Ex(Test_vo23)(10);
		//test

		//
		//while(true)
		{
			_myfunction::CMyFunction<void (int, std::string,std::string,std::string)> My(&CTest::TestMony,&Test);
			_myfunction::CMyFunction_v20<void (int, std::string,std::string,std::string)>  wa1 = My;
			wa1 = My;
			wa1(10,"8888888888","123456789","987654321");

			_myfunction::CMyFunction<void (int, boost::shared_ptr<int>)> My1(&CTest::TestMony_sptr,&Test);
			_myfunction::CMyFunction_v20<void (void)>  wa2 = _myfunction::bind_v20(&CTest::TestMony_sptr,&Test,10,sptr);
			_myfunction::CMyFunction_v20<void (void)>  wa3 = _myfunction::bind_v20(&CTest::TestMony_sptr,&Test,10,sptr);
			wa2 = wa3;
			wa2 = wa3;
			wa2();
		};
	}

	Function_v20	vis;
	list_av_2_v20<Value_av<int>,Value_av<int>>	lav2(10,20);
	list_av_0_v20<>			lav;
	int iw1 = lav[lav2._a1];
	int iw2 = lav[lav2._a2];

	{
		int imt = sizeof(_myfunction::CMyFunction_v20<void (char*,int)>);
		int imt1 = sizeof(boost::function<void (char*,int)>);

		_myfunction::CMyFunction_v20<void (char*,int)>  wa1 = _myfunction::bind_v20(&Function_v20::CMyFunction_v20Test,&vis,
			10, 200, _foundation_::_1, _foundation_::_2);
		int iNum = 100000000;
		int ntime = 0;
		//while(true)
		{
			_myfunction::CMyFunction_v20<void (void)>  wa2 = _myfunction::bind_v20(&CTest::TestMony_sptr,&Test,10,sptr);
			ntime = GetTickCount();
			for(int i = 0; i < iNum; i ++)
			{
				/*_myfunction::CMyFunction_v20<void (char*,int)>  wa1 = _myfunction::bind_v20(&Function_v20::CMyFunction_v20Test,&vis,
					10, 200, _foundation_::_1, _foundation_::_2);
				wa1("123",4);*/
				_myfunction::CMyFunction_v20<void (void)>  wa2 = _myfunction::bind_v20(&CTest::TestMony_sptr,&Test,10,sptr);
				//_myfunction::CMyFunction_v20<void (void)>  wa3 = wa2;
				wa2();
			}
			printf("MyFunction wa1:%d\r\n",GetTickCount() - ntime);
		}


		boost::function<void (char*,int)>  bswa1 = boost::bind(&Function_v20::CMyFunction_v20Test,&vis,
			10, 200, _1, _2);

		{
			SmartPTR<int> sptri(new int);
			boost::function<void (char*,int)>  bswa2 = boost::bind(&g_StringMyFunction_v20Test,
				10, sptri);
			int is = sizeof(bswa2);
			{
				//boost::function<void (char*,int)>  bswa3 = bswa2;
			}
		}

		boost::function<void (void)>  wa2 = boost::bind(&CTest::TestMony_sptr,&Test,10,sptr);
		ntime = GetTickCount();
		for(int i = 0; i < iNum; i ++)
		{
			/*boost::function<void (char*,int)>  bswa1 = boost::bind(&Function_v20::CMyFunction_v20Test,&vis,
				10, 200, _1, _2);
			bswa1("123",4);*/
			boost::function<void (void)>  wa2 = boost::bind(&CTest::TestMony_sptr,&Test,10,sptr);
			//boost::function<void (void)>  wa3 = wa2;
			wa2();
		}
		printf("boost Function wa1:%d\r\n",GetTickCount() - ntime);
	}


	boost::shared_ptr<Function_v20> vis_sptr(new Function_v20());
	boost::shared_ptr<int> bst_sptr(new int);
	{
		_myfunction::CMyFunction_v20<void (void)>  wa_sptr = _myfunction::bind_v20(&Function_v20::sptrMyFunction_v20Test,vis_sptr,
			10, 200, bst_sptr);
		wa_sptr();
	}


	/*_myfunction::Invoke2_Handle<void, int, int>	gse(g_NoneMyFunction_v20Test);
	gse(10,20);*/

	_myfunction::CMyFunction_v20<void (char*,int)>  wa1 = _myfunction::bind_v20(&g_CMyFunction_v20Test,
		10, 200, _foundation_::_1, _foundation_::_2);

	_myfunction::CMyFunction_v20<void (void)>  wa2 = _myfunction::bind_v20(&g_NoneMyFunction_v20Test,
		10, 200);
	wa2();

	_myfunction::CMyFunction_v20<void (void)>  wa3 = _myfunction::bind_v20(&g_MyFunction_v20Test);
	wa3();

	_myfunction::CMyFunction_v20<void (char*,int)>  wao1 = _myfunction::bind_v20(&Function_v20::CMyFunction_v20Test,
		vis, 10, 200, _foundation_::_1, _foundation_::_2);
	wao1("123",4);

	_myfunction::CMyFunction_v20<void (void)>  wao2 = _myfunction::bind_v20(&Function_v20::NoneMyFunction_v20Test,
		&vis, 10,20);
	wao2();
	_myfunction::CMyFunction_v20<void (void)>  wao3 = _myfunction::bind_v20(&Function_v20::MyFunction_v20Test,
		&vis);
	wao3();

}

void LogTest( void )
{
	//CMyDirectory::RecursiveOnlyFile("c:\\1",0);
	CMyDirectory::Create("E:\\1\\22\\33\\44");

	LogFile_Capacity	logDate(100);
	logDate.Open("D:\\123.txt");
	Logger<0,LogFile_Capacity>	log(logDate);
	while(true)
	{
		//logDate.Print("123456789\r\n");
		log.Info("123456789\r\n");
		Sleep(2000);
	}
}

template<int N>
class TemplaObj
{
public:
	TemplaObj( void )
	{
		YU = N;
	}

	~TemplaObj( void )
	{

	}

	int YU;
};

class SockS : public _smart_ptr::enable_shared_from_this<SockS>
{
public:
	typedef _shared_ptr<SockS> self_type;

public:
	SockS( void )
	{

	}

	~SockS( void )
	{

	}

	const self_type Func( void )
	{
		return shared_from_this();
	}
};

void share_from_this_test( void )
{
	{
		_shared_ptr<SockS> puie1;
		{
			_shared_ptr<SockS> puie = _shared_ptr<SockS>( new SockS );
			puie1 = puie;
			int i = puie.use_count();
			puie->Func();

			i = puie.use_count();
			puie1 = puie->Func();
			i = puie.use_count();
			i ++;
		}

		int i = puie1.use_count();
		i ++;
	}
}

int main(int argc, char* argv[])
{
	Function_v20Test();

	MemoryTest();

	PtrTest();
	//LogTest();
	//boost::shared_ptr<CTest> pstr = boost::shared_ptr<CTest>(new CTest);
	//boost::function< void (int, int)>	bst_fn = boost::bind(&CTest::More1234, pstr/*&test1*/, 100, (char*) "123", _1, _2);
	{
		CTest	test1;

		function_buffer123 functor;
		int se = sizeof(functor);
		int sew = sizeof(boost::bind(&CTest::More1234, &test1, 100, (char*) "123", _1, _2));

		boost::function< void (int, int)>	bst_fn = boost::bind(&CTest::More1234, &test1, 100, (char*) "123", _1, _2);
		//boost::function< void (int, int)>	bst_fn = boost::bind(TestBoostFunction, 100, (char*) "123", _1, _2);
		bst_fn(500,20);
		int ii = sizeof(bst_fn);

		boost::function< void (int, int)>	bst_fn12 = bst_fn;
	}

	_myfunction::CMyFunction<void (int,int,int)> MyDelgM;
	int ii = sizeof(MyDelgM);


	Arg<0> a1;
	TestArg<int>	ar;
	ar[TetArg2];

	/*char szw[100] = {0};
	_buf_hdr*	se = (_buf_hdr*)szw;
	se->is_reserve = true;
	strcpy((char*)&se->data, "01234567891234567890");
	se->payload = strlen("01234567891234567890");
	const char* s2w = StreamBuf::GetData(se);
	int yszei = StreamBuf::GetSizeToData(s2w);

	_pre_hdr* pre = StreamBuf::GetPreHdr(se);
	pre->pre_size = 100;
	strcpy((char*)&pre->pre_data, "99999888888");
	_pre_hdr* pre212 = StreamBuf::GetPreHdr(se);*/



	/*ThreadWrap::Init();
	while(true)
	{
		ThreadWrap	Wrap(ThreadWrapTest, NULL);
		Sleep(10);
		Wrap.interrupt(20);
		Wrap.join();
		int i = 0;
	};
	ThreadWrap::Release();*/

	//MessageQueueTest();
	QueueTest();
	//LinkListTest();
	{
		char Hex1[] = {0x01,0x02,0x03,0x04,0x05};

		_myfunction::CMyFunction< void ( const StreamBuf_ptr& Buf_ptr )> lse;
		if( lse )
		{
			int i = 0;
		}

		_commandstream_hdr* pHdr = reinterpret_cast<_commandstream_hdr*>(_Pool.Alloc());
		if( pHdr != NULL )
		{
			memcpy(CommandStreamHdr::GetData(pHdr), Hex1, sizeof(Hex1));
			CommandStreamHdr::SetHead(pHdr,g_commandhead);
			CommandStreamHdr::SetSize( pHdr, sizeof(Hex1) );
			CommandStreamHdr::SetMF( pHdr, false );
		}

		char pt123[100] = {0};
		memcpy(pt123, pHdr, CommandStreamHdr::GetPayloadSize(pHdr));
		
		_stream_hdr* hdr_ptr = reinterpret_cast<_stream_hdr*>(_Pool.Alloc());
		hdr_ptr->payload = CommandStreamHdr::GetPayloadSize(pHdr);
		memcpy(StreamBuf::GetData(hdr_ptr), pHdr, hdr_ptr->payload);

		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		char head[] = {0xbb,0xaa};
		char mf[] = {0x00};
		char size[] = {0x05,0x00,0x00,0x00};
		char data[] = {0x11,0x12,0x13,0x14,0x15};
		char data1[] = {0x21,0x22};
		char data2[] = {0x23,0x24,0x25,0x87,0x98};

		_stream_hdr* hdr_ptr1 = reinterpret_cast<_stream_hdr*>(_Pool.Alloc());
		hdr_ptr->payload = 2;
		memcpy(StreamBuf::GetData(hdr_ptr), head, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 1;
		memcpy(StreamBuf::GetData(hdr_ptr), mf, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 4;
		memcpy(StreamBuf::GetData(hdr_ptr), size, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 5;
		memcpy(StreamBuf::GetData(hdr_ptr), data, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 2;
		memcpy(StreamBuf::GetData(hdr_ptr), data1, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 5;
		memcpy(StreamBuf::GetData(hdr_ptr), data2, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		char head1[] = {0xbb};
		char head2[] = {0xaa};
		char size22[] = {0x00,0x05,0x00,0x00,0x00};
		char data22[] = {0x11,0x12,0x13,0x14,0x15,0xbb,0xaa};
		char size23[] = {0x00,0x05,0x00,0x00,0x00,0x21};
		char data21[] = {0x22};
		char data23[] = {0x23,0x24,0x25};

		hdr_ptr->payload = 1;
		memcpy(StreamBuf::GetData(hdr_ptr), head1, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 1;
		memcpy(StreamBuf::GetData(hdr_ptr), head2, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 5;
		memcpy(StreamBuf::GetData(hdr_ptr), size22, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 7;
		memcpy(StreamBuf::GetData(hdr_ptr), data22, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 6;
		memcpy(StreamBuf::GetData(hdr_ptr), size23, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 1;
		memcpy(StreamBuf::GetData(hdr_ptr), data21, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));

		hdr_ptr->payload = 3;
		memcpy(StreamBuf::GetData(hdr_ptr), data23, hdr_ptr->payload);
		HandleRecv(StreamBuf_ptr(hdr_ptr,false));
	}

	//SmartFunctionTest();
	PtrTest();

	{
		boost::shared_ptr<CTest>	g_P;
	}

	MyFunctionTest();

	switch_magic();
	switch_magic();

	EvClass evclas;
	EvBase* ptrclas = &evclas;
	ptrclas->Typeof();

	DllTest();
	IntTemTest<>  Argv1;
	IntTemTest<>  Argv2;
	IntTemTest<>* pser = Singleton< IntTemTest<> >::instance();

	CTest1 t89;
	CTest1 t891 = &t89;
	int iLend = sizeof(_myfunction::CMyFunction<void (char*)>);
	int ilKW = sizeof(boost::function< void (char*)>);
	int isi = sizeof(stSMTHdr);
	int iLen = sizeof(boost::shared_ptr<long>);
	int il = sizeof(int);
	int s2l = sizeof(SmartPtr<long>);
	int i3232 = sizeof(boost::detail::shared_count);

	PtrTest();

	/*char sz980[100] = {"000000"};
	char sz981[100] = {"111111"};
	char sz982[100] = {"222222"};
	char sz983[100] = {"333333"};
	char sz984[100] = {"444444"};

	while(true)
	{
	PriorityQueue	que;
	int time1 = GetTickCount();
	for(int i = 0; i < iNumou; i ++)
	{
	que.push(i,sz984,sizeof(sz984),rand() * GetTickCount() % 5);
	}

	for(int i = 0; i < iNumou; i ++)
	{
	msgbuf* ptr = que.pop();
	}
	printf("time1:%d\r\n",GetTickCount() - time1);
	Sleep(2000);
	}

	std::vector<boost::shared_ptr<boost::thread> > threads;
	for(std::size_t i = 0; i < 40; ++ i)
	{
	boost::shared_ptr<boost::thread> thread(new boost::thread(
	boost::bind(ThreadTest)));
	threads.push_back(thread);
	}

	for(std::size_t i = 0; i < threads.size(); ++ i)
	{
	threads[i]->join();
	}

	printf("g_Count:%d\r\n",g_Count);

	while(true) Sleep(100);*/

	/*LONG you = 5;
	LONG ou = 10;

	CASFetchAndAdd(&you);
	CASFetchAndSub(&ou);*/

	MemoryTest();
	MyFunctionTest();
	while(true) Sleep(1000);

#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/variadic/size.hpp>

	std::string sywoy;
	sywoy.resize(1024 * 10);
	int len = sywoy.size();

	//std::string s78 = MACRO_PP_SEQ_FOR_EACH_I_M_I(10,DEFINE_FILTERING_KEYS_AUX_GET_FIELD,_,(int),0);//MACRO_PP_SEQ_FOR_EACH_I_M(4,(DEFINE_FILTERING_KEYS_AUX_GET_FIELD,_,(int),0));//BOOST_PP_SEQ_FOR_EACH_I_M(10,(DEFINE_FILTERING_KEYS_AUX_GET_FIELD,_,(int),0));//int yyn = MACRO_PP_TUPLE_ELEM(4,2,(10,20,30,40));
	//BOOST_PP_VARIADIC_TO_SEQ(1,2,3,4,5);
	//std::string s98 = BOOST_PP_SEQ_FOR_EACH_I(DEFINE_FILTERING_KEYS_AUX_GET_FIELD,_,MACRO_PP_VARIADIC_TO_SEQ(int1,int2,int3,int4,int5));
	std::string s98 = MACRO_PP_SEQ_FOR_EACH_I(DEFINE_FILTERING_KEYS_AUX_GET_FIELD,&,MACRO_PP_VARIADIC_TO_SEQ(int l1,int l2));

	//BOOST_PP_SEQ_FOR_EACH_I
	char sz21[] = "1467953453";
	/*int i = 0;
	for(i = 0; i < 10; i ++)
	{

	}

	BOOST_PP_VARIADIC_TO_SEQ*/

	/*#   define PP_SEQ_TAIL(seq) PP_SEQ_TAIL_ID(PP_SEQ_TAIL_I seq)
	#   define PP_SEQ_TAIL_ID(id) id
	#	define PP_SEQ_TAIL_I(x)

	PP_SEQ_TAIL((10));

	#    define PP_SEQ_SIZE(seq)	PP_CAT(PP_SEQ_SIZE_, PP_SEQ_SIZE_0 seq)
	#    define PP_CAT(a, b)		PP_CAT_I(a, b)
	#    define PP_CAT_I(a, b)		a ## b
	#	 define PP_SEQ_SIZE_0(_)	PP_SEQ_SIZE_1
	#	 define PP_SEQ_SIZE_PP_SEQ_SIZE_0 0
	#	 define PP_SEQ_SIZE_PP_SEQ_SIZE_1 1

	int yyu = PP_SEQ_SIZE();*/

	int yy = 0,uu = 0;
	int oou = MACRO_VA_NARG_SIZE(int,int,int);

	//int n = log((double)256)/log((double)2);//log((double)256);//log(8)/log(2);//
	NetControlTest();
	//list_av_6<char,int,int,char,std::string,int>	av('b',20,20,'x',std::string("300000"),10);
	//te4( 10,10,'a',std::string("10000000") );
	//te4( MACRO_RANGELIST_DEF(2,5,av.) );

	/*CTest	Test;
	boost::function<void (int,int,std::string)> f11 = boost::bind(&CTest::Test, &Test,_1,_2,_3); // 无参数，无返回值
	f11(10,10,std::string("12314654"));

	int len = sizeof(objbuffer21);

	//= bind
	int num = 100000000;
	int time1 = 0;

	{
	time1 = GetTickCount();
	for(int i = 0; i < num; i ++)
	{
	objbuffer21	buf;
	new(&buf.buf) CTest();
	}
	time1 = GetTickCount() - time1;
	printf("objbuffer21 :%d,size:%d\r\n",time1,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
	}

	{
	time1 = GetTickCount();
	for(int i = 0; i < num; i ++)
	{
	char buf[40];
	new(&buf) CTest();
	}
	time1 = GetTickCount() - time1;
	printf("buf :%d,size:%d\r\n",time1,sizeof(_myfunction::CMyFunction<int (int,int,int)>));
	}*/

	/*char obj1[100] = {0};
	{
	char* obj2 = reinterpret_cast<char*>(malloc(100));
	new (reinterpret_cast<void*>(obj2)) CTest();
	memcpy(obj1,obj2,sizeof(obj1));
	free(obj2);
	}

	(reinterpret_cast<CTest*>(obj1))->sss();*/


	//NetControlTest();
	//test
	//char *sz11 = "user=w=%u674e%u5f3a%u62c9&id=95390707&f=1;";
	/*char sysa[100] = "";
	sysa[0] = 0x67;
	sysa[1] = 0x4e;
	sysa[2] = 0x5f;
	sysa[3] = 0x4E;
	std::string st = UTF8ToGBK(sysa);*/
	//test

	//NewFunction();
	//MyFunctionTest();

	CMyDeque< boost::shared_ptr<CTest3> >	y;
	{
		_myfunction::CMyFunction<void (int,int,int)> MyDelgM;
		{
			boost::shared_ptr<CTest3>	p(new CTest3);
			p->Add();
			MyDelgM = _myfunction::bind(&CTest3::More,p);
			y.push_back(p);
		}

		MyDelgM(10,10,10);
		printf("main 11111\r\n");
	}

	printf("pop_front1 11111\r\n");
	y.pop_front();
	printf("inti\r\n");

	{
		CMyDeque< boost::shared_ptr<CTest> >	yy;
		boost::shared_ptr<CTest>	p(new CTest);
	}
	//g_P.reset();

	MemPool_type	MyMemPool(1024);
	//CMyMemPool_boost	MemPool_boost(1024);

	CMyDeque<int>				MyRANGEVector;
	//std::deque<int>	MyRANGEVector;

	void* sz[TIMER_DEF] = {0};
	int i = 0;
	int time = GetTickCount();

	while(true)
	{
		time = GetTickCount();
		for(i = 0; i < TIMER_DEF; i ++)
		{
			sz[i] = MyMemPool.Alloc();
			if(sz[i] != NULL)
				memset(sz[i],'1',1024);
		}
		time = GetTickCount() - time;
		printf("MyMemPool malloc:%d\r\n",time);

		time = GetTickCount();
		for(i = 0; i < TIMER_DEF; i ++)
		{
			MyMemPool.Free((unsigned char*)sz[i]);
			//MyRANGEVector.push_back(i);
		}
		time = GetTickCount() - time;
		printf("MyMemPool free:%d\r\n",time);

		Sleep(1000);
	}

	/*time = GetTickCount();
	for(i = 0; i < TIMER_DEF; i ++)
	{
	sz[i] = MemPool_boost.Alloc();
	memset(sz[i],'1',1024);
	}
	time = GetTickCount() - time;

	printf("MemPool_boost malloc:%d\r\n",time);

	time = GetTickCount();
	for(i = 0; i < TIMER_DEF; i ++)
	MemPool_boost.Free(sz[i]);
	time = GetTickCount() - time;
	printf("MemPool_boost Free:%d\r\n",time);*/

	i = 0;
	time = GetTickCount();
	for(i = 0; i < TIMER_DEF; i ++)
	{
		MyRANGEVector.pop_front2();
	}
	time = GetTickCount() - time;
	printf("pop:%d\r\n",time);

	i = 0;
	time = GetTickCount();
	for(i = 0; i < TIMER_DEF; i ++)
	{
		sz[i] = MyMemPool.Alloc();
		//if(sz[i] != NULL)
		memset(sz[i],'1',1024);
	}
	time = GetTickCount() - time;

	printf("MemPool malloc:%d\r\n",time);

	/*time = GetTickCount();
	for(i = 0; i < TIMER_DEF; i ++)
	{
	sz[i] = MemPool_boost.Alloc();
	memset(sz[i],'1',1024);
	}
	time = GetTickCount() - time;

	printf("MemPool_boost malloc:%d\r\n",time);*/

	time = GetTickCount();
	for(i = 0; i < TIMER_DEF; i ++)
	{
		sz[i] = (char*)malloc(1024);
		memset(sz[i],'1',1024);
	}
	time = GetTickCount() - time;

	printf("System malloc:%d\r\n",time);

	cin.get();

	try
	{
		EXCEPTION_BEGIN()
			char* p = NULL;
		//strcpy(p,"1111");
		EXCEPTION_END_THROW("p is NULL",0);

	}
	catch (CMyException& e)
	{
		printf("%s\r\n",e.what());
		printf("%s\r\n",e.display_message().c_str());
	}
	catch (...)
	{
		printf("sdfsdfdsf\r\n");
		return 0;
	}
	//return 0;

	printf("OS information:\r\n");
	printf("osName:%s,osVersion:%s,osArchitecture:%s,nodeName:%s,nodeId:%s",Environment::osName().c_str(),Environment::osVersion().c_str(),
		Environment::osArchitecture().c_str(),Environment::nodeName().c_str(),Environment::nodeId().c_str());

	UUIDGenerator* pUUIDGen = UUIDGenerator::defaultGenerator();
	for(int i = 0; i < 10000; i ++)
	{
		_UUID::UUID uid = pUUIDGen->createRandom();
		std::string suid = uid.toString();
		printf("uuid:%s\r\n",suid.c_str());
	}

	cin.get();

	return 0;
}