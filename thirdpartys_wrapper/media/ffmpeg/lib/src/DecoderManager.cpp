
#include <libThirdWrap_FFmpeg/DecoderManager.h>
#include <libThirdWrap_FFmpeg/DecoderFactory.h>

namespace _ffmpeg_{

	//注册ffmpeg_read_data的回调函数，这里只是最简处理，实际应用中应加上出错处理，超时等待...
	static int ffmpeg_read_data(void *opaque, uint8_t *buf, int buf_size)  
	{  
		return buf_size;
	}

	int	GetCodecContextToData( unsigned char* c_szData,
							unsigned long uSize,
							AVCodecContext** out_pCodecCtx )
	{
		int iRet = -1;
		AVFormatContext* pFormatCtx = NULL; 
		AVInputFormat* piFmt = NULL; 
		AVIOContext* pb = NULL; 

		uint8_t* buf = (uint8_t*)av_malloc(sizeof(uint8_t)* uSize);
		memcpy(buf, c_szData, uSize);

		pb = avio_alloc_context(buf, uSize, 0, NULL, ffmpeg_read_data, NULL, NULL);
		if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)
		{  
			//fprintf(stderr, "probe format failed\n");
			iRet = -1; 
			goto gt_end;
		}  
		else
		{  
			//fprintf(stdout, "format:%s[%s]\n", piFmt->name, piFmt->long_name);
		}

		pFormatCtx = avformat_alloc_context();
		pFormatCtx->pb = pb;
		pFormatCtx->pb->read_packet = 0;

		if (avformat_open_input(&pFormatCtx, "", piFmt, NULL) != 0)
		{//iformat，priv_data赋值，pb, nbstreams,streams为null  
			//printf("Couldn't open input stream.（无法打开输入流）\n");  
			iRet = -1;
			goto gt_end;
		}  
		if (avformat_find_stream_info(pFormatCtx, NULL) < 0)//nbstreams,streams赋值， pb还是为null  
		{  
			//printf("Couldn't find stream information.（无法获取流信息）\n");  
			iRet = -1;
			goto gt_end;
		}

		for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
		{
			if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO
				|| pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				if( *out_pCodecCtx == NULL )
				{
					AVCodec* pCodec = avcodec_find_decoder((AVCodecID)pFormatCtx->streams[i]->codec->codec_id);
					if(NULL == pCodec)
					{
						iRet = -2;
						goto gt_end;
					}

					*out_pCodecCtx = avcodec_alloc_context3(pCodec);
					if( *out_pCodecCtx == NULL )
					{
						iRet = -3;
						avcodec_free_context(out_pCodecCtx);
						out_pCodecCtx = NULL;
						goto gt_end;
					}
				}

				avcodec_copy_context(*out_pCodecCtx, pFormatCtx->streams[i]->codec);
				(*out_pCodecCtx)->time_base.num = pFormatCtx->streams[i]->r_frame_rate.den;
				(*out_pCodecCtx)->time_base.den = pFormatCtx->streams[i]->r_frame_rate.num;

				switch( (*out_pCodecCtx)->codec_id )
				{
				case AV_CODEC_ID_H264:
					{
						iRet = EN_TYPE_H264;
						if( strcmp(piFmt->name,"flv") == 0 )
						{
							iRet = EN_TYPE_AVC;
						}
					}
					break;
				case AV_CODEC_ID_H265:
					{
						iRet = EN_TYPE_H265;
					}
					break;
				case AV_CODEC_ID_VP6F:
					{
						iRet = EN_TYPE_VP6F;
					}
					break;
				case AV_CODEC_ID_MP3:
					{
						iRet = EN_TYPE_MP3;
					}
					break;
				case AV_CODEC_ID_AAC:
					{
						iRet = EN_TYPE_AAC;
					}
					break;
				}

				break;
			}
		}

