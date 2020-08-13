#ifndef __CUSTOM_LOCK_WIN32_HEAD_DEF_
#define __CUSTOM_LOCK_WIN32_HEAD_DEF_

	#include <libCommon/os/Platform.h>

	#if defined(PLATFORM_OS_FAMILY_WINDOWS)

		#include <libCommon/os/windowsi.h>

		namespace _lock_ {

		#define LOCK_INVALID_ZERO(Handle)    \
			if(Handle == NULL) { return 0; }

		#define LOCK_INVALID_NONE(Handle)    \
			if(Handle == NULL) { return ; }

			class  CLock_Win32
			{
			public:
				CLock_Win32( void );

				~CLock_Win32( void );

				bool TryLock( void );

				void Lock( void );

				void UnLock( void );

				bool isLock( void );

			private:
				CRITICAL_SECTION	m_cs;
				bool    _isLock;
			};

			class CEventLock
			{
			public:
				CEventLock( void );

				~CEventLock( void );

				bool Lock( void );

				bool UnLock( void );

			private:
				HANDLE m_Handle;
			};

		} //_lock_

	#endif

#endif // __CUSTOM_LOCK_WIN32_HEAD_DEF_