
#include <libEPoll_Net/CAsynIO.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{
	
		//�����첽IO
		HANDLE CAsynIO::CreateAsynIO( void )
		{
			HANDLE fd = epoll_create( get_system_resource("/proc/sys/fs/file-max") );
			if (fd != -1)
				::fcntl(fd, F_SETFD, FD_CLOEXEC);

			return fd;
		}

		//�رն˿�
		void	CAsynIO::CloseAsynIO( HANDLE* hCompletionPort )
		{
			// �ͷ���ɶ˿ھ��
			if(*hCompletionPort != NULL)
			{
				//֪ͨ��ɶ˿��˳�
				//PostAsynIO(*hCompletionPort, NULL, NULL);
				::close(*hCompletionPort);
				*hCompletionPort = NULL;
			}
		}

		//Ͷ����Ϣ
		/*int	CAsynIO::PostAsynIO( HANDLE hCompletionPort, void* pParam, OVERLAPPED* lpOverlapped )
		{
			return PostQueuedCompletionStatus(hCompletionPort,
										0, (DWORD)pParam, lpOverlapped);
		}*/

		int	CAsynIO::PostIO( HANDLE hCompletionPort, HANDLE Handle, int iType, int iValue, UInt64 wParam )
		{
			struct epoll_event ev = { 0, { 0 } };
			ev.events = iValue;
			ev.data.fd = Handle;
			ev.data.ptr = (void*)wParam;
			return epoll_ctl(hCompletionPort, iType, Handle, &ev) < 0 ? -1 : 1;
		}

		int CAsynIO::GetQueuedCompletionStatus(  HANDLE CompletionPort,
										struct epoll_event* pEvents, UInt32 uSize,
										DWORD dwMilliseconds )
		{
			return epoll_wait( CompletionPort, pEvents, uSize, dwMilliseconds );
		}

		//�󶨾��
		int CAsynIO::BindHandle( HANDLE hCompletionPort, HANDLE Handle )
		{
			struct epoll_event ev = { 0, { 0 } };
			//ev.events = EPOLLIN | EPOLLERR | EPOLLET;
			ev.events = EPOLLERR | EPOLLET;
			ev.data.fd = Handle;
			ev.data.ptr = 0;
			return epoll_ctl(hCompletionPort, EPOLL_CTL_ADD, Handle, &ev) < 0 ? -1 : 1;
		}

		int CAsynIO::DettachHandle( HANDLE hCompletionPort, HANDLE Handle )
		{
			struct epoll_event ev = { 0, { 0 } };
			ev.data.ptr = NULL;
			ev.data.fd = 0;
			if( epoll_ctl(hCompletionPort, EPOLL_CTL_MOD, Handle, &ev) < 0 )
			{
				printf("CAsynIO::DettachHandle epoll_ctl(%d) failed!!\r\n", errno);
				return -1;
			};
			
			return epoll_ctl(hCompletionPort, EPOLL_CTL_DEL, Handle, &ev) < 0 ? -1 : 1;
		}

		int CAsynIO::PostRead( HANDLE hCompletionPort, HANDLE Handle, UInt64 wParam )
		{
			struct epoll_event ev = { 0, { 0 } };
			ev.events = EPOLLONESHOT | EPOLLIN | EPOLLERR | EPOLLET | EPOLLHUP; //ע���¼�ֻ����һ��
			ev.data.fd = Handle;
			ev.data.ptr = (void*)wParam;
			return epoll_ctl(hCompletionPort, EPOLL_CTL_MOD, Handle, &ev) < 0 ? -1 : 1;
		}

		int CAsynIO::PostWrite( HANDLE hCompletionPort, HANDLE Handle, UInt64 wParam )
		{
			struct epoll_event ev = { 0, { 0 } };
			ev.events = EPOLLOUT | EPOLLERR | EPOLLET;
			ev.data.fd = Handle;
			ev.data.ptr = (void*)wParam;
			return epoll_ctl(hCompletionPort, EPOLL_CTL_MOD, Handle, &ev) < 0 ? -1 : 1;
		}

	} //_iocp_net_

#endif
