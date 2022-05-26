
#include <libFrameWork_NtpClients/Ntp.h>

NtpClients::self_sptr	NtpClients::Alloc(void)
{
	static MemPool_type s_Pool(sizeof(self_type));
	return self_sptr(s_Pool.AllocObj<self_type>(),
		function20_bind_Ex(&MemPool_type::FreeObj<self_type>, &s_Pool));
}


NtpClients::NtpClients(void)
	: _hNet(-1)
	, _isOpen(false)
{
	//test
	LOG_Print_SetLog(libFrameWork_NtpClients, MLog_GetAllLevel());
	//test
}

NtpClients::~NtpClients(void)
{
	Release();
}

int NtpClients::Init(const HEvent& hEvent)
{
	_hEvent = hEvent;
	return 1;
}

void NtpClients::Release(void)
{
	Close();
}

int NtpClients::Open(const char* c_szUrl)
{
	//test
	LOG_Print_Info(libFrameWork_NtpClients, "NtpClients::Open() Open begin Url:%s", c_szUrl);
	//test

	char* pData = (char*)c_szUrl;
	UInt16 uPort = 123;
	_ntp_string sIP = c_szUrl;
	int iPos = sIP.find(":");
	if (iPos != -1)
	{
		uPort = _ntp_string::StringToInt(_ntp_string(&sIP[iPos + 1]));
		sIP = _ntp_string(sIP.c_str(), iPos);
	}

	int iCount = 0;
	sIP = GetIPByDomain(sIP.c_str(), &iCount).c_str();

	int iRet = GetSockAddrInFormIPPort(sIP.c_str(), uPort, &_DstAddr);
	if (iRet == -1)
	{
		LOG_Print_Error(libFrameWork_NtpClients, "NtpClients::Open() GetSockAddrInFormIPPort(%s:%d) Open Failed",
			sIP.c_str(), uPort);
		return -1;
	}

	UInt16 uStart = 50000, uEnd = 60000;
	UInt16 uTraversal = 0;

	do
	{
		_hNet = GetUDPStreamInstance()->Open(uStart);
		if (_hNet == -1)
		{
			++uStart;
		}
		else
		{
			//test
			LOG_Print_Info(libFrameWork_NtpClients, "NtpClients::Open(%d) Open Success", uStart);
			//test
			break;
		}

		if (uStart >= uEnd)
		{
			LOG_Print_Error(libFrameWork_NtpClients, "NtpClients::Open() Failed");
			return -1;
		}

	} while (true);

	if (GetUDPStreamInstance()->SetDestroyHandle(_hNet,
		function20_bind(&NtpClients::HandleDestroy, shared_from_this(), _function_::_1)) == -1)
	{
		LOG_Print_Error(libFrameWork_NtpClients, "NtpClients::Open() SetDestroyHandle() Failed");
		return -1;
	}

	_isOpen = true;
	GetUDPStreamInstance()->SetRecvHandle(_hNet,
		function20_bind(&NtpClients::HandleRecv, shared_from_this(),
			_function_::_1, _function_::_2, _function_::_3));

	//test
	LOG_Print_Info(libFrameWork_NtpClients, "NtpClients::Open(%s,%d) Open End Url:%s",
		sIP.c_str(), uStart, c_szUrl);
	//test

	return 1;
}

int NtpClients::SendNtp(Int64* pTime)
{
	//获取NTP
	int iRet = -1;
	if (pTime != NULL)
	{
		GetUDPStreamInstance()->SetRecvHandle(_hNet,
			function20_bind(&NtpClients::HandleRecvBlock, shared_from_this(),
				&iRet, pTime, _function_::_1, _function_::_2, _function_::_3));
	}

	ntp_packet packet = { 0 };
	packet.li_vn_mode = LI << 6 | VN << 3 | MODE;
	packet.stratum = STRATUM;
	packet.poll = POLL;
	packet.precision = PREC & 0xFF;

	if (Send((char*)&packet, sizeof(packet)) == -1)
	{
		LOG_Print_Error(libFrameWork_NtpClients, "NtpClients::SendNtp(%d) Send Failed", sizeof(packet));
		return -1;
	}

	if (pTime == NULL)
		return 1;

	int iNumber = 50;
	while (iNumber -- > 0)
	{
		if (iRet == 1)
			break;
		Sleep(100);
	};

	if (iRet == -1)
	{
		LOG_Print_Error(libFrameWork_NtpClients, "NtpClients::SendNtp() Time Out");
		return -2;//超时
	}

	return 1;
}

