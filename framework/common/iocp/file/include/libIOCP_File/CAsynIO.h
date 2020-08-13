#ifndef  __CUSTOM_IOCPFILE_CASYNIO_HEAD_DEF__
#define  __CUSTOM_IOCPFILE_CASYNIO_HEAD_DEF__

#include <stdio.h>
#include <stdlib.h>
#include <libCommon/os/winsock2i.h>

namespace _iocp_file_{
	
	struct CAsynIO
	{
		//�����첽IO
		static HANDLE	CreateAsynIO( void );

		//�رն˿�
		static void	CloseAsynIO( HANDLE* hCompletionPort);

		//Ͷ����Ϣ
		static int	PostAsynIO( HANDLE hCompletionPort, void* pParam, OVERLAPPED* lpOverlapped );\

		static int GetQueuedCompletionStatus(  HANDLE CompletionPort,
							__out LPDWORD lpNumberOfBytesTransferred,
							__out PULONG_PTR lpCompletionKey,
							__out LPOVERLAPPED *lpOverlapped,
							__in  DWORD dwMilliseconds = INFINITE );

		//�󶨾��
		static HANDLE BindHandle( HANDLE hCompletionPort, HANDLE handle, DWORD wParam );
	};

} //_iocp_net_

#endif