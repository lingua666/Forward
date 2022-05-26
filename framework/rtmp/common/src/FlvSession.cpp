
#include <libFrameWork_Rtsp/H264Info.h>
#include <libFrameWork_RtmpCommon/FlvSession.h>


namespace _rtmp_ {

	FlvSession::FlvSession(void)
		: IRtmpSession()
		, _uDstSize(0)
		, _TimeVal(0)
	{
		//LOG_Print_SetLog(libFrameWork_RtmpCommon, MLog_GetAllLevel());

		memset(&_RTMPPacket, 0, sizeof(_RTMPPacket));
	}

	FlvSession::~FlvSession(void)
	{
		Release();

		if (_hEvent)
			_hEvent(RTMP_STATUS_Destroy, shared_from_this(), NULL);
	}

	int FlvSession::Init(const HFNEvent& hEvent)
	{
		_pVideo = new (std::nothrow) H264Info();
		if (_pVideo == NULL)
		{
			LOG_Print_Info(libFrameWork_RtmpCommon, "FlvSession::Init() new H264Info Failed!!");
			return -1;
		}

		_VideoInfo.uVideoType = 0;
		_VideoInfo.uWidth = 0;
		_VideoInfo.uHeight = 0;
		_VideoInfo.uMax_Framerate = 0;
		_VideoInfo.uRemain = 0;
		_VideoInfo.iWaitTime = 0;
		_VideoInfo.SndData.clear();
		_hEvent = hEvent;
		return 1;
	}

	void FlvSession::Release(void)
	{
		Close();
	}

	int FlvSession::Connect(const _rtmp_string& sUrl)
	{
		RTMP_Init(&_Rtmp);

		/*设置URL*/
		if (RTMP_SetupURL(&_Rtmp, (char*)sUrl.c_str()) == FALSE)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::Connect() SetupURL:%s Failed!");
			return -1;
		}

		/*设置可写,即发布流,这个函数必须在连接前使用,否则无效*/
		RTMP_EnableWrite(&_Rtmp);

