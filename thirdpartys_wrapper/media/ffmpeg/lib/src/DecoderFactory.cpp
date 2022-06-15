
#include <libThirdWrap_FFmpeg/DecoderFactory.h>
#include <libThirdWrap_FFmpeg/H264_Decoder.h>
#include <libThirdWrap_FFmpeg/H265_Decoder.h>
#include <libThirdWrap_FFmpeg/Flv_nonAVC_Decoder.h>
#include <libThirdWrap_FFmpeg/Flv_AVC_Decoder.h>
#include <libThirdWrap_FFmpeg/MP3_Decoder.h>
#include <libThirdWrap_FFmpeg/AAC_Decoder_V20.h>

namespace _ffmpeg_{

	void DecoderFactory::FreeDecoder(IDecoder* pDecoder)
	{
		if (pDecoder != NULL)
			delete pDecoder;
	}

	DecoderFactory::DecoderFactory( void )
	{
		static CreateCallback CreateEntries [] = { &H264Decoder::Create,
			&H265Decoder::Create, &Flv_nonAVC_Decoder::Create, &Flv_AVC_Decoder::Create,
			&MP3_Decoder::Create, &AAC_Decoder_V20::Create };

		_CreateVec.resize(sizeof(CreateEntries) / sizeof(CreateCallback));

		for ( int i = 0 ; i < _CreateVec.size() ; i++ )
		{
			_CreateVec[i] = CreateEntries[i];
		}

		//挂上所有的codec。也许只挂一个H264的codec就行，我没试过： 
		av_register_all();
		avcodec_register_all();
		//关闭日志
		av_log_set_level(AV_LOG_QUIET);
	}

	IDecoder* DecoderFactory::CreateRender ( enDecoderType Type )
	{
		IDecoder* p = NULL;
		try
		{
			CreateCallback pCallback = _CreateVec.at(Type);// 越界抛出异常
			p = pCallback();
		}
		catch(const std::exception& e)
		{//越界处理
			return 0;
		}

		return p;
	}
	 
	static DecoderFactory g_DecoderFactory201612311830;

	DecoderFactory* DecoderFactory_GetInstance( void )
	{
		return &g_DecoderFactory201612311830;
	}

} //namespace _ffmpeg_
