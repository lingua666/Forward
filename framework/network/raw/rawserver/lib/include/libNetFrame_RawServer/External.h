
#ifndef __EXTERNAL_20160502195604_2222199361_H__
#define __EXTERNAL_20160502195604_2222199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAcceptor.h>
#include <libNetFrame_RawSession/RawSession.h>
#include <libFoundation/modafx.h>

#define PLList		LinkList_type

namespace	_server_{

	typedef	_session_::_tcp_::RawSession	RawSession;
	typedef	SmartPTR<RawSession>	RawSession_sptr;

	struct __M_ALIGNED_PACKED__ tagRawIdentity
	{
		RawSession*	_Identity;
	};
}


#endif//__EXTERNAL_20150914105604_2222199361_H__
