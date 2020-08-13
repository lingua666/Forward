
#include <libNetFrame_Core/SockStream.h>

namespace _io_net_{

	namespace _tcp_{

		SockStream::SockStream( io_service* pIOService,
			UInt32 uBufSize,
			UInt16 u16PostSize )
			: _Socket( *pIOService )
			, _pRevBuf( NULL )
			, _uPostSize( u16PostSize )
			, _Pool( uBufSize + StreamBuf::GetHdr() )
			, _uBufSize( uBufSize )
			, _isOpen( false )
		{

		}

		SockStream::~SockStream( void )
		{
			Close(true);
		}

		int	SockStream::Bind( const _SOCKET_::HSOCKET& Socket )
		{
			return _Socket.Bind(Socket);
		}

		int	SockStream::Attach( const _SOCKET_::HSOCKET& Socket )
		{
			return _Socket.Attach(Socket);
		}

		void	SockStream::HandleRecv( tagIOData_INFO* pIOData )
		{
			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if(pIOData->_ibytes_transferred == 0)
			{
				printf("failed _tcp_::SockStream::HandleRecv error %s, this:%p, soc:%p, pIOData->_pOverlapped:%p\r\n",
					error_code(error_code::GetLastError()).message().c_str(), this, &_Socket, pIOData->_pOverlapped);


#if defined( PLATFORM_OS_FAMILY_UNIX )
				Close(false, true);
				pOver->_ADR._Sock = INVALID_SOCKET;
#elif defined( PLATFORM_OS_FAMILY_WINDOWS )
				Close();
#endif	
				return ;
			}
#if defined( PLATFORM_OS_FAMILY_UNIX )
			else if( pIOData->_ibytes_transferred == -1 && error_code::GetLastError() == EAGAIN )
			{
				PostRecv(pOver);
				return ;
			}
#endif

			const char* tmBuf = _pRevBuf;

			//数据处理
			StreamBuf::SetSizeToData(tmBuf, pIOData->_ibytes_transferred);

			EVENT_EXEC_READ(shared_from_this(), 
				StreamBuf_ptr(StreamBuf::GetHeadToData(tmBuf),
				function20_bind_Ex(&SockStream::FreeStreamBuf, shared_from_this())) );
			//数据处理

			PostRecv(pOver);
		}

		int	SockStream::PostRecv( tagOverlapped* pOverlapped )
		{
			_buf_hdr* hdr = reinterpret_cast<_buf_hdr*>(_Pool.Alloc());
			_pRevBuf = StreamBuf::GetData(hdr);

			if( pOverlapped == NULL )
			{
				if( 1 != _Socket.AsyncRecv( _pRevBuf, _uPostSize,
					function20_bind(&self_type::HandleRecv, shared_from_this(),
					_foundation_::_1) ) )
				{
					if( hdr != NULL )
						_Pool.Free(hdr);
					return -1;
				}
				return 1;
			}
			else
			{
				if( 1 != _Socket.AsyncRecv(pOverlapped, _pRevBuf, _uPostSize) )
				{
#if defined( PLATFORM_OS_FAMILY_UNIX )
					pOverlapped->_ADR._Sock = INVALID_SOCKET;
#endif
					if( hdr != NULL )
						_Pool.Free(hdr);
					return -1;
				}
				return 1;
			}
			return -1;
		}

		int	SockStream::SendData( const char* c_szData, UInt32 u32Size )
		{
			EVENT_EXEC_PREPARE_WRITE(shared_from_this(), c_szData, u32Size);
			return 0;
		}

		void	SockStream::PostSend( const char* c_szData, UInt32 u32Size )
		{
			if( !_isOpen )
				return ;

			if( _Socket.AsyncSend( c_szData, u32Size, 
				function20_bind(&self_type::HandleSend, shared_from_this(), c_szData, 
				_foundation_::_1) ) == -1 )
			{
				printf("failed SockStream::PostSend error %s, this:%p, soc:%p\r\n",
					error_code(error_code::GetLastError()).message().c_str(), this, &_Socket);

				Close();
			}
		}

		void	SockStream::HandleSend( const char* pBuf, tagIOData_INFO* pIOData )
		{
			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
			if(pIOData->_ibytes_transferred == 0 && error_code::GetLastError() != 997)
#elif defined( PLATFORM_OS_FAMILY_UNIX )
			if(pIOData->_ibytes_transferred == 0)
#endif
			{
				printf("SockStream::HandleSend error %s\r\n",
					error_code(error_code::GetLastError()).message().c_str());

#if defined( PLATFORM_OS_FAMILY_UNIX )
				Close(false, true);
				pOver->_ADR._Sock = INVALID_SOCKET;
#elif defined( PLATFORM_OS_FAMILY_WINDOWS )
				Close();
#endif	
				return ;
			}
#if defined( PLATFORM_OS_FAMILY_UNIX )
			else if( pIOData->_ibytes_transferred == -1 && error_code::GetLastError() == EAGAIN )
			{
				//test
				while(true)
				{
					printf("SockStream::HandleSend failed!!!\r\n");
				}
				//test
			}
#endif
			else
			{
				EVENT_EXEC_WRITE(shared_from_this(), pBuf, pIOData->_ibytes_transferred);
			}
		}

	} //namespace _tcp_

} //_io_net_
