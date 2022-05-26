
#include <libThirdWrap_FFmpeg/PSPacket.h>

#ifndef AV_RB16
#   define AV_RB16(x)                           \
    ((((const unsigned char*)(x))[0] << 8) |          \
    ((const unsigned char*)(x))[1])
#endif


static inline UInt64 ff_parse_pes_pts(const unsigned char* buf) 
{
    return (UInt64)(*buf & 0x0e) << 29 |
        (AV_RB16(buf+1) >> 1) << 15 |
        AV_RB16(buf+3) >> 1;
}

static UInt64 get_pts(optional_pes_header* option)
{
    if(option->PTS_DTS_flags != 2 && option->PTS_DTS_flags != 3 && option->PTS_DTS_flags != 0)
    {
        return 0;
    }

	unsigned char* pts = (unsigned char*)option + sizeof(optional_pes_header);
	UInt64 uPTS = 0;
	if ((option->PTS_DTS_flags & 0x3) == 2)
	{
		uPTS = (UInt64)(pts[0] & 0x0E) << 29;			//取3bit

		uPTS |= (((((UInt64)(pts[1])) << 8) |			//取15bit
			(((UInt64)(pts[2])))) >> 1) << 15;

		uPTS |= ((((UInt64)(pts[3])) << 8) |			//取15bit
			(((UInt64)(pts[4])))) >> 1;
	}
	else if ((option->PTS_DTS_flags & 0x3) == 3)
	{
		uPTS = (UInt64)(pts[0] & 0x0E) << 29;			//取3bit

		uPTS |= (((((UInt64)(pts[1])) << 8) |			//取15bit
			(((UInt64)(pts[2])))) >> 1) << 15;

		uPTS |= ((((UInt64)(pts[3])) << 8) |			//取15bit
			(((UInt64)(pts[4])))) >> 1;
	}

	return uPTS;
   /* if((option->PTS_DTS_flags & 2) == 2)
    {
        unsigned char* pts = (unsigned char*)option + sizeof(optional_pes_header);
        return ff_parse_pes_pts(pts);
    }
    return 0;*/
}

static UInt64 get_dts(optional_pes_header* option)
{
    if(option->PTS_DTS_flags != 2 && option->PTS_DTS_flags != 3 && option->PTS_DTS_flags != 0)
    {
        return 0;
    }
    if((option->PTS_DTS_flags & 3) == 3)
    {
        unsigned char* dts = (unsigned char*)option + sizeof(optional_pes_header) + 5;
        return ff_parse_pes_pts(dts);
    }
    return 0;
}

bool inline is_ps_header(ps_header_t* ps)
{
    if(ps->pack_start_code[0] == 0 && ps->pack_start_code[1] == 0 && ps->pack_start_code[2] == 1 && ps->pack_start_code[3] == 0xBA)
        return true;
    return false;
}

bool inline is_sh_header(sh_header_t* sh)
{
    if(sh->system_header_start_code[0] == 0 && sh->system_header_start_code[1] == 0 && sh->system_header_start_code[2] == 1 && sh->system_header_start_code[3] == 0xBB)
        return true;
    return false;
}

bool inline is_psm_header(psm_header_t* psm)
{
    if(psm->promgram_stream_map_start_code[0] == 0 && psm->promgram_stream_map_start_code[1] == 0 && psm->promgram_stream_map_start_code[2] == 1 && psm->promgram_stream_map_start_code[3] == 0xBC)
        return true;
    return false;
}

bool inline is_pes_video_header(pes_header_t* pes)
{
    if(pes->pes_start_code_prefix[0]==0 && pes->pes_start_code_prefix[1] == 0 && pes->pes_start_code_prefix[2] == 1 && pes->stream_id == 0xE0)
        return true;
    return false;
}

bool inline is_pes_audio_header(pes_header_t* pes)
{
    if(pes->pes_start_code_prefix[0]==0 && pes->pes_start_code_prefix[1] == 0 && pes->pes_start_code_prefix[2] == 1 && pes->stream_id == 0xC0)
        return true;
    return false;
}

bool inline is_pes_header(pes_header_t* pes)
{
    if(pes->pes_start_code_prefix[0]==0 && pes->pes_start_code_prefix[1] == 0 && pes->pes_start_code_prefix[2] == 1)
    {
        if(pes->stream_id == 0xC0 || pes->stream_id == 0xE0)
        {
            return true;
        }
    }
    return false;
}

