
#include <libEPoll_Net/connector.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{

		namespace _tcp_{

			connector::connector( void )
			{
	
			}

			connector::connector( const io_service& service )
				: _service(service)
			{

			}

			connector::~connector( void )
			{
				close();
				reset_function();
			}

			int connector::attach( const io_service& service )
			{
				_service = service;
				return 1;
			}

			int connector::listen( void )
			{
				return 1;
			}

			int connector::async_connect( const char* szIP, UInt16 uPort, int iSocketRecv, int iSocketSend )
			{
				if( !_f )
					return -1;

				SOCKET s = APIWSACreate(CMyInitSock::SOCKET_TCP_TYPE, iSocketRecv, iSocketSend);
				if( s != INVALID_SOCKET )
				{
					tagOverlapped* p = _OverlappedManage.Alloc();
					p->_hfnComplete = _f;
					p->_Free_Overlapp = function1_handle(&connector::FreeOverlapped, this);
					p->_ADR._Sock = s;
					p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_CONNECT;
				
					tagIOData_INFO	IODataINFO;
					if( APIWSAConnect(s, szIP, uPort) != SOCKET_ERROR )
					{
						p->_RealSize = 0;
						_service.post(p, 0);
					}
					else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == EINPROGRESS) )
					{
						p->_hfnPerform = function20_bind(&connector::ConnectCallBack, this, p,
							_function_::_1, _function_::_2, _function_::_3);

						if( _service.post(s, EPOLL_CTL_ADD, EPOLLONESHOT | EPOLLOUT | EPOLLERR | EPOLLET, p) == -1 )
						{
							goto gt_failed;
						}
					}
					else
					{
	gt_failed:
						APIWSAClose(s);
					
						p->_RealSize = 0;
						p->_ADR._Sock = INVALID_SOCKET;
						_service.post(p, 0);
						return -1;
					}
				}

				return 1;
			}

			int connector::async_connect( const char* szIP, UInt16 uPort,
										const fn_connect_handle& handle,
										int iSocketRecv, int iSocketSend )
			{
				SOCKET s = APIWSACreate(CMyInitSock::SOCKET_TCP_TYPE, iSocketRecv, iSocketSend);
				if( s != INVALID_SOCKET )
				{
					tagIOData_INFO	IODataINFO;
					tagOverlapped* p = _OverlappedManage.Alloc();
					p->_hfnComplete = function20_bind(&connector::connect_handle, this, handle, _foundation_::_1 );
					p->_Free_Overlapp = function1_handle(&connector::FreeOverlapped, this);
					p->_ADR._Sock = s;
					p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_CONNECT;

					if( APIWSAConnect(s, szIP, uPort) != SOCKET_ERROR )
					{
						//test
						p->_RealSize = 0;
						_service.post(p, 0);
						//test
					
						/*IODataINFO._ibytes_transferred = 0;
						IODataINFO._pOverlapped = p;
						p->_isPerform = false;
						_service.process(&IODataINFO, p);*/
					}
					else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == EINPROGRESS) )
					{
						p->_hfnPerform = function20_bind(&connector::ConnectCallBack, this, p,
													_function_::_1, _function_::_2, _function_::_3);
						//p->_isPerform = true;
						if( _service.post(s, EPOLL_CTL_ADD, EPOLLONESHOT | EPOLLOUT | EPOLLERR | EPOLLET, p) == -1 )
						{
							goto gt_failed;
						}
					}
					else
					{
	gt_failed:
						APIWSAClose(s);
					
						//test
						p->_RealSize = 0;
						p->_ADR._Sock = INVALID_SOCKET;
						_service.post(p, 0);
						//test
					
						/*IODataINFO._ibytes_transferred = 0;
						IODataINFO._pOverlapped = p;
						IODataINFO._pOverlapped->_ADR._Sock = INVALID_SOCKET;
						p->_isPerform = false;
						_service.process(&IODataINFO, p);*/
						return -1;
					}
				}

				return 1;
			}

			int connector::ConnectCallBack( tagOverlapped* pOverlapped,
				SOCKET s, char* c_szData, unsigned long uSize )
			{
				if( pOverlapped->_SWSAOverlapp.wsaOverlapped.uEvent & EPOLLERR )
				{//连接服务器失败
					APIWSAClose(pOverlapped->_ADR._Sock);
					pOverlapped->_ADR._Sock = INVALID_SOCKET;
				}
				else if( pOverlapped->_SWSAOverlapp.wsaOverlapped.uEvent & EPOLLOUT )
				{

				}
				else
				{
					LOG_Print_Info(libEPoll_Net, "connector::ConnectCallBack unknow Event:%d, errno:%d!!!\r\n", pOverlapped->_SWSAOverlapp.wsaOverlapped.uEvent, errno)
				}

				LOG_Print_Info(libEPoll_Net, "connector::ConnectCallBack Socket:%d\r\n", pOverlapped->_ADR._Sock)
				return 1;
			}

			void connector::connect_handle( const fn_connect_handle& f, tagIOData_INFO* pIOData )
			{
				tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;
				if( pOver->_ADR._Sock != INVALID_SOCKET )
					_service.bind((HANDLE)pOver->_ADR._Sock);

				((fn_connect_handle)f)(pIOData);
			}

			void connector::close( void )
			{
			
			}

		} //namespace _tcp_

	} //_iocp_net_

#endif

