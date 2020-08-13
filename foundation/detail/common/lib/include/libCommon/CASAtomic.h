
#ifndef __CASATOMIC_20150806193305_1438860785_H__
#define __CASATOMIC_20150806193305_1438860785_H__

#include <libCommon/os/Platform.h>
#include <libCommon/os/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

bool	CASCompareExchange( long* Destination, long oldval,
							long newval);

//i++
const long	CASFetchAndAdd( long* Destination );

//++i
const long	CASAddAndFetch( long* Destination );

//i--
const long	CASFetchAndSub( long* Destination );

//--i
const long	CASSubAndFetch( long* Destination );

#ifdef  __cplusplus
}
#endif

#endif//__CASATOMIC_20150806193305_1438860785_H__