		/*连接服务器*/
		if (ConnectImpl(&_Rtmp, NULL, sUrl) == -1)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::Connect() ConnectImpl:%s Failed!");
			return -1;
		}

		return 1;
	}

	int FlvSession::Close(bool isPassive)
	{
		if (_isOpen)
		{
			_isOpen = false;

			int i;

			if (_Rtmp.m_stream_id > 0)
			{
				if ((_Rtmp.Link.protocol & RTMP_FEATURE_WRITE))
					SendFCUnpublish(&_Rtmp);
				i = _Rtmp.m_stream_id;
				_Rtmp.m_stream_id = 0;
				SendDeleteStream(&_Rtmp, i);
			}

			if (_Rtmp.m_clientID.av_val)
			{
				HTTP_Post(&_Rtmp, RTMPT_CLOSE, "", 1);
				free(_Rtmp.m_clientID.av_val);
				_Rtmp.m_clientID.av_val = NULL;
				_Rtmp.m_clientID.av_len = 0;
			}

			GetAsynStreamClientsInstance()->Close(_hNet);

			_Rtmp.m_stream_id = -1;
			_Rtmp.m_sb.sb_socket = -1;
			_Rtmp.m_nBWCheckCounter = 0;
			_Rtmp.m_nBytesIn = 0;
			_Rtmp.m_nBytesInSent = 0;

			if (_Rtmp.m_read.flags & RTMP_READ_HEADER) {
				free(_Rtmp.m_read.buf);
				_Rtmp.m_read.buf = NULL;
			}
			_Rtmp.m_read.dataType = 0;
			_Rtmp.m_read.flags = 0;
			_Rtmp.m_read.status = 0;
			_Rtmp.m_read.nResumeTS = 0;
			_Rtmp.m_read.nIgnoredFrameCounter = 0;
			_Rtmp.m_read.nIgnoredFlvFrameCounter = 0;

			_Rtmp.m_write.m_nBytesRead = 0;
			RTMPPacket_Free(&_Rtmp.m_write);

			for (i = 0; i < RTMP_CHANNELS; i++)
			{
				if (_Rtmp.m_vecChannelsIn[i])
				{
					RTMPPacket_Free(_Rtmp.m_vecChannelsIn[i]);
					free(_Rtmp.m_vecChannelsIn[i]);
					_Rtmp.m_vecChannelsIn[i] = NULL;
				}
				if (_Rtmp.m_vecChannelsOut[i])
				{
					free(_Rtmp.m_vecChannelsOut[i]);
					_Rtmp.m_vecChannelsOut[i] = NULL;
				}
			}
			AV_clear(_Rtmp.m_methodCalls, _Rtmp.m_numCalls);
			_Rtmp.m_methodCalls = NULL;
			_Rtmp.m_numCalls = 0;
			_Rtmp.m_numInvokes = 0;

			_Rtmp.m_bPlaying = FALSE;
			_Rtmp.m_sb.sb_size = 0;

			_Rtmp.m_msgCounter = 0;
			_Rtmp.m_resplen = 0;
			_Rtmp.m_unackd = 0;

			free(_Rtmp.Link.playpath0.av_val);
			_Rtmp.Link.playpath0.av_val = NULL;

			if (_Rtmp.Link.lFlags & RTMP_LF_FTCU)
			{
				free(_Rtmp.Link.tcUrl.av_val);
				_Rtmp.Link.tcUrl.av_val = NULL;
				_Rtmp.Link.lFlags ^= RTMP_LF_FTCU;
			}

#ifdef CRYPTO
			if (_Rtmp.Link.dh)
			{
				MDH_free(_Rtmp.Link.dh);
				_Rtmp.Link.dh = NULL;
			}
			if (_Rtmp.Link.rc4keyIn)
			{
				RC4_free(_Rtmp.Link.rc4keyIn);
				_Rtmp.Link.rc4keyIn = NULL;
			}
			if (_Rtmp.Link.rc4keyOut)
			{
				RC4_free(_Rtmp.Link.rc4keyOut);
				_Rtmp.Link.rc4keyOut = NULL;
			}
#endif
			if (_hEvent)
				_hEvent(isPassive ? RTMP_STATUS_Passive_Closing : RTMP_STATUS_Closing,
					shared_from_this(), NULL);
			return 1;
		}

		return -1;
	}

	int FlvSession::ConnectImpl(RTMP *r, RTMPPacket *cp, const _rtmp_string& sUrl)
	{
		_rtmp_string sHost = _rtmp_string(r->Link.hostname.av_val, r->Link.hostname.av_len);

		r->m_sb.sb_timedout = FALSE;
		r->m_pausing = 0;
		r->m_fDuration = 0.0;

		if (cp != NULL)
		{
			memcpy(&_RTMPPacket, cp, sizeof(RTMPPacket));
		}

		if (GetAsynStreamClientsInstance()->Connect(sHost.c_str(), r->Link.port,
			function20_bind(&FlvSession::HandleConnect, this, shared_from_this(),
				sUrl, sHost, r->Link.port, _function_::_1)) == -1)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::ConnectImpl(%s:%d) Url:%s",
				sHost.c_str(), r->Link.port, sUrl.c_str());
			return -1;
		}

		return 1;
	}

	void FlvSession::HandleConnect(const self_sptr& sptr, const _rtmp_string& sUrl,
		const _rtmp_string& sIP, UInt16 uPort, NETHANDLE hNet)
	{
		if (hNet == -1)
		{//连接失败
			LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::HandleConnect(%s:%d) Url:%s Failed!!",
				sIP.c_str(), uPort, sUrl.c_str());

			if (_hEvent)
				_hEvent(RTMP_STATUS_ERROR_Connect, shared_from_this(), NULL);
			return;
		}
		else
		{
			GetAsynStreamClientsInstance()->SetRecvHandle(hNet,
				function20_bind(&FlvSession::HandleRecv, this, shared_from_this(),
					_function_::_1, _function_::_2, _function_::_3));

			if (GetAsynStreamClientsInstance()->SetDestroyHandle(hNet,
				function20_bind(&FlvSession::HandleDestroy, this, shared_from_this(), _function_::_1)) == -1)
			{//失败
				LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::HandleConnect(%s:%d) SetDestroyHandle Url:%s Failed!!",
					sIP.c_str(), uPort, sUrl.c_str());

				if (_hEvent)
					_hEvent(RTMP_STATUS_ERROR_Connect, shared_from_this(), NULL);
				return;
			}

			_Rtmp.m_bSendCounter = TRUE;
			_isOpen = true;
			_hNet = hNet;
		}

		int iRet = -1;

		if (_RTMPPacket.m_body == NULL)
			iRet = CreateNetConnect(&_Rtmp, NULL);
		else
			iRet = CreateNetConnect(&_Rtmp, &_RTMPPacket);

		//握手
		if (iRet == 1)
		{
			SendHandleHandShake(&_Rtmp, IRtmpSession::RTMP_STATUS_HandShake_C0C1);
		}
		else if(iRet == -1)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::HandleConnect(%s:%d) CreateNetConnect Url:%s Failed!!",
				sIP.c_str(), uPort, sUrl.c_str());

			if (_hEvent)
				_hEvent(RTMP_STATUS_ERROR_Connect, shared_from_this(), NULL);
		}
	}

	int FlvSession::Send(RTMPSockBuf *sb, const char *buf, int len)
	{
		int rc;

#if defined(CRYPTO) && !defined(NO_SSL)
		if (sb->sb_ssl)
		{
			rc = TLS_write(sb->sb_ssl, buf, len);
		}
		else
#endif
		{
			rc = GetAsynStreamClientsInstance()->Send(_hNet, buf, len) == 1 ? len : 0;
		}
		return rc;
	}

	int FlvSession::SendPacket(const char* c_szH264Data, UInt32 uH264Size)
	{
		if (!IsOpen())
			return -1;

		UInt32 uOffset = 0;
		RTMPPacket_sptr Packet_sptr = AllocPacket();
		RTMPPacket* pPacket = Packet_sptr.get();

		PacketList Packets;
		if (FlvSession::ParseH264Data(c_szH264Data, uH264Size, Packets) == 1)
		{
			for (PacketList::iterator iter = Packets.begin(); iter != Packets.end(); iter++)
			{
				uOffset = 0;

				/*分配包内存和初始化,len为包体长度*/
				memset(pPacket, 0, RTMP_HEAD_SIZE);

				/*包体内存*/
				pPacket->m_body = (char *)pPacket + RTMP_HEAD_SIZE;
				pPacket->m_hasAbsTimestamp = 0;
				pPacket->m_packetType = RTMP_PACKET_TYPE_VIDEO; /*此处为类型有两种一种是音频,一种是视频*/
				pPacket->m_nInfoField2 = _Rtmp.m_stream_id;
				pPacket->m_nChannel = 0x04;
				pPacket->m_headerType = RTMP_PACKET_SIZE_LARGE;
				if (_TimeVal == 0)
				{
					pPacket->m_nTimeStamp = _TimeVal / 1000;
					_TimeVal = Timestamp_type().epochMicroseconds();
					_Time.update();
				}
				else
				{
					pPacket->m_nTimeStamp = (Timestamp_type().epochMicroseconds() - _TimeVal) / 1000;
				}

				if ((*iter).isKeyFrame)
				{
					//发送SPS, PPS
					SendSpsPps();
					pPacket->m_body[uOffset++] = 0x017;// 1:Iframe  7:AVC 
				}
				else
				{
					pPacket->m_body[uOffset++] = 0x27;// 2:Pframe  7:AVC
				}

				pPacket->m_body[uOffset++] = 0x01;
				pPacket->m_body[uOffset++] = 0x00;
				pPacket->m_body[uOffset++] = 0x00;
				pPacket->m_body[uOffset++] = 0x00;
				UInt32 u = Conversion32ToByteOrder((*iter).uLen, EN_BOT_BIG);
				memcpy(&pPacket->m_body[uOffset], (char*)&u, sizeof(UInt32));
				uOffset += sizeof(UInt32);
				memcpy(&pPacket->m_body[uOffset], (*iter).pBuf, (*iter).uLen);
				uOffset += (*iter).uLen;

				pPacket->m_nBodySize = uOffset;

				//test
				printf("RtmpSession::SendH264Packet(%d)\r\n", (*iter).uLen);
				/*static FILE* s_fp = fopen("C:\\Users\\2020\\Desktop\\智慧垃圾亭系统设计\\监控\\rtmp\\simplest_librtmp_example\\资源\\test.flv", "wb");
				static _ffmpeg_::FlvConverter	FlvConverter;
				static bool bl = false;
				if ((*iter).isKeyFrame)
				{
					char sps[1024] = { 0 };
					char pps[1024] = { 0 };
					memcpy(sps, _VideoInfo.SPS.c_str(), _VideoInfo.SPS.size());
					memcpy(sps, _VideoInfo.PPS.c_str(), _VideoInfo.PPS.size());

					FlvConverter.Init(_ffmpeg_::_ff_string(_VideoInfo.SPS.c_str(), _VideoInfo.SPS.size()),
						_ffmpeg_::_ff_string(_VideoInfo.PPS.c_str(), _VideoInfo.PPS.size()));

					if (!bl)
					{
						if (s_fp)
						{
							fwrite(FlvConverter.GetFlvHeader().c_str(), 1, FlvConverter.GetFlvHeader().size(), s_fp);

							{
								FILE* fp = fopen("C:\\Users\\2020\\Desktop\\智慧垃圾亭系统设计\\监控\\rtmp\\simplest_librtmp_example\\资源\\output.flv", "rb");
								char szData[1024] = { 0 };
								int iRet = fread(szData, 1, sizeof(szData), fp);

								UInt32 uOffset = 9 + 4 + 1;
								UInt32 u = *(UInt32*)&szData[uOffset];
								u = Conversion32ToByteOrder(u, EN_BOT_BIG) >> 8;
								u += 4 + 4 + 4 + 3;
								char szDa[1024] = { 0 };
								memcpy(szDa, &szData[9], u);
								fwrite(szDa, 1, u, s_fp);
							}

							fflush(s_fp);
						}
						bl = true;
					}
				}

				if (s_fp)
				{
					UInt32 uLe = (*iter).uLen;

					_ffmpeg_::_ff_string_big sw = FlvConverter.ConvertH264To((*iter).pBuf, (*iter).uLen,
						pPacket->m_nTimeStamp, (*iter).isKeyFrame);
					fwrite(sw.c_str(), 1, sw.size(), s_fp);
					fflush(s_fp);
				}*/
				//test

				/*发送*/
				IRtmpSession::SendPacket(&_Rtmp, pPacket, TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/

				//等待
				int iWait = _VideoInfo.iWaitTime - (Timestamp_type() - _Time) / 1000;
				if(iWait > 0)
					Sleep(iWait);
				_Time.update();
			}
		}

		return 1;
	}

	int FlvSession::ParseH264Data(const char* c_szInData, UInt32 uInSize, PacketList& Packets)
	{
		const char szHead[] = { 0x00, 0x00, 0x01 };
		tagVideoPlayload_INFO PlayloadInfo;
		char* pData = NULL;
		UInt32 uLen = 0;
		tagBuffer tagBuf;
		if (_VideoInfo.uRemain > 0)
		{
			std::memmove(&_VideoInfo.SndData[0], &_VideoInfo.SndData[_VideoInfo.SndData.size() - _VideoInfo.uRemain], _VideoInfo.uRemain);
			_VideoInfo.SndData.update_size(_VideoInfo.uRemain);
			_VideoInfo.uRemain = 0;
		}

		_VideoInfo.SndData.append(c_szInData, uInSize);
		pData = (char*)_VideoInfo.SndData.c_str();
		uLen = _VideoInfo.SndData.size();

		int iRet = 0, iOffset = 0;

	gt_start:
		int iStartPos = _rtmp_string::FindData(pData, uLen, szHead, 0, sizeof(szHead));
		if (iStartPos >= 0)
		{
			iOffset = iStartPos + sizeof(szHead);
			int iEndPos = _rtmp_string::FindData(pData, uLen, szHead, iOffset, sizeof(szHead));

			if (iEndPos < 0)
			{
				if (_VideoInfo.SndData.c_str() != pData)
				{
					_VideoInfo.uRemain = uLen;
				}
				goto gt_end;
			}

			if (iEndPos > 0)
			{
				if (pData[iEndPos - 1] == 0)
					iEndPos--;
			}

			_pVideo->Parse(&pData[iOffset], iEndPos - iOffset, &PlayloadInfo, false);
			if (PlayloadInfo.isFull)
			{
				if (PlayloadInfo.uHeadType != NAL_UNIT_TYPE_SPS
					&& PlayloadInfo.uHeadType != NAL_UNIT_TYPE_PPS
					&& PlayloadInfo.uHeadType != NAL_UNIT_TYPE_SEI)
				{
					if (_pVideo->is_I_Frame(PlayloadInfo.uHeadType, PlayloadInfo.pData))
					{
						_rtmp_::FlvSession::tagVideoInfo	VideoInfo;
						VideoInfo.SPS = _pVideo->GetSPS();
						VideoInfo.PPS = _pVideo->GetPPS();
						VideoInfo.uWidth = _pVideo->GetStreamInfo()->uWidth;
						VideoInfo.uHeight = _pVideo->GetStreamInfo()->uHeight;
						VideoInfo.uVideoType = _pVideo->GetStreamInfo()->uVideoType;
						VideoInfo.uMax_Framerate = _pVideo->GetStreamInfo()->uMax_Framerate;
						SetVideoInfo(VideoInfo);
					}

					tagBuf.pBuf = &pData[iOffset];
					tagBuf.uLen = iEndPos - iOffset;
					tagBuf.isKeyFrame = _pVideo->is_I_Frame(PlayloadInfo.uHeadType, PlayloadInfo.pData);
					Packets.push_back(tagBuf);
				}

				if (uLen - iEndPos > 4)
				{
					pData += iEndPos;
					uLen -= iEndPos;
					goto gt_start;
				}
				else
				{
					_VideoInfo.uRemain = uLen - iEndPos;
				}
			}
		}
gt_end:
		return Packets.size() > 0 ? 1 : -1;
	}

	void FlvSession::HandleRecv(const self_sptr& sptr, NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
	{
		//test
		//LOG_Print_Info(libFrameWork_RtmpCommon, "Http::HandleRecv(%d) Data:%s", Buf_ptr->payload, Buf_ptr->data);
		//test

		_sData.append(Buf_ptr->data, Buf_ptr->payload);

gt_start:
		switch (_Status)
		{
		case RTMP_STATUS_RTMPT_OPEN:
		{
			HTTP_readV20(&_Rtmp, 1, _sData.c_str(), _sData.size());
			_Rtmp.m_msgCounter = 0;

			//握手
			SendHandleHandShake(&_Rtmp, RTMP_STATUS_HandShake_C0C1);
		}
		break;
		case RTMP_STATUS_HandShake_C0C1:
		{

		}
		break;
		case RTMP_STATUS_HandShake_S0S1S2:
		{
			if (_sData.size() >= 0xC01)
			{//发送响应
				uint32_t uptime, suptime;
				int bMatch;
				char type;
				char serversig[RTMP_SIG_SIZE];

				if (ReadNV20(&_Rtmp, &_sData[0], 1, &type, 1) != 1)	/* 0x03 or 0x06 */
				{
					if (_hEvent)
						_hEvent(RTMP_STATUS_ERROR_HandShake, shared_from_this(), NULL);
					return;
				}

				LOG_Print_Info(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() %s: Type Answer   : %02X", __FUNCTION__, type);

				if (type != 0x03)
					LOG_Print_Info(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() %s: Type mismatch: client sent %d, server answered %d",
						__FUNCTION__, 0x03, type);

				if (ReadNV20(&_Rtmp, &_sData[1], RTMP_SIG_SIZE, serversig, RTMP_SIG_SIZE) != RTMP_SIG_SIZE)
				{
					if (_hEvent)
						_hEvent(RTMP_STATUS_ERROR_HandShake, shared_from_this(), NULL);
					return;
				}

				/* decode server response */

				memcpy(&suptime, serversig, 4);
				suptime = ntohl(suptime);

				LOG_Print_Info(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() %s: Server Uptime : %d", __FUNCTION__, suptime);
				LOG_Print_Info(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() %s: FMS Version   : %d.%d.%d.%d", __FUNCTION__,
					serversig[4], serversig[5], serversig[6], serversig[7]);

				/*bMatch = (memcmp(&_sData[1 + RTMP_SIG_SIZE], clientsig, RTMP_SIG_SIZE) == 0);
				if (!bMatch)
				{
					LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() %s, client signature does not match!", __FUNCTION__);
					Close();
					return ;
				}*/

				SendHandleHandShake(&_Rtmp, RTMP_STATUS_HandShake_C2, serversig, RTMP_SIG_SIZE);

				_sData.clear();
				//连接流
				_Status = RTMP_STATUS_CONNECT_STREAM;
				if (!SendConnectPacket(&_Rtmp, NULL))
				{
					LOG_Print_Error(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() %s, RTMP connect failed.", __FUNCTION__);
					Close();

					if (_hEvent)
						_hEvent(RTMP_STATUS_ERROR_Stream, shared_from_this(), NULL);
				}
			}
		}
		break;
		case RTMP_STATUS_HandShake_C2:
		{
			
		}
		break;
		case RTMP_STATUS_CONNECT_STREAM:
		{
			UInt32 OutSize = 0;
			if (IsOpen() &&
				ReadPacketV20(&_Rtmp, &_RTMPPacket, _sData.c_str(), _sData.size(), OutSize))
			{
				if (RTMPPacket_IsReady(&_RTMPPacket))
				{
					if (_sData.size() > OutSize)
					{
						std::memmove(&_sData[0], &_sData[OutSize], _sData.size() - OutSize);
						_sData.update_size(_sData.size() - OutSize);
					}
					else
					{
						_sData.clear();
					}

					if (!_RTMPPacket.m_nBodySize)
					{
						goto gt_faild;
						return;
					}

					if ((_RTMPPacket.m_packetType == RTMP_PACKET_TYPE_AUDIO) ||
						(_RTMPPacket.m_packetType == RTMP_PACKET_TYPE_VIDEO) ||
						(_RTMPPacket.m_packetType == RTMP_PACKET_TYPE_INFO))
					{
						LOG_Print_Info(libFrameWork_RtmpCommon, "FlvSession::HandleRecv() Received FLV packet before play()! Ignoring.");
						RTMPPacket_Free(&_RTMPPacket);
						goto gt_faild;
						return;
					}

					ClientPacket(&_Rtmp, &_RTMPPacket);
					RTMPPacket_Free(&_RTMPPacket);

					if (_sData.size() > 0)
						goto gt_start;
				}
			}

			if (_Rtmp.m_bPlaying)
			{//开始传输流
				_Status = RTMP_STATUS_Playing;

				if (_hEvent)
					_hEvent(RTMP_STATUS_Playing, shared_from_this(), NULL);
			}
		}
		break;
		}
		return;

	gt_faild:
		_sData.clear();
	}

	void FlvSession::HandleDestroy(const self_sptr& sptr, NETHANDLE hNet)
	{
		if (_isOpen)
		{
			Close(true);
		}
	}

	int FlvSession::SendSpsPps(void)
	{
		RTMPPacket_sptr Packet_sptr = AllocPacket();
		RTMPPacket* pPacket = Packet_sptr.get();

		UInt16 uOffset = 0;
		/*分配包内存和初始化,len为包体长度*/
		memset(pPacket, 0, RTMP_HEAD_SIZE);

		/*包体内存*/
		pPacket->m_body = (char *)pPacket + RTMP_HEAD_SIZE;
		pPacket->m_body[uOffset++] = 0x17;
		pPacket->m_body[uOffset++] = 0x00;

		pPacket->m_body[uOffset++] = 0x00;
		pPacket->m_body[uOffset++] = 0x00;
		pPacket->m_body[uOffset++] = 0x00;

		//AVCDecoderConfigurationRecord
		pPacket->m_body[uOffset++] = 0x01;
		pPacket->m_body[uOffset++] = _VideoInfo.SPS[1];
		pPacket->m_body[uOffset++] = _VideoInfo.SPS[2];
		pPacket->m_body[uOffset++] = _VideoInfo.SPS[3];
		pPacket->m_body[uOffset++] = 0xFF;

		/*sps*/
		pPacket->m_body[uOffset++] = 0xE1;
		pPacket->m_body[uOffset++] = (_VideoInfo.SPS.size() >> 8) & 0xFF;
		pPacket->m_body[uOffset++] = _VideoInfo.SPS.size() & 0xFF;
		memcpy(&pPacket->m_body[uOffset], _VideoInfo.SPS.c_str(), _VideoInfo.SPS.size());
		uOffset += _VideoInfo.SPS.size();

		/*pps*/
		pPacket->m_body[uOffset++] = 0x01;
		pPacket->m_body[uOffset++] = (_VideoInfo.PPS.size() >> 8) & 0xFF;
		pPacket->m_body[uOffset++] = (_VideoInfo.PPS.size()) & 0xFF;
		memcpy(&pPacket->m_body[uOffset], _VideoInfo.PPS.c_str(), _VideoInfo.PPS.size());
		uOffset += _VideoInfo.PPS.size();

		pPacket->m_packetType = RTMP_PACKET_TYPE_VIDEO;
		pPacket->m_nBodySize = uOffset;
		pPacket->m_nChannel = 0x04;
		pPacket->m_nTimeStamp = 0;
		pPacket->m_hasAbsTimestamp = 0;
		pPacket->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		pPacket->m_nInfoField2 = _Rtmp.m_stream_id;

		return IRtmpSession::SendPacket(&_Rtmp, pPacket, TRUE);
	}
}

