
#include <libFrameWork_Uart/Uart.h>
#include <iostream>

#define UART_RECV_BUF_SIZE	1024

namespace	_uart_{

	CUart::CUart( void )
	{
		memset(&_MyUart, 0, sizeof(_MyUart));
		memset(&_Config, 0, sizeof(_Config));
		_sRecvBuf.reserve(UART_RECV_BUF_SIZE);
	}

	CUart::~CUart( void )
	{

	}

	bool CUart::SetConfig( UInt8 uStopBits, bool isParityCheck, bool isHex )
	{
		_Config.uStopBits = uStopBits;
		_Config.isParityCheck = isParityCheck;
		_Config.isHex = isHex;
		return true;
	}

	bool CUart::Open( UInt8 Com_No, UInt32 uBaudrate, UInt8 uByteSize )
	{
		Close();

		_string_type sCom;
		if( Com_No > 9 )
		{//大于10, 需要增加改变名称
			sCom += "\\\\.\\";
		}

		sCom += "COM" + _string_type::NumberToStr(Com_No);
		sCom += ":";

		_MyUart.hWnd = NULL;
		_MyUart.hWnd = CreateFileA(sCom.c_str() ,
			GENERIC_READ | GENERIC_WRITE ,
			0,
			NULL ,
			OPEN_EXISTING ,
			//FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED ,
			0,//No Overlapped
			NULL
			) ;
		if ( _MyUart.hWnd == NULL )
		{
			return false ;
		}

		//设置缓冲区
		if (!SetupComm( _MyUart.hWnd, 40960, 40960 ))
		{
			Close();
			return false ;
		}
		// 清除湲冲区
		PurgeComm( _MyUart.hWnd, PURGE_TXABORT | PURGE_RXABORT |
			PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

		//设置
		DCB dcb;
		//取得串行端口设置
		GetCommState( _MyUart.hWnd, &dcb );
		//更改传输速率
		if ( uBaudrate <= 1200 )
			dcb.BaudRate = 1200 ;
		else if ( uBaudrate <= 2400 )
			dcb.BaudRate = 2400 ;
		else if ( uBaudrate <= 4800 )
			dcb.BaudRate = 4800 ;
		else if ( uBaudrate <= 9600 )
			dcb.BaudRate = 9600 ;
		else if ( uBaudrate <= 19200 )
			dcb.BaudRate = 19200 ;
		else if ( uBaudrate <= 38400 )
			dcb.BaudRate = 38400 ;
		else if ( uBaudrate <= 57600 )
			dcb.BaudRate = 57600 ;
		else if ( uBaudrate <= 115200 )
			dcb.BaudRate = 115200 ;
		else if ( uBaudrate <= 230400 )
			dcb.BaudRate = 230400 ;
		else
			dcb.BaudRate = 9600 ;
		dcb.fBinary  = 1; //必须指定为1
		dcb.Parity =  NOPARITY ;//Parity的指定
		dcb.fParity = false ;   // Enable parity check
		dcb.fRtsControl=0;
		//dcb.fOutX = 1; //dcb_OutX;
		//dcb.fInX  = 1; //dcb_InX;
		dcb.ByteSize = uByteSize;
		dcb.StopBits = _Config.uStopBits;//ONESTOPBIT ;

		//将设置写入
		SetCommState( _MyUart.hWnd, &dcb );
		COMMTIMEOUTS CommTimeouts;
		GetCommTimeouts (_MyUart.hWnd, &CommTimeouts);

		// Change the COMMTIMEOUTS structure settings.
		CommTimeouts.ReadIntervalTimeout = 100;//MAXDWORD;  
		CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
		CommTimeouts.ReadTotalTimeoutConstant = 0;    
		CommTimeouts.WriteTotalTimeoutMultiplier = 1;  
		CommTimeouts.WriteTotalTimeoutConstant = 0;    

		// Set the time-out parameters for all read and write operations
		// on the port. 
		if (!SetCommTimeouts(_MyUart.hWnd, &CommTimeouts))
		{
			// Could not set the time-out parameters.
			Close();
			return false;
		}

		//初始化其他参数
		_MyUart.GetIn = 0 ;
		_MyUart.GetRead = 0 ;
		_MyUart.CmdIn = 0 ;

		return true ;
	}

	void CUart::Close( void )
	{
		if(_MyUart.hWnd == 0)
			return ;

		CloseHandle(_MyUart.hWnd) ;
		_MyUart.hWnd = 0;
		memset(&_Config, 0, sizeof(_Config));
	}

	_string_type CUart::Receive( void )
	{
		if(_MyUart.hWnd == NULL)
			return "";

		DWORD    nBytesRead = 0, dwCommError;
		COMSTAT  CS;
		ClearCommError(_MyUart.hWnd, &dwCommError, &CS);  //取得状态
		if( CS.cbInQue != 0 )  //若缓冲区有数据，则读取
		{
			//ReadLen = CS.cbInQue; 获取读取的长度
			ReadFile(_MyUart.hWnd, (char*)_sRecvBuf.c_str(),
				UART_RECV_BUF_SIZE, &nBytesRead, NULL); // 接收COM 的数据
			_sRecvBuf.update_size(nBytesRead);

			if( _Config.isHex )
				return _string_type::HexToStr(_sRecvBuf);

			return _sRecvBuf;
		} //cs.binQue Loop

		return "";
	}

	int  CUart::Send( const char* szData, UInt32 uLen )
	{
		if(_MyUart.hWnd == NULL)
			return 0;

		UInt32 uSize = 0;

		if( !_Config.isHex )
		{//原文
			WriteFile(_MyUart.hWnd, szData, uLen, (DWORD*)&uSize, NULL);
		}
		else
		{//转换16进制发送
			_string_type s = _string_type::StrToHex( _string_type(szData, uLen) );
			WriteFile(_MyUart.hWnd, s.c_str(), s.size(), (DWORD*)&uSize, NULL);
		}

		return uSize;
	}

} //_uart_