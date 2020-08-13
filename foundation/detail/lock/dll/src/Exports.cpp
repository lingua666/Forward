
#include <libLock/Lock.hpp>
#include <dllLock/Exports.h>

/*****************************************************************
/*函数说明：	分配同步锁句柄
/*参数说明：	
/*				
/*返回值：		同步锁句柄
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOCK _CALLTYPE MLock_Alloc( void )
{
	return reinterpret_cast<HLOCK>(new CLock());
}


/*****************************************************************
/*函数说明：	释放同步锁资源
/*参数说明：	hLock : 同步锁句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLock_Free( HLOCK hLock )
{
	if( hLock != NULL )
		delete reinterpret_cast<CLock*>(hLock);
}


/*****************************************************************
/*函数说明：	同步锁锁操作
/*参数说明：	hLock : 同步锁句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLock_Lock( HLOCK hLock )
{
	if( hLock != NULL )
		reinterpret_cast<CLock*>(hLock)->Lock();
}


/*****************************************************************
/*函数说明：	同步锁解锁操作
/*参数说明：	hLock : 同步锁句柄
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLock_UnLock( HLOCK hLock )
{
	if( hLock != NULL )
		reinterpret_cast<CLock*>(hLock)->UnLock();
}