#ifndef __MODBUS_MODBUSSESSION_20170616104004_1444199361_H__
#define __MODBUS_MODBUSSESSION_20170616104004_1444199361_H__

#include "External.h"
#include <libFrameWork_Modbus/ModbusHandle.h>
#include <libFrameWork_Modbus/TransferTcp.h>
#include <libFrameWork_Modbus/TransferUart.h>

namespace _modbus_ {

	enum enModBus_FuncCode
	{
		EN_MODBUS_FC_READ_COILS = 0x01,
		EN_MODBUS_FC_READ_DISCRETE_INPUTS = 0x02,
		EN_MODBUS_FC_READ_HOLDING_REGISTERS = 0x03,
		EN_MODBUS_FC_READ_INPUT_REGISTERS = 0x04,
		EN_MODBUS_FC_WRITE_SINGLE_COIL = 0x05,
		EN_MODBUS_FC_WRITE_SINGLE_REGISTER = 0x06,
		EN_MODBUS_FC_READ_EXCEPTION_STATUS = 0x07,
		EN_MODBUS_FC_WRITE_MULTIPLE_COILS = 0x0F,
		EN_MODBUS_FC_WRITE_MULTIPLE_REGISTERS = 0x10
	};

	enum enModBus_ErrorCode
	{
		EN_MODBUS_ERROR_ILLEGALITY_FUNCTION = 0x01,			//非法功能
		EN_MODBUS_ERROR_ILLEGALITY_ADDRESS = 0x02,			//非法数据地址
		EN_MODBUS_ERROR_ILLEGALITY_VALUE = 0x03,			//非法数据值
		EN_MODBUS_ERROR_SLAVE_FAULT = 0x04,					//从站设备故障
		EN_MODBUS_ERROR_CONFIRM = 0x05,						//确认
		EN_MODBUS_ERROR_SLAVE_BUSY = 0x07,					//从属设备忙
		EN_MODBUS_ERROR_MEMORY_PARITY = 0x08,				//存储奇偶性差错
		EN_MODBUS_ERROR_GATEWAY_INVALID = 0x0A,				//不可用网关路径
		EN_MODBUS_ERROR_GATEWAY_NO_RESPONSE = 0x0B			//网关目标设备响应失败
	};

	typedef int(*fnModBusExec)(modbus_t*, int, int, UInt8*);

	const fnModBusExec	g_fnModBus201801271800[] = { NULL ,
		(fnModBusExec)modbus_read_bits,
		(fnModBusExec)modbus_read_input_bits,
		(fnModBusExec)modbus_read_registers,
		(fnModBusExec)modbus_read_input_registers,
		(fnModBusExec)modbus_write_bit,
		(fnModBusExec)modbus_write_register,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		(fnModBusExec)modbus_write_bits,
		(fnModBusExec)modbus_write_registers
	};

	const UInt8	g_Fc_Code_Byte201801271800[] = { 0,
		1,
		1,
		2,
		2,
		1,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		1,
		2
	};

	const UInt8	g_Fc_Code_Error201801271800[] = { 0,
		0x81,
		0x82,
		0x83,
		0x84,
		0, 0, 0, 0, 0, 0, 0,
		0x8F,
		0x90
	};

	class ModbusSession
	{
	public:
		typedef ModbusSession						self_type;
		typedef SmartPTR<ModbusSession>				self_sptr;
		typedef function20_handle<void(THandle)>	HFNDestroy;
		typedef ITransfer::self_sptr				Transfer_sptr;
		typedef ITransfer::HFNResponds				HFNResponds;
		typedef ITransfer::HFNRecvEvent				HFNRecvEvent;
		typedef ModbusHandle::self_sptr				ModbusHandle_sptr;
		
		typedef struct tagSendConfig
		{
			Int16 ReSendTimer;
			Int16 ReSendInterval;
		}tagSendConfig;

		static int TCP_InitEnvironment(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1, bool isSequence = true);

		static int RTU_InitEnvironment(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1);

		static void EnvironmentTimeLock(void);

		static void EnvironmentTimeUnLock(void);

		static void EnvironmentUpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime);

		static self_sptr	Alloc(void)
		{
			static MemPool_type	s_Pool(sizeof(ModbusSession));
			return self_sptr(s_Pool.AllocObj<ModbusSession>(),
				function20_bind_Ex(&MemPool_type::FreeObj<ModbusSession>, &s_Pool));
		}

	public:
		ModbusSession(void);

		~ModbusSession(void);

		int Init(void);

		void Release(void);

		int Close(void);

		int OpenRtu(const _mod_string& ComName, const _mod_string& BaudRate,
			const _mod_string& DataBit, const _mod_string& StopBit,
			const _mod_string& Parity, const HFNDestroy& hDestroy, bool isOriginal = false);

		int OpenTcp(const _mod_string& sIP, UInt16 uPort,
			const HFNDestroy& hDestroy, bool isRtu = false);

		int SendRead(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress, UInt32 uCount,
			const _mod_string& sIdentity = "", const HFNResponds& hResp = HFNResponds(),
			UInt8 u8Priority = 0, tagSendConfig* pConfig = NULL);

		int SendWrite(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress, void* pData, UInt32 uSize,
			const _mod_string& sIdentity = "", const HFNResponds& hResp = HFNResponds(),
			UInt8 u8Priority = 0, tagSendConfig* pConfig = NULL);

		void SetSendConfig(UInt16 uReSendTime, UInt16 uReSendInterval, UInt32 uOccupyTimer = -1)
		{
			if (_Transfer)
				_Transfer->SetConfig(uReSendTime, uReSendInterval, uOccupyTimer);
		}

		const Transfer_sptr& Get(void)
		{
			return _Transfer;
		}

	protected:
		int ModbusRead(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress, UInt32 uCount,
			UInt8 *pOut_Data, UInt32 uOut_Size);

		int ModbusWrite(const ModbusHandle_sptr& Handle, enModBus_FuncCode Code, UInt32 uAddress,
			UInt8 *pInOut_Data, UInt32 uInOut_Size);

	private:
		Transfer_sptr	_Transfer;
	};

}

#endif