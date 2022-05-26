#ifndef  __MP3_DECODER_20170111090004_1444199361_H__
#define  __MP3_DECODER_20170111090004_1444199361_H__

#include "IDecoder.h"

namespace _ffmpeg_{

	class MP3_Decoder : public IDecoder
	{
	public:
		typedef MP3_Decoder	self_type;

		static	IDecoder* Create( void )
		{
			return reinterpret_cast<IDecoder*>(new self_type());
		}

	public:
		MP3_Decoder( void );

		~MP3_Decoder( void );

		int  Init(long lWidth, long lHeight,
			UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder,
			const HFNPrevOpen& hfnPrev);

		int  Init( AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
			const HFNPrevOpen& hfnPrev );

		void Release( void );

		void Close( void );

		int ReOpen( const HFNPrevOpen& hOpen );

		int  SetParamter( const HFNDecoder& hfnFNDecoder, bool isRealTime = false );

		int  SetParamterRaw( const HFNDecoderRaw& hfnFNDecoder, bool isRealTime = false );

		int  Decoder( unsigned char* szData, UInt32 uSize );

		int  DecoderRaw( unsigned char* szData, UInt32 uSize );

		void SetSpeed( UInt8 uSpeed );

		void Reset( void );

	private:
		HFNDecoder		_hfnDecoder;
		AVCodecContext * _pCodecCtx;
		HFNDecoderRaw	_hfnDecoderRaw;
		AVCodecParserContext* _pCodecParserCtx; 
		AVPacket _avpkt;
		AVFrame * _pavfFrame;
		tagAudio_INFO	_AudioInfo;
		_string_type	_sBuf;

		UInt8	_uSpeed;
	};

} //_ffmpeg_

#endif