
#ifndef __EXPORTS_V20_20160108115641_1451301281_H__
#define __EXPORTS_V20_20160108115641_1451301281_H__

#include "External.h"

/*****************************************************************
/*原型声明：	连接响应回调函数
/*参数说明：	pUser: 用户数据
				handle: 客户端连接成功句柄(64bit连接句柄)
						-1: 失败	>0: 句柄值

				c_szSvrIP: 客户端IP地址
				uSvrPort: 客户端端口号
/*				
/*返回值：	
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_C_V20_Connect)( void* pUser, const char* c_szSvrIP,
											int uSvrPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	连接断开回调函数
/*参数说明：	pUser: 用户数据
				handle: 64bit连接句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：	
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_C_V20_Close)( void* pUser, const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	pUser: 用户数据
				handle: 64bit连接句柄
				c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_C_V20_RecData)( void* pUser, NETHANDLE handle,
											const char* c_szData,
											int u32Size );

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	iIOThreadNum : IO操作线程数量, 无特殊情况，请默认
/*				iProcThreadNum : 数据处理线程数量, 无特殊情况，请默认
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Init( int iIOThreadNum = 0, int iProcThreadNum = 0  );


/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Free( void );


/*****************************************************************
/*函数说明：	设置连接响应回调函数
/*参数说明：	pCMD_ClientConnect: 回调函数地址
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetConnect( fpnMCmd_C_V20_Connect fpnConnect );


/*****************************************************************
/*函数说明：	连接服务器
/*参数说明：	c_szIP: 服务器IP
				u16Port: 服务器端口号
				pUser: 连接用户数据
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_C_V20_Connect( const char* c_szIP,
																int u16Port, void* pUser );


/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	handle: 64bit连接句柄
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_Close( NETHANDLE handle );


/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	handle: 64bit连接句柄
				fpnClose: 回调函数地址
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetClose( NETHANDLE handle,
															fpnMCmd_C_V20_Close fpnClose,
															void* pUser);

/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
				pUse: 用户参数
/*
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SetRecData( NETHANDLE handle,
															fpnMCmd_C_V20_RecData fpnRecData,
															void* pUser );

/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	handle: 64bit连接句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_C_V20_SentData( NETHANDLE handle,
														const char* c_pData,
														int u32Size );


/*****************************************************************
/*函数说明：	地址错误发生
/*参数说明：	szData: 错误缓冲区
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MCmd_C_V20_Error( const char* szData );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