PSStatus inline pes_type(pes_header_t* pes)
{
    if(pes->pes_start_code_prefix[0]==0 && pes->pes_start_code_prefix[1] == 0 && pes->pes_start_code_prefix[2] == 1)
    {
        if(pes->stream_id == 0xC0)
        {//音频
            return ps_pes_audio;
        }
        else if(pes->stream_id == 0xE0)
        {//视频
            return ps_pes_video;
        }
		else if (pes->stream_id == 0xBD)
		{//私有流
			return ps_pes_private;
		}
    }

    return ps_padding;
}

static PSTrickModeCtrl get_trick_mode_control(optional_pes_header* option)
{
	int iOffset = 0;
	unsigned char* data = (unsigned char*)option + sizeof(optional_pes_header);

	if (option->PTS_DTS_flags == 2 || option->PTS_DTS_flags == 3)
	{//PTS
		iOffset = 5;
	}

	if (option->PTS_DTS_flags == 3)
	{//DTS
		iOffset += 5;
	}

	if (option->ESCR_flag == 1)
	{//ESCR
		iOffset += 6;
	}

	if (option->ES_rate_flag == 1)
	{//ES_rate
		iOffset += 3;
	}

	if (option->DSM_trick_mode_flag == 1)
	{
		return (PSTrickModeCtrl)(((UInt8)data[iOffset]) >> 5);
	}

	return ps_null;
}


PSPacket::PSPacket(void)
	: _uWaitBytes(sizeof(ps_header_t))
	, _uStatus(ps_ps)
{
	memset(_PESHeader, 0, sizeof(_PESHeader));
}

PSPacket::~PSPacket(void)
{

}

int PSPacket::Init(const HFNData& hData)
{
	_hData = hData;
	return 1;
}

void PSPacket::Release(void)
{

}

