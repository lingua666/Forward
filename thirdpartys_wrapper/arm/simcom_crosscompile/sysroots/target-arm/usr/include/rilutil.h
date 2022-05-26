#ifndef RILUTIL_H
#define RILUTIL_H

#include <libubus.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int num;
	char **str;
}rilutilstrings;

int rilutil_makeRequestBlob(struct blob_buf *msg, unsigned int requestid, const void * data, int len);
int rilutil_parseResponse(struct blob_attr *msg, unsigned int *requestid, unsigned int *rilerrno,
		void **response, int *responselen);
int rilutil_freeResponseData(int reqid, void *data, int datalen);

#ifdef __cplusplus
}
#endif

#endif
