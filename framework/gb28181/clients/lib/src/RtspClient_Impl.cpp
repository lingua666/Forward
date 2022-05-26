
#include <libFrameWork_RtspClients/RtspClient_Impl.h>

#define DUMYSINK_BUFFER_SIZE 2048//5120

const UInt8 H264Header[] = {0x00, 0x00, 0x00, 0x01};

#define RTP_TCP_MAX_BUFFER			1500	
#define RTP_RECV_DATA_TIMEOUT_DEF	10000	//单位毫秒
#define RTP_RECV_DATA_CHECK_TIME	5000	//单位毫秒


RtspClient_Impl::RtspClient_Impl( void )
	: _hNet( -1 )
	, _uSeq( 0 )
	, _SeqCycle( 1 )
	, _uRecvSeq( 0 )
	, _uRecvSeqCycle( 1 )
	, _isIFrameEnable( false )
	, _isWait_IFrame ( true )
	, _uStatus( EN_STATUS_NULL )
{
	_CtrlInfo.uRtpPort = -1;
	_CtrlInfo.uRtcpPort = -1;
	_CtrlInfo.hRtp = -1;
	_CtrlInfo.hRtcp = -1;
	_ErrorCode = RTSP_ERRCODE_NULL;
}

RtspClient_Impl::~RtspClient_Impl( void )
{
	Release();
	if( _CtrlInfo.hExcepNT )
	{
		_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), _ErrorCode);
	}
}

int RtspClient_Impl::Init( void )
{
	_MemBuf_sptr = _MEMBUF_sptr(GetPoolInstance()->AllocObj<_MEMBUF>(),
		function20_bind_Ex(&MemPool_type::FreeObj<_MEMBUF>, GetPoolInstance()));
	InitBuf();
	return 1;
}

void RtspClient_Impl::InitBuf( void )
{
	_MemBuf_sptr->uLen = 4;
	memcpy(_MemBuf_sptr->Buf, H264Header, sizeof(H264Header));
}

void RtspClient_Impl::Release( void )
{
	Close();
}

int RtspClient_Impl::Open( const _string_type& sSessionID, UInt8 uStreamType,
	const _string_type& sRtspPath, UInt16 uRtpPort,
	HFNExceptNT hExcepNT, HFNSdpNT hSdpNT,
	HFNSuccessNT hSuccessNT, HFNRtpDataNT hRtpDataNT )
{
	_CtrlInfo.RtspPath = sRtspPath;
	_CtrlInfo.uStreamType = uStreamType;
	_CtrlInfo.SessionID = sSessionID;
	_CtrlInfo.uRtpPort = uRtpPort;
	_CtrlInfo.uRtcpPort = uRtpPort + 1;
	_CtrlInfo.hExcepNT = hExcepNT;
	_CtrlInfo.hSuccessNT = hSuccessNT;
	_CtrlInfo.hRtpDataNT = hRtpDataNT;
	_CtrlInfo.hSdpNT = hSdpNT;
	return 1;
}

int RtspClient_Impl::Play( void )
{
	int iStart = _CtrlInfo.RtspPath.find("rtsp://");
	if( iStart == -1 )
		return -1;

	_string_type sUserPwd;
	_string_type sUser;
	_string_type sPassword;
	iStart = iStart + strlen("rtsp://");
	int iEnd = _CtrlInfo.RtspPath.find('@', iStart);
	if( iEnd != -1 )
	{//标准格式的用户名密码
		sUserPwd = _CtrlInfo.RtspPath.substr(iStart, iEnd - iStart);
		iStart = iEnd;
		iStart += 1;
	}

	iEnd = _CtrlInfo.RtspPath.find('/', iStart);
	_string_type sHost = _CtrlInfo.RtspPath.substr(iStart, iEnd - iStart);
	_string_type sPath = _CtrlInfo.RtspPath.substr(iEnd + 1);

	if( sUserPwd != "" )
	{
		iStart = sUserPwd.find(":");
		sUser = sUserPwd.substr(0, iStart);
		sPassword = sUserPwd.substr(iStart + 1);
	}

	_string_type sIP;
	_string_type sPort;
	iStart = sHost.find(":");
	if(iStart == -1)
	{
		sIP = sHost;
		sPort = "554";
	}
	else
	{
		sIP = sHost.substr(0, iStart);
		sPort = sHost.substr(iStart + 1);
	}
	_SvrInfo.sIP = sIP;

	_Rtsp.SetUserInfo(_CtrlInfo.uStreamType, sUser, sPassword, sIP, sPort);
	return GetAsynStreamClientsInstance()->Connect(sIP.c_str(), 
		_string_type::StringToInt(sPort),
		function20_bind(&RtspClient_Impl::HandleRtspConnect, shared_from_this(), sPath, _function_::_1));
}

void RtspClient_Impl::SetResultEvent( const HFNExceptNT& hExcepNT, const HFNSuccessNT& hSuccessNT )
{
	_CtrlInfo.hExcepNT = hExcepNT;
	_CtrlInfo.hSuccessNT = hSuccessNT;
}