int PSPacket::Parse(const char* c_szData, UInt32 uSize)
{
	//ps header
	_sData.append(c_szData, uSize);
	_uWaitBytes -= __min(_uWaitBytes, uSize);
	if (_uWaitBytes == 0)
	{//开始解析
	gt_start:
		UInt32 uOffset = 0;
		pes_header_t* pPesHead = NULL;
		ps_header_t* pPSHead = (ps_header_t*)_sData.c_str();
		if (is_ps_header(pPSHead))
		{
			uOffset += sizeof(ps_header_t) + pPSHead->pack_stuffing_length;
			if (_sData.size() < uOffset)
			{//长度不够
				_uWaitBytes = uOffset - _sData.size();
				goto gt_end;
			}

			sh_header_t* pSHead = (sh_header_t*)&_sData[uOffset];
			if (is_sh_header(pSHead))
			{//PS system header
				uOffset += sizeof(pSHead->system_header_start_code) + sizeof(pSHead->header_length) + Conversion16ToByteOrder((*(UInt16*)pSHead->header_length), EN_BOT_BIG);
				if (_sData.size() < uOffset)
				{//长度不够
					_uWaitBytes = uOffset - _sData.size();
					goto gt_end;
				}

				psm_header_t* pSMHead = (psm_header_t*)&_sData[uOffset];
				if (is_psm_header(pSMHead))
				{//program_stream_map
					uOffset += sizeof(pSMHead->promgram_stream_map_start_code) + sizeof(pSMHead->program_stream_map_length) + Conversion16ToByteOrder((*(UInt16*)pSMHead->program_stream_map_length), EN_BOT_BIG);
					if (_sData.size() < uOffset)
					{//长度不够
						_uWaitBytes = uOffset - _sData.size();
						goto gt_end;
					}

					_StreamInfoDesc = GetProgramStreamInfoDesc(pSMHead);
					GetElementaryStreamInfo(pSMHead);
					if (_sData.size() - uOffset > 0)
					{
						std::memmove(&_sData[0], &_sData[uOffset], _sData.size() - uOffset);
						_sData.update_size(_sData.size() - uOffset);

						if (_sData.size() > sizeof(pes_header_t) + sizeof(optional_pes_header))
							goto gt_start;
					}
					else
						_sData.clear();
				}
			}
		}
		else if (is_pes_header((pes_header_t*)_sData.c_str()))
		{
			pPesHead = (pes_header_t*)&_sData[uOffset];
		gt_PES:
			UInt16 PES_packet_length = Conversion16ToByteOrder(pPesHead->PES_packet_length, EN_BOT_BIG);
			if (_sData.size() < uOffset + sizeof(pes_header_t) + PES_packet_length)
			{//长度不够
				_uWaitBytes = uOffset + sizeof(pes_header_t) + PES_packet_length - _sData.size();
				goto gt_end;
			}

			optional_pes_header* pOptional = (optional_pes_header*)((char*)&pPesHead->PES_packet_length + sizeof(pPesHead->PES_packet_length));

			UInt64 ll = get_pts(pOptional);

			switch (pes_type(pPesHead))
			{
			case PSStatus::ps_pes_video://视频
			{
				uOffset += sizeof(pes_header_t) + sizeof(optional_pes_header) + pOptional->PES_header_data_length;

				//负载内容、长度
				int iPayLoad = PES_packet_length - sizeof(optional_pes_header) - pOptional->PES_header_data_length;
				if (_hData)
				{
					_hData(PSStatus::ps_pes_video, &_sData[uOffset], iPayLoad);
				}

				uOffset += iPayLoad;
			}
			break;
			case PSStatus::ps_pes_audio://音频
			{
				uOffset += sizeof(pes_header_t) + sizeof(optional_pes_header) + pOptional->PES_header_data_length;

				//负载内容、长度
				int iPayLoad = PES_packet_length - sizeof(optional_pes_header) - pOptional->PES_header_data_length;
				if (_hData)
				{
					_hData(PSStatus::ps_pes_audio, &_sData[uOffset], iPayLoad);
				}

				uOffset += iPayLoad;
			}
			break;
			case PSStatus::ps_pes_private://私有流
			{
				uOffset += sizeof(pes_header_t) + sizeof(optional_pes_header) + pOptional->PES_header_data_length;

				//负载内容、长度
				int iPayLoad = PES_packet_length - sizeof(optional_pes_header) - pOptional->PES_header_data_length;
				if (_hData)
				{
					_hData(PSStatus::ps_pes_private, &_sData[uOffset], iPayLoad);
				}

				uOffset += iPayLoad;
			}
			break;
			default:
			{
				printf("PSPacket::Parse(%d) Unknow\r\n", pes_type(pPesHead));
			}
			break;
			}

			if (_sData.size() - uOffset > 0)
			{
				std::memmove(&_sData[0], &_sData[uOffset], _sData.size() - uOffset);
				_sData.update_size(_sData.size() - uOffset);
				if (_sData.size() > sizeof(pes_header_t) + sizeof(optional_pes_header))
					goto gt_start;
			}
			else
				_sData.clear();
		}
		else
		{
			const char c_PSStartCode[] = { 0x00, 0x00, 0x01, 0xBA };
			const char c_PESVideoStartCode[] = { 0x00, 0x00, 0x01, 0xE0 };
			const char c_PESAudioStartCode[] = { 0x00, 0x00, 0x01, 0xC0 };
			int iPos = _sData.find(c_PSStartCode, 0, sizeof(c_PSStartCode));
			if (iPos > 0)
			{//找到PS头
				std::memmove(&_sData[0], &_sData[iPos], _sData.size() - iPos);
				_sData.update_size(_sData.size() - iPos);
				goto gt_end;
			}

			iPos = _sData.find(c_PESVideoStartCode, 0, sizeof(c_PESVideoStartCode));
			if (iPos > 0)
			{//找到PES 视频
				std::memmove(&_sData[0], &_sData[iPos], _sData.size() - iPos);
				_sData.update_size(_sData.size() - iPos);
				goto gt_end;
			}

			iPos = _sData.find(c_PESAudioStartCode, 0, sizeof(c_PESAudioStartCode));
			if (iPos > 0)
			{//找到PES 音频
				std::memmove(&_sData[0], &_sData[iPos], _sData.size() - iPos);
				_sData.update_size(_sData.size() - iPos);
				goto gt_end;
			}

			_sData.clear();
			return -1;
		}
	}

gt_end:

	return 1;
}


