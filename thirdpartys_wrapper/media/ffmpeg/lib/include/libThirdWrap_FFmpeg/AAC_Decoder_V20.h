
#ifndef __AUDIO_ACC_DECODER_V20_20170111090004_1444199361_H__
#define __AUDIO_ACC_DECODER_V20_20170111090004_1444199361_H__

#include "External.h"
#include "IDecoder.h"

namespace _ffmpeg_{

	class AAC_Decoder_V20 : public IDecoder
	{
	public:
		typedef AAC_Decoder_V20	self_type;

		static	IDecoder* Create(void)
		{
			return reinterpret_cast<IDecoder*>(new self_type());
		}

	public:
		AAC_Decoder_V20(void);

		~AAC_Decoder_V20(void);

		int Init(long lWidth, long lHeight,
			UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder = HFNDecoder(),
			const HFNPrevOpen& hfnPrev = HFNPrevOpen());

		int  Init(AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
			const HFNPrevOpen& hfnPrev);

		void Release(void);

		void Close(void);

		int ReOpen(const HFNPrevOpen& hOpen);

		int  SetParamter(const HFNDecoder& hfnFNDecoder, bool isRealTime = false);

		int  SetParamterRaw(const HFNDecoderRaw& hfnFNDecoder, bool isRealTime = false);

		int  Decoder(unsigned char* szData, UInt32 uSize);

		int  DecoderRaw(unsigned char* szData, UInt32 uSize);

		void SetSpeed(UInt8 uSpeed);

		void Reset(void);

	private:
		SwrContext*		_SwrCtx;
		HFNDecoder		_hfnDecoder;
		AVCodecContext * _pCodecCtx;
		HFNDecoderRaw	_hfnDecoderRaw;
		AVCodecParserContext* _pCodecParserCtx;
		AVPacket _avpkt;
		AVFrame * _pavfFrame;
		tagAudio_INFO	_AudioInfo;
		_string_type	_sBuf;
		_string_type	_sFrameBuf;

		UInt8	_uSpeed;
	};

} //namespace _ffmpeg_

#endif//__DECODERFACTORY_20170111090004_1444199361_H__
