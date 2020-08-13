
#ifndef __EXTERNAL_20160428213104_1442199361_H__
#define __EXTERNAL_20160428213104_1442199361_H__

#include <libNetFrame_Core/SockStream.h>
#include <libNetFrame_Core/NetAsyncConnector.h>
#include <libMessage/PriorityQueue.h>
#include <libNetFrame_Packet/Packet.h>
#include <libFoundation/modafx.h>

#include <dllLog/Exports.h>

typedef Packet<PHead>	CmdNPacket;
typedef	PKBuf_ptr		CmdDBuf_ptr;
typedef PKBList_ptr		CmdList_ptr;

LOG_OBJ_DECL(NetFrame_CmdSession)

#endif//__EXTERNAL_20160428213104_1442199361_H__
