
#ifndef __CONNECTOR_20160323164906_1458722946_H__
#define __CONNECTOR_20160323164906_1458722946_H__

#include "tcp_io_service.h"
#include "TcpSocket.h"

namespace _iocp_net_{

	namespace _tcp_{

		/*!
		* @class  connector
		* @brief   connector
		* @{
		*/

		class connector
		{
		public:
			typedef	HFNIOData	fn_connect_handle;

		public:
			/*!
			* @function   connector
			* @brief    
			* @return  
			*/
			connector( void );

			connector( const io_service& service );

			/*!
			* @function   ~connector
			* @brief    
			* @return  
			*/
			~connector( void );

			int attach( const io_service& service );

			int listen( int iSocketRecv = -1, int iSocketSend = -1 );

			int async_connect( const char* szIP, UInt16 uPort,
							int iSocketRecv = -1, int iSocketSend = -1 );

			int async_connect( const char* szIP, UInt16 uPort,
						const fn_connect_handle& handle,
						int iSocketRecv = -1, int iSocketSend = -1 );

			void connect_handle( const fn_connect_handle& f, tagIOData_INFO* pIOData);

			void close( void );

			inline void set_function( const fn_connect_handle& f )
			{
				_f = function20_bind(&connector::connect_handle, this, f, _foundation_::_1 );
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
				return _lpfnConnectEx != NULL;
				//return _Sock != INVALID_SOCKET;
			}
		protected:
			void	FreeOverlapped( tagOverlapped* p )
			{
				_OverlappedManage.Free(p);
			}
		private:
			//SOCKET		_Sock;
			io_service	_service;
			fn_connect_handle _f;
			OverlappedManage<sizeof(SOCKADDR_IN)>	_OverlappedManage;
			LPFN_CONNECTEX               _lpfnConnectEx; 
		};
		/** @} end connector */

	} //namespace _tcp_

} //_iocp_net_

#endif//__CONNECTOR_20160323164906_1458722946_H__