//推荐使用 逻辑层次更清晰，阅读性更高
int PSPacket::ParseEx(const char* c_szData, UInt32 uSize)
{
	//ps header
	switch (_uStatus)
	{
	case PSStatus::ps_ps:
	{
		_sData.append(c_szData, uSize);
		_uWaitBytes -= __min(_uWaitBytes, uSize);
		if (_uWaitBytes == 0)
		{//开始解析
		gt_start:
			UInt32 uOffset = 0;
			pes_header_t* pPesHead = NULL;
			ps_header_t* pPSHead = (ps_header_t*)_sData.c_str();
			if (is_ps_header(pPSHead))
			{
				uOffset += sizeof(ps_header_t) + pPSHead->pack_stuffing_length;
				if (_sData.size() < uOffset)
				{//长度不够
					_uWaitBytes = uOffset - _sData.size();
					goto gt_end;
				}

				sh_header_t* pSHead = (sh_header_t*)&_sData[uOffset];
				if (is_sh_header(pSHead))
				{//PS system header
					uOffset += sizeof(pSHead->system_header_start_code) + sizeof(pSHead->header_length) + Conversion16ToByteOrder((*(UInt16*)pSHead->header_length), EN_BOT_BIG);
					if (_sData.size() < uOffset)
					{//长度不够
						_uWaitBytes = uOffset - _sData.size();
						goto gt_end;
					}

					psm_header_t* pSMHead = (psm_header_t*)&_sData[uOffset];
					if (is_psm_header(pSMHead))
					{//program_stream_map
						uOffset += sizeof(pSMHead->promgram_stream_map_start_code) + sizeof(pSMHead->program_stream_map_length) + Conversion16ToByteOrder((*(UInt16*)pSMHead->program_stream_map_length), EN_BOT_BIG);
						if (_sData.size() < uOffset)
						{//长度不够
							_uWaitBytes = uOffset - _sData.size();
							goto gt_end;
						}

						_StreamInfoDesc = GetProgramStreamInfoDesc(pSMHead);
						GetElementaryStreamInfo(pSMHead);
						if (_sData.size() - uOffset > 0)
						{
							std::memmove(&_sData[0], &_sData[uOffset], _sData.size() - uOffset);
							_sData.update_size(_sData.size() - uOffset);

							if (_sData.size() > sizeof(pes_header_t) + sizeof(optional_pes_header))
								goto gt_start;
						}
						else
							_sData.clear();
					}
				}
			}
			else if (is_pes_header((pes_header_t*)_sData.c_str()))
			{
				int iRet = ParsePES(&_sData[uOffset], _sData.size() - uOffset);
				if (iRet < 0)
				{//长度不够
					_uWaitBytes = abs(iRet);
					goto gt_end;
				}

				if (iRet > 0)
				{//还有剩余数据处理
					std::memmove(&_sData[0], &_sData[_sData.size() - iRet], iRet);
					_sData.update_size(iRet);
					if (_sData.size() > sizeof(pes_header_t) + sizeof(optional_pes_header))
						goto gt_start;
				}
				else
					_sData.clear();
			}
			else
			{
				const char c_PSStartCode[] = { 0x00, 0x00, 0x01, 0xBA };
				const char c_PESVideoStartCode[] = { 0x00, 0x00, 0x01, 0xE0 };
				const char c_PESAudioStartCode[] = { 0x00, 0x00, 0x01, 0xC0 };
				int iPos = _sData.find(c_PSStartCode, 0, sizeof(c_PSStartCode));
				if (iPos > 0)
				{//找到PS头
					std::memmove(&_sData[0], &_sData[iPos], _sData.size() - iPos);
					_sData.update_size(_sData.size() - iPos);
					goto gt_end;
				}

				iPos = _sData.find(c_PESVideoStartCode, 0, sizeof(c_PESVideoStartCode));
				if (iPos > 0)
				{//找到PES 视频
					std::memmove(&_sData[0], &_sData[iPos], _sData.size() - iPos);
					_sData.update_size(_sData.size() - iPos);
					goto gt_end;
				}

				iPos = _sData.find(c_PESAudioStartCode, 0, sizeof(c_PESAudioStartCode));
				if (iPos > 0)
				{//找到PES 音频
					std::memmove(&_sData[0], &_sData[iPos], _sData.size() - iPos);
					_sData.update_size(_sData.size() - iPos);
					goto gt_end;
				}

				_sData.clear();
				return -1;
			}
		}
	}
	break;
	case PSStatus::ps_pes:
	{
		if (_uWaitBytes > 0)
		{
			PESPayload((pes_header_t*)_PESHeader, c_szData, __min(_uWaitBytes, uSize));
		}

		if (_uWaitBytes <= uSize)
		{
			_sData.clear();
			if (_uWaitBytes < uSize)
			{
				_sData.append(&c_szData[_uWaitBytes], uSize - _uWaitBytes);
			}

			_uStatus = PSStatus::ps_ps;
			_uWaitBytes = 0;

			if (_sData.size() > sizeof(pes_header_t) + sizeof(optional_pes_header))
				goto gt_start;
			goto gt_end;
		}

		_uWaitBytes -= __min(_uWaitBytes, uSize);
	}
	break;
	}

gt_end:

	return 1;
}

