
#include <dllFrameWork_Modbus/Exports.h>
#include <libFrameWork_Modbus/ModbusSession.h>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172032,
	"45000001000U2016021700000000UZZZ00000000",
	"Modbusͨ��ģ��",
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
/*����˵����	��ʼ��modbus tcpģ��
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Tcp(UInt16 uIOThreadNum, UInt8 uProcThreadNum, bool isSequence)
{
	return _modbus_::ModbusSession::TCP_InitEnvironment(uIOThreadNum, uProcThreadNum, isSequence);
}


/*****************************************************************
/*����˵����	��ʼ��modbus tcpģ��
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0:������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Init_Rtu(UInt16 uIOThreadNum, UInt8 uProcThreadNum)
{
	return _modbus_::ModbusSession::RTU_InitEnvironment(uIOThreadNum, uProcThreadNum);
}


/*****************************************************************
/*����˵����	�ͷ�modbusģ����Դ(�ڲ�ʹ�ø�ģ��ʱʹ��)
/*����˵����
/*
/*����ֵ��		1:�ɹ�  <0: ������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MModbus_Free(void)
{
	return 1;
}