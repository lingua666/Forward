
#include <libEPoll_Net/acceptor.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{

	#define	LISTEN_NUM		32

		namespace _tcp_{

			acceptor::acceptor( void )
				: _Sock(INVALID_SOCKET)
			{
			
			}

			acceptor::acceptor( const io_service& service )
				: _service(service)
				, _Sock(INVALID_SOCKET)
			{
			
			}

			acceptor::~acceptor( void )
			{
				close();
				reset_function();
			}

			int acceptor::attach( const io_service& service )
			{
				_service = service;
				return 1;
			}

			int acceptor::bind( UInt16 u16Port, const char* c_szIP )
			{
				_Sock = APIWSACreate(CMyInitSock::SOCKET_TCP_TYPE, -1, 65535);
				if( _Sock == INVALID_SOCKET )
					return -1;

				if( APIWSABind(_Sock, u16Port, c_szIP) == SOCKET_ERROR )
				{
					APIWSAClose(_Sock);
					_Sock = INVALID_SOCKET;
					return -1;
				}

				int i_val = 1;
				if( APIWSASetSockopt(_Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i_val, sizeof(i_val)) == SOCKET_ERROR )
				{
					APIWSAClose( _Sock );
					return INVALID_SOCKET;
				}
				return 1;
			}

			int acceptor::listen( void )
			{
				if( !is_open() )
					return -1;

				if(APIWSAListen(_Sock, SOMAXCONN) == SOCKET_ERROR)
				{
					APIWSAClose(_Sock);
					_Sock = INVALID_SOCKET;
					return -1;
				}

				_service.bind((HANDLE)_Sock);
				return 1;
			}

			int acceptor::async_accept( void )
			{
				if( !_f )
					return -1;

				tagOverlapped* p = _OverlappedManage.Alloc();
				p->_hfnComplete = _f;
				p->_Free_Overlapp = function1_handle(&acceptor::FreeOverlapped, this);
				p->_hfnPerform = function20_bind(&acceptor::AcceptCallBack, this, p,
											_function_::_1, _function_::_2, _function_::_3);
										
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_ACCEPT;
				struct sockaddr_in in_addr = {0};
				SOCKET s = APIWSAAccept(_Sock, &in_addr);
				if( s >= 0 )
				{		
					//test
					p->_RealSize = 0;
					p->_ADR._Sock = s;
					_service.post(p, 0);
					//test
				
					/*tagIOData_INFO	IODataINFO;
					IODataINFO._ibytes_transferred = 0;
					IODataINFO._pOverlapped = p;
					p->_ADR._Sock = s;
					p->_isPerform = false;
					_service.process(&IODataINFO, p);*/
				}
				else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) )
				{
					LOG_Print_Info(libEPoll_Net, "acceptor::async_accept post\r\n")

					//p->_isPerform = true;
					if( _service.post_read(_Sock, p) == -1 )
					{
						_OverlappedManage.Free(p);
						return -1;
					}
				}
				else
				{
					_OverlappedManage.Free(p);
					return -1;
				}

				return 1;
			}

			int acceptor::async_accept( const fn_accept_handle& handle )
			{
				tagOverlapped* p = _OverlappedManage.Alloc();
				p->_hfnComplete = function20_bind(&acceptor::accept_handle, this, handle, _function_::_1);
				p->_Free_Overlapp = function1_handle(&acceptor::FreeOverlapped, this);
				p->_hfnPerform = function20_bind(&acceptor::AcceptCallBack, this, p,
					_function_::_1, _function_::_2, _function_::_3);
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_ACCEPT;

				struct sockaddr_in in_addr = {0};
				SOCKET s = APIWSAAccept(_Sock, &in_addr);
				if( s >= 0 )
				{
					//test
					p->_RealSize = 0;
					p->_ADR._Sock = s;
					_service.post(p, 0);
					//test

					/*tagIOData_INFO	IODataINFO;
					IODataINFO._ibytes_transferred = 0;
					IODataINFO._pOverlapped = p;
					p->_ADR._Sock = s;
					p->_isPerform = false;
					_service.process(&IODataINFO, p);*/
				}
				else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) )
				{
					LOG_Print_Info(libEPoll_Net, "acceptor::async_accept post\r\n")

					p->_RealSize = -1;
					if( _service.post_read(_Sock, p) == -1 )
					{
						_OverlappedManage.Free(p);
						return -1;
					}
				}
				else
				{
					_OverlappedManage.Free(p);
					return -1;
				}

				return 1;
			}

			int acceptor::async_accept( tagOverlapped* pOverlapped )
			{
				tagOverlapped::AddRef(pOverlapped);

				struct sockaddr_in in_addr;
				pOverlapped->_ADR._Sock = APIWSAAccept(_Sock, &in_addr);
				if( pOverlapped->_ADR._Sock > 0 )
				{			
					//test
					pOverlapped->_RealSize = 0;
					_service.post(pOverlapped, 0);
					//test
				
					/*tagIOData_INFO	IODataINFO;
					IODataINFO._ibytes_transferred = 0;
					IODataINFO._pOverlapped = pOverlapped;
					pOverlapped->_isPerform = false;
					_service.process(&IODataINFO, pOverlapped);*/
				}
				else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) )
				{				
					pOverlapped->_RealSize = -1;
					if( _service.post_read(_Sock, pOverlapped) == -1 )
					{
						//test
						printf("exit failed acceptor::async_accept11 failed pOverlapped:%p, errno:%d!\r\n", pOverlapped, errno);
						//system("cat /proc/sys/fs/file-nr");
						Sleep(100);
						_exit(0);
						//test

						tagOverlapped::SubRef(pOverlapped);
						return -1;
					}
				}
				else
				{
					//test
					printf("exit failed acceptor::async_accept22 failed pOverlapped:%p, errno:%d!\r\n", pOverlapped, errno);
					//system("cat /proc/sys/fs/file-nr");
					Sleep(100);
					_exit(0);
					//test

					tagOverlapped::SubRef(pOverlapped);
					return -1;
				}

				return 1;
			}

			int acceptor::AcceptCallBack( tagOverlapped* pOverlapped, SOCKET s, char* c_szData, unsigned long uSize )
			{
				struct sockaddr_in in_addr = {0};
				pOverlapped->_ADR._Sock = APIWSAAccept(_Sock, &in_addr);
				if( pOverlapped->_ADR._Sock != SOCKET_ERROR )
				{
					APIWSASetnonBlocking(pOverlapped->_ADR._Sock);
				}
				else
				{
					pOverlapped->_ADR._Sock = INVALID_SOCKET;
				}

				LOG_Print_Info(libEPoll_Net, "acceptor::AcceptCallBack Socket:%d\r\n", pOverlapped->_ADR._Sock)
				return 1;
			}

			void acceptor::accept_handle( const fn_accept_handle& f, tagIOData_INFO* pIOData )
			{
				tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;
				LOG_Print_Info(libEPoll_Net, "acceptor::accept_handle Socket:%d\r\n", pOver->_ADR._Sock)

				if( pOver->_ADR._Sock != INVALID_SOCKET )
					_service.bind((HANDLE)pOver->_ADR._Sock);
				((fn_accept_handle)f)(pIOData);
			}

			void acceptor::close( void )
			{
				if( is_open() )
				{
					APIWSAClose(_Sock);
					_Sock = INVALID_SOCKET;
				}
			}

		} //namespace _tcp_

	#undef LISTEN_NUM

	} // _iocp_net_

#endif
