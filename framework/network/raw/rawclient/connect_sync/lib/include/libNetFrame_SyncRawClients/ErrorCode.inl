
#define SPVMN_ERROR_LIST(m) \
	m(OK, 0, "Success") \
	m(ERR_BAD_MESSAGE_FORMAT, 1, "Bad message format") \
	m(ERR_FAILED, 2, "Unknown error") \
	m(ERR_INTERNAL_FAILED, 3, "Internal error") \
	m(ERR_INVALID_CLIENT_TYPE, 100, "Invalid client type") \
	m(ERR_UNKNOWN_TOKEN, 101, "Unknown token") \
	m(ERR_INVALID_TOKEN, 102, "Invalid token") \
	m(ERR_TOKEN_EXPIRED, 103, "Token expired") \
	m(ERR_UNKNOWN_DEVICE_TYPE, 200, "Unknown device type") \
	m(ERR_INVALID_CHANNEL_NUMBER, 201, "Invalid channel number") \
	m(ERR_INVALID_STREAM_NUMBER, 202, "Invalid stream number") \
	m(ERR_NOT_FOUND_MEDIA_SEREVER, 203, "Not Found Media Server")\
	m(ERR_DAHUA_FIRST, 10000, "") \
	m(ERR_DAHUA_LAST, 19999, "")