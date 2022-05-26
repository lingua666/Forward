#ifndef __FFMPEG_SHOWVIDEO_20170928174048_1443447648_H__
#define __FFMPEG_SHOWVIDEO_20170928174048_1443447648_H__

#include <libThirdWrap_FFmpeg/DecoderManager.h>
#include <libFoundation/modafx.h>
#include <libThirdWrap_D3D/Shader/D3DManager.h>

class	FFmpeg_ShowVideo;

class VideoImageD3D_Impl : public _enable_shared_from_this<VideoImageD3D_Impl>
{
public:
	friend	class FFmpeg_ShowVideo;

	static TimerEvent* D3DTimerEventInstance( void )
	{
		static TimerEvent s_TimerEv(1);
		return &s_TimerEv;
	}

	static TimerEvent* D3DRealShowTimerEventInstance( void )
	{
		static TimerEvent s_TimerEv(1);
		return &s_TimerEv;
	}

public:
	VideoImageD3D_Impl( _ffmpeg_::HDecoder hDec )
		: _hDec( hDec )
		, _hImage( NULL )
		, _hWnd( NULL )
		, _isInit( false )
		, _isInitRet ( 0 )
		, _isShow( false )
		, _uWidth( 0 )
		, _uHeight( 0 )
	{

	}

	~VideoImageD3D_Impl( void )
	{
		Release();
		if( _hDec != NULL )
		{
			Singleton<_ffmpeg_::DecoderManager>::instance()->Destroy( _hDec );
			_hDec = NULL;
		}
	}

	int Init( _ffmpeg_::HDecoder hDec, UInt32 uWidth, UInt32 uHeight, HWND hWnd )
	{
		Release();

		_hDec = hDec;
		_isInitRet = 0;
		_D3dMan.Init(function20_bind(&VideoImageD3D_Impl::HandleLostD3DDevice, shared_from_this()));
		D3DTimerEventInstance()->push_back(function20_bind(&VideoImageD3D_Impl::Init_Impl,
			shared_from_this(), uWidth, uHeight, hWnd), 0);
		while(!_isInitRet)
		{
			Sleep(1);
		};

		_uWidth = uWidth;
		_uHeight = uHeight;
		return _isInitRet;
	}

	void Release( void )
	{
		if( _isInit )
		{
			if( _hImage != NULL )
			{
				_D3dMan.DestroyImage(_hImage);
				_hImage = NULL;
			}

			_D3dMan.Destroy();
			_isInit = false;
			_hWnd = NULL;
		}
	}

	void Close( void )
	{
		if( _hDec != NULL )
		{
			Singleton<_ffmpeg_::DecoderManager>::instance()->Reset( _hDec );
		}

		_D3dMan.Reset();
	}

	void HandleShow_Raw( AVCodecContext* pContext, AVFrame* pFrame,	int iShowTime )
	{
		if( !_isInit || _hWnd == NULL || pContext == NULL || pFrame == NULL || _D3dMan.isLost() ||
			(pContext->height != pContext->coded_height || pContext->width != pContext->coded_width) )
			return ;

		if( pContext->width != _uWidth || pContext->height != _uHeight )
		{
			return ;
		}

		Timestamp_type Time;
		_d3d_::CD3DManager* pMan = Singleton<_d3d_::CD3DManager>::instance();
		_d3d_::CDisplay::tagDisplayRect Rect = {0};
		BYTE *pDest = NULL;
		int i = 0,offset = 0;

		_Lock.Lock();

		if( _D3dMan.LockRect(_hImage, 0, &Rect) )
		{
			pDest = (BYTE*)Rect.pBits ;
			for(i = 0; i < pContext->height; i ++)
			{//Y
				memcpy(pDest, pFrame->data[0] + i * pFrame->linesize[0], pContext->width); 
				pDest += Rect.Pitch;
			}

			_D3dMan.UnlockRect(_hImage, 0);
		}

		if( _D3dMan.LockRect(_hImage, 1, &Rect) )
		{
			pDest = (BYTE*)Rect.pBits ;
			for(i = 0; i < pContext->height / 2; i ++)
			{//U
				memcpy(pDest, pFrame->data[1] + i * pFrame->linesize[1], pContext->width / 2); 
				pDest += Rect.Pitch;		
			}

			_D3dMan.UnlockRect(_hImage, 1);
		}

		if( _D3dMan.LockRect(_hImage, 2, &Rect) )
		{
			pDest = (BYTE*)Rect.pBits ;
			for(i = 0; i < pContext->height / 2; i ++)
			{//V
				memcpy(pDest, pFrame->data[2] + i * pFrame->linesize[2], pContext->width / 2); 
				pDest += Rect.Pitch;
			}

			_D3dMan.UnlockRect(_hImage, 2);
		}

		if( !_isShow )
		{
			if( iShowTime > 0 )
			{
				_isShow = true;
				D3DRealShowTimerEventInstance()->push_back(function20_bind(&VideoImageD3D_Impl::DrawDelay,
					shared_from_this(), Time, iShowTime), 0);
			}
			else
			{
				_isShow = true;
				D3DRealShowTimerEventInstance()->push_back(function20_bind(&VideoImageD3D_Impl::Draw,
					shared_from_this()), 0);
			}
		}

		_Lock.UnLock();
	}

