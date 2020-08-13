
#include <libFrameWork_AsyncFiles/File_impl.h>

namespace	_async_{

	namespace	_files_{

		File_impl::File_impl( const io_service::io_service_sptr& IOService )
			: _Handle( INVALID_HANDLE_VALUE )
			, _FileIO( IOService )
			, _Pool( ASYNC_FILES_POOL_SIZE + sizeof(tagFileBuf) )
			, _isWrite( false )
			, _uROffset( 0 )
			, _uWOffset( 0 )
		{
			
		}

		File_impl::~File_impl( void )
		{
			Release();
			if( _hDestroy )
				_hDestroy();
		}

		void File_impl::Init( void )
		{

		}

		void File_impl::Release( void )
		{
			Close();
		}

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		HANDLE	File_impl::Open( const char* szPath, int iMode )
		{
			HANDLE Handle = NULL;
			switch(iMode)
			{
			case enMode_Read:
				Handle = CreateFile(szPath,
					GENERIC_READ, FILE_SHARE_READ, 
					NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 
					NULL);  
				break;
			case enMode_Write:
				Handle = CreateFile(szPath,
					GENERIC_WRITE, 0, 
					NULL, CREATE_ALWAYS,
					FILE_FLAG_OVERLAPPED, NULL);
				break;
			case enMode_ReadWrite:
				Handle = CreateFile(szPath,
					GENERIC_WRITE | GENERIC_READ,
					0, NULL, CREATE_ALWAYS/*OPEN_EXISTING*/,
					FILE_FLAG_OVERLAPPED, NULL);
				break;
			}

			if( Handle == INVALID_HANDLE_VALUE )
				return INVALID_HANDLE_VALUE;

			if( _FileIO.Bind(Handle) == -1 )
			{//关联失败
				return INVALID_HANDLE_VALUE;
			}

			_Handle = Handle;
			return Handle;
		}
#elif defined( PLATFORM_OS_FAMILY_UNIX )
		HANDLE	File_impl::Open( const char* szPath, int iMode )
		{
			HANDLE Handle = NULL;
			switch(iMode)
			{
			case enMode_Read:
				Handle = open(szPath, O_RDONLY, 0644);  
				break;
			case enMode_Write:
				Handle = open(szPath, O_WRONLY | O_CREAT, 0644);
				break;
			case enMode_ReadWrite:
				Handle = open(szPath, O_RDWR | O_CREAT, 0644);
				break;
			}

			if( Handle == INVALID_HANDLE_VALUE )
				return INVALID_HANDLE_VALUE;

			if( _FileIO.Bind(Handle) == -1 )
			{//关联失败
				return INVALID_HANDLE_VALUE;
			}

			_Handle = Handle;
			return Handle;
		}
#endif

		void File_impl::Close( void )
		{
			HANDLE h = _Handle;
			_Lock.Lock();
			if( is_open() )
			{
				_Handle = INVALID_HANDLE_VALUE;
				_isWrite = false;
				_uROffset = 0;
				_uWOffset = 0;
				_Lock.UnLock();
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
				CloseHandle(h);
#elif defined( PLATFORM_OS_FAMILY_UNIX )
				close(h);
#endif
				_FileIO.close();
				_List.clear();
			}
			else
			{
				_Lock.UnLock();
			}
		}

		void File_impl::SetCallBack( const HFNNotify& hRead,
			const HFNNotify& hWrite,
			const HFNDestroy& hDestroy )
		{
			_hRead = hRead;
			_hWrite = hWrite;
			_hDestroy = hDestroy;
		}

		int File_impl::Write( const char* szData, UInt32 uSize, Int64 Offset )
		{
			if( !is_open() )
				return -1;

			FileBuf_sptr Buf(reinterpret_cast<tagFileBuf*>(_Pool.Alloc()),
				function20_bind_Ex(&MemPool_type::Free, &_Pool));

			Buf->uSize = uSize;
			memcpy(Buf->Data, szData, uSize);

			_Lock.Lock();
			if( !_isWrite )
			{
				_isWrite = true;
				_Lock.UnLock();

				Offset = Offset == -1 ? _uWOffset : Offset;
				if( _FileIO.async_write(Buf->Data, Buf->uSize,
					function20_bind(&File_impl::HandleWrite,
					shared_from_this(), Buf, _function_::_1),
					Offset) == -1 )
				{
					Close();
				}
			}
			else
			{
				Buf->iOffset = Offset;
				_List.push_back(Buf);
				_Lock.UnLock();
			}

			return 1;	
		}

		int File_impl::Read( UInt32 uSize, char* szBuf, Int64 Offset )
		{
			if( !is_open() )
				return -1;

			FileBuf_sptr	Buf;
			Offset = Offset == -1 ? _uROffset : Offset;

			uSize = __min(uSize, ASYNC_FILES_POOL_SIZE);
			if( szBuf == NULL )
			{
				Buf = FileBuf_sptr(reinterpret_cast<tagFileBuf*>(_Pool.Alloc()),
									function20_bind_Ex(&MemPool_type::Free, &_Pool));
				Buf->uSize = uSize;
				return _FileIO.async_read(Buf->Data, Buf->uSize,
					function20_bind(&File_impl::HandleRead,
					shared_from_this(), Buf, _function_::_1),
					Offset);
			}
			else
			{
				return _FileIO.async_read(szBuf, uSize,
					function20_bind(&File_impl::HandleRead,
					shared_from_this(), szBuf, uSize, _function_::_1),
					Offset);
			}
			return -1;
		}

		void File_impl::HandleRead( const FileBuf_sptr& Buf,
						tagIOData_INFO* pIOData )
		{
			if( is_open() && _hRead )
			{
				_uROffset += pIOData->_ibytes_transferred;
				Buf->Data[pIOData->_ibytes_transferred] = 0;
				_hRead(_Handle, Buf->Data, pIOData->_ibytes_transferred);
			}
		}

		void File_impl::HandleRead( const char* szBuf, UInt32 uSize,
						tagIOData_INFO* pIOData )
		{
			if( is_open() && _hRead )
			{
				_uROffset += pIOData->_ibytes_transferred;
				_hRead(_Handle, szBuf, pIOData->_ibytes_transferred);
			}
		}

		void File_impl::HandleWrite( const FileBuf_sptr& Buf,
									tagIOData_INFO* pIOData )
		{
			FileBuf_sptr sptr;
			_uWOffset += pIOData->_ibytes_transferred;

			_Lock.Lock();
			if( _List.size() == 0 )
			{
				_isWrite = false;
				_Lock.UnLock();
			}
			else
			{
				sptr = _List.front();
				_List.pop();
				_Lock.UnLock();
			}
			
			if( sptr )
			{
				UInt64 uOffset = sptr->iOffset == -1 ? _uWOffset : sptr->iOffset;
				Buf->uSize = sptr->uSize;
				memcpy(Buf->Data, sptr->Data, sptr->uSize);
				pIOData->_pOverlapped->_SWSAOverlapp.wsaBuf.buf = (char*)Buf->Data;
				pIOData->_pOverlapped->_SWSAOverlapp.wsaBuf.len = Buf->uSize;
				if( _FileIO.async_write(pIOData->_pOverlapped, uOffset) == -1 )
				{
					Close();
				}
			}
			
			if( is_open() && _hWrite )
			{
				_hWrite(_Handle, Buf->Data, pIOData->_ibytes_transferred);
			}
		}

	}// namespace	_files_

}// namespace	_async_