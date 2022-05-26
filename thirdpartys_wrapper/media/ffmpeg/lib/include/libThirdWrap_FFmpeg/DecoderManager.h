#ifndef  __DECODERMANAGER_20170111090004_1444199361_H__
#define  __DECODERMANAGER_20170111090004_1444199361_H__

#include "IDecoder.h"
#include "Dxva2_Guid.h"

namespace _ffmpeg_{

	class DecoderManager
	{	
	public:
		typedef IDecoder::HFNPrevOpen	HFNPrevOpen;
		typedef	IDecoder::HFNDecoder	HFNDecoder;
		typedef	IDecoder::HFNDecoderRaw	HFNDecoderRaw;
		typedef	IDecoder::tagDecoder_INFO	tagDecoder_INFO;

	public:
		DecoderManager( void );

		~DecoderManager( void );
		
		HDecoder  GetDecoder( long lWidth, long lHeight,
						int iFrameRate,
						enDecoderType enType,
						const HFNDecoder& hfnFNDecoder = HFNDecoder(),
						const HFNPrevOpen& hfnPrev = HFNPrevOpen() );

		//数据长度最小为2048
		HDecoder  GetDecoder( const char* c_szData,
						UInt32	uSize,
						const HFNDecoder& hfnFNDecoder = HFNDecoder(),
						const HFNPrevOpen& hfnPrev = HFNPrevOpen() );

		HDecoder  GetDecoder( AVCodecContext* pCodecCtx,
			const HFNDecoder& hfnFNDecoder,
			const HFNPrevOpen& hfnPrev = HFNPrevOpen() );

		//数据长度最小为2048
		int	GetCodecCtx( const char* c_szData, UInt32 uSize, AVCodecContext** out_pCodecCtx );

		int	GetCodecCtx_V20( const char* c_szData, UInt32 uSize,
			const _string_type& sEncType, AVCodecContext** out_pCodecCtx );

		void Close( HDecoder hDec );

		int ReOpen( HDecoder hDec, const HFNPrevOpen& hfnPrev = HFNPrevOpen() );

		int	Decoder( HDecoder hDec, unsigned char* szData, UInt32 uSize );

		int  DecoderRaw( HDecoder hDec, unsigned char* szData, UInt32 uSize );

		void Destroy( HDecoder hDec );

		int  SetParamter( HDecoder hDec, const HFNDecoder& hfnFNDecoder, bool isRealTime );

		int  SetParamterRaw( HDecoder hDec, const HFNDecoderRaw& hfnFNDecoder, bool isRealTime );

		IDecoder::tagDecoder_INFO* GetDecoderInfo( HDecoder hDec );

		void SetSpeed( HDecoder hDec, UInt8 uSpeed );

		void Reset( HDecoder hDec );
	};

	inline DecoderManager* GetDecoderManagerInstance( void )
	{
		static DecoderManager* pDecoderM = Singleton<DecoderManager>::instance();
		return pDecoderM;
	}

} //_ffmpeg_

#endif