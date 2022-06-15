
#ifndef __EXPORTS_20171108112341_1451301281_H__
#define __EXPORTS_20171108112341_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(FrameWork_AsyncFiles_Module)

/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	pUser: 用户数据
				Handle: 串口句柄
				c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMAsyncUart_RecData)( void* pUser, THandle Handle,
											const char* c_szData, UInt32 u32Size );


/*****************************************************************
/*函数说明：	初始化串口
/*参数说明：	
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Init( void );


/*****************************************************************
/*函数说明：	释放串口模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Free( void );



/*****************************************************************
/*函数说明：	通过索引获取COM名称(该接口只对windows下有效)
/*参数说明：	uComNO: COM索引
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_GetWinComName(UInt8 uComNO, std::string& sOut);


/*****************************************************************
/*函数说明：	打开串口
/*参数说明：	c_szComName: 串口名称
/*			Baudrate: 波特率
			ByteSize: 数据位
			StopBit: 停止位
			isParity: 校验位
/*返回值：		> 0: 句柄		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MAsyncUart_Open( const char* c_szComName, int Baudrate, int ByteSize = 8,
															UInt8 StopBit = 1, bool isParity = false );


/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	Handle: 串口句柄
/*				fnpRec: 回调函数地址
				pUser:  用户数据

/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_SetRecData( THandle Handle,
													fpnMAsyncUart_RecData fnpRec,
													void* pUser );



/*****************************************************************
/*函数说明：	关闭串口
/*参数说明：	
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Close( THandle Handle );


/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	Handle: 串口句柄
				c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		实际发送长度
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_SentData( THandle Handle,
															const char* c_pData,
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
