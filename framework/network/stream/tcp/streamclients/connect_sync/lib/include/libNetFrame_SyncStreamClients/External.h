
#ifndef __EXTERNAL_20180622225704_1443339361_H__
#define __EXTERNAL_20180622225704_1443339361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetConnector.h>
#include <libFoundation/modafx.h>
#include <libNetFrame_StreamSession/StreamSession.h>

namespace	_client_{

	namespace _sync_{

		typedef _session_::_tcp_::StreamSession	StreamSession;
		typedef	SmartPTR<StreamSession>			StreamSession_sptr;

	} //namespace _sync_;

} //namespace _client_;


#endif//__EXTERNAL_20150914105604_1443339361_H__
