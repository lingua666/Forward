
#ifndef __EXPORTS_20180119161941_1451301281_H__
#define __EXPORTS_20180119161941_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(NetFrame_UDP_Stream_Module)

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	LocalPort: ���ض˿�
/*				c_szDestIP: �㲥ϵͳ���ip��ַ
/*				DestPort: �㲥ϵͳ����˿ں�
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Init( int LocalPort,
											const char* c_szDestIP, int iDestPort );


/*****************************************************************
/*����˵����	�ͷ���Դ
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Free( void );


/*****************************************************************
/*����˵����	������������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Call( void );


/*****************************************************************
/*����˵����	�Ҷ���������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Hangup( void );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
