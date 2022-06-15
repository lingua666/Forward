
#include <libThirdWrap_FFmpeg/PSPacket.h>

#ifndef AV_RB16
#   define AV_RB16(x)                           \
    ((((const unsigned char*)(x))[0] << 8) |          \
    ((const unsigned char*)(x))[1])
#endif


/*TSPacket::TSPacket(void)
	: _uWaitBytes(sizeof(ps_header_t))
	, _uStatus(ts_header)
{
	
}

TSPacket::~TSPacket(void)
{

}

int TSPacket::Init(const HFNData& hData)
{
	_hData = hData;
	return 1;
}

void TSPacket::Release(void)
{

}

int TSPacket::Parse(const char* c_szData, UInt32 uSize)
{
	int iOffset = 0;
	_sData.append(c_szData, uSize);
	switch (_uStatus)
	{
	case TSStatus::ts_header:
	{
		if (_sData.size() < 4)
			return -1;
		
		if (_sData[0] != 0x47)
		{
			printf("TSPacket::Parse() Header Error, ts: sync_bytes must be 0x47, actual=%#x\r\n", _sData[0]);
			return -1;
		}

		tagTSHeader Header;
		memcpy((void*)&Header, _sData.c_str(), sizeof(tagTSHeader));
		iOffset = sizeof(tagTSHeader);

		// optional: adaptation field
		if (Header.adaptation_field_control == TSAdaptationFieldType::TSAdaptationFieldTypeAdaptionOnly ||
			Header.adaptation_field_control == TSAdaptationFieldType::TSAdaptationFieldTypeBoth)
		{
			if (_sData.size() - iOffset < sizeof(tagTSAdaptationField))
			{//长度不够
				return -1;
			}

			TSAdaptationField AdaptField;
			int iLen = AdaptField->Parse(&_sData[iOffset], _sData.size() - iOffset);
			if (iLen == -1)
			{
				return -1;
			}

		}
	}
	break;
	}

	return 1;
}

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
}*/
