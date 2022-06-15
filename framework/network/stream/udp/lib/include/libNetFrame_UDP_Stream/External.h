
#ifndef __EXTERNAL_20171124154704_2442199361_H__
#define __EXTERNAL_20171124154704_2442199361_H__

#include <libNetFrame_UDP_Core/SockStream.h>
#include <libNetFrame_UDP_StreamSession/StreamSession.h>
#include <libFoundation/modafx.h>

namespace	_server_{

	namespace	_udp_{

		typedef	_session_::_udp_::StreamSession	StreamSession;
		typedef	SmartPTR<StreamSession>			StreamSession_sptr;

		struct __M_ALIGNED_PACKED__ tagStreamIdentity
		{
			StreamSession*	_Identity;
		};

	} //namespace	_udp_

} //namespace	_server_

#endif//__EXTERNAL_20150914105604_2442199361_H__
