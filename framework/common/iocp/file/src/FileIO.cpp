
#include <libIOCP_File/FileIO.h>

namespace	_iocp_file_{

	FileIO::FileIO( const io_service::io_service_sptr& IOServer_sptr )
					: _IOService_sptr(IOServer_sptr)
					, _hFile(0)
	{
		
	}

	FileIO::~FileIO( void )
	{
		
	}

	int FileIO::Bind( const HFILE& Handle )
	{
		if( _IOService_sptr->bind(Handle) == 1 )
		{
			_hFile = Handle;
			return 1;
		}
		
		return -1;
	}

	int FileIO::Attach( const HFILE& Handle )
	{
		_hFile = Handle;
		return 1;
	}

	int FileIO::async_read( char* c_szBuf,
					UInt32 uSize,
					const function_handle& hFn,
					UInt64 uOffset )
	{
		tagOverlapped* p = _IOService_sptr->AllocOverlapped();
		p->_Func = hFn;
		p->_ADR._Handle = _hFile;
		p->_SWSAOverlapp.wsaBuf.buf = c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_SWSAOverlapp.wsaOverlapped.Offset = uOffset;
		p->_SWSAOverlapp.wsaOverlapped.OffsetHigh = uOffset >> 32;

		if ( !ReadFile(_hFile, p->_SWSAOverlapp.wsaBuf.buf,
			p->_SWSAOverlapp.wsaBuf.len, NULL,
			&p->_SWSAOverlapp.wsaOverlapped) && GetLastError() != 997 )
		{
			_IOService_sptr->FreeOverlapped(p);
			return -1;
		}

		return 1;
	}

	int FileIO::async_read( tagOverlapped* pOverlapped,
					char* pData, UInt32 u32Size,
					UInt64 uOffset )
	{
		pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
		pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
		return async_read(pOverlapped, uOffset);
	}

	int FileIO::async_read( tagOverlapped* pOverlapped, UInt64 uOffset )
	{
		tagOverlapped::AddRef(pOverlapped);

		pOverlapped->_SWSAOverlapp.wsaOverlapped.Offset = uOffset;
		pOverlapped->_SWSAOverlapp.wsaOverlapped.OffsetHigh = uOffset >> 32;
		if ( !ReadFile(_hFile,pOverlapped->_SWSAOverlapp.wsaBuf.buf,
			pOverlapped->_SWSAOverlapp.wsaBuf.len, NULL,
			&pOverlapped->_SWSAOverlapp.wsaOverlapped) && GetLastError() != 997 )
		{
			tagOverlapped::SubRef(pOverlapped);
			return -1;
		}

		return 1;
	}

	int FileIO::async_write( const char* c_szBuf,
						UInt32 uSize,
						const function_handle& hFn,
						UInt64 uOffset )
	{
		tagOverlapped* p = _IOService_sptr->AllocOverlapped();
		p->_Func = hFn;
		p->_ADR._Handle = _hFile;

		p->_SWSAOverlapp.wsaBuf.buf = (char*)c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_SWSAOverlapp.wsaOverlapped.Offset = uOffset;
		p->_SWSAOverlapp.wsaOverlapped.OffsetHigh = uOffset >> 32;

		if( !WriteFile(_hFile,p->_SWSAOverlapp.wsaBuf.buf,
			p->_SWSAOverlapp.wsaBuf.len, NULL,
			&p->_SWSAOverlapp.wsaOverlapped) && GetLastError() != 997 )
		{
			_IOService_sptr->FreeOverlapped(p);
			return -1;
		}

		return 1;
	}

	int FileIO::async_write( tagOverlapped* pOverlapped, UInt64 uOffset )
	{
		tagOverlapped::AddRef(pOverlapped);
		pOverlapped->_SWSAOverlapp.wsaOverlapped.Offset = uOffset;
		pOverlapped->_SWSAOverlapp.wsaOverlapped.OffsetHigh = uOffset >> 32;
		if( !WriteFile(_hFile,pOverlapped->_SWSAOverlapp.wsaBuf.buf,
			pOverlapped->_SWSAOverlapp.wsaBuf.len, NULL,
			&pOverlapped->_SWSAOverlapp.wsaOverlapped) && GetLastError() != 997 )
		{
			tagOverlapped::SubRef(pOverlapped);
			return -1;
		}

		return 1;
	}

	void FileIO::close( void )
	{
		if( is_open() )
		{
			//CloseHandle(_hFile);
			_hFile = 0;
		}
	}

	int FileIO::seek_read(Int64 uOffset)
	{
		if (!is_open())
			return -1;

		return 1;
		/*LARGE_INTEGER newPos = {0};
		//newPos.LowPart = uOffset;
		newPos.QuadPart = uOffset;
		//z 将file pointer移动到新的位置 
		int iRet = SetFilePointerEx(_hFile, newPos, NULL, FILE_BEGIN) ? 1 : GetLastError() * (-1);
		return iRet;*/
	}

	Int64 FileIO::tell_read(void)
	{
		if (!is_open())
			return -1;

		return 1;
		/*LARGE_INTEGER newPos;
		newPos.QuadPart = 0;
		//z 将file pointer移动到新的位置 
		SetFilePointerEx(_hFile, newPos, &newPos, SEEK_CUR);
		return newPos.QuadPart;*/
	}

} //_iocp_net_

