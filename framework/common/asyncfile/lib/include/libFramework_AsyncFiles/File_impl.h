#ifndef __FILE_IMPL_20170405105204_1442199361_H__
#define __FILE_IMPL_20170405105204_1442199361_H__

#include "External.h"

#define		ASYNC_FILES_POOL_SIZE	1024 * 5

namespace	_async_{

	namespace	_files_{

		#if defined( PLATFORM_OS_FAMILY_WINDOWS )
			typedef _iocp_file_::io_service						io_service;
			typedef _iocp_file_::FileIO							FileIO;
			typedef _iocp_file_::tagIOData_INFO					tagIOData_INFO;
		#elif defined( PLATFORM_OS_FAMILY_UNIX )
			typedef _epoll_regular_file_::io_service			io_service;
			typedef _epoll_regular_file_::FileIO				FileIO;
			typedef _epoll_regular_file_::tagIOData_INFO		tagIOData_INFO;
		#endif

		struct tagFileBuf
		{
			Int64	iOffset;
			UInt32	uSize;
			__M_ALIGNED_PACKED__ char	Data[1];
		};

		class File_impl : public _enable_shared_from_this<File_impl>
		{
		public:
			typedef SmartPTR<File_impl>		self_sptr;
			typedef SmartPTR<tagFileBuf>	FileBuf_sptr;
			typedef	LinkList_type<FileBuf_sptr, CNoneLock>	LList;
			typedef	function20_handle<void (void)>	HFNDestroy;
			typedef	function20_handle<void (THandle, const char*, UInt32)>	HFNNotify;

			enum FilesMode
			{
				enMode_Read,
				enMode_Write,
				enMode_ReadWrite
			};
		public:
			File_impl( const io_service::io_service_sptr& IOService );

			~File_impl( void );

			void Init( void );

			void Release( void );
		
			HANDLE	Open( const char* szPath, int iMode, THandle hToken);

			void Close( void );

			void SetCallBack( const HFNNotify& hRead,
				const HFNNotify& hWrite,
				const HFNDestroy& hDestroy );

			int Write( const char* szData, UInt32 uSize, Int64 Offset = -1 );

			int Read( UInt32 uSize, char* szBuf = NULL, Int64 Offset = -1 );

			int Seek_Read( Int64 uOffset );

			UInt64 Tell_Read( void );

			bool is_open( void ) const
			{
				//return _Handle != INVALID_HANDLE_VALUE ? true : false;
				return _isOpen;
			}

		protected:

			void Close_Impl(void);

			void HandleRead( const FileBuf_sptr& Buf,
				tagIOData_INFO* pIOData );

			void HandleRead( const char* szBuf, UInt32 uSize,
				tagIOData_INFO* pIOData );

			void HandleWrite( const FileBuf_sptr& Buf,
				tagIOData_INFO* pIOData );

			void HandleFlush(void);

		private:
			UInt64			_uROffset;
			UInt64			_uWOffset;
			CLock			_Lock;
			MemPool_type	_Pool;
			bool			_isWrite;
			LList			_List;
			HANDLE			_Handle;
			HFNNotify		_hRead;
			HFNNotify		_hWrite;
			HFNDestroy		_hDestroy;
			FileIO			_FileIO;
			bool			_isOpen;
			THandle 			_hToken;
		};

	}// namespace	_files_

}// namespace	_async_


#endif