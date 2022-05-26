
#include <libIOCP_Net/UdpSocket.h>

namespace	_iocp_net_{

	namespace _udp_{

		UdpSocket::UdpSocket( const io_service& service )
			: _Service(service)
			, _Sock(INVALID_SOCKET)
		{
			memset(&_tagInfo, 0, sizeof(_tagInfo));
		}

		UdpSocket::~UdpSocket( void )
		{

		}

		UdpSocket::HSOCKET UdpSocket::Listen( UInt16 u16Port, const char* c_szIP)
		{
			_Sock = APIWSACreate(SOCK_DGRAM, 1024 * 100, -1);
			if( _Sock == SOCKET_ERROR )
				return INVALID_SOCKET;

			SOCKADDR_IN addr;
			addr.sin_family			= AF_INET;
			if(c_szIP == NULL)
				addr.sin_addr.s_addr	= htonl(INADDR_ANY);
			else
				addr.sin_addr.s_addr = inet_addr(c_szIP);

			addr.sin_port			= htons( u16Port );

			if( ::bind( _Sock, (sockaddr *) &addr, sizeof(addr) ) == -1 )
			{
				APIWSAClose(_Sock);
				return INVALID_SOCKET;
			}

			if( Bind(_Sock) == -1 )
			{
				APIWSAClose(_Sock);
				return INVALID_SOCKET;
			}

			return _Sock;
		}

		int UdpSocket::Bind( const HSOCKET& Sock )
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

		int UdpSocket::Attach( const HSOCKET& Sock )
		{
			if( ReadInfo(Sock) != 1 )
			{
				APIWSAClose(Sock);
				return -1;
			}

			_Sock = Sock;
			return 1;
		}

		int UdpSocket::ReadInfo( const HSOCKET& Sock )
		{
			if( APIGetWSASocketLocalIP( Sock, _tagInfo._szLocal, sizeof(_tagInfo._szLocal) ) &&
				APIGetWSASocketLocalPort( Sock, _tagInfo._u16LocalPort ) )
			{
				return 1;
			}

			return -1;
		}

		int UdpSocket::async_read( const WSABUF& wBuf, const HFNIOData& handle )
		{
			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = handle;
			p->_ADR._Sock = _Sock;
			p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
			p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
			p->_Free_Overlapp = function1_handle(&UdpSocket::FreeOverlapped, this);

			int iResult = APIWSARecvFrom(_Sock, p->_SWSAOverlapp.wsaBuf.buf,
				p->_SWSAOverlapp.wsaBuf.len, (sockaddr*)p->_pAddr,
				&p->_SWSAOverlapp.wsaOverlapped);

			if ( iResult == SOCKET_ERROR )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int UdpSocket::async_read( tagOverlapped* pOverlapped,
			char* pData, UInt32 u32Size )
		{
			pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
			pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
			return async_read(pOverlapped);
		}

		int UdpSocket::async_read( tagOverlapped* pOverlapped )
		{
			tagOverlapped::AddRef(pOverlapped);
			int iResult = APIWSARecvFrom(_Sock,pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len, (sockaddr*)pOverlapped->_pAddr,
				&pOverlapped->_SWSAOverlapp.wsaOverlapped);

			if ( iResult == SOCKET_ERROR )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}

			return 1;
		}

		int UdpSocket::async_write( sockaddr_in* pAddr, const WSABUF& wBuf, const HFNIOData& handle )
		{
			tagOverlapped* p = _OverlappedManage.Alloc();
			p->_Func = handle;
			p->_ADR._Sock = _Sock;

			p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
			p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
			p->_Free_Overlapp = function1_handle(&UdpSocket::FreeOverlapped, this);
			memcpy(p->_pAddr, pAddr, sizeof(sockaddr_in));

			int iResult = APIWSASendto(_Sock,p->_SWSAOverlapp.wsaBuf.buf,
				p->_SWSAOverlapp.wsaBuf.len, (sockaddr*)pAddr,
				&p->_SWSAOverlapp.wsaOverlapped);

			if( iResult == SOCKET_ERROR )
			{
				_OverlappedManage.Free(p);
				return -1;
			}

			return 1;
		}

		int UdpSocket::async_write( sockaddr_in* pAddr, tagOverlapped* pOverlapped )
		{
			tagOverlapped::AddRef(pOverlapped);
			memcpy(pOverlapped->_pAddr, pAddr, sizeof(sockaddr_in));
			int iResult = APIWSASendto(_Sock,pOverlapped->_SWSAOverlapp.wsaBuf.buf,
				pOverlapped->_SWSAOverlapp.wsaBuf.len, (sockaddr*)pAddr,
				&pOverlapped->_SWSAOverlapp.wsaOverlapped);
			if( iResult == SOCKET_ERROR )
			{
				tagOverlapped::SubRef(pOverlapped);
				return -1;
			}

			return 1;
		}

		void UdpSocket::close( void )
		{
			if( is_open() )
			{
				::APIWSAClose(_Sock);
				_Sock = INVALID_SOCKET;
			}
		}

	} //_udp_

} //_iocp_net_

