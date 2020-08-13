#ifndef __CHEARTHEADER_20160405151120_1442717660_H__
#define __CHEARTHEADER_20160405151120_1442717660_H__

#include <libNetFrame_SyncCmdClients/External.h>

namespace	_client_{

	namespace _sync_{

#define HASH_SIZE	100000
#define HASH_BIT	64
		typedef NETHANDLE		_HASH;

#define	HEART_INTERVAL_TIME	60000		//单位毫秒
#define	HEART_RESEND_TIME	3			//

		class Heart
		{
		public:

			struct  stHeartInfo
			{
				CmdSession_sptr _Session_ptr;
				UInt32	_u32CurrentTimer;
				UInt32	_u32Remain;
				bool	_isValid;

				stHeartInfo( const CmdSession_sptr& Session_ptr )
				{
					_Session_ptr = Session_ptr;
					_u32CurrentTimer = GetTickCount();
					_u32Remain = HEART_RESEND_TIME;
					_isValid = true;
				}

				~stHeartInfo( void )
				{

				}

				void Update( void )
				{
					_u32CurrentTimer = GetTickCount();
					_u32Remain = HEART_RESEND_TIME;
				}

				void Reset( void )
				{
					if( _u32Remain != 0 )
					{
						_u32CurrentTimer = GetTickCount();
					}
				}
			};

			typedef Container_HashMap_type<_HASH, stHeartInfo*>	HashMap_type;
			typedef	_container_::CycleList<stHeartInfo*, CNoneLock>		List_type;

			static void Release( void )
			{
				s_List.clear();
				s_HashMap.clear();
				s_Pool.Release();
			}

			static void Clear( void )
			{
				s_List.clear();
				s_HashMap.clear();
			}

			static	void	Attach( NETHANDLE Node, const CmdSession_sptr& Session_ptr )
			{
				stHeartInfo* ptr = (s_Pool.AllocObj<stHeartInfo>(Session_ptr));
				s_Lock.Lock();
				s_HashMap.insert(std::make_pair(Node, ptr));
				s_List.push_back(ptr);
				s_Lock.UnLock();
			}

			static	void	Stop( NETHANDLE Node )
			{
				s_Lock.Lock();
				HashMap_type::iterator iter = s_HashMap.find(Node);
				if( iter != s_HashMap.end() )
				{
					iter->second->_u32CurrentTimer = 0;
					iter->second->_u32Remain = 0;
				}
				s_Lock.UnLock();
			}

			static	void	Detach( NETHANDLE Node )
			{
				s_Lock.Lock();
				HashMap_type::iterator iter = s_HashMap.find(Node);
				if( iter != s_HashMap.end() )
				{
					iter->second->_u32CurrentTimer = 0;
					iter->second->_u32Remain = 0;
					iter->second->_isValid = false;
					s_HashMap.erase(iter);
				}
				s_Lock.UnLock();
			}

			static void		CheckHeart( void )
			{
				if( s_List.size() > 0 )
				{
					s_Lock.Lock();
					stHeartInfo* pHeart = s_List.current();
					if( pHeart && GetTickCount() - pHeart->_u32CurrentTimer > HEART_INTERVAL_TIME )
					{
						s_List.pop();
						if( pHeart->_u32CurrentTimer != 0 && pHeart->_u32Remain != 0 )
						{//重发
							pHeart->Reset();
							s_List.push_back(pHeart);
							pHeart->_u32Remain --;
							Send(pHeart->_Session_ptr);
							s_Lock.UnLock();
						}
						else
						{
							if( pHeart->_u32CurrentTimer != 0 )
							{//超时断开
								s_Lock.UnLock();
								pHeart->_Session_ptr->HeartTimeOut();
							}
							else 
							{ // 主动断开
								s_Lock.UnLock();
							}

							if( pHeart->_isValid )
								Detach( pHeart->_Session_ptr->GetLocalNode() );

							s_Pool.FreeObj(pHeart);
						}
					}
					else
					{
						s_List.next();
						s_Lock.UnLock();
					}
				}
			}

			static void	Update( NETHANDLE Node )
			{
				s_Lock.Lock();
				HashMap_type::iterator iter = s_HashMap.find(Node);
				if( iter != s_HashMap.end() )
				{
					iter->second->Update();
				}
				s_Lock.UnLock();
			}

		protected:
			static void	Send( const CmdSession_sptr& Session_ptr )
			{
				UInt32 UnixTime = Timestamp_type().unixTime();
				//心跳包
				_session_hdr	Hdr;
				Hdr._uType = SESSION_HEART_TYPE_DEF;
				Hdr._uPriority = 0;
				Session_ptr->Send(&Hdr, reinterpret_cast<char*>(&UnixTime), sizeof(UnixTime));
			}

		private:
			static List_type	s_List;
			static HashMap_type	s_HashMap;
			static MemPool_type		s_Pool;
			static CLock		s_Lock;
		};

		Heart::List_type	Heart::s_List;
		Heart::HashMap_type	Heart::s_HashMap;
		CLock				Heart::s_Lock;
		MemPool_type		Heart::s_Pool( sizeof(Heart::stHeartInfo) );

	} //_sync

} //namespace	_client_

#endif