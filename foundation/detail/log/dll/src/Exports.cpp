
#include <libLog/Logger.h>
#include <dllLog/Exports.h>

class _CLog_: public ILog
{
public:
	_CLog_( void )
		: _uLevel(-1)
	{

	}

	_CLog_( const char* c_szPath )
		: _uLevel(-1)
	{
		Open(c_szPath);
	}

	~_CLog_( void )
	{

	}

	int	Open( const char* c_szPath )
	{
#if defined(ENABLE_OUTPUT_FILE) || defined(ENABLE_OUTPUT_ALL)
		return _Log.Open(c_szPath);
#endif
		return 1;
	}

	bool isOpen( void ) const
	{
#if defined(ENABLE_OUTPUT_FILE) || defined(ENABLE_OUTPUT_ALL)
		return _Log.isOpen();
#endif
		return 1;
	}

	void Close( void )
	{
#if defined(ENABLE_OUTPUT_FILE) || defined(ENABLE_OUTPUT_ALL)
		_Log.Close();
#endif
	}

	int SetLevel( UInt8 uLevel )
	{
		_uLevel = uLevel;
		return 1;
	}

	void Info( const char *format, ... )
	{
		LOG_PRINF_DECL(1,"[Info]")
	}

	void Warning( const char *format, ... )
	{
		LOG_PRINF_DECL(2,"[Warning]")
	}

	void Debug( const char *format, ... )
	{
		LOG_PRINF_DECL(3,"[Debug]")
	}

	void Error( const char *format, ... )
	{
		LOG_PRINF_DECL(4,"[Error]")
	}

	void FATAL( const char *format, ... )
	{
		LOG_PRINF_DECL(5,"[FATAL]")
	}
	
private:
	void	Print( const char* format, const char* szRemark )
	{
#if defined(ENABLE_OUTPUT_FILE) || defined(ENABLE_OUTPUT_ALL)
		_Log.Print(format, szRemark);
#endif

#if defined(ENABLE_OUTPUT_CONSOLE) || defined(ENABLE_OUTPUT_ALL)
		_Std.Print(format, szRemark);
#endif
	}

private:
	UInt8				_uLevel;

#if defined(ENABLE_OUTPUT_FILE) || defined(ENABLE_OUTPUT_ALL)
	_log_::LogFile_Date _Log;
#endif

#if defined(ENABLE_OUTPUT_CONSOLE) || defined(ENABLE_OUTPUT_ALL)
	_log_::LogIOStd		_Std;
#endif
};

/*****************************************************************
/*函数说明：	分配日志接口对象资源（只适用C++）
/*参数说明：	
/*				
/*返回值：		NULL:失败  非NULL: 成功
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOGOBJ _CALLTYPE MLog_AllocObj( void )
{
	return reinterpret_cast<HLOGOBJ>(new (std::nothrow) _CLog_());
}


/*****************************************************************
/*函数说明：	分配日志接口对象资源（只适用C++）
/*参数说明：	szPath: 文件路径
/*				
/*返回值：		NULL:失败  非NULL: 成功
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOGOBJ _CALLTYPE MLog_AllocObjWithPath( const char* szPath )
{
	return reinterpret_cast<HLOGOBJ>(new (std::nothrow) _CLog_(szPath));
}


/*****************************************************************
/*函数说明：	释放日志接口对象资源（只适用C++）
/*参数说明：	hObj : 日志接口对象指针
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_FreeObj( HLOGOBJ hObj )
{
	if( hObj > 0 )
		delete hObj;
}


/*****************************************************************
/*函数说明：	获取日志完整路径（组成: 程序运行路径+Log+模块名称+当前系统时间(如20160616)+.txt）（只适用C++）
/*参数说明：	c_szSubDirName: 模块名称 为空则不新建子目录
/*				szOut: 接收日志完整路径缓冲区
/*				uSize: 缓冲区大小
/*返回值：		如: E:\LogTest\Log\Module\20160616.txt
*****************************************************************/
_SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_GetModuleLogPath( const char* c_szSubDirName,
														char* szOut, int uSize )
{
	_string_type s;
	
	if( c_szSubDirName != NULL )
	{
		s = _file_::CMyDirectory::GetCurrentDirPath() + _file_::CMyDirectory::GetSplit();
		s +=  "Log";
		s += _file_::CMyDirectory::GetSplit();
		s += c_szSubDirName;
	}
	else
	{
		s = _file_::CMyDirectory::GetCurrentDirPath() + _file_::CMyDirectory::GetSplit();
		s += "Log";
	}

	s += _file_::CMyDirectory::GetSplit() + Timestamp_type().DateString() + ".txt";
	if( szOut == NULL )
		return s.size();

	int iLen = __min(s.size(), uSize);
	strncpy(szOut, s.c_str(), iLen);
	return iLen;
}

