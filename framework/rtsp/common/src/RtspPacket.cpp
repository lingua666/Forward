
#include <libFrameWork_Rtsp/RtspPacket.h>
#include <libFrameWork_Rtsp/IVideoInfo.h>

const char* g_szWeek20171221[] = {"Sun","Mon", "Tus", "Wed", "Thu", "Fri", "Sat"};
const char* g_szMonth20171221[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

RtspPacket::RtspPacket( void )
{

}

RtspPacket::~RtspPacket( void )
{

}

void RtspPacket::SetUserInfo( int iStreamType, const _string_type& sUser, const _string_type& sPassword,
	const _string_type& sHost, const _string_type& sPort )
{
	_UserInfo.uType = iStreamType;
	_UserInfo.sUser = sUser;
	_UserInfo.sPassword = sPassword;
	_UserInfo.sHost = sHost;
	_UserInfo.sPort = sPort;
}

_string_type RtspPacket::Describe( const char* szPath, const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	_sUrl = "";
	_sUrl += "rtsp://";
	if( _UserInfo.sUser != "" && _UserInfo.sPassword != "" )
	{
		_sUrl += _UserInfo.sUser + ":";
		_sUrl += _UserInfo.sPassword + "@";
	}
	_sUrl += _UserInfo.sHost;
	if( _UserInfo.sPort != "" )
	{
		_sUrl += ":";
		_sUrl += _UserInfo.sPort;
	}
	_sUrl += "/";
	_sUrl += szPath;
	s += "DESCRIBE ";
	s += _sUrl;
	s += " ";
	s += RTSP_VERSION_DEF;
	s += "\r\n";

	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	put_field("Accept", "application/sdp", s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Describe_Ack( const _string_type& sUrl, const tagVideo_INFO& VideoInfo,
	const char* c_szSessionID, const char* c_szMediaHeader,
	const char* c_szLevelID, const char* c_szSpropParameter,
	const char* c_szSeq, UInt8 uTransType )
{
	_string_type s;
	s.reserve(1024);
	s += "RTSP/1.0 200 OK\r\n";
	put_field("CSeq", c_szSeq, s);
	put_field("Content-Type", "application/sdp", s);

	_string_type sBaseUrl;
	size_t iPos= sUrl.find("?");
	if( iPos == -1 )
		sBaseUrl = sUrl;
	else
		sBaseUrl = sUrl.substr(0, iPos) + "/";
	put_field("Content-Base", sBaseUrl.c_str(), s);

	_string_type sSdp;
	sSdp = get_sdp_header(sUrl, c_szSessionID);
	get_sdp_video(sUrl, VideoInfo, c_szMediaHeader, c_szLevelID, c_szSpropParameter, sSdp, uTransType);

	put_field("Content-Length", _string_type::NumberToStr(sSdp.size() + 2).c_str(), s);
	s += "\r\n";
	s += sSdp;
	s += "\r\n";
	return s;
}

_string_type RtspPacket::get_sdp_header( const _string_type& sUrl, const char* c_szSessionID )
{
	_string_type s;
	s.reserve(500);

	int iStart = sUrl.find("rtsp://");
	if( iStart == -1 )
		return "";

	iStart = iStart + strlen("rtsp://");
	int iEnd = sUrl.find('@', iStart);
	if( iEnd != -1 )
	{//标准格式的用户名密码
		iStart = iEnd;
		iStart += 1;
	}

	iEnd = sUrl.find('/', iStart);
	_string_type sHost = sUrl.substr(iStart, iEnd - iStart);

	//SDP版本信息
	s += "v=0\r\n";
	//<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
	s += "o=- ";//"o=- 1271659412 1271659412 IN IP4 ";
	s += c_szSessionID;
	s += " ";
	s += c_szSessionID;
	s += " IN IP4 ";
	s += sHost + "\r\n";
	//会话名称
	s += "s=Media Presentation\r\n";
	//Email 地址
	s += "e=NONE\r\n";
	//会话活动时间
	s += "t=0 0\r\n";
	//连接信息
	s += "c=IN IP4 0.0.0.0\r\n";
	//描述了建议的带宽,包括两种CT和AS。
	//CT：ConferenceTotal，总带宽。
	//AS：Application-SpecificMaximum，单个媒体带宽的最大值
	s += "b=AS:5050\r\n";
	//控制指定流
	if( sUrl != "" )
	{
		s += "a=control:";
		s += sUrl;
		s += "\r\n";
	}

	return s;
}

_string_type RtspPacket::get_sdp_video( const _string_type& sUrl, const tagVideo_INFO& VideoInfo,
	const char* c_szMediaHeader, const char* c_szLevelID,
	const char* c_szSpropParameter, _string_type& sHeader, UInt8 uTransType )
{
	if( VideoInfo.uVideoType == IVideoInfo::ENUM_ENC_H264 ||
		VideoInfo.uVideoType == IVideoInfo::ENUM_ENC_H265 )
	{
		if(uTransType == ENUM_UDP_TYPE )
			sHeader += "m=video 0 RTP/AVP 96\r\n";
		else
			sHeader += "m=video 0 RTP/AVP/TCP 96\r\n";

		//bit rate
		sHeader += "b=AS:5000\r\n";
		//收发模式
		sHeader += "a=recvonly\r\n";
		//图像尺寸
		if( VideoInfo.uWidth != 0 && VideoInfo.uHeight != 0 )
		{
			sHeader += "a=x-dimensions:";
			sHeader += _string_type::NumberToStr(VideoInfo.uWidth);
			sHeader += ",";
			sHeader += _string_type::NumberToStr(VideoInfo.uHeight) + "\r\n";
		}

		//控制指定流
		if( sUrl != "" )
		{
			sHeader += "a=control:";
			sHeader += sUrl;
			sHeader += "\r\n";	
		}

		_string_type sType;
		if( VideoInfo.uVideoType == IVideoInfo::ENUM_ENC_H264 )
			sType = "H264";
		else
			sType = "H265";

		//<rtp负载类型><编码名>/<时钟速率 每秒多少个字节>
		if( VideoInfo.uFrequency != 0 )
		{
			sHeader += "a=rtpmap:96 ";
			sHeader += sType + "/";
			sHeader += _string_type::NumberToStr(VideoInfo.uFrequency) + "\r\n";
		}
		else
		{
			sHeader += "a=rtpmap:96 ";
			sHeader += sType + "\r\n";
		}

		if( VideoInfo.uFrameRate != 0 )
		{
			sHeader += "a=framerate:";
			sHeader += _string_type::NumberToStr(VideoInfo.uFrameRate) + "\r\n";
		}

		sHeader += "a=fmtp:96 ";

		//这个参数用于指示 H.264 流的 profile 类型和级别.
		//由 Base16(十六进制) 表示的 3 个字节.
		//第一个字节表示 H.264 的 Profile 类型,
		//第三个字节表示 H.264 的 Profile 级别:
		//在H264中除了00 00 00 01之后，SPS，一般是67 42 开头,
		//base64编码时从67开始。PPS从68开始。
		//profile-level-id 的值 是从SPS的第二个字节开始的三个字节，即从42开始的。
		sHeader += "profile-level-id=";
		sHeader += c_szLevelID;
		sHeader += "; ";
		//表示支持的封包模式. 
		//当 packetization-mode 的值为 0 时或不存在时, 必须使用单一 NALU 单元模式.
		//当 packetization-mode 的值为 1 时必须使用非交错(non-interleaved)封包模式.
		//当 packetization-mode 的值为 2 时必须使用交错(interleaved)封包模式.
		sHeader += "packetization-mode=1; ";

		//sprop-parameter-sets等于base64 sps+pps,要加第一个字节
		//这个参数可以用于传输 H.264 的序列参数集和图像参数 NAL 单元.
		//这个参数的值采用 Base64 进行编码. 不同的参数集间用","号隔开即SPS,PPS.
		sHeader += "sprop-parameter-sets=";
		sHeader += c_szSpropParameter;
		sHeader += "\r\n";

		if( c_szMediaHeader != NULL && strcmp(c_szMediaHeader,"") != 0 )
		{
			//40个字节头部
			sHeader += "a=Media_header:MEDIAINFO=";
			sHeader += c_szMediaHeader;
			sHeader += ";\r\n";
		}

		sHeader += "a=appversion:1.0";
	}
	else
	{//不支持

	}

	return sHeader;
}

_string_type RtspPacket::Setup( const char* c_szCtrlPath,
	const char* c_szRtpPort,
	const char* c_szRtcpPort,
	const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	s += "SETUP ";
	s += c_szCtrlPath;//_sUrl;
	s += " ";
	s += RTSP_VERSION_DEF;
	s += "\r\n";
	put_field("CSeq",c_szSeq, s);

	if( _UserInfo.uType == ENUM_UDP_TYPE)
	{
		_string_type sInfo = "RTP/AVP;unicast;client_port=";
		sInfo += c_szRtpPort;
		sInfo += "-";
		sInfo += c_szRtcpPort;
		put_field("Transport", sInfo.c_str(), s);
	}
	else
	{
		put_field("Transport", "RTP/AVP/TCP;unicast;interleaved=0-1", s);
	}
	put_field("User-Agent", RTSP_USER_AGENT, s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Setup_Ack( const char* c_szSessionUid,
	UInt8 uTimeout, unsigned long uSSrc,
	const char* c_szClientInfo, UInt16 uRtpPort,
	UInt16 uRtcpPort, const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	s += "RTSP/1.0 200 OK\r\n";
	put_field("CSeq", c_szSeq, s);

	_string_type sSession = c_szSessionUid;
	sSession += ";timeout=";
	sSession += _string_type::NumberToStr(uTimeout);
	put_field("Session", sSession.c_str(), s);
	_string_type sTransport;
	if( _UserInfo.uType == ENUM_UDP_TYPE )
	{//UDP
		sTransport += "RTP/AVP;";
		sTransport += "unicast;";
		sTransport += c_szClientInfo;
		sTransport += ";server_port=";
		sTransport += _string_type::NumberToStr(uRtpPort);
		sTransport += "-";
		sTransport += _string_type::NumberToStr(uRtcpPort);
		sTransport += ";";
	}
	else
	{
		sTransport += "RTP/AVP/TCP;unicast;interleaved=0-1;";
	}

	sTransport += "ssrc=";
	sTransport += _string_type::NumberToStr(uSSrc) + ";mode=\"play\"";
	put_field("Transport", sTransport.c_str(), s);
	put_field("Date", get_date().c_str(), s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Play( const char* SessionUid, const char* c_szSeq,
	const char* c_szRange, const char* c_szScale )
{
	_string_type sRange = "npt=0.000-";
	_string_type sScale;
	_string_type s;
	s.reserve(500);
	s += "PLAY ";
	s += _sUrl;
	s += " ";
	s += RTSP_VERSION_DEF;
	s += "\r\n";
	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	put_field("Session", SessionUid, s);
	
	if( c_szRange != NULL )
	{
		sRange = "npt=";
		sRange += c_szRange;
		sRange += "-";
	}

	put_field("Range", sRange.c_str(), s);

	if( c_szScale != NULL )
	{
		put_field("Scale", c_szScale, s);
	}

	s += "\r\n";
	return s;
}

_string_type RtspPacket::Play_Ack( const char* c_szUrl, const char* SessionUid, const char* c_szSeq,
	const char* c_szRtpSeq, const char* c_szRtpTime )
{
	_string_type s;
	s.reserve(500);
	s += "RTSP/1.0 200 OK\r\n";
	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	put_field("Session", SessionUid, s);
	put_field("Range", "npt=now-", s);
	_string_type sRTPInfo = "url=";
	sRTPInfo += c_szUrl;
	sRTPInfo += ";";
	sRTPInfo += "seq=";
	sRTPInfo += c_szRtpSeq;
	sRTPInfo += ";rtptime=";
	sRTPInfo += c_szRtpTime;
	put_field("RTP-Info", sRTPInfo.c_str(), s);
	put_field("Date", get_date().c_str(), s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Teardown( const char* szPath, const char* SessionUid, const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	s += "TEARDOWN ";
	s += _sUrl;
	s += " ";
	s += RTSP_VERSION_DEF;
	s += "\r\n";
	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	put_field("Session", SessionUid, s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Teardown_Ack( const char* SessionUid, const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	s += "RTSP/1.0 200 OK\r\n";
	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	put_field("Session", SessionUid, s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Option( const char* SessionUid, const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	s += "OPTIONS ";
	s += _sUrl;
	s += " ";
	s += RTSP_VERSION_DEF;
	s += "\r\n";
	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::Option_Ack( const char* c_szSeq )
{
	_string_type s;
	s.reserve(500);
	s += "RTSP/1.0 200 OK\r\n";
	put_field("CSeq",c_szSeq, s);
	put_field("User-Agent", RTSP_USER_AGENT, s);
	put_field("Public", "DESCRIBE, PLAY, SETUP, TEARDOWN, OPTION", s);
	s += "\r\n";
	return s;
}

_string_type RtspPacket::get_field( const char* c_szFieldName, const char* c_szData, UInt32 uLen, UInt32 uOff )
{
	if( c_szFieldName == NULL || c_szData == NULL || 
		uLen <= uOff )
		return "";
	_string_type sField = c_szFieldName;
	 sField += ":";

	const char* pStart = strstr(c_szData, sField.c_str());
	pStart += sField.size();
	const char* pEnd = strstr(pStart, "\r\n");
	if( pEnd == NULL )
		return "";

	return _string_type(pStart, pEnd - pStart);
}

void RtspPacket::put_field( const char* c_szFieldName, const char* c_szValue,
	_string_type& sOut )
{
	sOut += c_szFieldName;
	sOut += ": ";
	sOut += c_szValue;
	sOut += "\r\n";
}

_string_type RtspPacket::get_date( void )
{
	_string_type sDate;
	struct tm * timeinfo = Timestamp_type().epochTM();
	sDate += g_szWeek20171221[timeinfo->tm_wday];
	sDate += ", "; 
	sDate += g_szMonth20171221[timeinfo->tm_mon];
	sDate += " ";
	sDate += _string_type::NumberToStr(timeinfo->tm_mday);
	sDate += " ";
	sDate += _string_type::NumberToStr(timeinfo->tm_year + 1900);
	sDate += " ";
	sDate += _string_type::NumberToStr(timeinfo->tm_hour);
	sDate += ":";
	sDate += _string_type::NumberToStr(timeinfo->tm_min);
	sDate += ":";
	sDate += _string_type::NumberToStr(timeinfo->tm_sec);
	sDate += " GMT";
	return sDate;
}