// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Modbus/ModbusSession.h>
#include "Tool.cpp"

void HandleDestroy(NETHANDLE hNet)
{

}

int HandleRecv(_modbus_::ModbusSession* pSession, const _modbus_::ModbusHandle::self_sptr& sptr,
	int iRet, const char* c_szData, UInt32 uSize)
{
	if (iRet == -1)
	{//超时

	}
	else
	{//
		printf("HandleElectRecv(%d) Data:%s\r\n", uSize, _string_type::HexToStr(c_szData, uSize).c_str());
	}

	//厂家新主板电机检测
	/*GetTimerEventInstance()->push_back(function20_bind(&_modbus_::ModbusSession::SendRead, pSession,
		sptr, _modbus_::EN_MODBUS_FC_READ_HOLDING_REGISTERS, 0x000A, 0x04,
		"1", function20_bind(HandleRecv, pSession, sptr, _function_::_1, _function_::_2, _function_::_3), 0, 0), 6000);*/

	/*float V1 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)c_szData, EN_BOT_BIG)) * 30) / 10000;
	float V2 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[2], EN_BOT_BIG)) * 30) / 10000;
	float V3 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[4], EN_BOT_BIG)) * 30) / 10000;
	float V4 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[6], EN_BOT_BIG)) * 30) / 10000;

	printf("第一路电压:%f, 第二路电压:%f, 第三路电压:%f, 第四路电压:%f\r\n", V1, V2, V3, V4);

	float E1 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[8], EN_BOT_BIG)) * 30) / 10000;
	float E2 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[10], EN_BOT_BIG)) * 30) / 10000;
	float E3 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[12], EN_BOT_BIG)) * 30) / 10000;
	float E4 = ((float)((Int16)Conversion16ToByteOrder(*(UInt16*)&c_szData[14], EN_BOT_BIG)) * 30) / 10000;

	printf("第一路电流:%f, 第二路电流:%f, 第三路电流:%f, 第四路电流:%f\r\n", E1, E2, E3, E4);

	//无符号
	/ *GetTimerEventInstance()->push_back(function20_bind(&_modbus_::ModbusSession::SendRead, pSession,
	sptr, _modbus_::EN_MODBUS_FC_READ_HOLDING_REGISTERS, 0x0000, 0x10,
	"1", function20_bind(HandleRecv, pSession, sptr, _function_::_1, _function_::_2, _function_::_3), 0), 1000);* /

	//有符号
	GetTimerEventInstance()->push_back(function20_bind(&_modbus_::ModbusSession::SendRead, pSession,
		sptr, _modbus_::EN_MODBUS_FC_READ_HOLDING_REGISTERS, 0x0000, 0x10,
		"1", function20_bind(HandleRecv, pSession, sptr, _function_::_1, _function_::_2, _function_::_3), 0), 6000);*/

	return 1;
}

void TimeExce(_modbus_::ModbusSession*	pModbus,
	_modbus_::ModbusHandle::self_sptr sptr,
	_modbus_::ModbusHandle::self_sptr sptr2, 
	_modbus_::ModbusHandle::self_sptr sptr3, 
	_modbus_::ModbusHandle::self_sptr sptr4, 
	UInt16 isOpen)
{
	UInt16 uOpen = isOpen == 0x01 ? 0x00 : 0x01;

	pModbus->SendWrite(sptr, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
		"1", _modbus_::ModbusSession::HFNResponds(), 0);
	//Sleep(1000);
	pModbus->SendWrite(sptr2, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
		"2", _modbus_::ModbusSession::HFNResponds(), 0);
	//Sleep(1000);
	pModbus->SendWrite(sptr3, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
		"3", _modbus_::ModbusSession::HFNResponds(), 0);
	//Sleep(1000);
	pModbus->SendWrite(sptr4, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
		"4", _modbus_::ModbusSession::HFNResponds(), 0);

	GetTimerEventInstance()->push_back(function20_bind(TimeExce, pModbus, sptr, sptr2, sptr3, sptr4, uOpen),
		60000);
}

