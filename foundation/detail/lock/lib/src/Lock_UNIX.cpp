
#include <libLock/Lock_UNIX.h>

#if defined(PLATFORM_OS_FAMILY_UNIX)

	#include <libCommon/CMyException.h>

	namespace _lock_ {

		CLock_Linux::CLock_Linux( void ) 
			: _isLock(false)
		{ 
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
	#if defined(PTHREAD_MUTEX_RECURSIVE_NP)
			pthread_mutexattr_settype_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	#else
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	#endif
			if (pthread_mutex_init(&m_mutex, &attr))
			{
				pthread_mutexattr_destroy(&attr);
				EXCEPTION_THROW(SystemException,"cannot create mutex");
			}
			pthread_mutexattr_destroy(&attr);
		}

		CLock_Linux::~CLock_Linux( void )	
		{ 
			if(_isLock)
				UnLock();
			pthread_mutex_destroy(&m_mutex);
		}

		bool CLock_Linux::TryLock( void )
		{
			#if PLATFORM_OS != PLATFORM_OS_ANDROID
				const int sleepMillis = 5;
				int		iTryTime = 100;
				do
				{
					int rc = pthread_mutex_trylock(&m_mutex);
					if (rc == 0)
						return true;
					else if (rc != EBUSY)
						return false;

					struct timeval tv;
					tv.tv_sec  = 0;
					tv.tv_usec = sleepMillis * 1000;
					select(0, NULL, NULL, NULL, &tv);
				}while (-- iTryTime);
			#endif
			
			return false;
		}

		void CLock_Linux::Lock( void )
		{ 
			if (pthread_mutex_lock(&m_mutex))
				EXCEPTION_THROW(SystemException,"cannot lock mutex")
			else
			_isLock = true;
		}

		void CLock_Linux::UnLock( void ) 
		{
			if (pthread_mutex_unlock(&m_mutex))
				EXCEPTION_THROW(SystemException,"cannot unlock mutex")
			else
			_isLock = false;
		}

		bool CLock_Linux::isLock( void )
		{
			return _isLock;
		}

	} //_lock_

#endif