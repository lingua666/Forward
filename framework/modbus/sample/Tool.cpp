// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Modbus/ModbusSession.h>
#include "Config.hpp"

typedef _app_::_app_string _app_string;
typedef _session_::_tcp_::StreamBuf_ptr		StreamBuf_ptr;

static _app_::Config	g_Cfg;
static NETHANDLE	g_hNet = -1;

inline void HandleWaterDestroy(NETHANDLE hNet)
{
	printf("HandleWaterDestroy()\r\n");
}

inline void HandleWaterRecv(NETHANDLE hNet, const StreamBuf_ptr& Buf_ptr, long lRemain)
{
	printf("Recv Data(%d):%s\r\n", Buf_ptr->payload,
		_app_string::HexToStr(Buf_ptr->data, Buf_ptr->payload).c_str());

	if (Buf_ptr->payload < 22)
	{
		printf("Parse data failed, The length is less than 22!!");
		return;
	}

	UInt32 uValue = Conversion32ToByteOrder(*(UInt32*)&Buf_ptr->data[14]);

	printf("Parse Data(%d) Data(Raw):%s, Data(float):%x.00\r\n",
		4, _string_type::HexToStr(&Buf_ptr->data[14], 4).c_str(), uValue >> 8);
	return;
}

inline void HandleWaterConnect(const _app_string& sIP, UInt16 uPort, NETHANDLE hNet)
{
	if (hNet == -1)
	{
		printf("HandleConnect Destination Failed, Host:%s:%d!!\r\n", sIP.c_str(), uPort);
		return;
	}

	GetAsynStreamClientsInstance()->SetRecvHandle(hNet,
		function20_bind(HandleWaterRecv, _function_::_1, _function_::_2, _function_::_3));

	if (GetAsynStreamClientsInstance()->SetDestroyHandle(hNet,
		function20_bind(HandleWaterDestroy, _function_::_1)) == -1)
	{//失败
		printf("HandleWaterConnect(%s:%d) SetDestroyHandle() Failed!!", sIP.c_str(), uPort);
		return;
	}
	
	g_hNet = hNet;

	//读取数据
	UInt16 uOffset = 0;
	char szData[100] = { 0 };
	szData[uOffset ++] = 0x68;
	szData[uOffset ++] = _app_string::StrToHex(g_Cfg.Local.UsageType)[0];
	_app_string Addr = _app_string::StrToHex(g_Cfg.Local.Addr485);
	memcpy(&szData[uOffset], Addr.c_str(), Addr.size());
	uOffset += Addr.size();
	szData[uOffset++] = 0x01;
	szData[uOffset++] = 0x03;
	szData[uOffset++] = 0x90;
	szData[uOffset++] = 0x1F;
	szData[uOffset++] = 0x00;

	UInt8 uCS = 0;
	for (int i = 0; i < uOffset; i ++)
	{
		uCS += szData[i];
	}

	szData[uOffset++] = uCS % 256;
	szData[uOffset++] = 0x16;

	if (GetAsynStreamClientsInstance()->Send(hNet, szData, uOffset) == -1)
	{
		printf("Send Data Failed!! Data(%d):%s\r\n", uOffset,
			_app_string::HexToStr(szData, uOffset).c_str());
		return;
	}

	printf("Send Data(%d):%s\r\n", uOffset,
		_app_string::HexToStr(szData, uOffset).c_str());
}

inline void HandleElectDestroy(NETHANDLE hNet)
{

}

inline int HandleElectRecv(_modbus_::ModbusSession* pSession, const _modbus_::ModbusHandle::self_sptr& sptr,
	int iRet, const char* c_szData, UInt32 uSize)
{
	if (iRet == -1)
	{//超时
		printf("Wait For Receive Timeout\r\n");
	}
	else
	{//
		Int32 Value = -1;
		if (uSize == 2)
			Value = Conversion16ToByteOrder(*(UInt16*)c_szData, EN_BOT_BIG);
		else if (uSize == 4)
			Value = Conversion32ToByteOrder(*(UInt32*)c_szData, EN_BOT_BIG);
		
		printf("Parse Data(%d) Data(Raw):%s, Data(Int):%d\r\n",
			uSize, _string_type::HexToStr(c_szData, uSize).c_str(), Value);
	}

	return 1;
}

