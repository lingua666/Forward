
#ifndef __NETACCEPTOR_V20_20150721153706_1437464226_H__
#define __NETACCEPTOR_V20_20150721153706_1437464226_H__

#include <libNetFrame_Core/Socket.h>
#include <libNetFrame_Core_Common/Event_Handle.h>
#include <libNetFrame_Core/SockStream.h>

namespace _io_net_{

	namespace _tcp_{

		/*!
		* @class  NetAcceptor_V20
		* @brief   NetAcceptor_V20
		* @{
		*/

		class NetAcceptor_V20
		{
		public:
			typedef ThreadWrap									Thread_type;
			typedef SmartPTR<Thread_type>						Thread_ptr;
			typedef std::vector<Thread_ptr>						ThreadList;
			typedef function20_handle<void(const socket_type::HSOCKET&)>	HFNAccept;
			typedef Container_HashMap_type<socket_type::HSOCKET, socket_type::HSOCKET>	Socket_Map;

		public:
			NetAcceptor_V20( void );

			~NetAcceptor_V20( void );

			int Init(UInt8 uNum = 1);

			void Release( void );

			const io_service& GetIoServer(void) const;

			socket_type::HSOCKET Open(UInt16 u16Port, const char* c_szIP = NULL);

			void	Close(const socket_type::HSOCKET& Sock);

			void	SetAcceptHandle(const HFNAccept& hAccept)
			{
				_hAccept = hAccept;
			}

			bool	is_open( void ) const
			{
				return _Acceptor.is_open();
			}

			template<typename H>
			void	Post( H handle )
			{
				_ioService.post(handle);
			}

		protected:
			bool	PostAccept(const socket_type::HSOCKET& Sock);

			bool	PostAccept(const socket_type::HSOCKET& Sock, tagOverlapped* pOverlapped);

			void	HandleAccept(const socket_type::HSOCKET& Sock, tagIOData_INFO* pIOData);

		private:
			acceptor_V20_type	_Acceptor;
			ThreadList		_Threads;
			io_service		_ioService;
			Socket_Map		_SocketMap;
			CLock			_Lock;
			HFNAccept		_hAccept;
		};
		/** @} end NetAcceptor_V20 */

	} //namespace _tcp_

} //_io_net_



#endif//__NETACCEPTOR_V20_20150721153706_1437464226_H__
