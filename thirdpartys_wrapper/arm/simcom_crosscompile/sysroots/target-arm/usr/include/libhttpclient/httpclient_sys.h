#ifndef __HTTP_CLIENT_ARCH_H__
#define __HTTP_CLIENT_ARCH_H__
#include <include/log.h>

#define _HC_DIAG_LOG_OUT
#ifdef _HC_DIAG_LOG_OUT
#define PRINTINFO(format, args...) RDPRINTF(format, ##args);
#define PRINTERR(format, args...) RERRMSG(format, ##args);
#else
#define PRINTINFO(format, args...) fprintf(stderr, "[%s@%s,%d] " format "\r\n", __FUNCTION__, __FILE__, __LINE__, ## args);
#define PRINTERR(format, args...) fprintf(stderr, "[%s@%s,%d] " format "\r\n", __FUNCTION__, __FILE__, __LINE__, ## args);
#endif

#define	ENTER() PRINTINFO("Enter %s", __FUNCTION__)
#define	LEAVE() PRINTINFO("Leave %s", __FUNCTION__)

extern char * __http_client_strdup(const char *s);

extern void * __http_client_malloc(int size);

extern void __http_client_free(void * p);

#ifndef UNUSED
#define UNUSED(a)    (void)(a)
#endif

#define HTTP_CLIENT_MALLOC __http_client_malloc
#define HTTP_CLIENT_FREE   __http_client_free

#undef strdup
#define strdup __http_client_strdup


#endif
