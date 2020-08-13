
#ifndef __LOCK_UNIX_20150731180430_1438337070_H__
#define __LOCK_UNIX_20150731180430_1438337070_H__

#include <libCommon/os/Platform.h>

	#if defined(PLATFORM_OS_FAMILY_UNIX)

		#include <pthread.h>
		#include <errno.h>

		namespace _lock_ {

		#define LOCK_INVALID_ZERO(Handle)    \
			if(Handle == NULL) { return 0; }

		#define LOCK_INVALID_NONE(Handle)    \
			if(Handle == NULL) { return ; }

			class  CLock_Linux
			{
			public:
				CLock_Linux( void );

				~CLock_Linux( void );

				bool TryLock( void );

				void Lock( void );

				void UnLock( void );

				bool isLock( void );

			private:
				pthread_mutex_t m_mutex;
				bool    _isLock;
			};
		}

	#endif

#endif // __CUSTOM_LOCK_UNIX_HEAD_DEF_