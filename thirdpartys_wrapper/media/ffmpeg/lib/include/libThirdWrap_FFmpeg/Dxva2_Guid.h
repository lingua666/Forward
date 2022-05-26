
#ifndef __DXVA2_GUID_20170111090004_1444199361_H__
#define __DXVA2_GUID_20170111090004_1444199361_H__

#include "External.h"
#include "IDecoder.h"
#include <dxva2api.h>
#include <initguid.h>

#pragma comment(lib, "Dxva2.lib")

namespace _ffmpeg_{

	DEFINE_GUID(IID_IDirectXVideoDecoderService, 0xfc51a551, 0xd5e7, 0x11d9, 0xaf,0x55,0x00,0x05,0x4e,0x43,0xff,0x02);
	DEFINE_GUID(IID_IDirectXVideoAccelerationService, 0xfc51a550, 0xd5e7, 0x11d9, 0xaf,0x55,0x00,0x05,0x4e,0x43,0xff,0x02);

	DEFINE_GUID(DXVA_NoEncrypt, 0x1b81bed0, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_NoEncrypt,   0x1b81beD0, 0xa0c7,0x11d3,0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5);

	DEFINE_GUID(DXVA2_ModeMPEG2_MoComp, 0xe6a9f44b, 0x61b0, 0x4563, 0x9e, 0xa4, 0x63, 0xd2, 0xa3, 0xc6, 0xfe, 0x66);
	DEFINE_GUID(DXVA2_ModeMPEG2_IDCT, 0xbf22ad00, 0x03ea, 0x4690, 0x80, 0x77, 0x47, 0x33, 0x46, 0x20, 0x9b, 0x7e);
	DEFINE_GUID(DXVA2_ModeMPEG2_VLD, 0xee27417f, 0x5e28, 0x4e65, 0xbe, 0xea, 0x1d, 0x26, 0xb5, 0x08, 0xad, 0xc9);
	DEFINE_GUID(DXVA2_ModeMPEG2and1_VLD, 0x86695f12, 0x340e, 0x4f04, 0x9f, 0xd3, 0x92, 0x53, 0xdd, 0x32, 0x74, 0x60);
	DEFINE_GUID(DXVA2_ModeMPEG1_VLD, 0x6f3ec719, 0x3735, 0x42cc, 0x80, 0x63, 0x65, 0xcc, 0x3c, 0xb3, 0x66, 0x16);

	DEFINE_GUID(DXVA2_ModeH264_A,                       0x1b81be64, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeH264_B,                       0x1b81be65, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeH264_C,                       0x1b81be66, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeH264_D,                       0x1b81be67, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeH264_E,                       0x1b81be68, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeH264_F,                       0x1b81be69, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA_ModeH264_VLD_Multiview,            0x9901CCD3, 0xca12, 0x4b7e, 0x86, 0x7a, 0xe2, 0x22, 0x3d, 0x92, 0x55, 0xc3); // MVC
	DEFINE_GUID(DXVA_ModeH264_VLD_WithFMOASO_NoFGT,     0xd5f04ff9, 0x3418, 0x45d8, 0x95, 0x61, 0x32, 0xa7, 0x6a, 0xae, 0x2d, 0xdd);
	DEFINE_GUID(DXVADDI_Intel_ModeH264_A,               0x604F8E64, 0x4951, 0x4c54, 0x88, 0xFE, 0xAB, 0xD2, 0x5C, 0x15, 0xB3, 0xD6);
	DEFINE_GUID(DXVADDI_Intel_ModeH264_C,               0x604F8E66, 0x4951, 0x4c54, 0x88, 0xFE, 0xAB, 0xD2, 0x5C, 0x15, 0xB3, 0xD6);
	DEFINE_GUID(DXVADDI_Intel_ModeH264_E,               0x604F8E68, 0x4951, 0x4c54, 0x88, 0xFE, 0xAB, 0xD2, 0x5C, 0x15, 0xB3, 0xD6); // DXVA_Intel_H264_NoFGT_ClearVideo
	DEFINE_GUID(DXVA_ModeH264_VLD_NoFGT_Flash,          0x4245F676, 0x2BBC, 0x4166, 0xa0, 0xBB, 0x54, 0xE7, 0xB8, 0x49, 0xC3, 0x80);

