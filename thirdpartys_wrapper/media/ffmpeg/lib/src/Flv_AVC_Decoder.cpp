#include <libThirdWrap_FFmpeg/Flv_AVC_Decoder.h>

namespace _ffmpeg_{

	Flv_AVC_Decoder::Flv_AVC_Decoder( void )
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
		_FlvParse.Init( function20_bind_Ex(&Flv_AVC_Decoder::VideoDecoder, this) );
	}

	Flv_AVC_Decoder::~Flv_AVC_Decoder( void )
	{
		Release();
	}

	int  Flv_AVC_Decoder::Init( long lWidth, long lHeight,
							UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder,
							const HFNPrevOpen& hfnPrev )
	{
		return -1;
	}

	int  Flv_AVC_Decoder::Init( AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
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
			//挂上所有的codec。也许只挂一个H264的codec就行，我没试过
			av_init_packet(&_avpkt);
			_avpkt.data = NULL;
			_avpkt.size = 0;
		}
	
		_hfnDecoder = hfnFNDecoder;
		_uFrameNumber = 0;
		_uSpeed = 1;
		_uFps = pCodecCtx->time_base.den / pCodecCtx->time_base.num;

		//得到H264的解码器： 
		AVCodec* pCodec = NULL; 
		pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
		if(NULL != pCodec)
		{
			//创建一个AVCodecContext，并用默认值初始化： 
			_pCodecCtx = avcodec_alloc_context3(pCodec); 
			if(!_pCodecCtx)
				return -1;

			/* 初始化AVCodecParserContext */  
			_pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);  
			if (!_pCodecParserCtx)  
			{  
				Release();
				return -1;
			}

			avcodec_copy_context(_pCodecCtx, pCodecCtx);

			if( hfnPrev )
			{
				hfnPrev((HDecoder)this, _pCodecCtx);
			}

			if(avcodec_open2(_pCodecCtx, pCodec, NULL) < 0) 
			{ 
				Release();
				return -1;
			}

			_pavfFrame = av_frame_alloc();// Allocate video frame 
			if( _pavfFrame == NULL )
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
		_DecoderInfo.uEncType = EN_TYPE_AVC;
		_DecoderInfo.iThreadType = _pCodecCtx->active_thread_type;
		_DecoderInfo.pCodecCtx = _pCodecCtx;

		_isOpen = true;

		//size for YUV 420 
		_sBuf.resize(_pCodecCtx->width * _pCodecCtx->height * 3 / 2);

		if( _NextTime == 0 )
			_NextTime = _timestamp_::Timestamp().epochMicroseconds();
		return 1;
	}

	void Flv_AVC_Decoder::Close( void )
	{
		if( _pCodecCtx != NULL && _isOpen )
		{
			_isOpen = false;
			avcodec_close(_pCodecCtx);
		}
	}

	int Flv_AVC_Decoder::ReOpen( const HFNPrevOpen& hOpen )
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
			return -1;

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
			Release();
			return -1;
		}

		_isOpen = true;

		return 1;
	}

	void Flv_AVC_Decoder::Release( void )
	{
		_NextTime = 0;
		_TimeStamp = _timestamp_::Timestamp(0);

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
		}
	}

	int  Flv_AVC_Decoder::SetParamter( const HFNDecoder& hfnFNDecoder, bool isRealTime )
	{
		_hfnDecoder = hfnFNDecoder;
		_isRealTime = isRealTime;
		return 1;
	}

	int  Flv_AVC_Decoder::SetParamterRaw( const HFNDecoderRaw& hfnFNDecoder, bool isRealTime )
	{
		return 1;
	}

	int  Flv_AVC_Decoder::Decoder( unsigned char* szData, UInt32 uSize )
	{
		_FlvParse.Parse((char*)szData, uSize);
		return 1;
	}

	int  Flv_AVC_Decoder::DecoderRaw( unsigned char* szData, UInt32 uSize )
	{
		return 1;
	}

	int	Flv_AVC_Decoder::VideoDecoder( const unsigned char* szData, UInt32 uSize )
	{
		int nGot= 0;

		if(!_pCodecCtx || !_pavfFrame || !_pCodecParserCtx || !_isOpen)
			return -1;

		if( _TimeStamp.epochMicroseconds() == 0 )
			_TimeStamp = _timestamp_::Timestamp();

		//decode
		uint8_t *cur_ptr = (uint8_t*)szData;

		while (uSize > 0)
		{
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
					

					if( _hfnDecoder )
					{//25帧速度计算
						if( _uFps > 25 )
							_uFps = 25;

						int iTime = 1000000 / _uFps;

						//pack YUV data
						/*int i = 0,offset = 0;
						for(i = 0; i < _pCodecCtx->height; i ++)
						{//Y
							memcpy(&_sBuf[offset], _pavfFrame->data[0] + i * _pavfFrame->linesize[0], _pCodecCtx->width); 
							offset += _pCodecCtx->width;
						}

						for(i = 0; i < _pCodecCtx->height / 2; i ++)
						{//U
							memcpy(&_sBuf[offset], _pavfFrame->data[1] + i * _pavfFrame->linesize[1], _pCodecCtx->width / 2); 
							offset += _pCodecCtx->width / 2;			
						}

						for(i = 0; i < _pCodecCtx->height / 2; i ++)
						{//V
							memcpy(&_sBuf[offset], _pavfFrame->data[2] + i * _pavfFrame->linesize[2], _pCodecCtx->width / 2); 
							offset += _pCodecCtx->width / 2;
						}*/

						if( _isRealTime )
						{
							if( (_NextTime + iTime) - _timestamp_::Timestamp().epochMicroseconds() >= 0 )
							{
								YUV420PToYUVBuffer(_pCodecCtx, _pavfFrame, _sBuf);
								_hfnDecoder(_sBuf, _pavfFrame->width, _pavfFrame->height, 0);
							}

							_NextTime = _timestamp_::Timestamp().epochMicroseconds();
							//_NextTime += iTime;
							//_hfnDecoder(_sBuf, _pavfFrame->width, _pavfFrame->height, 0);
						}
						else
						{
							YUV420PToYUVBuffer(_pCodecCtx, _pavfFrame, _sBuf);
							_hfnDecoder(_sBuf, _pavfFrame->width, _pavfFrame->height, 
								iTime - (_timestamp_::Timestamp() - _TimeStamp));

							/*_hfnDecoder(_sBuf, _pavfFrame->width, _pavfFrame->height, 
								1000000 / _uFps - (_timestamp_::Timestamp() - _TimeStamp));*/
						}
					}

					_TimeStamp = _timestamp_::Timestamp(0);
				}
				else
				{

				}
			}while(_avpkt.size > 0);
		}

		return -1;
	}

	void Flv_AVC_Decoder::SetSpeed( UInt8 uSpeed )
	{
		_uSpeed = max(1, uSpeed);
	}

	void Flv_AVC_Decoder::Reset( void )
	{
		_hfnDecoder.reset();
	}

} //namespace _ffmpeg_