
#ifndef __THREADWRAPWIN32_20151007194953_1444218593_H__
#define __THREADWRAPWIN32_20151007194953_1444218593_H__

#include <libFunction_v20/CMyFunction.hpp>

namespace	_thread_{

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

	DECLARE_EXCEPTION(thread_interrupted,CMyException)

	struct ThreadAPI
	{
		static HANDLE	CreateThread( LPTHREAD_START_ROUTINE lpStartAddress, 
									void* pParam, UInt64& ulThreadID );

		static void	ExitThread( HANDLE handle );

		static	void	Wait( HANDLE handle, UInt32 u32Milliseconds );

		static	UInt64	GetCurrentThreadId( void );

		static	int Close( HANDLE Handle );

		static	UInt32	TslAlloc( void );

		static	bool	TlsFree( UInt32	u32TlsIndex );

		static	void	TlsSetValue( UInt32	u32TlsIndex, void* lpTlsValue );

		static	void*	TlsGetValue( UInt32	u32TlsIndex );
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

		void	set( UInt16 u16Code = 0 );

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
	public:
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

		bool	is_run(void);

	protected:
		void close( void );

	protected:
		static DWORD WINAPI WorkerThread( void* pParam );

	private:
		function_handle	_ThreadProc;
		HANDLE	_Handle;
		UInt64	_uThreadId;				//Ïß³ÌID
		ThreadData	_ThrdData;
		bool		_is_run;
		static	UInt32	s_u32TlsIndex;
	};
	/** @} end ThreadWrap */

#endif

}	//namespace	_foundation

#endif//__THREADWRAP_20151007194953_1444218593_H__
