
#ifndef __EXPORTS_20180119161941_1451301281_H__
#define __EXPORTS_20180119161941_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(NetFrame_UDP_Stream_Module)

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	LocalPort: 本地端口
/*				c_szDestIP: 广播系统软件ip地址
/*				DestPort: 广播系统软件端口号
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Init( int LocalPort,
											const char* c_szDestIP, int iDestPort );


/*****************************************************************
/*函数说明：	释放资源
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Free( void );


/*****************************************************************
/*函数说明：	呼叫所有喇叭
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Call( void );


/*****************************************************************
/*函数说明：	挂断所有喇叭
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Hangup( void );


/*****************************************************************
/*原型声明：	连接断开回调函数
/*参数说明：	pUser: 用户参数
Handle: 64bit连接句柄
c_szIP: 客户端IP地址
uPort: 客户端端口号
/*
/*返回值：
*****************************************************************/
typedef void (_CALLTYPE *fpnMUdpStream_Close)(void* pUser, UInt16 uPort, NETHANDLE Handle);


/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	pUser: 用户参数
Handle: 64bit连接句柄
c_pData: 接收缓冲区
u32Size: 接收长度
/*
/*返回值：		64bit连接句柄
*****************************************************************/
typedef void (_CALLTYPE *fpnMUdpStream_RecData)(void* pUser, NETHANDLE Node,
	struct sockaddr_in* pSrc, const char* c_szData, UInt32 u32Size);

/*****************************************************************
/*函数说明：	初始化客户端集
/*参数说明：	uIOThreadNum： IO线程数
/*			uProcThreadNum: 数据处理线程
/*			isSequence:		是否顺序提交
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Init(UInt8 uIOThreadNum = 0,
	UInt8 uProcThreadNum = 0, bool isSequence = false);


/*****************************************************************
/*函数说明：	释放客户端模块资源(在不使用该模块时使用)
/*参数说明：
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Free(void);


/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Handle: 64bit连接句柄
/*			uPort: 本地端口
/*          c_szIP: 本地IP地址， NULL则选择默认
/*
/*返回值：		Handle > 0: 成功		<=0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MUdpStream_Open(UInt16 uPort, const char* c_szIP = NULL);


/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Handle: 64bit连接句柄
/*
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Close(NETHANDLE Handle);


/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	Handle: 64bit连接句柄
/*			fpnClose: 回调函数地址
/*			pUser: 用户参数
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetClose(NETHANDLE Handle,
	fpnMUdpStream_Close fpnClose, void* pUser);

/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	Handle: 64bit连接句柄
/* pCMD_ClientRecData: 回调函数地址
/* pUser: 用户参数
/* 返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetRecData(NETHANDLE Handle,
	fpnMUdpStream_RecData fpnRecData, void* pUser);

/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	Handle: 64bit连接句柄
/*			c_pData: 数据缓冲区
/*			u32Size: 数据长度
/*          pAddr: 目标地址
/*
/*返回值：	1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData(NETHANDLE Handle,
	struct sockaddr_in* pAddr, const char* c_pData, UInt32 u32Size);


/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	Handle: 64bit连接句柄
/*			c_pData: 数据缓冲区
/*			u32Size: 数据长度
/*			c_szIP: 目标IP
/*			uPort: 目标端口
/*			
/*返回值：	1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData2(NETHANDLE Handle,
	const char* c_szIP, UInt16 uPort, const char* c_pData, UInt32 u32Size);


#endif//__PLC_OMRON_20151228191441_1451301281_H__
