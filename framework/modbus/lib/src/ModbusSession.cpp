
#include <libFrameWork_Modbus/ModbusSession.h>
#include <libFrameWork_Modbus/TransferTcp_Init.h>

namespace _modbus_ {

	int ModbusSession::TCP_InitEnvironment(UInt16 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence)
	{
		return _modbus_::_tcp_::TransferTcp::InitEnvironment(uIOThreadNum, uProcThreadNum, isSequence);
	}

	int ModbusSession::RTU_InitEnvironment(UInt16 uIOThreadNum, UInt8 uProcThreadNum)
	{
		return _modbus_::_rtu_::TransferUart::InitEnvironment(uIOThreadNum, uProcThreadNum);
	}

	void ModbusSession::EnvironmentTimeLock(void)
	{
		_modbus_::_tcp_::TransferTcp_Init::TimeLock();
	}

	void ModbusSession::EnvironmentTimeUnLock(void)
	{
		_modbus_::_tcp_::TransferTcp_Init::TimeUnLock();
	}

	void ModbusSession::EnvironmentUpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime)
	{
		_modbus_::_tcp_::TransferTcp_Init::UpdateTime(LocalTime, NewTime);
	}

	ModbusSession::ModbusSession(void)
	{
		//test
		LOG_Print_SetLog(libFrameWork_Modbus, MLog_GetAllLevel());
		//test
	}

	ModbusSession::~ModbusSession(void)
	{
		Release();
	}

	int ModbusSession::Init(void)
	{
		return 1;
	}

	void ModbusSession::Release(void)
	{
		Close();
	}

	int ModbusSession::OpenRtu(const _mod_string& ComName, const _mod_string& BaudRate,
		const _mod_string& DataBit, const _mod_string& StopBit,
		const _mod_string& Parity, const HFNDestroy& hDestroy, bool isOriginal)
	{
		Transfer_sptr sptr = _rtu_::TransferUart::Alloc();
		sptr->Init(isOriginal);
		if (!sptr)
			return -1;

		if (((_rtu_::TransferUart*)sptr.get())->Open(ComName, BaudRate, DataBit,
			StopBit, Parity, hDestroy) == -1)
			return -1;

		_Transfer = sptr;
		return 1;
	}

	int ModbusSession::OpenTcp(const _mod_string& sIP, UInt16 uPort, const HFNDestroy& hDestroy)
	{
		Transfer_sptr sptr = _tcp_::TransferTcp::Alloc();
		if (!sptr)
			return -1;

		if (((_tcp_::TransferTcp*)sptr.get())->Open(sIP, uPort, hDestroy) == -1)
			return -1;

		_Transfer = sptr;
		return 1;
	}

	int ModbusSession::Close(void)
	{
		if (!_Transfer)
			return -1;
		return _Transfer->Close();
	}

	int ModbusSession::SendRead(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress, UInt32 uCount,
		const _mod_string& sIdentity, const HFNResponds& hResp, UInt8 u8Priority, tagSendConfig* pConfig)
	{
		uint8_t szData[1024] = { 0 };
		int iRet = ModbusRead(Handle, Code, uAddress, uCount, szData, sizeof(szData));
		if (iRet > 0)
		{
			if(pConfig == NULL)
				return _Transfer->Send((const char*)szData, iRet, sIdentity, hResp, u8Priority);
			else
				return _Transfer->Send((const char*)szData, iRet, sIdentity, hResp, u8Priority,
					pConfig->ReSendTimer, pConfig->ReSendInterval);
		}

		return -1;
	}

	int ModbusSession::SendWrite(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress, void* pData, UInt32 uSize,
		const _mod_string& sIdentity, const HFNResponds& hResp, UInt8 u8Priority, tagSendConfig* pConfig)
	{
		uint8_t szBuffer[1024] = { 0 };
		memcpy(szBuffer, pData, uSize);
		int iRet = ModbusWrite(Handle, Code, uAddress, szBuffer, uSize);
		if (iRet > 0)
		{
			if (pConfig == NULL)
				return _Transfer->Send((const char*)szBuffer, iRet, sIdentity, hResp, u8Priority);
			else
				return _Transfer->Send((const char*)szBuffer, iRet, sIdentity, hResp, u8Priority,
					pConfig->ReSendTimer, pConfig->ReSendInterval);
		}

		return -1;
	}

	int ModbusSession::ModbusRead(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress, UInt32 uCount,
		UInt8 *pOut_Data, UInt32 uOut_Size)
	{
		if (Code >= sizeof(g_fnModBus201801271800) / sizeof(fnModBusExec) ||
			g_fnModBus201801271800[Code] == NULL || !Handle)
			return -1;

		if (Code != EN_MODBUS_FC_READ_COILS &&
			Code != EN_MODBUS_FC_READ_DISCRETE_INPUTS &&
			Code != EN_MODBUS_FC_READ_HOLDING_REGISTERS &&
			Code != EN_MODBUS_FC_READ_INPUT_REGISTERS &&
			Code != EN_MODBUS_FC_READ_EXCEPTION_STATUS)
			return -1;

		if (Code == EN_MODBUS_FC_READ_COILS ||
			Code == EN_MODBUS_FC_READ_DISCRETE_INPUTS)
			uCount = uCount * 8;

		return g_fnModBus201801271800[Code](Handle->Get(), uAddress, uCount, pOut_Data);
	}

	int ModbusSession::ModbusWrite(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress,
		UInt8 *pInOut_Data, UInt32 uInOut_Size)
	{
		if (Code >= sizeof(g_fnModBus201801271800) / sizeof(fnModBusExec) ||
			g_fnModBus201801271800[Code] == NULL || !Handle)
			return -1;

		if (Code != EN_MODBUS_FC_WRITE_SINGLE_COIL &&
			Code != EN_MODBUS_FC_WRITE_SINGLE_REGISTER &&
			Code != EN_MODBUS_FC_WRITE_MULTIPLE_COILS &&
			Code != EN_MODBUS_FC_WRITE_MULTIPLE_REGISTERS)
			return -1;

		if (Code == EN_MODBUS_FC_WRITE_SINGLE_REGISTER ||
			Code == EN_MODBUS_FC_WRITE_MULTIPLE_REGISTERS)
			uInOut_Size = uInOut_Size / 2;

		if (Code == EN_MODBUS_FC_WRITE_SINGLE_REGISTER)
			return g_fnModBus201801271800[Code](Handle->Get(), uAddress, *(UInt16*)pInOut_Data, pInOut_Data);

		return g_fnModBus201801271800[Code](Handle->Get(), uAddress, uInOut_Size, pInOut_Data);
	}

}