gt_end:
		if( pFormatCtx != NULL )
		{
			avformat_close_input(&pFormatCtx);
			avformat_free_context(pFormatCtx);
			pFormatCtx = NULL;
		}

		if( pb != NULL )
		{
			if( pb->buffer != NULL )
				av_free(pb->buffer);

			av_free(pb);
			pb = NULL;
		}

		return iRet;
	}

	struct tagffmpegBuf
	{
		uint8_t* buf;
		int	size;
		int offset;
	};

	static MemPool_type* GetffmpegBufPool( void )
	{
		static MemPool_type Pool( sizeof(tagffmpegBuf) );
		return &Pool;
	}

	//注册ffmpeg_read_data的回调函数，这里只是最简处理，实际应用中应加上出错处理，超时等待...
	static int ffmpeg_read_data_v20(void *opaque, uint8_t *buf, int buf_size)  
	{  
		tagffmpegBuf* pffmpegBuf = (tagffmpegBuf*)opaque;
		if( buf_size < 0 )
			return -1;

		buf_size = __min(buf_size, pffmpegBuf->size - pffmpegBuf->offset);

		if (!buf_size)
			return AVERROR_EOF;

		/* copy internal buffer data to buf */
		memcpy(buf, &pffmpegBuf->buf[pffmpegBuf->offset], buf_size);
		pffmpegBuf->offset += buf_size;

		return buf_size;
	}

	int	GetCodecContextToData_V20( unsigned char* c_szData,
		unsigned long uSize, const _string_type& sEncType,
		AVCodecContext** out_pCodecCtx )
	{
		int iRet = -1;
		AVIOContext* pb = NULL; 
		AVFormatContext* pFormatCtx = NULL; 
		AVInputFormat* piFmt = av_find_input_format(sEncType.c_str());
		if( piFmt == NULL )
			return -1;

		tagffmpegBuf* pffmpegBuf = (tagffmpegBuf*)GetffmpegBufPool()->Alloc();

		uint8_t* buf = (uint8_t*)av_malloc(sizeof(uint8_t)* uSize);
		pffmpegBuf->buf = (uint8_t*)av_malloc(sizeof(uint8_t)* uSize);
		pffmpegBuf->size = uSize;
		pffmpegBuf->offset = 0;
		memcpy(pffmpegBuf->buf, c_szData, uSize);

		pb = avio_alloc_context(buf, uSize, 0, pffmpegBuf, ffmpeg_read_data_v20, NULL, NULL);
		pFormatCtx = avformat_alloc_context();
		pFormatCtx->pb = pb;
		//pFormatCtx->pb->read_packet = 0;

		if (avformat_open_input(&pFormatCtx, "", piFmt, NULL) != 0)
		{//iformat，priv_data赋值，pb, nbstreams,streams为null  
			//printf("Couldn't open input stream.（无法打开输入流）\n");  
			iRet = -1;
			goto gt_end;
		}  
		if (avformat_find_stream_info(pFormatCtx, NULL) < 0)//nbstreams,streams赋值， pb还是为null  
		{  
			//printf("Couldn't find stream information.（无法获取流信息）\n");  
			iRet = -1;
			goto gt_end;
		}

		for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
		{
			if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				if( pFormatCtx->streams[i]->codec->width == 0 ||
					pFormatCtx->streams[i]->codec->height == 0 )
				{
					iRet = -1;
					goto gt_end;
				}

				if( *out_pCodecCtx == NULL )
				{
					AVCodec* pCodec = avcodec_find_decoder((AVCodecID)pFormatCtx->streams[i]->codec->codec_id);
					if(NULL == pCodec)
					{
						iRet = -2;
						goto gt_end;
					}

					*out_pCodecCtx = avcodec_alloc_context3(pCodec);
					if( *out_pCodecCtx == NULL )
					{
						iRet = -3;
						avcodec_free_context(out_pCodecCtx);
						out_pCodecCtx = NULL;
						goto gt_end;
					}
				}

				avcodec_copy_context(*out_pCodecCtx, pFormatCtx->streams[i]->codec);
				(*out_pCodecCtx)->time_base.num = pFormatCtx->streams[i]->r_frame_rate.den;
				(*out_pCodecCtx)->time_base.den = pFormatCtx->streams[i]->r_frame_rate.num;

				switch( (*out_pCodecCtx)->codec_id )
				{
				case AV_CODEC_ID_H264:
					{
						iRet = EN_TYPE_H264;
						if( strcmp(piFmt->name,"flv") == 0 )
						{
							iRet = EN_TYPE_AVC;
						}
					}
					break;
				case AV_CODEC_ID_H265:
					{
						iRet = EN_TYPE_H265;
					}
					break;
				case AV_CODEC_ID_VP6F:
					{
						iRet = EN_TYPE_VP6F;
					}
					break;
				}

				break;
			}
		}

