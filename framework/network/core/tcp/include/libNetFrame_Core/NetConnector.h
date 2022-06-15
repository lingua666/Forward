
#ifndef __NETCONNECTOR_20150721151608_1437462968_H__
#define __NETCONNECTOR_20150721151608_1437462968_H__

#include <libNetFrame_Core/External.h>
#include <libNetFrame_Core/SockStream.h>

#define REVBUF_SIZE		1460			//1450-1470
#define SENDBUF_SIZE	REVBUF_SIZE

namespace _io_net_{

	namespace _tcp_{

		struct NetConnector;

		class InitConnector
		{
		public:
			typedef ThreadWrap				Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;

			friend struct NetConnector;

		public:
			InitConnector( void );
			~InitConnector( void );
			int	Run( UInt8 u8ThreadNum );
			bool	is_run( void ) const
			{ return _is_run; }

		private:
			io_service	_ioService;
			ThreadList	_Threads;
			bool	_is_run;
		};

		/*!
		* @class  NetConnector
		* @brief   NetConnector
		* @{
		*/
		struct NetConnector
		{
			static int Init( UInt8 u8ThreadNum );

			static io_service& GetIoServer( void );

			static _SOCKET_::HSOCKET Connect( const char* c_szIP, UInt16 u16Port );

			template<typename H>
			static void	Post( H handle )
			{
				NetConnector::GetIoServer().post(handle);
			}
		};

		/** @} end NetConnector */

	} //namespace _tcp_

} //_io_net_

#endif//__NETCONNECTOR_20150721151608_1437462968_H__
