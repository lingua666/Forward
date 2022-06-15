
#ifndef __EXPORTS_20160508114441_1451301281_H__
#define __EXPORTS_20160508114441_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Cmd_SV10_Module)

/*****************************************************************
/*ԭ��������	�ͻ������ӻص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_SV10_Accept)( const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ������ӶϿ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_SV10_Close)( const char* c_szIP,
											int uPort,
											NETHANDLE handle );

/*****************************************************************
/*ԭ��������	�ͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *pfnCmd_SV10_RecData)( NETHANDLE handle,
											const char* c_szData,
											int u32Size );

/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uWorkerThreads: ���ݴ����̸߳���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Init( int uWorkerThreads = 1 );


/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pCMD_ServerAccept: �ͻ������ӻص�����
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Listen( int u16Port,
															pfnCmd_SV10_Accept pfnAccept );

/*****************************************************************
/*����˵����	�رշ�����
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Close( void );


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_Free( void );


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_DisConnectClient( NETHANDLE handle );


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SetClientDisConnect( NETHANDLE handle,
																	pfnCmd_SV10_Close pClientClose );

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SetClientRecData( NETHANDLE handle,
																	pfnCmd_SV10_RecData pClientRecData );

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCmd_SV10_SentData( NETHANDLE handle,
															const char* c_pData,
															int u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
