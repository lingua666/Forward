
#ifndef __EXPORTS_20160616102142_1451301281_H__
#define __EXPORTS_20160616102142_1451301281_H__

#include "External.h"

//��־�������
#define ENABLE_LOG_PRINT

//��־��Ϣ���������̨
//#define ENABLE_OUTPUT_CONSOLE

//��־��Ϣ������ļ�
//#define ENABLE_OUTPUT_FILE

//��־��Ϣ������ļ��Ϳ���̨
#define ENABLE_OUTPUT_ALL

//Ĭ����־����
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

//������֤����
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
			/*HMODULE hModule = GetModuleHandle(NULL); //��ý��̿ռ�Ļ�ַ*/\
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
/*����˵����	������־�ӿڶ�����Դ��ֻ����C++��
/*����˵����	
/*				
/*����ֵ��		NULL:ʧ��  ��NULL: �ɹ�
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOGOBJ _CALLTYPE MLog_AllocObj( void );


/*****************************************************************
/*����˵����	������־�ӿڶ�����Դ������־�ļ���ֻ����C++��
/*����˵����	szPath: �ļ�·��
/*				
/*����ֵ��		NULL:ʧ��  ��NULL: �ɹ�
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOGOBJ _CALLTYPE MLog_AllocObjWithPath( const char* szPath );


/*****************************************************************
/*����˵����	�ͷ���־�ӿڶ�����Դ��ֻ����C++��
/*����˵����	hObj : ��־�ӿڶ���ָ��
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_FreeObj( HLOGOBJ hObj );


/*****************************************************************
/*����˵����	��ȡ��־����·��
/*����˵����	c_szSubDirName: ��Ŀ¼ Ϊ�����½���Ŀ¼
/*				szOut: ������־����·��������
/*				uSize: ��������С
/*����ֵ��		·������
*****************************************************************/
_SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_GetModuleLogPath( const char* c_szSubDirName, 
														char* szOut, int uSize);


/*****************************************************************
/*����˵����	������־���
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <=0: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOG _CALLTYPE MLog_Alloc( void );


/*****************************************************************
/*����˵����	�ͷ���־�����Դ
/*����˵����	hLog : ��־���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_Free( HLOG hLog );


/*****************************************************************
/*����˵����	����־�ļ�������ļ��Ѿ����ڣ��������ڽ�β׷�ӣ�
/*����˵����	hLog : ��־���
/*				c_szPath: ��־�ļ�����·��
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Open( HLOG hLog, const char* c_szPath );


/*****************************************************************
/*����˵����	������־�����ܹ�6��: 0 - 4, ��־ֻ���������ڻ������־�������־��
/*����˵����	hLog : ��־���
/*				c_szPath: ��־�ļ�����·��
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_SetLevel( HLOG hLog, unsigned char uLevel );


/*****************************************************************
/*����˵����	�ر���־�ļ�
/*����˵����	hLog : ��־���
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Close( HLOG hLog );


/*****************************************************************
/*����˵����	�����־��Ϣ����־����0��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Info( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*����˵����	�����־��Ϣ����־����1��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Warning( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*����˵����	�����־��Ϣ����־����2��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Debug( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*����˵����	�����־��Ϣ����־����3��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_Error( HLOG hLog, const char* c_szMessage );


/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MLog_FATAL( HLOG hLog, const char* c_szMessage );



/*****************************************************************
/*����˵����	����������־�����ܹ�6��: 0 - 4, ��־ֻ���������ڻ������־�������־��
/*����˵����	hLog : ��־���
/*				c_szPath: ��־�ļ�����·��
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLog_SetAllLevel(unsigned char uLevel);



/*****************************************************************
/*����˵����	������־�����ܹ�6��: 0 - 4, ��־ֻ���������ڻ������־�������־��
/*����˵����	hLog : ��־���
/*				c_szPath: ��־�ļ�����·��
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS unsigned char _CALLTYPE MLog_GetAllLevel(void);

#endif//__PLC_OMRON_20151228191441_1451301281_H__
