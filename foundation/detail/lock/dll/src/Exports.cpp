
#include <libLock/Lock.hpp>
#include <dllLock/Exports.h>

/*****************************************************************
/*����˵����	����ͬ�������
/*����˵����	
/*				
/*����ֵ��		ͬ�������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HLOCK _CALLTYPE MLock_Alloc( void )
{
	return reinterpret_cast<HLOCK>(new CLock());
}


/*****************************************************************
/*����˵����	�ͷ�ͬ������Դ
/*����˵����	hLock : ͬ�������
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLock_Free( HLOCK hLock )
{
	if( hLock != NULL )
		delete reinterpret_cast<CLock*>(hLock);
}


/*****************************************************************
/*����˵����	ͬ����������
/*����˵����	hLock : ͬ�������
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLock_Lock( HLOCK hLock )
{
	if( hLock != NULL )
		reinterpret_cast<CLock*>(hLock)->Lock();
}


/*****************************************************************
/*����˵����	ͬ������������
/*����˵����	hLock : ͬ�������
/*				
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MLock_UnLock( HLOCK hLock )
{
	if( hLock != NULL )
		reinterpret_cast<CLock*>(hLock)->UnLock();
}