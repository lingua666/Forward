#ifndef __FFMPEG_SHOWVIDEO_20170928174048_1443447648_H__
#define __FFMPEG_SHOWVIDEO_20170928174048_1443447648_H__

#include <libThirdWrap_FFmpeg/DecoderManager.h>
#include <libFoundation/modafx.h>
#include <libThirdWrap_D3D/Shader/D3DManager.h>

class FFmpeg_ShowVideo
{
public:
	typedef	Container_HashMap_type<THandle, _d3d_::HIMAGE>		HImage_map;
	typedef function20_handle<void (HIMAGE hImage, const char*, int, int, int, int)>	HFNYUV420Decoder;

	static void DecodeYUV420CallBack( const HFNYUV420Decoder& pFunc, _d3d_::HIMAGE hImage,
									const _string_type& sData,
									int Width, int Height, int iShowTime )
	{
		if( pFunc )
		{
			pFunc((HIMAGE)hImage, sData.c_str(), sData.size(), Width, Height, iShowTime);
		}
	}

	static void ShowVideoCallBack( _d3d_::HIMAGE hImage, HWND hWnd,
								const _string_type& sData,
								int Width, int Height, int iShowTime )
	{
		_timestamp_::Timestamp Time;
		_d3d_::CD3DManager* pMan = Singleton<_d3d_::CD3DManager>::instance();
		pMan->UpdateImage( hImage, (BYTE*)sData.c_str() ) ;
		pMan->BeginRender () ;
		pMan->Draw_H(hImage, hWnd);
		pMan->EndRender (hWnd) ;

		if( iShowTime > 0 )
		{
			int iDelay = (iShowTime - (_timestamp_::Timestamp() - Time)) / 1000;
			if( iDelay > 0 )
			{
				Sleep(iDelay);
			}
		}
	}

public:
	FFmpeg_ShowVideo( void )
		: _isInit( false )
	{

	}

	~FFmpeg_ShowVideo( void )
	{
		Release();
	}

	int Init( HWND hWnd = NULL )
	{
		if( !_isInit )
		{
			HWND h = hWnd;
			if( hWnd == NULL )
			{
				HDC hDCScreen = GetDC( NULL ); 
				h = WindowFromDC(hDCScreen);
			}

			RECT Rect = {0};
			::GetWindowRect(h, &Rect);
			long lWidth = Rect.right - Rect.left; //ImageInfo.lWidth
			long lHeight = Rect.bottom - Rect.top;//ImageInfo.lHeight

			if( Singleton<_d3d_::CD3DManager>::instance()->CreateDevice(h,
															lWidth, lHeight) == 1 )
			{
				_isInit = true;
			}
		}

		return 1;
	}

	void Release( void )
	{
		if( _isInit )
		{
			HImage_map::iterator iter;
			_Lock.Lock();
			for ( iter = _Map.begin() ; iter != _Map.end() ; iter++ )
			{
				if(iter->second != NULL)
				{
					Singleton<_d3d_::CD3DManager>::instance()->DestroyImage(iter->second);
				}
			}
			_Map.clear();
			_Lock.UnLock();

			Singleton<_d3d_::CD3DManager>::instance()->Destroy();
			_isInit = false;
		}
	}

	int Close( THandle Handle )
	{
		_d3d_::HIMAGE hImage = NULL;
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		hImage = iter->second;
		_Map.erase(iter);
		_Lock.UnLock();
		Singleton<_d3d_::CD3DManager>::instance()->DestroyImage(hImage);
		Singleton<_ffmpeg_::DecoderManager>::instance()->Close( reinterpret_cast<_ffmpeg_::HDecoder>(Handle) );
		return 1;
	}

	int SetParamter( THandle Handle, HWND hWnd, bool isRealTime )
	{
		_d3d_::HIMAGE hImage = NULL;
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		hImage = iter->second;
		_Lock.UnLock();

		Singleton<_ffmpeg_::DecoderManager>::instance()->SetParamter( reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			function20_bind(&FFmpeg_ShowVideo::ShowVideoCallBack, hImage, hWnd,
			_function_::_1, _function_::_2, _function_::_3, _function_::_4), isRealTime );

		return 1;
	}

