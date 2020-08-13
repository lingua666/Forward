
#include <libIOCP_File/CAsynIO.h>
#include <libIOCP_File/io_service.h>

namespace _iocp_file_{

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
		
	}

	io_service& io_service::operator=( const self_type& self )
	{
		_io_handle = self._io_handle;
		return *this;
	}

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

	int io_service::post( const function_handle& f )
	{
		tagOverlapped* p = _OverlappedManage.Alloc();
		p->_Func = f;
		p->_Free_Overlapp = function1_handle(&io_service::FreeOverlapped, this);
		return CAsynIO::PostAsynIO(_io_handle, NULL, &p->_SWSAOverlapp.wsaOverlapped);
	}

	int io_service::post( tagOverlapped* pOverlapped )
	{
		tagOverlapped::AddRef(pOverlapped);
		CAsynIO::PostAsynIO(_io_handle, NULL, &pOverlapped->_SWSAOverlapp.wsaOverlapped );
		return 1;
	}

	int io_service::post( HFILE hFile, const tagIOWSABuf& wBuf, const function_handle& f )
	{
		tagOverlapped* p = _OverlappedManage.Alloc();
		p->_Func = f;
		p->_ADR._Handle = hFile;
		p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
		p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
		p->_Free_Overlapp = function1_handle(&io_service::FreeOverlapped, this);
		CAsynIO::PostAsynIO(_io_handle, NULL, &p->_SWSAOverlapp.wsaOverlapped );
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
					function_handle h = pOver->_Func;
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

	int	io_service::bind( HFILE Handle )
	{
		if( CAsynIO::BindHandle(_io_handle, Handle, 0) == NULL )
		{
			return -1;
		}
		return 1;
	}

	tagOverlapped* io_service::AllocOverlapped( void )
	{
		tagOverlapped* p = _OverlappedManage.Alloc();
		p->_Free_Overlapp = function1_handle(&io_service::FreeOverlapped, this);
		return p;
	}

	void io_service::FreeOverlapped( tagOverlapped* pOverlapped )
	{
		_OverlappedManage.Free(pOverlapped);
	}

} // _iocp_net_

