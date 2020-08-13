
#include <dllMessage/Exports.h>
#include <libMessage/TimerEvent.h>

/*****************************************************************
/*函数说明：	初始化(在调用后续接口前请调用)
/*参数说明：	iThreadNum: 工作线程数量
/*
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Init( int iThreadNum )
{
	GetTimerEventInstance()->Init(iThreadNum);
	return 1;
}


/*****************************************************************
/*函数说明：	释放模块资源
/*参数说明：	
/*			
/*
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Free( void )
{
	GetTimerEventInstance()->Release();
	return 1;
}


/*****************************************************************
/*函数说明：	释放模块资源(在不使用该模块时使用)
/*参数说明：	pEvent: 事件指针
/*				uInterval: 触发间隔时间
/*				
/*返回值：		>0: 成功   <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_push_back( IEvent* pEvent, unsigned int uInterval/*毫秒*/ )
{
	if( pEvent == NULL )
		return -1;

	GetTimerEventInstance()->push_back(function20_bind(&IEvent::Invoke, pEvent) , uInterval);
	return 1;
}
