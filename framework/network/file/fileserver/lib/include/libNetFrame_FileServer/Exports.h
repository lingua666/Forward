
#ifndef __EXPORTS_20160308115541_1451301281_H__
#define __EXPORTS_20160308115541_1451301281_H__

#include "FileServer.h"

typedef	void*	THandle;

struct tagTModuleInfoRecord
{
	char* MyModuleID;	//模块ID
	char* MyModuleName;	//模块名称
	char* MyModuleAuthor;
	char* MyModuleVer;
	char* MyModuleTime;
	THandle	MyModuleHandle;
	char*	MyModuleBit;
	char*	MyRunPlatform;
};

/*****************************************************************
/*函数说明：	模块初始化
/*参数说明：	
/*返回值：		结构体指针
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS tagTModuleInfoRecord* _CALLTYPE ModulelIni( THandle Handle );

/*****************************************************************
/*原型声明：	客户端连接回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：
*****************************************************************/
typedef void (_CALLTYPE *fpnCMD_ServerAccept)( NETHANDLE handle,
											const char* c_szIP,
											UInt16 uPort );

/*****************************************************************
/*原型声明：	客户端连接断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_szIP: 客户端IP地址
				uPort: 客户端端口号
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *fpnCMD_ClientClose)( NETHANDLE handle,
											const char* c_szIP,
											UInt16 uPort );

/*****************************************************************
/*原型声明：	客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *fpnCMD_ClientRecData)( NETHANDLE handle,
												const char* c_szData,
												UInt32 u32Size );

/*****************************************************************
/*函数说明：	监听端口
/*参数说明：	u16Port: 端口号
				pCMD_ServerAccept: 客户端连接回调函数
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MListen( UInt16 u16Port,
															fpnCMD_ServerAccept pCMD_ServerAccept );

/*****************************************************************
/*函数说明：	关闭服务器
/*参数说明：	
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MClose( void );


/*****************************************************************
/*函数说明：	释放服务器资源(必须在调用CMD_Server_MClose后执行该接口)
/*参数说明：	
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MFree( void );


/*****************************************************************
/*函数说明：	关闭客户端连接
/*参数说明：	handle: 64bit客户端句柄
/*				
/*返回值：		>0:成功  <1: 失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MDisConnectClient( NETHANDLE handle );


/*****************************************************************
/*函数说明：	设置客户端断开回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientClose: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MSetClientDisConnect( NETHANDLE handle,
																	fpnCMD_ClientClose pCMD_ClientClose );

/*****************************************************************
/*函数说明：	设置客户端接收回调函数
/*参数说明：	handle: 64bit客户端句柄
				pCMD_ClientRecData: 回调函数地址
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MSetClientRecData( NETHANDLE handle,
																	fpnCMD_ClientRecData pCMD_ClientRecData );

/*****************************************************************
/*函数说明：	向客户端发送数据
/*参数说明：	handle: 64bit客户端句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		>0:成功		<=0:失败
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MSentData( NETHANDLE handle,
																const char* c_pData,
																UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