	void HandleShow( const _string_type& sData,
					int Width, int Height, int iShowTime )
	{
		Timestamp_type Time;
		_D3dMan.UpdateImage( _hImage, (BYTE*)sData.c_str() ) ;
		_D3dMan.BeginRender () ;
		_D3dMan.Draw_H(_hImage, _hWnd);
		_D3dMan.EndRender () ;

		if( iShowTime > 0 )
		{
			int iDelay = (iShowTime - (Timestamp_type() - Time)) / 1000;
			if( iDelay > 0 )
			{
				Sleep(iDelay);
			}
		}
	}

protected:
	int Init_Impl( UInt32 uWidth, UInt32 uHeight, HWND hWnd )
	{
		if( InitD3D(hWnd) == -1 )
		{
			_isInitRet = -1;
			return -1;
		}

		_hWnd = hWnd;
		return (_isInitRet = (_hImage = _D3dMan.CreateImage(uWidth, uHeight, _d3d_::CS_YV12)) == 0 ? -1 : 1);
	}

	void DrawDelay( const Timestamp_type& Time, int iShowTime )
	{
		Draw();
		int iDelay = (iShowTime - (Timestamp_type() - Time)) / 1000;
		if( iDelay > 0 )
		{
			Sleep(iDelay);
		}
	}

	void Draw( void )
	{
		_Lock.Lock();
		_D3dMan.BeginRender () ;
		_D3dMan.Draw_H(_hImage, _hWnd);
		_D3dMan.EndRender () ;
		_isShow = false;
		_Lock.UnLock();
	}

	int InitD3D( HWND hWnd )
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

			if( _D3dMan.CreateDevice(h, lWidth, lHeight) != 1 )
			{
				return -1;
			}

			_isInit = true;
		}

		return 1;
	}

	void HandleLostD3DDevice( void )
	{
		D3DTimerEventInstance()->push_back(function20_bind(&VideoImageD3D_Impl::D3DDeviceRecovery,
			shared_from_this()), 0);
	}

	void D3DDeviceRecovery( void )
	{
		_D3dMan.LostRecovery();
	}

private:
	_d3d_::CD3DManager _D3dMan;
	_ffmpeg_::HDecoder _hDec;
	_d3d_::HIMAGE	_hImage;
	TimerEvent		_TimeEv;
	HWND			_hWnd;
	bool			_isInit;
	int				_isInitRet;
	CLock			_Lock;
	bool			_isShow;
	UInt32			_uWidth;
	UInt32			_uHeight;

};

class FFmpeg_ShowVideo
{
public:
	typedef SmartPTR<VideoImageD3D_Impl>								VideoImage_sptr;
	typedef	Container_HashMap_type<THandle, VideoImage_sptr>			HImage_map;
	typedef function20_handle<void (HIMAGE hImage, const char*, int, int, int, int)>	HFNYUV420Decoder;

	void DecodeYUV420CallBack( const HFNYUV420Decoder& pFunc, _d3d_::HIMAGE hImage,
							const _string_type& sData,
							int Width, int Height, int iShowTime )
	{
		if( pFunc )
		{
			pFunc((HIMAGE)hImage, sData.c_str(), sData.size(), Width, Height, iShowTime);
		}
	}

public:
	FFmpeg_ShowVideo( void )
		: _isInit( false )
		, _Pool( sizeof(VideoImageD3D_Impl) )
	{

	}

	~FFmpeg_ShowVideo( void )
	{
		Release();
	}

	int Init( HWND hWnd = NULL )
	{
		return 1;
	}

	void Release( void )
	{
		_Lock.Lock();
		_Map.clear();
		_Lock.UnLock();
	}

