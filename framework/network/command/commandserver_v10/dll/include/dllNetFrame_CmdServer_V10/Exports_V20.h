
#ifndef __EXPORTS_V20_20160508114441_1451301281_H__
#define __EXPORTS_V20_20160508114441_1451301281_H__

#include "External.h"

DLL_LOG_DECL_DEF(Cmd_S_V20_Module)

/*****************************************************************
/*ԭ��������	�ͻ������ӻص�����
/*����˵����	pUser: �û�����
/*				handle: 64bit�ͻ��˾��
/*				c_szIP: �ͻ���IP��ַ
/*				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_S_V20_Accept)( void* pUser, const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ������ӶϿ��ص�����
/*����˵����	pUser: �û�����
/*				c_szIP: �ͻ���IP��ַ
/*				uPort: �ͻ��˶˿ں�
/*				handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_S_V20_Close)( void* pUser, const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ��˽��ջص�����
/*����˵����	pUser: �û�����
/*				handle: 64bit�ͻ��˾��
/*				c_pData: ���ջ�����
/*				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_S_V20_RecData)( void* pUser, NETHANDLE handle,
											const char* c_szData,
											int u32Size );

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uWorkerThreads: ���ݴ����̸߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Init( int uWorkerThreads = 1 );


/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
/*				pfnAccept: �ͻ������ӻص�����
/*				pUser: �û�����
/*				uIOThreadNum: IO�߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Listen( int u16Port,
															pfnCmd_S_V20_Accept pfnAccept,
															void* pUser, UInt8 uIOThreadNum = 0 );

/*****************************************************************
/*����˵����	�رշ�����
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Close( void );


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���MCmd_S_V20_Close��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_Free( void );


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_DisConnectClient( NETHANDLE handle );


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
/*				pClientClose: �ص�������ַ
/*				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientDisConnect( NETHANDLE handle,
																	pfnCmd_S_V20_Close pClientClose,
																	void* pUser );

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
/*				pClientRecData: �ص�������ַ
/*				pUser: �û�����
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SetClientRecData( NETHANDLE handle,
																	pfnCmd_S_V20_RecData pClientRecData,
																	void* pUser );

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				c_pData: ���ݻ�����
/*				u32Size: ���ݳ���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_S_V20_SentData( NETHANDLE handle,
															const char* c_pData,
															int u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
