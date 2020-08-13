
#ifndef __EXTERNAL_20160502105604_1442199361_H__
#define __EXTERNAL_20160502105604_1442199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAsyncConnector.h>
#include <libNetFrame_CmdSession/CommandSession.h>
#include <libFoundation/modafx.h>
#include <dllLog/Exports.h>

namespace	_client_{

	typedef _session_::_tcp_::_session_hdr						_session_hdr;
	typedef	_session_::_tcp_::CommandSession< _session_hdr >	CmdSession;
	typedef	SmartPTR< CmdSession >		CmdSession_sptr;

	struct __M_ALIGNED_PACKED__ tagCmdIdentity
	{
		CmdSession*	_Identity;
	};

	LOG_OBJ_DECL(NetFrame_AsyncCmdClients)

} // namespace _client_

#endif//__EXTERNAL_20150914105604_1442199361_H__
