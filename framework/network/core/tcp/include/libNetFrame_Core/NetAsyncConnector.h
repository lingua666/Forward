
#ifndef __NETASYNCCONNECTOR_20160323201738_1458735458_H__
#define __NETASYNCCONNECTOR_20160323201738_1458735458_H__

#include <libNetFrame_Core/Socket.h>
#include <libNetFrame_Core_Common/Event_Handle.h>
#include <libNetFrame_Core/SockStream.h>

namespace _io_net_{

	namespace _tcp_{

		/*!
		* @class  NetAsyncConnector
		* @brief   NetAsyncConnector
		* @{
		*/

		class NetAsyncConnector
		{
		public:
			typedef ThreadWrap				Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;

			typedef	function20_handle<void (const _SOCKET_::HSOCKET&)>	fn_connect_handle;
		public:
			/*!
			* @function   NetAsyncConnector
			* @brief    
			* @return  
			*/
			NetAsyncConnector( void );

			/*!
			* @function   ~NetAsyncConnector
			* @brief    
			* @return  
			*/
			~NetAsyncConnector( void );

			const io_service& GetIoServer( void ) const
			{
				return _ioService;
			}

			bool	Listen( void );

			int	Connect( const char* szIP, UInt16 uPort );

			int	Connect( const char* szIP, UInt16 uPort,
						const fn_connect_handle& hConnect );

			void	Stop( void );

			void	Run( UInt8 uNum = 1 );
			void	RunLoop( UInt8 uNum = 1 );

			void	SetHandleConnect( const fn_connect_handle& fn_handle )
			{
				_Connector.set_function(function20_bind(&NetAsyncConnector::HandleConnect, this,
									fn_handle, _foundation_::_1));
			}

			bool	is_open( void ) const
			{
				return _ioService.is_open();
			}

		protected:
			bool	HandleConnect( const fn_connect_handle& hConnect, tagIOData_INFO* pIOData );

		private:
			connector_type	_Connector;
			ThreadList		_Threads;
			io_service		_ioService;
		};
		/** @} end NetAsyncConnector */

	} //namespace _tcp_

} //_io_net_

#endif//__NETASYNCCONNECTOR_20160323201738_1458735458_H__
