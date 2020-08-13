
#ifndef __EXPORTS_20160506114441_1451301281_H__
#define __EXPORTS_20160506114441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Stream_S_Module)

/*****************************************************************
/*原型声明：	客户端连接回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_Accept)( const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	客户端连接断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_ClientClose)( const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*原型声明：	客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_ClientRecData)( NETHANDLE handle,
											const char* c_szData,
											UInt32 u32Size );

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	uWorkerThreads: 数据处理线程个数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Init( UInt8 uWorkerThreads );


/*****************************************************************
/*函数说明：	监听端口
/*参数说明：	u16Port: 端口号
				pCMD_ServerAccept: 客户端连接回调函数
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Listen( UInt16 u16Port,
															pfnStream_S_Accept pfnAccept );

/*****************************************************************
/*函数说明：	关闭服务器
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Close( void );


/*****************************************************************
/*函数说明：	释放服务器资源(必须在调用CMD_Server_MClose后执行该接口)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Free( void );


/*****************************************************************
/*函数说明：	关闭客户端连接
/*参数说明：	handle: 64bit客户端句柄
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_DisConnectClient( NETHANDLE handle );


/*****************************************************************
/*函数说明：	设置客户端断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientClose: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SetClientDisConnect( NETHANDLE handle,
																	pfnStream_S_ClientClose pClientClose );

/*****************************************************************
/*函数说明：	设置客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SetClientRecData( NETHANDLE handle,
																	pfnStream_S_ClientRecData pClientRecData );

/*****************************************************************
/*函数说明：	向客户端发送数据
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
