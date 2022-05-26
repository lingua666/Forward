
#include <libEPoll_Regular_File/CAsynIO.h>
#include <libEPoll_Regular_File/io_service.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )
	#include <signal.h>
	
	#define SIG_STOP_EPOLL_WORK    __SIGRTMIN + 11  //SIGUSR1

	namespace _epoll_regular_file_{

		typedef _sys_error_					error_code;

		io_service::io_service( void )
			: _is_run(false)
			, _io_handle(NULL)
			, _ThreadList( new LList() )
			, _List( new OPList() )
			, _Lock( new CLock() )
			, _FreeLock( new CLock() )
			, _OverlappedManage( new OverlappedManage_type() )
		{	
			//test
			LOG_Print_SetLog(libEPoll_Regular_File, MLog_GetAllLevel());
			//test
		}

		io_service::io_service( const self_type& self )
			: _io_handle(self._io_handle)
			, _is_run(self._is_run)
			, _ThreadList(self._ThreadList)
			, _List(self._List)
			, _Lock(self._Lock)
			, _FreeLock(self._FreeLock)
			, _OverlappedManage(self._OverlappedManage)
		{		

		}

		io_service::~io_service( void )
		{
			//stop();
		}

		io_service& io_service::operator=( const self_type& self )
		{
			_io_handle = self._io_handle;
			return *this;
		}

		int io_service::Init(void)
		{
			return 1;
		}

		void io_service::Release(void)
		{}

		int io_service::open( void )
		{
			_io_handle = CAsynIO::CreateAsynIO();
			if( _io_handle == NULL )
			{
				LOG_Print_Error(libEPoll_Regular_File, "io_service Create Error!");
				return -1;
			}

			if( GetCAIOInstance()->Init() == -1 )
			{
				close();
				return -1;
			}
			
			if( bind(GetCAIOInstance()->GetHandle()) == -1 )
			{
				close();
				return -1;
			}

			return 1;
		}

		int io_service::close( void )
		{
			if( _io_handle == NULL )
				return -1;

			IOHANDLE h = _io_handle;
			_io_handle = NULL;
			while(_ThreadList->size() > 0)
			{
				UInt64 uThreadID = _ThreadList->front();
				_ThreadList->pop();
				pthread_kill(uThreadID, SIG_STOP_EPOLL_WORK);
			}

			CAsynIO::CloseAsynIO(&h);

			GetCAIOInstance()->Release();
			return 1;
		}

		int io_service::post_read( HANDLE Handle, tagOverlapped* pOverlapped )
		{
			struct iocb      *piocb[1];
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_data = (uint64_t) (uintptr_t) pOverlapped;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_lio_opcode = IOCB_CMD_PREAD;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_fildes = pOverlapped->_ADR._Handle;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_buf = (uint64_t) (uintptr_t) pOverlapped->_SWSAOverlapp.wsaBuf.buf;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = pOverlapped->_SWSAOverlapp.wsaBuf.len;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_offset = pOverlapped->_SWSAOverlapp.wsaOverlapped.uOffset;  
			/***当IOCB_FLAG_RESFD标识被设置时就使用aio->aiocb.aio_resfd变量中的描述符中通知用户态I/O事件已完成***/
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_flags = IOCB_FLAG_RESFD;
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_resfd = GetCAIOInstance()->GetHandle();

			//提交I/O事件 
			piocb[0] = &pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb;
			return GetCAIOInstance()->Submit(1, piocb);
		}

		int io_service::post_write( HANDLE Handle, tagOverlapped* pOverlapped )
		{
			struct iocb      *piocb[1];
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_data = (uint64_t) (uintptr_t) pOverlapped;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_lio_opcode = IOCB_CMD_PWRITE;
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_fildes = pOverlapped->_ADR._Handle;
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_buf = (uint64_t) (uintptr_t) pOverlapped->_SWSAOverlapp.wsaBuf.buf;  
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = pOverlapped->_SWSAOverlapp.wsaBuf.len;
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_offset = pOverlapped->_SWSAOverlapp.wsaOverlapped.uOffset;
			/***当IOCB_FLAG_RESFD标识被设置时就使用aio->aiocb.aio_resfd变量中的描述符中通知用户态I/O事件已完成***/
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_flags = IOCB_FLAG_RESFD;
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_resfd = GetCAIOInstance()->GetHandle();

			//提交I/O事件  
			piocb[0] = &pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb;
			return GetCAIOInstance()->Submit(1, piocb);
		}

		int io_service::post_cancel( tagOverlapped* pOverlapped )
		{
			struct io_event result;

			//取消I/O事件 
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = pOverlapped->_SWSAOverlapp.wsaBuf.len;
			pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_resfd = GetCAIOInstance()->GetHandle();
			return GetCAIOInstance()->Cancel(&pOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb, &result);
		}

		int io_service::post_delete( HANDLE Handle )
		{
			return CAsynIO::DettachHandle(_io_handle, Handle);
		}

		int io_service::post( HANDLE Handle, int iType, int iValue, tagOverlapped* pOverlapped )
		{
			return CAsynIO::PostIO(_io_handle, Handle, iType, iValue, (UInt64)pOverlapped);
		}

		tagOverlapped* io_service::Alloc( void )
		{
			return _OverlappedManage->Alloc();
		}

		void io_service::Free( tagOverlapped* pOverlapped )
		{
			FreeOverlapped(pOverlapped);
		}

		void io_service::CloseOverlapped( tagOverlapped* pOverlapped )
		{
			pOverlapped->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_CLOSE;

			_FreeLock->Lock();
			if( pOverlapped->_hfnComplete )
				pOverlapped->_hfnComplete.reset();
			else
			{
				LOG_Print_Error(libEPoll_Regular_File, "error exit failed io_service::CloseOverlapped pOverlapped:%p has free\r\n", pOverlapped);
				_exit(0);
			}
			_FreeLock->UnLock();
			
			tagOverlapped::AddRef(pOverlapped);
			this->post(pOverlapped);
		}

		int io_service::post( tagOverlapped* pOverlapped )
		{
			_Lock->Lock();
			_List->push_back(pOverlapped);
			_Lock->UnLock();
			return 1;
		}

		int io_service::Immediate_post( tagOverlapped* pOverlapped )
		{
			if( this->is_open() )
			{
				_Lock->Lock();
				_List->push_back(pOverlapped);
				_Lock->UnLock();
				for(int i = 0; i < _ThreadList->size(); i ++)
				{
					UInt64 ThreadID = (*_ThreadList.get())[i];
					if(	ThreadID != _thread_::ThreadAPI::GetCurrentThreadId() )
					{
						int iRet = 0;
						iRet = pthread_kill(ThreadID, SIG_STOP_EPOLL_WORK);
						break;
					}
				}
			}
		}

		//信号捕捉回调函数
		static void EpollProcExit(int sig)
		{

		}

		void io_service::run( void )
		{
			int			iRet = 0, iNum = 0, iEvNum = 0;
			UInt64		uReady = 0;
			UInt32	uEvent, uOP;
			tagOverlapped*	lpOverlapped = NULL;
			tagOverlapped*	lpTmpOver = NULL;
			tagIOData_INFO	IODataINFO = {0};
			struct epoll_event events[REGULAR_FILE_EVENTS_MAX];
			struct io_event   io_ev[REGULAR_FILE_EVENTS_MAX];

			//设置信号捕捉
			struct sigaction act;
			act.sa_handler = EpollProcExit;
			sigemptyset(&act.sa_mask);
			act.sa_flags = 0;
			if( sigaction(SIG_STOP_EPOLL_WORK,&act,0) != 0 )
			{
				LOG_Print_Error(libEPoll_Regular_File, "io_service::run sigaction fail");
				return ;
			} 

			_ThreadList->push_back(_thread_::ThreadAPI::GetCurrentThreadId());

			tagOverlapped* pOver = _OverlappedManage->Alloc();
			lpTmpOver = pOver;
			pOver->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WAIT;

			post(pOver);
			_is_run = true;

			while( _is_run )
			{
				pOver = NULL;
				_Lock->Lock();
				if( _List->size() > 0 )
				{
					pOver = _List->front();
					_List->pop();
				}
				_Lock->UnLock();

				if( pOver != NULL )
				{
					switch( pOver->_SWSAOverlapp.wsaOverlapped.uOP )
					{
					case IO_OP_CLOSE:
						{
							if (pOver->_ProcRef <= 0)
							{
								if (pOver->_ProcRef < 0)
								{
									LOG_Print_Error(libEPoll_Regular_File, "error io_service::process yyyyy lpOverlapped:%p,lpOverlapped->_ProcRef:%d, WuOP:%d\r\n", pOver, pOver->_ProcRef,
										pOver->_SWSAOverlapp.wsaOverlapped.uOP);
									_exit(0);
								}

								if (pOver->_Free_Overlapp)
								{
									pOver->_Free_Overlapp(pOver);
								}
							}
							else
							{
								post(pOver);
							}	
						}
						break;
					case IO_OP_READ:
					case IO_OP_WRITE:
						{
							IODataINFO._ibytes_transferred = pOver->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes;
							IODataINFO._pOverlapped = pOver;
							CASSubAndFetch(&pOver->_ProcRef);
							if(pOver->_ProcRef < 0)
							{
								LOG_Print_Error(libEPoll_Regular_File, "error Write And Read 111 pOver:%p, uOP:%d, Handle:%lld, offset:%lld", pOver, pOver->_SWSAOverlapp.wsaOverlapped.uOP,  pOver->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_fildes, pOver->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_offset);
								_exit(0);
							}

							process(&IODataINFO, pOver, pOver->_SWSAOverlapp.wsaOverlapped.uEvent);

						}
						break;
					case IO_OP_WAIT:
						{
							iRet = CAsynIO::GetQueuedCompletionStatus(_io_handle, events, REGULAR_FILE_EVENTS_MAX);
							// Dispatch the waiting events.
							if( iRet != -1 )
							{
								for( int i = 0; i < iRet; ++ i )
								{
									uEvent = events[i].events;

									// 开始读取完成的事件，并将完成的 数目 设置到ready 中，这个ready可以大于64
									uReady = GetCAIOInstance()->ReadEventNum();
									if( uReady != -1 )
									{
										for(int j = 0; j < uReady; j ++)
										{
											iEvNum = GetCAIOInstance()->GetEvents(io_ev);

											for(int k = 0; k < iEvNum; k ++)
											{
												lpOverlapped = (tagOverlapped *) (uintptr_t) io_ev[k].data;

												if( uEvent != 0 && lpOverlapped != NULL )
												{
													lpOverlapped->_SWSAOverlapp.wsaOverlapped.Aiocb.aio_nbytes = io_ev[k].res;
													uOP = lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP;
													lpOverlapped->_SWSAOverlapp.wsaOverlapped.uEvent = uEvent;


													if( lpOverlapped->_ADR._Handle != INVALID_HANDLE_VALUE )
													{
														CASAddAndFetch(&lpOverlapped->_ProcRef);
														post(lpOverlapped);
													}
													else
													{
														LOG_Print_Error(libEPoll_Regular_File, "error lpOverlapped->_ADR._Handle == INVALID_HANDLE_VALUE\r\n");
														_exit(0);
													}
												}

											}
										}
									}
								}

								post(pOver);
							}
							else
							{//出现错误
								if( errno == EINTR && this->is_open() )
								{
									LOG_Print_Error(libEPoll_Regular_File, "io_service::run GetQueuedCompletionStatus(%p) failed, error code:%d!!", lpTmpOver, errno)
									post(lpTmpOver);
								}
								else
								{
									LOG_Print_Error(libEPoll_Regular_File, "io_service::run GetQueuedCompletionStatus failed, error code:%d!!", errno)
									break;//退出线程
								}
							}
						}
						break;
					case IO_OP_USER:

						break;
					}
				}
				else
				{//延时
					Sleep(1);
				}
			}

			printf("thread exit threadid(%d):%lld\r\n", _is_run, _thread_::ThreadAPI::GetCurrentThreadId());
			if( lpTmpOver != NULL )
				_OverlappedManage->Free(lpTmpOver);
		}

		void io_service::process( tagIOData_INFO* pInfo, tagOverlapped* lpOverlapped, UInt32 Event )
		{
			if( lpOverlapped != NULL )
			{
				HFNComplete hComplete;

				if( lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP == IO_OP_READ )
				{
					_FreeLock->Lock();
					hComplete = lpOverlapped->_hfnComplete;
					_FreeLock->UnLock();
				}
				else
					hComplete = lpOverlapped->_hfnComplete;

				if( hComplete )
				{
					hComplete(pInfo);
					if( (lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP != IO_OP_READ ||
						lpOverlapped->_ADR._Handle != INVALID_HANDLE_VALUE) && lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP != IO_OP_CLOSE )
					{
						if( tagOverlapped::SubRef(lpOverlapped) <= 0 && lpOverlapped->_Free_Overlapp )
						{//释放
							if(lpOverlapped->_Ref < 0)
							{
								LOG_Print_Error(libEPoll_Regular_File, "error failed io_service::process33333 lpOverlapped Failed\r\n");
								_exit(0);	
							}

							lpOverlapped->_Free_Overlapp(lpOverlapped);
						}	
					}
				}
			}
			//test
			else
			{
				LOG_Print_Error(libEPoll_Regular_File, "failed io_service::process lpOverlapped is NULL\r\n");
				_exit(0);
			}
			//test
		}

		int io_service::stop(void)
		{
			if( _is_run )
				_is_run = false;
			return 1;
		}

		int	io_service::bind( HFILE Handle )
		{
			if( Handle == INVALID_HANDLE_VALUE )
				return -1;

			if( CAsynIO::BindHandle(_io_handle, Handle) == NULL )
			{
				return -1;
			}
			return 1;
		}

	} // _iocp_net_

#endif

