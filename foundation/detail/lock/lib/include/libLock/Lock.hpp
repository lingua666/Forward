

#ifndef __LOCK_20150930171651_1443604611_H__
#define __LOCK_20150930171651_1443604611_H__

#include <libCommon/modafx.h>

#if defined USE_LOCK_DEF
#define LOCK(MyLock)     MyLock.Lock();
#define UNLOCK(MyLock)   MyLock.UnLock();
#else
#define LOCK(MyLock)     
#define UNLOCK(MyLock)   
#endif

#include <libLock/CASLock.h>
typedef	_lock_::CASLock		CLock;
typedef _lock_::CASLockNone	CNoneLock;


#endif // __CUSTOM_LOCK_HEAD_DEF_
