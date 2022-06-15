
#include <libFrameWork_AsyncUarts/AsyncUarts.h>

namespace	_async_{

	namespace	_uarts_{

		AsyncUarts* GetAsyncUartsInstance(void)
		{
			return Singleton<AsyncUarts>::instance();
		}

		void AsyncUarts::WorkThread( void* pParamter )
		{
			AsyncUarts* pUart = reinterpret_cast<AsyncUarts*>(pParamter);
			while(true)
			{
				try
				{
					ThreadWrap::interruption_point();
					if( pUart->_DataList.size() > 0 )
					{
						pUart->_ReadLock.Lock();
						if( pUart->_DataList.size() > 0 )
						{
							HFNData hData = pUart->_DataList.front();
							pUart->_DataList.pop();
							pUart->_ReadLock.UnLock();
							if( hData )
							{
								hData();
							}
						}
						else
						{
							pUart->_ReadLock.UnLock();
						}
					}
					else
					{
						Sleep(10);
					}
				}
				catch (const thread_interrupted& e)
				{
					switch( e.get_code() )
					{
					case ThreadData::en_INTERRUPTED_EXIT:	//线程退出
						goto exit;
						break;
					}
					break;
				}
			}
exit:
			return ;
		}

		AsyncUarts::AsyncUarts( void )
			: _Pool( sizeof(Uart_impl) )
		{

		}

		AsyncUarts::~AsyncUarts( void )
		{
			Release();
		}

		//IO底层最少两个线程(在IC2000系统里面一个线程会出现接收的数据乱码问题)
		int AsyncUarts::Init( UInt8 uThreadNum, UInt8 uProcNum )
		{
			if( _ioService.open() == -1 )
				return -1;

			if( _Threads.size() <= 0 )
			{
				uThreadNum = __max(uThreadNum, 2);
				for(std::size_t i = 0; i < uThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type(
						function20_bind(&io_service::run, &_ioService)) );
					_Threads.push_back(thread);
					thread->wait_run();
				}
			}

			if( _Workers.size() <= 0 )
			{
				for(std::size_t i = 0; i < uProcNum; ++ i)
				{
					Thread_ptr thread( new Thread_type(
						function20_bind(&AsyncUarts::WorkThread, this)) );
					_Workers.push_back(thread);
					thread->wait_run();
				}
			}
			return 1;
		}

		void AsyncUarts::SetCallBack( THandle Handle, 
				const HFNRead& hRead,
				const HFNDestroy& hDestroy,
				const HFNWrite& hWrite )
		{
			UartImpl_ptr sptr = FindUartImpl( Handle );
			if( sptr )
			{
				sptr->SetCallBack(function20_bind(&AsyncUarts::HandleReceive, this,
					hRead, _function_::_1, _function_::_2),
					function20_bind(&AsyncUarts::HandleSend, this, hWrite,
					_function_::_1, _function_::_2, _function_::_3),
					function20_bind(&AsyncUarts::HandleDestroy, this, hDestroy, Handle));
			}
		}

		void AsyncUarts::Release( void )
		{
			_ioService.stop();
			_ioService.close();
			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
				_Threads[i]->join();
			}
			_Threads.clear();

			for(std::size_t i = 0; i < _Workers.size(); ++ i)
			{
				_Workers[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
				_Workers[i]->join();
			}
			_Workers.clear();
		}

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		THandle AsyncUarts::Open( const char* ComName, UInt32 uBaudrate, UInt8 ByteSize, UInt8 StopBit, bool isParity )
		{
			if(strlen(ComName) <= 3)
			{
				printf("AsyncUarts::Open(%s) Failed!!\r\n", ComName);
				return -1;
			}

			UartImpl_ptr sptr( _Pool.AllocObj<Uart_impl>( io_service::io_service_sptr(&_ioService, false)),
				function20_bind_Ex(&MemPool_type::FreeObj<Uart_impl>,
				&_Pool) );

			HANDLE hFile = sptr->Open(_string_type::StringToInt(&ComName[3]), uBaudrate, ByteSize,
									StopBit, isParity);
			if( hFile == INVALID_HANDLE_VALUE )
			{
				printf("AsyncUarts::Open() Failed!!\r\n");
				return -1;
			}

			_Lock.Lock();
			_Map.insert(std::make_pair((THandle)hFile, sptr));
			_Lock.UnLock();
			return (THandle)hFile;
		}
#elif defined( PLATFORM_OS_FAMILY_UNIX )
		THandle AsyncUarts::Open( const char* szFile, UInt32 uBaudrate, UInt8 ByteSize, UInt8 StopBit, bool isParity )
		{
			UartImpl_ptr sptr( _Pool.AllocObj<Uart_impl>( io_service::io_service_sptr(&_ioService, false)),
				function20_bind_Ex(&MemPool_type::FreeObj<Uart_impl>,
				&_Pool) );

			HANDLE hFile = sptr->Open(szFile, uBaudrate, ByteSize, StopBit, isParity);
			if( hFile == INVALID_HANDLE_VALUE )
				return -1;

			_Lock.Lock();
			_Map.insert(std::make_pair((THandle)hFile, sptr));
			_Lock.UnLock();
			return (THandle)hFile;
		}
#endif

		int	AsyncUarts::Close( THandle Handle )
		{
			UartImpl_ptr sptr;
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

		void AsyncUarts::CloseAll( void )
		{
			while(_Map.size() > 0 )
			{
				Close(_Map.begin()->first);
			}

			_Map.clear();
		}

		int AsyncUarts::Send( THandle Handle, const char* szData, UInt32 uSize )
		{
			UartImpl_ptr sptr = FindUartImpl( Handle );
			if( !sptr )
				return -1;

			return sptr->Send(szData, uSize);
		}

		AsyncUarts::UartImpl_ptr  AsyncUarts::FindUartImpl( THandle Handle )
		{
			_Lock.Lock();
			HashMap_type::iterator	iter = _Map.find(Handle);
			if( iter != _Map.end() )
			{
				UartImpl_ptr sptr = iter->second;
				_Lock.UnLock();
				return sptr;
			}
			_Lock.UnLock();
			return UartImpl_ptr();	
		}

		void AsyncUarts::HandleReceive( const HFNRead& hRead,
								const UartImpl_ptr& UartImpl_sptr,
								const UartBuf_sptr& Buf_sptr )
		{
			if( hRead )
			{
				_ReadLock.Lock();
				_DataList.push_back(function20_bind(&AsyncUarts::ProcessData, hRead,
					UartImpl_sptr, Buf_sptr));
				_ReadLock.UnLock();
			}
		}

		void AsyncUarts::ProcessData( const HFNRead& hRead,
								const UartImpl_ptr& UartImpl_sptr,
								const UartBuf_sptr& Buf_sptr )
		{
			if( hRead )
			{
				hRead((THandle)UartImpl_sptr->GetHandle(), Buf_sptr->Data, Buf_sptr->uSize);
			}
		}

		void AsyncUarts::HandleSend( const HFNWrite& hWrite, HANDLE Handle,
								const char* szBuf, UInt32 uSize )
		{
			if( hWrite )
			{
				hWrite((THandle)Handle, uSize);
			}
		}

		void AsyncUarts::HandleDestroy( const HFNDestroy& hDestroy, THandle Handle )
		{
			if( hDestroy )
				hDestroy(Handle);
		}

	} //namespace	_uarts_

} //namespace	_async_