
#include <dllMessage/Exports.h>
#include <libMessage/TimerEvent.h>

/*****************************************************************
/*����˵����	��ʼ��(�ڵ��ú����ӿ�ǰ�����)
/*����˵����	iThreadNum: �����߳�����
/*
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Init( int iThreadNum )
{
	GetTimerEventInstance()->Init(iThreadNum);
	return 1;
}


/*****************************************************************
/*����˵����	�ͷ�ģ����Դ
/*����˵����	
/*			
/*
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_Free( void )
{
	GetTimerEventInstance()->Release();
	return 1;
}


/*****************************************************************
/*����˵����	�ͷ�ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	pEvent: �¼�ָ��
/*				uInterval: �������ʱ��
/*				
/*����ֵ��		>0: �ɹ�   <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MMessage_TimerEvent_push_back( IEvent* pEvent, unsigned int uInterval/*����*/ )
{
	if( pEvent == NULL )
		return -1;

	GetTimerEventInstance()->push_back(function20_bind(&IEvent::Invoke, pEvent) , uInterval);
	return 1;
}
