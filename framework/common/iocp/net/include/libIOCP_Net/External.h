
#ifndef __EXTERNAL_20150914105604_3344199361_H__
#define __EXTERNAL_20150914105604_3344199361_H__

#include <libFoundation/modafx.h>

#define	function_v10 function10_handle
#define	function_v20 function20_handle

struct tagIOData_INFO
{
	Int32	_ibytes_transferred;
	void*	_pOverlapped;
};

typedef	function_v20<void (tagIOData_INFO*)> HFNIOData;

#endif//__EXTERNAL_20150914105604_3344199361_H__
