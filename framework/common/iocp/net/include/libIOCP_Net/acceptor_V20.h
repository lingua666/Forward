
#ifndef __ACCEPTOR_V20_20151201222754_1448980074_H__
#define __ACCEPTOR_V20_20151201222754_1448980074_H__

#include "tcp_io_service.h"
#include "TcpSocket.h"

namespace _iocp_net_{

	namespace _tcp_{

		/*!
		* @class  acceptor_V20
		* @brief   acceptor_V20
		* @{
		*/

		class acceptor_V20
		{
		public:
			typedef	HFNIOData	fn_accept_handle;

		public:
			/*!
			* @function   acceptor_V20
			* @brief    
			* @return  
			*/
			acceptor_V20( void );

			acceptor_V20( const io_service& service );

			/*!
			* @function   ~acceptor_V20
			* @brief    
			* @return  
			*/
			~acceptor_V20( void );

			void close(SOCKET Sock);

			int attach( const io_service& service );

			SOCKET bind( UInt16 u16Port, const char* c_szIP = NULL );

			int listen( SOCKET Sock );

			int async_accept( SOCKET Sock );

			int async_accept( SOCKET Sock, const fn_accept_handle& handle );

			int async_accept( SOCKET Sock, tagOverlapped* pOverlapped );

			void accept_handle( SOCKET Sock, const fn_accept_handle& f, tagIOData_INFO* pIOData );

			inline void set_function( SOCKET Sock, const fn_accept_handle& f )
			{
				_f = function20_bind(&acceptor_V20::accept_handle, this, Sock, f, _foundation_::_1 );
			}

			inline void reset_function( void )
			{
				if( _f )
				{
					_f.reset();
				}
			}

			bool is_open( void ) const
			{
				return _service.is_open();
			}
		protected:
			void	FreeOverlapped( tagOverlapped* p )
			{
				_OverlappedManage.Free(p);
			}
		private:
			io_service _service;
			fn_accept_handle _f;
			OverlappedManage<(sizeof(SOCKADDR_IN) + 16) * 4>	_OverlappedManage;
			LPFN_ACCEPTEX               _lpfnAcceptEx; 
			LPFN_GETACCEPTEXSOCKADDRS   _lpfnGetAcceptExSockAddrs;
		};
		/** @} end acceptor_V20 */

	} //namespace _tcp_

} //_iocp_net_


#endif//__ACCEPTOR_V20_20151201222754_1448980074_H__