void RtspClient_Impl::SetDataEvent( const HFNSdpNT& hSdpNT, const HFNRtpDataNT& hRtpDataNT )
{
	_CtrlInfo.hRtpDataNT = hRtpDataNT;
	_CtrlInfo.hSdpNT = hSdpNT;
}

void RtspClient_Impl::Close( void )
{
	NETHANDLE h = -1;
	_Lock.Lock();
	_uStatus = EN_STATUS_CLOSE;
	if( _hNet != -1 )
	{
		h = _hNet;
		_hNet = -1;
	}
	_Lock.UnLock();

	if( h != -1 )
	{
		_string_type s = _Rtsp.Teardown(_CtrlInfo.RtspPath.c_str(),
			_CtrlInfo.SessionID.c_str(),
			_string_type::NumberToStr(GetSeq()).c_str());

		GetAsynStreamClientsInstance()->Send(h, s.c_str(), s.size());
		Sleep(50);

		GetAsynStreamClientsInstance()->Close(h);
		LOG_Print_Info(libFrameWork_RtspClients, "RtspClient_Impl::Close()\r\n")

		if( _CtrlInfo.hRtp != -1 )
		{//关闭rtp
			h = _CtrlInfo.hRtp;
			_CtrlInfo.hRtp = -1;
			GetUDPStreamInstance()->Close(h);
		}

		if( _CtrlInfo.hRtcp != -1 )
		{//关闭rtcp
			h = _CtrlInfo.hRtcp;
			_CtrlInfo.hRtcp = -1;
			GetUDPStreamInstance()->Close(h);
		}
	}
}

RtspClient_Impl::tagRtspCtrlInfo& RtspClient_Impl::GetRtspCtrlInfo( void )
{
	return _CtrlInfo;
}

void RtspClient_Impl::HandleRtspConnect( const _string_type& sPath, 
	NETHANDLE Handle )
{
	if( Handle == -1 )
	{
		_ErrorCode = RTSP_ERRCODE_CONNECT_FAILED;
		if( _CtrlInfo.hExcepNT )
			_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), -1);
		return ;
	}

	_Lock.Lock();
	if( _uStatus == EN_STATUS_CLOSE )
	{
		_Lock.UnLock();
		GetAsynStreamClientsInstance()->Close(Handle);
		_ErrorCode = RTSP_ERRCODE_CONNECT_CLOSE;
		if( _CtrlInfo.hExcepNT )
			_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), -1);
		return ;
	}

	_hNet = Handle;
	_Lock.UnLock();

	if( _CtrlInfo.uStreamType == RtspPacket::ENUM_UDP_TYPE )
	{
		GetAsynStreamClientsInstance()->SetRecvHandle(_hNet, 
			function20_bind(&RtspClient_Impl::HandleRtspRecv_Udp, shared_from_this(), 
			sPath, _function_::_1, _function_::_2, _function_::_3));
	}
	else
	{
		GetAsynStreamClientsInstance()->SetRecvHandle(_hNet, 
			function20_bind(&RtspClient_Impl::HandleRtspRecv_Tcp, shared_from_this(), 
			sPath, _function_::_1, _function_::_2, _function_::_3));
	}

	if( GetAsynStreamClientsInstance()->SetDestroyHandle(_hNet,
		function20_bind(&RtspClient_Impl::HandleRtspDestroy, shared_from_this(), 
		_function_::_1) ) == -1 )
	{//失败
		LOG_Print_Error(libFrameWork_RtspClients, "connect %s failed!!\r\n", sPath.c_str())
		_ErrorCode = RTSP_ERRCODE_CONNECT_FAILED;
		Close();
		if( _CtrlInfo.hExcepNT )
			_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), -1);
		return ;
	}

	_string_type s = _Rtsp.Describe(sPath.c_str(), _string_type::NumberToStr(GetSeq()).c_str());

	_uStatus = EN_STATUS_DESCRIBE_ACK;
	GetAsynStreamClientsInstance()->Send(_hNet, s.c_str(), s.size());

}

