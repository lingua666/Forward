
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
			//_ReadWriteBarrier();//�ڴ�դ��, ǿ������ڴ��д, �������Ĵ������û�м�ʱд���ڴ�
			if (k < 4)
			{
			}
			else if (k < 16)
			{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
				_mm_pause();
#elif defined(PLATFORM_OS_FAMILY_UNIX)
				asm("nop"::);
				//__asm__ __volatile__ ("pause" : : :"memory");    //arm ƽ̨���޷�ʶ���ָ��
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
