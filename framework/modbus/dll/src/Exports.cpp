
#include <dllFrameWork_Modbus/Exports.h>
#include <libFrameWork_Modbus/ModbusSession.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172032,
	"45000001000U2016021700000000UZZZ00000000",
	"Modbus通信模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(dllFramework_Modbus)

static  _modbus_::ModbusSession& GetModbusSessionInstance(void)
{
	static _modbus_::ModbusSession s_Modbus;
	return s_Modbus;
}


/*****************************************************************
/*函数说明：	初始化modbus tcp模块
/*参数说明：
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Tcp(UInt16 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence)
{
	return _modbus_::ModbusSession::TCP_InitEnvironment(uIOThreadNum, uProcThreadNum, isSequence);
}


/*****************************************************************
/*函数说明：	初始化modbus tcp模块
/*参数说明：
/*
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Rtu(UInt16 uIOThreadNum, UInt8 uProcThreadNum)
{
	return _modbus_::ModbusSession::RTU_InitEnvironment(uIOThreadNum, uProcThreadNum);
}


/*****************************************************************
/*函数说明：	释放modbus模块资源(在不使用该模块时使用)
/*参数说明：
/*
/*返回值：		1:成功  <0: 错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Free(void)
{
	return 1;
}