void RtspClient_Impl::HandleRtspRecv_Udp( const _string_type& sPath,
	NETHANDLE Handle,
	const StreamBuf_ptr& Stream_sptr,
	long lRemain )
{
	LOG_Print_Info(libFrameWork_RtspClients, "HandleRtspRecv_Udp(%d):%s\r\n", Stream_sptr->payload, Stream_sptr->data);
	switch(_uStatus)
	{
	case EN_STATUS_DESCRIBE_ACK:
		{
			if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
			{
				if( strstr(Stream_sptr->data, "application/sdp") == NULL )//不是SDP响应
					return ;

				tagSdpInfo	Info;
				Info.InfoCount = 0;
				if( ParseSdp(Stream_sptr->data, Stream_sptr->payload, &Info) == 1 && _CtrlInfo.hSdpNT )
				{
					_CtrlInfo.hSdpNT(_CtrlInfo.SessionID.c_str(), &Info);
				}

				//打开RTP/RTCP端口
				_CtrlInfo.hRtp = GetUDPStreamInstance()->Open(_CtrlInfo.uRtpPort);
				if( _CtrlInfo.hRtp == -1 )
				{
					LOG_Print_Error(libFrameWork_RtspClients,
						"Open Rtp Port:%d Failed!!\r\n", _CtrlInfo.uRtpPort);

					_ErrorCode = RTSP_ERRCODE_OPEN_RTP_FAILED;
					Close();
					return ;
				}

				if(Info.Info[0].MimeType == "H264")
				{
					_Rtp.Init(0,0,0,IVideoInfo::ENUM_ENC_H264);
				}
				else if(Info.Info[0].MimeType == "H265")
				{
					_Rtp.Init(0,0,0,IVideoInfo::ENUM_ENC_H265);
				}
				else
				{
					LOG_Print_Error(libFrameWork_RtspClients, "MimeType:%s unkonw!\r\n", Info.Info[0].MimeType);
				}

				GetUDPStreamInstance()->SetRecvHandle(_CtrlInfo.hRtp, 
					function20_bind_Ex(&RtspClient_Impl::HandleRtpRecv, shared_from_this()));

				if( GetUDPStreamInstance()->SetDestroyHandle(_CtrlInfo.hRtp, 
					function20_bind(&RtspClient_Impl::HandleRtpDestroy, shared_from_this(), 
					_CtrlInfo.hRtp, _CtrlInfo.uRtpPort)) == -1 )
				{
					_ErrorCode = RTSP_ERRCODE_OPEN_RTP_FAILED;
					Close();
					return ;
				}

				_CtrlInfo.hRtcp = GetUDPStreamInstance()->Open(_CtrlInfo.uRtpPort + 1);
				if( _CtrlInfo.hRtcp == -1 )
				{
					LOG_Print_Error(libFrameWork_RtspClients, 
						"Open Rtcp Port:%d Failed!!\r\n", _CtrlInfo.uRtpPort + 1);

					_ErrorCode = RTSP_ERRCODE_OPEN_RTCP_FAILED;
					Close();
					return ;
				}

				GetUDPStreamInstance()->SetRecvHandle(_CtrlInfo.hRtcp, 
					function20_bind_Ex(&RtspClient_Impl::HandleRtcpRecv, shared_from_this()));

				if( GetUDPStreamInstance()->SetDestroyHandle(_CtrlInfo.hRtcp, 
					function20_bind(&RtspClient_Impl::HandleRtcpDestroy, shared_from_this(), 
					_CtrlInfo.hRtcp, _CtrlInfo.uRtcpPort)) == -1 )
				{
					_ErrorCode = RTSP_ERRCODE_OPEN_RTCP_FAILED;
					Close();
					return ;
				}

				AddSeq();
				_string_type s = _Rtsp.Setup(Info.Info[0].CtrlPath.c_str(),
					_string_type::NumberToStr(_CtrlInfo.uRtpPort).c_str(),
					_string_type::NumberToStr(_CtrlInfo.uRtpPort + 1).c_str(),
					_string_type::NumberToStr(GetSeq()).c_str());
				_uStatus = EN_STATUS_SETUP_ACK;
				GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());
			}
			else
			{
				_ErrorCode = RTSP_ERRCODE_DESCRIBE_ACK_FAILED;
				Close();
			}
		}
		break;
	case EN_STATUS_SETUP_ACK:
		{
			if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
			{
				char* pStart = strstr(Stream_sptr->data, "Session:");
				pStart += strlen("Session:");
				char* pEnd1 = strstr(pStart, ";");
				char* pEnd2 = strstr(pStart, "\r\n");
				while( pStart[0] == ' ' )
				{//过滤空格
					pStart ++;
				}

				_string_type sPlaySession;//播放回话信息
				if( pEnd1 < pEnd2 )
					sPlaySession = _string_type(pStart, pEnd1 - pStart);
				else
					sPlaySession = _string_type(pStart, pEnd2 - pStart);

				pStart = strstr(Stream_sptr->data, "server_port=");
				if( pStart != NULL )
				{
					pStart += strlen("server_port=");
					pEnd1 = strchr(pStart, '-');
					if( pEnd1 != NULL )
					{
						_SvrInfo.uRtpPort = _string_type::StringToInt(_string_type(pStart, pEnd1 - pStart));
						pStart = pEnd1 + 1;
						pEnd1 = strchr(pStart, ';');
						if( pEnd1 != NULL )
						{
							_SvrInfo.uRtcpPort = _string_type::StringToInt(_string_type(pStart, pEnd1 - pStart));
						}
					}
				}

				AddSeq();
				_string_type s = _Rtsp.Play(sPlaySession.c_str(),
					_string_type::NumberToStr(GetSeq()).c_str());
				_uStatus = EN_STATUS_PLAY_ACK;
				GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());
			}
			else
			{
				_ErrorCode = RTSP_ERRCODE_SETUP_ACK_FAILED;
				Close();
			}
		}
		break;
	case EN_STATUS_PLAY_ACK:
		{
			if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
			{
				if( _CtrlInfo.hSuccessNT )
					_CtrlInfo.hSuccessNT(_CtrlInfo.SessionID.c_str());
				_RtpIntervalTimer = Timestamp_type();
				GetTimerEventInstance()->push_back(function20_bind(&RtspClient_Impl::HandleRtpCheck,
					shared_from_this()), RTP_RECV_DATA_CHECK_TIME);
			}
			else
			{
				_ErrorCode = RTSP_ERRCODE_PLAY_ACK_FAILED;
				Close();
			}
		}
		break;
	case EN_STATUS_TEARDOWN_ACK:
		{

		}
		break;
	case EN_STATUS_OPTIONS_ACK:
		{

		}
		break;
	}
}

