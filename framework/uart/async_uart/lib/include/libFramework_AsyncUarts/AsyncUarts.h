
#ifndef __ASYNCUARTS_20170405105420_1442717660_H__
#define __ASYNCUARTS_20170405105420_1442717660_H__

#include "External.h"
#include "Uart_impl.h"

namespace	_async_{

	namespace	_uarts_{

		class AsyncUarts
		{
		public:
			typedef _thread_::ThreadWrap	Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;

			typedef Int64 THandle;
			typedef Uart_impl::UartBuf_sptr	UartBuf_sptr;
			typedef Uart_impl::self_sptr	UartImpl_ptr;
			typedef Container_HashMap_type<THandle, UartImpl_ptr>	HashMap_type;

			typedef	function20_handle<void (THandle, const char*, UInt32)>	HFNRead;
			typedef function20_handle<void (THandle, UInt32)>				HFNWrite;
			typedef function20_handle<void (THandle)>						HFNDestroy;

			typedef function20_handle<void (void)>							HFNData;
			typedef LinkList_type<HFNData>									DataList;

		public:
			static	void	WorkThread( void* pParamter );

			static	void	ProcessData( const HFNRead& hRead,
				const UartImpl_ptr& UartImpl_sptr,
				const UartBuf_sptr& Buf_sptr );

		public:
			AsyncUarts( void );

			~AsyncUarts( void );
			
			//IO底层最少两个线程(在IC2000系统里面一个线程会出现接收的数据乱码问题)
			int Init( UInt8 uThreadNum = 2, UInt8 uProcNum = 1 );

			void SetCallBack( THandle Handle, 
							const HFNRead& hRead = HFNRead(),
							const HFNDestroy& hDestroy = HFNDestroy(),
							const HFNWrite& hWrite = HFNWrite() );

			void Release( void );

			THandle Open( const char* ComName, UInt32 uBaudrate, UInt8 ByteSize = 8,
						UInt8 StopBit = 1, bool isParity = false );
			
			int	Close( THandle Handle );

			void CloseAll( void );

			int Send( THandle Handle, const char* szData, UInt32 uSize );

			//单位毫秒
			void	SetSleepStep(UInt32 uMillisecond);

		protected:
			AsyncUarts::UartImpl_ptr  FindUartImpl( THandle Handle );

			void HandleReceive( const HFNRead& hRead,
					const UartImpl_ptr& UartImpl_sptr,
					const UartBuf_sptr& Buf_sptr );

			void HandleSend( const HFNWrite& hWrite, HANDLE Handle,
					const char* szBuf, UInt32 uSize );

			void HandleDestroy( const HFNDestroy& hDestroy, THandle Handle );

		private:
			HashMap_type	_Map;
			MemPool_type	_Pool;
			ThreadList		_Threads;
			ThreadList		_Workers;
			CLock			_Lock;
			CLock			_ReadLock;
			io_service 		_ioService;
			DataList		_DataList;
			UInt32			_uMillisecond;
		};

		AsyncUarts* GetAsyncUartsInstance(void);

	}// namespace _files_

} // namespace _async_


#endif//__COMMANDCLIENT_20150920105420_1442717660_H__
