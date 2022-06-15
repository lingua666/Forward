
#ifndef __MEDIAVIDEOSTREAM_20180418154204_3344199361_H__
#define __MEDIAVIDEOSTREAM_20180418154204_3344199361_H__

#include "External.h"
#include "RtspSession.h"

class MediaVideoStream : public _enable_shared_from_this<MediaVideoStream>
{
public:
	typedef SmartPTR<MediaVideoStream>		self_sptr;
	typedef RtspSession::self_sptr			RtspSession_sptr;
	typedef function20_handle<void (const MediaVideoStream::self_sptr&)>	HFNReady;
	typedef function20_handle<void (NETHANDLE, const StreamBuf_ptr&)>	HFNRecvData;
	typedef function20_handle<void (NETHANDLE, const StreamBuf_ptr&)>	HFNStreamProcessEvent;
	typedef function20_handle<void (void)>								HFNClose;
	typedef function20_handle<void (const _string_type&, UInt8) >		HFNMediaVideoIdle;

	typedef RtspPacket::tagVideo_INFO	tagVideo_INFO;

	enum enRtspSessiontatus
	{
		EN_INIT_STATUS,
		EN_RUN_STATUS,
		EN_STOP_STATUS
	};

	struct tagRtspSession_INFO
	{
		RtspSession_sptr RtspSession;
		UInt8 uStatus;
	};

	struct tagMediaInfo
	{
		UInt8 uVideoType;
		_string_type sProfileLevelID;
		_string_type sSpropParameterSets;
		UInt32		uWidth;
		UInt32		uHeight;
		UInt32		uFramerate;
	};

	typedef SmartPTR<tagRtspSession_INFO>										tagRtspSession_INFO_sptr;
	typedef Container_Map_type<_string_type, tagRtspSession_INFO_sptr>			RtspSession_Map;
	typedef LinkList_type<tagRtspSession_INFO_sptr>								RtspSession_List;
			
public:
	MediaVideoStream( void );

	~MediaVideoStream( void );

	int Init( UInt8 uVideoType = IVideoInfo::ENUM_ENC_H264 );

	void Release( void );

	MediaVideoStream::HFNStreamProcessEvent Open( const _string_type& sUrl, const HFNReady& hReady, 
		const HFNClose& hClose, const HFNMediaVideoIdle& hIdle = HFNMediaVideoIdle() );

	void Close( void );

	MediaVideoStream::tagMediaInfo* GetMediaInfo( void );

	int Register( const _string_type& sSessionUid, const RtspSession_sptr& sptr);

	void unRegister( const _string_type& sSessionUid );

	bool isReady( void ) const
	{
		return _isReady;
	}

protected:
	void HandleStreamProcessEvent( NETHANDLE Node, const StreamBuf_ptr& Buf_sptr );

private:
	HFNReady	_hReady;
	HFNClose	_hClose;
	HFNMediaVideoIdle _hIdle;
	UInt8		_uVideoType;
	IVideoInfo*	_pIVideoInfo;
	bool		_isReady;
	tagMediaInfo	_Info;
	CLock			_Lock;
	RtspSession_Map	_Map;
	RtspSession_List _List;
	MemPool_type	_Pool;
	UInt32			_uUserCount;
	_string_type	_sUrl;
};


#endif
