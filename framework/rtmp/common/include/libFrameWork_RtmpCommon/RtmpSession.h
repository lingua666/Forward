
#ifndef __RTMPSESSION_20201009172804_1442199364_H__
#define __RTMPSESSION_20201009172804_1442199364_H__

#include "External.h"
#include <libThirdWrap_FFmpeg/FlvConverter.h>

namespace _rtmp_ {

	//定义包头长度，RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)

	//存储Nal单元数据的buffer大小
#define NAL_BUFFER_SIZE 32768

	//存储Nal单元数据的buffer大小
#define RTMP_PACKET_MAX_BUFFER_SIZE (NAL_BUFFER_SIZE + RTMP_HEAD_SIZE)

	class RtmpSession : public _enable_shared_from_this<RtmpSession>
	{
	public:
		struct tagVideoInfo
		{
			_string_type SPS;
			_string_type PPS;
			_rtmp_string_big_t SndData;
			UInt16		uRemain;	//发送缓冲区剩余数据
			UInt8		uVideoType;	//类型为枚举 enVideoEnc
			UInt32		uWidth;		//宽度
			UInt32		uHeight;	//高度
			UInt32		uMax_Framerate;	//帧率
			Int16		iWaitTime;		//每帧等待时间
		};

		struct tagBuffer
		{
			UInt32	uLen; /* the length of the buffer */
			char*	pBuf; /* the pointer to the buffer */
			bool	isKeyFrame;

			tagBuffer(void)
			{}

			tagBuffer(const tagBuffer& t1)
			{
				this->uLen = t1.uLen;
				this->pBuf = t1.pBuf;
				this->isKeyFrame = t1.isKeyFrame;
			}

			tagBuffer& operator=(const tagBuffer& t1)
			{
				this->uLen = t1.uLen;
				this->pBuf = t1.pBuf;
				this->isKeyFrame = t1.isKeyFrame;
				return *this;
			}
		};

		typedef RtmpSession			self_type;
		typedef SmartPTR<RtmpSession> self_sptr;
		typedef SmartPTR<RTMPPacket>  RTMPPacket_sptr;
		typedef LinkList_type<tagBuffer>	PacketList;

		typedef enum {
			RTMPT_OPEN = 0, RTMPT_SEND, RTMPT_IDLE, RTMPT_CLOSE
		} RTMPTCmd;

		typedef enum {
			RTMP_STATUS_NULL,
			RTMP_STATUS_RTMPT_OPEN,
			RTMP_STATUS_HandShake_C0C1,
			RTMP_STATUS_HandShake_S0S1S2,
			RTMP_STATUS_HandShake_C2,
			RTMP_STATUS_CONNECT_STREAM,	//connect
			RTMP_STATUS_WINACKSIZE,		//Window Acknowledgement Size
			RTMP_STATUS_Playing,		//播放
			RTMP_STATUS_Closing,		//停止
			RTMP_STATUS_Passive_Closing,	//被动关闭，对方关闭会话
			RTMP_STATUS_Destroy,		//会话销毁
			RTMP_STATUS_ERROR_Connect,	//连接失败
			RTMP_STATUS_ERROR_Stream,	//连接流失败
			RTMP_STATUS_ERROR_HandShake	//握手失败
		}RTMPStatus;
		
		typedef function20_handle<void(void)>	HandleCoroutines;
		typedef function20_handle<void(RTMPStatus, const self_sptr&, void*)>	HFNEvent;

