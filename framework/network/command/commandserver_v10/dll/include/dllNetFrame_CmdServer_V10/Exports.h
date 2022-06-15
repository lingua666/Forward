
#ifndef __EXPORTS_20160508114441_1451301281_H__
#define __EXPORTS_20160508114441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Cmd_SV10_Module)

/*****************************************************************
/*原型声明：	客户端连接回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_SV10_Accept)( const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	客户端连接断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_SV10_Close)( const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_SV10_RecData)( NETHANDLE handle,
											const char* c_szData,
											int u32Size );

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	uWorkerThreads: 数据处理线程个数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Init( int uWorkerThreads = 1 );


/*****************************************************************
/*函数说明：	监听端口
/*参数说明：	u16Port: 端口号
				pCMD_ServerAccept: 客户端连接回调函数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Listen( int u16Port,
															pfnCmd_SV10_Accept pfnAccept );

/*****************************************************************
/*函数说明：	关闭服务器
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Close( void );


/*****************************************************************
/*函数说明：	释放服务器资源(必须在调用CMD_Server_MClose后执行该接口)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Free( void );


/*****************************************************************
/*函数说明：	关闭客户端连接
/*参数说明：	handle: 64bit客户端句柄
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_DisConnectClient( NETHANDLE handle );


/*****************************************************************
/*函数说明：	设置客户端断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientClose: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SetClientDisConnect( NETHANDLE handle,
																	pfnCmd_SV10_Close pClientClose );

/*****************************************************************
/*函数说明：	设置客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SetClientRecData( NETHANDLE handle,
																	pfnCmd_SV10_RecData pClientRecData );

/*****************************************************************
/*函数说明：	向客户端发送数据
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SentData( NETHANDLE handle,
															const char* c_pData,
															int u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
