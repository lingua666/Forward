
#ifndef __EXPORTS_20160108115641_1451301281_H__
#define __EXPORTS_20160108115641_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

/*****************************************************************
/*ԭ��������	������Ӧ�ص�����
/*����˵����	handle: �ͻ������ӳɹ����(64bit���Ӿ��)
						-1: ʧ��	>0: ���ֵ

				c_szSvrIP: �ͻ���IP��ַ
				uSvrPort: �ͻ��˶˿ں�
/*				
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_SynC_Connect)( NETHANDLE handle,
											const char* c_szSvrIP,
											UInt16 uSvrPort );

/*****************************************************************
/*ԭ��������	���ӶϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_SynC_Close)(const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	handle: 64bit���Ӿ��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *fpnMCmd_SynC_RecData)( NETHANDLE handle,
												const char* c_szData,
												UInt32 u32Size );

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	CMD_Clients_Init
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Init( void );


/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Free( void );


/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MCmd_SynC_Connect( const char* c_szIP,
																	UInt16 u16Port );

/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_Close( NETHANDLE handle );


/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetClose( NETHANDLE handle,
															fpnMCmd_SynC_Close fpnClose );

/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SetRecData( NETHANDLE handle,
																fpnMCmd_SynC_RecData fpnRecData );

/*****************************************************************
/*����˵����	��������
/*����˵����	handle: 64bit���Ӿ��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SynC_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
