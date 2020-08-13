
#ifndef __EXPORTS_20160308115541_1451301281_H__
#define __EXPORTS_20160308115541_1451301281_H__

#include "FileServer.h"

typedef	void*	THandle;

struct tagTModuleInfoRecord
{
	char* MyModuleID;	//ģ��ID
	char* MyModuleName;	//ģ������
	char* MyModuleAuthor;
	char* MyModuleVer;
	char* MyModuleTime;
	THandle	MyModuleHandle;
	char*	MyModuleBit;
	char*	MyRunPlatform;
};

/*****************************************************************
/*����˵����	ģ���ʼ��
/*����˵����	
/*����ֵ��		�ṹ��ָ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS tagTModuleInfoRecord* _CALLTYPE ModulelIni( THandle Handle );

/*****************************************************************
/*ԭ��������	�ͻ������ӻص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��
*****************************************************************/
typedef void (_CALLTYPE *fpnCMD_ServerAccept)( NETHANDLE handle,
											const char* c_szIP,
											UInt16 uPort );

/*****************************************************************
/*ԭ��������	�ͻ������ӶϿ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *fpnCMD_ClientClose)( NETHANDLE handle,
											const char* c_szIP,
											UInt16 uPort );

/*****************************************************************
/*ԭ��������	�ͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *fpnCMD_ClientRecData)( NETHANDLE handle,
												const char* c_szData,
												UInt32 u32Size );

/*****************************************************************
/*����˵����	�����˿�
/*����˵����	u16Port: �˿ں�
				pCMD_ServerAccept: �ͻ������ӻص�����
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MListen( UInt16 u16Port,
															fpnCMD_ServerAccept pCMD_ServerAccept );

/*****************************************************************
/*����˵����	�رշ�����
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MClose( void );


/*****************************************************************
/*����˵����	�ͷŷ�������Դ(�����ڵ���CMD_Server_MClose��ִ�иýӿ�)
/*����˵����	
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MFree( void );


/*****************************************************************
/*����˵����	�رտͻ�������
/*����˵����	handle: 64bit�ͻ��˾��
/*				
/*����ֵ��		>0:�ɹ�  <1: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MDisConnectClient( NETHANDLE handle );


/*****************************************************************
/*����˵����	���ÿͻ��˶Ͽ��ص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientClose: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MSetClientDisConnect( NETHANDLE handle,
																	fpnCMD_ClientClose pCMD_ClientClose );

/*****************************************************************
/*����˵����	���ÿͻ��˽��ջص�����
/*����˵����	handle: 64bit�ͻ��˾��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MSetClientRecData( NETHANDLE handle,
																	fpnCMD_ClientRecData pCMD_ClientRecData );

/*****************************************************************
/*����˵����	��ͻ��˷�������
/*����˵����	handle: 64bit�ͻ��˾��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		>0:�ɹ�		<=0:ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE CMD_Server_MSentData( NETHANDLE handle,
																const char* c_pData,
																UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
