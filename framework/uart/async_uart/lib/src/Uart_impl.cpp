
#include <libFrameWork_AsyncUarts/Uart_impl.h>

namespace	_async_{

	namespace	_uarts_{
	
		Uart_impl::Uart_impl( const io_service::io_service_sptr& IOService )
			: _FileIO( IOService )
			, _Pool( ASYNC_UARTS_POOL_SIZE + sizeof(tagUartBuf) )
			, _isWrite( false )
		{
			memset(&_MyUart, 0, sizeof(_MyUart));
			_MyUart.hWnd = INVALID_HANDLE_VALUE;
		}

		Uart_impl::~Uart_impl( void )
		{
			Release();
			if( _hDestroy )
				_hDestroy();
		}

		void Uart_impl::Init( void )
		{

		}

		void Uart_impl::Release( void )
		{
			Close();
		}

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		const char g_StopBits201711102321[] = { ONESTOPBIT, ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS };
		HANDLE	Uart_impl::Open( UInt8 ComNo, UInt32 uBaudrate, UInt8 uByteSize,
			UInt8 uStopBits, bool isParityCheck )
		{
			_string_type sCom = "\\\\.\\COM";
			sCom += _string_type::NumberToStr(ComNo);

			_MyUart.hWnd = CreateFileA(sCom.c_str() ,
				GENERIC_READ | GENERIC_WRITE ,
				0,
				NULL ,
				OPEN_EXISTING ,
				//FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED ,
				FILE_FLAG_OVERLAPPED,//No Overlapped
				NULL
				) ;
			if ( _MyUart.hWnd == INVALID_HANDLE_VALUE )
			{
				int ir = GetLastError();
				return INVALID_HANDLE_VALUE ;
			}

			//设置缓冲区
			if (!SetupComm( _MyUart.hWnd, 40960, 40960 ))
			{
				Close();
				return INVALID_HANDLE_VALUE ;
			}
			// 清除湲冲区
			PurgeComm( _MyUart.hWnd, PURGE_TXABORT | PURGE_RXABORT |
				PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

			//设置
			DCB dcb;
			GetState(&dcb);

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
			dcb.fParity = isParityCheck ;   // Enable parity check
			if( !isParityCheck )
			{
				dcb.Parity =  NOPARITY ;//Parity的指定
			}
			else
			{//
				
			}
			dcb.fRtsControl = 0;
			dcb.ByteSize = uByteSize;
			dcb.StopBits = g_StopBits201711102321[uStopBits];//ONESTOPBIT ;

			//将设置写入
			SetState(&dcb);

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

			if( _MyUart.hWnd == INVALID_HANDLE_VALUE )
				return INVALID_HANDLE_VALUE;

			if( _FileIO.Bind(_MyUart.hWnd) == -1 )
			{//关联失败
				Close();
				return INVALID_HANDLE_VALUE;
			}

			if( PostRecv(ASYNC_UARTS_POOL_SIZE) == -1 )
			{
				Close();
				return INVALID_HANDLE_VALUE;
			}

			return _MyUart.hWnd;
		}

		//! 获得串口参数 DCB
		int Uart_impl::GetState( DCB *pDcb )
		{
			if( !is_open() || pDcb == NULL )
				return -1;

			return ::GetCommState(_MyUart.hWnd, pDcb) == TRUE ? (pDcb != NULL ? 1 : -1) : -1;
		}

		//! 设置串口参数 DCB
		bool Uart_impl::SetState( DCB *pDcb )
		{
			if( !is_open() || pDcb == NULL )
				return -1;

			return SetCommState(_MyUart.hWnd, pDcb) == TRUE ? 1 : -1;
		}

#elif defined( PLATFORM_OS_FAMILY_UNIX )
		//局部优化选项设置
		/*#pragma GCC push_options
		#pragma GCC optimize ("O0")

		#pragma GCC pop_options*/

		const int speed_arr_20170609[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
		const int name_arr_20170609[] =	{ 115200, 38400, 19200, 9600, 4800, 2400, 1200,  300 };

		////////////////////////////////////////////////////////////////////////////////  
		/** 
		*@brief  设置串口通信速率 
		*@param  Handle    类型 int  打开串口的文件句柄 
		*@param  BaudRate  类型 int  串口速度 
		*@return  void 
		*/  
		static int SetSpeed( HANDLE Handle, int BaudRate )
		{
			struct termios   Opt;
			//tcgetattr(Handle, &Opt);
			for ( int i = 0;  i < sizeof(speed_arr_20170609) / sizeof(int);  i ++ )
			{
				if ( BaudRate == name_arr_20170609[i] )
				{ 
					tcflush(Handle, TCIOFLUSH);
					cfsetispeed(&Opt, speed_arr_20170609[i]);
					cfsetospeed(&Opt, speed_arr_20170609[i]);
					if ( tcsetattr(Handle, TCSANOW, &Opt) != 0 )
					{
						perror("Uart SetSpeed tcsetattr");
						return -1;
					}
					tcflush(Handle,TCIOFLUSH);
					break;
				}
			}

			return 1;
		}

		////////////////////////////////////////////////////////////////////////////////
		/**
		*@brief   设置串口数据位，停止位和效验位
		*@param  Handle     类型  int  打开的串口文件句柄
		*@param  DataBits 类型  int 数据位   取值 为 7 或者8
		*@param  StopBits 类型  int 停止位   取值为 1 或者2
		*@param  Parity  类型  int  效验类型 取值为N,E,O,S
		*/
		static int SetParity( HANDLE Handle, int DataBits, int StopBits, int Parity = 'N' )
		{ 
			struct termios options;
			if ( tcgetattr(Handle, &options) !=  0 )
			{ 
				perror("SetParity 1");     
				return -1;  
			}
			options.c_cflag |= CLOCAL | CREAD;
			options.c_cflag &= ~CSIZE; 
			
			// 硬件流控制模式，flow control
			int fctl = '0';
			switch (fctl) 
			{
			case '0':
				{
					options.c_cflag &= ~CRTSCTS;        //no flow control
				}
				break;
			case '1': 
				{
					options.c_cflag |= CRTSCTS;         //hardware flow control
				}
			break;
			case '2': 
				{
					options.c_iflag |= IXON | IXOFF | IXANY; //software flow control
				}
				break;
			}

			switch (DataBits) /*设置数据位数*/
			{   
			case 7:		
				options.c_cflag |= CS7; 
				break;
			case 8:     
				options.c_cflag |= CS8;
				break;   
			default:
				fprintf(stderr,"Unsupported data size\n");
				return -1;  
			}

			switch (Parity) 
			{   
			case 'n':
			case 'N':    
				options.c_cflag &= ~PARENB;   /* Clear parity enable */
				options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
				break;  
			case 'o':   
			case 'O':     
				options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
				options.c_iflag |= INPCK;             /* Disnable parity checking */ 
				break;  
			case 'e':  
			case 'E':   
				options.c_cflag |= PARENB;     /* Enable parity */    
				options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
				options.c_iflag |= INPCK;       /* Disnable parity checking */
				break;
			case 'S': 
			case 's':  /*as no parity*/   
				options.c_cflag &= ~PARENB;
				options.c_cflag &= ~CSTOPB;
				break;  
			default:   
				fprintf(stderr,"Unsupported parity\n");    
				return -1;  
			}  

			/* 设置停止位*/  
			switch (StopBits)
			{   
			case 1:    
				options.c_cflag &= ~CSTOPB;
				break;  
			case 2:    
				options.c_cflag |= CSTOPB;  
				break;
			default:    
				fprintf(stderr,"Unsupported stop bits\n");  
				return -1; 
			} 
			/* Set input parity option */ 
			/*if ( Parity != 'n' ) 
				options.c_iflag |= INPCK; */

			options.c_iflag = 0; //~(IGNCR | INLCR | ICRNL | IUCLC | OLCUC | ISTRIP);
			
			tcflush(Handle,TCIFLUSH);
			options.c_cc[VTIME] = 150; /* 设置超时1 seconds*/   
			options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
			if (tcsetattr(Handle, TCSANOW, &options) != 0)   
			{ 
				perror("SetParity 3");
				return -1;  
			} 
			options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
			options.c_oflag  &= ~OPOST;   /*Output*/
			return 1;  
		}

		HANDLE	Uart_impl::Open( const char* szFile, UInt32 uBaudrate, UInt8 uByteSize,
			UInt8 uStopBits, bool isParityCheck )
		{
			_MyUart.hWnd = open(szFile, O_RDWR | O_NOCTTY | O_NONBLOCK);
			if ( _MyUart.hWnd < 0 ) 
			{  
				printf("Open Uart: %s Failed!!\r\n", szFile);
				return INVALID_HANDLE_VALUE;
			}

			tcgetattr(_MyUart.hWnd, &_old_options);

			if( SetSpeed(_MyUart.hWnd, uBaudrate) == -1 )
			{
				Close();
				return INVALID_HANDLE_VALUE;
			}

			if ( SetParity(_MyUart.hWnd, uByteSize, uStopBits, isParityCheck ? 'N' : 'S') == -1 ) 
			{  
				Close();
				return INVALID_HANDLE_VALUE;
			}

			if( _FileIO.Bind(_MyUart.hWnd) == -1 )
			{//关联失败
				Close();
				return INVALID_HANDLE_VALUE;
			}

			if( PostRecv(ASYNC_UARTS_POOL_SIZE) == -1 )
			{
				Close();
				return INVALID_HANDLE_VALUE;
			}

			return _MyUart.hWnd;
		}
#endif
		void Uart_impl::Close( void )
		{
			if( is_open() )
			{
				HANDLE hWnd = _MyUart.hWnd;
				_MyUart.hWnd = INVALID_HANDLE_VALUE;

#if defined( PLATFORM_OS_FAMILY_UNIX )
				tcsetattr(hWnd, TCSANOW, &_old_options);              //恢复之前的串口配置、
#endif

				_FileIO.close();

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
				if( !CloseHandle(hWnd) )
				{
					int iError = GetLastError();
					iError;
				}
#endif		
				_Lock.Lock();
				_List.clear();
				_isWrite = false;
				_Lock.UnLock();
			}	
		}

		void Uart_impl::SetCallBack( const HFNRead& hRead,
			const HFNNotify& hWrite,
			const HFNDestroy& hDestroy )
		{
			_hRead = hRead;
			_hWrite = hWrite;
			_hDestroy = hDestroy;
		}

		int Uart_impl::Send( const char* szData, UInt32 uSize )
		{
			if( !is_open() )
				return -1;

			UartBuf_sptr Buf(reinterpret_cast<tagUartBuf*>(_Pool.Alloc()), false);

			Buf->uSize = uSize;
			memcpy(Buf->Data, szData, uSize);

			_Lock.Lock();
			if( !_isWrite )
			{
				_isWrite = true;
				_Lock.UnLock();
				if( _FileIO.async_write(Buf->Data, Buf->uSize,
					function20_bind(&Uart_impl::HandleSend,
					shared_from_this(), Buf->Data, Buf->uSize, _function_::_1)) == -1 )
				{
					Close();
				}
			}
			else
			{
				_List.push_back(Buf);
				_Lock.UnLock();
			}

			return 1;	
		}

		void Uart_impl::HandleReceive( tagIOData_INFO* pIOData )
		{
			//tcflush(_MyUart.hWnd, TCIFLUSH);

			if( _hRead && pIOData->_ibytes_transferred > 0 )
			{
				tagUartBuf*	pBuf = (tagUartBuf*)(pIOData->_pOverlapped->_SWSAOverlapp.wsaBuf.buf - (sizeof(tagUartBuf) - 1));
				pBuf->uSize = pIOData->_ibytes_transferred;
				pBuf->Data[pBuf->uSize] = 0;
				_hRead(shared_from_this(), UartBuf_sptr(pBuf,
					function20_bind_Ex(&MemPool_type::FreeObj<tagUartBuf>, &_Pool)));
			}

			if( _MyUart.hWnd != INVALID_HANDLE_VALUE )
			{
				PostRecv(ASYNC_UARTS_POOL_SIZE, pIOData->_pOverlapped);
			}
		}

		int Uart_impl::PostRecv( UInt32 uSize, tagOverlapped* pOverlapped )
		{
			if( !is_open() )
				return -1;

			tagUartBuf*	pBuf;
			uSize = __min(uSize, ASYNC_UARTS_POOL_SIZE);
			pBuf = reinterpret_cast<tagUartBuf*>(_Pool.AllocObj<tagUartBuf>());
			pBuf->uSize = uSize;
			if( pOverlapped == NULL )
			{
				return _FileIO.async_read(pBuf->Data, pBuf->uSize,
					function20_bind(&Uart_impl::HandleReceive,
					shared_from_this(), _function_::_1));
			}

			return _FileIO.async_read(pOverlapped, pBuf->Data, pBuf->uSize);
		}

		void Uart_impl::HandleSend( const char* c_pData, UInt32 u32Size,
									tagIOData_INFO* pIOData )
		{
			//tcflush(_MyUart.hWnd, TCOFLUSH);

			UartBuf_sptr sptr;
		
			_Lock.Lock();
			if( _List.size() == 0 )
			{
				_isWrite = false;
				_Lock.UnLock();
			}
			else
			{
				sptr = _List.front();
				_List.pop();
				_Lock.UnLock();
			}
			
			if( sptr )
			{
				pIOData->_pOverlapped->_SWSAOverlapp.wsaBuf.buf = (char*)sptr->Data;
				pIOData->_pOverlapped->_SWSAOverlapp.wsaBuf.len = sptr->uSize;

				if( _FileIO.async_write(pIOData->_pOverlapped) == -1 )
				{
					Close();
				}
			}
			
			if( _hWrite )
			{
				_hWrite(_MyUart.hWnd, c_pData, pIOData->_ibytes_transferred);
			}
			
			if( c_pData != NULL )
			{
				_Pool.Free( (void*)(c_pData - (sizeof(tagUartBuf) - 1)) ) ;
			}
		}

	}// namespace	_files_

}// namespace	_async_