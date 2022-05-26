
#ifndef __RTMPFLVSESSION_20201009172804_1442199364_H__
#define __RTMPFLVSESSION_20201009172804_1442199364_H__

#include <libFrameWork_RtmpCommon/IRtmpSession.h>
#include <libThirdWrap_FFmpeg/FlvConverter.h>

namespace _rtmp_ {

	class FlvSession : public IRtmpSession
	{
	public:
		typedef FlvSession					self_type;
		typedef IRtmpSession::self_sptr		self_sptr;
		typedef SmartPTR<RTMPPacket>		RTMPPacket_sptr;
		typedef LinkList_type<tagBuffer>	PacketList;

		typedef IRtmpSession::HFNEvent		HFNEvent;

		static self_sptr Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(self_type));
			return self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind(&MemPool_type::FreeMemObj<self_type>, &s_Pool, _function_::_1));
		}

		static RTMPPacket_sptr AllocPacket(void)
		{
			static MemPool_type s_Pool(RTMP_PACKET_MAX_BUFFER_SIZE);
			return RTMPPacket_sptr(s_Pool.AllocObj<RTMPPacket>(),
				function20_bind_Ex(&MemPool_type::FreeObj<RTMPPacket>, &s_Pool));
		}

	public:
		FlvSession(void);

		~FlvSession(void);

		int Init(const HFNEvent& hEvent);

		void Release(void);

		//�������H264ͷ(0x00 0x00 0x00 0x01��0x00 0x00 0x01)
		int SendPacket(const char* c_szH264Data, UInt32 uH264Size);
		
		int Connect(const _rtmp_string& sUrl);

		int Close(bool isPassive = false);

	protected:

		int ConnectImpl(RTMP *r, RTMPPacket *cp, const _rtmp_string& sUrl);

		void HandleConnect(const self_sptr& sptr, const _rtmp_string& sUrl, const _rtmp_string& sIP,
			UInt16 uPort, NETHANDLE hNet);

		int Send(RTMPSockBuf *sb, const char *buf, int len);

		void HandleRecv(const self_sptr& sptr, NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain);

		void HandleDestroy(const self_sptr& sptr, NETHANDLE hNet);

		int SendSpsPps(void);

		int ParseH264Data(const char* c_szInData, UInt32 uInSize, PacketList& Packets);

	private:
		Timestamp_type::TimeVal	_TimeVal;
		Timestamp_type			_Time;
		RTMPPacket _RTMPPacket;
		NETHANDLE	_hNet;
		IVideoInfo*		_pVideo;
		_rtmp_string_big_t	_sData;
		UInt32				_uDstSize;
		HFNEvent			_hEvent;
		_ffmpeg_::FlvConverter	_FlvConverter;
	};

}

#endif