void CircuitBreakerTest(void)
{
	{
		GetTimerEventInstance()->Init(1);
		_modbus_::ModbusSession::RTU_InitEnvironment();

		_modbus_::ModbusSession	Modbus;
		while (true)
		{
			Modbus.Init();
			int iRet = Modbus.OpenRtu("COM3", "9600", "8", "1", "0", function20_bind_Ex(HandleDestroy));
			if (iRet == 1)
			{
				_modbus_::ModbusHandle::self_sptr sptr = _modbus_::ModbusHandle::Alloc();
				sptr->Init();
				sptr->Open(_modbus_::ModbusHandle::EN_TYPE_RTU, 0x01);

				_modbus_::ModbusHandle::self_sptr sptr2 = _modbus_::ModbusHandle::Alloc();
				sptr2->Init();
				sptr2->Open(_modbus_::ModbusHandle::EN_TYPE_RTU, 0x02);

				_modbus_::ModbusHandle::self_sptr sptr3 = _modbus_::ModbusHandle::Alloc();
				sptr3->Init();
				sptr3->Open(_modbus_::ModbusHandle::EN_TYPE_RTU, 0x03);

				_modbus_::ModbusHandle::self_sptr sptr4 = _modbus_::ModbusHandle::Alloc();
				sptr4->Init();
				sptr4->Open(_modbus_::ModbusHandle::EN_TYPE_RTU, 0x04);

				UInt16 uOpen = 0x00;

				//垃圾亭电机电压电流检测
				Modbus.SendWrite(sptr, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
					"1", _modbus_::ModbusSession::HFNResponds(), 0);
				//Sleep(1000);
				Modbus.SendWrite(sptr2, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
					"2", _modbus_::ModbusSession::HFNResponds(), 0);
				//Sleep(1000);
				Modbus.SendWrite(sptr3, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
					"3", _modbus_::ModbusSession::HFNResponds(), 0);
				//Sleep(1000);
				Modbus.SendWrite(sptr4, _modbus_::EN_MODBUS_FC_WRITE_SINGLE_REGISTER, 0x0000, (void*)&uOpen, sizeof(uOpen),
					"4", _modbus_::ModbusSession::HFNResponds(), 0);

				GetTimerEventInstance()->push_back( function20_bind(TimeExce, &Modbus, sptr, sptr2, sptr3, sptr4, uOpen),
					60000);
			}
			else
			{
				printf("Modbus.OpenTcp Failed!!\r\n");
			}

			Sleep(100000000);

			Modbus.Release();
		}
	}
}

