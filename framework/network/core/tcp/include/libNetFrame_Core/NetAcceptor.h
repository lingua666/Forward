
#ifndef __NETACCEPTOR_20150721153706_1437464226_H__
#define __NETACCEPTOR_20150721153706_1437464226_H__

#include <libNetFrame_Core/Socket.h>
#include <libNetFrame_Core/INetAcceptor.h>

namespace _io_net_{

	namespace _tcp_{

		/*!
		* @class  NetAcceptor
		* @brief   NetAcceptor
		* @{
		*/

		class NetAcceptor : public INetAcceptor
		{
		public:
			typedef ThreadWrap				Thread_type;
			typedef SmartPTR<Thread_type>	Thread_ptr;
			typedef std::vector<Thread_ptr>	ThreadList;

		public:

			NetAcceptor( void );

			~NetAcceptor( void );

			const io_service& GetIoServer( void ) const
			{
				return _ioService;
			}

			inline bool	Listen( UInt16 u16Port, const char* c_szIP = NULL )
			{
				_ioService.open();
				_Acceptor.attach(_ioService);
				if( _Acceptor.bind(u16Port, c_szIP) == -1 )
					return false;
				_Acceptor.listen();
				return true;
			}

			void	Stop( void );

			void	Run( UInt8 uNum = 1 );
			void	RunLoop( UInt8 uNum = 1 );

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
			bool	PostAccept( void );

			bool	PostAccept( tagOverlapped* pOverlapped );

			void	HandleAccept( tagIOData_INFO* pIOData );

		private:
			acceptor_type	_Acceptor;
			ThreadList		_Threads;
			io_service		_ioService;
		};
		/** @} end NetAcceptor */

	} //namespace _tcp_

} //_io_net_



#endif//__NETACCEPTOR_20150721153706_1437464226_H__
