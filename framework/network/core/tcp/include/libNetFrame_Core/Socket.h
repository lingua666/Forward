
#ifndef __SOCKET_20150914105604_1442199364_H__
#define __SOCKET_20150914105604_1442199364_H__

#include <libNetFrame_Core/InitSock.h>
#include <libNetFrame_Core/ISocket.h>
#include <libNetFrame_Core_Common/NetNode.h>

namespace _io_net_{

	namespace _tcp_{

		typedef _sys_error_							error_code;
	#if defined( PLATFORM_OS_FAMILY_WINDOWS )
		typedef _iocp_net_::_tcp_::io_service		io_service;
		typedef _iocp_net_::_tcp_::TcpSocket		socket_type;
		typedef _iocp_net_::_tcp_::tagOverlapped	tagOverlapped;
		typedef	_iocp_net_::_tcp_::acceptor			acceptor_type;
		typedef	_iocp_net_::_tcp_::acceptor_V20		acceptor_V20_type;
		typedef	_iocp_net_::_tcp_::connector		connector_type;
	#elif defined( PLATFORM_OS_FAMILY_UNIX )
		typedef _epoll_net_::_tcp_::io_service		io_service;
		typedef _epoll_net_::_tcp_::TcpSocket		socket_type;
		typedef _epoll_net_::_tcp_::tagOverlapped	tagOverlapped;
		typedef	_epoll_net_::_tcp_::acceptor		acceptor_type;
		typedef	_epoll_net_::_tcp_::acceptor_V20	acceptor_V20_type;
		typedef	_epoll_net_::_tcp_::connector		connector_type;
	#endif

		class	MySocket;
		typedef MySocket _SOCKET_;

		#define REVBUF_SIZE		1460			//1450-1470
		#define SENDBUF_SIZE	REVBUF_SIZE

		class MySocket //: public ISocket
		{
		public:
			typedef socket_type::HSOCKET				HSOCKET;
			typedef socket_type::OverlappedManage_type	OverlappedManage_type;

		public:
			MySocket( io_service& IOService )
					:_Sock(IOService)
			{

			}

			~MySocket( void )
			{}

			void DefaultConfig( void )
			{
				//update recv buffer
		#ifdef	USE_BOOST_ASIO
				boost::asio::socket_base::receive_buffer_size ReavOpt(32 * 1024);
				SetOption(ReavOpt);

				//close send buffer
				boost::asio::socket_base::send_buffer_size	SendOpt(0);
				SetOption(SendOpt);

				//delay	30 second
				/*boost::asio::socket_base::linger LngOption(true, 30);
				SetOption(LngOption);*/
		#endif
			}

			socket_type::HSOCKET	Connect( const char* c_szIP, UInt16 u16Port,
				int iSocketRecv = -1, int iSocketSend = -1 )
			{
				return _Sock.Connect(c_szIP, u16Port, iSocketRecv, iSocketSend);
			}

			bool isOpen( void ) const
			{
				return _Sock.is_open();
			}

	#if defined( PLATFORM_OS_FAMILY_UNIX )
			void	Close( bool isPassivity = false )
	#elif defined( PLATFORM_OS_FAMILY_WINDOWS )
			void	Close( void )
	#endif
			{
				_WLock.Lock();
				_RLock.Lock();
				if( isOpen() )
				{
				#if defined( PLATFORM_OS_FAMILY_UNIX )
					_Sock.close(isPassivity);
				#elif defined( PLATFORM_OS_FAMILY_WINDOWS )
					_Sock.close();
				#endif
				}
				_RLock.UnLock();
				_WLock.UnLock();
			}

			_string_type	GetRemoteIP( void )
			{
				return _Sock.GetInfo()->_szRemote;
			}

			unsigned short	GetRemotePort( void )
			{
				return _Sock.GetInfo()->_u16RemotePort;
			}

			_string_type	GetLocalIP( void )
			{
				return _Sock.GetInfo()->_szLocal;
			}

			unsigned short	GetLocalPort( void )
			{
				return _Sock.GetInfo()->_u16LocalPort;
			}

			template <typename T1>
			int	AsyncRecv( const char* c_pData, UInt32 u32Size, const T1& handle )
			{
				_RLock.Lock();
				if( isOpen() )
				{
					_RLock.UnLock();
					WSABUF	wBuf;
					wBuf.buf = (char*)c_pData;
					wBuf.len = u32Size;
					return _Sock.async_read(wBuf,handle);	
				}
				_RLock.UnLock();
				return -1;
			}

			template <typename T1>
			int	AsyncRecv( const _string_type& sData, const T1& handle )
			{
				_RLock.Lock();
				if( isOpen() )
				{
					_RLock.UnLock();
					WSABUF	wBuf;
					wBuf.buf = sData.c_str();
					wBuf.len = sData.size();
					return _Sock.async_read(wBuf,handle);
				}
				_RLock.UnLock();
				return -1;
			}

			int	Bind( const socket_type::HSOCKET& Socket )
			{
				return _Sock.Bind(Socket);
			}

			int Attach( const socket_type::HSOCKET& Socket )
			{
				return _Sock.Attach(Socket);
			}

			int	AsyncRecv( tagOverlapped* pOverlapped )
			{
				_RLock.Lock();
				if( isOpen() )
				{				
					_RLock.UnLock();
					return _Sock.async_read(pOverlapped);
				}
				_RLock.UnLock();
				return -1;
			}

			int	AsyncRecv( tagOverlapped* pOverlapped, const char* c_pData, UInt32 u32Size )
			{
				_RLock.Lock();
				if( isOpen() )
				{
					_RLock.UnLock();
					return _Sock.async_read(pOverlapped, (char*)c_pData, u32Size);
				}
				_RLock.UnLock();
				return -1;
			}
	
			template <typename T1>
			int	AsyncSend( const _string_type& sData, const T1& handle )
			{
				_WLock.Lock();
				if( isOpen() )
				{	
					_WLock.UnLock();
					WSABUF	wBuf;
					wBuf.buf = sData.c_str();
					wBuf.len = sData.size();
					return _Sock.async_write(wBuf,handle);	
				}
				_WLock.UnLock();
				return -1;
			}

			template <typename T1>
			int	AsyncSend( const char* c_pData, UInt32 u32Size, const T1& handle )
			{
				_WLock.Lock();
				if( isOpen() )
				{
					_WLock.UnLock();
					WSABUF	wBuf;
					wBuf.buf = (char*)c_pData;
					wBuf.len = u32Size;
					return _Sock.async_write(wBuf,handle);
				}
				_WLock.UnLock();
				return -1;
			}

			socket_type&	GetSocket( void )
			{
				return _Sock;
			}

			NETHANDLE	GetRemoteNetNode( void )
			{
				_string_type sIP = GetRemoteIP();
				return GetEndPointFromSIPv4(sIP.c_str(), GetRemotePort());
			}

			NETHANDLE	GetLocalNetNode( void )
			{
				_string_type sIP = GetLocalIP();
				return GetEndPointFromSIPv4(sIP.c_str(), GetLocalPort());
			}

		private:
			socket_type			_Sock;
			CLock				_RLock;
			CLock				_WLock;
		};

	} //namespace _tcp_

} //_io_net_

#endif

