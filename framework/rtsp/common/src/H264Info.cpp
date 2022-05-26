
#include <libFrameWork_Rtsp/H264Info.h>
#include <libFrameWork_Rtsp/Base64.h>

/**
* H264的NAL起始码防竞争机制
*
* @param buf SPS数据内容
*
* @无返回值
*/
static void de_emulation_prevention(UInt8* buf, UInt32* buf_size)
{
	int i = 0, j = 0;
	BYTE* tmp_ptr = NULL;
	unsigned int tmp_buf_size = 0;
	int val = 0;

	tmp_ptr = buf;
	tmp_buf_size = *buf_size;
	for (i = 0; i<(tmp_buf_size - 2); i++)
	{
		//check for 0x000003
		val = (tmp_ptr[i] ^ 0x00) + (tmp_ptr[i + 1] ^ 0x00) + (tmp_ptr[i + 2] ^ 0x03);
		if (val == 0)
		{
			//kick out 0x03
			for (j = i + 2; j<tmp_buf_size - 1; j++)
				tmp_ptr[j] = tmp_ptr[j + 1];

			//and so we should devrease bufsize
			(*buf_size)--;
		}
	}

	return;
}

H264Info::H264Info( void )
	: _Ready(0)
{
	memset(&_SPS, 0, sizeof(_SPS));
	memset(&_PPS, 0, sizeof(_PPS));
	_Info.uVideoType = GetVideoType();
}

H264Info::~H264Info( void )
{

}

void H264Info::Parse( char* c_szData, UInt32 uLen, tagVideoPlayload_INFO* pPlayloadInfo, bool isNormalize)
{
	Nalu264 Nal;
	Nal.Init(c_szData, uLen);
	pPlayloadInfo->uHeadType = Nal.Get_Type(c_szData[0]);
	switch(pPlayloadInfo->uHeadType)
	{
	case NAL_UNIT_TYPE_SPS: 
		{
			UInt32 uSize = uLen;
			_sSPSRaw.resize(0);
			_sSPSRaw.append(c_szData, uLen);

			if (isNormalize)
			{//NAL起始码防竞争机制
				de_emulation_prevention((UInt8*)&_sSPSRaw[0], &uSize);
				_sSPSRaw.update_size(uSize);
			}

			Nal.GetSPS(&_SPS);
			_Info.uWidth = (_SPS.pic_width_in_mbs_minus1+1) * 16;
			_Info.uHeight = (2 - _SPS.frame_mbs_only_flag)* (_SPS.pic_height_in_map_units_minus1 +1) * 16;
			if( _SPS.vui_parameters_present_flag )
			{
				// 注：这里的帧率计算还有问题，x264编码25fps，time_scale为50，num_units_in_tick为1，计算得50fps
				//网上说法，当nuit_field_based_flag为1时，再除以2，又说x264将该值设置为0.
				//地址：http://forum.doom9.org/showthread.php?t=153019
				//

				if( _SPS.vui.num_units_in_tick > 0)
					_Info.uMax_Framerate = (float)(_SPS.vui.time_scale) / (float)(_SPS.vui.num_units_in_tick) / 2;
				else //if (_Info.sps.vui.num_units_in_tick != 0)
					_Info.uMax_Framerate = (float)(_SPS.vui.time_scale) / (float)(_SPS.vui.num_units_in_tick);
			}

			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
			_Ready |= 0x1;
		}
		break;
	case NAL_UNIT_TYPE_PPS:
		{
			_sPPSRaw.resize(0);
			_sPPSRaw.append(c_szData, uLen);

			Nal.GetPPS(&_PPS, &_SPS);

			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
			_Ready |= 0x2;
		}
		break;
	case NAL_UNIT_TYPE_SEI:
		{
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = c_szData;
			pPlayloadInfo->uDataLen = uLen;
		}
		break;
	case NAL_UNIT_TYPE_FU_A:
		{
			tagFU_A_HEADER* pFuA = ((tagFU_A_HEADER*)&c_szData[0]);
			pPlayloadInfo->isFull = pFuA->E;
			if( pFuA->S )
			{//第一个包，修改负载类型
				tagNALU_HEADER* pHead = (tagNALU_HEADER*)(&c_szData[sizeof(tagFU_A_HEADER) - sizeof(tagNALU_HEADER)]);
				pHead->F = pFuA->Indicator.F;
				pHead->NRI = pFuA->Indicator.NRI;
				pHead->TYPE = pFuA->TYPE;
				pPlayloadInfo->pData = &c_szData[sizeof(tagFU_A_HEADER) - sizeof(tagNALU_HEADER)];
				pPlayloadInfo->uDataLen = uLen - (sizeof(tagFU_A_HEADER) - sizeof(tagNALU_HEADER));

				if( pHead->TYPE == NAL_UNIT_TYPE_CODED_SLICE_IDR  && _sIDRSlice == "" )
				{
					_sIDRSlice = _string_type(pPlayloadInfo->pData, pPlayloadInfo->uDataLen);
					_Ready |= 0x4;
				}
			}
			else
			{
				pPlayloadInfo->pData = &c_szData[sizeof(tagFU_A_HEADER)];
				pPlayloadInfo->uDataLen = uLen - sizeof(tagFU_A_HEADER);
			}
		}
		break;
	case NAL_UNIT_TYPE_FU_B:
		{

		}
		break;
	case NAL_UNIT_TYPE_CODED_SLICE_NON_IDR:
		{
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = &c_szData[0];
			pPlayloadInfo->uDataLen = uLen;
		}
		break;
	case NAL_UNIT_TYPE_CODED_SLICE_IDR:
		{
			pPlayloadInfo->isFull = true;
			pPlayloadInfo->pData = &c_szData[0];
			pPlayloadInfo->uDataLen = uLen;

			//if( _sIDRSlice == "" )
			{
				_sIDRSlice = _string_type(c_szData, uLen);
				_Ready |= 0x4;
			}
		}
		break;
	default:
		{
			printf("H264Info::Parse() unkonw:%d\r\n", Nal.Get_Type(c_szData[0]));
			/*while(true)
			{
				Sleep(100);
			}*/
		}
	}
}

bool H264Info::isReady( void )
{
	return _Ready == 0x07;
}

_string_type H264Info::Get_SDP_ProfileLevelID( void )
{
	//参考http://blog.csdn.net/wh8_2011/article/details/53558504
	_string_type s;
	UInt8 profile_iop = (_SPS.constraint_set0_flag << 7 & 0x01) | 
		(_SPS.constraint_set1_flag << 6 & 0x01) |
		(_SPS.constraint_set2_flag << 5 & 0x01) |
		(_SPS.constraint_set3_flag << 4 & 0x01) |
		(_SPS.constraint_set4_flag << 3 & 0x01) |
		(_SPS.constraint_set5_flag << 2 & 0x01) |
		(_SPS.reserved_zero_2bits & 0x03 );
	UInt8 level_idc = _SPS.level_idc * 10;
	s.append(1, _SPS.profile_idc);
	s.append(1, profile_iop);
	s.append(1, level_idc);

	return _string_type::HexToStr(s);
}

_string_type H264Info::Get_SDP_SpropParameterSets( void )
{
	_string_type s;
	s += base64Encode(_sSPSRaw.c_str(), _sSPSRaw.size());
	s += ",";
	s += base64Encode(_sPPSRaw.c_str(), _sPPSRaw.size());
	return s;
}

_string_type H264Info::GetHeadBuffer( void )
{
	return _sSPSRaw + _sPPSRaw + _sIDRSlice;
}
