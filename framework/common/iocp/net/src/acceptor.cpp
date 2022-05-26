
#include <libIOCP_Net/acceptor.h>

namespace _iocp_net_{

	namespace _tcp_{

#define	LISTEN_NUM		32

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

		int acceptor::bind( UInt16 u16Port, const char* c_szIP)
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

			if(APIWSAListen(_Sock,0,_lpfnAcceptEx,
				_lpfnGetAcceptExSockAddrs) == SOCKET_ERROR)
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
			p->_Func = _f;
			p->_Free_Overlapp = function1_handle(&acceptor::FreeOverlapped, this);
			if( !APIWSAacceptEx(_lpfnAcceptEx, _Sock,
				p->_ADR._Sock, &p->_SWSAOverlapp,0) )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int acceptor::async_accept( const fn_accept_handle& handle )
		{
			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = function20_bind(&acceptor::accept_handle, this, handle, _foundation_::_1 );
			p->_Free_Overlapp = function1_handle(&acceptor::FreeOverlapped, this);
			if( !APIWSAacceptEx(_lpfnAcceptEx, _Sock,
				p->_ADR._Sock, &p->_SWSAOverlapp,0) )
			{
				_OverlappedManage.Free(p);
				return -1;
			}
			return 1;
		}

		int acceptor::async_accept( tagOverlapped* pOverlapped )
		{
			tagOverlapped::AddRef(pOverlapped);
			if( !APIWSAacceptEx(_lpfnAcceptEx, _Sock,
				pOverlapped->_ADR._Sock, &pOverlapped->_SWSAOverlapp,0) )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}
			return 1;
		}

		void acceptor::accept_handle( const fn_accept_handle& f, tagIOData_INFO* pIOData )
		{
			//test
			printf("acceptor::accept_handle()\r\n");
			//test

			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if(	APIWSASetSockopt( pOver->_ADR._Sock, SOL_SOCKET,
				SO_UPDATE_ACCEPT_CONTEXT, (char *)&_Sock, sizeof(_Sock) ) != SOCKET_ERROR )
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

		void acceptor::close( void )
		{
			if( is_open() )
			{
				::APIWSAClose(_Sock);
				_Sock = INVALID_SOCKET;
			}
		}

#undef LISTEN_NUM

	} //namespace _tcp_

} // _iocp_net_

