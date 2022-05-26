
#ifndef __AUDIO_ACC_DECODER_20170111090004_1444199361_H__
#define __AUDIO_ACC_DECODER_20170111090004_1444199361_H__

#include "External.h"
#include "IDecoder.h"

namespace _ffmpeg_{

	class AAC_Decoder
	{
	public:
		typedef function20_handle<void(const UInt8*, UInt32 uSize)>		HFNDecoder;

	public:
		AAC_Decoder(void);

		~AAC_Decoder(void);

		int Init(UInt32 uSampleRate, UInt8 uChannel, const HFNDecoder& hDecoder);

		void Release(void);

		int Decoder(const char* c_szData, UInt32 uSize);

	private:
		SwrContext*	_swr_ctx;
		AVCodecContext*	_pCodecCtx;
		AVCodec*	_pCodec;
		AVCodecParserContext*	_pParser;
		AVFrame*	_pFrame;
		uint8_t*	_Frame_Buf;
		int			_Frame_Size;
		HFNDecoder _hDecoder;
		_string_type	_sData;
	};

} //namespace _ffmpeg_

#endif//__DECODERFACTORY_20170111090004_1444199361_H__
