
#include <libLock/CASLock.h>
#include <libCommon/CASAtomic.h>
#include <libCommon/modafx.h>

namespace	_lock_{

	//CASÔ­×ÓËø

	CASLock::CASLock( void )
		: _mutex(0)
	{

	}

	void CASLock::Lock(  void )
	{
		while( !CASCompareExchange(&_mutex, 0, 1) ) Sleep(0);
	}

	void CASLock::UnLock( void )
	{
		CASCompareExchange(&_mutex, 1, 0);
	}

	bool CASLock::TryLock( void )
	{
		return CASCompareExchange(&_mutex, 1, 0) ? 1 : 0;
	}
}
