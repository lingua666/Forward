
#ifndef __EXTERNAL_20171122165104_2442199361_H__
#define __EXTERNAL_20171122165104_2442199361_H__

#include <libNetFrame_UDP_Core/SockStream.h>
#include <libMessage/PriorityQueue.h>
#include <libNetFrame_Packet/Packet_Raw_V2.h>
#include <libFoundation/modafx.h>

namespace	_session_{

	namespace	_udp_{

		typedef Packet_Raw_V2	MediaNPacket;
		typedef Packet_Raw_V2::tagBuf_sptr	MediaDBuf_ptr;
		typedef Packet_Raw_V2::BufList_sptr	MPList_ptr;

	} //namespace	_udp_

} //namespace	_session_

#endif//__EXTERNAL_20150914105604_2442199361_H__
