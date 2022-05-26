// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Gb28181/External.h>
#include <libFrameWork_Gb28181/gb28181_req.hpp>
#include <libFrameWork_Gb28181/gb28181_rsp.hpp>
#include <libFrameWork_Gb28181/GB28181_Proto.h>
#include <sstream>
#define SIP_NAMESPACE _sip_v20_

#define SERVER_ID		"34020000002000000002"
#define SERVER_IP		"192.168.1.58"	
#define SERVER_P0RT		5060
#define SERVER_REALM	"34020000"
#define DEVICE_USERNAME "34020000001320000001"
#define DEVICE_PASSWORD	"12345678"
#define DEVICE_NONCE	"1615184577"
#define DEVICE_RESPONSE	"f587dbc0dba4b43ee9d247c05519451d"

void HandleGB28181Destroy(NETHANDLE hNet, UInt16 uPort)
{
	printf("HandleGB28181Destroy\r\n");
}

void HandleGB28181RecvUdp(NETHANDLE hNet, struct sockaddr_in* pSrc, const _io_net_::StreamBuf_ptr& Buf_ptr)
{
	LOG_Print_Info(libFrameWork_GB28181, "HandleGB28181Recv Data(%d):%s", Buf_ptr->payload, Buf_ptr->data);

	_gb28181_::GB28181SessionUdp::HFNSend hSend = function20_bind(&_server_::_udp_::AppMain::Send,
		GetUDPStreamInstance(), hNet, _function_::_1, _function_::_2, _function_::_3, _function_::_4);
	_gb28181_::GB28181SessionUdp::HFNClose hClose = function20_bind(&_server_::_udp_::AppMain::Close,
		GetUDPStreamInstance(), hNet);
	_gb28181_::GB28181SessionUdp::HFNEvent hEvent;
	_gb28181_::GB28181SessionUdp::HFNSetRecv hSetRecv = function20_bind(&_server_::_udp_::AppMain::SetRecvHandle,
		GetUDPStreamInstance(), hNet, _function_::_1);
	_gb28181_::GB28181SessionUdp::HFNSetDestroy hSetDestroy;

	SIP_NAMESPACE::UriDetails_INFO LocalHost;
	LocalHost.Coding = SERVER_ID;
	LocalHost.Host.Host = SERVER_IP;
	LocalHost.Host.Port = SERVER_P0RT;

	_gb28181_::GB28181SessionUdp::self_sptr sptr = _gb28181_::GB28181SessionUdp::ParseRegister(hSend, hClose,
		hEvent, hSetRecv, hSetDestroy, pSrc, LocalHost, SERVER_REALM,
		DEVICE_NONCE, DEVICE_USERNAME, DEVICE_PASSWORD, Buf_ptr->data, Buf_ptr->payload);

	if (sptr)
	{
		sptr->GetCatalogInfo();
	}
}

void Gb28181TestUdp(void)
{
	LOG_Print_SetLog(libFrameWork_GB28181, 0);

	LOG_Print_Info(libFrameWork_GB28181, "Gb28181Test Start");

	GetUDPStreamInstance()->Init(1,1);
	NETHANDLE hNet = GetUDPStreamInstance()->Open(5060);
	if (hNet != -1)
	{
		GetUDPStreamInstance()->SetRecvHandle(hNet,
			function20_bind_Ex(HandleGB28181RecvUdp));

		GetUDPStreamInstance()->SetDestroyHandle(hNet,
			function20_bind(HandleGB28181Destroy, hNet, 5060));
	}

	while (true)
	{
		Sleep(1000);
	};
}

void HandleGB28181RecvTcp(NETHANDLE hNet, const _io_net_::StreamBuf_ptr& Buf_ptr)
{
	LOG_Print_Info(libFrameWork_GB28181, "HandleGB28181Recv Data(%d):%s", Buf_ptr->payload, Buf_ptr->data);

	_gb28181_::GB28181SessionTcp::HFNSend hSend = function20_bind(&_server_::_tcp_::StreamServer::Send,
		GetStreamSvrInstance(), hNet, _function_::_1, _function_::_2);
	_gb28181_::GB28181SessionTcp::HFNClose hClose = function20_bind(&_server_::_tcp_::StreamServer::Close,
		GetStreamSvrInstance(), hNet);
	_gb28181_::GB28181SessionTcp::HFNEvent hEvent;
	_gb28181_::GB28181SessionTcp::HFNSetRecv hSetRecv = function20_bind(&_server_::_tcp_::StreamServer::SetRecvHandle,
		GetStreamSvrInstance(), hNet, _function_::_1);
	_gb28181_::GB28181SessionTcp::HFNSetDestroy hSetDestroy;

	SIP_NAMESPACE::UriDetails_INFO LocalHost;
	LocalHost.Coding = SERVER_ID;
	LocalHost.Host.Host = SERVER_IP;
	LocalHost.Host.Port = SERVER_P0RT;

	_gb28181_::GB28181SessionTcp::self_sptr sptr = _gb28181_::GB28181SessionTcp::ParseRegister(hSend, hClose,
		hEvent, hSetRecv, hSetDestroy, LocalHost, SERVER_REALM,
		DEVICE_NONCE, DEVICE_USERNAME, DEVICE_PASSWORD, Buf_ptr->data, Buf_ptr->payload);

	if (sptr)
	{
		sptr->GetCatalogInfo();
	}
}

void HandleAccept(const char* c_szIP, int uPort, NETHANDLE hNet)
{
	GetStreamSvrInstance()->SetRecvHandle(hNet,
		function20_bind_Ex(HandleGB28181RecvTcp));

	GetStreamSvrInstance()->SetDestroyHandle(hNet,
		function20_bind(HandleGB28181Destroy, hNet, 5060));
}

void Gb28181TestTcp(void)
{
	LOG_Print_SetLog(libFrameWork_GB28181, 0);

	LOG_Print_Info(libFrameWork_GB28181, "Gb28181Test Start");

	GetStreamSvrInstance()->Init();
	if (GetStreamSvrInstance()->Listen(5060,
		function20_bind(HandleAccept, _function_::_1, _function_::_2, _function_::_3)))
	{
		GetStreamSvrInstance()->Run(get_processor_number() * 2, true);
	}

	while (true)
	{
		Sleep(1000);
	};
}

int main(int argc, char* argv[])
{
	Gb28181TestUdp();

	//Gb28181TestTcp();

	std::cin.get();
	return 0;
}