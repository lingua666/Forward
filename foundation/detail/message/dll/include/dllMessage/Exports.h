// BlueToothBLEDll.cpp : ���� DLL Ӧ�ó���ĵ���������
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
/*����˵����	�Ĵ�����д�����ص�����
/*����˵����	pPLC_Omron: PLC������Ϣ�ṹ��
/*����ֵ��		��
*****************************************************************/
typedef void (_CALLTYPE *fpnTimerEvent)( IEvent* pEvent );


/*****************************************************************
/*����˵����	��ʼ��(�ڵ��ú����ӿ�ǰ�����)
/*����˵����	iThreadNum: �����߳�����
/*
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Init( int iThreadNum = 1 );


/*****************************************************************
/*����˵����	�ͷ�ģ����Դ
/*����˵����	
/*			
/*
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Free( void );


/*****************************************************************
/*����˵����	�ͷ�ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	pEvent: �¼�ָ��
/*				uInterval: �������ʱ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_push_back( IEvent* pEvent, unsigned int uInterval/*����*/ );


#endif