void RtspClient_Impl::HandleRtspRecv_Tcp( const _string_type& sPath,
	NETHANDLE Handle,
	const StreamBuf_ptr& Stream_sptr,
	long lRemain )
{
	//LOG_Print_Info(libFrameWork_RtspClients, "HandleRtspRecv_Tcp(%d):%s\r\n", Stream_sptr->payload, Stream_sptr->data);

	if( _uStatus == EN_STATUS_RTP_DATA )
	{
		int iOffset = 0;
		tagTCP_RTP_INFO* pInfo = (tagTCP_RTP_INFO*)_sTcpData.c_str();

gt_start:
		if( pInfo->uRemain == 0 && pInfo->InterLeaved.uLength > 0 )
		{
			if( pInfo->InterLeaved.uChannel == 0x00 )
			{//RTP包
				RtpRecv_Impl(pInfo->Data, pInfo->InterLeaved.uLength);
			}
			else if( pInfo->InterLeaved.uChannel == 0x01 )
			{//RTCP包
				if( _Rtp.GetHeadInfo()->Ver != 0 )
				{
					UInt32 uSSRC = RtcpPacket::GetSenderSSRC(pInfo->Data, pInfo->InterLeaved.uLength);
					RtcpPacket Rtcp;
					Rtcp.Init( uSSRC );
					PKBuf_ptr ptr = Rtcp.GetRRReport_Tcp(_Rtp.GetHeadInfo()->SSRC, 0, _uRecvSeqCycle, _uRecvSeq, 0);
					GetAsynStreamClientsInstance()->Send(_hNet, ptr->_Data, ptr->_uPayload);
				}
			}
			else
			{//未知

			}

			pInfo->uRemain = 0;
			pInfo->InterLeaved.uLength = 0;
			if( iOffset < Stream_sptr->payload )
				goto gt_start;
			return ;
		}

		if( iOffset >= Stream_sptr->payload )
		{
			return ;
		}

		if( pInfo->uRemain == 0 )
		{//解析InterLeaved 头部
			for( ; iOffset < Stream_sptr->payload; iOffset ++ )
			{
				if( Stream_sptr->data[iOffset] == 0x24 )
				{//解析InterLeaved头部 起始码
					break;
				}
			}

			if( iOffset >= Stream_sptr->payload )
			{//没有找到起始码
				return ;
			}

			//找到起始码
			if( Stream_sptr->payload - iOffset < sizeof(tagINTERLEAVED) )
			{//头部不完整

			}
			else
			{//头部完整
				memcpy(pInfo, &Stream_sptr->data[iOffset], sizeof(tagINTERLEAVED));
			#if defined( LITTLE_ENDIAN_DEF )
				pInfo->InterLeaved.uLength = BigEndian16_UInt16(pInfo->InterLeaved.uLength);
			#endif
				pInfo->uRemain = pInfo->InterLeaved.uLength;
				iOffset += sizeof(tagINTERLEAVED);
				goto gt_start;
			}
		}
		else
		{
			int iSize = 0;
			if( iOffset + pInfo->uRemain > Stream_sptr->payload )
			{
				iSize = Stream_sptr->payload - iOffset;
				memcpy(&pInfo->Data[pInfo->InterLeaved.uLength - pInfo->uRemain], &Stream_sptr->data[iOffset], iSize);
			}
			else
			{
				iSize = pInfo->uRemain;
				memcpy(&pInfo->Data[pInfo->InterLeaved.uLength - pInfo->uRemain], &Stream_sptr->data[iOffset], iSize);
			}
			pInfo->uRemain -= iSize;
			iOffset += iSize;
			if( iOffset < Stream_sptr->payload )
				goto gt_start;
		}
	}
	else
	{
		switch(_uStatus)
		{
		case EN_STATUS_DESCRIBE_ACK:
			{
				if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
				{
					if( strstr(Stream_sptr->data, "application/sdp") == NULL )//不是SDP响应
						return ;

					tagSdpInfo	Info;
					Info.InfoCount = 0;
					if( ParseSdp(Stream_sptr->data, Stream_sptr->payload, &Info) == 1 && _CtrlInfo.hSdpNT )
					{
						_CtrlInfo.hSdpNT(_CtrlInfo.SessionID.c_str(), &Info);
					}

					if(Info.Info[0].MimeType == "H264")
					{
						_Rtp.Init(0,0,0,IVideoInfo::ENUM_ENC_H264);
					}
					else if(Info.Info[0].MimeType == "H265")
					{
						_Rtp.Init(0,0,0,IVideoInfo::ENUM_ENC_H265);
					}
					else
					{
						LOG_Print_Error(libFrameWork_RtspClients, "MimeType:%s unkonw!\r\n", Info.Info[0].MimeType);
					}

					AddSeq();
					_string_type s = _Rtsp.Setup(Info.Info[0].CtrlPath.c_str(),
						_string_type::NumberToStr(_CtrlInfo.uRtpPort).c_str(),
						_string_type::NumberToStr(_CtrlInfo.uRtpPort + 1).c_str(),
						_string_type::NumberToStr(GetSeq()).c_str());
					_uStatus = EN_STATUS_SETUP_ACK;
					GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());
				}
				else
				{
					_ErrorCode = RTSP_ERRCODE_DESCRIBE_ACK_FAILED;
					Close();
				}
			}
			break;
		case EN_STATUS_SETUP_ACK:
			{
				if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
				{
					char* pStart = strstr(Stream_sptr->data, "Session:");
					pStart += strlen("Session:");
					char* pEnd1 = strstr(pStart, ";");
					char* pEnd2 = strstr(pStart, "\r\n");
					while( pStart[0] == ' ' )
					{//过滤空格
						pStart ++;
					}

					_string_type sPlaySession;//播放回话信息
					if( pEnd1 < pEnd2 )
						sPlaySession = _string_type(pStart, pEnd1 - pStart);
					else
						sPlaySession = _string_type(pStart, pEnd2 - pStart);

					pStart = strstr(Stream_sptr->data, "server_port=");
					if( pStart != NULL )
					{
						pStart += strlen("server_port=");
						pEnd1 = strchr(pStart, '-');
						if( pEnd1 != NULL )
						{
							_SvrInfo.uRtpPort = _string_type::StringToInt(_string_type(pStart, pEnd1 - pStart));
							pStart = pEnd1 + 1;
							pEnd1 = strchr(pStart, ';');
							if( pEnd1 != NULL )
							{
								_SvrInfo.uRtcpPort = _string_type::StringToInt(_string_type(pStart, pEnd1 - pStart));
							}
						}
					}

					AddSeq();
					_string_type s = _Rtsp.Play(sPlaySession.c_str(),
						_string_type::NumberToStr(GetSeq()).c_str());
					_uStatus = EN_STATUS_PLAY_ACK;
					GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());
					//_sTcpData.reserve(RTP_TCP_MAX_BUFFER + sizeof(tagTCP_RTP_INFO));
					_sTcpData.reserve(0xFFFF);
					tagTCP_RTP_INFO *pInfo = (tagTCP_RTP_INFO*)_sTcpData.c_str();
					memset(pInfo, 0, sizeof(tagTCP_RTP_INFO));
				}
				else
				{
					_ErrorCode = RTSP_ERRCODE_SETUP_ACK_FAILED;
					Close();
				}
			}
			break;
		case EN_STATUS_PLAY_ACK:
			{
				if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
				{
					if( _CtrlInfo.hSuccessNT )
						_CtrlInfo.hSuccessNT(_CtrlInfo.SessionID.c_str());
					_RtpIntervalTimer = Timestamp_type();

					_uStatus = EN_STATUS_RTP_DATA;
				}
				else
				{
					_ErrorCode = RTSP_ERRCODE_PLAY_ACK_FAILED;
					Close();
				}
			}
			break;
		case EN_STATUS_TEARDOWN_ACK:
			{

			}
			break;
		case EN_STATUS_OPTIONS_ACK:
			{

			}
			break;
		}
	}
}

