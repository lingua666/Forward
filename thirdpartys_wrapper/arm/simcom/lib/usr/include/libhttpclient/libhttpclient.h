#ifndef __LIBHTTPCLIENT_H__
#define __LIBHTTPCLIENT_H__


#define closesocket(a) close(a)
#define CPUartLogPrintf printf
#define _snprintf snprintf

typedef int (*client_response_cb)(char *buffer, int size, int nitems, void *private_data);
typedef void http_client;

#define HTTP_CLIENT_ERROR -1
#define HTTP_CLIENT_OK     0

#define HTTPCLIENT_MEM_SIZE 2048

enum {
	HTTPCLIENT_OPT_URL,
	HTTPCLIENT_OPT_METHOD,
	HTTPCLIENT_OPT_HTTP1_0,
	HTTPCLIENT_OPT_HTTPHEADER,
	HTTPCLIENT_OPT_POSTDATA,
	HTTPCLIENT_OPT_POSTLENGTH,
	HTTPCLIENT_OPT_RESPONSECB,
	HTTPCLIENT_OPT_RESPONSECB_DATA,
	HTTPCLIENT_OPT_AUTH_TYPE,
	HTTPCLIENT_OPT_AUTH_USERNAME,
	HTTPCLIENT_OPT_AUTH_PASSWORD,
};

enum {
	HTTPCLIENT_GETINFO_RESPONSE_CODE,
};

enum {
	HTTPCLIENT_REQUEST_GET,
	HTTPCLIENT_REQUEST_POST,
	HTTPCLIENT_REQUEST_PUT,
	HTTPCLIENT_REQUEST_MAX
};

enum {
	HTTP_AUTH_TYPE_BASE = 1,
	HTTP_AUTH_TYPE_DIGEST
};

struct http_client_list {
	char * data;
	struct http_client_list * next;
};

extern struct http_client * http_client_init(void);

extern void http_client_shutdown(struct http_client *);

extern struct http_client_list * http_client_list_append(struct http_client_list *, const char *);

extern void http_client_list_destroy(struct http_client_list *);

extern int http_client_setopt(struct http_client *, int, ...);

extern int http_client_perform(struct http_client *);

extern int http_client_getinfo(struct http_client *, int, void *);

#endif
