/*
 ============================================================================
 Name        : Uart.c
 Author      : 
 Version     : V1.0
 Copyright   : 
 Description : 
 ============================================================================
 */
#ifndef __UART_20161222215604_1438860785_H__
#define __UART_20161222215604_1438860785_H__

#include "External.h"

#define UART_GETSIZE	1024 * 100

namespace	_uart_{

#pragma pack(push, 1)

	struct tagUart_CONFIG
	{
		UInt8 Com_No;
		UInt32 uBaudrate;
		UInt8 uByteSize;
		UInt8 uStopBits;
		bool isParityCheck;
		bool isHex;
	};

	typedef struct uart_s {
		HANDLE hWnd ;
		unsigned char GetBuf[UART_GETSIZE] ;
		int GetIn , GetRead ;
		int CmdIn ;
		unsigned char CmdInBuf[128] ;
	} uart_t, *uart_p;

#pragma pack(pop)

	class CUart
	{
	public:
		CUart( void );

		~CUart( void );

		bool SetConfig( UInt8 uStopBits = 1 , bool isParityCheck = false, bool isHex = false );

		bool Open( UInt8 Com_No, UInt32 uBaudrate, UInt8 ByteSize = 8 );

		void Close( void );

		_string_type Receive( void ) ;

		int Send( const char* szData, UInt32 uLen );

	private:
		tagUart_CONFIG	_Config;
		uart_t			_MyUart;
		_string_type	_sRecvBuf;
	};

} //_uart_

#endif
