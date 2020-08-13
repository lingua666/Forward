
#include <libIOCP_Net/CAsynIO.h>

namespace _iocp_net_{
	
	//�����첽IO
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

	//�رն˿�
	void	CAsynIO::CloseAsynIO( HANDLE* hCompletionPort)
	{
		// �ͷ���ɶ˿ھ��
		if(*hCompletionPort != NULL)
		{
			//֪ͨ��ɶ˿��˳�
			PostAsynIO(*hCompletionPort, NULL, NULL);
			Sleep(100);
			CloseHandle(*hCompletionPort);
			*hCompletionPort = NULL;
		}
	}

	//Ͷ����Ϣ
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

	//�󶨾��
	HANDLE CAsynIO::BindHandle( HANDLE hCompletionPort, HANDLE handle, DWORD wParam )
	{
		return CreateIoCompletionPort(handle, hCompletionPort, wParam, 0 );
	}

} //_iocp_net_
