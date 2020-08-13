
#ifndef __EXPORTS_20180719102642_1451301281_H__
#define __EXPORTS_20180719102642_1451301281_H__

#include "External.h"

DLL_LOG_DECL_DEF(Framework_RtspSvr_Module)

/*****************************************************************
/*原型声明：	RTSP会话接入回调函数
/*参数说明：	pUser: 用户参数
/*				c_szSessionUid: 会话UID
/*				c_szsChildUrl: 会话请求的Url
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspSessionAccept)( void* pUser, const char* c_szSessionUid,
											const char* c_szsChildUrl ) ;


/*****************************************************************
/*原型声明：	RTSP会话销毁回调函数
/*参数说明：	pUser: 用户参数
/*				c_szSessionUid: 会话UID
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspSessionDestroy)(void* pUser, const char* c_szSessionUid) ;


/*****************************************************************
/*原型声明：	RTSP会话状态回调函数
/*参数说明：	pUser: 用户参数
/*				c_szSessionUid: 会话UID
/*				iStatus: 状态
						1: 播放
						2: 暂停
						3: 停止
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspSessionOPStatus)(void* pUser, const char* c_szSessionUid, int iStatus) ;


/*****************************************************************
/*函数说明：	初始化
/*参数说明：	c_szUser: 用户名
/*				c_szPassword: 密码
/*				c_szHost: 主机名
/*				c_szPort: 端口号
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Init( const char* c_szUser,
						const char* c_szPassword,
						const char* c_szHost,
						const char* c_szPort );


/*****************************************************************
/*函数说明：	模块释放
/*参数说明：	
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspSvr_Free( void );


/*****************************************************************
/*函数说明：	运行rtsp服务器
/*参数说明：	iPort: 监听端口号
/*				iIOThreadNum: 处理网络事件的线程数	
/*				iProcThreadNum: 处理接收到的网络数据的线程数
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Run(  int iPort, int iIOThreadNum = 0,
	int iProcThreadNum = 0 );


/*****************************************************************
/*函数说明：	停止rtsp服务器
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Stop( void );


/*****************************************************************
/*函数说明：	设置rtsp服务器事件
/*参数说明：	c_szSessionUid:  会话uid
/*				fpAccept: 接收rtsp会话回调函数
/*				pAptUser: 接收会话回调函数携带的用户数据
/*				fpDestroy: 接收会话销毁回调函数
/*				pDesUser: 销毁回调函数所携带的用户数据
/*				fpStatus: rtsp会话状态回调函数
/*				pStaUser: 状态回调函数携带的用户数据
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_SetEvent( const char* c_szSessionUid, 
	fpnMRtspSessionAccept fpAccept, void* pAptUser,
	fpnMRtspSessionDestroy fpDestroy, void* pDesUser,
	fpnMRtspSessionOPStatus fpStatus, void* pStaUser );

/*****************************************************************
/*函数说明：	关闭rtsp会话
/*参数说明：	c_szSessionUid:  会话uid
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Close( const char* c_szSessionUid );


/*****************************************************************
/*函数说明：	增加一个rtsp地址
/*参数说明：	c_szUrl:  rtsp会话连接地址(不包含rtsp主机信息部分)
/*				iVideoType: 视频类型
/*							0: H264
/*							0: H265
/*返回值：		-1: 失败	>0: 句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspSvr_AddMediaStream( const char* c_szUrl, int iVideoType );


/*****************************************************************
/*函数说明：	往rtsp地址填充视频数据
/*参数说明：	Handle:		rtsp地址所对应的句柄（MRtspSvr_AddMediaStream所返回的句柄）
/*				c_szData:	视频数据
/*				iSize:		视频数据长度
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_PutMediaStream( THandle Handle, const char* c_szData, int iSize );


/*****************************************************************
/*函数说明：	移除rtsp地址
/*参数说明：	c_szUrl:	rtsp地址
/*				Handle:		rtsp会话连接地址句柄
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_RemoveMediaStream( const char* c_szUrl, THandle Handle );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
