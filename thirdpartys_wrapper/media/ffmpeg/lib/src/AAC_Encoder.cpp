
#include <libThirdWrap_FFmpeg/AAC_Encoder.h>

#define	AUDIO_FRAME_SIZE_DEF		1024

namespace _ffmpeg_ {

	AAC_Encoder::AAC_Encoder(void)
		: _swr_ctx(NULL)
		, _pCodecCtx(NULL)
		, _pCodec(NULL)
		, _pFrame(NULL)
		, _Frame_Buf(NULL)
		, _ConvertData(NULL)
		, _Frame_Size(0)
		, _Pts(0)
		, _BitRate(0)
	{
		_sData.reserve(AUDIO_FRAME_SIZE_DEF * 2 * 2 * 4);
	}

	AAC_Encoder::~AAC_Encoder(void)
	{
		Release();
	}

	int AAC_Encoder::Init(UInt32 uSampleRate, UInt8 uChannel, int64_t BitRate, const HFNEncoder& hEncoder)
	{
		int iRet = 0;
		AVCodecID codec_id = AV_CODEC_ID_AAC;
		_hEncoder = hEncoder;

		int profile = 2;    //AAC LC  
		int freqIdx = 4;	//默认 44.1KHz 

		switch (uSampleRate)
		{
		case 96000:
			freqIdx = 0;
			break;
		case 88200:
			freqIdx = 1;
			break;
		case 64000:
			freqIdx = 2;
			break;
		case 48000:
			freqIdx = 3;
			break;
		case 44100:
			freqIdx = 4;
			break;
		case 32000:
			freqIdx = 5;
			break;
		case 24000:
			freqIdx = 6;
			break;
		case 22050:
			freqIdx = 7;
			break;
		case 16000:
			freqIdx = 8;
			break;
		case 12000:
			freqIdx = 9;
			break;
		case 11025:
			freqIdx = 10;
			break;
		case 8000:
			freqIdx = 11;
			break;
		case 7350:
			freqIdx = 12;
			break;
		}

		int chanCfg = uChannel;	//MPEG-4 Audio Channel Configuration. 1 Channel front-center,channel_layout.h  
		_Padts[0] = (char)0xFF; // 11111111     = syncword  
		_Padts[1] = (char)0xF1; // 1111 1 00 1  = syncword MPEG-2 Layer CRC  
		_Padts[2] = (char)(((profile - 1) << 6) + (freqIdx << 2) + (chanCfg >> 2));
		_Padts[6] = (char)0xFC;

		/*_mp3padts[0] = (char)0xFF;
		_mp3padts[1] = (char)0xFB;
		_mp3padts[2] = (char)0x90;
		_mp3padts[3] = (char)0x04;*/

		_Pts = 0;
		_Frame_Size = 0;

		avcodec_register_all();

		_pCodec = avcodec_find_encoder(codec_id);
		if (!_pCodec)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Encoder::Init() Codec not found!");
			return -1;
		}

