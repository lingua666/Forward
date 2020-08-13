
#include <libThread/ThreadWrap_UNIX.h>
#include <libCommon/CMyException.h>

namespace	_thread_{

#if defined(PLATFORM_OS_FAMILY_UNIX)

	pthread_key_t*	ThreadWrap::s_pTlsKey = ThreadAPI::TslAlloc();

	int	ThreadAPI::CreateThread( pthread_t* thread, 
		void* (*start_routine)(void *), void *arg )
	{
		return pthread_create(thread, NULL, start_routine, arg);
	}

	void	ThreadAPI::Wait( pthread_t thread, UInt32 u32Milliseconds )
	{
		#if PLATFORM_OS == PLATFORM_OS_ANDROID
			pthread_join(thread, NULL);//tid1一直阻赛，等到tid2的退出，获得TID2的退出码
		#else
			if( u32Milliseconds == INFINITE )
			{
				pthread_join(thread, NULL);//tid1一直阻赛，等到tid2的退出，获得TID2的退出码
			}
			else
			{
				timespec Delay;
				Delay.tv_nsec = u32Milliseconds;
				pthread_timedjoin_np(thread, NULL, &Delay);
			}
		#endif
	}

	UInt64	ThreadAPI::GetCurrentThreadId( void )
	{
		return (UInt64)pthread_self();
	}

	int ThreadAPI::Close( UInt64 Thread_ID )
	{
		pthread_detach( Thread_ID );
		return 1;
	}

	pthread_key_t*	ThreadAPI::TslAlloc( void )
	{
		pthread_key_t* key = new pthread_key_t;
		pthread_key_create(key, NULL);
		return key;
	}

	int	ThreadAPI::TlsFree( pthread_key_t key )
	{
		return pthread_key_delete(key);
	}

	int	ThreadAPI::TlsSetValue( pthread_key_t key, const void* lpTlsValue )
	{
		return pthread_setspecific(key, lpTlsValue);
	}

	void*	ThreadAPI::TlsGetValue( pthread_key_t key )
	{
		return pthread_getspecific(key);
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

	void	ThreadData::set( const UInt16 u16Code )
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
		ThreadWrap::Release();
		if( s_pTlsKey == NULL )
		{
			s_pTlsKey = ThreadAPI::TslAlloc();
		}
	}

	void	ThreadWrap::Release( void )
	{
		if( s_pTlsKey != NULL )
		{
			ThreadAPI::TlsFree(*s_pTlsKey);
			delete s_pTlsKey;
			s_pTlsKey = NULL;
		}
	}

	ThreadWrap*	ThreadWrap::get_current_thread( void )
	{
		return reinterpret_cast<self_type*>(ThreadAPI::TlsGetValue(*s_pTlsKey));
	}

	void	ThreadWrap::interruption_point( void )
	{
		self_type* self = reinterpret_cast<self_type*>( get_current_thread() );
		if ( self != NULL && self->_ThrdData.isInterrupted() )
		{
			self->_ThrdData.clear();
			throw	thread_interrupted("", self->_ThrdData.get() );
		}
	}



	ThreadWrap::ThreadWrap( void )
		: _uThreadId(0)
		, _is_run(false)
		, _Handle(0)
	{

	}

	/*!
	* @function   ThreadWrap
	* @brief    
	* @return  
	*/
	ThreadWrap::ThreadWrap( const function_handle& handle )
		: _uThreadId(0)
		, _is_run(false)
		, _ThreadProc(handle)
	{
		ThreadAPI::CreateThread( &_Handle, WorkerThread, this );
	}

	/*!
	* @function   ~ThreadWrap
	* @brief    
	* @return  
	*/
	ThreadWrap::~ThreadWrap( void )
	{
		exit();
	}

	int ThreadWrap::run( const function_handle& handle )
	{
		if( _Handle != 0 )
			exit();	

		_ThreadProc = handle;
		ThreadAPI::CreateThread( &_Handle, WorkerThread, this );
		return _Handle != 0 ? 1 : -1;
	}

	void	ThreadWrap::wait_run( void )
	{
		while( !_is_run )
			Sleep(0);
	}

	void	ThreadWrap::join( UInt32 u32Milliseconds )
	{
		ThreadAPI::Wait(_Handle, u32Milliseconds);
	}

	void	ThreadWrap::exit( void )
	{
		_ThrdData.set(ThreadData::en_INTERRUPTED_EXIT);
		join(INFINITE);
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

	void ThreadWrap::close( void )
	{
		if( _uThreadId != 0 )
		{
			ThreadAPI::Close(_uThreadId);
			_uThreadId = 0;
		}
	}

	void* ThreadWrap::WorkerThread( void* pParam )
	{
		self_type* self = reinterpret_cast<self_type*>(pParam);
		self->_uThreadId = ThreadAPI::GetCurrentThreadId();
		ThreadAPI::TlsSetValue(*s_pTlsKey, pParam);

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
