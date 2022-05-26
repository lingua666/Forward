
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
/*函数说明：	初始化解码库
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Init( HWND hWnd = NULL );


/*****************************************************************
/*函数说明：	释放解码库
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Free( void );


/*****************************************************************
/*函数说明：	获取解码器句柄
/*参数说明：	lWidth : 图像宽度
/*				lHeight : 图像高度
				iFrameRate : 帧率

/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Get( long lWidth, long lHeight, int iFrameRate, int iDecType );


/*****************************************************************
/*函数说明：	通过媒体数据获取解码器句柄
/*参数说明：	c_szData: 媒体数据缓冲区
/*				uSize：数据的长度 必须大于2048

/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Parse( const char* c_szData, UInt32	uSize );


/*****************************************************************
/*函数说明：	通过解码器直接显示图像
/*参数说明：	Handle： 解码器句柄
/*				hWnd：	显示句柄
/*
/*返回值：		1:成功  <0:错误码
/*注意:			不能与MDecoder_SetFunc函数同时使用，会覆盖前者的操作
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Set( THandle Handle, HWND hWnd, bool isRealTime = false );


/*****************************************************************
/*函数说明：	由解码器解码并将解码数据提交给回调函数，由上层控制显示
/*参数说明：	Handle： 解码器句柄
/*				pFunc：	解码后YUV420数据回调函数
/*
/*返回值：		1:成功  <0:错误码
/*注意:			不能与MDecoder_Set函数同时使用，会覆盖前者的操作
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetFunc( THandle Handle, fpnYUV420DecoderBackCall pFunc,
															void* pUser, bool isRealTime = false );


/*****************************************************************
/*函数说明：	数据解码并显示
/*参数说明：	Handle： 解码器句柄
/*				c_szData: 需要解码的数据缓冲区		
/*				uSize：缓冲区长度
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Decode( THandle Handle,
														const char* c_szData, UInt32 uSize );

/*****************************************************************
/*函数说明：	数据解码并显示
/*参数说明：	Handle： 解码器句柄
/*				uSpeed: 速度
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetSpeed( THandle Handle, UInt8 uSpeed );


/*****************************************************************
/*函数说明：	关闭解码器
/*参数说明：	Handle：解码器句柄
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Close( THandle Handle );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
