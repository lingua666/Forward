
#ifndef __RTSPCLIENT_IMPL_V20_20180104162504_3344199361_H__
#define __RTSPCLIENT_IMPL_V20_20180104162504_3344199361_H__

#include "RtspClient_Impl.h"

class RtspClient_Impl_V20 : public _enable_shared_from_this<RtspClient_Impl_V20>
{
public:
#pragma pack(push, 1)
	struct tagINTERLEAVED
	{
		UInt8	uMagic;
		UInt8	uChannel;
		UInt16	uLength;
	};

	struct tagTCP_RTP_INFO
	{
		tagINTERLEAVED	InterLeaved;
		UInt16			uRemain;
		char			Data[1];
	};

#pragma pack(pop)

	typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;
	enum enStatus
	{
		EN_STATUS_NULL,
		EN_STATUS_CLOSE,
		EN_STATUS_DESCRIBE_ACK,
		EN_STATUS_SETUP_ACK,
		EN_STATUS_PLAY_ACK,
		EN_STATUS_TEARDOWN_ACK,
		EN_STATUS_OPTIONS_ACK,
		EN_STATUS_RTP_DATA,
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

	typedef SmartPTR<_MEMBUF>		_MEMBUF_sptr;
	static MemPool_type* GetPoolInstance( void )
	{
		static MemPool_type s_Pool( sizeof(_MEMBUF) + RTP_MAX_BUF );
		return &s_Pool;
	}

	typedef function20_handle<void (const char*, Int32)>				HFNExceptNT;
	typedef function20_handle<void (const char*, tagSdpInfo*)>			HFNSdpNT;
	typedef function20_handle<void (const char*)>						HFNSuccessNT;
	typedef function20_handle<void (const char*, const _MEMBUF_sptr&)>	HFNRtpDataNT;

	struct tagRtspCtrlInfo
	{
		_string_type	RtspPath;
		UInt8 uStreamType;// udp/tcp
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

	struct tagRtspSvrInfo
	{//RTSP服务器信息
		_string_type	sIP;
		UInt16 uRtpPort;
		UInt16 uRtcpPort;
	};

public:
	RtspClient_Impl_V20( void );

	~RtspClient_Impl_V20( void );

	int Init( void );

	void Release( void );

	int Open( const _string_type& sSessionID, UInt8 uStreamType,
		const _string_type& sRtspPath, UInt16 uRtpPort,
		HFNExceptNT hExcepNT, HFNSdpNT hSdpNT,
		HFNSuccessNT hSuccessNT, HFNRtpDataNT hRtpDataNT );

	int Play( void );

	void Close( void );

	void SetResultEvent( const HFNExceptNT& hExcepNT, const HFNSuccessNT& hSuccessNT );

	void SetDataEvent( const HFNSdpNT& hSdpNT, const HFNRtpDataNT& hRtpDataNT );

	RtspClient_Impl_V20::tagRtspCtrlInfo& GetRtspCtrlInfo( void );

	void SetSeq( UInt32 uSeq );

	void AddSeq( void );

	UInt32 GetSeq( void );

	tagMediaVideoINFO* GetMediaVideoInfo( void )
	{
		return _Rtp.GetStreamInfo();
	}

protected:
	void InitBuf( void );

	void HandleRtspConnect( const _string_type& sPath, 
		NETHANDLE Handle );

	void HandleRtspRecv_Udp( const _string_type& sPath, 
		NETHANDLE Handle,
		const StreamBuf_ptr& Stream_sptr,
		long lRemain );

	void HandleRtspRecv_Tcp( const _string_type& sPath, 
		NETHANDLE Handle,
		const StreamBuf_ptr& Stream_sptr,
		long lRemain );

	void HandleRtspDestroy( NETHANDLE handle );

	void HandleRtpRecv( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr );

	void HandleRtpDestroy( NETHANDLE Node, UInt16 uPort );

	void HandleRtcpRecv( NETHANDLE Node, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr );

	void RtpRecv_Impl( const char* c_szData, UInt32 uLen );

	void HandleRtcpDestroy( NETHANDLE Node, UInt16 uPort );

	void HandleRtpCheck( void );

	void SendRtcpRRReport_Tcp( UInt32 uSSRC );

	void SendRtcpRRReport_Udp( UInt32 uSSRC );

	int	ParseSdp( const char* c_szData, UInt32 uLen, tagSdpInfo* tagInfo );

	_string_type  ParseRet( const char* c_szData, UInt32 uLen );

private:
	bool			_isIFrameEnable;	//丢帧等待I帧开关
	bool			_isWait_IFrame;
	tagRtspCtrlInfo	_CtrlInfo;
	tagRtspSvrInfo	_SvrInfo;
	RtspPacket		_Rtsp;
	RtcpPacket		_Rtcp;
	RtpPacket		_Rtp;
	NETHANDLE		_hNet;
	UInt32			_uSeq;
	UInt16			_SeqCycle;//
	UInt16			_uRecvSeq;
	UInt16			_uRecvSeqCycle;//
	UInt8			_uStatus;
	_MEMBUF_sptr	_MemBuf_sptr;
	Timestamp_type	_RtpIntervalTimer;
	CLock			_Lock;
	_string_type	_sTcpData;
	int				_ErrorCode;
};

#endif
