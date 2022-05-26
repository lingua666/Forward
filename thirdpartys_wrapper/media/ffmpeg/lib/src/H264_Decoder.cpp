#include <libThirdWrap_FFmpeg/H264_Decoder.h>

namespace _ffmpeg_{

	H264Decoder::H264Decoder( void )
		: _pCodecCtx(NULL)
		, _pavfFrame(NULL)
		, _pCodecParserCtx(NULL)
		, _uFrameNumber(0)
		, _uSpeed(1)
		, _TimeStamp(0)
		, _uFps(25)
		, _NextTime(0)
	{
		_isOpen = false;
		_isRealTime = false;

		//RGB
		_RgbBuffer = NULL;
		m_RgbCtx = NULL;
		_FrameRGB = NULL;

		//YV12
		_YV12Buffer = NULL;
		m_YV12Ctx = NULL;
		_FrameYV12 = NULL;
	}

	H264Decoder::~H264Decoder( void )
	{
		Release();
	}

	int  H264Decoder::Init( long lWidth, long lHeight,
						UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder,
						const HFNPrevOpen& hfnPrev )
	{
		//已经打开
		if( _pCodecCtx )
		{
			Release();
		}

		_hfnDecoder = hfnFNDecoder;
		_uFrameNumber = 0;
		_uSpeed = 1;
		_uFps = uFrameRate;

		//得到H264的解码器： 
		AVCodec* pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

		if(NULL != pCodec)
		{
			//创建一个AVCodecContext，并用默认值初始化： 
			_pCodecCtx = avcodec_alloc_context3(pCodec); 
			if(!_pCodecCtx)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() avcodec_alloc_context3 Failed!")
				return -1;
			}

			av_init_packet(&_avpkt);
			_avpkt.data = NULL;
			_avpkt.size = 0;

			/* 初始化AVCodecParserContext */  
			_pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);  
			if (!_pCodecParserCtx)  
			{  
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() av_parser_init Failed!")
				Release();
				return -1;
			}

			//更改g_pCodecCtx的一些成员变量的值，您应该从解码方得到这些变量值： 
			/*m_pCodecCtx->time_base.num = 1; //这两行：一秒钟25帧 
			m_pCodecCtx->time_base.den = 25; 
			m_pCodecCtx->bit_rate = 0; //初始化为0 
			m_pCodecCtx->frame_number = 1; //每包一个视频帧 
			m_pCodecCtx->codec_type = CODEC_TYPE_VIDEO; 
			m_pCodecCtx->width = 320; //这两行：视频的宽度和高度 
			m_pCodecCtx->height = 240; */

			//打开codec。如果打开成功的话，分配AVFrame：
			/*if(pCodec->capabilities & CODEC_CAP_TRUNCATED)
				m_pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;*/ /* we do not send complete frames */
			
			_pCodecCtx->time_base.num = 1; //这两行：一秒钟25帧 
			_pCodecCtx->time_base.den = uFrameRate; 
			_pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
			_pCodecCtx->coded_width =  _pCodecCtx->width = lWidth; //这两行：视频的宽度和高度 
			_pCodecCtx->coded_height = _pCodecCtx->height = lHeight; 
			_pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
			_pCodecCtx->frame_number = 1; //每包一个视频帧

			//test
			_pCodecCtx->flags |= AV_CODEC_FLAG_LOW_DELAY; //解码器零延时。其实解码器内部默认会缓存几帧数据，用于后续关联帧的解码，大概是3-5帧
			//test

			if( hfnPrev )
			{
				hfnPrev((HDecoder)this, _pCodecCtx);
			}

			if(avcodec_open2(_pCodecCtx, pCodec, NULL) < 0) 
			{ 
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() avcodec_open2 Failed!")
				Release();
				return -1;
			} 

