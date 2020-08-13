
#include <libThread/ThreadWrap_Win32.h>
#include <libCommon/CMyException.h>

namespace	_thread_{

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

	UInt32	ThreadWrap::s_u32TlsIndex = ThreadAPI::TslAlloc();

	HANDLE	ThreadAPI::CreateThread( LPTHREAD_START_ROUTINE lpStartAddress, 
		void* pParam, UInt64& ulThreadID )
	{
		return ::CreateThread(NULL, 0, lpStartAddress, pParam, 0, reinterpret_cast<LPDWORD>(&ulThreadID));
	}

	void	ThreadAPI::ExitThread( HANDLE handle )
	{
		DWORD dwExitCode;

		//µÈ´ý100ºÁÃëÈð³ö
		Wait( handle, 100 );  //INFINITE
		if(!GetExitCodeThread(handle,&dwExitCode))
			TerminateThread(handle,dwExitCode);
		CloseHandle(handle);
	}
	void	ThreadAPI::Wait( HANDLE handle, UInt32 u32Milliseconds )
	{
		WaitForSingleObject(handle, u32Milliseconds);
	}

	UInt64	ThreadAPI::GetCurrentThreadId( void )
	{
		return (UInt64)::GetCurrentThreadId();
	}

	int ThreadAPI::Close( HANDLE Handle )
	{
		if( Handle == NULL )
			return 0;

		CloseHandle(Handle);
		return 1;
	}

	UInt32	ThreadAPI::TslAlloc( void )
	{
		return ::TlsAlloc();
	}

	bool	ThreadAPI::TlsFree( UInt32	u32TlsIndex )
	{
		return ::TlsFree(u32TlsIndex) ? true : false;
	}

	void	ThreadAPI::TlsSetValue( UInt32	u32TlsIndex, void* lpTlsValue )
	{
		::TlsSetValue(u32TlsIndex, lpTlsValue);
	}

	void*	ThreadAPI::TlsGetValue( UInt32	u32TlsIndex )
	{
		return ::TlsGetValue(u32TlsIndex);
	}




	ThreadData::ThreadData( void )
		: _isInterrupted( false )
		, _u16Code(en_INTERRUPTED_NULL)
	{

	}

	ThreadData::~ThreadData( void )
	{
		_isInterrupted = false;
	}

	bool	ThreadData::isInterrupted( void ) const
	{
		return _isInterrupted;
	}

	void	ThreadData::clear( void )
	{
		_isInterrupted = false;
	}

	void	ThreadData::set( UInt16 u16Code )
	{
		_isInterrupted = true;
		_u16Code = u16Code;
	}

	const UInt16	ThreadData::get( void ) const
	{
		return _u16Code;
	}




	bool	ThreadWrap::Init( void )
	{
		if( s_u32TlsIndex == TLS_OUT_OF_INDEXES )
		{
			s_u32TlsIndex = ThreadAPI::TslAlloc();
		}

		return s_u32TlsIndex == TLS_OUT_OF_INDEXES ? false : true;
	}

	void	ThreadWrap::Release( void )
	{
		if( s_u32TlsIndex != TLS_OUT_OF_INDEXES )
		{
			ThreadAPI::TlsFree(s_u32TlsIndex);
			s_u32TlsIndex = TLS_OUT_OF_INDEXES;
		}
	}

	ThreadWrap*	ThreadWrap::get_current_thread( void )
	{
		return s_u32TlsIndex != TLS_OUT_OF_INDEXES ?\
			reinterpret_cast<self_type*>(ThreadAPI::TlsGetValue(s_u32TlsIndex)) : NULL;
	}

	void ThreadWrap::interruption_point( void )
	{
		self_type* self = reinterpret_cast<self_type*>( get_current_thread() );
		if ( self != NULL && self->_ThrdData.isInterrupted() )
		{
			self->_ThrdData.clear();
			throw	thread_interrupted("", self->_ThrdData.get() );
		}
	}



	ThreadWrap::ThreadWrap( void )
			: _is_run(false)
			, _Handle(NULL)
	{

	}

	ThreadWrap::ThreadWrap( const function_handle& handle )
		: _is_run(false)
		, _ThreadProc(handle)
	{
		_Handle = ThreadAPI::CreateThread( WorkerThread, this, _uThreadId );
	}

	ThreadWrap::~ThreadWrap( void )
	{
		exit();
	}

	int ThreadWrap::run( const function_handle& handle )
	{
		if( _Handle != NULL )
			exit();	

		_ThreadProc = handle;
		_Handle = ThreadAPI::CreateThread( WorkerThread, this, _uThreadId );
		return _Handle != NULL ? 1 : -1;
	}

	void	ThreadWrap::wait_run( void )
	{
		while( !_is_run )
			Sleep(0);
	}

	void	ThreadWrap::join( UInt32 u32Milliseconds )
	{
		if( _uThreadId == ThreadAPI::GetCurrentThreadId())
			printf("ThreadWrap::join error!!!\r\n");
		else
			ThreadAPI::Wait(_Handle, u32Milliseconds);
	}

	void	ThreadWrap::exit( void )
	{
		_ThrdData.set(ThreadData::en_INTERRUPTED_EXIT);
		join(100);
		close();
	}

	UInt64	ThreadWrap::get_id( void ) const
	{
		return _uThreadId;
	}

	void	ThreadWrap::interrupt( UInt32 iCode )
	{
		_ThrdData.set(iCode);
	}

	void	ThreadWrap::close( void )
	{
		if( _Handle != NULL )
		{
			ThreadAPI::Close(_Handle);
			_Handle = NULL;
		}
	}

	DWORD WINAPI ThreadWrap::WorkerThread( void* pParam )
	{
		self_type* self = reinterpret_cast<self_type*>(pParam);
		if( s_u32TlsIndex != TLS_OUT_OF_INDEXES )
			ThreadAPI::TlsSetValue(s_u32TlsIndex, pParam);

		self->_is_run = true;
		if( self->_ThreadProc )
		{
			self->_ThreadProc();
		}
		self->_is_run = false;

		self->close();

		return 0;
	}

#endif

}	//namespace	_foundation_