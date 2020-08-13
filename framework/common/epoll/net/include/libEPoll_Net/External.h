
#ifndef __EXTERNAL_20170505115504_3344199361_H__
#define __EXTERNAL_20170505115504_3344199361_H__

#include <libFoundation/modafx.h>
#include <libCommon/os/Types.h>
#include <libCommon/os/winsock2i.h>
#include <libMessage/PriorityQueueV2.h>
#include <dllLog/Exports.h>

#define	function_v10 function10_handle
#define	function_v20 function20_handle

#pragma pack(push,1)  
struct WSABUF
{
	UInt32	len;	/* the length of the buffer */
	char	*buf;	/* the pointer to the buffer */
};

struct tagIOData_INFO
{
	Int32	_ibytes_transferred;
	void*	_pOverlapped;
};
#pragma pack(pop)

typedef	function_v20<void (tagIOData_INFO*)> HFNIOData;
typedef void*	LPOVERLAPPED;

LOG_OBJ_DECL(libEPoll_Net)

#endif//__EXTERNAL_20150914105604_3344199361_H__
