
#ifndef __EXPORTS_20160108115641_1451301281_H__
#define __EXPORTS_20160108115641_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

/*****************************************************************
/*原型声明：	连接响应回调函数
/*参数说明：	handle: 客户端连接成功句柄(64bit连接句柄)
						-1: 失败	>0: 句柄值

				c_szSvrIP: 客户端IP地址
				uSvrPort: 客户端端口号
/*				
/*返回值：	
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_SynC_Connect)( NETHANDLE handle,
											const char* c_szSvrIP,
											UInt16 uSvrPort );

/*****************************************************************
/*原型声明：	连接断开回调函数
/*参数说明：	handle: 64bit连接句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：	
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_SynC_Close)(const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	handle: 64bit连接句柄
				c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_SynC_RecData)( NETHANDLE handle,
												const char* c_szData,
												UInt32 u32Size );

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	CMD_Clients_Init
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Init( void );


/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Free( void );


/*****************************************************************
/*函数说明：	连接服务器
/*参数说明：	c_szIP: 服务器IP
				u16Port: 服务器端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_SynC_Connect( const char* c_szIP,
																	UInt16 u16Port );

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	handle: 64bit连接句柄
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Close( NETHANDLE handle );


/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetClose( NETHANDLE handle,
															fpnMCmd_SynC_Close fpnClose );

/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	handle: 64bit连接句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetRecData( NETHANDLE handle,
																fpnMCmd_SynC_RecData fpnRecData );

/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	handle: 64bit连接句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
