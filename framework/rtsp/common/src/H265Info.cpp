
#include <libFrameWork_Rtsp/H265Info.h>
#include <libFrameWork_Rtsp/Base64.h>

H265Info::H265Info( void )
	: _Ready(0)
{
	memset(&_SPS, 0, sizeof(_SPS));
	memset(&_PPS, 0, sizeof(_PPS));
	_Info.uVideoType = GetVideoType();
}

H265Info::~H265Info( void )
{

}

void H265Info::Parse( char* c_szData, UInt32 uLen, tagVideoPlayload_INFO* pPlayloadInfo )
{
	Nalu265 Nal;
	Nal.Init(c_szData, uLen);
	pPlayloadInfo->uHeadType = Nal.Get_Type(c_szData, uLen);
	switch( pPlayloadInfo->uHeadType )
	{
	case NAL_UNIT_VPS:
		{
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
		}
		break;
	case NAL_UNIT_SPS: 
		{
			_sSPSRaw.resize(0);
			_sSPSRaw.append(c_szData, uLen);

			Nal.GetSPS(&_SPS);
			_Info.uWidth = Nal.GetVideoInfo()->width;
			_Info.uHeight = Nal.GetVideoInfo()->height;
			_Info.uMax_Framerate = Nal.GetVideoInfo()->max_framerate == 0 ? 25 : Nal.GetVideoInfo()->max_framerate;
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
			_Ready |= 0x1;
		}
		break;
	case NAL_UNIT_PPS:
		{
			_sPPSRaw.resize(0);
			_sPPSRaw.append(c_szData, uLen);

			Nal.GetPPS(&_PPS);
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
			_Ready |= 0x2;
		}
		break;
	case NAL_UNIT_PREFIX_SEI:
	case NAL_UNIT_SUFFIX_SEI:
		{
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
		}
		break;
	case NAL_UNIT_UNSPECIFIED_49://NAL_UNIT_TYPE_FU_A:
		{
			tagFU265_A_HEADER FuA = {0};
			memcpy(&FuA, &c_szData[0], sizeof(tagFU265_A_HEADER));
			//tagFU265_A_HEADER* pFuA = ((tagFU265_A_HEADER*)&c_szData[0]);
			pPlayloadInfo->uHeadType = NAL_UNIT_UNSPECIFIED_49;//NAL_UNIT_TYPE_FU_A;
			pPlayloadInfo->isFull = FuA.E;
			if( FuA.S )
			{//第一个包，修改负载类型
				tagNALU265_HEADER* pHead = (tagNALU265_HEADER*)(&c_szData[sizeof(tagFU265_A_HEADER) - sizeof(tagNALU265_HEADER)]);
				pHead->F = FuA.Indicator.F;
				pHead->TYPE = FuA.TYPE;
				pHead->lLayerID = FuA.Indicator.lLayerID;
				pHead->TID = FuA.Indicator.TID;
				pHead->hLayerID = FuA.Indicator.hLayerID;
				
				
				pPlayloadInfo->pData = &c_szData[sizeof(tagFU265_A_HEADER) - sizeof(tagNALU265_HEADER)];
				pPlayloadInfo->uDataLen = uLen - (sizeof(tagFU265_A_HEADER) - sizeof(tagNALU265_HEADER));

				if( pHead->TYPE == NAL_UNIT_CODED_SLICE_IDR_W_RADL  && _sIDRSlice == "" )
				{
					_sIDRSlice = _string_type(pPlayloadInfo->pData, pPlayloadInfo->uDataLen);
					_Ready |= 0x4;
				}
			}
			else
			{
				pPlayloadInfo->pData = &c_szData[sizeof(tagFU265_A_HEADER)];
				pPlayloadInfo->uDataLen = uLen - sizeof(tagFU265_A_HEADER);
			}
		}
		break;
	case NAL_UNIT_CODED_SLICE_IDR_W_RADL:
	case NAL_UNIT_CODED_SLICE_IDR_N_LP:
		{
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = &c_szData[0];
			pPlayloadInfo->uDataLen = uLen;

			if( _sIDRSlice == "" )
			{
				_sIDRSlice = _string_type(c_szData, uLen);
				_Ready |= 0x4;
			}
		}
		break;
	default:
		{
			if( Nal.isSlice() )
			{
				pPlayloadInfo->isFull = true;
				pPlayloadInfo->pData = &c_szData[0];
				pPlayloadInfo->uDataLen = uLen;
			}
			else
			{
				printf("H265Info::Parse() unkonw:%d\r\n", Nal.Get_Type(c_szData, uLen));
			}
		}
	}
}

bool H265Info::isReady( void )
{
	return _Ready == 0x07;
}

_string_type H265Info::Get_SDP_ProfileLevelID( void )
{
	//参考http://blog.csdn.net/wh8_2011/article/details/53558504
	/*_string_type s;
	UInt8 profile_iop = (_Info.sps.constraint_set0_flag << 7 & 0x01) | 
		(_Info.sps.constraint_set1_flag << 6 & 0x01) |
		(_Info.sps.constraint_set2_flag << 5 & 0x01) |
		(_Info.sps.constraint_set3_flag << 4 & 0x01) |
		(_Info.sps.constraint_set4_flag << 3 & 0x01) |
		(_Info.sps.constraint_set5_flag << 2 & 0x01) |
		(_Info.sps.reserved_zero_2bits & 0x03 );
	UInt8 level_idc = _Info.sps.level_idc * 10;
	s.append(1, _Info.sps.profile_idc);
	s.append(1, profile_iop);
	s.append(1, level_idc);

	return _string_type::HexToStr(s);*/

	return "";
}

_string_type H265Info::Get_SDP_SpropParameterSets( void )
{
	_string_type s;
	s += base64Encode(_sSPSRaw.c_str(), _sSPSRaw.size());
	s += ",";
	s += base64Encode(_sPPSRaw.c_str(), _sPPSRaw.size());
	return s;
}

_string_type H265Info::GetHeadBuffer( void )
{
	return _sSPSRaw + _sPPSRaw + _sIDRSlice;
}
