
#ifndef __THREADWRAP_UNIX_20151011015754_1444499874_H__
#define __THREADWRAP_UNIX_20151011015754_1444499874_H__

#include <libFunction_v20/CMyFunction.hpp>

namespace	_thread_{

#if defined(PLATFORM_OS_FAMILY_UNIX)

	DECLARE_EXCEPTION(thread_interrupted,CMyException)

	#define INFINITE            0xFFFFFFFF  // Infinite timeout

	struct ThreadAPI
	{
		static int	CreateThread( pthread_t* thread, 
			void* (*start_routine)(void *), void *arg );

		static	void	Wait( pthread_t thread, UInt32 u32Milliseconds );

		static	UInt64	GetCurrentThreadId( void );

		static	int Close( UInt64 Thread_ID );

		static	pthread_key_t*	TslAlloc( void );

		static	int	TlsFree( pthread_key_t key );

		static	int	TlsSetValue( pthread_key_t key, const void* lpTlsValue );

		static	void*	TlsGetValue( pthread_key_t key );
	};

	class	ThreadData
	{
	public:
		enum en_InterruptedCode
		{
			en_INTERRUPTED_NULL,
			en_INTERRUPTED_SUSPEND,
			en_INTERRUPTED_RESUMING,
			en_INTERRUPTED_EXIT
		};

	public:
		ThreadData( void );

		~ThreadData( void );

		bool	isInterrupted( void ) const;

		void	clear( void );

		void	set( const UInt16 u16Code = 0 );

		const UInt16	get( void ) const;

	private:
		bool _isInterrupted;
		UInt16	_u16Code;
	};

	/*!
	* @class  ThreadWrap
	* @brief   ThreadWrap
	* @{
	*/

	class ThreadWrap
	{
	public:
		typedef	ThreadWrap	self_type;
		typedef function20_handle<void (void)>	function_handle;
		typedef	pthread_t	HANDLE;

	public:
		static	pthread_key_t*	s_pTlsKey;
		static	bool	Init( void );

		static	void	Release( void );

		static	ThreadWrap*	get_current_thread( void );

		static	void interruption_point( void );

	public:
		ThreadWrap( void );

		/*!
		* @function   ThreadWrap
		* @brief    
		* @return  
		*/
		ThreadWrap( const function_handle& handle );

		/*!
		* @function   ~ThreadWrap
		* @brief    
		* @return  
		*/
		~ThreadWrap( void );

		int	run( const function_handle& handle );

		void	wait_run( void );

		void	join( UInt32 u32Milliseconds = INFINITE );

		void	exit( void );

		UInt64	get_id( void ) const;

		void	interrupt( UInt32 iCode = 0 );

	protected:
		void close( void );

	protected:
		static void* WorkerThread( void* pParam );

	private:
		function_handle	_ThreadProc;
		HANDLE	_Handle;
		bool	_is_run;
		UInt64	_uThreadId;		//Ïß³ÌID
		ThreadData	_ThrdData;
	};
	/** @} end ThreadWrap */

#endif

}	//namespace	_foundation

#endif//__THREADWRAP_UNIX_20151011015754_1444499874_H__