Int32	PSPacket::ParsePES(const char* c_szData, Int32 iSize)
{
	pes_header_t* pPesHead = (pes_header_t*)c_szData;
	Int32 iOffset = sizeof(pes_header_t) + sizeof(optional_pes_header);
	if (iSize < iOffset)
	{
		return iSize - iOffset;
	}

	Int32 PES_packet_length = Conversion16ToByteOrder(pPesHead->PES_packet_length, EN_BOT_BIG);

	optional_pes_header* pOptional = (optional_pes_header*)((char*)&pPesHead->PES_packet_length + sizeof(pPesHead->PES_packet_length));

	iOffset += pOptional->PES_header_data_length;
	if (iSize < iOffset)
	{
		return iSize - iOffset;
	}

	_uStatus = PSStatus::ps_pes;
	memcpy((void*)_PESHeader, (void*)pPesHead, sizeof(pes_header_t) + sizeof(optional_pes_header));

	if (iSize < iOffset + PES_packet_length)
	{//长度不够
		PESPayload(pPesHead, &c_szData[iOffset], iSize - iOffset);
		return iSize - (PES_packet_length + sizeof(pes_header_t));
	}

	Int32 iPayloadLen = PES_packet_length - sizeof(optional_pes_header) - pOptional->PES_header_data_length;
	PESPayload(pPesHead, &c_szData[iOffset], iPayloadLen);
	return iSize - (iOffset + iPayloadLen);
}

int	PSPacket::PESPayload(pes_header_t* pPesHead, const char* c_szData, Int32 iSize)
{
	UInt16 PES_packet_length = Conversion16ToByteOrder(pPesHead->PES_packet_length, EN_BOT_BIG);

	optional_pes_header* pOptional = (optional_pes_header*)((char*)&pPesHead->PES_packet_length + sizeof(pPesHead->PES_packet_length));

	UInt64 pts = get_pts(pOptional);

	switch (pes_type(pPesHead))
	{
	case PSStatus::ps_pes_video://视频
	case PSStatus::ps_pes_audio://音频
	case PSStatus::ps_pes_private://私有流
	{
		if (_hData)
		{
			_hData(pes_type(pPesHead), c_szData, iSize);
		}
	}
	break;
	default:
	{
		printf("PSPacket::Parse(%d) Unknow\r\n", pes_type(pPesHead));
	}
	break;
	}

	return 1;
}

_string_type PSPacket::GetProgramStreamInfoDesc(const psm_header_t* pHead)
{
	return _string_type((char*)&pHead->program_stream_info_length + sizeof(pHead->program_stream_info_length),
		Conversion16ToByteOrder((*(UInt16*)pHead->program_stream_info_length), EN_BOT_BIG));
}

void PSPacket::GetElementaryStreamInfo(const psm_header_t* pHead)
{
	char* pstream_map_length = (char*)&pHead->program_stream_info_length + sizeof(pHead->program_stream_info_length) + Conversion16ToByteOrder((*(UInt16*)pHead->program_stream_info_length), EN_BOT_BIG);
	char* pStart = pstream_map_length + 2;
	UInt16 uMapLength = Conversion16ToByteOrder((*(UInt16*)pstream_map_length), EN_BOT_BIG);

	if (uMapLength > 0)
	{
		UInt16 uOffset = 0;
		while (uOffset < uMapLength)
		{
			tagElementaryStreamInfo Info;
			//1、MPEG - 4 视频流： 0x10
			//2、H.264 视频流： 0x1B
			//3、SVAC 视频流： 0x80
			//4、G.711 音频流： 0x90
			//5、G.722.1 音频流： 0x92
			//6、G.723.1 音频流： 0x93
			//7、G.729 音频流： 0x99
			//8、SVAC音频流： 0x9B
			Info.stream_type = pStart[uOffset];

			//（这个字段的定义，其中0x(C0~DF)指音频，0x(E0~EF)为视频）
			Info.elementary_stream_id = pStart[uOffset + 1];

			UInt16 uStrLen = Conversion16ToByteOrder((*(UInt16*)&pStart[uOffset + 2]), EN_BOT_BIG);
			Info.desc = _string_type(&pStart[uOffset + 4], uStrLen);
			uOffset += 4 + uStrLen;

			_StreamInfos.push_back(Info);
		};
	}
}
