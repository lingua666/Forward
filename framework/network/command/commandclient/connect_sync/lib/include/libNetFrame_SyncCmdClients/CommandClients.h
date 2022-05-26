
#ifndef __COMMANDCLIENT_20160405151120_1442717660_H__
#define __COMMANDCLIENT_20160405151120_1442717660_H__

#include <libNetFrame_SyncCmdClients/External.h>

namespace	_client_{

	namespace _sync_{

		using namespace _session_::_tcp_;

		typedef NETHANDLE						_HASH;
		typedef _io_net_::_tcp_::NetConnector	NetConnector;

		/*!
		* @class  CommandClient
		* @brief   CommandClient
		* @{
		*/

		class CommandClients
		{
		public: 
			typedef	Comamand_HClose									Comamand_HDestroy;

			typedef Container_HashMap_type<_HASH, CmdSession_sptr>	HashMap_type;
			static	void	WorkThread( void* pParamter );
			static	void	MessageThread( void* pParamter );

		public:
			/*!
			* @function   CommandClient
			* @brief    
			* @return  
			*/
			CommandClients( void );

			/*!
			* @function   ~CommandClient
			* @brief    
			* @return  
			*/
			~CommandClients( void );

			void CloseAll( void );

			void Release( void );

			void Init( UInt8 uThreadNum = 1 );

			NETHANDLE Connect( const char* c_szIP, UInt16 uPort );
			int Close( NETHANDLE Node );
			int	SendError( NETHANDLE Node, const char* c_pData, UInt32 uSize );
			int	Send( NETHANDLE Node, const char* c_pData, UInt32 uSize );

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(CmdSession)
				EVENT_REGISTER_READ(&CommandClients::HandleRecv)
				EVENT_REGISTER_CLOSE(&CommandClients::HandleClose)
				EVENT_REGISTER_EXCEPT(&CommandClients::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			int	SetCloseHandle( NETHANDLE Node, const Comamand_HClose& Handle )
			{
				CmdSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetCloseHandle(Handle);
				return 1;
			}

			int	SetDestroyHandle( NETHANDLE Node, const Comamand_HDestroy& hDestroy )
			{
				CmdSession_sptr ptr = FindSession(Node);
				if( !ptr )
				{
					return -1;
				}

				ptr->SetDestroyHandle( function20_bind(&CommandClients::HandleDestroy, this,
										hDestroy, Node) );
				return 1;
			}

			int	SetExceptHandle( NETHANDLE Node, const Comamand_HExcept& Handle )
			{
				CmdSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetExceptHandle(Handle);
				return 1;
			}

			int	SetRecvHandle( NETHANDLE Node, const Comamand_HRecv& Handle )
			{
				CmdSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetRecvHandle(Handle);
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

		protected:
			void	CloseSession( const CmdSession_sptr& ptr );
			const CmdSession_sptr	FindSession( NETHANDLE Node );
			void	FreeSession( void* pSession );

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
			PriorityQueue<CmdDBuf_ptr>				_MQueue;
			LinkList_type<CmdSession_sptr,CNoneLock>		_QuitQueue;

			long			_Count;
			HashMap_type	_Map;
			MemPool_type	_Pool;
			ThreadWrap		_Worker;
			ThreadWrap		_Message;
			CLock			_Lock;
			CLock			_ReadLock;
			CLock			_QuitLock;
		};
		/** @} end CoZmmandClient */

	} //namespace _sync_

} //namespace	_client_

typedef	_client_::_sync_::CommandClients	syncCommandClients_type;

inline syncCommandClients_type* GetSyncCmdClientsInstance( void )
{
	static syncCommandClients_type* pClients = Singleton<syncCommandClients_type>::instance();
	return pClients;
}

#endif//__COMMANDCLIENT_20150920105420_1442717660_H__
