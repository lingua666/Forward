
#include <libEPoll_File/FileIO.h>

namespace	_epoll_non_regular_file_{

	typedef _sys_error_					error_code;

	static int APIWFARecv( HFILE hFile, char* szDataBuf, unsigned long ulDataSize )
	{
		FILE_INVALID(hFile, FILE_ERROR);

		int iRet = read(hFile, szDataBuf, ulDataSize);
		if( iRet < 0 )
		{
			return FILE_ERROR;
		}

		return iRet;
	}

	static int APIWFASend( HFILE hFile, const char* c_szDataBuf, unsigned long ulDataSize )
	{
		FILE_INVALID(hFile, FILE_ERROR);

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
		if( hFile == INVALID_FILE )
			return -1;

		if( _IOService_sptr->bind((HANDLE)hFile) == 1 )
		{
			_hFile = hFile;
			return 1;
		}
		
		return -1;
	}

	int FileIO::Attach( const HFILE& hFile )
	{
		_hFile = hFile;
		return 1;
	}

	int FileIO::async_read( char* c_szBuf,
		UInt32 uSize, const HFNComplete& handle )
	{		
		tagOverlapped* p = _IOService_sptr->Alloc();
		p->_hfnPerform = function20_bind_Ex(APIWFARecv);
		p->_hfnComplete = handle;
		p->_ADR._Handle = _hFile;
		p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_READ;
		p->_SWSAOverlapp.wsaBuf.buf = c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_Free_Overlapp = function1_handle(&io_service::OverlappedManage_type::Free, _IOService_sptr->GetOverlappedManage().get());//function1_handle(&TcpSocket::FreeOverlapped, this);

		//增加1,统一close后删除
		tagOverlapped::AddRef(p);

		_Lock.Lock();
		int iRet = APIWFARecv(_hFile, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len);
		if( iRet >= 0 )
		{
			_pRecv = p;
			_Lock.UnLock();
				
			tagOverlapped::AddRef(p);
			CASAddAndFetch(&p->_ProcRef);
			p->_RealSize = iRet;
			_IOService_sptr->post(p, 0);
		}
		else if( errno == EAGAIN )
		{
			if( _IOService_sptr->post_read(_hFile, p) == -1 )
			{
				_Lock.UnLock();
				_IOService_sptr->Free(p);
				return -1;
			}
			_pRecv = p;
			_Lock.UnLock();
		}
		else
		{
			_Lock.UnLock();
			_IOService_sptr->Free(p);
			return -1;
		}

		return 1;
	}

	int FileIO::async_read( tagOverlapped* pOverlapped,
		char* pData, UInt32 u32Size )
	{
		pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
		pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
		return async_read(pOverlapped);
	}

	int FileIO::async_read( tagOverlapped* pOverlapped )
	{
		pOverlapped->_RealSize = -1;
		int iRet = APIWFARecv(_hFile, pOverlapped->_SWSAOverlapp.wsaBuf.buf, pOverlapped->_SWSAOverlapp.wsaBuf.len);
		if( iRet >= 0 )
		{
			tagOverlapped::AddRef(pOverlapped);
			CASAddAndFetch(&pOverlapped->_ProcRef);
			pOverlapped->_RealSize = iRet;							
			_IOService_sptr->post(pOverlapped, 0);
		}
		else if( errno == EAGAIN )
		{
			if( _IOService_sptr->post_read(_hFile, pOverlapped) == -1 )
			{
				//tagOverlapped::SubRef(pOverlapped);
				return -1;
			}
		}
		else
		{
			//tagOverlapped::SubRef(pOverlapped);
			return -1;
		}
		
		return 1;
	}

	int FileIO::async_write( const char* c_szBuf,
		UInt32 uSize, const HFNComplete& handle )
	{		
		tagOverlapped* p = _OverlappedManage.Alloc();
		p->_hfnPerform = function20_bind_Ex(APIWFASend);
		p->_hfnComplete = handle;
		p->_ADR._Handle = _hFile;
		p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WRITE;
		p->_SWSAOverlapp.wsaBuf.buf = (char*)c_szBuf;
		p->_SWSAOverlapp.wsaBuf.len = uSize;
		p->_Free_Overlapp = function1_handle(&FileIO::FreeOverlapped, this);

		int iRet = APIWFASend(_hFile, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len);
		if( iRet >= 0 )
		{
			p->_RealSize = iRet;		
			tagIOData_INFO	IODataINFO = {0};
			IODataINFO._pOverlapped = p;
			_IOService_sptr->process(&IODataINFO, p);
			//_IOService_sptr->post(p, 0);
		}
		else if( errno == EAGAIN )
		{			
			p->_RealSize = -1;
			if( _IOService_sptr->post_write(_hFile, p) == -1 )
			{
				_OverlappedManage.Free(p);
				return -1;
			}
		}
		else
		{
			_OverlappedManage.Free(p);
			return -1;
		}

		return 1;
	}

	int FileIO::async_write( tagOverlapped* pOverlapped )
	{			
		tagOverlapped::AddRef(pOverlapped);
		pOverlapped->_RealSize = -1;

		int iRet = APIWFASend(_hFile, pOverlapped->_SWSAOverlapp.wsaBuf.buf, pOverlapped->_SWSAOverlapp.wsaBuf.len);
		if( iRet >= 0 )
		{
			pOverlapped->_RealSize = iRet;
		
			tagIOData_INFO	IODataINFO = {0};
			IODataINFO._pOverlapped = pOverlapped;
			_IOService_sptr->process(&IODataINFO, pOverlapped);
			//_IOService_sptr->post(pOverlapped, 0);
		}
		else if( errno == EAGAIN )
		{			
			pOverlapped->_RealSize = -1;
			if( _IOService_sptr->post_write(_hFile, pOverlapped) == -1 )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}
		}
		else
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
			HFILE h = _hFile;
			_hFile = INVALID_HANDLE_VALUE;
			_IOService_sptr->post_delete(h);
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

