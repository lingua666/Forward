
#include <libThirdWrap_FFmpeg/AAC_Decoder.h>

#define	AUDIO_FRAME_SIZE_DEF		1024

namespace _ffmpeg_ {

	AAC_Decoder::AAC_Decoder(void)
		: _swr_ctx(NULL)
		, _pCodecCtx(NULL)
		, _pCodec(NULL)
		, _pParser(NULL)
		, _pFrame(NULL)
		, _Frame_Size(0)
	{
		_sData.reserve(AUDIO_FRAME_SIZE_DEF * 2 * 2 * 4);
	}

	AAC_Decoder::~AAC_Decoder(void)
	{
		Release();
	}

	int AAC_Decoder::Init(UInt32 uSampleRate, UInt8 uChannel, const HFNDecoder& hDecoder)
	{
		int iRet = 0;
		AVCodecID codec_id = AV_CODEC_ID_AAC;
		_hDecoder = hDecoder;

		_Frame_Size = 0;

		avcodec_register_all();

		_pCodec = avcodec_find_decoder(codec_id);
		if (!_pCodec)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Init() Codec not found!");
			return -1;
		}

		//���ݽ�����ID��ȡ�����Ľ�����
		_pParser = av_parser_init(codec_id);
		if (!_pParser)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Init() Parser not found!\n");
			return 0;
		}

		_pCodecCtx = avcodec_alloc_context3(_pCodec);
		if (!_pCodecCtx)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Init() Could not allocate audio codec context!");
			return -1;
		}

		if (avcodec_open2(_pCodecCtx, _pCodec, NULL) < 0)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Init() avcodec_open2 Failed!");
			return -1;
		}

		_pFrame = av_frame_alloc();
		if (_pFrame == NULL)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "AAC_Decoder::Init() av_frame_alloc Failed!");
			return -1;
		}

		return 1;
	}

	void AAC_Decoder::Release(void)
	{
		if (_pCodecCtx != NULL)
		{
			avcodec_close(_pCodecCtx);
			av_free(_pCodecCtx);
			_pCodecCtx = NULL;
		}

		if (_pParser != NULL)
		{
			av_parser_close(_pParser);
			_pParser = NULL;
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

		if (_Frame_Buf != NULL)
		{
			av_free(_Frame_Buf);
			_Frame_Buf = NULL;
		}
	}

	int AAC_Decoder::Decoder(const char* c_szData, UInt32 uSize)
	{
		//Encode
		AVPacket	pkt;
		int got_output = 0, iRet = 0, iDecLen;
		int iLen = 0, iRemain = 0;
		int ReadLen = AUDIO_FRAME_SIZE_DEF * 2;

		_sData.append(c_szData, uSize);
		if (_sData.size() < ReadLen)
			return -1;

		iRemain = _sData.size();

		do
		{
			av_init_packet(&pkt);
			pkt.data = NULL;
			pkt.size = 0;

			iLen = ReadLen < iRemain ? ReadLen : iRemain;

			//��ȡ���뵽avcodec_send_packetһ��packet��������
			//��һ֡��������Ҳ���֡���������������һ֡������
			iDecLen = av_parser_parse2(_pParser, _pCodecCtx, &pkt.data, &pkt.size,
				(uint8_t*)&_sData[_sData.size() - iRemain], iLen,
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
					//����Ҫ�����packet
					iRet = avcodec_send_packet(_pCodecCtx, &pkt);
					//AVERROR(EAGAIN) ����ʧ�ܣ���ʾ��Ҫreceive frame������send packet
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
						//����avcodec_receive_frame�����ڲ����ȵ���av_frame_unref���ͷ�frame����������
						//������ε��ûὫ�ϴε��÷��ص�frame�����ͷ�
						iRet = avcodec_receive_frame(_pCodecCtx, _pFrame);
						if (iRet == AVERROR(EAGAIN) || iRet == AVERROR_EOF)
						{
							break;
						}
						else if (iRet < 0)
						{
							//fprintf(stderr, "avcodec_receive_frame = ret < 0\n");
							break;
						}

						if (_swr_ctx == NULL)
						{
							//�ڵ�һ֡��ʱ�����һ����Ƶ��Ϣ
							printf("ar-samplerate: %uHz\n", _pFrame->sample_rate);
							printf("ac-channel: %u\n", _pFrame->channels);
							printf("f-format: %u\n", _pFrame->format);

							// ��AV_SAMPLE_FMT_S16תΪAV_SAMPLE_FMT_FLTP
							_swr_ctx = swr_alloc_set_opts(
								NULL,
								av_get_default_channel_layout(_pCodecCtx->channels),
								AV_SAMPLE_FMT_S16,
								_pCodecCtx->sample_rate,
								av_get_default_channel_layout(_pCodecCtx->channels),
								_pCodecCtx->sample_fmt,										//PCMԴ�ļ��Ĳ�����ʽ  
								_pCodecCtx->sample_rate,
								0, NULL);

							swr_init(_swr_ctx);

							//����buffer
							_Frame_Size = av_samples_get_buffer_size(NULL, _pFrame->channels, _pFrame->nb_samples, AV_SAMPLE_FMT_S16, 1);

							//ע��Ҫ��av_malloc
							_Frame_Buf = (uint8_t *)av_malloc(_Frame_Size);
						}

						swr_convert(_swr_ctx, &_Frame_Buf, _Frame_Size, (const uint8_t**)&_pFrame->data[0], _pFrame->nb_samples);

						if (_hDecoder)
						{
							_hDecoder(_Frame_Buf, _Frame_Size);
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

		if(iRemain > 0)
			std::memmove(&_sData[0], &_sData[_sData.size() - iRemain], iRemain);

		_sData.update_size(iRemain);
		return 1;
	}

} //namespace _ffmpeg_

