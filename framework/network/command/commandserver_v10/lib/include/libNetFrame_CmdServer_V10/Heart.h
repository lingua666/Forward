
#ifndef __HEART_V10_20151014100020_1444788020_H__
#define __HEART_V10_20151014100020_1444788020_H__

#include <libNetFrame_CmdServer_V10/External.h>

namespace	_server_{

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
			bool	_isValid;
			
			stHeartInfo( const CmdSession_sptr& Session_ptr )
				: _Session_ptr(Session_ptr)
			{
				_u32CurrentTimer = GetTickCount();
				_isValid = true;
			}

			~stHeartInfo( void )
			{

			}

			void Update( void )
			{
				Reset();
			}

			void Reset( void )
			{
				_u32CurrentTimer = GetTickCount();
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
			}
			s_Lock.UnLock();
		}

		static	void	Detach( NETHANDLE Node )
		{
			s_Lock.Lock();
			HashMap_type::iterator iter = s_HashMap.find(Node);
			if( iter != s_HashMap.end() )
			{
				iter->second->_u32CurrentTimer = 0;//非超时关闭
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
				if( GetTickCount() - pHeart->_u32CurrentTimer > HEART_INTERVAL_TIME * HEART_RESEND_TIME + 5000 )
				{
					s_List.pop();
					if( pHeart->_u32CurrentTimer != 0 )
					{//超时断开
						s_Lock.UnLock();
						pHeart->_Session_ptr->HeartTimeOut();
					}
					else
					{//主动断开
						s_Lock.UnLock();
					}

					if( pHeart->_isValid )
						Detach( pHeart->_Session_ptr->GetRemoteNode() );

					s_Pool.FreeObj( pHeart );
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

		static void	Send( CmdSession* pSession )
		{
			//Timestamp_type::UtcTimeVal UtcTime = Timestamp_type().utcTime();
			UInt32 UtcTime = Timestamp_type().unixTime();

			//心跳包
			_session_hdr Hdr;
			Hdr._uType = SESSION_HEART_TYPE_DEF;
			Hdr._uPriority = 0;
			pSession->Send(&Hdr, reinterpret_cast<char*>(&UtcTime), sizeof(UtcTime));
		}

	private:
		static List_type	s_List;
		static HashMap_type	s_HashMap;
		static CLock		s_Lock;
		static MemPool_type		s_Pool;
	};

	Heart::List_type	Heart::s_List;
	Heart::HashMap_type	Heart::s_HashMap;
	CLock				Heart::s_Lock;
	MemPool_type		Heart::s_Pool( sizeof(Heart::stHeartInfo) );

} //namespace	_server_

#endif//__HEART_20151014100020_1444788020_H__
