
#include <dllFrameWork_AsyncUarts/Exports.h>
#include <libFrameWork_AsyncUarts/AsyncUarts.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"系统通信指令模块",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(FrameWork_AsyncUarts_Module)

char g_ComName202108161607[100] = {0};


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
/*函数说明：	通过索引获取COM名称(该接口只对windows下有效)
/*参数说明：	uComNO: COM索引
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_GetWinComName(UInt8 uComNO, std::string& sOut)
{
	_string_type	sCom;
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	if (uComNO > 9)
	{//大于10, 需要增加改变名称
		sCom += "\\\\.\\";
	}
	sCom += "COM";
	sCom += _string_type::NumberToStr(uComNO).c_str();
	sCom += ":";
#endif
	sOut = sCom.c_str();
	return 1;
}


/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	c_szComName: 串口名称
/*			Baudrate: 波特率
			ByteSize: 数据位
			StopBit: 停止位
			isParity: 校验位
/*返回值：		> 0: 句柄		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MAsyncUart_Open(const char* c_szComName, int Baudrate, int ByteSize,
															UInt8 StopBit, bool isParity )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Open(c_szComName, Baudrate, ByteSize,
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
	//test
	printf("SendFun(%d):%s\r\n", u32Size, _string_type::HexToStr(c_pData, u32Size).c_str());
	//test

	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Send(Handle, c_pData, u32Size);
}