void RtspClient_Impl::HandleRtpCheck( void )
{
	if(Timestamp_type().epochMicroseconds() - _RtpIntervalTimer.epochMicroseconds() >
		RTP_RECV_DATA_TIMEOUT_DEF * 1000)
	{
		_ErrorCode = RTSP_ERRCODE_RTP_TIMEOUT;
		Close();
	}
	else
	{
		GetTimerEventInstance()->push_back(function20_bind(&RtspClient_Impl::HandleRtpCheck,
			shared_from_this()), RTP_RECV_DATA_CHECK_TIME);
	}
}

void RtspClient_Impl::HandleRtspDestroy( NETHANDLE handle )
{
	LOG_Print_Info(libFrameWork_RtspClients, "RtspClient_Impl::HandleRtspDestroy(%p)\r\n",this)

	HFNExceptNT hNt = _CtrlInfo.hExcepNT;
	_string_type sSessionID = _CtrlInfo.SessionID;

	if( hNt )
	{
		if( _hNet != -1 )
			_ErrorCode = RTSP_ERRCODE_EXCEPT_CLOSE;
		
		hNt(sSessionID.c_str(), -1);
	}
}

void RtspClient_Impl::HandleRtpRecv( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr )
{
	RtpRecv_Impl(Buf_ptr->data, Buf_ptr->payload);
}