	DEFINE_GUID(DXVA2_ModeWMV8_A,                       0x1b81be80, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeWMV8_B,                       0x1b81be81, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);

	DEFINE_GUID(DXVA2_ModeWMV9_A,                       0x1b81be90, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeWMV9_B,                       0x1b81be91, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeWMV9_C,                       0x1b81be94, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);

	DEFINE_GUID(DXVA2_ModeVC1_A,                        0x1b81beA0, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeVC1_B,                        0x1b81beA1, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeVC1_C,                        0x1b81beA2, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeVC1_D,                        0x1b81beA3, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
	DEFINE_GUID(DXVA2_ModeVC1_D2010,                    0x1b81beA4, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5); // August 2010 update
	DEFINE_GUID(DXVA_Intel_VC1_ClearVideo,              0xBCC5DB6D, 0xA2B6, 0x4AF0, 0xAC, 0xE4, 0xAD, 0xB1, 0xF7, 0x87, 0xBC, 0x89);
	DEFINE_GUID(DXVA_Intel_VC1_ClearVideo_2,            0xE07EC519, 0xE651, 0x4CD6, 0xAC, 0x84, 0x13, 0x70, 0xCC, 0xEE, 0xC8, 0x51);

	DEFINE_GUID(DXVA_nVidia_MPEG4_ASP,                  0x9947EC6F, 0x689B, 0x11DC, 0xA3, 0x20, 0x00, 0x19, 0xDB, 0xBC, 0x41, 0x84);
	DEFINE_GUID(DXVA_ModeMPEG4pt2_VLD_Simple,           0xefd64d74, 0xc9e8, 0x41d7, 0xa5, 0xe9, 0xe9, 0xb0, 0xe3, 0x9f, 0xa3, 0x19);
	DEFINE_GUID(DXVA_ModeMPEG4pt2_VLD_AdvSimple_NoGMC,  0xed418a9f, 0x010d, 0x4eda, 0x9a, 0xe3, 0x9a, 0x65, 0x35, 0x8d, 0x8d, 0x2e);
	DEFINE_GUID(DXVA_ModeMPEG4pt2_VLD_AdvSimple_GMC,    0xab998b5b, 0x4258, 0x44a9, 0x9f, 0xeb, 0x94, 0xe5, 0x97, 0xa6, 0xba, 0xae);
	DEFINE_GUID(DXVA_ModeMPEG4pt2_VLD_AdvSimple_Avivo,  0x7C74ADC6, 0xe2ba, 0x4ade, 0x86, 0xde, 0x30, 0xbe, 0xab, 0xb4, 0x0c, 0xc1);

	DEFINE_GUID(DXVA2_ModeHEVC_VLD_Main, 0x5b11d51b, 0x2f4c, 0x4452, 0xbc, 0xc3, 0x09, 0xf2, 0xa1, 0x16, 0x0c, 0xc0);
	DEFINE_GUID(DXVA2_ModeHEVC_VLD_Main10, 0x107af0e0, 0xef1a, 0x4d19, 0xab, 0xa8, 0x67, 0xa1, 0x63, 0x07, 0x3d, 0x13);
	DEFINE_GUID(DXVA2_ModeVP9_VLD_Profile0, 0x463707f8, 0xa1d0, 0x4585, 0x87, 0x6d, 0x83, 0xaa, 0x6d, 0x60, 0xb8, 0x9e);

	DEFINE_GUID(DXVA2_VideoProcProgressiveDevice, 0x5a54a0c9,0xc7ec,0x4bd9,0x8e,0xde,0xf3,0xc7,0x5d,0xc4,0x39,0x3b);
	DEFINE_GUID(DXVA2_VideoProcBobDevice,         0x335aa36e,0x7884,0x43a4,0x9c,0x91,0x7f,0x87,0xfa,0xf3,0xe3,0x7e);
	DEFINE_GUID(DXVA2_VideoProcSoftwareDevice,    0x4553d47f,0xee7e,0x4e3f,0x94,0x75,0xdb,0xf1,0x37,0x6c,0x48,0x10);

	//DEFINE_GUID(GUID_NULL, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

	GUID Dxva2_MatchingGUID( IDirectXVideoDecoderService* pDecoderService, AVCodecID CodecID );

} //namespace _ffmpeg_

#endif//__DXVA2_GUID_20170111090004_1444199361_H__