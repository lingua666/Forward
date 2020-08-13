
#ifndef __EXPORTS_V20_20160108115641_1451301281_H__
#define __EXPORTS_V20_20160108115641_1451301281_H__

#include "External.h"

/*****************************************************************
/*ԭ��������	������Ӧ�ص�����
/*����˵����	pUser: �û�����
				handle: �ͻ������ӳɹ����(64bit���Ӿ��)
						-1: ʧ��	>0: ���ֵ

				c_szSvrIP: �ͻ���IP��ַ
				uSvrPort: �ͻ��˶˿ں�
/*				
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *fpnMStream_C_V20_Connect)( void* pUser, const char* c_szSvrIP,
											UInt16 uSvrPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	���ӶϿ��ص�����
/*����˵����	pUser: �û�����
				handle: 64bit���Ӿ��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *fpnMStream_C_V20_Close)( void* pUser, const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	pUser: �û�����
				handle: 64bit���Ӿ��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *fpnMStream_C_V20_RecData)( void* pUser, NETHANDLE handle,
											const char* c_szData,
											UInt32 u32Size );

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	isSequence: �Ƿ�����ģʽ�ύ���յ�������
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Init( bool isSequence = true );


/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Free( void );


/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
				pUser: �û�����
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetConnect( fpnMStream_C_V20_Connect fpnConnect );


/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
				pUser: �û�����
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Connect( const char* c_szIP,
															UInt16 u16Port, void* pUser );


/*****************************************************************
/*����˵����	�ر�����
/*����˵����	handle: 64bit���Ӿ��
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_Close( NETHANDLE handle );


/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	handle: 64bit���Ӿ��
				fpnClose: �ص�������ַ
				pUser: �û�����
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetClose( NETHANDLE handle,
															fpnMStream_C_V20_Close fpnClose,
															void* pUser );

/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
				pUser: �û�����
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SetRecData( NETHANDLE handle,
															fpnMStream_C_V20_RecData fpnRecData,
															void* pUser );

/*****************************************************************
/*����˵����	��������
/*����˵����	handle: 64bit���Ӿ��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_V20_SentData( NETHANDLE handle,
														const char* c_pData,
														UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
