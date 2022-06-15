
#ifndef __FRAMEWORK_MODBUS_20170621191441_1451301281_H__
#define __FRAMEWORK_MODBUS_20170621191441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(dllFramework_Modbus);


/*****************************************************************
/*函数说明：	初始化modbus tcp模块
/*参数说明：
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Tcp(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1, bool isSequence = true);


/*****************************************************************
/*函数说明：	初始化modbus tcp模块
/*参数说明：
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Rtu(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1);


/*****************************************************************
/*函数说明：	释放modbus模块资源(在不使用该模块时使用)
/*参数说明：
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Free(void);


#endif//__PLC_OMRON_20151228191441_1451301281_H__
