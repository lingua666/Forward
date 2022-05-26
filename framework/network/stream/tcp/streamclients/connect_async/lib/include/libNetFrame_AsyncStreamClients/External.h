
#ifndef __EXTERNAL_20150914105604_1443339361_H__
#define __EXTERNAL_20150914105604_1443339361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAsyncConnector.h>
#include <libNetFrame_StreamSession/StreamSession.h>
#include <libFoundation/modafx.h>
#include <dllLog/Exports.h>

namespace	_client_{

	namespace	_async_{

		typedef	_session_::_tcp_::StreamSession	StreamSession;
		typedef	SmartPTR< StreamSession >	StreamSession_sptr;

	}//namespace	_async_

	LOG_OBJ_DECL(libNetFrame_AsyncStreamClients)

} //namespace _client_


#endif//__EXTERNAL_20150914105604_1443339361_H__