gt_end:
		if( pFormatCtx != NULL )
		{
			avformat_close_input(&pFormatCtx);
			avformat_free_context(pFormatCtx);
			pFormatCtx = NULL;
		}

		if( pb != NULL )
		{
			if( pb->buffer != NULL )
				av_free(pb->buffer);

			av_free(pb);
			pb = NULL;
		}

		if( pffmpegBuf != NULL )
		{
			if( pffmpegBuf->buf != NULL )
			{
				av_free(pffmpegBuf->buf);
				pffmpegBuf->buf = NULL;
			}

			GetffmpegBufPool()->Free(pffmpegBuf);
		}

		return iRet;
	}

	DecoderManager::DecoderManager( void )
	{
		//test
		LOG_Print_SetLog(libThirdWrap_FFmpeg, MLog_GetAllLevel())
		//test
	}

	DecoderManager::~DecoderManager( void )
	{

	}

	HDecoder	DecoderManager::GetDecoder( long lWidth, long lHeight,
											int iFrameRate,
											enDecoderType enType,
											const HFNDecoder& hfnFNDecoder,
											const HFNPrevOpen& hfnPrev )
	{
		if( EN_TYPE_COUNT <= enType )
			return 0;

		IDecoder* pDec = NULL;
		pDec = DecoderFactory_GetInstance()->CreateRender(enType);
		if( pDec->Init(lWidth, lHeight, iFrameRate, hfnFNDecoder, hfnPrev) <= 0 )
		{
			Destroy(pDec);
			return 0;
		}
		return reinterpret_cast<HDecoder>(pDec); 
	}

	//数据长度最小为2048
	HDecoder	DecoderManager::GetDecoder( const char* c_szData,
										UInt32	uSize,
										const HFNDecoder& hfnFNDecoder,
										const HFNPrevOpen& hfnPrev )
	{
		if( uSize < FFMPEG_PROBE_BUF_MIN )
			return 0;

		IDecoder* pDec = NULL;
		AVCodecContext* pCodecCtx = NULL;
		int iType = GetCodecContextToData((unsigned char*)c_szData, uSize, &pCodecCtx);
		if( iType >= 0 && iType < EN_TYPE_COUNT )
		{
			pDec = DecoderFactory_GetInstance()->CreateRender((enDecoderType)iType);

			int iRet = pDec->Init(pCodecCtx, hfnFNDecoder, hfnPrev);
			avcodec_close(pCodecCtx);
			av_free(pCodecCtx);
			if( iRet <= 0 )
			{
				Destroy(pDec);
				return 0;
			}
			return reinterpret_cast<HDecoder>(pDec);
		}

		return 0;
	}

	HDecoder  DecoderManager::GetDecoder( AVCodecContext* pCodecCtx,
		const HFNDecoder& hfnFNDecoder,
		const HFNPrevOpen& hfnPrev )
	{
		if( pCodecCtx == NULL )
			return NULL;

		IDecoder* pDec = NULL;
		int iType = -1;

		switch( pCodecCtx->codec_id )
		{
		case AV_CODEC_ID_H264:
			{
				iType = EN_TYPE_H264;
			}
			break;
		case AV_CODEC_ID_H265:
			{
				iType = EN_TYPE_H265;
			}
			break;
		case AV_CODEC_ID_VP6F:
			{
				iType = EN_TYPE_VP6F;
			}
			break;
		}

		if( iType >= 0 && iType < EN_TYPE_COUNT )
		{
			pDec = DecoderFactory_GetInstance()->CreateRender((enDecoderType)iType);

			int iRet = pDec->Init(pCodecCtx, hfnFNDecoder, hfnPrev);
			if( iRet <= 0 )
			{
				Destroy(pDec);
				return 0;
			}
			return reinterpret_cast<HDecoder>(pDec);
		}
	}

	//数据长度最小为2048
	int	DecoderManager::GetCodecCtx( const char* c_szData, UInt32 uSize, AVCodecContext** out_pCodecCtx )
	{
		if( uSize < FFMPEG_PROBE_BUF_MIN )
			return 0;

		IDecoder* pDec = NULL;
		int iType = GetCodecContextToData((unsigned char*)c_szData, uSize, out_pCodecCtx);
		if( iType >= 0 && iType < EN_TYPE_COUNT )
		{
			return iType;
		}

		return -1;
	}

	//数据长度最小为2048
	int	DecoderManager::GetCodecCtx_V20( const char* c_szData, UInt32 uSize,
		const _string_type& sEncType, AVCodecContext** out_pCodecCtx )
	{
		if( uSize < FFMPEG_PROBE_BUF_MIN )
			return 0;

		IDecoder* pDec = NULL;
		int iType = GetCodecContextToData_V20((unsigned char*)c_szData, uSize, sEncType, out_pCodecCtx);
		if( iType >= 0 && iType < EN_TYPE_COUNT )
		{
			return iType;
		}

		return -1;
	}

	void DecoderManager::Close( HDecoder hDec )
	{
		reinterpret_cast<IDecoder*>(hDec)->Close();
		DecoderFactory::FreeDecoder(reinterpret_cast<IDecoder*>(hDec));
	}

	int DecoderManager::ReOpen( HDecoder hDec, const HFNPrevOpen& hfnPrev )
	{
		return reinterpret_cast<IDecoder*>(hDec)->ReOpen(hfnPrev);
	}

	int  DecoderManager::Decoder( HDecoder hDec, unsigned char* szData, UInt32 uSize )
	{
		if( hDec == 0 )
			return 0;

		reinterpret_cast<IDecoder*>(hDec)->Decoder(szData, uSize);
		return 1;
	}

	int  DecoderManager::DecoderRaw( HDecoder hDec, unsigned char* szData, UInt32 uSize )
	{
		if( hDec == 0 )
			return 0;

		reinterpret_cast<IDecoder*>(hDec)->DecoderRaw(szData, uSize);
		return 1;
	}

	void DecoderManager::Destroy( HDecoder hDec )
	{
		if( hDec != 0 )
			delete reinterpret_cast<IDecoder*>(hDec);
	}

	int DecoderManager::SetParamter( HDecoder hDec, const HFNDecoder& hfnFNDecoder, bool isRealTime )
	{
		if( hDec == 0 )
			return 0;

		return reinterpret_cast<IDecoder*>(hDec)->SetParamter(hfnFNDecoder, isRealTime);
	}

	int  DecoderManager::SetParamterRaw( HDecoder hDec, const HFNDecoderRaw& hfnFNDecoder, bool isRealTime )
	{
		if( hDec == 0 )
			return 0;

		//test
		IDecoder* p = reinterpret_cast<IDecoder*>(hDec);
		//test

		return reinterpret_cast<IDecoder*>(hDec)->SetParamterRaw(hfnFNDecoder, isRealTime);
	}

	IDecoder::tagDecoder_INFO* DecoderManager::GetDecoderInfo( HDecoder hDec )
	{
		if( hDec == 0 )
			return 0;

		return reinterpret_cast<IDecoder*>(hDec)->GetDecoderInfo();
	}

	void DecoderManager::SetSpeed( HDecoder hDec, UInt8 uSpeed )
	{
		if( hDec != 0 )
			reinterpret_cast<IDecoder*>(hDec)->SetSpeed(uSpeed);
	}

	void DecoderManager::Reset( HDecoder hDec )
	{
		if( hDec == 0 )
			return ;

		return reinterpret_cast<IDecoder*>(hDec)->Reset();
	}

} //namespace _ffmpeg_
