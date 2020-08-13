
#ifndef __STRUCT_20151022174359_1445507039_H__
#define __STRUCT_20151022174359_1445507039_H__

#include <libCommon/os/Platform.h>
#include <libCommon/os/Types.h>
#include <libCommon/os/error.h>

typedef struct _MEMBUF
{
	UInt32	uLen; /* the length of the buffer */
	char	Buf[1]; /* the pointer to the buffer */
} MEMBUF, * LPMEMBUF;

typedef struct _MEMBUF_PRT
{
	UInt32	uLen; /* the length of the buffer */
	char*	pBuf; /* the pointer to the buffer */
} MEMBUF_PRT, * LPMEMBUF_PRT;


#endif//__STRUCT_20151022174359_1445507039_H__
