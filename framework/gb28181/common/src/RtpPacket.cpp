
#include <libFrameWork_Rtsp/RtpPacket.h>
#include <libFrameWork_Rtsp/H264Info.h>
#include <libFrameWork_Rtsp/H265Info.h>

RtpPacket::RtpPacket( void )
	: _uSeq( 0 )
	, _uSSRC( 0 )
	, _pIVideoInfo( NULL )
	, _pNalu_Header( NULL )
{
	memset(&_Head, 0, sizeof(_Head));
}

RtpPacket::~RtpPacket( void )
{
	Release();
}

void RtpPacket::Init( UInt32 uSSRC, UInt8 uFrameRate,
	UInt32 uFrequency, UInt8 uVideoType )
{
	_uSSRC = uSSRC;
	_uStep = uFrameRate > 0 ? uFrequency / uFrameRate : 3600;
	_Timestamp = Timestamp_type().epochMicroseconds();
	if( uVideoType == IVideoInfo::ENUM_ENC_H264 )
	{
		_pIVideoInfo = IVideoInfo::GetMediaInfoPoolInstance()->AllocObj<H264Info>();
		if( _pIVideoInfo != NULL )
		{
			_pNalu_Header = IVideoInfo::GetMediaInfoPoolInstance()->AllocObj<tagNALU_HEADER>();
			if( _pNalu_Header != NULL )
			{
				((tagNALU_HEADER*)_pNalu_Header)->TYPE = 0;
			}
		}
	}
	else if( uVideoType == IVideoInfo::ENUM_ENC_H265 )
	{
		_pIVideoInfo = IVideoInfo::GetMediaInfoPoolInstance()->AllocObj<H265Info>();
		if( _pIVideoInfo != NULL )
		{
			_pNalu_Header = IVideoInfo::GetMediaInfoPoolInstance()->AllocObj<tagNALU265_HEADER>();
			if( _pNalu_Header != NULL )
			{
				((tagNALU265_HEADER*)_pNalu_Header)->TYPE = 0;
			}
		}
	}
}

void RtpPacket::Release( void )
{
	if( _pIVideoInfo != NULL )
	{
		if( _pNalu_Header != NULL )
		{
			if( _pIVideoInfo->GetVideoType() == IVideoInfo::ENUM_ENC_H264  )
			{
				IVideoInfo::GetMediaInfoPoolInstance()->FreeObj<tagNALU_HEADER>((tagNALU_HEADER*)_pNalu_Header);
				_pNalu_Header = NULL;
			}
			else if( _pIVideoInfo->GetVideoType() == IVideoInfo::ENUM_ENC_H265 )
			{
				IVideoInfo::GetMediaInfoPoolInstance()->FreeObj<tagNALU265_HEADER>((tagNALU265_HEADER*)_pNalu_Header);
				_pNalu_Header = NULL;
			}
			else
			{
				IVideoInfo::GetMediaInfoPoolInstance()->Free(_pNalu_Header);
			}
		}

		IVideoInfo::GetMediaInfoPoolInstance()->FreeObj(_pIVideoInfo);
		_pIVideoInfo = NULL;
	}
}

int RtpPacket::Parse( char* c_szData, UInt32 uLen, tagVideoPlayload_INFO* pPlayloadInfo )
{
	int iRet = isVaild(c_szData, uLen);
	if( iRet > 0 )
	{//
		tagRTP_HEAD* pHead = (tagRTP_HEAD*)(c_szData);
		if( (pHead->PT == 96 || pHead->PT == 97 || pHead->PT == 98) && uLen > iRet )
		{//H264
			//if( !_Info.isReady() )
			{
				if( pHead->P == 1 )
				{//���������
					UInt8 uPaddingLen = 0;
					if( c_szData[uLen - 1] > 0 )
					{
						uPaddingLen = c_szData[uLen - 1];
						int iSize = uLen - iRet - uPaddingLen;
						if( iSize > 0 )
						{
							_pIVideoInfo->Parse(&c_szData[iRet], iSize, pPlayloadInfo);
						}
					}
				}
				else
				{
					if( uLen - iRet > 0 )
					{
						_pIVideoInfo->Parse(&c_szData[iRet], uLen - iRet, pPlayloadInfo);
					}
					else
					{
						return -1;
					}
				}

				if( _pIVideoInfo != NULL && _pIVideoInfo->isReady() )
					return 1;
			}
		}
	}
	else if( iRet == 0 )
	{//��ʽ��ȷ�����ݰ�������
		return 0;
	}

	return -1;
}

