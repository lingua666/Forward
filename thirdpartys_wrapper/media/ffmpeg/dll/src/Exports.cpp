
#include <dllThirdWrap_FFmpeg/Exports.h>
#include <dllThirdWrap_FFmpeg/FFmpeg_ShowVideo.hpp>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172024,
	"45000001000X2016021700000000WZZZ00000000",
	"MySQL数据库模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(FFmpeg_Module)


/*****************************************************************
/*函数说明：	初始化解码库
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Init( HWND hWnd )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Init(hWnd);
}


/*****************************************************************
/*函数说明：	释放解码库
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Free( void )
{
	Singleton<FFmpeg_ShowVideo>::instance()->Release();
	return 1;
}

/*****************************************************************
/*函数说明：	获取解码器句柄
/*参数说明：	lWidth : 图像宽度
/*				lHeight : 图像高度
				iFrameRate : 帧率

/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Get( long lWidth, long lHeight, int iFrameRate, int iDecType )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Get(lWidth, lHeight, iFrameRate, (_ffmpeg_::enDecoderType)iDecType);
}


/*****************************************************************
/*函数说明：	通过媒体数据获取解码器句柄
/*参数说明：	c_szData: 媒体数据缓冲区
/*				uSize：数据的长度

/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Parse( const char* c_szData, UInt32	uSize )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Parse(c_szData, uSize);
}


/*****************************************************************
/*函数说明：	设置解码器显示句柄
/*参数说明：	Handle： 解码器句柄
/*				hWnd：	显示句柄
/*				isRealTime: 是否实时数据
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Set( THandle Handle, HWND hWnd, bool isRealTime )
{
	//return Singleton<FFmpeg_ShowVideo>::instance()->SetParamter(Handle, hWnd, isRealTime);
	return Singleton<FFmpeg_ShowVideo>::instance()->SetParamterRaw(Handle, hWnd, isRealTime);
}


/*****************************************************************
/*函数说明：	设置解码器显示句柄
/*参数说明：	Handle：解码器句柄
/*				pFunc:	解码后YUV420数据回调函数
/*				pUser:  用户数据
/*				isRealTime: 是否实时数据
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetFunc( THandle Handle, fpnYUV420DecoderBackCall pFunc,
														void* pUser, bool isRealTime )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->SetParamter(Handle,
		function20_bind(pFunc, pUser, _function_::_1, _function_::_2, _function_::_3,
		_function_::_4, _function_::_5, _function_::_6), isRealTime);
}


/*****************************************************************
/*函数说明：	数据解码并显示
/*参数说明：	Handle： 解码器句柄
/*				c_szData: 需要解码的数据缓冲区		
/*				uSize：缓冲区长度
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Decode( THandle Handle,
												const char* c_szData, UInt32 uSize )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Decode(Handle, c_szData, uSize);
}


/*****************************************************************
/*函数说明：	数据解码并显示
/*参数说明：	Handle： 解码器句柄
/*				uSpeed: 速度
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetSpeed( THandle Handle, UInt8 uSpeed )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->SetSpeed(Handle, uSpeed);
}

/*****************************************************************
/*函数说明：	关闭解码器
/*参数说明：	Handle：解码器句柄
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Close( THandle Handle )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Close(Handle);
}