
#include <libFrameWork_Modbus/ModbusHandle.h>

namespace _modbus_ {

	ModbusHandle::ModbusHandle(void)
		: _pModbus(NULL)
	{

	}

	ModbusHandle::~ModbusHandle(void)
	{
		Release();
	}

	int ModbusHandle::Init(void)
	{
		return 1;
	}

	void ModbusHandle::Release(void)
	{
		Close();
	}

	int ModbusHandle::Open(ModbusType Type, int iAddress)
	{
		switch (Type)
		{
		case EN_TYPE_RTU:
		{
			_pModbus = modbus_new_rtu();
			_pModbus->slave = iAddress;//从机地址
			return 1;
		}
		break;
		case EN_TYPE_TCP:
		{
			_pModbus = modbus_new_tcp();
			_pModbus->slave = 0;//从机地址
			return 1;
		}
		break;
		}

		return -1;
	}

	int ModbusHandle::Close(void)
	{
		if (_pModbus != NULL)
		{
			modbus_free(_pModbus);
			_pModbus = NULL;
			return 1;
		}

		return -1;
	}

	modbus_t*	ModbusHandle::Get(void) const
	{
		return _pModbus;
	}

}