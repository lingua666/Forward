
#ifndef __EXPORTS_20180719102642_1451301281_H__
#define __EXPORTS_20180719102642_1451301281_H__

#include "External.h"

DLL_LOG_DECL_DEF(Framework_RtspSvr_Module)

/*****************************************************************
/*ԭ��������	RTSP�Ự����ص�����
/*����˵����	pUser: �û�����
/*				c_szSessionUid: �ỰUID
/*				c_szsChildUrl: �Ự�����Url
/*				
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspSessionAccept)( void* pUser, const char* c_szSessionUid,
											const char* c_szsChildUrl ) ;


/*****************************************************************
/*ԭ��������	RTSP�Ự���ٻص�����
/*����˵����	pUser: �û�����
/*				c_szSessionUid: �ỰUID
/*				
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspSessionDestroy)(void* pUser, const char* c_szSessionUid) ;


/*****************************************************************
/*ԭ��������	RTSP�Ự״̬�ص�����
/*����˵����	pUser: �û�����
/*				c_szSessionUid: �ỰUID
/*				iStatus: ״̬
						1: ����
						2: ��ͣ
						3: ֹͣ
/*����ֵ��		
*****************************************************************/
typedef void (_CALLTYPE *fpnMRtspSessionOPStatus)(void* pUser, const char* c_szSessionUid, int iStatus) ;


/*****************************************************************
/*����˵����	��ʼ��
/*����˵����	c_szUser: �û���
/*				c_szPassword: ����
/*				c_szHost: ������
/*				c_szPort: �˿ں�
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Init( const char* c_szUser,
						const char* c_szPassword,
						const char* c_szHost,
						const char* c_szPort );


/*****************************************************************
/*����˵����	ģ���ͷ�
/*����˵����	
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MRtspSvr_Free( void );


/*****************************************************************
/*����˵����	����rtsp������
/*����˵����	iPort: �����˿ں�
/*				iIOThreadNum: ���������¼����߳���	
/*				iProcThreadNum: ������յ����������ݵ��߳���
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Run(  int iPort, int iIOThreadNum = 0,
	int iProcThreadNum = 0 );


/*****************************************************************
/*����˵����	ֹͣrtsp������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Stop( void );


/*****************************************************************
/*����˵����	����rtsp�������¼�
/*����˵����	c_szSessionUid:  �Ựuid
/*				fpAccept: ����rtsp�Ự�ص�����
/*				pAptUser: ���ջỰ�ص�����Я�����û�����
/*				fpDestroy: ���ջỰ���ٻص�����
/*				pDesUser: ���ٻص�������Я�����û�����
/*				fpStatus: rtsp�Ự״̬�ص�����
/*				pStaUser: ״̬�ص�����Я�����û�����
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_SetEvent( const char* c_szSessionUid, 
	fpnMRtspSessionAccept fpAccept, void* pAptUser,
	fpnMRtspSessionDestroy fpDestroy, void* pDesUser,
	fpnMRtspSessionOPStatus fpStatus, void* pStaUser );

/*****************************************************************
/*����˵����	�ر�rtsp�Ự
/*����˵����	c_szSessionUid:  �Ựuid
/*				
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_Close( const char* c_szSessionUid );


/*****************************************************************
/*����˵����	����һ��rtsp��ַ
/*����˵����	c_szUrl:  rtsp�Ự���ӵ�ַ(������rtsp������Ϣ����)
/*				iVideoType: ��Ƶ����
/*							0: H264
/*							0: H265
/*����ֵ��		-1: ʧ��	>0: ���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MRtspSvr_AddMediaStream( const char* c_szUrl, int iVideoType );


/*****************************************************************
/*����˵����	��rtsp��ַ�����Ƶ����
/*����˵����	Handle:		rtsp��ַ����Ӧ�ľ����MRtspSvr_AddMediaStream�����صľ����
/*				c_szData:	��Ƶ����
/*				iSize:		��Ƶ���ݳ���
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_PutMediaStream( THandle Handle, const char* c_szData, int iSize );


/*****************************************************************
/*����˵����	�Ƴ�rtsp��ַ
/*����˵����	c_szUrl:	rtsp��ַ
/*				Handle:		rtsp�Ự���ӵ�ַ���
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MRtspSvr_RemoveMediaStream( const char* c_szUrl, THandle Handle );


#endif//__PLC_OMRON_20151228191441_1451301281_H__