		static self_sptr Alloc(void)
		{
			static MemPool_type s_Pool(sizeof(self_type));
			return self_sptr(s_Pool.AllocObj<self_type>(),
				function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
		}

		static RTMPPacket_sptr AllocPacket(void)
		{
			static MemPool_type s_Pool(RTMP_PACKET_MAX_BUFFER_SIZE);
			return RTMPPacket_sptr(s_Pool.AllocObj<RTMPPacket>(),
				function20_bind_Ex(&MemPool_type::FreeObj<RTMPPacket>, &s_Pool));
		}

	public:
		RtmpSession(void);

		~RtmpSession(void);

		int Init(const HFNEvent& hEvent);

		void Release(void);

		//必须包含H264头(0x00 0x00 0x00 0x01或0x00 0x00 0x01)
		int SendH264Packet(const char* c_szH264Data, UInt32 uH264Size);

		int SendH264ToFlvPacket(const char* c_szH264Data, UInt32 uH264Size);

		int SendPacket(RTMPPacket *packet, int queue);
		
		int Connect(const _rtmp_string& sUrl);

		int Close(bool isPassive = false);

		void SetVideoInfo(const tagVideoInfo& Info);

		const tagVideoInfo* GetVideoInfo(void) const
		{
			return &_VideoInfo;
		}

		bool IsOpen(void)
		{
			return _isOpen;
		}

		const RTMP* Get(void) const
		{
			return &_Rtmp;
		}

	protected:

		int SendFCUnpublish(RTMP *r);

		int SendDeleteStream(RTMP *r, double dStreamId);

		int ConnectImpl(RTMP *r, RTMPPacket *cp, const _rtmp_string& sUrl);

		void HandleConnect(const _rtmp_string& sUrl, const _rtmp_string& sIP,
			UInt16 uPort, NETHANDLE hNet);

		int CreateNetConnect(RTMP *r, RTMPPacket *cp);

		int HTTP_Post(RTMP *r, RTMPTCmd cmd, const char *buf, int len);

		int Send(RTMPSockBuf *sb, const char *buf, int len);

		int HTTP_readV20(RTMP *r, int fill, const char* c_szData, UInt32 uSize);

		int WriteN(RTMP *r, const char *buffer, int n);

		int ReadNV20(RTMP *r, const char *in_buffer, int in_n, char *out_buffer, int out_n);

		int SendBytesReceived(RTMP *r);

		int SendConnectPacket(RTMP *r, RTMPPacket *cp);

		int ClientPacket(RTMP *r, RTMPPacket *packet);

		int ReadPacketV20(RTMP *r, RTMPPacket *packet,
			const char* c_szData, UInt32 uSize, UInt32& out_uSize);

		int HandleInvoke(RTMP *r, const char *body, unsigned int nBodySize);

		void DecodeTEA(AVal *key, AVal *text);

		int SendSecureTokenResponse(RTMP *r, AVal *resp);

		int SendReleaseStream(RTMP *r);

		int SendFCPublish(RTMP *r);

		int SendServerBW(RTMP *r);

		int SendCtrl(RTMP *r, short nType, unsigned int nObject, unsigned int nTime);

		int SendCreateStream(RTMP *r);

		int SendFCSubscribe(RTMP *r, AVal *subscribepath);

		int SendPublish(RTMP *r);

		int SendPlaylist(RTMP *r);

		int SendPlay(RTMP *r);

		int SendCheckBW(RTMP *r);

		int SendPong(RTMP *r, double txn);

		int SendCheckBWResult(RTMP *r, double txn);

		int SendPause(RTMP *r, int DoPause, int iTime);

		void HandleCtrl(RTMP *r, const RTMPPacket *packet);

		int HandleMetadata(RTMP *r, char *body, unsigned int len);

		int SockBuf_FillV20(RTMPSockBuf *sb, const char* c_szData, UInt32 uSize);

		void HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain);

		void HandleDestroy(NETHANDLE hNet);

		int SendHandleHandShake(RTMPStatus Status, const char* c_szData = NULL, UInt32 uSize = 0);

		int SendSpsPps(void);

		int ParseH264Data(const char* c_szInData, UInt32 uInSize, PacketList& Packets);

	private:
		Timestamp_type::TimeVal	_TimeVal;
		Timestamp_type			_Time;
		RTMPStatus	_Status;
		RTMP _Rtmp;
		RTMPPacket _RTMPPacket;
		bool _isOpen;
		NETHANDLE	_hNet;
		tagVideoInfo	_VideoInfo;
		IVideoInfo*		_pVideo;
		_rtmp_string_big_t	_sData;
		UInt32				_uDstSize;
		HFNEvent			_hEvent;
		_ffmpeg_::FlvConverter	_FlvConverter;
	};

}

#endif

