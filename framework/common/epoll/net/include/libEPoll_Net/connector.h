
#ifndef __CONNECTOR_20160323164906_1458722946_H__
#define __CONNECTOR_20160323164906_1458722946_H__

#include "tcp_io_service.h"
#include "TcpSocket.h"
#include "TcpOverlappedManage.hpp"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_net_{

		namespace _tcp_{

			/*!
			* @class  connector
			* @brief   connector
			* @{
			*/

			class connector
			{
			public:
				typedef	HFNComplete	fn_connect_handle;

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

				int listen( void );

				int async_connect( const char* szIP, UInt16 uPort );

				int async_connect( const char* szIP, UInt16 uPort,
							const fn_connect_handle& handle );

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
					return _service.is_open();
				}
			protected:
				int ConnectCallBack( tagOverlapped* pOverlapped,
					SOCKET s, char* c_szData, unsigned long uSize );

				void	FreeOverlapped( tagOverlapped* p )
				{
					this->_OverlappedManage.Free(p);
				}
			private:
				io_service	_service;
				fn_connect_handle _f;
				OverlappedManage<sizeof(struct sockaddr_in)>	_OverlappedManage;
			};
			/** @} end connector */

		} //namespace _tcp_

	} //_iocp_net_

#endif

#endif//__CONNECTOR_20160323164906_1458722946_H__
