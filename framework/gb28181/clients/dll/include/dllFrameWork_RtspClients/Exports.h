
#ifndef __EXPORTS_20180619103042_1451301281_H__
#define __EXPORTS_20180619103042_1451301281_H__

#include "External.h"

DLL_LOG_DECL_DEF(Framework_RtspClients_Module)

/*****************************************************************
/*ԭ��������	�쳣֪ͨ�ص�����
/*����˵����	pUser: �û�����
/*				Handle: Rtsp�Ự���
/*				ErrCode: ������ 
/*						1:�޷����ӵ�Ŀ��
/*                      2:��RTP�˿�ʧ��
/*						3:��RTCP�˿�ʧ��
/*				
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspClients_Except)( void* pUser, THandle Handle, int ErrCode );


/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	pUser: �û�����
/*				Handle: Rtsp�Ự���
/*				
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspClients_Success)( void* pUser, THandle Handle );


/*****************************************************************
/*ԭ��������	���ջص�����
/*����˵����	pUser: �û�����
				Handle: Rtsp�Ự���
				c_szRtpData: ������(������ͷ����Ϣ)
				u32Size: �����ݳ���
/*				
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspClients_RtpData)( void* pUser, THandle Handle,
	const char* c_szRtpData, int iSize );


/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	uRtspIOThreadNum: Rtsp�������Ӵ����̸߳���
/*				uRtspProcThreadNum: Rtsp���ݴ����̸߳���
/*				uRtpIOThreadNum: Rtp�������Ӵ����̸߳���
/*				uRtpProcThreadNum: Rtp���ݴ����̸߳���
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Init( unsigned char uRtspIOThreadNum = 1,
																unsigned char uRtspProcThreadNum = 1,
																unsigned char uRtpIOThreadNum = 1,
																unsigned char uRtpProcThreadNum = 1 );


/*****************************************************************
/*����˵����	ģ���ͷ�
/*����˵����	
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Free( void );


/*****************************************************************
/*����˵����	��Rtsp�Ự
/*����˵����	c_szRtspPath: Rtsp·��
/*				iStreamType: ������	
/*							0: TCP
/*							1: UDP
/*				iRtpPort: rtp�˿ں�
/*����ֵ��		�Ự���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspClients_Open( const char* c_szRtspPath,
	int iStreamType, int iRtpPort );


/*****************************************************************
/*����˵����	����Rtsp�Ự����ص�����
/*����˵����	Handle:  rtsp�Ự���
/*				fpExcept: �쳣�ص�����
/*				pExceptUser: �û�Я������Ϣ
/*				fpSuccess: �ɹ��ص�����
/*				pUser: �û�Я������Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetResultEvent( THandle Handle,
	fpnMRtspClients_Except fpExcept, void* pExceptUser,
	fpnMRtspClients_Success fpSuccess, void* pUser );


/*****************************************************************
/*����˵����	�������ݻص�����
/*����˵����	Handle:  rtsp�Ự���
/*				fpRtpData: rtp���ݻص�����
/*				pUser: �û�Я������Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_SetDataEvent( THandle Handle, 
	fpnMRtspClients_RtpData fpRtpData, void* pUser );

/*****************************************************************
/*����˵����	����Rtsp�Ự
/*����˵����	Handle:  rtsp�Ự���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspClients_Play( THandle Handle );


/*****************************************************************
/*����˵����	�ر�Rtsp�Ự
/*����˵����	Handle:  rtsp�Ự���
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspClients_Close( THandle Handle );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