/*****************************************************************
/*函数说明：	分配日志句柄
/*参数说明：	
/*				
/*返回值：		>0:成功  <=0: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOG _CALLTYPE MLog_Alloc( void )
{
	return reinterpret_cast<HLOG>(new (std::nothrow) _CLog_());
}


/*****************************************************************
/*函数说明：	释放日志句柄资源
/*参数说明：	hLog : 日志句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_Free( HLOG hLog )
{
	if( hLog > 0 )
	{
		delete reinterpret_cast<_CLog_*>(hLog);
	}
}


/*****************************************************************
/*函数说明：	打开日志文件
/*参数说明：	hLog : 日志句柄
/*				c_szPath: 日志文件完整路径
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Open( HLOG hLog, const char* c_szPath )
{
	return reinterpret_cast<_CLog_*>(hLog)->Open(c_szPath);
}


/*****************************************************************
/*函数说明：	设置日志级别（总共6级: 0 - 5, 日志只输出级别高于或等于日志级别的日志）
/*参数说明：	hLog : 日志句柄
/*				c_szPath: 日志文件完整路径
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_SetLevel( HLOG hLog, UInt8 uLevel )
{
	if( hLog > 0 )
		return reinterpret_cast<_CLog_*>(hLog)->SetLevel(uLevel);
	return -1;
}


/*****************************************************************
/*函数说明：	关闭日志文件
/*参数说明：	hLog : 日志句柄
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Close( HLOG hLog )
{
	if( hLog > 0 )
	{
		reinterpret_cast<_CLog_*>(hLog)->Close();
		return 1;
	}

	return -1;
}


/*****************************************************************
/*函数说明：	输出日志信息（日志级别1）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Info( HLOG hLog, const char* c_szMessage )
{
	if( hLog > 0 )
	{
		reinterpret_cast<_CLog_*>(hLog)->Info(c_szMessage);
		return 1;
	}
	return -1;
}


/*****************************************************************
/*函数说明：	输出日志信息（日志级别2）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Warning( HLOG hLog, const char* c_szMessage )
{
	if( hLog > 0 )
	{
		reinterpret_cast<_CLog_*>(hLog)->Warning(c_szMessage);
		return 1;
	}
	return -1;
}


/*****************************************************************
/*函数说明：	输出日志信息（日志级别3）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Debug( HLOG hLog, const char* c_szMessage )
{
	if( hLog > 0 )
	{
		reinterpret_cast<_CLog_*>(hLog)->Debug(c_szMessage);
		return 1;
	}
	return -1;
}


/*****************************************************************
/*函数说明：	输出日志信息（日志级别4）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Error( HLOG hLog, const char* c_szMessage )
{
	if( hLog > 0 )
	{
		reinterpret_cast<_CLog_*>(hLog)->Error(c_szMessage);
		return 1;
	}
	return -1;
}


/*****************************************************************
/*函数说明：	输出日志信息（日志级别5）
/*参数说明：	hLog : 日志句柄
				c_szMessage： 日志信息
/*				
/*返回值：		1:成功  <0: 错误代码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_FATAL( HLOG hLog, const char* c_szMessage )
{
	if( hLog > 0 )
	{
		reinterpret_cast<_CLog_*>(hLog)->FATAL(c_szMessage);
		return 1;
	}
	return -1;
}