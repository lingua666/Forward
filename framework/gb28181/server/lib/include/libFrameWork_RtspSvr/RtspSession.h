
#ifndef __RTSPSESSION_20171226154204_3344199361_H__
#define __RTSPSESSION_20171226154204_3344199361_H__

#include "UserSession.h"

class RtspSession : public _enable_shared_from_this<RtspSession>
{
public:
	typedef SmartPTR<RtspSession>		self_sptr;
	typedef function20_handle< void (NETHANDLE, const StreamBuf_ptr&) >	HFNMediaStreamData;
	typedef function20_handle< int (const self_sptr&, const _string_type&,
							const _string_type&, UInt8) >	HFNRtspSessionOPStatus;

	typedef RtspPacket::tagUser_INFO	tagUser_INFO;
	typedef	RtspPacket::tagVideo_INFO	tagVideo_INFO;
	typedef	RtspPacket::tagAudio_INFO	tagAudio_INFO;

	enum enUserSession_Status
	{
		EN_INIT_STATUS,
		EN_RUN_STATUS,
		EN_STOP_STATUS
	};

	struct tagUserSession_INFO
	{
		UserSession User;
		UInt8 uStatus;
	};

	enum enRTSPSESSION_OP_STATUS
	{
		EN_OP_INIT_STATUS,
		EN_OP_PLAY_STATUS,
		EN_OP_PAUSE_STATUS,
		EN_OP_STOP_STATUS
	};

	typedef SmartPTR<tagUserSession_INFO>		tagUserSession_INFO_sptr;
	typedef Container_HashMap_type<_string_type, tagUserSession_INFO_sptr>	UserSession_Map;
	typedef LinkList_type<tagUserSession_INFO_sptr, CNoneLock>	UserSession_List;

	struct tagRtp_INFO
	{
		UInt16		uRtpPort;
		UInt32		uSeq;
	};

public:
	RtspSession( void );

	~RtspSession( void );

	int Init( int iStreamType, const _string_type& sUser, const _string_type& sPassword,
		const _string_type& sHost, const _string_type& sPort, UInt8 uVideoType );

	void Release( void );

	int Open( NETHANDLE Node, const HFNRtspSessionOPStatus& hStatus, UInt8 uFrameRate );

	int Close( void );

	const RtspSession::HFNMediaStreamData& GetMediaStreamProcessEvent( void );

	_string_type	GetSessionID( void ) const;

	void HandleStreamRecv( NETHANDLE hNet, const StreamBuf_ptr& Buf_sptr );

protected:
	void HandleRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr );

	void CloseRtp( void );

	int Send( const _string_type& sData );

	int SetRtspUserInfo( const _string_type& sUrl, RtspPacket::enRtspStreamType Type );

private:
	RtspPacket	_Rtsp;
	NETHANDLE	_hNet;
	NETHANDLE	_hRtcp;
	tagRtp_INFO	_RtpInfo;
	_string_type	_sSessionID;
	UInt32	_uSsrc;
	UInt8	_uFrameRate;
	UserSession_Map	_UserMap;
	UserSession_List	_UserList;
	CLock			_Lock;
	MemPool_type	_Pool;
	HFNRtspSessionOPStatus	_hStatusEvent;
	UInt8 _uVideoType;
};

#endif