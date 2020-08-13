
#include <libEPoll_Regular_File/FileIO.h>

namespace	_epoll_regular_file_{

	typedef _sys_error_					error_code;

	static int APIWFARead( HFILE hFile, char* szDataBuf, unsigned long ulDataSize, UInt64 uOffset )
	{
		FILE_INVALID(hFile, FILE_ERROR);

		if (lseek(hFile, uOffset, SEEK_SET) == -1) 
		{
			LOG_Print_Error(libEPoll_Regular_File, "APIWFARead() Failed!! error:%d\r\n", error_code::GetLastError());
			return FILE_ERROR;
		}

		int iRet = read(hFile, szDataBuf, ulDataSize);
		if( iRet < 0 )
		{
			return FILE_ERROR;
		}

		return iRet;
	}

	static int APIWFAWrite( HFILE hFile, const char* c_szDataBuf, unsigned long ulDataSize, UInt64 uOffset )
	{
		FILE_INVALID(hFile, FILE_ERROR);

		if (lseek(hFile, uOffset, SEEK_SET) == -1) 
		{
			LOG_Print_Error(libEPoll_Regular_File, "APIWFAWrite() Failed!! error:%d\r\n", error_code::GetLastError());
			return FILE_ERROR;
		}

		int iRet = write(hFile, c_szDataBuf, ulDataSize);
		if( iRet < 0 )
		{
			return FILE_ERROR;
		}

		return iRet;
	}

	static int APIWFAClose( HFILE hFile )
	{
		FILE_INVALID(hFile, FILE_ERROR);
		close( hFile );
		return 1;
	}

	FileIO::FileIO( const io_service::io_service_sptr& IOServer_sptr )
		: _IOService_sptr(IOServer_sptr)
		, _hFile(INVALID_FILE)
	{

	}

	FileIO::~FileIO( void )
	{

	}

	int FileIO::Bind( const HFILE& hFile )
	{
		Attach(hFile);
		return 1;
	}

	int FileIO::Attach( const HFILE& hFile )
	{
		_hFile = hFile;
		return 1;
	}

