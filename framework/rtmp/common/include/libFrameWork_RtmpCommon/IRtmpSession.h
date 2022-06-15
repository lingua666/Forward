
#ifndef __IRTMPSESSION_20201009172804_1442199364_H__
#define __IRTMPSESSION_20201009172804_1442199364_H__

#include "External.h"

namespace _rtmp_ {

	//�����ͷ���ȣ�RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)

	//�洢Nal��Ԫ���ݵ�buffer��С
#define NAL_BUFFER_SIZE 32768

	//�洢Nal��Ԫ���ݵ�buffer��С
#define RTMP_PACKET_MAX_BUFFER_SIZE (NAL_BUFFER_SIZE + RTMP_HEAD_SIZE)

	//�����ͷ���ȣ�RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)

#define RTMP_SIG_SIZE 1536
#define RTMP_LARGE_HEADER_SIZE 12

	class IRtmpSession : public _enable_shared_from_this<IRtmpSession>
	{
	public:
		struct tagVideoInfo
		{
			_string_type SPS;
			_string_type PPS;
			_rtmp_string_big_t SndData;
			UInt16		uRemain;	//���ͻ�����ʣ������
			UInt8		uVideoType;	//����Ϊö�� enVideoEnc
			UInt32		uWidth;		//���
			UInt32		uHeight;	//�߶�
			UInt32		uMax_Framerate;	//֡��
			Int16		iWaitTime;		//ÿ֡�ȴ�ʱ��
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

		typedef IRtmpSession			self_type;
		typedef SmartPTR<IRtmpSession> self_sptr;

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
			RTMP_STATUS_Playing,		//����
			RTMP_STATUS_Closing,		//ֹͣ
			RTMP_STATUS_Passive_Closing,	//�����رգ��Է��رջỰ
			RTMP_STATUS_Destroy,		//�Ự����
			RTMP_STATUS_ERROR_Connect,	//����ʧ��
			RTMP_STATUS_ERROR_Stream,	//������ʧ��
			RTMP_STATUS_ERROR_HandShake	//����ʧ��
		}RTMPStatus;
		
		typedef function20_handle<void(RTMPStatus, const self_sptr&, void*)>	HFNEvent;

	public:
		IRtmpSession(void)
			: _isOpen(false)
			, _Status(RTMP_STATUS_NULL)
		{
			memset(&_Rtmp, 0, sizeof(_Rtmp));
		}

		virtual ~IRtmpSession(void)
		{}

		virtual int Init(const HFNEvent& hEvent) = 0;

		virtual void Release(void) = 0;

		//�������H264ͷ(0x00 0x00 0x00 0x01��0x00 0x00 0x01)
		virtual int SendPacket(const char* c_szData, UInt32 uSize) = 0;
		
		virtual int Connect(const _rtmp_string& sUrl) = 0;

		virtual int Close(bool isPassive = false) = 0;

		virtual int Send(RTMPSockBuf *sb, const char *buf, int len) = 0;

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

		int SendPacket(RTMP *r, RTMPPacket *packet, int queue);

		int SendFCUnpublish(RTMP *r);

		int SendDeleteStream(RTMP *r, double dStreamId);

		int CreateNetConnect(RTMP *r, RTMPPacket *cp);

		int HTTP_Post(RTMP *r, RTMPTCmd cmd, const char *buf, int len);

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

		void IRtmpSession::AV_clear(RTMP_METHOD *vals, int num);

		int SendHandleHandShake(RTMP *r, RTMPStatus Status, const char* c_szData = NULL, UInt32 uSize = 0);

	protected:
		RTMPStatus		_Status;
		bool			_isOpen;
		RTMP			_Rtmp;
		tagVideoInfo	_VideoInfo;
	};

}

#endif

