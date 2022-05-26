
#ifndef __EXPORTS_20160616102142_1451301281_H__
#define __EXPORTS_20160616102142_1451301281_H__

#include "External.h"

//日志输出控制
#define ENABLE_LOG_PRINT

//日志信息输出到控制台
//#define ENABLE_OUTPUT_CONSOLE

//日志信息输出到文件
//#define ENABLE_OUTPUT_FILE

//日志信息输出到文件和控制台
#define ENABLE_OUTPUT_ALL

//默认日志级别
#define LOG_PRINT_LEVEL		0

#if !defined(PP_CAT)
	# define PP_CAT(a, b)		PP_CAT_I(a, b)
	# define PP_CAT_I(a, b)		a ## b
#endif

#define LOG_NAMESPACE_NAME(name)	PP_CAT(PP_CAT(_, name), _)

#define LOG_NAMESPACE_DECL_BEGIN1(name)\
		namespace  name{
		
#define LOG_NAMESPACE_DECL_BEGIN(name)\
		LOG_NAMESPACE_DECL_BEGIN1( LOG_NAMESPACE_NAME(name) )

#define LOG_NAMESPACE_DECL_END(name)	}


#define LOG_NAMESPACE_FUNCTION_VOID1(name, func)\
		name::func()

#define LOG_NAMESPACE_FUNCTION_VOID(name, func)\
		LOG_NAMESPACE_FUNCTION_VOID1(LOG_NAMESPACE_NAME(name), func)

#define LOG_NAMESPACE_FUNCTION1(name, func, parameter)\
		name::func(parameter)

#define LOG_NAMESPACE_FUNCTION(name, func, parameter)\
		LOG_NAMESPACE_FUNCTION1(LOG_NAMESPACE_NAME(name), func, parameter)
		
#define LOG_NAMESPACE_FUNCTION_ARGS1(name, func, ...)\
		name::func(__VA_ARGS__)

#define LOG_NAMESPACE_FUNCTION_ARGS(name, func, ...)\
		LOG_NAMESPACE_FUNCTION_ARGS1(LOG_NAMESPACE_NAME(name), func, __VA_ARGS__)

#if defined(ENABLE_LOG_PRINT)
	#if !defined(LOG_DECL)
		#define LOG_DECL
		#define LOG_OBJ_DECL(modulename)\
			LOG_NAMESPACE_DECL_BEGIN(modulename)\
					inline ILog* GetLogInstance( void )\
					{\
						static ILog* s_Log = MLog_AllocObj();\
						return s_Log;\
					}\
					inline int Open( void )\
					{\
						char szPath[1024] = {0};\
						MLog_GetModuleLogPath(#modulename, szPath, sizeof(szPath));\
						return GetLogInstance()->Open(szPath);\
					}\
					inline void Close( void )\
					{\
						GetLogInstance()->Close();\
					}\
					inline void SetLog( int iLevel )\
					{\
						LOG_Print_SetLevel(modulename, iLevel < 0 ? 0xFF : iLevel)\
						if( iLevel < 0 )\
							LOG_Print_Close(modulename)\
						else\
							LOG_Print_Open(modulename)\
					}\
			LOG_NAMESPACE_DECL_END(modulename)

		#define LOG_Print_SetLog(modulename, iLevel)	LOG_NAMESPACE_FUNCTION(modulename, SetLog, iLevel);//_##modulename##_##::SetLog(iLevel);
		#define LOG_Print_Open(modulename)				LOG_NAMESPACE_FUNCTION_VOID(modulename, Open);//_##modulename##_##::Open();
		#define LOG_Print_Close(modulename)				LOG_NAMESPACE_FUNCTION_VOID(modulename, Close);//_##modulename##_##::Close();

		#define LOG_Print_isOpen(modulename)			LOG_NAMESPACE_FUNCTION_VOID(modulename, GetLogInstance()->isOpen)//_##modulename##_##::GetLogInstance()->isOpen()
		#define LOG_Print_SetLevel(modulename, level)	LOG_NAMESPACE_FUNCTION(modulename, GetLogInstance()->SetLevel, level);//_##modulename##_##::GetLogInstance()->SetLevel(level);
		#define LOG_Print_Info(modulename, ...)			LOG_NAMESPACE_FUNCTION_ARGS(modulename, GetLogInstance()->Info, __VA_ARGS__);//_##modulename##_##::GetLogInstance()->Info(__VA_ARGS__);
		#define LOG_Print_Warning(modulename, ...)		LOG_NAMESPACE_FUNCTION_ARGS(modulename, GetLogInstance()->Warning, __VA_ARGS__);//_##modulename##_##::GetLogInstance()->Warning(__VA_ARGS__);
		#define LOG_Print_Debug(modulename, ...)		LOG_NAMESPACE_FUNCTION_ARGS(modulename, GetLogInstance()->Debug, __VA_ARGS__);//_##modulename##_##::GetLogInstance()->Debug(__VA_ARGS__);
		#define LOG_Print_Error(modulename, ...)		LOG_NAMESPACE_FUNCTION_ARGS(modulename, GetLogInstance()->Error, __VA_ARGS__);//_##modulename##_##::GetLogInstance()->Error(__VA_ARGS__);
		#define LOG_Print_FATAL(modulename, ...)		LOG_NAMESPACE_FUNCTION_ARGS(modulename, GetLogInstance()->FATAL, __VA_ARGS__);//_##modulename##_##::GetLogInstance()->FATAL(__VA_ARGS__);
	#endif		

	#define DLL_LOG_DECL_DEF(modulename)\
		EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE SetDebugModule( int iEnable );

	#define DLL_LOG_DEF(modulename)\
		LOG_OBJ_DECL(modulename)\
		EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE SetDebugModule( int iEnable )\
		{\
			LOG_Print_SetLevel(modulename, iEnable == -1 ? 0xFF : 0)\
			if( iEnable == -1 )\
			{\
				LOG_Print_Close(modulename)\
			}\
			else if( iEnable == 1)\
			{\
				LOG_Print_Open(modulename)\
			}\
		}
	
#else
	#define DLL_LOG_DECL_DEF(modulename)\
		EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE SetDebugModule( int iEnable );

	#define DLL_LOG_DEF(modulename)\
		EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE SetDebugModule( int iEnable )\
		{\
		}

	#define LOG_OBJ_DECL(modulename)
	#define LOG_Print_SetLog(modulename, iLevel)
	#define LOG_Print_Open(modulename)
	#define LOG_Print_Close(modulename)
	#define LOG_Print_isOpen(modulename)	0
	#define LOG_Print_SetLevel(modulename, level)
	#define LOG_Print_Info(modulename, ...)
	#define LOG_Print_Warning(modulename, ...)
	#define LOG_Print_Debug(modulename, ...)
	#define LOG_Print_Error(modulename, ...)
	#define LOG_Print_FATAL(modulename, ...)
#endif

//加密验证开关
//#define CHECK_FILE_DEF	1

#ifdef CHECK_FILE_DEF

	#define	CHECK_FILE_FAILED_DEF( log_module )\
		static bool CheckFailed_201609170000( int iValue )\
		{\
			if( ! LOG_Print_isOpen(log_module) )\
				LOG_Print_Open(log_module)\
			LOG_Print_Error(log_module, "File Error 0xE0000001(%d)", iValue);\
			return true;\
		}

	#define CHECK_FILE_IMPL_DEF(Handle)\
		{\
			char szPath[1024] = {0};\
			::GetModuleFileNameA(Handle, szPath, sizeof(szPath));\
			CheckValidity Check(szPath, function20_bind_Ex(CheckFailed_201609170000));\
		}

	#define CHECK_FILE_VALIDITY()\
		{\
			/*HMODULE hModule = GetModuleHandle(NULL); //获得进程空间的基址*/\
			char szPath[1024] = {0};\
			MEMORY_BASIC_INFORMATION mbi;\
			static int dummy;\
			::VirtualQuery( &dummy, &mbi, sizeof(mbi) );\
			::GetModuleFileNameA(reinterpret_cast<HMODULE>(mbi.AllocationBase),\
							szPath, sizeof(szPath));\
			_validity_::CheckValidity Check(szPath, function20_bind_Ex(CheckFailed_201609170000));\
		}

#else
	#define	CHECK_FILE_FAILED_DEF( log_module )
	#define CHECK_FILE_IMPL_DEF( Handle )
	#define CHECK_FILE_VALIDITY()
#endif

#define LOAD_DLL_INIT(...)\
	CHECK_FILE_VALIDITY()
	//CHECK_FILE_IMPL_DEF( __VA_ARGS__ )

typedef	ILog*	HLOGOBJ;
typedef	void*	HLOG;


/*****************************************************************
/*函数说明：	分配日志接口对象资源（只适用C++）
/*参数说明：	
/*				
/*返回值：		NULL:失败  非NULL: 成功
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOGOBJ _CALLTYPE MLog_AllocObj( void );


/*****************************************************************
/*函数说明：	分配日志接口对象资源并打开日志文件（只适用C++）
/*参数说明：	szPath: 文件路径
/*				
/*返回值：		NULL:失败  非NULL: 成功
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOGOBJ _CALLTYPE MLog_AllocObjWithPath( const char* szPath );


/*****************************************************************
/*函数说明：	释放日志接口对象资源（只适用C++）
/*参数说明：	hObj : 日志接口对象指针
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_FreeObj( HLOGOBJ hObj );


/*****************************************************************
/*函数说明：	获取日志完整路径
/*参数说明：	c_szSubDirName: 子目录 为空则不新建子目录
/*				szOut: 接收日志完整路径缓冲区
/*				uSize: 缓冲区大小
/*返回值：		路径长度
*****************************************************************/
_SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_GetModuleLogPath( const char* c_szSubDirName, 
														char* szOut, int uSize);


/*****************************************************************
/*函数说明：	分配日志句柄
/*参数说明：	
/*				
/*返回值：		>0:成功  <=0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOG _CALLTYPE MLog_Alloc( void );


/*****************************************************************
/*函数说明：	释放日志句柄资源
/*参数说明：	hLog : 日志句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_Free( HLOG hLog );


/*****************************************************************
/*函数说明：	打开日志文件（如果文件已经存在，则内容在结尾追加）
/*参数说明：	hLog : 日志句柄
/*				c_szPath: 日志文件完整路径
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Open( HLOG hLog, const char* c_szPath );


/*****************************************************************
/*函数说明：	设置日志级别（总共6级: 0 - 4, 日志只输出级别高于或等于日志级别的日志）
/*参数说明：	hLog : 日志句柄
/*				c_szPath: 日志文件完整路径
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_SetLevel( HLOG hLog, unsigned char uLevel );


/*****************************************************************
/*函数说明：	关闭日志文件
/*参数说明：	hLog : 日志句柄
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Close( HLOG hLog );


/*****************************************************************
/*函数说明：	输出日志信息（日志级别0）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Info( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*函数说明：	输出日志信息（日志级别1）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Warning( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*函数说明：	输出日志信息（日志级别2）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Debug( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*函数说明：	输出日志信息（日志级别3）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Error( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*函数说明：	输出日志信息（日志级别4）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_FATAL( HLOG hLog, const char* c_szMessage );



/*****************************************************************
/*函数说明：	设置所有日志级别（总共6级: 0 - 4, 日志只输出级别高于或等于日志级别的日志）
/*参数说明：	hLog : 日志句柄
/*				c_szPath: 日志文件完整路径
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_SetAllLevel(unsigned char uLevel);



/*****************************************************************
/*函数说明：	设置日志级别（总共6级: 0 - 4, 日志只输出级别高于或等于日志级别的日志）
/*参数说明：	hLog : 日志句柄
/*				c_szPath: 日志文件完整路径
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS unsigned char _CALLTYPE MLog_GetAllLevel(void);

#endif//__PLC_OMRON_20151228191441_1451301281_H__
