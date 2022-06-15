
#include <dllThirdWrap_FFmpeg/Exports.h>
#include <dllThirdWrap_FFmpeg/FFmpeg_ShowVideo.hpp>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172024,
	"45000001000X2016021700000000WZZZ00000000",
	"MySQL���ݿ�ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(FFmpeg_Module)


/*****************************************************************
/*����˵����	��ʼ�������
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Init( HWND hWnd )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Init(hWnd);
}


/*****************************************************************
/*����˵����	�ͷŽ����
/*����˵����	
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Video_Free( void )
{
	Singleton<FFmpeg_ShowVideo>::instance()->Release();
	return 1;
}

/*****************************************************************
/*����˵����	��ȡ���������
/*����˵����	lWidth : ͼ����
/*				lHeight : ͼ��߶�
				iFrameRate : ֡��

/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Get( long lWidth, long lHeight, int iFrameRate, int iDecType )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Get(lWidth, lHeight, iFrameRate, (_ffmpeg_::enDecoderType)iDecType);
}


/*****************************************************************
/*����˵����	ͨ��ý�����ݻ�ȡ���������
/*����˵����	c_szData: ý�����ݻ�����
/*				uSize�����ݵĳ���

/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS THandle _CALLTYPE MDecoder_Parse( const char* c_szData, UInt32	uSize )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Parse(c_szData, uSize);
}


/*****************************************************************
/*����˵����	���ý�������ʾ���
/*����˵����	Handle�� ���������
/*				hWnd��	��ʾ���
/*				isRealTime: �Ƿ�ʵʱ����
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Set( THandle Handle, HWND hWnd, bool isRealTime )
{
	//return Singleton<FFmpeg_ShowVideo>::instance()->SetParamter(Handle, hWnd, isRealTime);
	return Singleton<FFmpeg_ShowVideo>::instance()->SetParamterRaw(Handle, hWnd, isRealTime);
}


/*****************************************************************
/*����˵����	���ý�������ʾ���
/*����˵����	Handle�����������
/*				pFunc:	�����YUV420���ݻص�����
/*				pUser:  �û�����
/*				isRealTime: �Ƿ�ʵʱ����
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetFunc( THandle Handle, fpnYUV420DecoderBackCall pFunc,
														void* pUser, bool isRealTime )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->SetParamter(Handle,
		function20_bind(pFunc, pUser, _function_::_1, _function_::_2, _function_::_3,
		_function_::_4, _function_::_5, _function_::_6), isRealTime);
}


/*****************************************************************
/*����˵����	���ݽ��벢��ʾ
/*����˵����	Handle�� ���������
/*				c_szData: ��Ҫ��������ݻ�����		
/*				uSize������������
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Decode( THandle Handle,
												const char* c_szData, UInt32 uSize )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Decode(Handle, c_szData, uSize);
}


/*****************************************************************
/*����˵����	���ݽ��벢��ʾ
/*����˵����	Handle�� ���������
/*				uSpeed: �ٶ�
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_SetSpeed( THandle Handle, UInt8 uSpeed )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->SetSpeed(Handle, uSpeed);
}

/*****************************************************************
/*����˵����	�رս�����
/*����˵����	Handle�����������
/*				
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MDecoder_Close( THandle Handle )
{
	return Singleton<FFmpeg_ShowVideo>::instance()->Close(Handle);
}