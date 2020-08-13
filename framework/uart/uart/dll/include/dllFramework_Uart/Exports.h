
#ifndef __EXPORTS_20161222225641_1451301281_H__
#define __EXPORTS_20161222225641_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Uart_Module)

/*****************************************************************
/*原型声明：	接收回调函数
/*参数说明：	c_pData: 接收缓冲区
				u32Size: 接收长度
/*				
/*返回值：		
*****************************************************************/
typedef void (_CALLTYPE *fpnMUart_RecData)( const char* c_szData,
											UInt32 u32Size );

/*****************************************************************
/*函数说明：	初始化串口
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Init( fpnMUart_RecData fnpRec );


/*****************************************************************
/*函数说明：	释放串口模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Free( void );


/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	StopBits: 停止位
				isParityCheck: 奇偶检验
				isHex: 是否16进制转换

/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Set( int StopBits, bool isParityCheck, bool isHex );

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Com_No: 串口索引（从0开始，即COM0）
/*				Baudrate: 波特率
				ByteSize: 数据位

/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Open( int Com_No, int Baudrate, int ByteSize );

/*****************************************************************
/*函数说明：	关闭串口
/*参数说明：	
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Close( void );


/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		实际发送长度
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_SentData( const char* c_pData,
														UInt32 u32Size );


/*****************************************************************
/*函数说明：	16进制字符串转浮点数
/*参数说明：	c_pData: 16进制字符串
/*				
/*返回值：		转换后的浮点数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MUart_String2Double( const char* c_pData );


/*****************************************************************
/*函数说明：	浮点数转16进制字符串
/*参数说明：	Value: 数据缓冲区
				szBuf: 存储转换后的16进制字符串缓冲区
				BufSize： 缓冲区长度
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Double2String( double Value,
														char* szBuf, int BufSize );

#endif//__EXPORTS_20161222225641_1451301281_H__