int main(int argc, char* argv[])
{
	ModbusTCP();

	//CircuitBreakerTest();

	unsigned char see[] = { 0x01, 0x03, 0x00, 0x01, 0x00, 0x06 }; //, 0x94, 0x01
	UInt16 us12e = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(see, sizeof(see));
	int i = 0;

	{
		unsigned char see[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x01 }; //, 0x94, 0x01
		UInt16 us12e = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(see, sizeof(see));
		int i = 0;
	}

	{
 		unsigned char see[] = { 0x01, 0x03, 0x02, 0x00, 0x00 }; //, 0x94, 0x01
		UInt16 us12e = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(see, sizeof(see));
		int i = 0;
	}

	{
		unsigned char see[] = { 0x01, 0x03, 0x02, 0x00, 0x01 }; //, 0x94, 0x01
		UInt16 us12e = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(see, sizeof(see));
		int i = 0;
	}

	{
		unsigned char see[] = { 0x01, 0x03, 0x02, 0xFF, 0xFF }; //, 0x94, 0x01
		UInt16 us12e = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(see, sizeof(see));
		int i = 0;
	}


	unsigned char se[] = { 0x01, 0x03, 0x00, 0x15, 0x00, 0x02 }; //, 0x94, 0x01
	UInt16 us12 = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(se, sizeof(se));

	unsigned char sz000[] = {0x01, 0x03, 0x04, 0x48, 0x41, 0x07, 0x14};


	unsigned char sz[] = {0x02, 0x03, 0x04, 0x48, 0x41, 0x07, 0x14};// { 0x02, 0x03, 0x00, 0x01, 0x00, 0x02 }; //0xA4, 0x08;
	UInt16 us = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(sz, sizeof(sz));
	unsigned char sz1[] = { 0x01, 0x03, 0x00, 0x0A, 0x00, 0x11 }; //0xA5, 0xC4;
	UInt16 us1 = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(sz1, sizeof(sz1));

	unsigned char sz2[] = { 0x01, 0x03, 0x00, 0x0A, 0x00, 0x0F }; //0xA5, 0xC4;
	UInt16 us2 = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(sz2, sizeof(sz2));

	unsigned char sz3[] = { 0x01, 0x03, 0x00, 0x0C, 0x00, 0x08 }; //0xA5, 0xC4;
	UInt16 us3 = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(sz3, sizeof(sz3));

	{
		unsigned char sz[] = { 0x01, 0x03, 0x00, 0x1A, 0x00, 0x01 };
		UInt16 us3 = _modbus_::_rtu_::TransferUart::modbus_rtu_crc16(sz, sizeof(sz));
		int i = 0;
	}

	/*{
		while (true)
		{
			_modbus_::ModbusSession	Modbus;
			Modbus.Init();
			int iRet = Modbus.OpenRtu("COM3", "115200", "8", "1", "0", function20_bind_Ex(HandleDestroy));
			if (iRet == 1)
			{
				_modbus_::ModbusHandle::self_sptr sptr = _modbus_::ModbusHandle::Alloc();
				sptr->Init();
				sptr->Open(_modbus_::ModbusHandle::EN_TYPE_RTU);
				printf("Modbus.OpenRtu Success\r\n");
			}
			else
			{
				printf("Modbus.OpenTcp Failed!!\r\n");
			}

			//Modbus.Release();
		}
	}*/

	{
		GetTimerEventInstance()->Init(1);
		_modbus_::ModbusSession::RTU_InitEnvironment();

		_modbus_::ModbusSession	Modbus;
		while (true)
		{
			Modbus.Init();
			int iRet = Modbus.OpenRtu("COM3", "9600", "8", "1", "0", function20_bind_Ex(HandleDestroy));
			if (iRet == 1)
			{
				_modbus_::ModbusHandle::self_sptr sptr = _modbus_::ModbusHandle::Alloc();
				sptr->Init();
				sptr->Open(_modbus_::ModbusHandle::EN_TYPE_RTU, 0x01);

				//垃圾亭电机电压电流检测
				/*Modbus.SendRead(sptr, _modbus_::EN_MODBUS_FC_READ_HOLDING_REGISTERS, 0x0032, 0x01,
					"1", function20_bind(HandleRecv, &Modbus, sptr, _function_::_1, _function_::_2, _function_::_3), 0);*/

				char Val[] = { 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 };
				//char Val[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00 };

				Modbus.SendWrite(sptr, _modbus_::EN_MODBUS_FC_WRITE_MULTIPLE_REGISTERS, 0x0001, (void*)Val, sizeof(Val),
					"1", _modbus_::ModbusSession::HFNResponds(), 0);

				//厂家新主板电机检测
				Modbus.SendRead(sptr, _modbus_::EN_MODBUS_FC_READ_HOLDING_REGISTERS, 0x000A, 0x04,
					"1", function20_bind(HandleRecv, &Modbus, sptr, _function_::_1, _function_::_2, _function_::_3), 0);

				printf("Modbus.OpenRtu Success\r\n");
			}
			else
			{
				printf("Modbus.OpenTcp Failed!!\r\n");
			}

			//Sleep(10000000);

			Modbus.Release();
		}
	}

	{
		_modbus_::ModbusSession::TCP_InitEnvironment();

		while (true)
		{
			_modbus_::ModbusSession	Modbus;
			Modbus.Init();
			int iRet = Modbus.OpenTcp("192.168.1.58", 21000, function20_bind_Ex(HandleDestroy));
			if (iRet == 1)
			{
				_modbus_::ModbusHandle::self_sptr sptr = _modbus_::ModbusHandle::Alloc();
				sptr->Init();
				sptr->Open(_modbus_::ModbusHandle::EN_TYPE_TCP);
			}
			else
			{
				printf("Modbus.OpenTcp Failed!!\r\n");
			}

			//Modbus.Release();
		}
	}

	{
		_modbus_::ModbusSession::TCP_InitEnvironment();

		_modbus_::ModbusSession	Modbus;
		while (true)
		{
			Modbus.Init();
			int iRet = Modbus.OpenTcp("192.168.1.58", 21000, function20_bind_Ex(HandleDestroy));
			if (iRet == 1)
			{
				_modbus_::ModbusHandle::self_sptr sptr = _modbus_::ModbusHandle::Alloc();
				sptr->Init();
				sptr->Open(_modbus_::ModbusHandle::EN_TYPE_TCP);
			}
			else
			{
				printf("Modbus.OpenTcp Failed!!\r\n");
			}

			Modbus.Release();
		}
	}
	

	std::cin.get();
	return 0;
}