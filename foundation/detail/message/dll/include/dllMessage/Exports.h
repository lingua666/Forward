// BlueToothBLEDll.cpp : 定义 DLL 应用程序的导出函数。
//

#ifndef __EXPORTS_20180609195304_1451301281_H__
#define __EXPORTS_20180609195304_1451301281_H__

#include "External.h"

class IEvent
{
public:
	virtual ~IEvent( void )
	{

	}

	virtual void Invoke( void ) = 0;

	void operator()( void )
	{
		Invoke();
	}
};


/*****************************************************************
/*函数说明：	寄存器读写操作回调函数
/*参数说明：	pPLC_Omron: PLC操作信息结构体
/*返回值：		无
*****************************************************************/
typedef void (_CALLTYPE *fpnTimerEvent)( IEvent* pEvent );


/*****************************************************************
/*函数说明：	初始化(在调用后续接口前请调用)
/*参数说明：	iThreadNum: 工作线程数量
/*
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Init( int iThreadNum = 1 );


/*****************************************************************
/*函数说明：	释放模块资源
/*参数说明：	
/*			
/*
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Free( void );


/*****************************************************************
/*函数说明：	释放模块资源(在不使用该模块时使用)
/*参数说明：	pEvent: 事件指针
/*				uInterval: 触发间隔时间
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_push_back( IEvent* pEvent, unsigned int uInterval/*毫秒*/ );


#endif