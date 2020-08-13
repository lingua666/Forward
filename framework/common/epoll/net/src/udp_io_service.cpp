
#include <libEPoll_Net/CAsynIO.h>
#include <libEPoll_Net/udp_io_service.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )
	#include <signal.h>
	
	#define SIG_STOP_EPOLL_WORK    __SIGRTMIN + 10  //SIGUSR1

	namespace _epoll_net_{

		namespace _udp_{

			io_service::io_service( void )
				: _is_run(false)
				, _io_handle(NULL)
				, _ThreadList( new LList() )
				, _List( new OPList() )
				, _Lock( new CLock() )
				, _FreeLock( new CLock() )
				, _OverlappedManage( new OverlappedManage_type() )
			{	

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

			}

			io_service& io_service::operator=( const self_type& self )
			{
				_io_handle = self._io_handle;
				_is_run = self._is_run;
				_ThreadList = self._ThreadList;
				_List = self._List;
				_Lock = self._Lock;
				_FreeLock = self._FreeLock;
				_OverlappedManage = self._OverlappedManage;
				return *this;
			}

			int io_service::open( void )
			{
				_io_handle = CAsynIO::CreateAsynIO();
				if( _io_handle == NULL )
				{
					LOG_Print_Error(libEPoll_Net, "io_service Create Error!");
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
				return 1;
			}

			int io_service::post_read( HANDLE Handle, tagOverlapped* pOverlapped )
			{
				return CAsynIO::PostRead(_io_handle, Handle, (UInt64)pOverlapped);
			}

			int io_service::post_write( HANDLE Handle, tagOverlapped* pOverlapped )
			{
				return CAsynIO::PostWrite(_io_handle, Handle, (UInt64)pOverlapped);
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
				_FreeLock->Lock();
				if( pOverlapped->_hfnComplete )
					pOverlapped->_hfnComplete.reset();
				else
				{
					printf("exit failed io_service::CloseOverlapped pOverlapped:%p has free\r\n", pOverlapped);
					_exit(0);
				}
				_FreeLock->UnLock();
				this->post(pOverlapped, -1, 0);
			}

			int io_service::post( tagOverlapped* pOverlapped, int iType, UInt8 u8Priority )
			{
				_Lock->Lock();
				_List->push_back(pOverlapped, iType, u8Priority);
				_Lock->UnLock();
				return 1;
			}

			int io_service::Immediate_post( tagOverlapped* pOverlapped )
			{
				if( this->is_open() )
				{
					_Lock->Lock();
					_List->push_back(pOverlapped, 0);
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
				int			iRet = 0, iType = 0;
				tagOverlapped*	lpOverlapped = NULL;
				tagOverlapped*	lpTmpOver = NULL;
				tagIOData_INFO	IODataINFO = {0};
				UInt32	uEvent, uOP;
				struct epoll_event events[128];

				//设置信号捕捉
				struct sigaction act;
				act.sa_handler = EpollProcExit;
				sigemptyset(&act.sa_mask);
				act.sa_flags = 0;
				if(sigaction(SIG_STOP_EPOLL_WORK, &act, 0) != 0)
				{
					LOG_Print_Error(libEPoll_Net, "io_service::run sigaction fail");
					return ;
				} 

				_ThreadList->push_back(_thread_::ThreadAPI::GetCurrentThreadId());

				tagOverlapped* pOver = _OverlappedManage->Alloc();
				lpTmpOver = pOver;
				pOver->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WAIT;

				post(pOver, 0);
				_is_run = true;

				while( _is_run )
				{
					pOver = NULL;
					_Lock->Lock();
					if( _List->size() > 0 )
					{
						pOver = _List->front_f();
						iType = _List->front_s();					
						_List->pop();
					}
					_Lock->UnLock();

					if( pOver != NULL )
					{
						switch( pOver->_SWSAOverlapp.wsaOverlapped.uOP )
						{
						case IO_OP_READ:
							{				
								process(&IODataINFO, pOver, pOver->_SWSAOverlapp.wsaOverlapped.uEvent);
								if( iType != -1 )
									CASSubAndFetch(&pOver->_ProcRef);

								if( pOver->_ProcRef < 0 && pOver->_hfnComplete )
								{
									printf("io_service::process vvvvv lpOverlapped:%p,lpOverlapped->_ProcRef:%d, WuOP:%d, uOP:%d, p->_ADR._Sock:%d\r\n", pOver,pOver->_ProcRef,
										pOver->_SWSAOverlapp.wsaOverlapped.uOP, uOP, pOver->_ADR._Sock);
									_exit(0);
								}

								if( iType == -1 )
								{
									if( !pOver->_hfnComplete )
									{//socket已经关闭
										if( pOver->_ProcRef <= 0 )
										{
											if( pOver->_ProcRef < 0 )
											{
												printf("io_service::process yyyyy lpOverlapped:%p,lpOverlapped->_ProcRef:%d, WuOP:%d, p->_ADR._Sock:%d\r\n", pOver, pOver->_ProcRef,
													pOver->_SWSAOverlapp.wsaOverlapped.uOP, pOver->_ADR._Sock);
												_exit(0);
											}

											if( pOver->_Free_Overlapp )
											{	
												pOver->_Free_Overlapp(pOver);
											}
										}
										else
										{
											post(pOver, -1, 0);
										}
									}
								}
							}
							break;
						case IO_OP_ACCEPT:
						case IO_OP_CONNECT:
						case IO_OP_WRITE:
							{							
								process(&IODataINFO, pOver, pOver->_SWSAOverlapp.wsaOverlapped.uEvent);
							}
							break;
						case IO_OP_WAIT:
							{								
								iRet = CAsynIO::GetQueuedCompletionStatus(_io_handle, events, 128);
								// Dispatch the waiting events.
								if( iRet != -1 )
								{
									for( int i = 0; i < iRet; ++ i )
									{
										uEvent = events[i].events;
										lpOverlapped = (tagOverlapped*)events[i].data.ptr;
										if( lpOverlapped != NULL )
										{
											uOP = lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP;
											lpOverlapped->_SWSAOverlapp.wsaOverlapped.uEvent = uEvent;

											if( uOP == IO_OP_ACCEPT ||
												lpOverlapped->_ADR._Sock != INVALID_SOCKET )
											{
												if( uOP == IO_OP_READ )
												{
													CASAddAndFetch(&lpOverlapped->_ProcRef);
													tagOverlapped::AddRef(lpOverlapped);
												}

												post(lpOverlapped, 0, 0);
											}
										}
									}

									post(pOver, 0, 0);
								}
								else
								{//出现错误
									if( errno == EINTR && this->is_open() )
									{
										post(lpTmpOver, 0, 0);
									}
									else
									{
										LOG_Print_Info(libEPoll_Net, "io_service::run GetQueuedCompletionStatus failed, error code:%d!!", errno)
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

				printf("thread exit threadid:%lld\r\n", _thread_::ThreadAPI::GetCurrentThreadId());
				if( lpTmpOver != NULL )
					_OverlappedManage->Free(lpTmpOver);
			}

			void io_service::process( tagIOData_INFO* pInfo, tagOverlapped* lpOverlapped, UInt32 Event )
			{
				if( lpOverlapped != NULL )
				{					
					HFNComplete hComplete;
					HFNPerform	hPerform;

					UInt32 uOP = lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP;
					pInfo->_pOverlapped = lpOverlapped;

					if( uOP == IO_OP_READ )
					{
						_FreeLock->Lock();
						hComplete = lpOverlapped->_hfnComplete;
						_FreeLock->UnLock();
					}
					else
					{
						hComplete = lpOverlapped->_hfnComplete;
					}


					if( hComplete )
					{
						if( lpOverlapped->_RealSize == -1 && lpOverlapped->_hfnPerform )
						{
							lpOverlapped->_RealSize = lpOverlapped->_hfnPerform(lpOverlapped->_ADR._Sock,
								lpOverlapped->_SWSAOverlapp.wsaBuf.buf,
								lpOverlapped->_SWSAOverlapp.wsaBuf.len,
								(struct sockaddr_in*)lpOverlapped->_pAddr);
							if( lpOverlapped->_RealSize < 0 && errno != EAGAIN )
							{//异常
								lpOverlapped->_RealSize = 0;
							}

							pInfo->_pOverlapped = lpOverlapped;
						}

						pInfo->_ibytes_transferred = lpOverlapped->_RealSize;

						hComplete(pInfo);

						if( tagOverlapped::SubRef(lpOverlapped) <= 0 && lpOverlapped->_Free_Overlapp )
						{//释放		
							//test
							if( lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP == IO_OP_READ )
							{
								printf("lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP == IO_OP_READ :%p,%d\r\n", lpOverlapped, lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP);
								_exit(0);
							}

							if( lpOverlapped->_SWSAOverlapp.wsaOverlapped.uOP == IO_OP_ACCEPT )
							{
								printf("exit failed io_service::process IO_OP_ACCEPT Free111\r\n");
								_exit(0);
							}
							//test

							lpOverlapped->_Free_Overlapp(lpOverlapped);
						}
					}
				}
				//test
				else
				{
					printf("failed io_service::process lpOverlapped is NULL\r\n");
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

			int	io_service::bind( HSOCKET sock )
			{
				if( sock == INVALID_SOCKET )
					return -1;

				if( CAsynIO::BindHandle(_io_handle, sock) == -1 )
				{
					return -1;
				}
				return 1;
			}

		} //namespace _tcp_

	} // _iocp_net_

#endif

