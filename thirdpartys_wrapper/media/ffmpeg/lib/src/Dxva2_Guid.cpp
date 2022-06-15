
#include <libThirdWrap_FFmpeg/Dxva2_Guid.h>

namespace _ffmpeg_{

	typedef struct dxva2_mode 
	{
		const char		*name;
		const GUID		*guid;
		enum AVCodecID codec;
	} dxva2_mode;

	static const dxva2_mode dxva2_modes[] = 
	{
		/* MPEG-1/2 */
		{ "MPEG-2 variable-length decoder",                                               &DXVA2_ModeMPEG2_VLD,                   AV_CODEC_ID_MPEG2VIDEO },
		{ "MPEG-2 & MPEG-1 variable-length decoder",                                      &DXVA2_ModeMPEG2and1_VLD,               AV_CODEC_ID_MPEG2VIDEO },
		{ "MPEG-2 motion compensation",                                                   &DXVA2_ModeMPEG2_MoComp,                AV_CODEC_ID_NONE },
		{ "MPEG-2 inverse discrete cosine transform",                                     &DXVA2_ModeMPEG2_IDCT,                  AV_CODEC_ID_NONE },
		{ "MPEG-1 variable-length decoder",                                               &DXVA2_ModeMPEG1_VLD,                   AV_CODEC_ID_NONE },

		/* H.264 */
		{ "H.264 variable-length decoder, film grain technology",                         &DXVA2_ModeH264_F,                      AV_CODEC_ID_H264 },
		{ "H.264 variable-length decoder, no film grain technology (Intel ClearVideo)",   &DXVADDI_Intel_ModeH264_E,              AV_CODEC_ID_H264 },
		{ "H.264 variable-length decoder, no film grain technology",                      &DXVA2_ModeH264_E,                      AV_CODEC_ID_H264 },
		{ "H.264 variable-length decoder, no film grain technology, FMO/ASO",             &DXVA_ModeH264_VLD_WithFMOASO_NoFGT,    AV_CODEC_ID_H264 },
		{ "H.264 variable-length decoder, no film grain technology, Flash",               &DXVA_ModeH264_VLD_NoFGT_Flash,         AV_CODEC_ID_H264 },

		{ "H.264 inverse discrete cosine transform, film grain technology",               &DXVA2_ModeH264_D,                      AV_CODEC_ID_NONE },
		{ "H.264 inverse discrete cosine transform, no film grain technology",            &DXVA2_ModeH264_C,                      AV_CODEC_ID_NONE },
		{ "H.264 inverse discrete cosine transform, no film grain technology (Intel)",    &DXVADDI_Intel_ModeH264_C,              AV_CODEC_ID_NONE },

		{ "H.264 motion compensation, film grain technology",                             &DXVA2_ModeH264_B,                      AV_CODEC_ID_NONE },
		{ "H.264 motion compensation, no film grain technology",                          &DXVA2_ModeH264_A,                      AV_CODEC_ID_NONE },
		{ "H.264 motion compensation, no film grain technology (Intel)",                  &DXVADDI_Intel_ModeH264_A,              AV_CODEC_ID_NONE },

		/* WMV */
		{ "Windows Media Video 8 motion compensation",                                    &DXVA2_ModeWMV8_B,                      AV_CODEC_ID_NONE },
		{ "Windows Media Video 8 post processing",                                        &DXVA2_ModeWMV8_A,                      AV_CODEC_ID_NONE },

		{ "Windows Media Video 9 IDCT",                                                   &DXVA2_ModeWMV9_C,                      AV_CODEC_ID_NONE },
		{ "Windows Media Video 9 motion compensation",                                    &DXVA2_ModeWMV9_B,                      AV_CODEC_ID_NONE },
		{ "Windows Media Video 9 post processing",                                        &DXVA2_ModeWMV9_A,                      AV_CODEC_ID_NONE },

		/* VC-1 / WMV3 */
		/* VC-1 */
		{ "VC-1 variable-length decoder",                                                 &DXVA2_ModeVC1_D,                       AV_CODEC_ID_VC1 },
		{ "VC-1 variable-length decoder",                                                 &DXVA2_ModeVC1_D,                       AV_CODEC_ID_WMV3 },
		{ "VC-1 variable-length decoder",                                                 &DXVA2_ModeVC1_D2010,                   AV_CODEC_ID_VC1 },
		{ "VC-1 variable-length decoder",                                                 &DXVA2_ModeVC1_D2010,                   AV_CODEC_ID_WMV3 },
		{ "VC-1 variable-length decoder 2 (Intel)",                                       &DXVA_Intel_VC1_ClearVideo_2,           AV_CODEC_ID_NONE },
		{ "VC-1 variable-length decoder (Intel)",                                         &DXVA_Intel_VC1_ClearVideo,             AV_CODEC_ID_NONE },

		{ "VC-1 inverse discrete cosine transform",                                       &DXVA2_ModeVC1_C,                       AV_CODEC_ID_NONE },
		{ "VC-1 motion compensation",                                                     &DXVA2_ModeVC1_B,                       AV_CODEC_ID_NONE },
		{ "VC-1 post processing",                                                         &DXVA2_ModeVC1_A,                       AV_CODEC_ID_NONE },

		/* HEVC/H.265 */
		{ "", &DXVA2_ModeHEVC_VLD_Main, AV_CODEC_ID_HEVC },
		{ "", &DXVA2_ModeHEVC_VLD_Main10, AV_CODEC_ID_HEVC },

		/* VP8/9 */
		{ "VP9", &DXVA2_ModeVP9_VLD_Profile0, AV_CODEC_ID_VP9 },

		{ NULL, NULL, AV_CODEC_ID_NONE },
	};

	static bool Dxva2_MatchingGUID_Impl( IDirectXVideoDecoderService* pDecoderService, const GUID& Guid )
	{
		if( pDecoderService == NULL )
			return false;

		GUID RetGuid = GUID_NULL;
		GUID *guid_list = NULL;
		unsigned guid_count = 0, j = 0;

		HRESULT hr = pDecoderService->GetDecoderDeviceGuids(&guid_count, &guid_list);
		if (FAILED(hr)) 
		{
			printf("Dxva2_D3DVideoDecoder::MatchingGUID() GetDecoderDeviceGuids Failed\n");
			goto gt_end;
		}

		for (j = 0; j < guid_count; j++) 
		{
			if (IsEqualGUID(Guid, guid_list[j]))
				break;
		}

		if (j == guid_count || IsEqualGUID(Guid, GUID_NULL) )
		{
			goto gt_end;
		}

		return true;

gt_end:
		if( guid_list != NULL )
		{
			CoTaskMemFree(guid_list);
			guid_list = NULL;
		}

		return false;
	}

	GUID Dxva2_MatchingGUID( IDirectXVideoDecoderService* pDecoderService, AVCodecID CodecID )
	{
		GUID Guid = GUID_NULL;
		for (int i = 0; i < sizeof(dxva2_modes) / sizeof(dxva2_mode); i++) 
		{
			D3DFORMAT *target_list = NULL;
			unsigned target_count = 0;
			const dxva2_mode *mode = &dxva2_modes[i];
			if (mode->codec == CodecID)
			{
				if( Dxva2_MatchingGUID_Impl( pDecoderService, *mode->guid) )
				{
					Guid = *mode->guid;
					break;
				}
			}	
		}

		return Guid;
	}

} //namespace _ffmpeg_
