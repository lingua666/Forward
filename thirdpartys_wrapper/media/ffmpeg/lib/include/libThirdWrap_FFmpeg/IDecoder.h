
#ifndef __IDECODER_20170111090004_1444199361_H__
#define __IDECODER_20170111090004_1444199361_H__

#include "External.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
	#include <libavutil/avutil.h>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
	#include <libavformat/avio.h>
	#include <libavutil/log.h>
	#include <libavcodec/dxva2.h>
	#include <libavutil/opt.h>
};

#define PI 3.14159265358979323846

#define FFMPEG_PROBE_BUF_MAX (1 << 20)
#define FFMPEG_PROBE_BUF_MIN 2048

#ifdef HAVE_AV_CONFIG_H
	#undef HAVE_AV_CONFIG_H
#endif

#ifndef __STDC_CONSTANT_MACROS
	#define __STDC_CONSTANT_MACROS
#endif

namespace _ffmpeg_{

	enum enDecoderType
	{
		EN_TYPE_H264,
		EN_TYPE_H265,
		EN_TYPE_VP6F,
		EN_TYPE_AVC,
		EN_TYPE_MP3,
		EN_TYPE_AAC,
		EN_TYPE_COUNT
	};

	typedef void*	HDecoder;

	class IDecoder
	{
	public:
		typedef SmartPTR<char>	Buf_sptr;
		typedef	function20_handle<void (HDecoder, AVCodecContext*)>				HFNPrevOpen;
		typedef	function20_handle<void (const _string_type&, int, int, int)>	HFNDecoder;
		typedef function20_handle<void (AVCodecContext*, AVFrame*, int)>		HFNDecoderRaw;

#pragma pack(push, 1)
		typedef struct tagVideo_INFO
		{
			UInt16 uWidth;			//图像宽度
			UInt16 uHeight;			//图像高度
			UInt8 uFormat;			//图像内容格式
			UInt8 uFrameRate;		//帧率	
		}tagVideo_INFO;

		typedef struct tagAudio_INFO
		{
			UInt8 uChannels;		//通道数
			UInt8 uFormat;			//采样位数
			UInt16 uSampleRate;		//样本率
			UInt16 uBitRate;		//码率		
		}tagAudio_INFO;

		struct tagDecoder_INFO
		{
			AVCodecContext*			pCodecCtx;
			UInt8 uEncType;			//编码格式
			int	  iThreadType;		//线程格式
			char* pInfo;			//信息内容结构体
		};
#pragma pack(pop)

	public:
		virtual ~IDecoder( void )
		{}

		virtual int  Init( long lWidth, long lHeight,
			UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder = HFNDecoder(),
			const HFNPrevOpen& hfnPrev = HFNPrevOpen() ) = 0;

		virtual int  Init( AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder = HFNDecoder(),
			const HFNPrevOpen& hfnPrev = HFNPrevOpen() ) = 0;

		virtual int  SetParamter( const HFNDecoder& hfnFNDecoder, bool isRealTime ) = 0;

		virtual int  SetParamterRaw( const HFNDecoderRaw& hfnFNDecoder, bool isRealTime ) = 0;

		virtual void Release( void ) = 0;

		virtual void Close( void ) = 0;

		virtual int ReOpen( const HFNPrevOpen& hOpen ) = 0;

		virtual int Decoder( unsigned char* szData, UInt32 uSize ) = 0;

		virtual int DecoderRaw( unsigned char* szData, UInt32 uSize ) = 0;
		
		virtual void SetSpeed( UInt8 uSpeed ) = 0;

		virtual void Reset( void ) = 0;

		inline tagDecoder_INFO* GetDecoderInfo( void )
		{
			return &_DecoderInfo;
		}

		inline int YUV420PToYUVBuffer( AVCodecContext* pCodecCtx, AVFrame * pavfFrame, _string_type& sBuf )
		{
			if( pCodecCtx == NULL || pavfFrame == NULL ||
				(pCodecCtx->height != pCodecCtx->coded_height || pCodecCtx->width != pCodecCtx->coded_width) )
			return -1;

			//pack YUV data
			int i = 0,offset = 0;
			for(i = 0; i < pCodecCtx->height; i ++)
			{//Y
				memcpy(&sBuf[offset], pavfFrame->data[0] + i * pavfFrame->linesize[0], pCodecCtx->width); 
				offset += pCodecCtx->width;
			}

			for(i = 0; i < pCodecCtx->height / 2; i ++)
			{//U
				memcpy(&sBuf[offset], pavfFrame->data[1] + i * pavfFrame->linesize[1], pCodecCtx->width / 2); 
				offset += pCodecCtx->width / 2;			
			}

			for(i = 0; i < pCodecCtx->height / 2; i ++)
			{//V
				memcpy(&sBuf[offset], pavfFrame->data[2] + i * pavfFrame->linesize[2], pCodecCtx->width / 2); 
				offset += pCodecCtx->width / 2;
			}

			return 1;
		}

	protected:
		tagDecoder_INFO	_DecoderInfo;
		bool	_isOpen;
		bool	_isRealTime;
	};

} //namespace _ffmpeg_

#endif//__IDECODER_20170111090004_1444199361_H__
