#ifndef __MODBUS_TRANSFERTCP_INIT_20180925172708_1445927228_H__
#define __MODBUS_TRANSFERTCP_INIT_20180925172708_1445927228_H__

#include <libFrameWork_Modbus/External.h>

namespace _modbus_ {

	namespace _tcp_ {

		struct TransferTcp_Init
		{
			static void TimeLock(void)
			{
				GetTimerEventInstance()->Lock();
			}

			static void TimeUnLock(void)
			{
				GetTimerEventInstance()->UnLock();
			}

			static void UpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime)
			{
				GetTimerEventInstance()->UpdateTime(LocalTime, NewTime);
			}

			TransferTcp_Init(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1, bool isSequence = true)
			{
				GetAsynStreamClientsInstance()->Init(uIOThreadNum, uProcThreadNum, isSequence);
				GetTimerEventInstance()->Init(1);
			}

			~TransferTcp_Init(void)
			{

			}
		};

	}

}

#endif