inline void ModbusTCP(void)
{
	_app_string sPath = _file_::RWIni::GetCurrentPath().c_str();

#if defined(PLATFORM_OS_FAMILY_UNIX)
	sPath += "/Config/ModbusTool/Config.txt";
#else
	sPath += "\\Config\\ModbusTool\\Config.txt";
#endif // DEBUG

	if (g_Cfg.Load(sPath.c_str()) == -1)
	{
		printf("ModbusTCP Load Config Error, Path:%s\r\n", sPath.c_str());
		return;
	}

	printf("=====================================================\r\n");
	printf("Runing %s\r\nInformation:\r\n", _timestamp_::Timestamp().DateMilliString().c_str());
	printf("\tLocal\r\n");
	printf("\t---IP:%s\r\n", g_Cfg.Local.IP.c_str());
	printf("\t---Port:%s\r\n", g_Cfg.Local.Port.c_str());
	printf("\t---Addr485:%s\r\n", g_Cfg.Local.Addr485.c_str());
	printf("\t---StartReg:%s\r\n", g_Cfg.Local.StartReg.c_str());
	printf("\t---RegCount:%s\r\n", g_Cfg.Local.RegCount.c_str());
	printf("\t---DeviceType:%s\r\n", g_Cfg.Local.DeviceType.c_str());
	printf("\t---UsageType:%s\r\n", g_Cfg.Local.UsageType.c_str());
	printf("=====================================================\r\n");
	
	_modbus_::ModbusSession::TCP_InitEnvironment();
	if (g_Cfg.Local.DeviceType != "0")
	{
		_modbus_::ModbusHandle::self_sptr sptr = _modbus_::ModbusHandle::Alloc();
		_modbus_::ModbusSession	Modbus;
		Modbus.Init();
		int iRet = Modbus.OpenTcp(g_Cfg.Local.IP.c_str(),
			_app_string::StringToInt(g_Cfg.Local.Port.c_str()),
			function20_bind_Ex(HandleElectDestroy), true);
		if (iRet == 1)
		{
			sptr->Init();
			sptr->Open(_modbus_::ModbusHandle::EN_TYPE_RTU,
				_app_string::StringToInt(g_Cfg.Local.Addr485.c_str()));
			Modbus.SetSendConfig(0, 5000);

			Modbus.SendRead(sptr, _modbus_::EN_MODBUS_FC_READ_HOLDING_REGISTERS,
				_app_string::StrHexToInt(g_Cfg.Local.StartReg), _app_string::StringToInt(g_Cfg.Local.RegCount),
				"1", function20_bind(HandleElectRecv, &Modbus, sptr, _function_::_1, _function_::_2, _function_::_3), 0);
		}
		else
		{
			printf("Modbus.OpenTcp Failed!!\r\n");
		}

		while (true) Sleep(1000);
	}
	else
	{
		int iRet = GetAsynStreamClientsInstance()->Connect(g_Cfg.Local.IP.c_str(),
			_app_string::StringToInt(g_Cfg.Local.Port.c_str()),
			function20_bind(HandleWaterConnect, g_Cfg.Local.IP,
				_app_string::StringToInt(g_Cfg.Local.Port), _function_::_1));

		if (iRet == -1)
		{
			printf("Connect Destination Failed, Host:%s:%d!!\r\n",
				g_Cfg.Local.IP.c_str(), _app_string::StringToInt(g_Cfg.Local.Port.c_str()));
		}

		while (true) Sleep(1000);
	}
	
	std::cin.get();
}