void RtspClient_Impl::RtpRecv_Impl( const char* c_szData, UInt32 uLen )
{
	//LOG_Print_Info(libFrameWork_RtspClients, "HandleRtpRecv(%d)\r\n", Buf_ptr->payload);
	bool isLost = false;
	tagVideoPlayload_INFO PlayloadInfo = {0};
	_RtpIntervalTimer = Timestamp_type();
	//RtpPacket Rtp;
	if( _CtrlInfo.hRtpDataNT )
	{
		int iLen = _Rtp.Parse((char*)c_szData, uLen, &PlayloadInfo);
		if( iLen == 1 )
		{
			if( !_Rtcp.isInit() )
			{
				_Rtcp.Init(_Rtp.GetHeadInfo()->SSRC);
				if( _CtrlInfo.uStreamType == RtspPacket::ENUM_UDP_TYPE )
				{
					GetTimerEventInstance()->push_back( 
						function20_bind(&RtspClient_Impl::SendRtcpRRReport_Udp, shared_from_this(),
						(UInt32)(Timestamp_type().utcTime())), 4000);
				}
				else
				{
					/*GetTimerEventInstance()->push_back( 
						function20_bind(&RtspClient_Impl::SendRtcpRRReport_Tcp, shared_from_this(),
						(UInt32)(Timestamp_type().utcTime())), 4000);*/
				}
			}

			if( _uRecvSeq == 0 )
				_uRecvSeq = _Rtp.GetHeadSeq();
			else
			{
				if( (UInt16)(_uRecvSeq + 1) != _Rtp.GetHeadSeq() )
				{//出现丢包现象，将已经接收的包丢弃
					//test
					LOG_Print_Error(libFrameWork_RtspClients, "RtspClient_Impl::HandleRtpRecv Lost Seq(%d,%d)", _uRecvSeq, _Rtp.GetHeadSeq());
					printf("RtspClient_Impl::HandleRtpRecv Lost Seq(%d,%d)\r\n", _uRecvSeq, _Rtp.GetHeadSeq());
					//test

					isLost = true;
					//InitBuf();
					if( _isIFrameEnable )
					{//丢帧等待I帧
						_isWait_IFrame = true;
						_MemBuf_sptr->uLen = RTP_MAX_BUF + 1;
					}
				}
			}

			_uRecvSeq = _Rtp.GetHeadSeq();
			if( _uRecvSeq == 0 )
			{
				_uRecvSeqCycle ++;
			}

			if( !isLost )
			{
				if( _isWait_IFrame )
				{
					if( _Rtp.is_I_Frame(PlayloadInfo.uHeadType, PlayloadInfo.pData) ) 
					{//是I帧
						_isWait_IFrame = false;
						InitBuf();
					}
					else
					{
						return ;
					}
				}

				if( PlayloadInfo.isFull )
				{
					if( _MemBuf_sptr->uLen + PlayloadInfo.uDataLen < RTP_MAX_BUF )
					{
						memcpy(&_MemBuf_sptr->Buf[_MemBuf_sptr->uLen], PlayloadInfo.pData, PlayloadInfo.uDataLen);
						_MemBuf_sptr->uLen += PlayloadInfo.uDataLen;

						if( _MemBuf_sptr->uLen > DUMYSINK_BUFFER_SIZE )
						{
							_CtrlInfo.hRtpDataNT(_CtrlInfo.SessionID.c_str(),
								_MemBuf_sptr->Buf, _MemBuf_sptr->uLen);
							_MemBuf_sptr->uLen = 0;
						}

						//const UInt8 H264Header[] = {0x00, 0x00, 0x00, 0x01};
						memcpy(&_MemBuf_sptr->Buf[_MemBuf_sptr->uLen], H264Header, sizeof(H264Header));
						_MemBuf_sptr->uLen += 4;
					}
					else
					{
						//test
						LOG_Print_Error(libFrameWork_RtspClients, "RtspClient_Impl::HandleRtpRecv1(%d) Recv Data is more than %d",
							_MemBuf_sptr->uLen, RTP_MAX_BUF);
						//test

						_MemBuf_sptr->uLen = 4;
						memcpy(_MemBuf_sptr->Buf, H264Header, sizeof(H264Header));
						//_MemBuf_sptr->uLen = 0;
					}
				}
				else
				{
					if( _MemBuf_sptr->uLen + PlayloadInfo.uDataLen < RTP_MAX_BUF )
					{
						memcpy(&_MemBuf_sptr->Buf[_MemBuf_sptr->uLen], PlayloadInfo.pData, PlayloadInfo.uDataLen);
						_MemBuf_sptr->uLen += PlayloadInfo.uDataLen;
					}
					else
					{//超过缓冲区长度
						_MemBuf_sptr->uLen = RTP_MAX_BUF + 1;

						//test
						LOG_Print_Error(libFrameWork_RtspClients, "RtspClient_Impl::HandleRtpRecv2(%d) Recv Data is more than %d", 
							_MemBuf_sptr->uLen, RTP_MAX_BUF);
						//test
					}
				}
			}
		}
	}
}

