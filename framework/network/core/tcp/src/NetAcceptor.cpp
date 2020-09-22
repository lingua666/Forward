
#include <libNetFrame_Core/NetAcceptor.h>

namespace _io_net_{

	namespace _tcp_{

		NetAcceptor::NetAcceptor( void )
		{

		}

		NetAcceptor::~NetAcceptor( void )
		{
			Stop();
		}

		void	NetAcceptor::Stop( void )
		{
			_ioService.stop();
			_Acceptor.close();

			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->join();
			}
			_Threads.clear();
			_ioService.close();
		}

		void NetAcceptor::Run( UInt8 uNum )
		{
			for(std::size_t i = 0; i < uNum; ++ i)
			{
				PostAccept();
				Thread_ptr thread( new Thread_type(
					function20_bind(&io_service::run, &_ioService)) );
				_Threads.push_back(thread);
				thread->wait_run();
			}
		}

		void NetAcceptor::RunLoop( UInt8 uNum )
		{
			for(std::size_t i = 0; i < uNum; ++ i)
			{
				PostAccept();
				Thread_ptr thread( new Thread_type( function20_bind(&io_service::run, &_ioService)) );
				_Threads.push_back(thread);
			}

			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->join();
			}

			_Threads.clear();
		}

		bool	NetAcceptor::PostAccept( void )
		{
			_Acceptor.async_accept( function20_bind(&NetAcceptor::HandleAccept, this,
				_foundation_::_1) );
			return true;
		}

		bool	NetAcceptor::PostAccept( tagOverlapped* pOverlapped )
		{
			if( pOverlapped == NULL )
				return false;

			_Acceptor.async_accept( pOverlapped );
			return true;
		}

		void	NetAcceptor::HandleAccept( tagIOData_INFO* pIOData )
		{
			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if( pOver->_ADR._Sock == INVALID_SOCKET )
			{
				PostAccept(pOver);
			}
			else
			{
				_SOCKET_::HSOCKET Socket = pOver->_ADR._Sock;
				PostAccept(pOver);
				EVENT_EXEC_ACCEPT(Socket);
			}
		}

	} //namespace _tcp_

} //_io_net_