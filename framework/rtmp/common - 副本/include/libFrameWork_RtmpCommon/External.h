
#ifndef __EXTERNAL_RTMP_20201009172304_3344199361_H__
#define __EXTERNAL_RTMP_20201009172304_3344199361_H__

#include <libNetFrame_AsyncStreamClients/StreamClients.h>
#include <libNetFrame_StreamServer/StreamServer.h>
#include <libNetFrame_StreamServer_V20/StreamServer_V20.h>
#include <libFoundation/modafx.h>
#include <dllLog/Exports.h>
#include <rtmp.h>
#include <libFrameWork_Rtsp/IVideoInfo.h>

namespace _rtmp_ {

	typedef _foundation_::_string_t			_rtmp_string;
	typedef _foundation_::_string_big_t		_rtmp_string_big_t;
	typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;

}

LOG_OBJ_DECL(libFrameWork_RtmpCommon)

#endif//__EXTERNAL_20201009172304_3344199361_H__
