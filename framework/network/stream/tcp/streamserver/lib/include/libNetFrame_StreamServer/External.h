
#ifndef __EXTERNAL_20150914105604_2442199361_H__
#define __EXTERNAL_20150914105604_2442199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAcceptor.h>
#include <libFoundation/modafx.h>
#include <libNetFrame_StreamSession/StreamSession.h>

namespace	_server_{

	namespace	_tcp_{

		typedef	_session_::_tcp_::StreamSession	StreamSession;
		typedef	SmartPTR<StreamSession>			StreamSession_sptr;

	} //namespace	_tcp_

} //namespace	_server_

#endif//__EXTERNAL_20150914105604_2442199361_H__
