
#ifndef __COMMANDSERVER_20150920172836_1442741316_H__
#define __COMMANDSERVER_20150920172836_1442741316_H__

#include <libNetFrame_CmdServer/External.h>

namespace	_server_{

	using namespace _session_::_tcp_;

	typedef NETHANDLE						_HASH;
	typedef _io_net_::_tcp_::NetAcceptor	NetAcceptor;

	/*!
	* @class  CommandServer
	* @brief   CommandServer
	* @{
	*/

	class CommandServer
	{
	public:
		typedef function20_handle<void (const char*, UInt16, NETHANDLE)>	Comamand_HAccept;
		typedef	Comamand_HClose								Comamand_HDestroy;

		typedef Container_HashMap_type<_HASH, CmdSession_sptr>	HashMap_type;
		static	void	WorkThread( void* pParamter );
		static	void	MessageThread( void* pParamter );

	public:
		/*!
		* @function   CommandServer
		* @brief    
		* @return  
		*/
		CommandServer( void );

		/*!
		* @function   ~CommandServer
		* @brief    
		* @return  
		*/
		~CommandServer( void );

		void Release( void );

		int	SendError( NETHANDLE Node, const char* c_pData, UInt16 u16Size );

		int	Send( NETHANDLE Node, const char* c_pData, UInt16 u16Size );

		void	Stop( void );
		void	Run( UInt8 uThreadNum = 0 );
		void	RunLoop( UInt8 uThreadNum = 0 );
		int	Close( NETHANDLE Node );

		bool	Listen( UInt16 u16Port, const Comamand_HAccept& Handle )
		{
			try
			{
				if( _Accept.Listen(u16Port) )
				{
					EVENT_REGISTER(&_Accept, this);
					_AcceptHandle = Handle;
					return true;
				}
			}
			catch (std::exception& e)
			{
				printf("%s\r\n", e.what());
				//std::cerr << e.what() << std::endl;
			}
			return false;
		}

		int	SetCloseHandle( NETHANDLE Node, const Comamand_HClose& Handle )
		{
			CmdSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetCloseHandle(Handle);
			return 1;
		}

		int	SetDestroyHandle( NETHANDLE Node, const Comamand_HDestroy& hDestroy )
		{
			CmdSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetDestroyHandle( function20_bind(&CommandServer::HandleDestroy, this,
				hDestroy, Node) );
			return 1;
		}

		int	SetExceptHandle( NETHANDLE Node, const Comamand_HExcept& Handle )
		{
			CmdSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetExceptHandle(Handle);
			return 1;
		}

		int	SetRecvHandle( NETHANDLE Node, const Comamand_HRecv& Handle )
		{
			CmdSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetRecvHandle(Handle);
			return 1;
		}

		int	SetSendCompleteNotify( NETHANDLE Node, const HSendComplete& Handle )
		{
			CmdSession_sptr sptr = FindSession(Node);
			if( !sptr )
			{
				return -1;
			}

			sptr->SetSendCompleteNotify(Handle);
			return 1;
		}

		const _scoket_info&	GetSocketInfo( NETHANDLE Node )
		{
			CmdSession_sptr sptr = FindSession(Node);
			if( sptr )
				return sptr->GetStream()->GetSocketInfo();
			return _scoket_info();
		}

		bool is_open( void )
		{
			return _Accept.is_open();
		}

		UInt32 GetCount( void ) const
		{
			return _Count;
		}

		/********************事件注册*****************************/
		EVENT_REGISTER_LIST_BEGIN(NetAcceptor)
			EVENT_REGISTER_ACCEPT(&CommandServer::HandleAccept)
		EVENT_REGISTER_LIST_END()

		EVENT_REGISTER_LIST_BEGIN(CmdSession)
			EVENT_REGISTER_READ(&CommandServer::HandleRecv)
			EVENT_REGISTER_CLOSE(&CommandServer::HandleClose)
			EVENT_REGISTER_EXCEPT(&CommandServer::HandleExcept)
		EVENT_REGISTER_LIST_END()
		/********************事件注册*****************************/

	protected:
		void	CloseAll( void );
		void	CloseSession( CmdSession_sptr ptr );
		const CmdSession_sptr	FindSession( NETHANDLE Node );
		void	FreeSession( void* pSession );

		void	HandleAccept( const _SOCKET_::HSOCKET& Socket );

		void	HandleRecv( CmdSession* pSession, const CmdDBuf_ptr& Buf_ptr );
		void	HandleClose( CmdSession* pSession );
		void	HandleDestroy( const Comamand_HDestroy& hDestroy, NETHANDLE Node );
		void	HandleExcept( CmdSession* pSession, const char* strError );

		void SetCmdIdentity( const CmdDBuf_ptr& Buf_ptr, CmdSession* pSession )
		{
			((tagCmdIdentity*)(Buf_ptr->_Data + Buf_ptr->_uPayload))->_Identity = pSession;
		}

		CmdSession* GetCmdIdentity( const CmdDBuf_ptr& Buf_ptr ) const
		{
			return ((tagCmdIdentity*)(Buf_ptr->_Data + Buf_ptr->_uPayload))->_Identity;
		}

	private:
		PriorityQueue<CmdDBuf_ptr>			_MQueue;
		LinkList_type<CmdSession_sptr,CNoneLock>	_QuitQueue;

		long			_Count;
		HashMap_type	_Map;
		MemPool_type	_Pool;
		ThreadWrap		_Worker;
		ThreadWrap		_Message;
		NetAcceptor		_Accept;
		CLock			_Lock;
		CLock			_ReadLock;
		CLock			_QuitLock;
		Comamand_HAccept	_AcceptHandle;
	};
	/** @} end CommandServer */

} //namespace	_server_

inline _server_::CommandServer* GetCommandServerInstance( void )
{
	static _server_::CommandServer* pServer = Singleton<_server_::CommandServer>::instance();
	return pServer;
}


#endif//__COMMANDSERVER_20150920172836_1442741316_H__
