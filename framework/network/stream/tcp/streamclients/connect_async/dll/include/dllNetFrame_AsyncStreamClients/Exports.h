
#ifndef __EXPORTS_20160108115641_1451301281_H__
#define __EXPORTS_20160108115641_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(Stream_C_Module)

/*****************************************************************
/*ԭ��������	������Ӧ�ص�����
/*����˵����	Handle: �ͻ������ӳɹ����(64bit���Ӿ��)
						-1: ʧ��	>0: ���ֵ

				c_szSvrIP: �ͻ���IP��ַ
				uSvrPort: �ͻ��˶˿ں�
/*				
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *fpnMStream_C_Connect)( const char* c_szSvrIP,
											UInt16 uSvrPort,
											NETHANDLE Handle);

/*****************************************************************
/*ԭ��������	���ӶϿ��ص�����
/*����˵����	Handle: 64bit���Ӿ��
				c_szIP: �ͻ���IP��ַ
				uPort: �ͻ��˶˿ں�
/*				
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *fpnMStream_C_Close)( const char* c_szIP,
											UInt16 uPort,
											NETHANDLE Handle);

/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	Handle: 64bit���Ӿ��
				c_pData: ���ջ�����
				u32Size: ���ճ���
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
typedef void (_CALLTYPE *fpnMStream_C_RecData)( NETHANDLE Handle,
											const char* c_szData,
											UInt32 u32Size );

/*****************************************************************
/*����˵����	��ʼ���ͻ��˼�
/*����˵����	isSequence: �Ƿ�����ģʽ�ύ���յ�������
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Init( bool isSequence = true );


/*****************************************************************
/*����˵����	�ͷſͻ���ģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Free( void );


/*****************************************************************
/*����˵����	����������Ӧ�ص�����
/*����˵����	pCMD_ClientConnect: �ص�������ַ
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SetConnect( fpnMStream_C_Connect fpnConnect );


/*****************************************************************
/*����˵����	���ӷ�����
/*����˵����	c_szIP: ������IP
				u16Port: �������˿ں�
/*				
/*����ֵ��		64bit���Ӿ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Connect( const char* c_szIP,
															UInt16 u16Port );


/*****************************************************************
/*����˵����	�ر�����
/*����˵����	Handle: 64bit���Ӿ��
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_Close( NETHANDLE Handle );


/*****************************************************************
/*����˵����	���öϿ��ص�����
/*����˵����	Handle: 64bit���Ӿ��
				fpnClose: �ص�������ַ
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SetClose( NETHANDLE Handle,
															fpnMStream_C_Close fpnClose );

/*****************************************************************
/*����˵����	���ý��ջص�����
/*����˵����	handle: 64bit���Ӿ��
				pCMD_ClientRecData: �ص�������ַ
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SetRecData( NETHANDLE Handle,
															fpnMStream_C_RecData fpnRecData );

/*****************************************************************
/*����˵����	��������
/*����˵����	Handle: 64bit���Ӿ��
				c_pData: ���ݻ�����
				u32Size: ���ݳ���
/*				
/*����ֵ��		1: �ɹ�		<0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MStream_C_SentData( NETHANDLE Handle,
														const char* c_pData,
														UInt32 u32Size );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
