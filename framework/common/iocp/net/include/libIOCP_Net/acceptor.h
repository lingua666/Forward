
#ifndef __ACCEPTOR_20151201222754_1448980074_H__
#define __ACCEPTOR_20151201222754_1448980074_H__

#include "tcp_io_service.h"
#include "TcpSocket.h"

namespace _iocp_net_{

	namespace _tcp_{

		/*!
		* @class  acceptor
		* @brief   acceptor
		* @{
		*/

		class acceptor
		{
		public:
			typedef	HFNIOData	fn_accept_handle;
		public:
			/*!
			* @function   acceptor
			* @brief    
			* @return  
			*/
			acceptor( void );

			acceptor( const io_service& service );

			/*!
			* @function   ~acceptor
			* @brief    
			* @return  
			*/
			~acceptor( void );

			int attach( const io_service& service );

			int bind( UInt16 u16Port, const char* c_szIP = NULL );

			int listen( void );

			int async_accept( void );

			int async_accept( const fn_accept_handle& handle );

			int async_accept( tagOverlapped* pOverlapped );

			void accept_handle( const fn_accept_handle& f, tagIOData_INFO* pIOData);

			void close( void );

			inline void set_function( const fn_accept_handle& f )
			{
				_f = function20_bind(&acceptor::accept_handle, this, f, _foundation_::_1 );
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
				return _Sock != INVALID_SOCKET;
			}
		protected:
			void	FreeOverlapped( tagOverlapped* p )
			{
				_OverlappedManage.Free(p);
			}
		private:
			SOCKET		_Sock;
			io_service _service;
			fn_accept_handle _f;
			OverlappedManage<(sizeof(SOCKADDR_IN) + 16) * 4>	_OverlappedManage;
			LPFN_ACCEPTEX               _lpfnAcceptEx; 
			LPFN_GETACCEPTEXSOCKADDRS   _lpfnGetAcceptExSockAddrs;
		};
		/** @} end acceptor */

	} //namespace _tcp_

} //_iocp_net_


#endif//__ACCEPTOR_20151201222754_1448980074_H__
