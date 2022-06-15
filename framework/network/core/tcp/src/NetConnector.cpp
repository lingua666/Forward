
#include <libNetFrame_Core/NetConnector.h>
#include <libFoundation/modafx.h>

namespace _io_net_{

	namespace _tcp_{

		InitConnector::InitConnector( void )
			: _is_run(false)
		{

		}

		InitConnector::~InitConnector( void )
		{
			_ioService.close();
			for(std::size_t i = 0; i < _Threads.size(); ++ i)
			{
				_Threads[i]->join();
			}

			_Threads.clear();
		}

		int InitConnector::Run( UInt8 u8ThreadNum )
		{
			if( !is_run() )
			{
				_is_run = true;
				_ioService.open();
				for(UInt8 i = 0; i < u8ThreadNum; ++ i)
				{
					Thread_ptr thread( new Thread_type(
						function20_bind(&io_service::run, &_ioService)) );
					_Threads.push_back(thread);
				}
				return 1;
			}
			return -1;
		}

		int NetConnector::Init( UInt8 u8ThreadNum )
		{
			return Singleton<InitConnector>::instance()->Run(u8ThreadNum);
		}

		io_service& NetConnector::GetIoServer( void )
		{
			return Singleton<InitConnector>::instance()->_ioService;
		}

		_SOCKET_::HSOCKET NetConnector::Connect( const char* c_szIP, UInt16 u16Port )
		{
			_SOCKET_	Socket(Singleton<InitConnector>::instance()->_ioService);
			_SOCKET_::HSOCKET hSocket = Socket.Connect(c_szIP, u16Port);
			if( hSocket == INVALID_SOCKET )
				return	INVALID_SOCKET;

			Socket.Bind(hSocket);
			return hSocket;
		}

	} //namespace _tcp_

} //_io_net_

