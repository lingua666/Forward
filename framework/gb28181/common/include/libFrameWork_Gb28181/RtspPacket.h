
#ifndef __RTSPPACKET_20171226154204_3344199361_H__
#define __RTSPPACKET_20171226154204_3344199361_H__

#include "External.h"

#define RTSP_VERSION_DEF	"RTSP/1.0"
#define RTSP_USER_AGENT		"rtsp (Tranwit Streaming Media v2017.12.19)"

class RtspPacket
{
public:
	enum enRtspStreamType
	{
		ENUM_TCP_TYPE,
		ENUM_UDP_TYPE,
		ENUM_COUNT_TYPE
	};

	struct tagUser_INFO
	{
		UInt8	uType;
		_string_type sUser;
		_string_type sPassword;
		_string_type sHost;
		_string_type sPort;
	};

	struct tagVideo_INFO
	{
		UInt8	uVideoType;
		UInt8	uFrameRate;
		UInt32	uWidth;
		UInt32	uHeight;
		UInt32	uFrequency;
	};

	struct tagAudio_INFO
	{
		_string_type sVersion;
	};

	static 	_string_type get_field( const char* c_szFieldName, const char* c_szData, UInt32 uLen, UInt32 uOff = 0 );

	static 	_string_type Describe_Ack( const _string_type& sUrl, const tagVideo_INFO& VideoInfo,
		const char* c_szSessionID, const char* c_szMediaHeader,
		const char* c_szLevelID, const char* c_szSpropParameter,
		const char* c_szSeq, UInt8 uTransType );

	static _string_type Option_Ack( const char* c_szSeq );

protected:
	//c_szSessionID: 建议使用NTP(Network Time Protocol)timestamp
	static 	_string_type get_sdp_header( const _string_type& sUrl, const char* c_szSessionID );

	static 	_string_type get_sdp_video( const _string_type& sUrl, const tagVideo_INFO& VideoInfo,
		const char* c_szMediaHeader, const char* c_szLevelID,
		const char* c_szSpropParameter, _string_type& sHeader, UInt8 uTransType );

	static 	void put_field( const char* c_szFieldName, const char* c_szValue,
		_string_type& sOut );

public:
	RtspPacket( void );

	~RtspPacket( void );

	void SetUserInfo( int iStreamType, const _string_type& sUser, const _string_type& sPassword,
		const _string_type& sHost, const _string_type& sPort );

	_string_type Describe( const char* szPath, const char* c_szSeq );

	_string_type Setup( const char* c_szCtrlPath,
		const char* c_szRtpPort,
		const char* c_szRtcpPort,
		const char* c_szSeq );

	_string_type Setup_Ack( const char* c_szSessionUid,
		UInt8 uTimeout, unsigned long uSSrc,
		const char* c_szClientInfo, UInt16 uRtpPort,
		UInt16 uRtcpPort, const char* c_szSeq );

	_string_type Play( const char* SessionUid, const char* c_szSeq,
		const char* c_szRange = NULL, const char* c_szScale = NULL );

	_string_type Play_Ack( const char* c_szUrl, const char* SessionUid, const char* c_szSeq,
		const char* c_szRtpSeq, const char* c_szRtpTime );

	_string_type Teardown( const char* szPath, const char* SessionUid, const char* c_szSeq );

	_string_type Teardown_Ack( const char* SessionUid, const char* c_szSeq );

	_string_type Option( const char* SessionUid, const char* c_szSeq );

	void SetUrl( const _string_type& sUrl )
	{
		_sUrl = sUrl;
	}

protected:

	_string_type get_date( void );

private:
	_string_type	_sUrl;
	tagUser_INFO	_UserInfo;
};

#endif