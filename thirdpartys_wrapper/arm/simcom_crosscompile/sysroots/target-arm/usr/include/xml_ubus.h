#ifndef __XML_UBUS_H_
#define __XML_UBUS_H_
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <libubox/avl.h>
#include <libubox/avl-cmp.h>
#include <libubox/list.h>
#include <libubox/uloop.h>
#include <libubox/ustream.h>
#include <libubox/blob.h>
#include <libubox/utils.h>
#include <libubox/usock.h>
#include <libubus.h>

#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>

#include <poll.h>
#include <mxml.h>
#include <cgi.h>
#include "blobmsg_xml.h"
#include <include/log.h>
#include <sys/prctl.h>

#define XML_GET_BUF_SIZE 16*1024
#define UBUS_DEFAULE_SID "00000000000000000000000000000000"
#define ARG_METHOD        "method"
#define ARG_FILE          "file"
#define ARG_COMMAND          "command"
#define ARG_MODULE        "module"
#define ARG_ACTION		"Action"
#define ERROR_CAUSE_TAG "error_cause"
#define RESPONSE_TAG "response"
#define OTA_UNIX_PATH  "/tmp/ota.unix"
#define CONFIG_FILE_NAME "pxa1826_cfg.tar.gz"

#define LOGCAT
#ifdef LOGCAT
#define CGI_LOGI(format, args...) RDPRINTF(format, ##args)
#define CGI_LOGD(format, args...) RDBGMSG(format, ##args)
#define CGI_LOGE(format, args...) RERRMSG(format, ##args)
#else
#define CGI_LOGI(format, args..
#endif

#ifndef UNUSEDPARAM
#define UNUSEDPARAM(param) (void)param;
#endif

enum {
	RPC_METHOD,
	RPC_SESSIONID,
	RPC_OBJPATH,
	PRC_OBJMETHOD,
	RPC_MAX,
};

enum {
	SES_ACCESS,
	__SES_MAX,
};

typedef enum _error_cause_format {
	ERR_NUMBER,
	ERR_STRING,
} e_error_cause_format;

typedef enum _error_cause {
	NO_ERR,
	ERR_CGI_FORMAT,
	ERR_PARAM_INVALID,
	ERR_SERVICE_NOT_FOUND,
	ERR_METHOD_NOT_SUPPORT,
	ERR_SESSION_INVALID,
	ERR_CGI_FILE,
	_ERR_MAX,
} e_error_cause;

struct rpc_data {
	char *sid;
	char *ubus_method;
	char *object_path;
	char *object_method;
	struct blob_attr *data;
	struct blob_attr *params;
};

const char *error_cause_msg[] = {
	"OK",
	"inner cgi error, wrong post XML",
	"invalid param",
	"not found the object path",
	"list method not support",
	"session invalid",
	"inner cgi error, open file failed",
};

typedef enum _e_upload_status{
	UPLOAD_OK,
	ERR_UPLOAD_FAILED,
} e_upload_status;

struct upload_ctx {
	unsigned int id;
	char f_name[32];
	int n_file_total;
	int n_file_segmnet;
	int n_send;
	int fd;
	e_upload_status upload_status; 
};

typedef int (*upload_fw_init_cb_t) (struct upload_ctx *, int, int);
typedef int (*upload_fw_write_cb_t) (struct upload_ctx *, char *, int);
typedef int (*upload_fw_done_cb_t) (struct upload_ctx *);
typedef int (*download_init_cb_t) ();
typedef int (*download_response_cb_t) ();
typedef int (*upload_response_cb_t) (struct upload_ctx *);

struct upload_forward_ops {
	upload_fw_init_cb_t upload_fw_init_cb;
	upload_fw_write_cb_t upload_fw_write_cb;
	upload_fw_done_cb_t upload_fw_done_cb;
	upload_response_cb_t upload_response_cb;
};

struct download_ops {
	download_init_cb_t download_init_cb;
	download_response_cb_t download_response_cb;
};

struct mime_vec {
	char *str;
	int len;
};

#endif
