
#ifndef __AUDIO_ACC_ENCODER_20170111090004_1444199361_H__
#define __AUDIO_ACC_ENCODER_20170111090004_1444199361_H__

#include "External.h"
#include "IDecoder.h"

namespace _ffmpeg_{

	class AAC_Encoder
	{
	public:
		typedef function20_handle<void(const UInt8*, UInt32 uSize)>		HFNEncoder;

	public:
		AAC_Encoder(void);

		~AAC_Encoder(void);

		int Init(UInt32 uSampleRate, UInt8 uChannel, int64_t BitRate, const HFNEncoder& hEncoder);

		void Release(void);

		int Encoder(const char* c_szData, UInt32 uSize, bool isHead = false);

	private:
		SwrContext*	_swr_ctx;
		AVCodecContext*	_pCodecCtx;
		AVCodec*	_pCodec;
		AVFrame*	_pFrame;
		int64_t		_BitRate;		//位率
		uint8_t*	_Frame_Buf;
		int			_Frame_Size;
		uint8_t**	_ConvertData;	//存储转换后的数据，再编码AAC 
		char		_Padts[7];
		char		_mp3padts[4];
		Int64		_Pts;
		HFNEncoder _hEncoder;
		_string_type	_sData;
	};

} //namespace _ffmpeg_

#endif//__DECODERFACTORY_20170111090004_1444199361_H__