	int SetParamter( THandle Handle, const HFNYUV420Decoder& hDecode, bool isRealTime )
	{
		_d3d_::HIMAGE hImage = NULL;
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		hImage = iter->second;
		_Lock.UnLock();

		Singleton<_ffmpeg_::DecoderManager>::instance()->SetParamter( reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			function20_bind(&FFmpeg_ShowVideo::DecodeYUV420CallBack, hDecode, hImage,
			_function_::_1, _function_::_2, _function_::_3, _function_::_4), isRealTime );

		return 1;
	}

	THandle Get( long lWidth, long lHeight, int iFrameRate,
				_ffmpeg_::enDecoderType enType )
	{
		_ffmpeg_::HDecoder hDec = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoder(lWidth, lHeight,
			iFrameRate, enType);
		if( hDec == NULL )
			return 0;

		if( _isInit )
		{
			_ffmpeg_::IDecoder::tagDecoder_INFO* ImageInfo = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoderInfo(hDec);
			_d3d_::HIMAGE hImage = Singleton<_d3d_::CD3DManager>::instance()->CreateImage(((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth,
				((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight, _d3d_::CS_YV12);
			if( hImage == 0 )
			{
				Singleton<_ffmpeg_::DecoderManager>::instance()->Close(hDec);
				return 0;
			}

			_Lock.Lock();
			_Map.insert(std::make_pair(reinterpret_cast<THandle>(hDec), hImage));
			_Lock.UnLock();
		}

		return reinterpret_cast<THandle>(hDec);
	}

	THandle Parse( const char* c_szData, UInt32	uSize )
	{
		_ffmpeg_::HDecoder hDec = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoder(c_szData, uSize);
		if( hDec == NULL )
			return 0;

		if( _isInit )
		{
			_ffmpeg_::IDecoder::tagDecoder_INFO* ImageInfo = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoderInfo(hDec);
			_d3d_::HIMAGE hImage = Singleton<_d3d_::CD3DManager>::instance()->CreateImage(((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth,
				((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight, _d3d_::CS_YV12);
			if( hImage == 0 )
			{
				Singleton<_ffmpeg_::DecoderManager>::instance()->Close(hDec);
				return 0;
			}

			_Lock.Lock();
			_Map.insert(std::make_pair(reinterpret_cast<THandle>(hDec), hImage));
			_Lock.UnLock();
		}

		return reinterpret_cast<THandle>(hDec);
	}

	int Decode( THandle Handle,
			const char* c_szData, UInt32 uSize )
	{
		return Singleton<_ffmpeg_::DecoderManager>::instance()->Decoder(reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			(unsigned char*)c_szData, uSize);
	}

	int Show( HIMAGE hImage, HWND hWnd )
	{
		_d3d_::CD3DManager* pMan = Singleton<_d3d_::CD3DManager>::instance();
		pMan->BeginRender () ;
		pMan->Draw_H(hImage, hWnd);
		pMan->EndRender (hWnd) ;
		return 1;
	}

	int	UpdateImage( HIMAGE hImage,
		const char* c_szYUV420Data, UInt32 uSize )
	{
		_d3d_::CD3DManager* pMan = Singleton<_d3d_::CD3DManager>::instance();
		return pMan->UpdateImage( hImage, (BYTE*)c_szYUV420Data ) ? 1 : -1 ;
	}

	int SetSpeed( THandle Handle, UInt8 uSpeed )
	{
		Singleton<_ffmpeg_::DecoderManager>::instance()->SetSpeed(reinterpret_cast<_ffmpeg_::HDecoder>(Handle), uSpeed);
		return 1;
	}

private:
	bool _isInit;
	HImage_map	_Map;
	CLock	_Lock;
};


#endif