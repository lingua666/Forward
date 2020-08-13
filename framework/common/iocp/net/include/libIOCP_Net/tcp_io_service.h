
#ifndef __TCP_IO_SERVICE_20151201220617_1448978777_H__
#define __TCP_IO_SERVICE_20151201220617_1448978777_H__

#include "External.h"

namespace _iocp_net_{

	namespace _tcp_{

		typedef	HANDLE	HSOCKET;
		typedef	HANDLE	IOHANDLE;

		/*!
		* @class  io_service
		* @brief   io_service
		* @{
		*/

		class io_service
		{
		public:
			typedef io_service	self_type;

			friend class acceptor;
			friend class TcpSocket;

		public:
			/*!
			* @function   io_service
			* @brief    
			* @return  
			*/
			io_service( void );

			io_service( const self_type& self );

			/*!
			* @function   ~io_service
			* @brief    
			* @return  
			*/
			~io_service( void );

			io_service& operator=( const self_type& self );

			int open( void );

			int close( void );

			void run( void );

			int stop(void);

			int	bind( HSOCKET sock );

			bool is_run( void ) const
			{
				return _is_run;
			}

			bool is_open( void ) const
			{
				return _io_handle != NULL ? true : false;
			}

		private:
			bool		_is_run;
			IOHANDLE	_io_handle;
		};
		/** @} end io_service */

	} //namespace _tcp_

} // _iocp_net_

#endif//__IO_SERVICE_20151201220617_1448978777_H__
