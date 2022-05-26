
#include <libThirdWrap_FFmpeg/MP3_Decoder.h>

namespace _ffmpeg_{

	MP3_Decoder::MP3_Decoder( void )
		: _pCodecCtx(NULL)
		, _pavfFrame(NULL)
		, _pCodecParserCtx(NULL)
		, _uSpeed(1)
	{
		_isOpen = false;
		_isRealTime = false;
	}

	MP3_Decoder::~MP3_Decoder( void )
	{
		Release();
	}

	int  MP3_Decoder::Init(long lWidth, long lHeight,
		UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder,
		const HFNPrevOpen& hfnPrev)
	{
		return -1;
	}

	int  MP3_Decoder::Init( AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
		const HFNPrevOpen& hfnPrev )
	{
		if( pCodecCtx == NULL || pCodecCtx->sample_rate <= 0)
			return -1;

		//�Ѿ���
		if( _pCodecCtx )
		{
			Release();
		}
		else
		{
			//�������е�codec��Ҳ��ֻ��һ��H264��codec���У���û�Թ�
			av_init_packet(&_avpkt);
			_avpkt.data = NULL;
			_avpkt.size = 0;
		}
	
		_hfnDecoder = hfnFNDecoder;
		_uSpeed = 1;
		//_uFps = pCodecCtx->time_base.den / pCodecCtx->time_base.num;

		//�õ�H264�Ľ������� 
		AVCodec* pCodec = NULL; 
		pCodec = avcodec_find_decoder(AV_CODEC_ID_MP3);
		if(NULL != pCodec)
		{
			//����һ��AVCodecContext������Ĭ��ֵ��ʼ���� 
			_pCodecCtx = avcodec_alloc_context3(pCodec); 
			if(!_pCodecCtx)
				return -1;

			/* ��ʼ��AVCodecParserContext */  
			_pCodecParserCtx = av_parser_init(AV_CODEC_ID_MP3);
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

		_AudioInfo.uBitRate = _pCodecCtx->bit_rate;
		_AudioInfo.uSampleRate = _pCodecCtx->sample_rate;
		_AudioInfo.uFormat = _pCodecCtx->sample_fmt;
		_AudioInfo.uChannels = _pCodecCtx->channels;
		_DecoderInfo.pInfo = (char*)&_AudioInfo;
		_DecoderInfo.uEncType = EN_TYPE_MP3;
		_DecoderInfo.iThreadType = _pCodecCtx->active_thread_type;
		_DecoderInfo.pCodecCtx = _pCodecCtx;

		_isOpen = true;

		_sBuf.resize(_AudioInfo.uChannels * _AudioInfo.uSampleRate / 8 * 2);
		return 1;
	}

	void MP3_Decoder::Close( void )
	{
		if( _pCodecCtx != NULL && _isOpen )
		{
			_isOpen = false;
			avcodec_close(_pCodecCtx);
		}
	}

	int MP3_Decoder::ReOpen( const HFNPrevOpen& hOpen )
	{
		if( _pCodecCtx == NULL )
			return -1;

		//�õ�H264�Ľ������� 
		AVCodec* pCodec = avcodec_find_decoder(AV_CODEC_ID_MP3); //�����

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

	void MP3_Decoder::Release( void )
	{
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

	int  MP3_Decoder::SetParamter( const HFNDecoder& hfnFNDecoder, bool isRealTime )
	{
		_hfnDecoder = hfnFNDecoder;
		_isRealTime = isRealTime;
		return 1;
	}

	int  MP3_Decoder::SetParamterRaw( const HFNDecoderRaw& hfnFNDecoder, bool isRealTime )
	{
		return 1;
	}

	int  MP3_Decoder::Decoder( unsigned char* szData, UInt32 uSize )
	{
		if (!_pCodecCtx || !_pavfFrame || !_pCodecParserCtx || !_isOpen)
			return -1;

		//decode
		int nGot = 0, iDataSize = 0;
		uint8_t *cur_ptr = szData;

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
				iLen = avcodec_decode_audio4(_pCodecCtx, _pavfFrame, \
					(int*)&nGot, &_avpkt);

				if (iLen < 0)
					break;

				_avpkt.data += iLen;
				_avpkt.size -= iLen;

				if (nGot > 0)
				{//�ɹ�����
					if (_hfnDecoder)
					{
						int iDataSize = av_samples_get_buffer_size(NULL, _AudioInfo.uChannels/*_pCodecCtx->channels*/,
							_pavfFrame->nb_samples, _pCodecCtx->sample_fmt, 1);

						if (iDataSize > 0)
						{
							//���������Ƶ����push����Ƶ������
							_sBuf.update_size(0);
							
							if(_pavfFrame->linesize[0] > 0)
								_sBuf.append((char*)_pavfFrame->data[0], _pavfFrame->linesize[0]);

							if (_pavfFrame->linesize[1] > 0)
								_sBuf.append((char*)_pavfFrame->data[1], _pavfFrame->linesize[1]);

							_hfnDecoder(_sBuf, 0, 0, 0);
						}
					}
				}
				else
				{

				}
			} while (_avpkt.size > 0);
		}

		return -1;
	}

	int  MP3_Decoder::DecoderRaw( unsigned char* szData, UInt32 uSize )
	{
		int nGot = 0;

		if (!_pCodecCtx || !_pavfFrame || !_pCodecParserCtx || !_isOpen)
			return -1;

		//decode
		uint8_t *cur_ptr = szData;

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
				iLen = avcodec_decode_video2(_pCodecCtx, _pavfFrame, \
					(int*)&nGot, &_avpkt);

				if (iLen < 0)
					break;

				_avpkt.data += iLen;
				_avpkt.size -= iLen;

				if (nGot > 0)
				{//�ɹ�����
					if (_hfnDecoderRaw)
					{//֡�ٶȼ���
						_hfnDecoderRaw(_pCodecCtx, _pavfFrame, 0);
					}
				}
				else
				{

				}
			} while (_avpkt.size > 0);
		}

		return -1;
	}

	void MP3_Decoder::SetSpeed( UInt8 uSpeed )
	{
		_uSpeed = max(1, uSpeed);
	}

	void MP3_Decoder::Reset( void )
	{
		_hfnDecoder.reset();
	}

} //namespace _ffmpeg_