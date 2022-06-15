
#ifndef __H265INFO_20171226154204_3344199361_H__
#define __H265INFO_20171226154204_3344199361_H__

#include "bs.h"
#include "Nalu265.h"
#include "IVideoInfo.h"

class H265Info : public IVideoInfo
{
public:
	H265Info( void );

	~H265Info( void );

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
		return IVideoInfo::ENUM_ENC_H265;
	}

	bool is_I_Frame( UInt8 uHeadType, const char* Nalu_Header )
	{
		int iType = -1;
		if( uHeadType == NAL_UNIT_UNSPECIFIED_49 )
		{
			iType = ((tagNALU265_HEADER*)(Nalu_Header))->TYPE;
		}

		if ( uHeadType == NAL_UNIT_SPS ||
			uHeadType == NAL_UNIT_PPS ||
			uHeadType == NAL_UNIT_CODED_SLICE_IDR_W_RADL ||
			uHeadType == NAL_UNIT_CODED_SLICE_IDR_N_LP ||
			//uHeadType == NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A ||
			iType == NAL_UNIT_CODED_SLICE_IDR_W_RADL || 
			iType == NAL_UNIT_CODED_SLICE_IDR_N_LP )
		{
			return true;
		}

		return false;
	}

private:
	tagMediaVideoINFO	_Info;
	h265_sps_t	_SPS;
	h265_pps_t	_PPS;
	_string_type _sSPSRaw;
	_string_type _sPPSRaw;
	_string_type _sIDRSlice;
	int		_Ready;
};

#endif
