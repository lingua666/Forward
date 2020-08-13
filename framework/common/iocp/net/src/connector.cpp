
#include <libIOCP_Net/connector.h>

namespace _iocp_net_{

	namespace _tcp_{

		connector::connector( void )
			: _lpfnConnectEx(NULL)
			//: _Sock(INVALID_SOCKET)
		{
	
		}

		connector::connector( const io_service& service )
			: _service(service)
			, _lpfnConnectEx(NULL)
			//, _Sock(INVALID_SOCKET)
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
			SOCKET Sock = APIWSACreate(CMyInitSock::SOCKET_TCP_TYPE,-1,-1);
			if( Sock != SOCKET_ERROR )
			{
				if( APIWSAConnectExInit(Sock, _lpfnConnectEx) != SOCKET_ERROR )
				{
					APIWSAClose(Sock);
					return 1;
				}
			}
			return SOCKET_ERROR;
		}

		int connector::async_connect( const char* szIP, UInt16 uPort )
		{
			if( !_f )
				return -1;

			sockaddr_in addrPeer ;  
			ZeroMemory (&addrPeer , sizeof (sockaddr_in ));
			addrPeer .sin_family = AF_INET ;
			addrPeer .sin_addr .s_addr = inet_addr ( szIP );
			addrPeer .sin_port = htons ( uPort );

			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = _f;
			p->_ADR._Sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  
			if( INVALID_SOCKET == p->_ADR._Sock )
			{  
				_OverlappedManage.Free(p);
				return -1;
			};

			_service.bind((HANDLE)p->_ADR._Sock);

			p->_Free_Overlapp = function1_handle(&connector::FreeOverlapped, this);
			if( !APIWSAConnectEx(_lpfnConnectEx, p->_ADR._Sock,
				addrPeer, &p->_SWSAOverlapp,0) )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int connector::async_connect( const char* szIP, UInt16 uPort,
									const fn_connect_handle& handle )
		{
			sockaddr_in addrPeer ;  
			ZeroMemory (&addrPeer , sizeof (sockaddr_in ));
			addrPeer .sin_family = AF_INET ;
			addrPeer .sin_addr .s_addr = inet_addr ( szIP );
			addrPeer .sin_port = htons ( uPort );

			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = function20_bind(&connector::connect_handle, this, handle, _foundation_::_1 );
			p->_Free_Overlapp = function1_handle(&connector::FreeOverlapped, this);

			p->_ADR._Sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  
			if( INVALID_SOCKET == p->_ADR._Sock )
			{  
				_OverlappedManage.Free(p);
				return -1;
			};

			_service.bind((HANDLE)p->_ADR._Sock);

			if( !APIWSAConnectEx(_lpfnConnectEx, p->_ADR._Sock,
				addrPeer, &p->_SWSAOverlapp,0) )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		void connector::connect_handle( const fn_connect_handle& f, tagIOData_INFO* pIOData)
		{
			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if(	APIWSASetSockopt( pOver->_ADR._Sock, SOL_SOCKET,
				SO_UPDATE_CONNECT_CONTEXT, NULL, 0 ) != SOCKET_ERROR )
			{
			
			}
			else
			{
				if( pOver->_ADR._Sock != INVALID_SOCKET )
				{
					APIWSAClose(pOver->_ADR._Sock);
					pOver->_ADR._Sock = INVALID_SOCKET;
				}
			}

			((fn_connect_handle)f)(pIOData);
		}

		void connector::close( void )
		{
			/*if( is_open() )
			{
				::APIWSAClose(_Sock);
				_Sock = INVALID_SOCKET;
			}*/
		}

	} //namespace _tcp_

} //_iocp_net_