PKBuf_ptr RtpPacket::GetPacket_OneNaul( UInt8 uType, const char* c_szData, UInt32 uLen )
{
	tagRTP_HEAD	Head = {0};
	Head.Ver = RTP_VERSION;
	//����������¿���
	//��Ҫ��䣬һ��ĳЩ�����㷨Ҫ�����ݿ��С�̶�
	//������һ���Ͳ�Э�����ݰ���װ�ض��RTP ���顣
	//NAL��Ԫ�Ĵ�С����RTP��Ч���أ�һ��1460����
	//�ָ�����һ����Ԫ�Ĵ�С���ܻ�С��N��
	//��ʱ����ʹ��RTP�غ��������ͬǰ��ķֿ��С��ͬ��
	//��ʱRTPͷ�е�����ʶλֵΪ1
	//�ָ��㷨����:
	/*if( (NalSize % RTP_MTU) > 0 )
		K = (NalSize / RTP_MTU) + 1;
	else
		K = NalSize / RTP_MTU;
	if( (NalSize % K) > 0 )
		N = NalSize % K + 1;
	else
		N = NalSize / K;*/

	Head.P = 0;
	Head.X = 0;
	Head.CC = 0;
	Head.M = 1;
	Head.PT = uType;
	Head.SequenceNumber = BigEndian16_UInt16(_uSeq);
	Head.Timestamp = BigEndian32_UInt32(_Timestamp);
	if( Head.M == 1 )
		_Timestamp += _uStep;
	Head.SSRC = _uSSRC;

	PKBuf_ptr sptr = Alloc();
	sptr->_uPayload = sizeof(tagRTP_HEAD) + uLen;
	memcpy(sptr->_Data, &Head, sizeof(tagRTP_HEAD));
	memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD)]), c_szData, uLen);

	_uSeq ++;
	return sptr;
}

