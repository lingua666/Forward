// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Rtsp/RtspPacket.h>
#include <libFrameWork_Rtsp/RtpPacket.h>
#include <libFrameWork_Rtsp/RtcpPacket.h>

//test
FILE* g_fp11 = fopen("D:\\test123.h264", "wb");
//test

enum RTSP_ERRCODE
{
	RTSP_ERRCODE_CONNECT_FAILED,
	RTSP_ERRCODE_OPEN_RTP_FAILED,
	RTSP_ERRCODE_OPEN_RTCP_FAILED,
	RTSP_ERRCODE_CLOSE
};

#define RTP_MAX_BUF		(102400)

class RtspClient_Impl
{
public:

	typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;
	enum enStatus
	{
		EN_STATUS_NULL,
		EN_STATUS_DESCRIBE_ACK,
		EN_STATUS_SETUP_ACK,
		EN_STATUS_PLAY_ACK,
		EN_STATUS_TEARDOWN_ACK,
		EN_STATUS_OPTIONS_ACK
	};

	enum enMediaType
	{
		EN_STATUS_VIDEO,
		EN_STATUS_AUDIO,
		EN_STATUS_UNKNOW
	};

	struct tagMediaInfo
	{
		UInt8 uType;			//媒体类型 视频/音频
		_string_type RtpType;	//RTP类型 udp/tcp
		_string_type Port;		//媒体端口
		_string_type PT;		//有效载荷类型
		_string_type CtrlPath;	//媒体流控制路径
		_string_type Format;	//媒体格式
		_string_type MimeType;	//媒体类型
		_string_type SampleRate;//样本率
		_string_type Header;	//媒体头
		_string_type Sps_base64;	//base64 加密后的sps信息
		_string_type Pps_base64;	//base64 加密后的pps信息

	};

	struct tagSdpInfo
	{
		_string_type	SessionID;//回话ID
		UInt8			InfoCount;//媒体信息数量
		tagMediaInfo	Info[2];  //媒体信息
	};

	typedef function20_handle<void (const char*, Int32)>				HFNExceptNT;
	typedef function20_handle<void (const char*, tagSdpInfo*)>			HFNSdpNT;
	typedef function20_handle<void (const char*)>						HFNSuccessNT;
	typedef function20_handle<void (const char*, const char*, UInt16)>	HFNRtpDataNT;

	struct tagRtspCtrlInfo
	{
		UInt16 uRtpPort;
		UInt16 uRtcpPort;
		NETHANDLE	hRtp;
		NETHANDLE	hRtcp;
		_string_type	SessionID;
		HFNExceptNT		hExcepNT;
		HFNSdpNT		hSdpNT;
		HFNSuccessNT	hSuccessNT;
		HFNRtpDataNT	hRtpDataNT;
	};

	typedef SmartPTR<_MEMBUF>		_MEMBUF_sptr;
	static MemPool_type* GetPoolInstance( void )
	{
		static MemPool_type s_Pool( sizeof(_MEMBUF) + RTP_MAX_BUF );
		return &s_Pool;
	}

public:
	RtspClient_Impl( void )
		: _hNet( -1 )
		, _uSeq( 0 )
		, _uStatus( EN_STATUS_NULL )
	{
		_CtrlInfo.uRtpPort = -1;
		_CtrlInfo.uRtcpPort = -1;
		_CtrlInfo.hRtp = -1;
		_CtrlInfo.hRtcp = -1;
	}

	~RtspClient_Impl( void )
	{

	}

	int Init( void )
	{
		_MemBuf_sptr = _MEMBUF_sptr(GetPoolInstance()->AllocObj<_MEMBUF>(),
			function20_bind_Ex(&MemPool_type::FreeObj<_MEMBUF>, GetPoolInstance()));
		_MemBuf_sptr->uLen = 4;
		const UInt8 H264Header[] = {0x00, 0x00, 0x00, 0x01};
		memcpy(_MemBuf_sptr->Buf, H264Header, sizeof(H264Header));
		return 1;
	}

