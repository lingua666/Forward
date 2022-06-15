
#include <dllFramework_Uart/Exports.h>
#include <libFramework_Uart/Uart.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"系统通信指令模块",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Uart_Module)

fpnMUart_RecData g_fnpUartRec = NULL;

void Uart_RecData_Processing()
{
	while(true)
	{
		try
		{
			ThreadWrap::interruption_point();
			_string_type s = Singleton<_uart_::CUart>::instance()->Receive();
			if( s.size() > 0 )
			{
				if( g_fnpUartRec != NULL )
				{
					g_fnpUartRec(s.c_str(), s.size());
				}
			}
		}
		catch (const _thread_::thread_interrupted& e)
		{
			switch( e.get_code() )
			{
			case ThreadData::en_INTERRUPTED_EXIT:	//线程退出
				return ;
				break;
			}
			break;
		}
	}
}

_thread_::ThreadWrap	g_hUartRec( function20_bind_Ex(Uart_RecData_Processing) );

/*****************************************************************
/*函数说明：	初始化串口
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Init( fpnMUart_RecData fnpRec )
{
	g_fnpUartRec = fnpRec;
	return 1;
}


/*****************************************************************
/*函数说明：	释放串口模块资源(在不使用该模块时使用)
/*参数说明：	
/*				
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Free( void )
{
	return 1;
}


/*****************************************************************
/*函数说明：	设置断开回调函数
/*参数说明：	StopBits: 停止位
				isParityCheck: 奇偶检验
				isHex: 是否16进制转换

/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Set( int StopBits, bool isParityCheck, bool isHex = false )
{
	return Singleton<_uart_::CUart>::instance()->SetConfig(StopBits, isParityCheck, isHex) ? 1 : -1;
}

/*****************************************************************
/*函数说明：	关闭连接
/*参数说明：	Com_No: 串口索引（从0开始，即COM0）
/*				Baudrate: 波特率
				ByteSize: 

/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Open( int Com_No, int Baudrate, int ByteSize )
{
	return Singleton<_uart_::CUart>::instance()->Open(Com_No, Baudrate, ByteSize) ? 1 : -1;
}

/*****************************************************************
/*函数说明：	关闭串口
/*参数说明：	
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Close( void )
{
	Singleton<_uart_::CUart>::instance()->Close();
	return 1;
}


/*****************************************************************
/*函数说明：	发送数据
/*参数说明：	c_pData: 数据缓冲区
				u32Size: 数据长度
/*				
/*返回值：		实际发送长度
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_SentData( const char* c_pData,
														UInt32 u32Size )
{
	return Singleton<_uart_::CUart>::instance()->Send(c_pData, u32Size);
}

/*****************************************************************
/*函数说明：	16进制字符串转浮点数
/*参数说明：	c_pData: 16进制字符串
/*				
/*返回值：		转换后的浮点数
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MUart_String2Double( const char* c_pData )
{
	_string_type s = _string_type::StrToHex(c_pData);
	UInt16 u1 = Bit_BigEndian_Buf2UInt16((UInt8*)&s[0]);
	UInt16 u2 = Bit_BigEndian_Buf2UInt16((UInt8*)&s[2]);

	UInt32 Value = u2 << 16;
	Value |= u1;
	float f1 = *((float*)&Value);

	return (double)f1;
}

/*****************************************************************
/*函数说明：	浮点数转16进制字符串
/*参数说明：	Value: 数据缓冲区
				szBuf: 存储转换后的16进制字符串缓冲区
				BufSize： 缓冲区长度
/*				
/*返回值：		1: 成功		<0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Double2String( double Value,
														char* szBuf, int BufSize )
{
	float f1 = (float)Value;
	UInt32 u32 = *((UInt32*)&f1);

	UInt16 u1 = (UInt16)u32;
	UInt16 u2 = u32 >> 16;

	_string_type sHex = BigEndian8_UInt16(u1);
	sHex += BigEndian8_UInt16(u2);

	_string_type s = _string_type::HexToStr(sHex);
	if( s.size() > BufSize )
		return -1;

	memcpy(szBuf, s.c_str(), s.size());
	szBuf[s.size()] = 0;
	return 1;
}