PKBuf_ptr RtpPacket::GetPacket_FUs( UInt8 uType, bool isMark,
	const char* c_szData, UInt32 uLen )
{
	tagRTP_HEAD	Head = {0};
	Head.Ver = RTP_VERSION;
	//����������¿���
	//��Ҫ��䣬һ��ĳЩ�����㷨Ҫ�����ݿ��С�̶�
	//������һ���Ͳ�Э�����ݰ���װ�ض��RTP ���顣
	//NAL��Ԫ�Ĵ�С����RTP��Ч���أ�һ��1460����
	//�ָ�����һ����Ԫ�Ĵ�С���ܻ�С��N��
	//��ʱ����ʹ��RTP�غ��������ͬǰ��ķֿ��С��ͬ��
	//��ʱRTPͷ�е�����ʶλֵΪ1
	//�ָ��㷨����:
	/*if( (NalSize % RTP_MTU) > 0 )
		K = (NalSize / RTP_MTU) + 1;
	else
		K = NalSize / RTP_MTU;
	if( (NalSize % K) > 0 )
		N = NalSize % K + 1;
	else
		N = NalSize / K;*/

	Head.P = 0;
	Head.X = 0;
	Head.CC = 0;
	Head.M = isMark ? 1 : 0;
	Head.PT = uType;
	Head.SequenceNumber = BigEndian16_UInt16(_uSeq);
	Head.Timestamp = BigEndian32_UInt32(_Timestamp);
	if( Head.M == 1 )
		_Timestamp += _uStep;
	Head.SSRC = _uSSRC;

	PKBuf_ptr sptr = Alloc();
	memcpy(sptr->_Data, &Head, sizeof(tagRTP_HEAD));

	bool isBegin = false;
	if( _pIVideoInfo != NULL )
	{
		if( _pIVideoInfo->GetVideoType() == IVideoInfo::ENUM_ENC_H264  )
		{
			tagNALU_HEADER* pHeader = (tagNALU_HEADER*)_pNalu_Header;
			if( pHeader->TYPE == 0 )
			{//��һ����
				isBegin = true;
				memcpy(pHeader, c_szData, sizeof(pHeader));
			}

			//FUs Header
			tagFU_A_HEADER Fu_Header = {0};
			memcpy(&Fu_Header.Indicator, pHeader, sizeof(tagNALU_HEADER));
			Fu_Header.Indicator.TYPE = NAL_UNIT_TYPE_FU_A;
			Fu_Header.TYPE = pHeader->TYPE;
			if(Head.M == 1)
			{
				Fu_Header.E = 1;
				Fu_Header.S = 0;
				pHeader->TYPE = 0;
				//sptr->_uPayload = sizeof(tagRTP_HEAD) + uLen + 2;
				sptr->_uPayload = sizeof(tagRTP_HEAD) + sizeof(Fu_Header) + uLen;
			}
			else
			{
				Fu_Header.E = 0;
				if( isBegin )
				{
					Fu_Header.S = 1;
					//sptr->_uPayload = sizeof(tagRTP_HEAD) + uLen + 1;
					sptr->_uPayload = sizeof(tagRTP_HEAD) + sizeof(Fu_Header) - sizeof(tagNALU_HEADER) + uLen;
				}
				else
				{
					Fu_Header.S = 0;
					sptr->_uPayload = sizeof(tagRTP_HEAD) + sizeof(Fu_Header) + uLen;
				}
			}

			memcpy(sptr->_Data, &Head, sizeof(tagRTP_HEAD));
			memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD)]), &Fu_Header, sizeof(Fu_Header));

			if( isBegin )
			{
				//memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD) + 2]), &c_szData[1], uLen - 1);
				memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD) + 2]), &c_szData[sizeof(tagNALU_HEADER)], uLen - sizeof(tagNALU_HEADER));
			}
			else
			{
				memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD) + 2]), c_szData, uLen);
			}
		}
		else if( _pIVideoInfo->GetVideoType() == IVideoInfo::ENUM_ENC_H265  )
		{
			tagNALU265_HEADER* pHeader = (tagNALU265_HEADER*)_pNalu_Header;
			if( pHeader->TYPE == 0 )
			{//��һ����
				isBegin = true;
				memcpy(pHeader, c_szData, sizeof(pHeader));
			}

			//FUs Header
			tagFU265_A_HEADER Fu_Header = {0};
			memcpy(&Fu_Header.Indicator, pHeader, sizeof(tagNALU265_HEADER));
			Fu_Header.Indicator.TYPE = NAL_UNIT_UNSPECIFIED_49;//49;
			Fu_Header.TYPE = pHeader->TYPE;
			if(Head.M == 1)
			{
				Fu_Header.E = 1;
				Fu_Header.S = 0;
				pHeader->TYPE = 0;
				sptr->_uPayload = sizeof(tagRTP_HEAD) + sizeof(Fu_Header) + uLen ;
			}
			else
			{
				Fu_Header.E = 0;
				if( isBegin )
				{
					Fu_Header.S = 1;
					sptr->_uPayload = sizeof(tagRTP_HEAD) + sizeof(Fu_Header) - sizeof(tagNALU265_HEADER) + uLen;
				}
				else
				{
					Fu_Header.S = 0;
					sptr->_uPayload = sizeof(tagRTP_HEAD) + sizeof(Fu_Header) + uLen;
				}
			}

			memcpy(sptr->_Data, &Head, sizeof(tagRTP_HEAD));
			memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD)]), &Fu_Header, sizeof(Fu_Header));

			if( isBegin )
				memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD) + sizeof(Fu_Header)]),
				&c_szData[sizeof(tagNALU265_HEADER)], uLen - sizeof(tagNALU265_HEADER));
			else
				memcpy(&(sptr->_Data[sizeof(tagRTP_HEAD) + sizeof(Fu_Header)]), c_szData, uLen);

			//test
			//printf("H265Info::Parse(%x,%d)\r\n", sptr->_Data[sizeof(tagRTP_HEAD)], sptr->_uPayload - sizeof(tagRTP_HEAD));
			//printf("H265Info::Parse(%d) hex:%s\r\n", sptr->_uPayload, _string_type::HexToStr(sptr->_Data,sptr->_uPayload).c_str());
			//Sleep(3000);
			//test
		}
	}

	_uSeq ++;

	return sptr;
}

int RtpPacket::isVaild( const char* c_szData, UInt32 uLen )
{
	if( uLen < sizeof(tagRTP_HEAD) )
		return -1;

	tagRTP_HEAD* pHead = (tagRTP_HEAD*)(c_szData);
	if( pHead->Ver != RTP_VERSION )
		return -1;

	UInt32 uXLen = 0;//= 4 * pHead->CSRC;
	if( pHead->X == 1 )
	{//������չͷ
		uXLen += sizeof(UInt32);
		if( uLen < uXLen + sizeof(tagRTP_HEAD) )
			return 0;

		uXLen += 4 * (c_szData[uXLen - 2] << 8 | c_szData[uXLen - 1]); 
	}

	if( uLen < uXLen + sizeof(tagRTP_HEAD) )
		return 0;

	memcpy(&_Head, c_szData, sizeof(tagRTP_HEAD));

#if defined( LITTLE_ENDIAN_DEF )
	_Head.SequenceNumber = BigEndian16_UInt16(_Head.SequenceNumber);
	_Head.SSRC = BigEndian32_UInt32(_Head.SSRC);
	_Head.Timestamp = BigEndian32_UInt32(_Head.Timestamp);
#endif
	//��Ч����ƫ����
	return (uXLen + sizeof(tagRTP_HEAD));
}

_string_type RtpPacket::GetVideoHeadBuffer( void )
{
	return _pIVideoInfo->GetHeadBuffer();
}