	int Close( THandle Handle )
	{
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		iter->second->Close();
		_Map.erase(iter);
		_Lock.UnLock();
		return 1;
	}

	int SetParamter( THandle Handle, HWND hWnd, bool isRealTime )
	{
		VideoImage_sptr sptr;
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		sptr = iter->second;
		_Lock.UnLock();

		_ffmpeg_::IDecoder::tagDecoder_INFO* ImageInfo = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoderInfo(reinterpret_cast<_ffmpeg_::HDecoder>(Handle));

		if( sptr->Init(reinterpret_cast<_ffmpeg_::HDecoder>(Handle), ((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth,
			((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight, hWnd) == -1 )
		{
			//Close(Handle);
			return -1;
		}

		Singleton<_ffmpeg_::DecoderManager>::instance()->SetParamter( reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			function20_bind(&VideoImageD3D_Impl::HandleShow, sptr,
			_function_::_1, _function_::_2, _function_::_3, _function_::_4), isRealTime );
		return 1;
	}

	int SetParamterRaw( THandle Handle, HWND hWnd, bool isRealTime )
	{
		VideoImage_sptr sptr;
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		sptr = iter->second;
		_Lock.UnLock();

		_ffmpeg_::IDecoder::tagDecoder_INFO* ImageInfo = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoderInfo(reinterpret_cast<_ffmpeg_::HDecoder>(Handle));

		if( sptr->Init(reinterpret_cast<_ffmpeg_::HDecoder>(Handle), ((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uWidth,
			((_ffmpeg_::IDecoder::tagVideo_INFO*)ImageInfo->pInfo)->uHeight, hWnd) == -1 )
		{
			//Close(Handle);
			return -1;
		}

		Singleton<_ffmpeg_::DecoderManager>::instance()->SetParamterRaw( reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			function20_bind(&VideoImageD3D_Impl::HandleShow_Raw, sptr,
			_function_::_1, _function_::_2, _function_::_3), isRealTime );
		return 1;
	}

	int SetParamter( THandle Handle, const HFNYUV420Decoder& hDecode, bool isRealTime )
	{
		VideoImage_sptr sptr;
		HImage_map::iterator iter;
		_Lock.Lock();
		iter = _Map.find(Handle);
		if( iter == _Map.end() )
		{
			_Lock.UnLock();
			return -1;
		}
		sptr = iter->second;
		_Lock.UnLock();

		Singleton<_ffmpeg_::DecoderManager>::instance()->SetParamter( reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			function20_bind(&FFmpeg_ShowVideo::DecodeYUV420CallBack, this, hDecode, sptr->_hImage,
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

		VideoImage_sptr sptr(_Pool.AllocObj<VideoImageD3D_Impl>(hDec),
			function20_bind_Ex(&MemPool_type::FreeObj<VideoImageD3D_Impl>, &_Pool));

		_Lock.Lock();
		_Map.insert(std::make_pair(reinterpret_cast<THandle>(hDec), sptr));
		_Lock.UnLock();

		return reinterpret_cast<THandle>(hDec);
	}

	THandle Parse( const char* c_szData, UInt32	uSize )
	{
		_ffmpeg_::HDecoder hDec = Singleton<_ffmpeg_::DecoderManager>::instance()->GetDecoder(c_szData, uSize);
		if( hDec == NULL )
			return 0;

		VideoImage_sptr sptr(_Pool.AllocObj<VideoImageD3D_Impl>(hDec),
			function20_bind_Ex(&MemPool_type::FreeObj<VideoImageD3D_Impl>, &_Pool));

		_Lock.Lock();
		_Map.insert(std::make_pair(reinterpret_cast<THandle>(hDec), sptr));
		_Lock.UnLock();

		return reinterpret_cast<THandle>(hDec);
	}

	int Decode( THandle Handle,
			const char* c_szData, UInt32 uSize )
	{
		return Singleton<_ffmpeg_::DecoderManager>::instance()->DecoderRaw(reinterpret_cast<_ffmpeg_::HDecoder>(Handle),
			(unsigned char*)c_szData, uSize);
	}

	int SetSpeed( THandle Handle, UInt8 uSpeed )
	{
		Singleton<_ffmpeg_::DecoderManager>::instance()->SetSpeed(reinterpret_cast<_ffmpeg_::HDecoder>(Handle), uSpeed);
		return 1;
	}

private:
	bool _isInit;
	HImage_map	_Map;
	ThreadWrap	_ShowThread;
	CLock		_Lock;
	CLock		_ShowLock;
	MemPool_type	_Pool;
};

#endif