#ifndef  __CUSTOM_REGULAR_FILE_CASYNIO_HEAD_DEF__
#define  __CUSTOM_REGULAR_FILE_CASYNIO_HEAD_DEF__

#include "External.h"
#if defined( PLATFORM_OS_FAMILY_UNIX )

	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/ioctl.h> 
	#include <sys/epoll.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>

	namespace _epoll_regular_file_{

		struct CAsynIO
		{
			//创建异步IO
			static HANDLE	CreateAsynIO( void );

			//关闭端口
			static void	CloseAsynIO( HANDLE* hCompletionPort);

			static int	PostIO( HANDLE hCompletionPort, HANDLE Handle, int iType, int iValue, UInt64 wParam );

			static int GetQueuedCompletionStatus(  HANDLE CompletionPort,
										struct epoll_event* pEvents, UInt32 uSize,
										DWORD dwMilliseconds = INFINITE );

			//绑定句柄
			static int BindHandle( HANDLE hCompletionPort, HANDLE Handle );

			static int DettachHandle( HANDLE hCompletionPort, HANDLE Handle );

			static int PostRead( HANDLE hCompletionPort, HANDLE Handle, UInt64 wParam );

			static int PostWrite( HANDLE hCompletionPort, HANDLE Handle, UInt64 wParam );
		};

	} //_iocp_net_

#endif

#endif