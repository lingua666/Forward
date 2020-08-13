#ifndef __UART_IMPL_20170405105204_1442199361_H__
#define __UART_IMPL_20170405105204_1442199361_H__

#include "External.h"

#define		ASYNC_UARTS_POOL_SIZE	1024 * 4

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	#include <libIOCP_File/FileIO.h>
	#include <libCommon/os/winsock2i.h>
	typedef _iocp_file_::io_service						io_service_file;
	typedef _iocp_file_::FileIO							FileIO_file;
	typedef _iocp_file_::tagIOData_INFO					tagIOData_INFO_file;
	typedef _iocp_file_::tagOverlapped					tagOverlapped;
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	#include <termios.h>
	#include <libEPoll_File/FileIO.h>
	typedef _epoll_non_regular_file_::io_service		io_service_file;
	typedef _epoll_non_regular_file_::FileIO			FileIO_file;
	typedef _epoll_non_regular_file_::tagIOData_INFO	tagIOData_INFO_file;
	typedef _epoll_non_regular_file_::tagOverlapped		tagOverlapped;

	#define NOPARITY            0
	#define ODDPARITY           1
	#define EVENPARITY          2
	#define MARKPARITY          3
	#define SPACEPARITY         4

	#define ONESTOPBIT          1
	#define ONE5STOPBITS        2
	#define TWOSTOPBITS         3
#endif

namespace	_async_{

	namespace	_uarts_{

		typedef io_service_file			io_service;
		typedef FileIO_file				FileIO;
		typedef tagIOData_INFO_file		tagIOData_INFO;

		#pragma pack(push, 1)
		struct tagUartBuf
		{
			UInt32	uSize;
			char	Data[1];
		};
		#pragma pack(pop)

		class Uart_impl : public _enable_shared_from_this<Uart_impl>
		{
		public:
			typedef SmartPTR<Uart_impl>		self_sptr;
			typedef SmartPTR<tagUartBuf>	UartBuf_sptr;
			typedef	LinkList_type<UartBuf_sptr, CNoneLock>	LList;
			typedef	function20_handle<void (void)>	HFNDestroy;
			typedef	function20_handle<void (const self_sptr&, const UartBuf_sptr&)>	HFNRead;
			typedef	function20_handle<void (HANDLE, const char*, UInt32)>	HFNNotify;

#pragma pack(push, 1)		
			typedef struct uart_s {
				HANDLE hWnd ;
			} uart_t, *uart_p;

#pragma pack(pop)

		public:
			Uart_impl( const io_service::io_service_sptr& IOService );

			~Uart_impl( void );

			void Init( void );

			void Release( void );
		
#if defined( PLATFORM_OS_FAMILY_WINDOWS )
			HANDLE	Open( UInt8 ComNo, UInt32 uBaudrate, UInt8 uByteSize,
				UInt8 uStopBits, bool isParityCheck ); 

			//! 获得串口参数 DCB
			int GetState( DCB *pDcb );

			//! 设置串口参数 DCB
			bool SetState( DCB *pDcb );
#elif defined( PLATFORM_OS_FAMILY_UNIX )
			HANDLE	Open( const char* szFile, UInt32 uBaudrate, UInt8 uByteSize,
						UInt8 uStopBits, bool isParityCheck );
#endif

			void Close( void );

			void SetCallBack( const HFNRead& hRead,
				const HFNNotify& hWrite,
				const HFNDestroy& hDestroy );

			int Send( const char* szData, UInt32 uSize );

			int PostRecv( UInt32 uSize, tagOverlapped* pOverlapped = NULL );

			bool is_open( void ) const
			{
				return _MyUart.hWnd != INVALID_HANDLE_VALUE ? true : false;
			}

			HANDLE GetHandle( void )
			{
				return _MyUart.hWnd;
			}

		protected:
			void HandleReceive( tagIOData_INFO* pIOData );

			void HandleSend( const char* c_pData, UInt32 u32Size,
							tagIOData_INFO* pIOData );

		private:
			uart_t			_MyUart;
			CLock			_Lock;
			MemPool_type	_Pool;
			bool			_isWrite;
			LList			_List;
			HFNRead			_hRead;
			HFNNotify		_hWrite;
			HFNDestroy		_hDestroy;
			FileIO			_FileIO;

#if defined( PLATFORM_OS_FAMILY_UNIX )
			struct termios	_old_options;   //串口的配置结构体   新的  旧的
#endif
		};

	}// namespace	_files_

}// namespace	_async_


#endif