
#ifndef __RAWSERVER_20150920172836_1442741316_H__
#define __RAWSERVER_20150920172836_1442741316_H__

#include "External.h"

namespace	_server_{

	using namespace _session_::_tcp_;

	typedef NETHANDLE						_HASH;
	typedef _io_net_::_tcp_::NetAcceptor	NetAcceptor;

	/*!
	* @class  RawServer
	* @brief   RawServer
	* @{
	*/
	typedef function20_handle<void (const char*, UInt16, NETHANDLE)>	Raw_HAccept;

	class RawServer
	{
	public:
		typedef	Raw_HClose					Raw_HDestroy;

		typedef Container_HashMap_type<_HASH, RawSession_sptr>	HashMap_type;
		static	void	WorkThread( void* pParamter );
		static	void	MessageThread( void* pParamter );

	public:
		/*!
		* @function   RawServer
		* @brief    
		* @return  
		*/
		RawServer( void );

		/*!
		* @function   ~RawServer
		* @brief    
		* @return  
		*/
		~RawServer( void );

		int Init(void);

		void Release( void );

		int	Send( NETHANDLE Node, const char* c_pData, UInt32 u32Size );

		void	Stop( void );
		void	Run( UInt32 u32Num );
		void	RunLoop( UInt32 u32Num );
		int	Close( NETHANDLE Node );

		bool	Listen( UInt16 u16Port, const Raw_HAccept& Handle )
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
				printf("%s\r\n",e.what());
				//std::cerr << e.what() << std::endl;
			}
			return false;
		}

		int	SetCloseHandle( NETHANDLE Node, const Raw_HClose& Handle )
		{
			RawSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetCloseHandle(Handle);
			return 1;
		}

		int	SetDestroyHandle( NETHANDLE Node, const Raw_HDestroy& hDestroy )
		{
			RawSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetDestroyHandle( function20_bind(&RawServer::HandleDestroy, this,
					hDestroy, Node) );
			return 1;
		}

		int	SetExceptHandle( NETHANDLE Node, const Raw_HExcept& Handle )
		{
			RawSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetExceptHandle(Handle);
			return 1;
		}

		int	SetRecvHandle( NETHANDLE Node, const Raw_HRecv& Handle )
		{
			RawSession_sptr ptr = FindSession(Node);
			if( !ptr )
			{
				return -1;
			}

			ptr->SetRecvHandle(Handle);
			return 1;
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
			EVENT_REGISTER_ACCEPT(&RawServer::HandleAccept)
		EVENT_REGISTER_LIST_END()

		EVENT_REGISTER_LIST_BEGIN(RawSession)
			EVENT_REGISTER_READ(&RawServer::HandleRecv)
			EVENT_REGISTER_CLOSE(&RawServer::HandleClose)
			EVENT_REGISTER_EXCEPT(&RawServer::HandleExcept)
		EVENT_REGISTER_LIST_END()
		/********************事件注册*****************************/

	protected:
		void	CloseAll( void );
		void	CloseSession( RawSession_sptr ptr );
		const RawSession_sptr	FindSession( NETHANDLE Node );
		void	FreeSession( void* pSession );

		void	HandleAccept( const _SOCKET_::HSOCKET& Socket );
		void	HandleRecv( RawSession* pSession, const StreamBuf_ptr& Buf_ptr );
		void	HandleClose( RawSession* pSession );
		void	HandleDestroy( const Raw_HDestroy& hDestroy, NETHANDLE Node );
		void	HandleExcept( RawSession* pSession, const char* strError );

		void SetCmdIdentity( _buf_hdr* Buf_ptr, RawSession* pSession )
		{
			((tagRawIdentity*)(StreamBuf::GetData(Buf_ptr) + StreamBuf::GetSize(Buf_ptr)))->_Identity = pSession;
		}

		RawSession* GetCmdIdentity( _buf_hdr* Buf_ptr ) const
		{
			return ((tagRawIdentity*)(StreamBuf::GetData(Buf_ptr) + StreamBuf::GetSize(Buf_ptr)))->_Identity;
		}

	private:
		PriorityQueue<StreamBuf_ptr>				_MQueue;
		LinkList_type<RawSession_sptr,CNoneLock>		_QuitQueue;

		long			_Count;
		HashMap_type	_Map;
		MemPool_type	_Pool;
		ThreadWrap		_Worker;
		ThreadWrap		_Message;
		NetAcceptor		_Accept;
		CLock			_Lock;
		CLock			_ReadLock;
		CLock			_QuitLock;
		Raw_HAccept		_AcceptHandle;
	};
	/** @} end RawServer */

} //namespace	_server_

inline _server_::RawServer* GetRawSvrInstance( void )
{
	static _server_::RawServer* pServer = Singleton<_server_::RawServer>::instance();
	return pServer;
}


#endif//__RAWSERVER_20150920172836_1442741316_H__