void RtspClient_Impl::SendRtcpRRReport_Udp( UInt32 uSSRC )
{
	if( _CtrlInfo.hRtcp != -1 )
	{
		PKBuf_ptr ptr = _Rtcp.GetRRReport_Udp(uSSRC, 0, _uRecvSeqCycle, _uRecvSeq, 0);

		struct sockaddr_in Dest = {0};
		Dest.sin_port			= htons( _SvrInfo.uRtcpPort );
		Dest.sin_addr.s_addr	= inet_addr( _SvrInfo.sIP.c_str() );
		Dest.sin_family			= AF_INET;

		if( GetUDPStreamInstance()->Send(_CtrlInfo.hRtcp, &Dest, ptr->_Data, ptr->_uPayload) == 1 )
		{
			GetTimerEventInstance()->push_back( 
				function20_bind(&RtspClient_Impl::SendRtcpRRReport_Udp, shared_from_this(),
				uSSRC), 4000);
		}	
	}
}

/*void RtspClient_Impl::SendRtcpRRReport_Tcp( UInt32 uSSRC )
{
	if( _hNet != -1 )
	{
		PKBuf_ptr ptr = _Rtcp.GetRRReport_Tcp(uSSRC, 0, _uRecvSeqCycle, _uRecvSeq, 0);
		if( GetAsynStreamClientsInstance()->Send(_hNet, ptr->_Data, ptr->_uPayload) == 1 )
		{
			GetTimerEventInstance()->push_back( 
				function20_bind(&RtspClient_Impl::SendRtcpRRReport_Tcp, shared_from_this(),
				uSSRC), 4000);
		}
	}
}*/

void RtspClient_Impl::HandleRtpDestroy( NETHANDLE Node, UInt16 uPort )
{
	LOG_Print_Info(libFrameWork_RtspClients, "RtspClient_Impl::HandleRtpDestroy()\r\n")
}

void RtspClient_Impl::HandleRtcpRecv( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr )
{
	printf("HandleRtcpRecv(%d)\r\n", Buf_ptr->payload);
}

void RtspClient_Impl::HandleRtcpDestroy( NETHANDLE Node, UInt16 uPort )
{
	LOG_Print_Info(libFrameWork_RtspClients, "RtspClient_Impl::HandleRtcpDestroy()\r\n")
}

