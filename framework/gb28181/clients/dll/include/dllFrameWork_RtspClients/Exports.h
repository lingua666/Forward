
#ifndef __EXPORTS_20180619103042_1451301281_H__
#define __EXPORTS_20180619103042_1451301281_H__

#include "External.h"

DLL_LOG_DECL_DEF(Framework_RtspClients_Module)

/*****************************************************************
/*原型声明：	异常通知回调函数
/*参数说明：	pUser: 用户数据
/*				Handle: Rtsp会话句柄
/*				ErrCode: 错误码 
/*						1:无法连接到目标
/*                      2:打开RTP端口失败
/*						3:打开RTCP端口失败
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspClients_Except)( void* pUser, THandle Handle, int ErrCode );


/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	pUser: 用户数据
/*				Handle: Rtsp会话句柄
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspClients_Success)( void* pUser, THandle Handle );


/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	pUser: 用户数据
				Handle: Rtsp会话句柄
				c_szRtpData: 流数据(增加了头部信息)
				u32Size: 流数据长度
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspClients_RtpData)( void* pUser, THandle Handle,
	const char* c_szRtpData, int iSize );


/*****************************************************************
/*函数说明：	初始化
/*参数说明：	uRtspIOThreadNum: Rtsp网络连接处理线程个数
/*				uRtspProcThreadNum: Rtsp数据处理线程个数
/*				uRtpIOThreadNum: Rtp网络连接处理线程个数
/*				uRtpProcThreadNum: Rtp数据处理线程个数
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Init( unsigned char uRtspIOThreadNum = 1,
																unsigned char uRtspProcThreadNum = 1,
																unsigned char uRtpIOThreadNum = 1,
																unsigned char uRtpProcThreadNum = 1 );


/*****************************************************************
/*函数说明：	模块释放
/*参数说明：	
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Free( void );


/*****************************************************************
/*函数说明：	打开Rtsp会话
/*参数说明：	c_szRtspPath: Rtsp路径
/*				iStreamType: 流类型	
/*							0: TCP
/*							1: UDP
/*				iRtpPort: rtp端口号
/*返回值：		会话句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspClients_Open( const char* c_szRtspPath,
	int iStreamType, int iRtpPort );


/*****************************************************************
/*函数说明：	设置Rtsp会话结果回调函数
/*参数说明：	Handle:  rtsp会话句柄
/*				fpExcept: 异常回调函数
/*				pExceptUser: 用户携带的信息
/*				fpSuccess: 成功回调函数
/*				pUser: 用户携带的信息
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetResultEvent( THandle Handle,
	fpnMRtspClients_Except fpExcept, void* pExceptUser,
	fpnMRtspClients_Success fpSuccess, void* pUser );


/*****************************************************************
/*函数说明：	设置数据回调函数
/*参数说明：	Handle:  rtsp会话句柄
/*				fpRtpData: rtp数据回调函数
/*				pUser: 用户携带的信息
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetDataEvent( THandle Handle, 
	fpnMRtspClients_RtpData fpRtpData, void* pUser );

/*****************************************************************
/*函数说明：	播放Rtsp会话
/*参数说明：	Handle:  rtsp会话句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Play( THandle Handle );


/*****************************************************************
/*函数说明：	关闭Rtsp会话
/*参数说明：	Handle:  rtsp会话句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Close( THandle Handle );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
