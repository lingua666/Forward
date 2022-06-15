
#include <dllFramework_Uart/Exports.h>
#include <libFramework_Uart/Uart.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"ϵͳͨ��ָ��ģ��",
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
			case ThreadData::en_INTERRUPTED_EXIT:	//�߳��˳�
				return ;
				break;
			}
			break;
		}
	}
}

_thread_::ThreadWrap	g_hUartRec( function20_bind_Ex(Uart_RecData_Processing) );

/*****************************************************************
/*����˵����	��ʼ������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Init( fpnMUart_RecData fnpRec )
{
	g_fnpUartRec = fnpRec;
	return 1;
}


/*****************************************************************
/*����˵����	�ͷŴ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Free( void )
{
	return 1;
}


/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	StopBits: ֹͣλ
				isParityCheck: ��ż����
				isHex: �Ƿ�16����ת��

/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Set( int StopBits, bool isParityCheck, bool isHex = false )
{
	return Singleton<_uart_::CUart>::instance()->SetConfig(StopBits, isParityCheck, isHex) ? 1 : -1;
}

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Com_No: ������������0��ʼ����COM0��
/*				Baudrate: ������
				ByteSize: 

/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Open( int Com_No, int Baudrate, int ByteSize )
{
	return Singleton<_uart_::CUart>::instance()->Open(Com_No, Baudrate, ByteSize) ? 1 : -1;
}

/*****************************************************************
/*����˵����	�رմ���
/*����˵����	
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_Close( void )
{
	Singleton<_uart_::CUart>::instance()->Close();
	return 1;
}


/*****************************************************************
/*����˵����	��������
/*����˵����	c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		ʵ�ʷ��ͳ���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUart_SentData( const char* c_pData,
														UInt32 u32Size )
{
	return Singleton<_uart_::CUart>::instance()->Send(c_pData, u32Size);
}

/*****************************************************************
/*����˵����	16�����ַ���ת������
/*����˵����	c_pData: 16�����ַ���
/*				
/*����ֵ��		ת����ĸ�����
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
/*����˵����	������ת16�����ַ���
/*����˵����	Value: ���ݻ�����
				szBuf: �洢ת�����16�����ַ���������
				BufSize�� ����������
/*				
/*����ֵ��		1: �ɹ�		<0:������
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
