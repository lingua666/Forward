
#include <libNetFrame_Core/NetAsyncConnector.h>

namespace _io_net_{

	namespace _tcp_{

		NetAsyncConnector::NetAsyncConnector( void )
		{

		}

		NetAsyncConnector::~NetAsyncConnector( void )
		{
			Stop();
		}

		bool	NetAsyncConnector::Listen( void )
		{
			_ioService.open();
			_Connector.attach(_ioService);
			_Connector.listen();
			return true;
		}

		int	NetAsyncConnector::Connect( const char* szIP, UInt16 uPort )
		{
			if( !is_open() )
				return -1;

			return _Connector.async_connect(szIP, uPort);
		}

		int	NetAsyncConnector::Connect( const char* szIP, UInt16 uPort,
			const fn_connect_handle& hConnect )
		{
			if( !is_open() )
				return -1;

			return _Connector.async_connect(szIP, uPort, 
				function20_bind(&NetAsyncConnector::HandleConnect, this,
				hConnect, _foundation_::_1));
		}

		void	NetAsyncConnector::Stop( void )
		{
			_ioService.stop();
			_ioService.close();
			_Connector.close();

			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->join();
			}
			_Threads.clear();
		}

		void NetAsyncConnector::Run( UInt8 uNum )
		{
			for(std::size_t i = 0; i < uNum; ++ i)
			{
				Thread_ptr thread( new Thread_type(
					function20_bind(&io_service::run, &_ioService)) );
				_Threads.push_back(thread);
				thread->wait_run();
			}
		}

		void NetAsyncConnector::RunLoop( UInt8 uNum )
		{
			for(std::size_t i = 0; i < uNum; ++ i)
			{
				Thread_ptr thread( new Thread_type( function20_bind(&io_service::run, &_ioService)) );
				_Threads.push_back(thread);
			}

			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->join();
			}

			_Threads.clear();
		}

		bool	NetAsyncConnector::HandleConnect( const fn_connect_handle& hConnect, tagIOData_INFO* pIOData )
		{
			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if(pOver->_ADR._Sock == INVALID_SOCKET )
			{
				if( hConnect )
					hConnect(-1);
			}
			else
			{
				if( hConnect )
					hConnect(pOver->_ADR._Sock);
			}
			return true;
		}

	} //namespace _tcp_

} //_io_net_
