#ifndef  __H265DECODER_20170111090004_1444199361_H__
#define  __H265DECODER_20170111090004_1444199361_H__

#include "IDecoder.h"

namespace _ffmpeg_{

	class H265Decoder : public IDecoder
	{
	public:
		static	IDecoder* Create( void )
		{
			return reinterpret_cast<IDecoder*>(new H265Decoder());
		}

	public:
		H265Decoder( void );

		~H265Decoder( void );

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

		int  Decoder( unsigned char* szData, UInt32 uSize );

		int  DecoderRaw( unsigned char* szData, UInt32 uSize );

		void SetSpeed( UInt8 uSpeed );

		void Reset( void );

	protected:
		int Convert_YV12ToRGB( AVFrame* pFrameYV12, AVFrame* Out_pFrameRgb );

		int Convert_RGBToYV12( AVFrame* pFrameRGB, AVFrame* Out_pFrameYV12 );

	private:
		HFNDecoder		_hfnDecoder;
		HFNDecoderRaw	_hfnDecoderRaw;
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

		//RGB
		AVFrame* _FrameRGB;
		unsigned char* _RgbBuffer;
		struct SwsContext* m_RgbCtx;

		//YV12
		AVFrame* _FrameYV12;
		unsigned char* _YV12Buffer;
		struct SwsContext* m_YV12Ctx;
	};

} //_ffmpeg_

#endif