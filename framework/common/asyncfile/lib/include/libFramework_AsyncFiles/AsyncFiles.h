
#ifndef __ASYNCFILES_20170405105420_1442717660_H__
#define __ASYNCFILES_20170405105420_1442717660_H__

#include "External.h"
#include "File_impl.h"

namespace	_async_{

	namespace	_files_{

		class AsyncFiles
		{
		public:
			typedef _thread_::ThreadWrap	Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;

			typedef Int64 THandle;
			typedef SmartPTR<File_impl>	FileImpl_ptr;
			typedef Container_HashMap_type<THandle, FileImpl_ptr>	HashMap_type;

			typedef	function20_handle<void (THandle, const char*, UInt32)>	HFNRead;
			typedef function20_handle<void (THandle, UInt32)>		HFNWrite;
			typedef function20_handle<void (THandle)>		HFNDestroy;

		public:
			AsyncFiles( void );

			~AsyncFiles( void );

			int Init( UInt8 uThreadNum = 0 );

			void SetCallBack( THandle Handle, 
							const HFNRead& hRead = HFNRead(),
							const HFNWrite& hWrite = HFNWrite(),
							const HFNDestroy& hDestroy = HFNDestroy() );

			void Release( void );

			THandle Open( const char* szPath, int iMode );
			
			int	Close( THandle Handle );

			void CloseAll( void );

			int Write( THandle Handle, const char* szData, UInt32 uSize );

			int Read( THandle Handle, UInt32 uSize, char* szBuf = NULL );

			bool is_open( void ) const
			{
				return _ioService.is_open() ? true : false;
			}

		protected:
			AsyncFiles::FileImpl_ptr  FindFileImpl( THandle Handle );

			void HandleRead( const HFNRead& hRead, HANDLE Handle,
					const char* szBuf, UInt32 uSize );

			void HandleWrite( const HFNWrite& hWrite, HANDLE Handle,
					const char* szBuf, UInt32 uSize );

			void HandleDestroy( const HFNDestroy& hDestroy, THandle Handle );

		private:
			HashMap_type	_Map;
			MemPool_type	_Pool;
			ThreadList		_Threads;
			CLock			_Lock;
			io_service		_ioService;
		};

	}// namespace _files_

} // namespace _async_

inline _async_::_files_::AsyncFiles* GetAsyncFilesInstance( void )
{
	static _async_::_files_::AsyncFiles* pObj = Singleton<_async_::_files_::AsyncFiles>::instance();
	return pObj;
}


#endif//__COMMANDCLIENT_20150920105420_1442717660_H__