	void Release( void )
	{
		Close();
	}

	int Open( const _string_type& sSessionID,
		const _string_type& sRtspPath, UInt16 uRtpPort,
		HFNExceptNT hExcepNT, HFNSdpNT hSdpNT,
		HFNSuccessNT hSuccessNT, HFNRtpDataNT hRtpDataNT )
	{
		int iStart = sRtspPath.find("rtsp://");
		if( iStart == -1 )
			return -1;
		iStart = iStart + strlen("rtsp://");
		int iEnd = sRtspPath.find('@', iStart);
		if( iEnd == -1 )
			return -1;

		_string_type sUserPwd = sRtspPath.substr(iStart, iEnd - iStart);

		iStart = iEnd;
		iEnd = sRtspPath.find('/', iStart);
		_string_type sHost = sRtspPath.substr(iStart + 1, iEnd - (iStart + 1));
		_string_type sPath = sRtspPath.substr(iEnd + 1);

		iStart = sUserPwd.find(":");
		_string_type sUser = sUserPwd.substr(0, iStart);
		_string_type sPassword = sUserPwd.substr(iStart + 1);
		iStart = sHost.find(":");
		_string_type sIP = sHost.substr(0, iStart);
		_string_type sPort = sHost.substr(iStart + 1);

		_CtrlInfo.SessionID = sSessionID;
		_CtrlInfo.uRtpPort = uRtpPort;
		_CtrlInfo.hExcepNT = hExcepNT;
		_CtrlInfo.hSuccessNT = hSuccessNT;
		_CtrlInfo.hRtpDataNT = hRtpDataNT;
		_CtrlInfo.hSdpNT = hSdpNT;

		_Rtsp.SetUserInfo(RtspPacket::ENUM_UDP_TYPE, sUser, sPassword, sIP, sPort);
		GetAsynStreamClientsInstance()->Connect(sIP.c_str(), 
			_string_type::StringToInt(sPort),
			function20_bind(&RtspClient_Impl::HandleRtspConnect, this, sPath, _function_::_1));
		return 1;
	}

	void Close( void )
	{
		NETHANDLE h;
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

		if( _hNet != -1 )
		{//关闭rtsp
			h = _hNet;
			_hNet = -1;
			GetAsynStreamClientsInstance()->Close(h);
		}
		
	}

