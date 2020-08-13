
#ifndef __RAWCLIENT_20160521130520_1442717660_H__
#define __RAWCLIENT_20160521130520_1442717660_H__

#include <libNetFrame_SyncRawClients/External.h>

namespace	_client_{

	namespace	_sync_{

		using namespace _session_::_tcp_;

		typedef NETHANDLE						_HASH;
		typedef _io_net_::_tcp_::NetConnector	NetConnector;

		/*!
		* @class  RawClient
		* @brief   RawClient
		* @{
		*/

		class RawClients
		{
		public:
			typedef	Raw_HClose					Raw_HDestroy;

			typedef Container_HashMap_type<_HASH, RawSession_sptr>	HashMap_type;
			static	void	WorkThread( void* pParamter );
			static	void	MessageThread( void* pParamter );

		public:
			/*!
			* @function   RawClient
			* @brief    
			* @return  
			*/
			RawClients( void );

			/*!
			* @function   ~RawClient
			* @brief    
			* @return  
			*/
			~RawClients( void );

			void Release( void );

			void CloseAll( void );

			void Init( UInt8 uThreadNum = 1 );

			NETHANDLE Connect( const char* c_szIP, UInt16 uPort );

			int	Close( NETHANDLE Node );

			Int32	Send( NETHANDLE Node, const char* c_pData, UInt32 u32Size );

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(RawSession)
				EVENT_REGISTER_READ(&RawClients::HandleRecv)
				EVENT_REGISTER_CLOSE(&RawClients::HandleClose)
				EVENT_REGISTER_EXCEPT(&RawClients::HandleExcept)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			int	SetCloseHandle( NETHANDLE Node, const Raw_HClose& Handle )
			{
				RawSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetCloseHandle(Handle);
				return 1;
			}

			int	SetDestroyHandle( NETHANDLE Node, const Raw_HDestroy& hDestroy )
			{
				RawSession_sptr ptr = FindSession(Node);
				if( !ptr )
				{
					return -1;
				}

				ptr->SetDestroyHandle( function20_bind(&RawClients::HandleDestroy, this,
						hDestroy, Node) );
				return 1;
			}

			int	SetExceptHandle( NETHANDLE Node, const Raw_HExcept& Handle )
			{
				RawSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetExceptHandle(Handle);
				return 1;
			}

			int	SetRecvHandle( NETHANDLE Node, const Raw_HRecv& Handle )
			{
				RawSession_sptr sptr = FindSession(Node);
				if( !sptr )
				{
					return -1;
				}

				sptr->SetRecvHandle(Handle);
				return 1;
			}

			const _scoket_info&	GetSocketInfo( NETHANDLE Node )
			{
				RawSession_sptr sptr = FindSession(Node);
				if( sptr )
					return sptr->GetStream()->GetSocketInfo();
				return _scoket_info();
			}

		protected:
			const RawSession_sptr	FindSession( NETHANDLE Node );
			void	CloseSession( const RawSession_sptr& ptr );
			void	FreeSession( void* pSession );

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
			CLock			_Lock;
			CLock			_ReadLock;
			CLock			_QuitLock;
		};
		/** @} end CoZmmandClient */

	} //namespace	_sync_

} //namespace	_client_

typedef	_client_::_sync_::RawClients	syncRawClients_type;

inline syncRawClients_type* GetSyncRawClientsInstance( void )
{
	static syncRawClients_type* pClients = Singleton<syncRawClients_type>::instance();
	return pClients;
}

#endif//__RAWCLIENT_20150920105420_1442717660_H__
