
#include <libEPoll_Regular_File/FileIO.h>

namespace	_epoll_regular_file_{

	typedef _sys_error_					error_code;

	static int APIWFARead( HFILE hFile, char* szDataBuf, unsigned long ulDataSize, UInt64 uOffset )
	{
		FILE_INVALID(hFile, FILE_ERROR);

		if (lseek(hFile, uOffset, SEEK_SET) == -1) 
		{
			LOG_Print_Error(libEPoll_Regular_File, "APIWFARead(%lld,%lld) lseek Failed!! error:%d\r\n", hFile, uOffset, error_code::GetLastError());
			return FILE_ERROR;
		}

		int iRet = read(hFile, szDataBuf, ulDataSize);
		if( iRet < 0 )
		{
			LOG_Print_Error(libEPoll_Regular_File, "APIWFARead(%lld,%lld) read Failed!! error:%d\r\n", hFile, uOffset, error_code::GetLastError());
			return FILE_ERROR;
		}

		return iRet;
	}

	static int APIWFAWrite( HFILE hFile, const char* c_szDataBuf, unsigned long ulDataSize, UInt64 uOffset )
	{
		FILE_INVALID(hFile, FILE_ERROR);

		if (lseek(hFile, uOffset, SEEK_SET) == -1) 
		{
			LOG_Print_Error(libEPoll_Regular_File, "APIWFAWrite(%lld) lseek Failed!! error:%d\r\n", hFile, error_code::GetLastError());
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
		p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_READ;
		p->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		p->_SWSAOverlapp.wsaBuf.buf = c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_Free_Overlapp = function1_handle(&io_service::OverlappedManage_type::Free, _IOService_sptr->GetOverlappedManage().get());

		_Lock.Lock();
		p->_ADR._Handle = _hFile;
		if( _IOService_sptr->post_read(_hFile, p) == -1 )
		{
			if (error_code::GetLastError() != EAGAIN && error_code::GetLastError() != ENOSYS)
			{
				_Lock.UnLock();
				_IOService_sptr->Free(p);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_read() post_read Failed!! error:%d\r\n", error_code::GetLastError());
				return -1;
			}

			//test
			while (true)
			{
				LOG_Print_Error(libEPoll_Regular_File, "While FileIO::async_read(%d) post_read Failed!! error:%d\r\n", uOffset, error_code::GetLastError());
				Sleep(2000);
			}
			//test

			p->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = APIWFARead(_hFile, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( p->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes == -1 )
			{
				_Lock.UnLock();
				//_IOService_sptr->Free(p);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_read(%d) APIWFARead Failed!! error:%d\r\n", uOffset, error_code::GetLastError());
				return -1;
			}
		}

		_Lock.UnLock();
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

		_Lock.Lock();
		pOverlapped->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		if( _IOService_sptr->post_read(_hFile, pOverlapped) == -1 )
		{
			if (error_code::GetLastError() != EAGAIN && error_code::GetLastError() != ENOSYS)
			{
				_Lock.UnLock();

				if (_hFile != INVALID_FILE)
				{
					LOG_Print_Error(libEPoll_Regular_File, "exit failed FileIO::async_read()0000 error(%d) %s, this:%p, pOverlapped:%p, Handle:%d\r\n",
						error_code::GetLastError(), error_code(error_code::GetLastError()).message().c_str(), this, pOverlapped, _hFile);
					_exit(0);
				}

				if (_hFile == INVALID_FILE)
				{
					pOverlapped->_ADR._Handle = INVALID_HANDLE_VALUE;
					tagOverlapped::SubRef(pOverlapped);
				}
				else
				{
					LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_read()0000\r\n");
					_exit(0);
				}

				return -1;
			}

			//test
			while (true)
			{
				LOG_Print_Error(libEPoll_Regular_File, "While FileIO::async_read(%d)000 post_read Failed!! error:%d\r\n", uOffset, error_code::GetLastError());
				Sleep(2000);
			}
			//test

			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = APIWFARead(_hFile, pOverlapped->_SWSAOverlapp.wsaBuf.buf,
			pOverlapped->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes == -1 )
			{
				_Lock.UnLock();

				tagOverlapped::SubRef(pOverlapped);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_read()0000 APIWFARead Failed!! error:%d\r\n", error_code::GetLastError());
				return -1;
			}

			CASAddAndFetch(&pOverlapped->_ProcRef);
		}
		_Lock.UnLock();

		return 1;
	}

	int FileIO::async_write( const char* c_szBuf,
		UInt32 uSize, const HFNComplete& handle, UInt64 uOffset )
	{
		tagOverlapped* p = _IOService_sptr->Alloc();
		p->_hfnComplete = handle;
		p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WRITE;
		p->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		p->_SWSAOverlapp.wsaBuf.buf = (char*)c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_Free_Overlapp = function1_handle(&io_service::OverlappedManage_type::Free, _IOService_sptr->GetOverlappedManage().get());

		_Lock.Lock();
		p->_ADR._Handle = _hFile;
		if( _IOService_sptr->post_write(_hFile, p) == -1 )
		{
			if (error_code::GetLastError() != EAGAIN && error_code::GetLastError() != ENOSYS)
			{
				_Lock.UnLock();
				_IOService_sptr->Free(p);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_write() post_write Failed!! error:%d\r\n", error_code::GetLastError());
				return -1;
			}

			//test
			while (true)
			{
				LOG_Print_Error(libEPoll_Regular_File, "While FileIO::async_write(%d) post_write Failed!! error:%d\r\n", uOffset, error_code::GetLastError());
				Sleep(2000);
			}
			//test

			p->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = APIWFAWrite(_hFile, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len, uOffset);
			if( p->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes == -1 )
			{
				_Lock.UnLock();
				//_IOService_sptr->Free(p);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_write(%d,%p) APIWFAWrite(%d) Failed!! error:%d\r\n", _hFile, c_szBuf, uSize, error_code::GetLastError());
				return -1;
			}
		}

		_Lock.UnLock();
		
		return 1;
	}

	int FileIO::async_write( tagOverlapped* pOverlapped, UInt64 uOffset )
	{
		tagOverlapped::AddRef(pOverlapped);
		
		_Lock.Lock();
		pOverlapped->_SWSAOverlapp.wsaOverlapped.uOffset = uOffset;
		if( _IOService_sptr->post_write(_hFile, pOverlapped) == -1 )
		{
			if (error_code::GetLastError() != EAGAIN && error_code::GetLastError() != ENOSYS)
			{
				_Lock.UnLock();
				tagOverlapped::SubRef(pOverlapped);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_write()0000 post_write Failed!! error:%d\r\n", error_code::GetLastError());
				return -1;
			}

			//test
			while (true)
			{
				LOG_Print_Error(libEPoll_Regular_File, "While FileIO::async_write(%d)0000 post_write Failed!! error:%d\r\n", uOffset, error_code::GetLastError());
				Sleep(2000);
			}
			//test

			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = APIWFAWrite(_hFile, pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len, uOffset);
			if (pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes == -1)
			{
				_Lock.UnLock();
				tagOverlapped::SubRef(pOverlapped);
				LOG_Print_Error(libEPoll_Regular_File, "FileIO::async_write(%d)0000 APIWFAWrite(%d,%d) Failed!! error:%d\r\n", _hFile, pOverlapped->_SWSAOverlapp.wsaBuf.len, uOffset, error_code::GetLastError());
				return -1;
			}

			CASAddAndFetch(&pOverlapped->_ProcRef);
		}
		
		_Lock.UnLock();
		return 1;
	}

	void FileIO::close( void )
	{
		if( is_open() )
		{
			HFILE h = _hFile;
			_Lock.Lock();
			_hFile = INVALID_HANDLE_VALUE;
			APIWFAClose(h);
			_Lock.UnLock();
		}
	}

	int FileIO::seek_read(Int64 uOffset)
	{
		if (!is_open())
			return -1;

		return lseek(_hFile, uOffset, SEEK_SET) == -1 ? -1 : 1;
	}

	Int64 FileIO::tell_read(void)
	{
		if (!is_open())
			return -1;

		return lseek(_hFile, 0, SEEK_CUR);
	}

} //_iocp_net_

