
#ifndef __COMMON_20160328140444_1459145084_H__
#define __COMMON_20160328140444_1459145084_H__

#include <dllCommon/modafx.h>
#include <dllLog/Exports.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

typedef	Int64			THandle;
typedef Int64			NETNODE;		// NET ENDPOINT
typedef NETNODE			NETHANDLE;

struct tagTModuleInfoRecord
{
	char*	MyModuleID;	//模块ID
	char*	MyModuleName;	//模块名称
	char*	MyModuleAuthor;
	char*	MyModuleVer;
	char*	MyModuleTime;
	char*	MyModuleFunc;
	char*	MyModuleBit;
	char*	MyRunPlatform;
};

#if (TYPES_ARCH_64_BIT == 1)
	#define OS_BIT_STRING "64Bit"
#else
	#define OS_BIT_STRING "32Bit"
#endif

#if defined(PLATFORM_OS_FAMILY_UNIX) && PLATFORM_OS == PLATFORM_OS_LINUX
	#define OS_TYPE_STRING "Liunx"
#elif defined(PLATFORM_OS_FAMILY_UNIX) && PLATFORM_OS == PLATFORM_OS_ANDROID
	#define OS_TYPE_STRING "Android"
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
	#define OS_TYPE_STRING "Windows"
#endif

#define DLL_INIT_DECL_DEF()\
	EXTERN_C _SYMBOL_DLL_EXPORTS tagTModuleInfoRecord* _CALLTYPE GetModuleInfo( void );

#define DLL_INIT_DEF(vari_name, module_id, module_name, module_version, module_time, module_func)\
	static tagTModuleInfoRecord*	vari_name = NULL;\
	EXTERN_C _SYMBOL_DLL_EXPORTS tagTModuleInfoRecord* _CALLTYPE GetModuleInfo( void )\
	{\
		if( vari_name == NULL )\
		{\
			vari_name = new tagTModuleInfoRecord();\
			vari_name->MyModuleID = (char*)calloc(1,50);\
			vari_name->MyModuleName = (char*)calloc(1,50);\
			vari_name->MyModuleAuthor = (char*)calloc(1,50);\
			vari_name->MyModuleVer = (char*)calloc(1,50);\
			vari_name->MyModuleTime = (char*)calloc(1,50);\
			vari_name->MyModuleFunc = (char*)calloc(1,50);\
			vari_name->MyModuleBit = (char*)calloc(1,50);\
			vari_name->MyRunPlatform = (char*)calloc(1,50);\
			strcpy(vari_name->MyModuleID,module_id);\
			strcpy(vari_name->MyModuleName,module_name);\
			strcpy(vari_name->MyModuleAuthor,"linys");\
			strcpy(vari_name->MyModuleVer,module_version);\
			strcpy(vari_name->MyModuleTime,module_time);\
			strcpy(vari_name->MyModuleBit,OS_BIT_STRING);\
			strcpy(vari_name->MyRunPlatform,OS_TYPE_STRING);\
			strcpy(vari_name->MyModuleFunc,module_func);\
		}\
		return vari_name;\
	}

typedef NETHANDLE	PLC_HANDLE;

struct tagPPLC_Info
{
	char* PLC_IP;
	int	  PLC_Port;
	char* UserName;
	char* Password;
	Int64 Index;
	Int64 Handle;
};

struct tagPLC_Omron_MemInfo
{
	PLC_HANDLE	_PLC_Handle;
	char*		_PLC_IP;
	int			_PLC_Port;
	int			_PLC_MemType;
	int			_PLC_MemAddr;
	int			_PLC_MemAddModule;
	int			_PLC_MemAddBitNum;
	int			_PLC_MemValueType;
	double		_PLC_MemValue;
	char*		_PLC_MemName;
	char*		_PLC_MemUID;
	int			_PLC_Number;
};


enum enRegType
{
	EN_REG_CIO = 0,
	EN_REG_WR,
	EN_REG_DM,
	EN_REG_HR,
	EN_REG_COUNT
};

enum enValueType
{
	EN_BOOL_TYPE = 0,
	EN_WORD_TYPE,
	EN_DWORD_TYPE,
	EN_FLOAT_TYPE,
	EN_TYPE_COUNT
};

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE DllCommonInit( void );


#endif//__COMMON_20160328140444_1459145084_H__