			_pavfFrame = av_frame_alloc();// Allocate video frame 
			if( _pavfFrame == NULL )
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() av_frame_alloc Failed!")
				Release();
				return -1;
			}

			_FrameRGB = av_frame_alloc();
			if( _FrameRGB == NULL )
			{
				Release();
				return -1;
			}

			int iLen = avpicture_get_size(AV_PIX_FMT_BGR24, _pCodecCtx->width, _pCodecCtx->height);
			// _RgbBuffer要到最后释放
			_RgbBuffer = (unsigned char *)av_malloc(iLen);
			if ( _RgbBuffer == NULL )
			{
				Release();
				return -1;
			}

			avpicture_fill((AVPicture *)_FrameRGB, _RgbBuffer, AV_PIX_FMT_BGR24,
				_pCodecCtx->width, _pCodecCtx->height);

			// 创建转换上下文
			//RGB
			m_RgbCtx = sws_getContext(_pCodecCtx->width, _pCodecCtx->height,
							_pCodecCtx->pix_fmt, _pCodecCtx->width, _pCodecCtx->height, 
				AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
			if ( m_RgbCtx == NULL )
			{
				Release();
				return -1;
			}

			//YV12
			_FrameYV12 = av_frame_alloc();
			if (!_FrameYV12)
			{
				Release();
				return -1;
			}

			iLen = avpicture_get_size(AV_PIX_FMT_YUV420P, _pCodecCtx->width, _pCodecCtx->height);
			_YV12Buffer = (unsigned char *)av_malloc(iLen);
			if (!_YV12Buffer)
			{
				Release();
				return -1;
			}
			avpicture_fill((AVPicture *)_FrameYV12, _YV12Buffer, AV_PIX_FMT_YUV420P, _pCodecCtx->width, _pCodecCtx->height);
			m_YV12Ctx = sws_getContext(_pCodecCtx->width, _pCodecCtx->height, _pCodecCtx->pix_fmt, _pCodecCtx->width, _pCodecCtx->height, 
				AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
			if (m_YV12Ctx == NULL)
			{
				Release();
				return -1;
			}

			_VideoInfo.uFrameRate = uFrameRate;
			_VideoInfo.uHeight = lHeight;
			_VideoInfo.uWidth = lWidth;
			_VideoInfo.uFormat = AV_PIX_FMT_YUVJ420P;
			_DecoderInfo.pInfo = (char*)&_VideoInfo;
			_DecoderInfo.uEncType = EN_TYPE_H264;
			_DecoderInfo.iThreadType = _pCodecCtx->active_thread_type;
			_DecoderInfo.pCodecCtx = _pCodecCtx;

			_isOpen = true;

			//size for YUV 420
			_sBuf.resize(_pCodecCtx->width * _pCodecCtx->height * 3 / 2);

			if( _NextTime == 0 )
				_NextTime = _timestamp_::Timestamp().epochMicroseconds();
			return 1;
		}

		return -1;
	}

	int  H264Decoder::Init( AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
						const HFNPrevOpen& hfnPrev )
	{
		if( pCodecCtx == NULL )
			return -1;

		//已经打开
		if( _pCodecCtx )
		{
			Release();
		}
		else
		{
			av_init_packet(&_avpkt);
			_avpkt.data = NULL;
			_avpkt.size = 0;
		}
	
		_hfnDecoder = hfnFNDecoder;
		_uFrameNumber = 0;
		_uSpeed = 1;
		_uFps = pCodecCtx->time_base.den / pCodecCtx->time_base.num;

		//得到H264的解码器： 
		AVCodec* pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

		if(NULL != pCodec)
		{
			//创建一个AVCodecContext，并用默认值初始化： 
			_pCodecCtx = avcodec_alloc_context3(pCodec); 
			if(!_pCodecCtx)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() avcodec_alloc_context3 Failed!")
				return -1;
			}

			/* 初始化AVCodecParserContext */  
			_pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);  
			if (!_pCodecParserCtx)  
			{  
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() av_parser_init Failed!")
				Release();
				return -1;
			}

			avcodec_copy_context(_pCodecCtx, pCodecCtx);

			//test
			_pCodecCtx->flags |= AV_CODEC_FLAG_LOW_DELAY; //解码器零延时。其实解码器内部默认会缓存几帧数据，用于后续关联帧的解码，大概是3-5帧
			//test

			if( hfnPrev )
			{
				hfnPrev((HDecoder)this, _pCodecCtx);
			}

			if(avcodec_open2(_pCodecCtx, pCodec, NULL) < 0) 
			{ 
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() avcodec_open2 Failed!")
				Release();
				return -1;
			}

			_pavfFrame = av_frame_alloc();// Allocate video frame 
			if( _pavfFrame == NULL )
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::Init() av_frame_alloc Failed!")
				Release();
				return -1;
			}

			_FrameRGB = av_frame_alloc();
			if( _FrameRGB == NULL )
			{
				Release();
				return -1;
			}

			int iLen = avpicture_get_size(AV_PIX_FMT_BGR24, _pCodecCtx->width, _pCodecCtx->height);
			// _RgbBuffer要到最后释放
			_RgbBuffer = (unsigned char *)av_malloc(iLen);
			if (!_RgbBuffer)
			{
				Release();
				return -1;
			}

			avpicture_fill((AVPicture *)_FrameRGB, _RgbBuffer, AV_PIX_FMT_BGR24,
				_pCodecCtx->width, _pCodecCtx->height);

			// 创建转换上下文
			//RGB
			m_RgbCtx = sws_getContext(_pCodecCtx->width, _pCodecCtx->height,
				_pCodecCtx->pix_fmt, _pCodecCtx->width, _pCodecCtx->height, 
				AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
			if ( m_RgbCtx == NULL )
			{
				Release();
				return -1;
			}

			//YV12
			_FrameYV12 = av_frame_alloc();
			if (!_FrameYV12)
			{
				Release();
				return -1;
			}

			iLen = avpicture_get_size(AV_PIX_FMT_YUV420P, _pCodecCtx->width, _pCodecCtx->height);
			_YV12Buffer = (unsigned char *)av_malloc(iLen);
			if (!_YV12Buffer)
			{
				Release();
				return -1;
			}

			avpicture_fill((AVPicture *)_FrameYV12, _YV12Buffer, AV_PIX_FMT_YUV420P, _pCodecCtx->width, _pCodecCtx->height);
			m_YV12Ctx = sws_getContext(_pCodecCtx->width, _pCodecCtx->height, _pCodecCtx->pix_fmt, _pCodecCtx->width, _pCodecCtx->height, 
				AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
			if (m_YV12Ctx == NULL)
			{
				Release();
				return -1;
			}
		}

		_VideoInfo.uFrameRate = _uFps;
		_VideoInfo.uHeight = _pCodecCtx->height;
		_VideoInfo.uWidth = _pCodecCtx->width;
		_VideoInfo.uFormat = _pCodecCtx->pix_fmt;
		_DecoderInfo.pInfo = (char*)&_VideoInfo;
		_DecoderInfo.uEncType = EN_TYPE_H264;
		_DecoderInfo.iThreadType = _pCodecCtx->active_thread_type;
		_DecoderInfo.pCodecCtx = _pCodecCtx;

		_isOpen = true;

		//size for YUV 420 
		_sBuf.resize(_pCodecCtx->width * _pCodecCtx->height * 3 / 2);

		if( _NextTime == 0 )
			_NextTime = _timestamp_::Timestamp().epochMicroseconds();

		return 1;
	}

	void H264Decoder::Close( void )
	{
		if( _pCodecCtx != NULL && _isOpen )
		{
			_isOpen = false;
			avcodec_close(_pCodecCtx);
		}
	}

	int H264Decoder::ReOpen( const HFNPrevOpen& hOpen )
	{
		if( _pCodecCtx == NULL )
			return -1;

		//得到H264的解码器： 
		AVCodec* pCodec = NULL; 
		pCodec = avcodec_find_encoder_by_name("h264_mediacodec");
		if( pCodec == NULL )
		{//不支持硬解码
			pCodec = avcodec_find_decoder(AV_CODEC_ID_H264); //软解码
		}

		if( avcodec_close(_pCodecCtx) < 0 )
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::ReOpen() avcodec_close Failed!")
			return -1;
		}

		if( _pCodecCtx->hwaccel_context != NULL )
		{
			av_freep(&_pCodecCtx->hwaccel_context);
			_pCodecCtx->hwaccel_context = NULL;
		}

		if( hOpen )
		{
			hOpen((HDecoder)this, _pCodecCtx);
		}

		if(avcodec_open2(_pCodecCtx, pCodec, NULL) < 0) 
		{ 
			LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::ReOpen() avcodec_open2 Failed!")
			Release();
			return -1;
		}

		_isOpen = true;

		return 1;
	}

	void H264Decoder::Release( void )
	{
		_NextTime = 0;
		_TimeStamp = _timestamp_::Timestamp(0);

		//RGB
		if( _RgbBuffer != NULL )
		{
			av_free(_RgbBuffer);
			_RgbBuffer = NULL;
		}

		if( m_RgbCtx != NULL )
		{
			sws_freeContext(m_RgbCtx);
			m_RgbCtx = NULL;
		}
	
		if( _FrameRGB != NULL )
		{
			av_free(_FrameRGB);
			_FrameRGB = NULL;
		}

		//YV12
		if( _YV12Buffer != NULL )
		{
			av_free(_YV12Buffer);
			_YV12Buffer = NULL;
		}

		if( m_YV12Ctx != NULL )
		{
			sws_freeContext(m_YV12Ctx);
			m_YV12Ctx = NULL;
		}

		if( _FrameYV12 != NULL )
		{
			av_free(_FrameYV12);
			_FrameYV12 = NULL;
		}

		if( _pavfFrame != NULL )
		{
			av_free(_pavfFrame);
			_pavfFrame = NULL;
		}

		if( _pCodecParserCtx != NULL )
		{
			av_parser_close(_pCodecParserCtx);
			_pCodecParserCtx = NULL;
		}

		if( _pCodecCtx != NULL )
		{
			if( _pCodecCtx->hwaccel_context != NULL )
			{
				av_freep(&_pCodecCtx->hwaccel_context);
				_pCodecCtx->hwaccel_context = NULL;
			}

			av_packet_unref(&_avpkt);
			avcodec_close(_pCodecCtx);
			av_free(_pCodecCtx);
			_pCodecCtx = NULL;

			_DecoderInfo.pCodecCtx = NULL;
		}
	}

	int  H264Decoder::SetParamter( const HFNDecoder& hfnFNDecoder, bool isRealTime )
	{
		_hfnDecoder = hfnFNDecoder;
		_isRealTime = isRealTime;
		return 1;
	}

	int  H264Decoder::SetParamterRaw( const HFNDecoderRaw& hfnFNDecoder, bool isRealTime )
	{
		_hfnDecoderRaw = hfnFNDecoder;
		_isRealTime = isRealTime;
		return 1;
	}

	int H264Decoder::Decoder( unsigned char* szData, UInt32 uSize )
	{
		int nGot= 0;

		if(!_pCodecCtx || !_pavfFrame || !_pCodecParserCtx || !_isOpen)
			return -1;

		//decode
		uint8_t *cur_ptr = szData;

		while (uSize > 0)
		{
			_TimeStamp = _timestamp_::Timestamp();

			int iLen = av_parser_parse2(_pCodecParserCtx, _pCodecCtx,  
				&_avpkt.data, &_avpkt.size, cur_ptr, uSize,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);  
			cur_ptr += iLen;
			uSize -= iLen;
			if (_avpkt.size == 0)
				continue; 

			do
			{
				iLen = avcodec_decode_video2(_pCodecCtx , _pavfFrame,\
					(int*)&nGot , &_avpkt);

				if(iLen < 0)
					break;

				_avpkt.data += iLen;
				_avpkt.size -= iLen;

				if(nGot > 0 && (_uFrameNumber ++ % _uSpeed) == 0)
				{//成功解码
					if( _hfnDecoder )
					{//帧速度计算
						if( _uFps > 25 )
							_uFps = 25;

						int iTime = 1000000 / _uFps;

						//pack YUV data
						if( _isRealTime )
						{
							if( (_NextTime + iTime) - _timestamp_::Timestamp().epochMicroseconds() >= 0 )
							{
								YUV420PToYUVBuffer(_pCodecCtx, _pavfFrame, _sBuf);
								_hfnDecoder(_sBuf, _pavfFrame->width, _pavfFrame->height, 0);
							}

							_NextTime = _timestamp_::Timestamp().epochMicroseconds();
						}
						else
						{
							YUV420PToYUVBuffer(_pCodecCtx, _pavfFrame, _sBuf);
							_hfnDecoder(_sBuf, _pavfFrame->width, _pavfFrame->height, 
								iTime - (_timestamp_::Timestamp() - _TimeStamp));
							_TimeStamp = _timestamp_::Timestamp();
						}
					}
				}
				else
				{

				}
			}while(_avpkt.size > 0);
		}

		return -1;
	}

	int  H264Decoder::DecoderRaw( unsigned char* szData, UInt32 uSize )
	{
		int nGot= 0, iFrameTime = 1000000 / _uFps;

		if(!_pCodecCtx || !_pavfFrame || !_pCodecParserCtx || !_isOpen)
			return -1;

		//decode
		uint8_t *cur_ptr = szData;

		while (uSize > 0)
		{
			_TimeStamp = _timestamp_::Timestamp();

			int iLen = av_parser_parse2(_pCodecParserCtx, _pCodecCtx,  
				&_avpkt.data, &_avpkt.size, cur_ptr, uSize,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			cur_ptr += iLen;
			uSize -= iLen;
			if (_avpkt.size == 0)  
				continue; 

			do
			{
				iLen = avcodec_decode_video2(_pCodecCtx , _pavfFrame,\
					(int*)&nGot , &_avpkt);

				if(iLen < 0)
					break;

				_avpkt.data += iLen;
				_avpkt.size -= iLen;

				if(nGot > 0 && (_uFrameNumber++ % _uSpeed) == 0)
				{//成功解码
					if( _hfnDecoderRaw )
					{//帧速度计算
						if( _uFps > 25 )
							_uFps = 25;

						Int64 iNow = _timestamp_::Timestamp().epochMicroseconds();
						
						if( _isRealTime )
						{
							if( (_NextTime + iFrameTime) - iNow >= -15000
								/*&& (_NextTime + iFrameTime) - iNow < iFrameTime - 10000*/)
							{
								_hfnDecoderRaw(_pCodecCtx, _pavfFrame, 0);
							}
							//test
							else
							{
								LOG_Print_Error(libThirdWrap_FFmpeg, "H264Decoder::DecoderRaw() Timeout Frame:%lld,%lld,%d\r\n",
									(_NextTime + iFrameTime), iNow, (_NextTime + iFrameTime) - iNow);
							}
							//test

							_NextTime = _timestamp_::Timestamp().epochMicroseconds();
						}
						else
						{
							_hfnDecoderRaw(_pCodecCtx, _pavfFrame, iFrameTime - (_timestamp_::Timestamp() - _TimeStamp));
							_TimeStamp = _timestamp_::Timestamp();
						}
					}
				}
				else
				{

				}
			}while(_avpkt.size > 0);
		}

		return -1;
	}

	void H264Decoder::SetSpeed( UInt8 uSpeed )
	{
		_uSpeed = max(1, uSpeed);
	}

	void H264Decoder::Reset( void )
	{
		_hfnDecoder.reset();
		_hfnDecoderRaw.reset();
	}

	int H264Decoder::Convert_YV12ToRGB( AVFrame* pFrameYV12, AVFrame* Out_pFrameRgb )
	{
		if( m_RgbCtx == NULL )
			return -1;

		sws_scale(m_RgbCtx, pFrameYV12->data, pFrameYV12->linesize, 0, _pCodecCtx->height, 
			Out_pFrameRgb->data, Out_pFrameRgb->linesize);

		return 1;
	}

	int H264Decoder::Convert_RGBToYV12( AVFrame* pFrameRGB, AVFrame* Out_pFrameYV12 )
	{
		if( m_RgbCtx == NULL )
			return -1;

		sws_scale(m_RgbCtx, pFrameRGB->data, pFrameRGB->linesize, 0, _pCodecCtx->height, 
			Out_pFrameYV12->data, Out_pFrameYV12->linesize);

		return 1;
	}

} //namespace _ffmpeg_