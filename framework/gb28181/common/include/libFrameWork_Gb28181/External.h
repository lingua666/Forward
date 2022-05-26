
#ifndef __EXTERNAL_20171226154204_3344199361_H__
#define __EXTERNAL_20171226154204_3344199361_H__

#include <libNetFrame_AsyncStreamClients/StreamClients.h>
#include <libNetFrame_StreamServer/StreamServer.h>
#include <libNetFrame_UDP_Stream/AppMain.h>
#include <libFoundation/modafx.h>
#include <libCommon/os/Types.h>
#include <libMessage/PriorityQueueV2.h>
#include <dllLog/Exports.h>
#include <libNetFrame_Packet/IPack.h>
#include <libProto_Sip_V20/modafx.h>
#include <libThirdWrap_Xml/XML.h>
#include <libTimestamp/Timestamp.h>

#define SIP_NAMESPACE _sip_v20_

namespace	_gb28181_ {

	typedef SIP_NAMESPACE::_string_t			_gb_string;
	typedef SIP_NAMESPACE::_string_big_t		_gb_string_big;

}

LOG_OBJ_DECL(libFrameWork_GB28181)

#endif//__EXTERNAL_20150914105604_3344199361_H__
