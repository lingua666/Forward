
#include <libIOCP_Net/TcpSocket.h>

namespace	_iocp_net_{

	namespace _tcp_{

		void TcpSocket::CloseSocket(const HSOCKET& Sock)
		{
			APIWSAClose(Sock);
		}

		TcpSocket::TcpSocket( const io_service& service )
			: _Service(service)
			, _Sock(INVALID_SOCKET)
		{
			memset(&_tagInfo, 0, sizeof(_tagInfo));
		}

		TcpSocket::~TcpSocket( void )
		{

		}

		TcpSocket::HSOCKET TcpSocket::Connect( const _string_type& sIP, UInt16 u16Port,
											int iSocketRecv, int iSocketSend )
		{
			_Sock = APIWSACreate(SOCK_STREAM, iSocketRecv, iSocketSend);
			if( _Sock == SOCKET_ERROR )
				return INVALID_SOCKET;

			SOCKADDR_IN addr;
			addr.sin_family				= AF_INET;
			addr.sin_addr.S_un.S_addr	= inet_addr( sIP.c_str() );
			addr.sin_port				= htons( u16Port );

			if( ::connect( _Sock, (sockaddr *) &addr, sizeof(addr) ) == -1 )
			{
				APIWSAClose(_Sock);
				return INVALID_SOCKET;
			}

			return _Sock;
		}

		int TcpSocket::Bind( const HSOCKET& Sock )
		{
			if( ReadInfo(Sock) == 1 &&
				_Service.bind((HANDLE)Sock) == 1 )
			{
				_Sock = Sock;
				return 1;
			}
			else
			{
				APIWSAClose(Sock);
			}

			return -1;
		}

		int TcpSocket::Attach( const HSOCKET& Sock )
		{
			if( ReadInfo(Sock) != 1 )
			{
				APIWSAClose(Sock);
				return -1;
			}

			_Sock = Sock;
			return 1;
		}

		int TcpSocket::ReadInfo( const HSOCKET& Sock )
		{
			if( APIGetWSASocketLocalIP( Sock, _tagInfo._szLocal, sizeof(_tagInfo._szLocal) ) &&
				APIGetWSASocketLocalPort( Sock, _tagInfo._u16LocalPort ) &&
				APIGetWSASocketRemoteIP( Sock, _tagInfo._szRemote, sizeof(_tagInfo._szRemote) ) &&
				APIGetWSASocketRemotePort( Sock, _tagInfo._u16RemotePort ) )
			{
				return 1;
			}

			return -1;
		}

		int TcpSocket::async_read( const WSABUF& wBuf, const HFNIOData& handle )
		{
			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = handle;
			p->_ADR._Sock = _Sock;
			p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
			p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
			p->_Free_Overlapp = function1_handle(&TcpSocket::FreeOverlapped, this);

			int iResult = APIWSARecv(_Sock,p->_SWSAOverlapp.wsaBuf.buf,
				p->_SWSAOverlapp.wsaBuf.len,
				&p->_SWSAOverlapp.wsaOverlapped);

			if ( iResult == SOCKET_ERROR )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int TcpSocket::async_read( tagOverlapped* pOverlapped,
			char* pData, UInt32 u32Size )
		{
			pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
			pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
			return async_read(pOverlapped);
		}

		int TcpSocket::async_read( tagOverlapped* pOverlapped )
		{
			tagOverlapped::AddRef(pOverlapped);
			int iResult = APIWSARecv(_Sock,pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len,
				&pOverlapped->_SWSAOverlapp.wsaOverlapped);

			if ( iResult == SOCKET_ERROR )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}

			return 1;
		}

		int TcpSocket::async_write( const WSABUF& wBuf, const HFNIOData& handle )
		{
			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = handle;
			p->_ADR._Sock = _Sock;

			p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
			p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
			p->_Free_Overlapp = function1_handle(&TcpSocket::FreeOverlapped, this);

			int iResult = APIWSASend(_Sock,p->_SWSAOverlapp.wsaBuf.buf,
				p->_SWSAOverlapp.wsaBuf.len,
				&p->_SWSAOverlapp.wsaOverlapped);

			if( iResult == SOCKET_ERROR )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int TcpSocket::async_write( tagOverlapped* pOverlapped )
		{
			tagOverlapped::AddRef(pOverlapped);
			int iResult = APIWSASend(_Sock,pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len,
				&pOverlapped->_SWSAOverlapp.wsaOverlapped);
			if( iResult == SOCKET_ERROR )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}

			return 1;
		}

		void TcpSocket::close( void )
		{
			if( is_open() )
			{
				::APIWSAClose(_Sock);
				_Sock = INVALID_SOCKET;
			}
		}

	} //namespace _tcp_

} //_iocp_net_

