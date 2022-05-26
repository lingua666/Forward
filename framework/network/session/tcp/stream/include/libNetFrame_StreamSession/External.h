
#ifndef __EXTERNAL_20160502165604_2442199361_H__
#define __EXTERNAL_20160502165604_2442199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAcceptor.h>
#include <libMessage/PriorityQueue.h>
#include <libNetFrame_Packet/Packet_Raw.h>
#include <libFoundation/modafx.h>
#include <dllLog/Exports.h>

typedef Packet_Raw		MediaNPacket;
typedef	PKBuf_ptr		MediaDBuf_ptr;
typedef PKBList_ptr		MPList_ptr;

LOG_OBJ_DECL(libNetFrame_StreamSession)

#endif//__EXTERNAL_20150914105604_2442199361_H__
