
#include <libEPoll_Net/acceptor_V20.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{

	#define	LISTEN_NUM		32

		namespace _tcp_{

			acceptor_V20::acceptor_V20( void )
			{
			
			}

			acceptor_V20::acceptor_V20( const io_service& service )
				: _service(service)
			{
			
			}

			acceptor_V20::~acceptor_V20( void )
			{
				reset_function();
			}

			void acceptor_V20::close(SOCKET Sock)
			{
				if (Sock != INVALID_SOCKET)
				{
					APIWSAClose(Sock);
					Sock = INVALID_SOCKET;
				}
			}

			int acceptor_V20::attach( const io_service& service )
			{
				_service = service;
				return 1;
			}

			int acceptor_V20::bind( UInt16 u16Port, const char* c_szIP )
			{
				int i_val = 1;

				SOCKET Sock = APIWSACreate(CMyInitSock::SOCKET_TCP_TYPE, -1, 65535);
				if (Sock == INVALID_SOCKET)
				{
					goto gt_end;
				}

				if( APIWSABind(Sock, u16Port, c_szIP) == SOCKET_ERROR )
				{
					APIWSAClose(Sock);
					Sock = INVALID_SOCKET;
					goto gt_end;
				}

				if( APIWSASetSockopt(Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i_val, sizeof(i_val)) == SOCKET_ERROR )
				{
					APIWSAClose(Sock);
					Sock = INVALID_SOCKET;
					goto gt_end;
				}

gt_end:
				return Sock;
			}

			int acceptor_V20::listen( SOCKET Sock )
			{
				if (!is_open() || Sock == INVALID_SOCKET)
				{
					return -1;
				}

				if(APIWSAListen(Sock, SOMAXCONN) == SOCKET_ERROR)
				{
					APIWSAClose(Sock);
					Sock = INVALID_SOCKET;
					return -1;
				}

				_service.bind((HANDLE)Sock);
				return 1;
			}

			int acceptor_V20::async_accept( SOCKET Sock )
			{
				if( !_f )
					return -1;

				tagOverlapped* p = _OverlappedManage.Alloc();
				p->_hfnComplete = _f;
				p->_Free_Overlapp = function1_handle(&acceptor_V20::FreeOverlapped, this);
				p->_hfnPerform = function20_bind(&acceptor_V20::AcceptCallBack, this, p, Sock,
											_function_::_1, _function_::_2, _function_::_3);
										
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_ACCEPT;
				struct sockaddr_in in_addr = {0};
				SOCKET s = APIWSAAccept(Sock, &in_addr);
				if( s >= 0 )
				{
					p->_RealSize = 0;
					p->_ADR._Sock = s;
					_service.post(p, 0);
				}
				else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) )
				{
					LOG_Print_Info(libEPoll_Net, "acceptor_V20::async_accept post\r\n")

					if( _service.post_read(Sock, p) == -1 )
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

			int acceptor_V20::async_accept( SOCKET Sock, const fn_accept_handle& handle )
			{
				tagOverlapped* p = _OverlappedManage.Alloc();
				p->_hfnComplete = function20_bind(&acceptor_V20::accept_handle, this, Sock, handle, _function_::_1);
				p->_Free_Overlapp = function1_handle(&acceptor_V20::FreeOverlapped, this);
				p->_hfnPerform = function20_bind(&acceptor_V20::AcceptCallBack, this, p, Sock,
					_function_::_1, _function_::_2, _function_::_3);
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_ACCEPT;

				struct sockaddr_in in_addr = {0};
				SOCKET s = APIWSAAccept(Sock, &in_addr);
				if( s >= 0 )
				{
					p->_RealSize = 0;
					p->_ADR._Sock = s;
					_service.post(p, 0);
				}
				else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) )
				{
					LOG_Print_Info(libEPoll_Net, "acceptor_V20::async_accept post\r\n")

					p->_RealSize = -1;
					if( _service.post_read(Sock, p) == -1 )
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

			int acceptor_V20::async_accept(SOCKET Sock, tagOverlapped* pOverlapped)
			{
				tagOverlapped::AddRef(pOverlapped);

				struct sockaddr_in in_addr = {0};
				pOverlapped->_ADR._Sock = APIWSAAccept(Sock, &in_addr);
				if( pOverlapped->_ADR._Sock > 0 )
				{			
					pOverlapped->_RealSize = 0;
					_service.post(pOverlapped, 0);
				}
				else if( (errno == EAGAIN) || (errno == EWOULDBLOCK) )
				{
					pOverlapped->_RealSize = -1;
					if( _service.post_read(Sock, pOverlapped) == -1 )
					{
						//test
						printf("exit failed acceptor_V20::async_accept11 failed pOverlapped:%p, errno:%d!\r\n", pOverlapped, errno);
						/*system("cat /proc/sys/fs/file-nr");
						Sleep(100);
						_exit(0);*/
						//test

						tagOverlapped::SubRef(pOverlapped);
						return -1;
					}
				}
				else
				{
					//test
					printf("exit failed acceptor_V20::async_accept22 failed pOverlapped:%p, errno:%d!\r\n", pOverlapped, errno);
					/*system("cat /proc/sys/fs/file-nr");
					Sleep(100);
					_exit(0);*/
					//test

					tagOverlapped::SubRef(pOverlapped);
					return -1;
				}

				return 1;
			}

			int acceptor_V20::AcceptCallBack( tagOverlapped* pOverlapped, SOCKET ListenSock, 
				SOCKET s, char* c_szData, unsigned long uSize )
			{
				struct sockaddr_in in_addr = {0};
				pOverlapped->_ADR._Sock = APIWSAAccept(ListenSock, &in_addr);
				if( pOverlapped->_ADR._Sock != SOCKET_ERROR )
				{
					APIWSASetnonBlocking(pOverlapped->_ADR._Sock);
				}
				else
				{
					pOverlapped->_ADR._Sock = INVALID_SOCKET;
				}

				LOG_Print_Info(libEPoll_Net, "acceptor_V20::AcceptCallBack Socket:%d\r\n", pOverlapped->_ADR._Sock)
				return 1;
			}

			void acceptor_V20::accept_handle( SOCKET Sock, const fn_accept_handle& f, tagIOData_INFO* pIOData )
			{
				tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;
				LOG_Print_Info(libEPoll_Net, "acceptor_V20::accept_handle Socket:%d\r\n", pOver->_ADR._Sock)

				if( pOver->_ADR._Sock != INVALID_SOCKET )
					_service.bind((HANDLE)pOver->_ADR._Sock);
				((fn_accept_handle)f)(pIOData);
			}

		} //namespace _tcp_

	#undef LISTEN_NUM

	} // _iocp_net_

#endif
