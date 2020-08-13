
#ifndef __EXPORTS_V20_20160506114441_1451301281_H__
#define __EXPORTS_V20_20160506114441_1451301281_H__

#include "External.h"

/*****************************************************************
/*ԭ��������	�ͻ������ӻص�����
/*����˵����	pUser: �û�����
				handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_V20_Accept)( void* pUser, const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ������ӶϿ��ص�����
/*����˵����	pUser: �û�����
				handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_V20_ClientClose)( void* pUser, const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ��˽��ջص�����
/*����˵����	pUser: �û�����
				handle: 64bit�ͻ��˾��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_V20_ClientRecData)( void* pUser, NETHANDLE handle,
											const char* c_szData,
											UInt32 u32Size );

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uWorkerThreads: ���ݴ����̸߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Init( UInt8 uWorkerThreads );


/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pCMD_ServerAccept: �ͻ������ӻص�����
				pUser: �û�����
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Listen( UInt16 u16Port,
															pfnStream_S_V20_Accept pfnAccept,
															void* pUser );

/*****************************************************************
/*����˵����	�رշ�����
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Close( void );


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_Free( void );


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_DisConnectClient( NETHANDLE handle );


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_SetClientDisConnect( NETHANDLE handle,
																	pfnStream_S_V20_ClientClose pClientClose,
																	void* pUser );

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_SetClientRecData( NETHANDLE handle,
																	pfnStream_S_V20_ClientRecData pClientRecData,
																	void* pUser );

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_V20_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
