
#ifndef __LOGIMPL_20151102222110_1446474070_H__
#define __LOGIMPL_20151102222110_1446474070_H__

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#ifdef WIN32
	#include <libCommon/os/windowsi.h>
	#include <winuser.h>
#else
	#include <pthread.h>
#endif

#include <libFile/CMyFile.h>
#include <libTimestamp/Timestamp.h>

#define log_ansi_snprintf	snprintf
#define log_ansi_vsprintf	vsprintf
#define log_ansi_vsnprintf	vsnprintf


#define LOG_MAX_LEVEL		5
#define	LOG_BUF_MAX_SIZE	8192//2049

#define VA_ARGS_LIST(format)\
	char log_buffer[LOG_BUF_MAX_SIZE] = {0};\
	va_list args;\
	va_start(args, format);\
	int iSize = log_ansi_vsnprintf(log_buffer, LOG_BUF_MAX_SIZE - 1, format, args);\
	va_end(args);


#ifdef	WIN32 
	#define LOG_NAME			"d:\\spvnm.log"
#else
	#define LOG_NAME			"/data/log/spvnm.log"
#endif

enum enDataTime
{
	SHOW_DATE,
	SHOW_TIME,
	SHOW_DATETIME
};

static	unsigned int	my_get_current_thread_id( void )
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return	pthread_self();
#endif
}

static	_string_type	my_get_localtime( int iShowType = SHOW_DATETIME )
{
	struct tm timeinfo = {0};
	Timestamp_type().epochTM(&timeinfo);

	char szTime[1024] = {0};
	switch(iShowType)
	{
	case SHOW_DATE:
		sprintf ( szTime,"[%04d-%02d-%02d]", 
			timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
			timeinfo.tm_mday);
		break;
	case SHOW_TIME:
		sprintf ( szTime,"[%02d:%02d:%02d]", 
			timeinfo.tm_hour,
			timeinfo.tm_min,timeinfo.tm_sec);
		break;
	case SHOW_DATETIME:
		sprintf ( szTime,"[%04d-%02d-%02d %02d:%02d:%02d]", 
			timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
			timeinfo.tm_mday,timeinfo.tm_hour,
			timeinfo.tm_min,timeinfo.tm_sec);
		break;
	}

	return szTime;
	//return _string_type(szTime);
}

static	void	my_log_stdout( const char* c_szMessage )
{
	_string_type	sDataTime = my_get_localtime();
	fwrite(sDataTime.c_str(),sDataTime.length(), 1, stdout);
	fwrite("  ", strlen("  "), 1, stdout);
	fwrite(c_szMessage, strlen(c_szMessage), 1, stdout);
	fwrite("\n", strlen("\n"), 1, stdout);
}

static	void	my_log_file( const char* c_szMessage )
{
	/*_string_type sDate;
	_string_type	sTime;
	sDate = my_get_localtime(SHOW_DATE).c_str();
	sTime = my_get_localtime(SHOW_TIME).c_str();
	char szDateTime[100] = {0};
	sprintf(szDateTime,"%s%s",sDate.c_str(),sTime.c_str());
	File_type::GetInstance(LOG_NAME)->Write(szDateTime,strlen(szDateTime),sDate.c_str());
	File_type::GetInstance(LOG_NAME)->Write("  ",strlen("  "));
	File_type::GetInstance(LOG_NAME)->Write(c_szMessage,strlen(c_szMessage));
	File_type::GetInstance(LOG_NAME)->Write("\n", strlen("\n"));*/
}

typedef void (*LogDirectFun)(const char*);
static	int	my_log_printf( LogDirectFun pDirectFun , const char *format, ... )
{
	/*char log_buffer[LOG_BUF_MAX_SIZE] = {0};
	va_list args;
	va_start(args, format);
	int iSize = log_ansi_vsprintf(log_buffer, format, args);
	va_end(args);*/
	VA_ARGS_LIST(format);
	pDirectFun(log_buffer);
	return iSize;
}

//static  const char* c_MyLogType_201406260945[] = {"error","warning"};
static	int g_my_log_level_201406260945 = LOG_MAX_LEVEL;


inline	void	my_set_log_level( int level )
{
	g_my_log_level_201406260945 = level;
}

inline const int	my_get_log_level( void ) 
{
	return g_my_log_level_201406260945;
}

#define MACRO_DIRCET					my_log_stdout//my_log_file//
static	LogDirectFun	g_my_directfun_201406260945 = MACRO_DIRCET;
inline	LogDirectFun	my_set_directfun( LogDirectFun pfun )
{
	g_my_directfun_201406260945 = pfun;
}

inline const LogDirectFun	my_get_directfun( void )
{
	return g_my_directfun_201406260945;
}

#define	PRINTF_LOG( arg, ... )			my_log_printf(my_get_directfun(),arg,##__VA_ARGS__);

#define PRINTF_CODE_INFO()\
		PRINTF_LOG("File:%s:Line:%d\t:%s\n\t\t",__FILE__,__LINE__,__FUNCTION__);

#define DECL_LOG_FUNTION(name,level)\
		inline void Log##level##_Printf( const char *format, ... )\
		{\
			VA_ARGS_LIST(format);\
			my_get_directfun()(log_buffer);\
		}

DECL_LOG_FUNTION(Log,1)
DECL_LOG_FUNTION(Log,2)
DECL_LOG_FUNTION(Log,3)
DECL_LOG_FUNTION(Log,4)
DECL_LOG_FUNTION(Log,5)


#undef DECL_LOG_FUNTION


#define	OUTPUT_LOG(level,args, ...)\
		if(my_get_log_level() >= level)\
			Log##level##_Printf(args, ##__VA_ARGS__);

#define OUTPUT_ERROR_LOG(args, ...)\
		PRINTF_LOG("error: File:%s:Line:%d\t:%s\t\t",__FILE__,__LINE__,__FUNCTION__)\
		PRINTF_LOG(args, ##__VA_ARGS__)
#endif

