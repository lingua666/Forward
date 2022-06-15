
#ifndef __EXPORTS_20170112121441_1444199361_H__
#define __EXPORTS_20170112121441_1444199361_H__

#include "External.h"
#include <windows.h>

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

enum enMDecoderType
{
	EN_TYPE_H264,
	EN_TYPE_H265
};

typedef void* HIMAGE;

typedef void (_CALLTYPE *fpnYUV420DecoderBackCall)( void* pUser, HIMAGE hImage, const char* c_szData,
											int DataSize, int Width, int Height, int iShowTime );

/*****************************************************************
/*����˵����	��ʼ�������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Init( HWND hWnd = NULL );


/*****************************************************************
/*����˵����	�ͷŽ����
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Free( void );


/*****************************************************************
/*����˵����	��ȡ���������
/*����˵����	lWidth : ͼ����
/*				lHeight : ͼ��߶�
				iFrameRate : ֡��

/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Get( long lWidth, long lHeight, int iFrameRate, int iDecType );


/*****************************************************************
/*����˵����	ͨ��ý�����ݻ�ȡ���������
/*����˵����	c_szData: ý�����ݻ�����
/*				uSize�����ݵĳ��� �������2048

/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Parse( const char* c_szData, UInt32	uSize );


/*****************************************************************
/*����˵����	ͨ��������ֱ����ʾͼ��
/*����˵����	Handle�� ���������
/*				hWnd��	��ʾ���
/*
/*����ֵ��		1:�ɹ�  <0:������
/*ע��:			������MDecoder_SetFunc����ͬʱʹ�ã��Ḳ��ǰ�ߵĲ���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Set( THandle Handle, HWND hWnd, bool isRealTime = false );


/*****************************************************************
/*����˵����	�ɽ��������벢�����������ύ���ص����������ϲ������ʾ
/*����˵����	Handle�� ���������
/*				pFunc��	�����YUV420���ݻص�����
/*
/*����ֵ��		1:�ɹ�  <0:������
/*ע��:			������MDecoder_Set����ͬʱʹ�ã��Ḳ��ǰ�ߵĲ���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetFunc( THandle Handle, fpnYUV420DecoderBackCall pFunc,
															void* pUser, bool isRealTime = false );


/*****************************************************************
/*����˵����	���ݽ��벢��ʾ
/*����˵����	Handle�� ���������
/*				c_szData: ��Ҫ��������ݻ�����		
/*				uSize������������
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Decode( THandle Handle,
														const char* c_szData, UInt32 uSize );

/*****************************************************************
/*����˵����	���ݽ��벢��ʾ
/*����˵����	Handle�� ���������
/*				uSpeed: �ٶ�
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetSpeed( THandle Handle, UInt8 uSpeed );


/*****************************************************************
/*����˵����	�رս�����
/*����˵����	Handle�����������
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Close( THandle Handle );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
