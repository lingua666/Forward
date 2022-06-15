
#include <libFrameWork_AsyncFiles/AsyncFiles.h>

namespace	_async_{

	namespace	_files_{

		AsyncFiles::AsyncFiles( void )
			: _Pool( sizeof(File_impl) )
		{

		}

		AsyncFiles::~AsyncFiles( void )
		{
			Release();
		}

		int AsyncFiles::Init( UInt8 uThreadNum )
		{
			if( _ioService.open() == -1 )
				return -1;

			uThreadNum = (uThreadNum == 0 ? get_processor_number() * 2 + 2 : uThreadNum);

			for(std::size_t i = 0; i < uThreadNum; ++ i)
			{
				Thread_ptr thread( new Thread_type(
					function20_bind(&io_service::run, &_ioService)) );
				_Threads.push_back(thread);
				thread->wait_run();
			}
			return 1;
		}

		void AsyncFiles::SetCallBack( THandle Handle,
				const HFNRead& hRead,
				const HFNWrite& hWrite,
				const HFNDestroy& hDestroy )
		{
			FileImpl_ptr sptr = FindFileImpl( Handle );
			if( sptr )
			{
				sptr->SetCallBack(function20_bind(&AsyncFiles::HandleRead, this,
					hRead, _function_::_1, _function_::_2, _function_::_3),
					function20_bind(&AsyncFiles::HandleWrite, this, hWrite,
					_function_::_1, _function_::_2, _function_::_3),
					function20_bind(&AsyncFiles::HandleDestroy, this, hDestroy, Handle));
			}
		}

		void AsyncFiles::Release( void )
		{
			_ioService.stop();
			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->join();
			}
			_Threads.clear();
			_ioService.close();
		}

		THandle AsyncFiles::Open( const char* szPath, int iMode )
		{
			if( szPath == NULL )
				return -1;

			FileImpl_ptr sptr( _Pool.AllocObj<File_impl>( io_service::io_service_sptr(&_ioService, false)),
							function20_bind_Ex(&MemPool_type::FreeObj<File_impl>,
							&_Pool) );
			HANDLE hFile = sptr->Open(szPath, iMode);
			if( hFile == INVALID_HANDLE_VALUE )
				return -1;

			_Lock.Lock();
			_Map.insert(std::make_pair((THandle)hFile, sptr));
			_Lock.UnLock();
			return (THandle)hFile;
		}

		int	AsyncFiles::Close( THandle Handle )
		{
			FileImpl_ptr sptr;
			_Lock.Lock();
			HashMap_type::iterator	iter = _Map.find(Handle);
			if( iter != _Map.end() )
			{
				sptr = iter->second;
				_Map.erase(iter);
				_Lock.UnLock();
				sptr->Close();
			}
			else
				_Lock.UnLock();
			return 1;
		}

		void AsyncFiles::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		int AsyncFiles::Write( THandle Handle, const char* szData, UInt32 uSize )
		{
			if( !is_open() )
				return -1;

			FileImpl_ptr sptr = FindFileImpl( Handle );
			if( !sptr )
				return -1;

			return sptr->Write(szData, uSize);
		}

		int AsyncFiles::Read( THandle Handle, UInt32 uSize, char* szBuf )
		{
			if( !is_open() )
				return -1;

			FileImpl_ptr sptr = FindFileImpl( Handle );
			if( !sptr )
				return -1;

			return sptr->Read(uSize, szBuf);
		}

		AsyncFiles::FileImpl_ptr  AsyncFiles::FindFileImpl( THandle Handle )
		{
			_Lock.Lock();
			HashMap_type::iterator	iter = _Map.find(Handle);
			if( iter != _Map.end() )
			{
				FileImpl_ptr sptr = iter->second;
				_Lock.UnLock();
				return sptr;
			}
			_Lock.UnLock();
			return FileImpl_ptr();	
		}

		void AsyncFiles::HandleRead( const HFNRead& hRead, HANDLE Handle,
								const char* szBuf, UInt32 uSize )
		{
			if( hRead )
			{
				hRead((THandle)Handle, szBuf, uSize);
			}
		}

		void AsyncFiles::HandleWrite( const HFNWrite& hWrite, HANDLE Handle,
								const char* szBuf, UInt32 uSize )
		{
			if( hWrite )
			{
				hWrite((THandle)Handle, uSize);
			}
		}

		void AsyncFiles::HandleDestroy( const HFNDestroy& hDestroy, THandle Handle )
		{
			if( hDestroy )
				hDestroy(Handle);
		}

	} //namespace	_files_

} //namespace	_async_