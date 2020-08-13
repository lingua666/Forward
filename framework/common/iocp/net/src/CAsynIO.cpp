
#include <libIOCP_Net/CAsynIO.h>

namespace _iocp_net_{
	
	//创建异步IO
	HANDLE CAsynIO::CreateAsynIO( void )
	{
		HANDLE hHandle = CreateIoCompletionPort( INVALID_HANDLE_VALUE
			, NULL
			, NULL
			, 0 );
		if(hHandle == NULL)
		{
			return NULL;
		}
		return hHandle;
	}

	//关闭端口
	void	CAsynIO::CloseAsynIO( HANDLE* hCompletionPort)
	{
		// 释放完成端口句柄
		if(*hCompletionPort != NULL)
		{
			//通知完成端口退出
			PostAsynIO(*hCompletionPort, NULL, NULL);
			Sleep(100);
			CloseHandle(*hCompletionPort);
			*hCompletionPort = NULL;
		}
	}

	//投递信息
	int	CAsynIO::PostAsynIO( HANDLE hCompletionPort, void* pParam, OVERLAPPED* lpOverlapped )
	{
		return PostQueuedCompletionStatus(hCompletionPort,
									0, (DWORD)pParam, lpOverlapped);
	}

	int CAsynIO::GetQueuedCompletionStatus(  HANDLE CompletionPort,
									LPDWORD lpNumberOfBytesTransferred,
									PULONG_PTR lpCompletionKey,
									LPOVERLAPPED *lpOverlapped,
									DWORD dwMilliseconds )
	{
		return ::GetQueuedCompletionStatus( CompletionPort
			, lpNumberOfBytesTransferred
			, lpCompletionKey
			, lpOverlapped
			, dwMilliseconds );
	}

	//绑定句柄
	HANDLE CAsynIO::BindHandle( HANDLE hCompletionPort, HANDLE handle, DWORD wParam )
	{
		return CreateIoCompletionPort(handle, hCompletionPort, wParam, 0 );
	}

} //_iocp_net_
