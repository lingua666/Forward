
#ifndef __H264INFO_20171226154204_3344199361_H__
#define __H264INFO_20171226154204_3344199361_H__

#include "bs.h"
#include "IVideoInfo.h"
#include "Nalu264.h"

class H264Info : public IVideoInfo
{
public:
	H264Info( void );

	~H264Info( void );

	void Parse( char* c_szData, UInt32 uLen, tagVideoPlayload_INFO* pPlayloadInfo );

	bool isReady( void );

	_string_type Get_SDP_ProfileLevelID( void );

	_string_type Get_SDP_SpropParameterSets( void );

	_string_type GetHeadBuffer( void );

	tagMediaVideoINFO*	GetStreamInfo( void )
	{
		return &_Info;
	}

	UInt8	GetVideoType( void ) const
	{
		return IVideoInfo::ENUM_ENC_H264;
	}

	bool is_I_Frame( UInt8 uHeadType, const char* Nalu_Header )
	{
		int iType = -1;
		if( uHeadType == NAL_UNIT_TYPE_FU_A || 
			uHeadType == NAL_UNIT_TYPE_FU_B )
		{
			iType = ((tagNALU_HEADER*)(Nalu_Header))->TYPE;
		}

		if ( uHeadType == NAL_UNIT_TYPE_SPS ||
			uHeadType == NAL_UNIT_TYPE_PPS ||
			uHeadType == NAL_UNIT_TYPE_CODED_SLICE_IDR ||
			uHeadType == NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A ||
			iType == NAL_UNIT_TYPE_CODED_SLICE_IDR )
		{
			return true;
		}

		return false;
	}

private:
	tagMediaVideoINFO	_Info;
	h264_sps_t	_SPS;
	h264_pps_t	_PPS;
	_string_type _sSPSRaw;
	_string_type _sPPSRaw;
	_string_type _sIDRSlice;
	int		_Ready;
};

#endif
