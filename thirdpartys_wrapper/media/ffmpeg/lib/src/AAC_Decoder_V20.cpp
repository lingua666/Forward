
#include <libThirdWrap_FFmpeg/AAC_Decoder_V20.h>

#define	AUDIO_FRAME_SIZE_DEF		1024

namespace _ffmpeg_ {

	AAC_Decoder_V20::AAC_Decoder_V20(void)
		: _pCodecCtx(NULL)
		, _pavfFrame(NULL)
		, _pCodecParserCtx(NULL)
		, _uSpeed(1)
		, _SwrCtx(NULL)
	{
		_isOpen = false;
		_isRealTime = false;
	}

	AAC_Decoder_V20::~AAC_Decoder_V20(void)
	{
		Release();
	}

	int  AAC_Decoder_V20::Init(long lWidth, long lHeight,
		UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder, const HFNPrevOpen& hfnPrev)
	{
		return -1;
	}

	int  AAC_Decoder_V20::Init(AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
		const HFNPrevOpen& hfnPrev)
	{
		AVCodecID codec_id = AV_CODEC_ID_AAC;
		if (pCodecCtx == NULL)
			return -1;

		//已经打开
		if (_pCodecCtx)
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
		_uSpeed = 1;
		//_uFps = pCodecCtx->time_base.den / pCodecCtx->time_base.num;

		//得到H264的解码器： 
		AVCodec* pCodec = NULL;
		pCodec = avcodec_find_decoder(codec_id);
		if (NULL != pCodec)
		{
			//创建一个AVCodecContext，并用默认值初始化： 
			_pCodecCtx = avcodec_alloc_context3(pCodec);
			if (!_pCodecCtx)
				return -1;

			/* 初始化AVCodecParserContext */
			_pCodecParserCtx = av_parser_init(codec_id);
			if (!_pCodecParserCtx)
			{
				Release();
				return -1;
			}

			avcodec_copy_context(_pCodecCtx, pCodecCtx);

			if (hfnPrev)
			{
				hfnPrev((HDecoder)this, _pCodecCtx);
			}

			if (avcodec_open2(_pCodecCtx, pCodec, NULL) < 0)
			{
				Release();
				return -1;
			}

			_pavfFrame = av_frame_alloc();// Allocate video frame 
			if (_pavfFrame == NULL)
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

	void AAC_Decoder_V20::Close(void)
	{
		if (_pCodecCtx != NULL && _isOpen)
		{
			_isOpen = false;
			avcodec_close(_pCodecCtx);
		}
	}

	int AAC_Decoder_V20::ReOpen(const HFNPrevOpen& hOpen)
	{
		if (_pCodecCtx == NULL)
			return -1;

		//得到H264的解码器： 
		AVCodec* pCodec = avcodec_find_decoder(AV_CODEC_ID_AAC); //软解码

		if (avcodec_close(_pCodecCtx) < 0)
			return -1;

		if (_pCodecCtx->hwaccel_context != NULL)
		{
			av_freep(&_pCodecCtx->hwaccel_context);
			_pCodecCtx->hwaccel_context = NULL;
		}

		if (hOpen)
		{
			hOpen((HDecoder)this, _pCodecCtx);
		}

		if (avcodec_open2(_pCodecCtx, pCodec, NULL) < 0)
		{
			Release();
			return -1;
		}

		_isOpen = true;

		return 1;
	}

	void AAC_Decoder_V20::Release(void)
	{
		if (_pavfFrame != NULL)
		{
			av_free(_pavfFrame);
			_pavfFrame = NULL;
		}

		if (_pCodecParserCtx != NULL)
		{
			av_parser_close(_pCodecParserCtx);
			_pCodecParserCtx = NULL;
		}

		if (_pCodecCtx != NULL)
		{
			if (_pCodecCtx->hwaccel_context != NULL)
			{
				av_freep(&_pCodecCtx->hwaccel_context);
				_pCodecCtx->hwaccel_context = NULL;
			}

			av_packet_unref(&_avpkt);
			avcodec_close(_pCodecCtx);
			av_free(_pCodecCtx);
			_pCodecCtx = NULL;
		}

		if (_SwrCtx != NULL)
		{
			swr_free(&_SwrCtx);
			_SwrCtx = NULL;
		}
	}

	int  AAC_Decoder_V20::SetParamter(const HFNDecoder& hfnFNDecoder, bool isRealTime)
	{
		_hfnDecoder = hfnFNDecoder;
		_isRealTime = isRealTime;
		return 1;
	}

	int  AAC_Decoder_V20::SetParamterRaw(const HFNDecoderRaw& hfnFNDecoder, bool isRealTime)
	{
		return 1;
	}

	int  AAC_Decoder_V20::Decoder(unsigned char* szData, UInt32 uSize)
	{
		//Encode
		AVPacket	pkt;
		int got_output = 0, iRet = 0, iDecLen;
		int iLen = 0, iRemain = 0;
		int ReadLen = AUDIO_FRAME_SIZE_DEF * 2;

		_sBuf.append((char*)szData, uSize);
		if (_sBuf.size() < ReadLen)
			return -1;

		iRemain = _sBuf.size();

		do
		{
			av_init_packet(&pkt);
			pkt.data = NULL;
			pkt.size = 0;

			iLen = ReadLen < iRemain ? ReadLen : iRemain;

			//获取传入到avcodec_send_packet一个packet的数据量
			//（一帧的量可能也会多帧的量，这里测试是一帧的量）
			iDecLen = av_parser_parse2(_pCodecParserCtx, _pCodecCtx, &pkt.data, &pkt.size,
				(uint8_t*)&_sBuf[_sBuf.size() - iRemain], iLen,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
			if (iDecLen < 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Decoder() av_parser_parser2 Error!");
				return -1;
			}

			if (pkt.size > 0)
			{
				int send_ret = 1;

				do
				{
					//传入要解码的packet
					iRet = avcodec_send_packet(_pCodecCtx, &pkt);
					//AVERROR(EAGAIN) 传入失败，表示先要receive frame再重新send packet
					if (iRet == AVERROR(EAGAIN))
					{
						send_ret = 0;
						//fprintf(stderr, "avcodec_send_packet = AVERROR(EAGAIN)\n");
					}
					else if (iRet < 0)
					{
						char err[128] = { 0 };
						av_strerror(iRet, err, 128);
						LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Decoder() avcodec_send_packet = ret < 0 : %s\n", err);
						break;
					}

					while (iRet >= 0)
					{
						//调用avcodec_receive_frame会在内部首先调用av_frame_unref来释放frame本来的数据
						//就是这次调用会将上次调用返回的frame数据释放
						iRet = avcodec_receive_frame(_pCodecCtx, _pavfFrame);
						if (iRet == AVERROR(EAGAIN) || iRet == AVERROR_EOF)
						{
							break;
						}
						else if (iRet < 0)
						{
							//fprintf(stderr, "avcodec_receive_frame = ret < 0\n");
							break;
						}

						if (_SwrCtx == NULL)
						{
							//在第一帧的时候输出一下音频信息
							printf("ar-samplerate: %uHz\n", _pavfFrame->sample_rate);
							printf("ac-channel: %u\n", _pavfFrame->channels);
							printf("f-format: %u\n", _pavfFrame->format);

							// 由AV_SAMPLE_FMT_S16转为AV_SAMPLE_FMT_FLTP
							_SwrCtx = swr_alloc_set_opts(
								NULL,
								av_get_default_channel_layout(_pCodecCtx->channels),
								AV_SAMPLE_FMT_S16,
								_pCodecCtx->sample_rate,
								av_get_default_channel_layout(_pCodecCtx->channels),
								_pCodecCtx->sample_fmt,										//PCM源文件的采样格式  
								_pCodecCtx->sample_rate,
								0, NULL);

							swr_init(_SwrCtx);

							//创建buffer
							int FrameSize = av_samples_get_buffer_size(NULL, _pavfFrame->channels, _pavfFrame->nb_samples, AV_SAMPLE_FMT_S16, 1);

							_sFrameBuf.resize(FrameSize);
						}

						uint8_t* Ptr = (uint8_t*)&_sFrameBuf[0];
						swr_convert(_SwrCtx, &Ptr, _sFrameBuf.size(), (const uint8_t**)&_pavfFrame->data[0], _pavfFrame->nb_samples);

						if (_hfnDecoder)
						{
							_hfnDecoder(_sFrameBuf, 0,0,0);
						}
					}

				} while (!send_ret);
			}

			if (pkt.data == NULL)
			{
				av_free_packet(&pkt);
				goto gt_end;
			}
		gt_end:
			iRemain -= iDecLen;
			av_free_packet(&pkt);
		} while (iRemain >= ReadLen);

		if (iRemain > 0)
			std::memmove(&_sBuf[0], &_sBuf[_sBuf.size() - iRemain], iRemain);

			_sBuf.update_size(iRemain);
		return 1;
	}

	int  AAC_Decoder_V20::DecoderRaw(unsigned char* szData, UInt32 uSize)
	{
		return -1;
	}

	void AAC_Decoder_V20::SetSpeed(UInt8 uSpeed)
	{
		_uSpeed = max(1, uSpeed);
	}

	void AAC_Decoder_V20::Reset(void)
	{
		_hfnDecoder.reset();
	}

} //namespace _ffmpeg_

