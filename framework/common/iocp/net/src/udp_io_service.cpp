
#include <libIOCP_Net/CAsynIO.h>
#include <libIOCP_Net/udp_io_service.h>
#include <libIOCP_Net/UdpOverlappedManage.hpp>

namespace _iocp_net_{

	namespace _udp_{

		io_service::io_service( void )
			: _is_run(false)
			, _io_handle(NULL)
		{

		}

		io_service::io_service( const self_type& self )
			: _io_handle(self._io_handle)
			, _is_run(self._is_run)
		{

		}

		io_service::~io_service( void )
		{
			//stop();
		}

		io_service& io_service::operator=( const self_type& self )
		{
			_io_handle = self._io_handle;
			return *this;
		}

		int io_service::Init(void)
		{
			return 1;
		}

		void io_service::Release(void)
		{}

		int io_service::open( void )
		{
			_io_handle = CAsynIO::CreateAsynIO();
			if( _io_handle == NULL )
			{
				printf("io_service Create Error!\r\n");
				return -1;
			}
			return 1;
		}

		int io_service::close( void )
		{
			if( _io_handle == NULL )
				return -1;

			CAsynIO::CloseAsynIO(&_io_handle);
			_io_handle = NULL;
			return 1;
		}

		void io_service::run( void )
		{
			BOOL			bResult = FALSE;
			DWORD NumberOfBytesTransferred = 0;
			PULONG_PTR	lpCompletionKey = NULL;
			LPOVERLAPPED lpOverlapped = NULL;
			tagIOData_INFO	IODataINFO = {0};

			_is_run = true;
			while( _is_run )
			{
				bResult = CAsynIO::GetQueuedCompletionStatus(_io_handle,
					&NumberOfBytesTransferred,
					(PULONG_PTR)&lpCompletionKey,
					(LPOVERLAPPED *)&lpOverlapped);
				if( lpOverlapped != NULL )
				{
					tagOverlapped*	pOver = reinterpret_cast<tagOverlapped*>( lpOverlapped );
					IODataINFO._ibytes_transferred = NumberOfBytesTransferred;
					IODataINFO._pOverlapped = pOver;

					if( pOver->_Func )
					{
						pOver->_Func(&IODataINFO);
					}

					if( tagOverlapped::SubRef(pOver) <= 0 && pOver->_Free_Overlapp )
					{//ÊÍ·Å
						HFNIOData h = pOver->_Func;
						pOver->_Free_Overlapp(pOver);
					}
				}
			}
		}

		int io_service::stop(void)
		{
			if( _is_run )
				_is_run = false;
			return 1;
		}

		int	io_service::bind( HSOCKET sock )
		{
			if( CAsynIO::BindHandle(_io_handle, sock, 0) == NULL )
			{
				return -1;
			}
			return 1;
		}

	} //namespace _udp_

} // _iocp_net_

