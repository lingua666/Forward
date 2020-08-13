#ifndef  __CUSTOM_CASYNIO_HEAD_DEF__
#define  __CUSTOM_CASYNIO_HEAD_DEF__

#include "CMyInitSock.h"

namespace _iocp_net_{
	
	struct CAsynIO
	{
		//创建异步IO
		static HANDLE	CreateAsynIO( void );

		//关闭端口
		static void	CloseAsynIO( HANDLE* hCompletionPort);

		//投递信息
		static int	PostAsynIO( HANDLE hCompletionPort, void* pParam, OVERLAPPED* lpOverlapped );\

		static int GetQueuedCompletionStatus(  HANDLE CompletionPort,
							__out LPDWORD lpNumberOfBytesTransferred,
							__out PULONG_PTR lpCompletionKey,
							__out LPOVERLAPPED *lpOverlapped,
							__in  DWORD dwMilliseconds = INFINITE );

		//绑定句柄
		static HANDLE BindHandle( HANDLE hCompletionPort, HANDLE handle, DWORD wParam );
	};

} //_iocp_net_

#endif