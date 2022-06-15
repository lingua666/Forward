#ifndef  __CUSTOM_REGULAR_FILE_CAIO_HEAD_DEF__
#define  __CUSTOM_REGULAR_FILE_CAIO_HEAD_DEF__

#include "External.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	#include <sys/syscall.h>      /* for __NR_* definitions */
	#include <linux/aio_abi.h>    /* for AIO types and constants */
	#include <fcntl.h>            /* O_RDWR */
	#define REGULAR_FILE_EVENTS_MAX 128

	namespace _epoll_regular_file_{

		class CAIO
		{
		public:
			CAIO( void );

			~CAIO( void );

			int	Init( void );

			void Release( void );

			int Setup( unsigned int nr_events );

			int Submit( long nr,  struct iocb *cbp[] );

			int Cancel( struct iocb* cbp,  struct io_event *result );

			int GetEvents( struct io_event *events );

			int ReadEventNum( void );

			bool is_open( void ) const
			{
				return _efd != -1 ? true : false;
			}

			const int	GetHandle( void ) const
			{
				return _efd;
			}

		private:
			int	_efd;
			struct timespec _tms;
			aio_context_t _ctx;
		};

		inline CAIO* GetCAIOInstance( void )
		{
			static CAIO* pObj = Singleton<CAIO>::instance();
			return pObj;
		}

	} //_iocp_net_

#endif

#endif