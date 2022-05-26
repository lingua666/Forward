#include <libThirdWrap_FFmpeg/Flv_Parse.h>

namespace _ffmpeg_{

	FlvParse::FlvParse( void )
		: _uOffset( 0 )
		, _uSize( 0 )
		, _pData( NULL )
	{
		_pData = new char[0xFFFF];
		memset(_pData, 0, 0xFFFF);
		memset(&_Tag, 0, sizeof(_Tag));
		_iStatus = STATUS_HEADER;
	}

	FlvParse::~FlvParse( void )
	{
		Release();
	}

	int FlvParse::Init( const HFNData& hData )
	{
		_hData = hData;
		return 1;
	}

	void FlvParse::Release( void )
	{
		if( _pData != NULL )
		{
			delete _pData;
			_pData = NULL;
		}
	}

	int FlvParse::Parse( const char* c_szData, UInt32 uSize )
	{
		int iRemain = 0;
		char* pData = (char*)c_szData;

gt_start:
		switch (_iStatus)
		{
		case STATUS_HEADER:
			{
				if( _Tag.DataSize == 0 )
				{//ͷ����Ϣ������
					if( _uSize + uSize >= sizeof(FLV_HEADER) )
					{//ͷ����Ϣ����
						FLV_HEADER* pHead = NULL;
						if( _uSize == 0 )
						{
							pHead = (FLV_HEADER*)c_szData;
							_Tag.DataSize = BigEndian32_UInt32(pHead->DataOffset);
							iRemain = uSize - _Tag.DataSize ;
							if( iRemain > 0 )
								pData = (char*)&c_szData[_Tag.DataSize];
						}
						else
						{
							memcpy(&_pData[_uSize], c_szData, uSize);
							_uSize += uSize;
							pHead = (FLV_HEADER*)_pData;

							_Tag.DataSize = BigEndian32_UInt32(pHead->DataOffset);
							iRemain = _uSize - _Tag.DataSize ;
							if( iRemain > 0 )
								pData = &_pData[_Tag.DataSize];
						}

						if( iRemain >= 0 )
						{//����������ͷ����Ϣ
							_iStatus = STATUS_AVC_HEAD;
							_uSize = _Tag.DataSize = 0;
							if( iRemain > 0 )
							{//����ʣ���������Ϊ��һ�׶ε�����
								memcpy(_pData, pData, iRemain);
								uSize = iRemain;
								goto gt_start;
							}
						}
					}
					else
					{//ͷ����Ϣ������
						memcpy(&_pData[_uSize], c_szData, uSize);
						_uSize += uSize;
					}
				}
				else
				{//ͷ����Ϣ����
					memcpy(&_pData[_uSize], c_szData, uSize);
					_uSize += uSize;

					if( _uSize >= _Tag.DataSize )
					{
						_iStatus = STATUS_AVC_HEAD;
						iRemain = _uSize - _Tag.DataSize ;
						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							memcpy(_pData, &_pData[_Tag.DataSize], iRemain);
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
			}
			break;
		case STATUS_AVC_HEAD:
			{
				memcpy(&_pData[_uSize], pData, uSize);
				_uSize += uSize;

				if( _Tag.DataSize == 0 )
				{
					if( _uSize + uSize >= sizeof(TAG_HEADER) + 4 )
					{
						TAG_HEADER* pHead = (TAG_HEADER*)&_pData[4];
						_Tag.TagType = pHead->TagType;
						_Tag.DataSize = BigEndian32_UInt32(pHead->DataSize << 8);
						_Tag.Timestamp = pHead->Timestamp;
						memcpy(_Tag.StreamID, pHead->StreamID, sizeof(pHead->StreamID));
					}

					int iLen = 4 + sizeof(TAG_HEADER) + _Tag.DataSize;
					if( _uSize >= iLen )
					{//��������
						iRemain = _uSize - iLen ;

						//��ȡͷ��ʵ�ʳ���
						ParseAvcVideo(_Tag, (unsigned char*)&_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);
						_iStatus = STATUS_AVC_DATA;
						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
				else
				{
					int iLen = sizeof(TAG_HEADER) + 4 + _Tag.DataSize;
					if( _uSize >= iLen )
					{//��������
						iRemain = _uSize - iLen ;
						ParseAvcVideo(_Tag, (unsigned char*)&_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);
						_iStatus = STATUS_AVC_DATA;
						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
			}
			break;
		case STATUS_AVC_DATA:
			{
				memcpy(&_pData[_uSize], pData, uSize);
				_uSize += uSize;

				if( _Tag.DataSize == 0 )
				{
					if( _uSize >= sizeof(TAG_HEADER) + 4 )
					{
						TAG_HEADER* pHead = (TAG_HEADER*)&_pData[4];
						_Tag.TagType = pHead->TagType;
						_Tag.DataSize = BigEndian32_UInt32(pHead->DataSize << 8);

						//test
						if(3592 == _Tag.DataSize )
						{
							int i = 0;
						}
						//test

						_Tag.Timestamp = pHead->Timestamp;
						memcpy(_Tag.StreamID, pHead->StreamID, sizeof(pHead->StreamID));
					}

					int iLen = sizeof(TAG_HEADER) + 4 + _Tag.DataSize;
					if( _uSize >= iLen )
					{//��������
						iRemain = _uSize - iLen ;
						ParseAvcVideo(_Tag, (unsigned char*)&_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);

						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
				else
				{
					int iLen = sizeof(TAG_HEADER) + 4 + _Tag.DataSize;
					if( _uSize >= iLen )
					{//��������
						iRemain = _uSize - iLen ;
						ParseAvcVideo(_Tag, (unsigned char*)&_pData[sizeof(TAG_HEADER) + 4], _Tag.DataSize);

						_uSize = _Tag.DataSize = 0;
						if( iRemain > 0 )
						{
							pData =  &_pData[iLen];
							uSize = iRemain;
							goto gt_start;
						}
					}
				}
			}
			break;
		}

		return 1;
	}

	int FlvParse::ParseAvcVideo( const TAG_HEADER& TagHead, const unsigned char* c_szData, UInt32 uSize )
	{
		switch(TagHead.TagType)
		{
		case 0x8://��Ƶ����

			break;
		case 0x9://��Ƶ����
			{
				UInt8 uCodeID = c_szData[0] & 0x0F;
				switch( uCodeID )
				{
				case 7: //AVC1
					{
						AVC_HEADER* pHead = (AVC_HEADER*)c_szData;
						if( pHead->AvcType == 0 )
						{//Я��pps��Ϣ
							//AVC_FIRST_PACKET* pPacket = (AVC_FIRST_PACKET*)&c_szData[sizeof(AVC_HEADER)];
							AVC_HEAD_INFO* pHead = (AVC_HEAD_INFO*)&c_szData[sizeof(AVC_HEADER)];
							int iHeadLen = BigEndian16_UInt16(pHead->Length);

							UInt32 uLen = sizeof(AVC_HEADER) + sizeof(AVC_HEAD_INFO) - 1 + iHeadLen + sizeof(AVC_PPS_INFO) - 1;
							if( uSize < uLen )
							{//���ݲ�����
								return uLen;
							}

							AVC_PPS_INFO* pPPS = (AVC_PPS_INFO*)(pHead->Data + iHeadLen);
							int iPPSLen = BigEndian16_UInt16(pPPS->PPSSize);

							uLen += iPPSLen;
							if( uSize < uLen  )
							{//���ݲ�����
								return uLen;
							}

							//��ȡAVCͷ����Ϣ
							if( _hData )
							{
								_string_type s;
								s.append((char*)g_uH264Head, sizeof(g_uH264Head));
								s.append((char*)pHead->Data, iHeadLen);
								_hData((unsigned char*)s.c_str(), s.size());

								//��ȡAVC PPS��Ϣ
								s.clear();
								s.append((char*)g_uH264Head, sizeof(g_uH264Head));
								s.append((char*)pPPS->Data, iPPSLen);
								_hData((unsigned char*)s.c_str(), s.size());
							}
						}
						else
						{//
							int PendingSize = 0;
							_string_type s;
							while( PendingSize + 2 + 3 < TagHead.DataSize )
							{//���ܴ��ڶ��nal����ȫ����ȡ  
								AVC_DATA_INFO* pData = (AVC_DATA_INFO*)&c_szData[sizeof(AVC_HEADER) + PendingSize];
								int iDataLen = BigEndian32_UInt32(pData->Length);

								//�ص�����
								if( _hData )
								{
									s.clear();
									s.append((char*)g_uH264Head, sizeof(g_uH264Head));
									s.append((char*)pData->Data, iDataLen);
									_hData((unsigned char*)s.c_str(), s.size());
								}

								PendingSize += iDataLen + sizeof(AVC_DATA_INFO) - 1;
							}
						}
					}
					break;
				case 4://VP6F
					{
						if( _hData )
						{
							_string_type s;
							s.append( ((char*)c_szData) + 2, TagHead.DataSize - 2);
							_hData((unsigned char*)s.c_str(), s.size());
						}
					}
					break;
				default:
					{
						
					}
					break;
				}
			}
			break;
		case 0x12://�ű�����

			break;
		}

		return -1;
	}

} //namespace _ffmpeg_