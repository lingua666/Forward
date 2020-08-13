
#ifndef __EXTERNAL_20150506115004_2222199361_H__
#define __EXTERNAL_20150506115004_2222199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAcceptor.h>
#include <libNetFrame_CmdSession/CommandSession.h>
#include <libMessage/PriorityEvent.h>
#include <libFoundation/modafx.h>

namespace	_server_{

	typedef _session_::_tcp_::_session_hdr					_session_hdr;
	typedef	_session_::_tcp_::CommandSession<_session_hdr>	CmdSession;
	typedef	SmartPTR< CmdSession >		CmdSession_sptr;

	struct __M_ALIGNED_PACKED__ tagCmdIdentity
	{
		CmdSession*	_Identity;
	};

} // namespace	_server_

#endif//__EXTERNAL_20150914105604_2222199361_H__