int	RtspClient_Impl::ParseSdp( const char* c_szData, UInt32 uLen, tagSdpInfo* tagInfo )
{
	char* pEnd = NULL;
	char* pStart = strstr((char*)c_szData, "o=");
	if( pStart != NULL )
	{//回话ID
		pStart = strchr(pStart, ' ');
		if( pStart != NULL )
			pStart ++;
		pEnd = strchr(pStart, ' ');
		if( pEnd != NULL )
			tagInfo->SessionID = _string_type(pStart, pEnd - pStart);
		pStart = pEnd + tagInfo->SessionID.size();
	}
	else
	{
		pStart = (char*)c_szData;
	}

	pStart = strstr((char*)c_szData, "m=");
	if( pStart != NULL )
	{//媒体类型
		pStart += 2;
		pEnd = strchr(pStart, ' ');
		if( strncmp(pStart, "video", 5) == 0 )
		{
			tagInfo->Info[tagInfo->InfoCount].uType = EN_STATUS_VIDEO;
			pStart += 6;
		}
		else if( strncmp(pStart, "audio", 5) == 0 )
		{
			tagInfo->Info[tagInfo->InfoCount].uType = EN_STATUS_AUDIO;
			pStart += 6;
		}
		else
		{
			tagInfo->Info[tagInfo->InfoCount].uType = EN_STATUS_UNKNOW;
		}

		pEnd = strchr(pStart, ' ');
		if( pEnd != NULL )
		{//媒体端口号
			tagInfo->Info[tagInfo->InfoCount].Port = _string_type(pStart, pEnd - pStart);
			pStart = pEnd + 1;
		}

		pEnd = strchr(pStart, ' ');
		if( pEnd != NULL )
		{//RTP 传输类型
			if( strncmp(pStart, "RTP/AVP", 7) == 0 || 
				strncmp(pStart, "RTP/AVP/UDP", 11) == 0 )
			{
				tagInfo->Info[tagInfo->InfoCount].RtpType = "udp";
			}
			else if( strncmp(pStart, "RTP/AVP/TCP", 11) == 0 )
			{
				tagInfo->Info[tagInfo->InfoCount].RtpType = "tcp";
			}
			else
			{
				tagInfo->Info[tagInfo->InfoCount].RtpType = "unknow";
			}

			pStart = pEnd + 1;
		}

		pEnd = strstr(pStart, "\r\n");
		if( pEnd != NULL )
		{//RTP 负载类型
			tagInfo->Info[tagInfo->InfoCount].PT = _string_type(pStart, pEnd - pStart);
			pStart = pEnd + 1;
		}
	}

	const char* c_szVideoInfoStart = c_szData;
	pStart = strstr((char*)c_szData, "m=video");
	if( pStart != NULL )
	{
		c_szVideoInfoStart = pStart;
		pStart = strstr(pStart, "a=control:");
		if( pStart != NULL )
		{//媒体控制路径
			pStart += strlen("a=control:");
			pEnd = strstr(pStart, "\r\n");
			if( pEnd != NULL )
			{
				tagInfo->Info[tagInfo->InfoCount].CtrlPath = _string_type(pStart, pEnd - pStart);
				if(tagInfo->Info[tagInfo->InfoCount].CtrlPath.find("rtsp://") == -1)
				{//不是完整路径
					_string_type sCtrlPath = tagInfo->Info[tagInfo->InfoCount].CtrlPath;
					pStart = strstr((char*)c_szData, "Content-Base: ");
					if( pStart != NULL )
					{
						pStart += strlen("Content-Base: ");
						pEnd = strstr(pStart, "\r\n");
						if( pEnd != NULL )
						{
							tagInfo->Info[tagInfo->InfoCount].CtrlPath = _string_type(pStart, pEnd - pStart);
							_Rtsp.SetUrl(tagInfo->Info[tagInfo->InfoCount].CtrlPath);
							tagInfo->Info[tagInfo->InfoCount].CtrlPath += sCtrlPath;
						}
					}
				}
			}
		}
	}

	pStart = strstr((char*)c_szVideoInfoStart, "a=rtpmap:");
	if( pStart != NULL )
	{//媒体格式和样本率
		pEnd = strchr(pStart, ' ');
		if( pEnd != NULL )
		{
			pStart += strlen("a=rtpmap:");
			tagInfo->Info[tagInfo->InfoCount].Format = _string_type(pStart, pEnd - pStart);
			pStart = pEnd;
		}

		pStart = strchr(pStart, ' ');
		if( pStart != NULL )
		{
			pStart += 1;
			pEnd = strchr(pStart, '/');
			if( pEnd != NULL )
			{
				tagInfo->Info[tagInfo->InfoCount].MimeType = _string_type(pStart, pEnd - pStart);
				pStart = pEnd +1;
				pEnd = strstr(pStart, "\r\n");
				if( pEnd != NULL )
				{
					tagInfo->Info[tagInfo->InfoCount].SampleRate = _string_type(pStart, pEnd - pStart);
					pStart += 2;
				}
			}
		}
	}

	pStart = strstr((char*)c_szData, "Media_header:MEDIAINFO=");
	if( pStart != NULL )
	{//媒体头
		pStart += strlen("Media_header:MEDIAINFO=");
		pEnd = strstr(pStart, "\r\n");
		if( pEnd != NULL )
		{
			tagInfo->Info[tagInfo->InfoCount].Header = _string_type(pStart, pEnd - pStart);
			pStart = pEnd + 2;
		}
	}

	pStart = strstr((char*)c_szData, "sprop-parameter-sets=");
	if( pStart != NULL )
	{//sps,pps
		pStart += strlen("sprop-parameter-sets=");
		pEnd = strchr(pStart, ',');
		if( pEnd != NULL )
		{
			tagInfo->Info[tagInfo->InfoCount].Sps_base64 = _string_type(pStart, pEnd - pStart);
			pStart = pEnd + 1;
			pEnd = strstr(pStart, "\r\n");
			if( pEnd != NULL )
			{
				tagInfo->Info[tagInfo->InfoCount].Pps_base64 = _string_type(pStart, pEnd - pStart);
			}
		}
	}

	tagInfo->InfoCount = 1;
	return 1;
}

_string_type  RtspClient_Impl::ParseRet( const char* c_szData, UInt32 uLen )
{
	_string_type s;
	char* pStart = strstr((char*)c_szData, "RTSP/1.0 ");
	if( pStart == NULL )
		return "";

	pStart += strlen("RTSP/1.0 ");
	char* pEnd = strchr(pStart, ' ');
	if( pEnd == NULL )
	{
		s = pStart;
	}
	else
	{
		s.append(pStart, pEnd - pStart);
		s.append(1, ',');
		pStart = pEnd + 1;
		pEnd = strstr(pStart, "\r\n");
		if( pEnd != NULL )
			s.append(pStart, pEnd - pStart);
	}

	return s.replace("\r\n","");
}

void RtspClient_Impl::SetSeq( UInt32 uSeq )
{
	_uSeq = uSeq;
}

void RtspClient_Impl::AddSeq( void )
{
	_uSeq ++;
}

UInt32 RtspClient_Impl::GetSeq( void )
{
	return _uSeq;
}

_string_type RtspClient_Impl::GetMediaVideoHeadBuffer( void )
{
	return _Rtp.GetVideoHeadBuffer();
}

#undef DUMYSINK_BUFFER_SIZE