
#include <dllFramework_AsyncUarts/Exports.h>
#include <libFramework_AsyncUarts/AsyncUarts.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
			"45000001000V2016021700000000VZZZ10000000",
			"ϵͳͨ��ָ��ģ��",
			"V1.0.0.1",
			"20160217",
			"")

DLL_LOG_DEF(Framework_AsyncUarts_Module)


/*****************************************************************
/*����˵����	��ʼ������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Init( void )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Init(2);
}


/*****************************************************************
/*����˵����	�ͷŴ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Free( void )
{
	Singleton<_async_::_uarts_::AsyncUarts>::instance()->Release();
	return 1;
}



/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Com_No: ������������0��ʼ����COM0��
/*				Baudrate: ������
				ByteSize: ����λ
				StopBit: ֹͣλ
				isParity: У��λ
/*����ֵ��		> 0: ���		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MAsyncUart_Open( int Com_No, int Baudrate, int ByteSize,
															UInt8 StopBit, bool isParity )
{
	_string_type sCom;
	#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		if( Com_No > 9 )
		{//����10, ��Ҫ���Ӹı�����
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
/*����˵����	���ý��ջص�����
/*����˵����	Handle: ���ھ��
/*				fnpRec: �ص�������ַ
				pUser:  �û�����

/*����ֵ��		1: �ɹ�		<0:������
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
/*����˵����	�رմ���
/*����˵����	
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Close( THandle Handle )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Close(Handle);
}


/*****************************************************************
/*����˵����	��������
/*����˵����	Handle: ���ھ��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		ʵ�ʷ��ͳ���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_SentData( THandle Handle,
															const char* c_pData,
															UInt32 u32Size )
{
	return Singleton<_async_::_uarts_::AsyncUarts>::instance()->Send(Handle, c_pData, u32Size);
}
