
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
	{//��׼��ʽ���û�������
		iStart = iEnd;
		iStart += 1;
	}

	iEnd = sUrl.find('/', iStart);
	_string_type sHost = sUrl.substr(iStart, iEnd - iStart);

	//SDP�汾��Ϣ
	s += "v=0\r\n";
	//<username> <sess-id> <sess-version> <nettype> <addrtype> <unicast-address>
	s += "o=- ";//"o=- 1271659412 1271659412 IN IP4 ";
	s += c_szSessionID;
	s += " ";
	s += c_szSessionID;
	s += " IN IP4 ";
	s += sHost + "\r\n";
	//�Ự����
	s += "s=Media Presentation\r\n";
	//Email ��ַ
	s += "e=NONE\r\n";
	//�Ự�ʱ��
	s += "t=0 0\r\n";
	//������Ϣ
	s += "c=IN IP4 0.0.0.0\r\n";
	//�����˽���Ĵ���,��������CT��AS��
	//CT��ConferenceTotal���ܴ���
	//AS��Application-SpecificMaximum������ý���������ֵ
	s += "b=AS:5050\r\n";
	//����ָ����
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
		//�շ�ģʽ
		sHeader += "a=recvonly\r\n";
		//ͼ��ߴ�
		if( VideoInfo.uWidth != 0 && VideoInfo.uHeight != 0 )
		{
			sHeader += "a=x-dimensions:";
			sHeader += _string_type::NumberToStr(VideoInfo.uWidth);
			sHeader += ",";
			sHeader += _string_type::NumberToStr(VideoInfo.uHeight) + "\r\n";
		}

		//����ָ����
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

		//<rtp��������><������>/<ʱ������ ÿ����ٸ��ֽ�>
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

		//�����������ָʾ H.264 ���� profile ���ͺͼ���.
		//�� Base16(ʮ������) ��ʾ�� 3 ���ֽ�.
		//��һ���ֽڱ�ʾ H.264 �� Profile ����,
		//�������ֽڱ�ʾ H.264 �� Profile ����:
		//��H264�г���00 00 00 01֮��SPS��һ����67 42 ��ͷ,
		//base64����ʱ��67��ʼ��PPS��68��ʼ��
		//profile-level-id ��ֵ �Ǵ�SPS�ĵڶ����ֽڿ�ʼ�������ֽڣ�����42��ʼ�ġ�
		sHeader += "profile-level-id=";
		sHeader += c_szLevelID;
		sHeader += "; ";
		//��ʾ֧�ֵķ��ģʽ. 
		//�� packetization-mode ��ֵΪ 0 ʱ�򲻴���ʱ, ����ʹ�õ�һ NALU ��Ԫģʽ.
		//�� packetization-mode ��ֵΪ 1 ʱ����ʹ�÷ǽ���(non-interleaved)���ģʽ.
		//�� packetization-mode ��ֵΪ 2 ʱ����ʹ�ý���(interleaved)���ģʽ.
		sHeader += "packetization-mode=1; ";

		//sprop-parameter-sets����base64 sps+pps,Ҫ�ӵ�һ���ֽ�
		//��������������ڴ��� H.264 �����в�������ͼ����� NAL ��Ԫ.
		//���������ֵ���� Base64 ���б���. ��ͬ�Ĳ���������","�Ÿ�����SPS,PPS.
		sHeader += "sprop-parameter-sets=";
		sHeader += c_szSpropParameter;
		sHeader += "\r\n";

		if( c_szMediaHeader != NULL && strcmp(c_szMediaHeader,"") != 0 )
		{
			//40���ֽ�ͷ��
			sHeader += "a=Media_header:MEDIAINFO=";
			sHeader += c_szMediaHeader;
			sHeader += ";\r\n";
		}

		sHeader += "a=appversion:1.0";
	}
	else
	{//��֧��

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