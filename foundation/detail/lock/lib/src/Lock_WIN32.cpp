
#include <libLock/Lock_WIN32.h>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

#include <libCommon/CMyException.h>

namespace _lock_ {

		CLock_Win32::CLock_Win32( void ) 
			: _isLock(false)
		{ 
			// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
			// the return only checks if the input address of &_cs was valid, so it is safe to omit it
			InitializeCriticalSectionAndSpinCount( &m_cs, 4000 );
		}

		CLock_Win32::~CLock_Win32( void )	
		{ 
			if(_isLock)
				UnLock();
			DeleteCriticalSection( &m_cs );
		}

		bool CLock_Win32::TryLock( void )
		{
			const int sleepMillis = 5;
			int		iTryTime = 100;
			do
			{
				if (TryEnterCriticalSection(&m_cs) == TRUE)
					return true;
				Sleep(sleepMillis);
			}
			while (-- iTryTime);
			return false;
		}

		void CLock_Win32::Lock( void )
		{ 
			EnterCriticalSection( &m_cs );
			_isLock = true;
		}

		void CLock_Win32::UnLock( void )
		{
			LeaveCriticalSection( &m_cs );
			_isLock = false;
		}

		bool CLock_Win32::isLock( void )
		{
			return _isLock;
		}

		CEventLock::CEventLock( void )
		{
			m_Handle = ::CreateEvent(0,0,1,0);
		}

		CEventLock::~CEventLock( void )
		{
			UnLock();
			if(m_Handle != NULL)
			{
				::CloseHandle( m_Handle );
				m_Handle = NULL;			
			}
		}

		bool CEventLock::Lock( void )
		{
			LOCK_INVALID_ZERO(m_Handle);
			if ( ::WaitForSingleObject(m_Handle, INFINITE) == WAIT_OBJECT_0)
			{
				return true;
			}
			return false;
		}

		bool CEventLock::UnLock( void )
		{
			LOCK_INVALID_ZERO(m_Handle);
			return ::SetEvent(m_Handle) ? true : false;
		}

		/*BOOL LockEx( unsigned long ulInterval, CMyDelegate<void>* pDelegateFun )
		{
			while(true)
			{
				unsigned long ulSatte = WaitForSingleObject(m_Handle,ulInterval);
				if(ulSatte == WAIT_OBJECT_0)
				{
					return TRUE;
				}
				else if(ulSatte == WAIT_TIMEOUT)
				{
					if(pDelegateFun != NULL)
						pDelegateFun();	
				}
				else
				{
					break;
				}
			};

			return FALSE;
		}*/

	} //_lock_

#endif