	tagRtspCtrlInfo& GetRtspCtrlInfo( void )
	{
		return _CtrlInfo;
	}

protected:
	void HandleRtspConnect( const _string_type& sPath, 
		NETHANDLE Handle )
	{
		if( Handle == -1 )
		{
			if( _CtrlInfo.hExcepNT )
				_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), RTSP_ERRCODE_CONNECT_FAILED);
			return ;
		}

		_hNet = Handle;
		GetAsynStreamClientsInstance()->SetRecvHandle(Handle, 
			function20_bind(&RtspClient_Impl::HandleRtspRecv, this, 
			sPath, _function_::_1, _function_::_2, _function_::_3));

		if( GetAsynStreamClientsInstance()->SetDestroyHandle(Handle,
			function20_bind(&RtspClient_Impl::HandleRtspDestroy, this, 
			_function_::_1) ) == -1 )
		{//失败
			return ;
		}

		_string_type s = _Rtsp.Describe(sPath.c_str(), _string_type::NumberToStr(GetSeq()).c_str());
		_uStatus = EN_STATUS_DESCRIBE_ACK;
		GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());

	}

	void HandleRtspRecv( const _string_type& sPath, 
		NETHANDLE Handle,
		const StreamBuf_ptr& Stream_sptr,
		long lRemain )
	{
		printf("HandleRecv(%d):%s\r\n", Stream_sptr->payload, Stream_sptr->data);
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
						printf("Open Rtp Port:%d Failed!!\r\n", _CtrlInfo.uRtpPort);
						if( _CtrlInfo.hExcepNT )
							_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), RTSP_ERRCODE_OPEN_RTP_FAILED);
						return ;
					}

					GetUDPStreamInstance()->SetRecvHandle(_CtrlInfo.hRtp, 
						function20_bind_Ex(&RtspClient_Impl::HandleRtpRecv, this));

					GetUDPStreamInstance()->SetDestroyHandle(_CtrlInfo.hRtp, 
						function20_bind(&RtspClient_Impl::HandleRtpDestroy, this, 
						_CtrlInfo.hRtp, _CtrlInfo.uRtpPort));

					_CtrlInfo.hRtcp = GetUDPStreamInstance()->Open(_CtrlInfo.uRtpPort + 1);
					if( _CtrlInfo.hRtcp == -1 )
					{
						printf("Open Rtcp Port:%d Failed!!\r\n", _CtrlInfo.uRtpPort + 1);
						if( _CtrlInfo.hExcepNT )
							_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), RTSP_ERRCODE_OPEN_RTCP_FAILED);
						return ;
					}

					GetUDPStreamInstance()->SetRecvHandle(_CtrlInfo.hRtcp, 
						function20_bind_Ex(&RtspClient_Impl::HandleRtcpRecv, this));

					GetUDPStreamInstance()->SetDestroyHandle(_CtrlInfo.hRtcp, 
						function20_bind(&RtspClient_Impl::HandleRtcpDestroy, this, 
						_CtrlInfo.hRtcp, _CtrlInfo.uRtcpPort));

					AddSeq();
					_string_type s = _Rtsp.Setup(Info.Info[0].CtrlPath.c_str(),
						_string_type::NumberToStr(_CtrlInfo.uRtpPort).c_str(),
						_string_type::NumberToStr(_CtrlInfo.uRtpPort + 1).c_str(),
						_string_type::NumberToStr(GetSeq()).c_str());
					_uStatus = EN_STATUS_SETUP_ACK;
					GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());
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

					_string_type sPlaySession;
					if( pEnd1 < pEnd2 )
						sPlaySession = _string_type(pStart, pEnd1 - pStart);
					else
						sPlaySession = _string_type(pStart, pEnd2 - pStart);

					AddSeq();
					_string_type s = _Rtsp.Play(sPlaySession.c_str(),
						_string_type::NumberToStr(GetSeq()).c_str());
					_uStatus = EN_STATUS_PLAY_ACK;
					GetAsynStreamClientsInstance()->Send(Handle, s.c_str(), s.size());
				}
			}
			break;
		case EN_STATUS_PLAY_ACK:
			{
				if( ParseRet(Stream_sptr->data, Stream_sptr->payload) == "200,OK" )
				{

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

	void HandleRtspDestroy( NETHANDLE handle )
	{
		if( _hNet != NULL && _CtrlInfo.hExcepNT )
			_CtrlInfo.hExcepNT(_CtrlInfo.SessionID.c_str(), RTSP_ERRCODE_CLOSE);

		_CtrlInfo.hExcepNT.reset();
		_CtrlInfo.hRtpDataNT.reset();
		_CtrlInfo.hSdpNT.reset();
		_CtrlInfo.hSuccessNT.reset();
	}

	void HandleRtpRecv( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr )
	{
		printf("HandleRtpRecv(%d)\r\n", Buf_ptr->payload);
		tagVideoPlayload_INFO PlayloadInfo = {0};
		RtpPacket Rtp;
		if( _CtrlInfo.hRtpDataNT )
		{
			int iLen = Rtp.Parse(Buf_ptr->data, Buf_ptr->payload, &PlayloadInfo);
			if( iLen == 1 )
			{
				if( PlayloadInfo.isFull )
				{
					if( _MemBuf_sptr->uLen == 0 )
					{
						//将RTP头的后四个字节替换成H264开始头
						PlayloadInfo.pData[PlayloadInfo.uDataLen - 4] = 0x00;
						PlayloadInfo.pData[PlayloadInfo.uDataLen - 3] = 0x00;
						PlayloadInfo.pData[PlayloadInfo.uDataLen - 2] = 0x00;
						PlayloadInfo.pData[PlayloadInfo.uDataLen - 1] = 0x01;

						_CtrlInfo.hRtpDataNT(_CtrlInfo.SessionID.c_str(), 
							&PlayloadInfo.pData[PlayloadInfo.uDataLen - 4], PlayloadInfo.uDataLen + 4);

						//test
						fwrite( &PlayloadInfo.pData[PlayloadInfo.uDataLen - 4], PlayloadInfo.uDataLen + 4, 1, g_fp11);
						fflush(g_fp11);
						//test
					}
					else
					{
						if( _MemBuf_sptr->uLen + PlayloadInfo.uDataLen < RTP_MAX_BUF )
						{
							memcpy(&_MemBuf_sptr->Buf[_MemBuf_sptr->uLen], PlayloadInfo.pData, PlayloadInfo.uDataLen);
							_MemBuf_sptr->uLen += PlayloadInfo.uDataLen;

							_CtrlInfo.hRtpDataNT(_CtrlInfo.SessionID.c_str(), 
								_MemBuf_sptr->Buf, _MemBuf_sptr->uLen);

							//test
							fwrite( _MemBuf_sptr->Buf, _MemBuf_sptr->uLen, 1, g_fp11);
							fflush(g_fp11);
							//test
						}
						
						_MemBuf_sptr->uLen = 4;
						const UInt8 H264Header[] = {0x00, 0x00, 0x00, 0x01};
						memcpy(_MemBuf_sptr->Buf, H264Header, sizeof(H264Header));
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
					}
				}
			}
		}
	}

	void HandleRtpDestroy( NETHANDLE Node, UInt16 uPort )
	{
		printf("HandleRtpDestroy\r\n");
	}

	void HandleRtcpRecv( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr )
	{
		printf("HandleRtcpRecv(%d)\r\n", Buf_ptr->payload);
	}

	void HandleRtcpDestroy( NETHANDLE Node, UInt16 uPort )
	{
		printf("HandleRtcpDestroy\r\n");
	}

	int	ParseSdp( const char* c_szData, UInt32 uLen, tagSdpInfo* tagInfo )
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

		pStart = strstr((char*)c_szData, "m=video");
		if( pStart != NULL )
		{
			pStart = strstr(pStart, "a=control:");
			if( pStart != NULL )
			{//媒体控制路径
				pStart += strlen("a=control:");
				pEnd = strstr(pStart, "\r\n");
				if( pEnd != NULL )
				{
					tagInfo->Info[tagInfo->InfoCount].CtrlPath = _string_type(pStart, pEnd - pStart);
				}
			}
		}

		pStart = strstr((char*)c_szData, "a=rtpmap:");
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

	_string_type  ParseRet( const char* c_szData, UInt32 uLen )
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

	void AddSeq( void )
	{
		_uSeq ++;
	}

	UInt32 GetSeq( void )
	{
		return _uSeq;
	}

private:
	tagRtspCtrlInfo	_CtrlInfo;
	RtspPacket		_Rtsp;
	NETHANDLE		_hNet;
	UInt32			_uSeq;
	UInt8			_uStatus;
	_MEMBUF_sptr	_MemBuf_sptr;
};

class RtspClients
{
public:
	typedef SmartPTR<RtspClient_Impl>	RtspClient_Impl_sptr;
	typedef Container_Map_type<_string_type, RtspClient_Impl_sptr>	ClientsMap;

	typedef RtspClient_Impl::HFNExceptNT	HFNExceptNT;
	typedef RtspClient_Impl::HFNSdpNT		HFNSdpNT;
	typedef RtspClient_Impl::HFNSuccessNT	HFNSuccessNT;
	typedef RtspClient_Impl::HFNRtpDataNT	HFNRtpDataNT;

public:
	RtspClients( void )
		: _Pool( sizeof(RtspClient_Impl) )
	{
		
	}

	~RtspClients( void )
	{

	}

	void Init( UInt8 uRtspIOThreadNum = 1, UInt8 uRtspProcThreadNum = 1,
		UInt8 uRtpIOThreadNum = 1, UInt8 uRtpProcThreadNum = 1 )
	{
		GetAsynStreamClientsInstance()->Init(uRtspIOThreadNum, uRtspProcThreadNum, true);
		GetUDPStreamInstance()->Init(uRtpIOThreadNum, uRtpProcThreadNum);
	}

	void Release( void )
	{

	}

	int Open( const _string_type& sRtspPath, UInt16 uRtpPort,
		HFNExceptNT hExcepNT, HFNSdpNT hSdpNT,
		HFNSuccessNT hSuccessNT, HFNRtpDataNT hRtpDataNT )
	{
		_string_type sSessionID = Timestamp_type().DateMilliString();
		RtspClient_Impl_sptr sptr( _Pool.AllocObj<RtspClient_Impl>(),
								function20_bind_Ex(&MemPool_type::FreeObj<RtspClient_Impl>, &_Pool) );
		sptr->Init();
		int iRet = sptr->Open(sSessionID, sRtspPath, uRtpPort, 
			function20_bind(&RtspClients::HandleExcept, this, hExcepNT, sptr, _function_::_1, _function_::_2),
			hSdpNT, function20_bind(&RtspClients::HandleSuccess, this, hSuccessNT, sptr, _function_::_1),
			hRtpDataNT);
		if( iRet == 1 )
		{
			_Lock.Lock();
			_ClientsMap.insert(std::make_pair(sSessionID, sptr));
			_Lock.UnLock();
		}

		return iRet;
	}

	void Close( const char* c_szSessionID )
	{
		RtspClient_Impl_sptr sptr;
		_Lock.Lock();
		ClientsMap::iterator iter = _ClientsMap.find(c_szSessionID);
		if( iter != _ClientsMap.end() )
		{
			_ClientsMap.erase(iter);
			sptr = iter->second;
		}
		_Lock.UnLock();
		if( sptr )
			sptr->Close();
	}

protected:
	void HandleExcept( const HFNExceptNT& hExcepNT,
		const RtspClient_Impl_sptr& sptr,
		const char* c_szSessionID, Int32 iErrCode )
	{
		_Lock.Lock();
		ClientsMap::iterator iter = _ClientsMap.find(c_szSessionID);
		if( iter != _ClientsMap.end() )
		{
			_ClientsMap.erase(iter);
		}
		_Lock.UnLock();

		sptr->Close();
		if( hExcepNT )
		{
			hExcepNT(c_szSessionID, iErrCode);
		}
	}

	void HandleSuccess( const HFNSuccessNT& hSuccess, 
		const RtspClient_Impl_sptr& sptr,
		const char* c_szSessionID )
	{
		if( hSuccess )
		{
			hSuccess(c_szSessionID);
		}
	}
	
private:
	ClientsMap		_ClientsMap;
	MemPool_type	_Pool;
	CLock			_Lock;
};

void ExceptNotify(const char* c_szSessionID, Int32 iErrCode )
{

}

void SdpNotify(const char* c_szSessionID, RtspClient_Impl::tagSdpInfo* pSdp)
{

}

void SuccessNotify(const char* c_szSessionID)
{

}

void RtpDataNotify(const char* c_szSessionID, const char* c_szData, UInt16 uLen)
{
	//printf("RtpDataNotify(%d)\r\n", uLen);
}

int main(int argc, char* argv[])
{
	RtspClients Clients;
	Clients.Init();
	Clients.Open("rtsp://admin:abcd-1234@192.168.8.166:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
		8000, ExceptNotify, SdpNotify, SuccessNotify, RtpDataNotify);

	std::cin.get();
	return 0;
}