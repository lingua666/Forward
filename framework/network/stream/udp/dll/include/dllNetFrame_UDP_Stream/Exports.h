
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


/*****************************************************************
/*ԭ��������	���ӶϿ��ص�����
/*����˵����	pUser: �û�����
Handle: 64bit���Ӿ��
c_szIP: �ͻ���IP��ַ
uPort: �ͻ��˶˿ں�
/*
/*����ֵ��
*****************************************************************/
typedef void (_CALLTYPE *fpnMUdpStream_Close)(void* pUser, UInt16 uPort, NETHANDLE Handle);


/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	pUser: �û�����
Handle: 64bit���Ӿ��
c_pData: ���ջ�����
u32Size: ���ճ���
/*
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *fpnMUdpStream_RecData)(void* pUser, NETHANDLE Node,
	struct sockaddr_in* pSrc, const char* c_szData, UInt32 u32Size);

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	uIOThreadNum�� IO�߳���
/*			uProcThreadNum: ���ݴ����߳�
/*			isSequence:		�Ƿ�˳���ύ
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Init(UInt8 uIOThreadNum = 0,
	UInt8 uProcThreadNum = 0, bool isSequence = false);


/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Free(void);


/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Handle: 64bit���Ӿ��
/*			uPort: ���ض˿�
/*          c_szIP: ����IP��ַ�� NULL��ѡ��Ĭ��
/*
/*����ֵ��		Handle > 0: �ɹ�		<=0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS NETHANDLE _CALLTYPE MUdpStream_Open(UInt16 uPort, const char* c_szIP = NULL);


/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Handle: 64bit���Ӿ��
/*
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_Close(NETHANDLE Handle);


/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	Handle: 64bit���Ӿ��
/*			fpnClose: �ص�������ַ
/*			pUser: �û�����
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetClose(NETHANDLE Handle,
	fpnMUdpStream_Close fpnClose, void* pUser);

/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	Handle: 64bit���Ӿ��
/* pCMD_ClientRecData: �ص�������ַ
/* pUser: �û�����
/* ����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SetRecData(NETHANDLE Handle,
	fpnMUdpStream_RecData fpnRecData, void* pUser);

/*****************************************************************
/*����˵����	��������
/*����˵����	Handle: 64bit���Ӿ��
/*			c_pData: ���ݻ�����
/*			u32Size: ���ݳ���
/*          pAddr: Ŀ���ַ
/*
/*����ֵ��	1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData(NETHANDLE Handle,
	struct sockaddr_in* pAddr, const char* c_pData, UInt32 u32Size);


/*****************************************************************
/*����˵����	��������
/*����˵����	Handle: 64bit���Ӿ��
/*			c_pData: ���ݻ�����
/*			u32Size: ���ݳ���
/*			c_szIP: Ŀ��IP
/*			uPort: Ŀ��˿�
/*			
/*����ֵ��	1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MUdpStream_SentData2(NETHANDLE Handle,
	const char* c_szIP, UInt16 uPort, const char* c_pData, UInt32 u32Size);


#endif//__PLC_OMRON_20151228191441_1451301281_H__