	int FileIO::async_read( char* c_szBuf,
		UInt32 uSize, const HFNComplete& handle, UInt64 uOffset )
	{
		tagOverlapped* p = _IOService_sptr->Alloc();
		p->_hfnComplete = handle;
		p->_ADR._Handle = _hFile;
		p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_READ;
		p->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		p->_SWSAOverlapp.wsaBuf.buf = c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_Free_Overlapp = function1_handle(&io_service::OverlappedManage_type::Free, _IOService_sptr->GetOverlappedManage().get());//function1_handle(&TcpSocket::FreeOverlapped, this);

		_Lock.Lock();
		if( _IOService_sptr->post_read(_hFile, p) == -1
			&& error_code::GetLastError() != EAGAIN
			&& error_code::GetLastError() != ENOSYS )
		{
			_Lock.UnLock();
			_IOService_sptr->Free(p);
			return -1;
		}

		if( error_code::GetLastError() == EAGAIN || error_code::GetLastError() == ENOSYS )
		{
			_Lock.UnLock();
			tagIOData_INFO	IODataINFO;
			IODataINFO._ibytes_transferred = APIWFARead(_hFile, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( IODataINFO._ibytes_transferred == -1 )
			{
				_IOService_sptr->Free(p);
				return -1;
			}

			_pRecv = p;
			IODataINFO._pOverlapped = p;
			_IOService_sptr->process(&IODataINFO, p);
		}
		else
		{
			_pRecv = p;
			_Lock.UnLock();
		}

		return 1;
	}

	int FileIO::async_read( tagOverlapped* pOverlapped,
		char* pData, UInt32 u32Size, UInt64 uOffset )
	{
		pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
		pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
		return async_read(pOverlapped, uOffset);
	}

	int FileIO::async_read( tagOverlapped* pOverlapped, UInt64 uOffset )
	{
		tagOverlapped::AddRef(pOverlapped);

		pOverlapped->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		if( _IOService_sptr->post_read(_hFile, pOverlapped) == -1
			&& error_code::GetLastError() != EAGAIN
			&& error_code::GetLastError() != ENOSYS )
		{
			//test
			if( _hFile != INVALID_FILE )
			{
				printf("exit failed FileIO::async_read()11 error %s, this:%p, pOverlapped:%p, Handle:%d\r\n",
					error_code(error_code::GetLastError()).message().c_str(), this, pOverlapped, _hFile);
				_exit(0);
			}
			//test 

			if( _hFile == INVALID_FILE )
			{
				pOverlapped->_ADR._Handle = INVALID_HANDLE_VALUE;
				tagOverlapped::SubRef(pOverlapped);
			}
			else
			{
				//test
				perror("FileIO::async_read()0000\r\n");
				_exit(0);
				//test
			}
			return -1;
		}

		if( error_code::GetLastError() == EAGAIN || error_code::GetLastError() == ENOSYS )
		{
			tagIOData_INFO	IODataINFO;
			IODataINFO._ibytes_transferred = APIWFARead(_hFile, pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( IODataINFO._ibytes_transferred == -1 )
			{
				//tagOverlapped::SubRef(pOverlapped);
				return -1;
			}
			
			IODataINFO._pOverlapped = pOverlapped;
			_IOService_sptr->process(&IODataINFO, pOverlapped);
		}

		return 1;
	}

	int FileIO::async_write( const char* c_szBuf,
		UInt32 uSize, const HFNComplete& handle, UInt64 uOffset )
	{
		tagOverlapped* p = _OverlappedManage.Alloc();
		p->_hfnComplete = handle;
		p->_ADR._Handle = _hFile;
		p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WRITE;
		p->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		p->_SWSAOverlapp.wsaBuf.buf = (char*)c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_Free_Overlapp = function1_handle(&FileIO::FreeOverlapped, this);

		if( _IOService_sptr->post_write(_hFile, p) == -1
			&& error_code::GetLastError() != EAGAIN
			&& error_code::GetLastError() != ENOSYS )
		{
			_IOService_sptr->Free(p);
			return -1;
		}

		if( error_code::GetLastError() == EAGAIN || error_code::GetLastError() == ENOSYS )
		{
			tagIOData_INFO	IODataINFO;
			IODataINFO._ibytes_transferred = APIWFAWrite(_hFile, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( IODataINFO._ibytes_transferred == -1 )
			{
				_IOService_sptr->Free(p);
				return -1;
			}

			IODataINFO._pOverlapped = p;
			_IOService_sptr->process(&IODataINFO, p);
		}
		
		return 1;
	}

	int FileIO::async_write( tagOverlapped* pOverlapped, UInt64 uOffset )
	{
		tagOverlapped::AddRef(pOverlapped);

		pOverlapped->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		if( _IOService_sptr->post_write(_hFile, pOverlapped) == -1
			&& error_code::GetLastError() != EAGAIN
			&& error_code::GetLastError() != ENOSYS )
		{
			tagOverlapped::SubRef(pOverlapped);
			return -1;
		}

		if( error_code::GetLastError() == EAGAIN || error_code::GetLastError() == ENOSYS )
		{
			tagIOData_INFO	IODataINFO;
			IODataINFO._ibytes_transferred = APIWFAWrite(_hFile, pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( IODataINFO._ibytes_transferred == -1 )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}
			IODataINFO._pOverlapped = pOverlapped;
			_IOService_sptr->process(&IODataINFO, pOverlapped);
		}

		return 1;
	}

	void FileIO::close( void )
	{
		if( is_open() )
		{
			HFILE h = _hFile;
			_hFile = INVALID_HANDLE_VALUE;
			APIWFAClose(h);

			_Lock.Lock();
			if( _pRecv != NULL )
			{
					tagOverlapped* p = _pRecv;
					_pRecv = NULL;
					_Lock.UnLock();
					_IOService_sptr->CloseOverlapped(p);	
			}
			else
			{
				_Lock.UnLock();
			}
		}
	}

} //_iocp_net_

