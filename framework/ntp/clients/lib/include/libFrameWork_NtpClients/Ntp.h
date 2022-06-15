
#ifndef __NTP_H__
#define __NTP_H__

#include "External.h"

#define NTP_TIMESTAMP_DELTA            2208988800ull

#define G_LI(packet)   (UInt8) ((packet.li_vn_mode & 0xC0) >> 6) /* (li   & 11 000 000) >> 6 */
#define G_VN(packet)   (UInt8) ((packet.li_vn_mode & 0x38) >> 3) /* (vn   & 00 111 000) >> 3 */
#define G_MODE(packet) (UInt8) ((packet.li_vn_mode & 0x07) >> 0) /* (mode & 00 000 111) >> 0 */

#define LI 3
#define VN 3   
#define MODE 3	//3:客户端  //4:服务端
#define STRATUM 0
#define POLL 4	//为2的指数次方，单位为秒。例如若设置为4，则表示是2的4次方，即16秒 取值范围为[4,17]
#define PREC -6


class NtpClients : public _enable_shared_from_this<NtpClients>
{
public:
	typedef NtpClients						self_type;
	typedef SmartPTR<NtpClients>			self_sptr;
	typedef _foundation_::_string_t			_ntp_string;
	typedef function20_handle<int (Int64)>	HEvent;
	
	/* Structure that defines the 48 byte NTP packet protocol */
	typedef struct ntp_packet
	{
		UInt8 li_vn_mode;		/* Eight bits. li, vn, and mode */
								/* li.   Two bits.   Leap indicator */
								/* vn.   Three bits. Version number of the protocol */
								/* mode. Three bits. Client will pick mode 3 for client */

		UInt8 stratum;         /* Eight bits. Stratum level of the local clock */
		UInt8 poll;            /* Eight bits. Maximum interval between successive messages */
		UInt8 precision;       /* Eight bits. Precision of the local clock */

		UInt32 rootDelay;      /* 32 bits. Total round trip delay time */
		UInt32 rootDispersion; /* 32 bits. Max error aloud from primary clock source */
		UInt32 refId;          /* 32 bits. Reference clock identifier */

		UInt32 refTm_s;        /* 32 bits. Reference time-stamp seconds */
		UInt32 refTm_f;        /* 32 bits. Reference time-stamp fraction of a second */

		UInt32 origTm_s;       /* 32 bits. Originate time-stamp seconds */
		UInt32 origTm_f;       /* 32 bits. Originate time-stamp fraction of a second */

		UInt32 rxTm_s;         /* 32 bits. Received time-stamp seconds */
		UInt32 rxTm_f;         /* 32 bits. Received time-stamp fraction of a second */

		UInt32 txTm_s;         /* 32 bits and the most important field the client cares about. Transmit time-stamp seconds */
		UInt32 txTm_f;         /* 32 bits. Transmit time-stamp fraction of a second */

	} ntp_packet;

	static self_sptr	Alloc(void);

public:
	NtpClients(void);

	~NtpClients(void);

	int Init(const HEvent& hEvent);

	void Release(void);

	int Open(const char* c_szUrl);

	void Close(void);

	int SendNtp(Int64* pTime = NULL);

	void SyncSystem(time_t Time);

protected:

	void HandleRecv(NETHANDLE hNet, struct sockaddr_in* pSrc,
		const _io_net_::StreamBuf_ptr& Buf_ptr);

	void HandleRecvBlock(int* pRet, Int64* pTime,
		NETHANDLE hNet, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr);

	void HandleDestroy(NETHANDLE hNet);

	int Send(const char* c_szData, UInt32 uSize);

private:
	bool IsOpen(void);

private:
	bool	_isOpen;
	NETHANDLE _hNet;
	HEvent _hEvent;
	struct sockaddr_in _DstAddr;
};

#endif /* __NTP_H__ */
