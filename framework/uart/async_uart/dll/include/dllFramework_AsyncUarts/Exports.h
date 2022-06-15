
#ifndef __EXPORTS_20171108112341_1451301281_H__
#define __EXPORTS_20171108112341_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(FrameWork_AsyncFiles_Module)

/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	pUser: �û�����
				Handle: ���ھ��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMAsyncUart_RecData)( void* pUser, THandle Handle,
											const char* c_szData, UInt32 u32Size );


/*****************************************************************
/*����˵����	��ʼ������
/*����˵����	
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Init( void );


/*****************************************************************
/*����˵����	�ͷŴ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Free( void );



/*****************************************************************
/*����˵����	ͨ��������ȡCOM����(�ýӿ�ֻ��windows����Ч)
/*����˵����	uComNO: COM����
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_GetWinComName(UInt8 uComNO, std::string& sOut);


/*****************************************************************
/*����˵����	�򿪴���
/*����˵����	c_szComName: ��������
/*			Baudrate: ������
			ByteSize: ����λ
			StopBit: ֹͣλ
			isParity: У��λ
/*����ֵ��		> 0: ���		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MAsyncUart_Open( const char* c_szComName, int Baudrate, int ByteSize = 8,
															UInt8 StopBit = 1, bool isParity = false );


/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	Handle: ���ھ��
/*				fnpRec: �ص�������ַ
				pUser:  �û�����

/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_SetRecData( THandle Handle,
													fpnMAsyncUart_RecData fnpRec,
													void* pUser );



/*****************************************************************
/*����˵����	�رմ���
/*����˵����	
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MAsyncUart_Close( THandle Handle );


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
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
