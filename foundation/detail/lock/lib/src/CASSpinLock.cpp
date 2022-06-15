
#include <libLock/CASSpinLock.h>
#include <libCommon/CASAtomic.h>
#include <libCommon/modafx.h>

namespace	_lock_{

	/*!
	* @function   CASLock
	* @brief    
	* @return  
	*/
	CASSpinLock::CASSpinLock( void )
		: _mutex(0)
	{

	}

	void CASSpinLock::Lock( void )
	{
		for(int k = 0; !CASCompareExchange(&_mutex, 0, 1); ++ k)
		{
			//_ReadWriteBarrier();//内存栅栏, 强制完成内存读写, 避免读入寄存器里的没有及时写回内存
			if (k < 4)
			{
			}
			else if (k < 16)
			{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
				_mm_pause();
#elif defined(PLATFORM_OS_FAMILY_UNIX)
				asm("nop"::);
				//__asm__ __volatile__ ("pause" : : :"memory");    //arm 平台下无法识别该指令
#endif
			}
			else if (k < 32)
			{
				Sleep(0);
			}
			else
			{
				Sleep(1);
			}
		}
	}

	void CASSpinLock::UnLock( void )
	{
		CASCompareExchange(&_mutex, 1, 0);
	}

	bool CASSpinLock::TryLock( void )
	{
		return CASCompareExchange(&_mutex, 0, 1) ? 1 : 0;
	}
}
