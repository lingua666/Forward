
#include <libCommon/WDump.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

	_string_type	WDump::GetLocalTime( void )
	{
		time_t t = time(0);
		struct tm* time = localtime(&t);
		char szTime[20] = {0};
		sprintf(szTime,"core_%04d%02d%02d%02d%02d%02d",time->tm_year + + 1900,time->tm_mon + 1,time->tm_mday,
			time->tm_hour,time->tm_min,time->tm_sec);
		return _string_type(szTime);
	}

	LONG __stdcall WDump::ExceptCallBack( EXCEPTION_POINTERS *pExcPointer)
	{
		char szFilePath[500] = {0};
		char ExePath[1024] = {0};
		::GetModuleFileNameA(NULL,ExePath,sizeof(ExePath));
		ExePath[strrchr( ExePath, '\\' ) - ExePath + 1] = 0;
		sprintf(szFilePath,"%s%s.dmp",ExePath,GetLocalTime().c_str());

		//创建dump文件
		HANDLE hFile = CreateFileA(szFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL ,NULL);

		//向文件写下当前程序崩溃相关信息
		if( hFile != INVALID_HANDLE_VALUE )
		{
			MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
			loExceptionInfo.ExceptionPointers = pExcPointer;
			loExceptionInfo.ThreadId = GetCurrentThreadId();
			loExceptionInfo.ClientPointers = TRUE;
			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
			CloseHandle(hFile);
		}

		/*EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了  
		EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束  
		EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行*/

		return EXCEPTION_CONTINUE_SEARCH;
	}

	WDump::WDump( void )
	{
		SetUnhandledExceptionFilter(&WDump::ExceptCallBack); 
	}

	/*!
	* @function   ~WDump
	* @brief    
	* @return  
	*/
	WDump::~WDump( void )
	{

	}

	static	WDump	g_WDump201510221749;

#endif


