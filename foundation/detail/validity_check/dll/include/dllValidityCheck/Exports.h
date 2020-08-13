
#ifndef __EXPORTS_20161110182842_1451301281_H__
#define __EXPORTS_20161110182842_1451301281_H__

#include "External.h"

/*****************************************************************
/*原型声明：	非法文件回调函数
/*参数说明：	c_szFilePath: 非法文件路径
/*				iValue: 错误码
/*				
/*返回值：		true:底层错误处理		false:用户自行处理
*****************************************************************/
typedef bool (_CALLTYPE *fpnMValidity_CheckFaild)( const char* c_szFilePath,
												int iValue );


/*****************************************************************
/*函数说明：	文件合法性验证
/*参数说明：	fpnFaild: 非法文件回调函数
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MValidity_Check( fpnMValidity_CheckFaild fpnFaild );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
