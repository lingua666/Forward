
#include <libNetFrame_Core/NetAcceptor_V20.h>

namespace _io_net_{

	namespace _tcp_{

		NetAcceptor_V20::NetAcceptor_V20( void )
		{

		}

		NetAcceptor_V20::~NetAcceptor_V20( void )
		{
			Release();
		}

		int NetAcceptor_V20::Init(UInt8 uNum)
		{
			if (_ioService.open() == -1)
				return -1;

			_Acceptor.attach(_ioService);

			if (_Threads.size() <= 0)
			{
				for (std::size_t i = 0; i < uNum; ++i)
				{
					Thread_ptr thread(new Thread_type(
						function20_bind(&io_service::run, &_ioService)));
					_Threads.push_back(thread);
					thread->wait_run();
				}
			}

			return 1;
		}

		void NetAcceptor_V20::Release(void)
		{
			_ioService.stop();

			while (_SocketMap.size() > 0)
			{
				Close(_SocketMap.begin()->second);
			}
			_SocketMap.clear();

			for (std::size_t i = 0; i < _Threads.size(); ++i)
			{
				_Threads[i]->join();
			}
			_Threads.clear();
			_ioService.close();
		}

		const io_service& NetAcceptor_V20::GetIoServer(void) const
		{
			return _ioService;
		}

		socket_type::HSOCKET NetAcceptor_V20::Open(UInt16 u16Port, const char* c_szIP)
		{
			socket_type::HSOCKET Sock = _Acceptor.bind(u16Port, c_szIP);

			if (Sock == INVALID_SOCKET)
				return INVALID_SOCKET;

			if (_Acceptor.listen(Sock) == -1)
			{//¼àÌý¶Ë¿ÚÊ§°Ü
				_Acceptor.close(Sock);
				return INVALID_SOCKET;
			}

			_Lock.Lock();
			_SocketMap.insert(std::make_pair(Sock, Sock));
			_Lock.UnLock();

			PostAccept(Sock);

			return Sock;
		}

		void	NetAcceptor_V20::Close(const socket_type::HSOCKET& Sock)
		{
			_Acceptor.close(Sock);

			_Lock.Lock();
			_SocketMap.erase(Sock);
			_Lock.UnLock();
		}

		bool	NetAcceptor_V20::PostAccept(const socket_type::HSOCKET& Sock)
		{
			_Acceptor.async_accept(Sock, function20_bind(&NetAcceptor_V20::HandleAccept, this,
				Sock, _foundation_::_1) );
			return true;
		}

		bool	NetAcceptor_V20::PostAccept(const socket_type::HSOCKET& Sock, tagOverlapped* pOverlapped)
		{
			if( pOverlapped == NULL )
				return false;

			_Acceptor.async_accept(Sock, pOverlapped);
			return true;
		}

		void	NetAcceptor_V20::HandleAccept(const socket_type::HSOCKET& Sock, tagIOData_INFO* pIOData )
		{
			tagOverlapped* pOver = (tagOverlapped*)pIOData->_pOverlapped;

			if( pOver->_ADR._Sock == INVALID_SOCKET )
			{
				PostAccept(Sock, pOver);
			}
			else
			{
				_SOCKET_::HSOCKET Socket = pOver->_ADR._Sock;
				PostAccept(Sock, pOver);
				if (_hAccept)
					_hAccept(Socket);
			}
		}

	} //namespace _tcp_

} //_io_net_