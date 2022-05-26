#ifndef __MODBUS_MODBUSHANDLE_20170616104004_1444199361_H__
#define __MODBUS_MODBUSHANDLE_20170616104004_1444199361_H__

#include "External.h"

namespace _modbus_ {

	class ModbusHandle
	{
	public:
		typedef ModbusHandle	self_type;
		typedef SmartPTR<self_type>	self_sptr;

		enum ModbusType
		{
			EN_TYPE_RTU,
			EN_TYPE_TCP
		};

		static self_sptr	Alloc(void)
		{
			static MemPool_type	s_Pool(sizeof(ModbusHandle));
			return self_sptr(s_Pool.AllocObj<ModbusHandle>(),
				function20_bind_Ex(&MemPool_type::FreeObj<ModbusHandle>, &s_Pool));
		}

	public:
		ModbusHandle(void);

		~ModbusHandle(void);

		int Init(void);

		void Release(void);
	
		int Open(ModbusType Type, int iAddress = 0);

		int Close(void);

		modbus_t*	Get(void) const;

	private:
		modbus_t* _pModbus;
	};

}

#endif