		_pCodecCtx = avcodec_alloc_context3(_pCodec);
		if (!_pCodecCtx)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Encoder::Init() Could not allocate audio codec context!");
			return -1;
		}

		_pCodecCtx->codec_id = codec_id;
		_pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
		_pCodecCtx->sample_fmt = _pCodec->sample_fmts[0];
		_pCodecCtx->sample_rate = uSampleRate;
		_pCodecCtx->channel_layout = uChannel == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
		_pCodecCtx->channels = av_get_channel_layout_nb_channels(_pCodecCtx->channel_layout);
		_pCodecCtx->frame_size = AUDIO_FRAME_SIZE_DEF;

		if ((iRet = avcodec_open2(_pCodecCtx, _pCodec, NULL)) < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Encoder::Init() avcodec_open2 error ----> %d", iRet);
			return -1;
		}

		_pCodecCtx->bit_rate = _BitRate;  //16000; //32000; //64000; //96000; //128000;
		_pFrame = av_frame_alloc();
		_pFrame->nb_samples = _pCodecCtx->frame_size;   //1024
		_pFrame->format = _pCodecCtx->sample_fmt;
		_pFrame->channels = uChannel;

		// 由AV_SAMPLE_FMT_S16转为AV_SAMPLE_FMT_FLTP
		_swr_ctx = swr_alloc_set_opts(
			NULL,
			av_get_default_channel_layout(_pCodecCtx->channels),
			_pCodecCtx->sample_fmt,
			_pCodecCtx->sample_rate,
			av_get_default_channel_layout(_pCodecCtx->channels),
			AV_SAMPLE_FMT_S16,										//PCM源文件的采样格式  
			_pCodecCtx->sample_rate,
			0, NULL);

		swr_init(_swr_ctx);

		// 分配空间
		_ConvertData = (uint8_t**)calloc(_pCodecCtx->channels, sizeof(*_ConvertData));

		av_samples_alloc(_ConvertData, NULL,
			_pCodecCtx->channels, _pCodecCtx->frame_size,
			_pCodecCtx->sample_fmt, 0);

		_Frame_Size = av_samples_get_buffer_size(NULL, _pCodecCtx->channels, _pCodecCtx->frame_size, _pCodecCtx->sample_fmt, 0);

		_Frame_Buf = (uint8_t *)av_malloc(_Frame_Size);
		//此时data[0],data[1]分别指向frame_buf数组起始、中间地址 
		iRet = avcodec_fill_audio_frame(_pFrame, _pCodecCtx->channels, _pCodecCtx->sample_fmt, (const uint8_t*)_Frame_Buf, _Frame_Size, 0);

		if (iRet < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Encoder::Init() avcodec_fill_audio_frame error!");
			return -1;
		}

		_BitRate = BitRate < 8000 ? 16000 : BitRate;
		return 1;
	}

	void AAC_Encoder::Release(void)
	{
		if (_pCodecCtx != NULL)
		{
			avcodec_close(_pCodecCtx);
			av_free(_pCodecCtx);
			_pCodecCtx = NULL;
		}

		if (_pFrame != NULL)
		{
			av_freep(&_pFrame->data[0]);
			av_frame_free(&_pFrame);
			_pFrame = NULL;
		}

		if (_swr_ctx != NULL)
		{
			swr_free(&_swr_ctx);
			_swr_ctx = NULL;
		}

		if (_ConvertData != NULL)
		{
			/*for (int i = 0; i < _pCodecCtx->channels; i++)
			{
			av_freep(&_ConvertData[i]);
			}*/

			av_freep(&_ConvertData[0]);
			free(_ConvertData);
			_ConvertData = NULL;
		}
	}

	int AAC_Encoder::Encoder(const char* c_szPcmData, UInt32 uSize, bool isHead)
	{
		//Encode
		AVPacket	pkt;
		int got_output = 0, iRet = 0;
		int iLen = 0, iRemain = 0;
		int ReadLen = AUDIO_FRAME_SIZE_DEF * _pCodecCtx->channels * 2;

		_sData.append(c_szPcmData, uSize);
		if (_sData.size() < ReadLen)
			return -1;

		iRemain = _sData.size();

		do
		{
			av_init_packet(&pkt);
			pkt.data = NULL;
			pkt.size = 0;

			iLen = ReadLen < iRemain ? ReadLen : iRemain;
			memcpy(_Frame_Buf, &_sData[_sData.size() - iRemain], iLen);

			/* 转换数据，令各自声道的音频数据存储在不同的数组（分别由不同指针指向）*/
			swr_convert(_swr_ctx, _ConvertData, _pCodecCtx->frame_size,
				(const uint8_t**)_pFrame->data, _pCodecCtx->frame_size);

			/* 将转换后的数据复制给pFrame */
			int length = _pCodecCtx->frame_size * av_get_bytes_per_sample(_pCodecCtx->sample_fmt);

			memcpy(_pFrame->data[0], _ConvertData[0], length);

			if(_pCodecCtx->channels == 2)
				memcpy(_pFrame->data[1], _ConvertData[1], length);

			_pFrame->pts =  _Pts++ * 100;

			iRet = avcodec_encode_audio2(_pCodecCtx, &pkt, _pFrame, &got_output);
			if (iRet < 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Encoder::Init() error encoding!");
				return -1;
			}

			if (pkt.data == NULL)
			{
				av_free_packet(&pkt);
				goto gt_end;
			}

			if (got_output)
			{
				if (isHead)
				{
					_Padts[3] = (char)(((_pCodecCtx->channels & 3) << 6) + ((7 + pkt.size) >> 11));
					_Padts[4] = (char)(((7 + pkt.size) & 0x7FF) >> 3);
					_Padts[5] = (char)((((7 + pkt.size) & 7) << 5) + 0x1F);
				}

				if (_hEncoder)
				{
					if (isHead)
						_hEncoder((UInt8*)_Padts, 7);

					_hEncoder(pkt.data, pkt.size);
				}
			}
gt_end:
			iRemain -= iLen;
			av_free_packet(&pkt);
		} while (iRemain >= ReadLen);


		std::memmove(&_sData[0], &_sData[_sData.size() - iRemain], iRemain);
		_sData.update_size(iRemain);

		/*do
		{
			iRet = avcodec_encode_audio2(_pCodecCtx, &pkt, NULL, &got_output);
			if (iRet < 0) {
				printf("Error encoding frame\n");
				return -1;
			}

			if (got_output)
			{
				_Padts[3] = (char)(((_pCodecCtx->channels & 3) << 6) + ((7 + pkt.size) >> 11));
				_Padts[4] = (char)(((7 + pkt.size) & 0x7FF) >> 3);
				_Padts[5] = (char)((((7 + pkt.size) & 7) << 5) + 0x1F);

				if (_hEncoder)
				{
					if (isHead)
					{
						_hEncoder((UInt8*)_Padts, 7);
						//_hEncoder((UInt8*)_mp3padts, 4);
					}

					_hEncoder(pkt.data, pkt.size);
				}
				av_free_packet(&pkt);
			}
		} while (got_output);*/

		return 1;
	}

} //namespace _ffmpeg_

