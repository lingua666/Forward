#ifndef  __CUSTOM_CASYNIO_HEAD_DEF__
#define  __CUSTOM_CASYNIO_HEAD_DEF__

#include "CMyInitSock.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{

		struct CAsynIO
		{
			//创建异步IO
			static HANDLE	CreateAsynIO( void );

			//关闭端口
			static void	CloseAsynIO( HANDLE* hCompletionPort);

			//投递信息
			//static int	PostAsynIO( HANDLE hCompletionPort, void* pParam, OVERLAPPED* lpOverlapped );\

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