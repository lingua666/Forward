
#ifndef __EXPORTS_20160506114441_1451301281_H__
#define __EXPORTS_20160506114441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Stream_S_Module)

/*****************************************************************
/*ԭ��������	�ͻ������ӻص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_Accept)( const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ������ӶϿ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_ClientClose)( const char* c_szIP,
											UInt16 uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnStream_S_ClientRecData)( NETHANDLE handle,
											const char* c_szData,
											UInt32 u32Size );

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uWorkerThreads: ���ݴ����̸߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Init( UInt8 uWorkerThreads );


/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pCMD_ServerAccept: �ͻ������ӻص�����
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Listen( UInt16 u16Port,
															pfnStream_S_Accept pfnAccept );

/*****************************************************************
/*����˵����	�رշ�����
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Close( void );


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_Free( void );


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_DisConnectClient( NETHANDLE handle );


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SetClientDisConnect( NETHANDLE handle,
																	pfnStream_S_ClientClose pClientClose );

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SetClientRecData( NETHANDLE handle,
																	pfnStream_S_ClientRecData pClientRecData );

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_S_SentData( NETHANDLE handle,
															const char* c_pData,
															UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
