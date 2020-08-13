
#include <libEPoll_Regular_File/CAIO.h>
#include <sys/ioctl.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

namespace _epoll_regular_file_{

	const int g_regular_file_events_20170630 = 128;

	//使用O_DIRECT，则使用DIRECT I/O，这样会减少内存拷贝次数，
	//cpu使用率也会减少。但是这样做就需要由用户层来做内存block对齐，并且其内存大小也必须是block整数倍
	#define CHUNK_ALIGNMENT       512    // align to 512-byte boundary
	#define CHUNK_ALIGNMENT_MASK  (~(CHUNK_ALIGNMENT - 1))

	inline UInt64 align_size( UInt64 unaligned )
	{
		return((unaligned + CHUNK_ALIGNMENT - 1 ) & CHUNK_ALIGNMENT_MASK);
	}

	inline void *align_buf( char* unaligned )
	{
		return((void *)((intptr_t)(unaligned + CHUNK_ALIGNMENT - 1) & CHUNK_ALIGNMENT_MASK));
	}

	inline bool buf_aligned( char* *ptr )
	{
		return(((intptr_t)(ptr) & (~CHUNK_ALIGNMENT_MASK)) == 0);
	}


	inline int io_setup( unsigned int nr, aio_context_t *ctxp )
	{
		//return syscall(__NR_io_setup, nr, ctxp);
		return syscall(SYS_io_setup, nr, ctxp);
	}

	inline int io_submit( aio_context_t ctx, long nr,  struct iocb **iocbpp )
	{
		//return syscall(__NR_io_submit, ctx, nr, iocbpp);
		return syscall(SYS_io_submit, ctx, nr, iocbpp);
	}

	inline int io_getevents( aio_context_t ctx, long min_nr, long max_nr,
						struct io_event *events, struct timespec *timeout )
	{
		//return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);
		return syscall(SYS_io_getevents, ctx, min_nr, max_nr, events, timeout);
	}

	inline int io_destroy( aio_context_t ctx )
	{
		//return syscall(__NR_io_destroy, ctx);
		return syscall(SYS_io_destroy, ctx);
	}

	inline int eventfd( unsigned int initval, int flags )
	{
		//return syscall(__NR_eventfd, initval, flags);
		return syscall(SYS_eventfd, initval, flags);
	}

	inline int io_cancel( aio_context_t ctx, struct iocb* cbp,  struct io_event *result )
	{
		//return syscall(__NR_io_cancel, ctx, cbp, result);
		return syscall(SYS_io_cancel, ctx, cbp, result);
	}

	CAIO::CAIO( void )
		: _efd( -1 )
	{
		_tms.tv_sec = 0;
		_tms.tv_nsec = 0;
		_ctx = 0;
	}

	CAIO::~CAIO( void )
	{
		Release();
	}

	int	CAIO::Init( void )
	{
		if( is_open() )
			return -1;

		_efd = syscall(SYS_eventfd, 0);//eventfd(0, O_NONBLOCK | O_CLOEXEC);
		if (_efd == -1)
		{
			perror("eventfd2");
			return -1;
		}
		
		int n = 1;
		if( ioctl(_efd, FIONBIO, &n) == -1 )
		{
			perror("ioctl(eventfd, FIONBIO) failed");
			return -1;
        }
		
		if ( Setup(g_regular_file_events_20170630) == -1 )
		{
			perror("io_setup");
			return -1;
		}

		return 1;
	}

	void CAIO::Release( void )
	{
		if( is_open() )
		{
			io_destroy(_efd);
			_efd = -1;
		}
	}

	int CAIO::Setup( unsigned int nr_events )
	{
		if( !is_open() )
			return -1;

		return io_setup(nr_events, &_ctx) == 0 ? 1 : -1;
	}

	int CAIO::Submit( long nr,  struct iocb *cbp[] )
	{
		if( !is_open() )
			return -1;

		return io_submit(_ctx, nr, cbp) == nr ? 1: -1;
	}

	int CAIO::Cancel( struct iocb* cbp,  struct io_event *result )
	{
		if( !is_open() )
			return -1;

		return io_cancel(_ctx, cbp, result) == 0 ? 1 : -1;
	}

	int CAIO::GetEvents( struct io_event *events )
	{
		if( !is_open() )
			return -1;

		return io_getevents(_ctx, 1, g_regular_file_events_20170630, events, &_tms);
	}

	int CAIO::ReadEventNum( void )
	{
		if( !is_open() )
			return -1;

		UInt64 uReady = 0;
		int iNum = read(_efd, &uReady, sizeof(UInt64));
		if( iNum != 8 )
		{
			if( iNum == -1 )
			{
				if (errno == EAGAIN)
				{
					return -1;
				}

				printf("CAIO::ReadEventNum failed\r\n");
				return -1;
			}

			printf("CAIO::ReadEventNum returned only %d bytes\r\n", iNum);
			return -1;
		}

		return uReady;
	}

} //_epoll_regular_file_

#endif