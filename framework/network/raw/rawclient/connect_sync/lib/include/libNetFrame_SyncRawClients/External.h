
#ifndef __EXTERNAL_20160521105604_1442199361_H__
#define __EXTERNAL_20160521105604_1442199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetConnector.h>
#include <libNetFrame_RawSession/RawSession.h>
#include <libFoundation/modafx.h>

#define PLList			LinkList_type

namespace	_client_{

	namespace _sync_{

		typedef	_session_::_tcp_::RawSession	RawSession;
		typedef	SmartPTR<RawSession>			RawSession_sptr;

		struct __M_ALIGNED_PACKED__ tagRawIdentity
		{
			RawSession*	_Identity;
		};

	} //namespace	_sync_

} //namespace	_client_

#endif//__EXTERNAL_20151223105604_1442199361_H__
