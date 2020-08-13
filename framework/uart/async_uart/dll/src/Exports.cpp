
#include <dllFramework_AsyncUarts/Exports.h>
#include <libFramework_AsyncUarts/AsyncUarts.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"系统通信指令模块",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Framework_AsyncUarts_Module)


/*****************************************************************
/*函数说明：	初始化串口
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Init( void )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Init(2);
}


/*****************************************************************
/*函数说明：	释放串口模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Free( void )
{
	Singleton<_async_::_uarts_::AsyncUarts>::instance()->Release();
	return 1;
}



/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Com_No: 串口索引（从0开始，即COM0）
/*				Baudrate: 波特率
				ByteSize: 数据位
				StopBit: 停止位
				isParity: 校验位
/*返回值：		> 0: 句柄		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MAsyncUart_Open( int Com_No, int Baudrate, int ByteSize,
															UInt8 StopBit, bool isParity )
{
	_string_type sCom;
	#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		if( Com_No > 9 )
		{//大于10, 需要增加改变名称
			sCom += "\\\\.\\";
		}
		sCom += "COM" + _string_type::NumberToStr(Com_No);
		sCom += ":";
	#elif defined( PLATFORM_OS_FAMILY_UNIX )
		sCom = "/dev/ttySP";
		sCom += _string_type::NumberToStr(Com_No);
	#endif

	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Open(sCom.c_str(), Baudrate, ByteSize,
																StopBit, isParity);
}


/*****************************************************************
/*函数说明：	设置接收回调函数
/*参数说明：	Handle: 串口句柄
/*				fnpRec: 回调函数地址
				pUser:  用户数据

/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_SetRecData( THandle Handle,
													fpnMAsyncUart_RecData fnpRec,
													void* pUser )
{
	Singleton<_async_::_uarts_::AsyncUarts>::instance()->SetCallBack(Handle,
		function20_bind(fnpRec, pUser, _function_::_1, _function_::_2, _function_::_3));
	return 1;
}



/*****************************************************************
/*函数说明：	关闭串口
/*参数说明：	
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Close( THandle Handle )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Close(Handle);
}


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
															UInt32 u32Size )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Send(Handle, c_pData, u32Size);
}