void NtpClients::HandleRecv(NETHANDLE hNet, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr)
{
	ntp_packet* pPacket = (ntp_packet*)Buf_ptr->data;

	/* These two fields contain the time-stamp seconds as the packet left the NTP server.
	The number of seconds correspond to the seconds passed since 1900.
	ntohl() converts the bit/byte order from the network's to host's "endianness". */
	pPacket->txTm_s = ntohl(pPacket->txTm_s); // Time-stamp seconds.
	pPacket->txTm_f = ntohl(pPacket->txTm_f); // Time-stamp fraction of a second.

											  /* Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
											  Subtract 70 years worth of seconds from the seconds since 1900.
											  This leaves the seconds since the UNIX epoch of 1970.
											  (1900)------------------(1970)**************************************(Time Packet Left the Server) */
	if (_hEvent)
	{
		time_t newTime = (time_t)(pPacket->txTm_s - NTP_TIMESTAMP_DELTA);
		if (_hEvent(newTime) == 1)
		{//时间同步到系统
			Timestamp_type Time(newTime * Timestamp_type::resolution());
#ifdef PLATFORM_OS_FAMILY_WINDOWS
			char szBuffer[100] = { 0 };
			sprintf(szBuffer, "date %s & time %s", Time.DateString("-").c_str(), Time.STimeString(":").c_str());
			system(szBuffer);
#else 
			_string_type sTime = "date -s '" + Time.localTimePrintf2() + "'";
			system(sTime.c_str());
#endif
		}
	}

	//test
	LOG_Print_Info(libFrameWork_NtpClients, "NtpClients::HandleRecv(%d,%d)",
		pPacket->txTm_s, pPacket->txTm_f);
	//test
}

void NtpClients::HandleRecvBlock(int* pRet, Int64* pTime,
	NETHANDLE hNet, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr)
{
	ntp_packet* pPacket = (ntp_packet*)Buf_ptr->data;

	/* These two fields contain the time-stamp seconds as the packet left the NTP server.
	The number of seconds correspond to the seconds passed since 1900.
	ntohl() converts the bit/byte order from the network's to host's "endianness". */
	pPacket->txTm_s = ntohl(pPacket->txTm_s); // Time-stamp seconds.
	pPacket->txTm_f = ntohl(pPacket->txTm_f); // Time-stamp fraction of a second.

											  /* Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
											  Subtract 70 years worth of seconds from the seconds since 1900.
											  This leaves the seconds since the UNIX epoch of 1970.
											  (1900)------------------(1970)**************************************(Time Packet Left the Server) */
	*pTime = (Int64)(pPacket->txTm_s - NTP_TIMESTAMP_DELTA);
	GetUDPStreamInstance()->SetRecvHandle(_hNet,
		function20_bind(&NtpClients::HandleRecv, shared_from_this(),
			_function_::_1, _function_::_2, _function_::_3));
	*pRet = 1;

	//test
	LOG_Print_Info(libFrameWork_NtpClients, "NtpClients::HandleRecvBlock(%d,%d)",
		pPacket->txTm_s, pPacket->txTm_f);
	//test
}


void NtpClients::SyncSystem(time_t TimeV)
{
	Timestamp_type Time(TimeV * Timestamp_type::resolution());
#ifdef PLATFORM_OS_FAMILY_WINDOWS
	char szBuffer[100] = { 0 };
	sprintf(szBuffer, "date %s & time %s", Time.DateString("-").c_str(), Time.STimeString(":").c_str());
	system(szBuffer);
#else 
	_string_type sTime = "date -s '" + Time.localTimePrintf2() + "'";
	system(sTime.c_str());
#endif
}


void NtpClients::HandleDestroy(NETHANDLE hNet)
{
	Close();
}

int NtpClients::Send(const char* c_szData, UInt32 uSize)
{
	int iRet = -1;
	if (IsOpen())
	{
		iRet = GetUDPStreamInstance()->Send(_hNet, &_DstAddr, c_szData, uSize);
		if (iRet == -1)
		{
			//test
			LOG_Print_Error(libFrameWork_NtpClients, "NtpClients::Send(%d) Failed", uSize);
			//test
		}
	}

	return iRet;
}

void NtpClients::Close(void)
{
	if (IsOpen())
	{
		_isOpen = false;
		if (_hNet != -1)
		{
			GetUDPStreamInstance()->Close(_hNet);
			_hNet = -1;
		}
	}
}

bool NtpClients::IsOpen(void)
{
	return _isOpen;
}
	