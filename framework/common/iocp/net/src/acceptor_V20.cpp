
#include <libIOCP_Net/acceptor_V20.h>

namespace _iocp_net_{

	namespace _tcp_{

#define	LISTEN_NUM		32

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
				::APIWSAClose(Sock);
				Sock = INVALID_SOCKET;
			}
		}

		int acceptor_V20::attach( const io_service& service )
		{
			_service = service;
			return 1;
		}

		SOCKET acceptor_V20::bind( UInt16 u16Port, const char* c_szIP)
		{
			int i_val = 1;

			SOCKET Sock = APIWSACreate(CMyInitSock::SOCKET_TCP_TYPE, -1, 65535);
			if (Sock == INVALID_SOCKET)
				goto gt_end;

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
			if( !is_open() || Sock == INVALID_SOCKET )
				return -1;

			if(APIWSAListen(Sock,0,_lpfnAcceptEx,
				_lpfnGetAcceptExSockAddrs) == SOCKET_ERROR)
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
			p->_Func = _f;
			p->_Free_Overlapp = function1_handle(&acceptor_V20::FreeOverlapped, this);
			if( !APIWSAacceptEx(_lpfnAcceptEx, Sock,
				p->_ADR._Sock, &p->_SWSAOverlapp,0) )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int acceptor_V20::async_accept( SOCKET Sock, const fn_accept_handle& handle )
		{
			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = function20_bind(&acceptor_V20::accept_handle, this, Sock, handle, _foundation_::_1 );
			p->_Free_Overlapp = function1_handle(&acceptor_V20::FreeOverlapped, this);
			if( !APIWSAacceptEx(_lpfnAcceptEx, Sock,
				p->_ADR._Sock, &p->_SWSAOverlapp,0) )
			{
				_OverlappedManage.Free(p);
				return -1;
			}
			return 1;
		}

		int acceptor_V20::async_accept( SOCKET Sock, tagOverlapped* pOverlapped )
		{
			tagOverlapped::AddRef(pOverlapped);
			if( !APIWSAacceptEx(_lpfnAcceptEx, Sock,
				pOverlapped->_ADR._Sock, &pOverlapped->_SWSAOverlapp,0) )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}
			return 1;
		}

		void acceptor_V20::accept_handle( SOCKET Sock, const fn_accept_handle& f, tagIOData_INFO* pIOData )
		{
			//test
			//printf("acceptor_V20::accept_handle()\r\n");
			//test

			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if(	APIWSASetSockopt( pOver->_ADR._Sock, SOL_SOCKET,
				SO_UPDATE_ACCEPT_CONTEXT, (char *)&Sock, sizeof(Sock) ) != SOCKET_ERROR )
			{
				_service.bind((HANDLE)pOver->_ADR._Sock);
			}
			else
			{
				if( pOver->_ADR._Sock != INVALID_SOCKET )
				{
					APIWSAClose(pOver->_ADR._Sock);
					pOver->_ADR._Sock = INVALID_SOCKET;
				}
			}
			((fn_accept_handle)f)(pIOData);
		}

#undef LISTEN_NUM

	} //namespace _tcp_

} // _iocp_net_

