#ifndef __SSL_HTTP_CLIENT_H__
#define __SSL_HTTP_CLIENT_H__

#include <polarssl/ssl.h>
#include <polarssl/entropy.h>
#include <polarssl/ctr_drbg.h>

struct ssl_client {
	ssl_context ssl;
	ssl_session ssn;
	int fd;
};


extern struct ssl_client * ssl_client_init(int fd);

extern void ssl_client_shutdown(struct ssl_client * client);

#endif
