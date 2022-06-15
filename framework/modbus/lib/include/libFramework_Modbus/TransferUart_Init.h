#ifndef __MODBUS_TRANSFERTCP_INIT_20180925172708_1445927228_H__
#define __MODBUS_TRANSFERTCP_INIT_20180925172708_1445927228_H__

#include <libFrameWork_Modbus/External.h>

namespace _modbus_ {

	namespace _rtu_ {

		struct TransferUart_Init
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

			TransferUart_Init(UInt16 uIOThreadNum = 1, UInt8 uProcThreadNum = 1)
			{
				_async_::_uarts_::GetAsyncUartsInstance()->Init(uIOThreadNum, uProcThreadNum);
				GetTimerEventInstance()->Init(1);
			}

			~TransferUart_Init(void)
			{

			}
		};

	}

}

#endif
