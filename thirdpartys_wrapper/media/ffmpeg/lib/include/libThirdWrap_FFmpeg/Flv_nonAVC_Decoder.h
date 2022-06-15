#ifndef  __FLV_NON_AVC_DECODER_20170111090004_1444199361_H__
#define  __FLV_NON_AVC_DECODER_20170111090004_1444199361_H__

#include "IDecoder.h"
#include "Flv_Parse.h"

namespace _ffmpeg_{

	class Flv_nonAVC_Decoder : public IDecoder
	{
	public:
		static	IDecoder* Create( void )
		{
			return reinterpret_cast<IDecoder*>(new Flv_nonAVC_Decoder());
		}

	public:
		Flv_nonAVC_Decoder( void );

		~Flv_nonAVC_Decoder( void );

		int  Init( long lWidth, long lHeight,
				UInt8 uFrameRate, const HFNDecoder& hfnFNDecoder,
				const HFNPrevOpen& hfnPrev );

		int  Init( AVCodecContext* pCodecCtx, const HFNDecoder& hfnFNDecoder,
			const HFNPrevOpen& hfnPrev );

		void Release( void );

		void Close( void );

		int ReOpen( const HFNPrevOpen& hOpen );

		int  SetParamter( const HFNDecoder& hfnFNDecoder, bool isRealTime = false );

		int  SetParamterRaw( const HFNDecoderRaw& hfnFNDecoder, bool isRealTime = false );

		void SetSpeed( UInt8 uSpeed );

		int  Decoder( unsigned char* szData, UInt32 uSize );

		int  DecoderRaw( unsigned char* szData, UInt32 uSize );

		void Reset( void );

	protected:
		int  VideoDecoder( const unsigned char* szData, UInt32 uSize );

	private:
		FlvParse		_FlvParse;
		HFNDecoder		_hfnDecoder;
		AVCodecContext * _pCodecCtx;
		AVCodecParserContext* _pCodecParserCtx; 
		AVPacket _avpkt;
		AVFrame * _pavfFrame;
		tagVideo_INFO	_VideoInfo;
		_string_type	_sBuf;

		UInt8	_uFps;
		UInt64	_uFrameNumber;
		UInt8	_uSpeed;
		_timestamp_::Timestamp	_TimeStamp;
		_timestamp_::Timestamp::TimeVal	_NextTime;
	};

} //_ffmpeg_

#endif