
/******************************************************************************
*(C) Copyright 2015 Marvell International Ltd.
* All Rights Reserved
******************************************************************************/

#ifndef __CHL__
#define __CHL__

#include "ril.h"

//////////////* defines for register/register_internet request *///////////////
#define CHL_REG_NAME_STR	"name"		//STRING
#define CHL_REG_APN_STR	"apn"		//STRING
#define CHL_REG_IP_TYPE_STR	"ip_type"	//INT32: 1:IPv4, 2:IPV6, 3:IPv4v6
#define CHL_REG_USER_STR	"user"		//STRING
#define CHL_REG_PASS_STR	"pass"		//STRING
#define CHL_REG_DATA_PROF_STR 	"data_profile"	//STRING
#define CHL_REG_AUTH_STR	"auth"		//INT32
#define CHL_REG_RADIO_TECH_STR	"radio_tech"	//INT32

/////////* defines for share_internet request *////////////////////////////////
#define CHL_SHARE_INT_NAME_STR	"name"		//STRING

///////* defines for route_add/route_del request */////////////////////////////
#define CHL_RT_ID_STR		"id"		//INT32
#define CHL_RT_HOST_IP_STR	"host_ip"	//STRING

///////* defines for open/close/unregister/route/status request *//////////////
#define CHL_ID_STR 		"id"		//INT32

///////* defines for set_lte_dflt request *////////////////////////////////////
#define CHL_LTED_APN_STR 	"apn"		//INT32
#define CHL_LTED_IP_TYPE_STR	"ip_type"	//INT32: 1:IPv4, 2:IPV6, 3:IPv4v6


/* possible value for errcode for register/register_internet/share_internet requests */
enum chl_reg_req_status {
	/* registration ok */
	CHL_REG_OK,
	/* invalid request for registration */
	CHL_REG_INVALID_REQ,
	/* CHL internal error occured */
	CHL_REG_INTERNAL_ERROR,
	/* if internet pdp was requested - internet pdp already exists */
	GW_EXISTS,
	/* if share_internet was requested but no pdp is available */
	PDP_UNAVAILABLE,
	/* PDP registered with different authentication */
	CONFLICT_AUTH,
	/* PDP registered with different radio technology */
	CONFLICT_RADIO_TECH,
	/* PDP registered with different data profile */
	CONFLICT_DATA_PROFILE,
};

/* possible value for errcode for open/close/unregister/route/lte_dflt requests */
enum chl_req_status {
	/* status OK*/
	CHL_OK,
	/* invalid request */
	CHL_INVALID_REQ,
	/* CHL internal error occured */
	CHL_INTERNAL_ERROR,
	/* CHL id not found for request */
	CHL_ID_NOT_FOUND,
	/* request for lte default failed */
	LTE_DEFAULT_REQ_FAIL,
	/* for 'open' - RIL rejected the request - check the 'ril_status'*/
	RIL_ERROR_OCCURED,
};

/* values for ip_type */
enum ip_type {
	IPV4 = 0x1,
	IPV6 = 0x2,
	IPV4V6 = (IPV4 | IPV6),
};

/* possible values for nw_status */
enum chl_nw_status {
	DATA_UNAVAILABLE,
	DATA_AVAILABLE,
	DATA_PENDING,
};

static inline char *nw_status_to_str(enum chl_nw_status state)
{
	switch(state) {
	case DATA_UNAVAILABLE:
		return "DATA_UNAVAILABLE";
	case DATA_AVAILABLE:
		return "DATA_AVAILABLE";
	case DATA_PENDING:
		return "DATA_PENDING";
	default:
		return "Unknown";
	}
}

static inline char *ip_type_to_str(enum ip_type type)
{
	switch(type) {
	case IPV4:
		return "IPV4";
	case IPV6:
		return "IPV6";
	case IPV4V6:
		return "IPV4V6";
	default:
		return "Unknown";
	}
}

static inline char *req_status_to_str(enum chl_req_status status)
{
	switch(status) {
	case CHL_OK:
		return "CHL_OK";
	case CHL_INVALID_REQ:
		return "CHL_INVALID_REQ";
	case CHL_INTERNAL_ERROR:
		return "CHL_INTERNAL_ERROR";
	case CHL_ID_NOT_FOUND:
		return "CHL_ID_NOT_FOUND";
	case LTE_DEFAULT_REQ_FAIL:
		return "LTE_DEFAULT_REQ_FAIL";
	case RIL_ERROR_OCCURED:
		return "RIL_ERROR_OCCURED";
	default:
		return "Unknown";
	}
}

static inline char *reg_req_status_to_str(enum chl_reg_req_status status)
{
	switch(status) {
	case CHL_REG_OK:
		return "CHL_REG_OK";
	case CHL_REG_INVALID_REQ:
		return "CHL_REG_INVALID_REQ";
	case CHL_REG_INTERNAL_ERROR:
		return "CHL_REG_INTERNAL_ERROR";
	case GW_EXISTS:
		return "GW_EXISTS";
	case PDP_UNAVAILABLE:
		return "PDP_UNAVAILABLE";
	case CONFLICT_AUTH:
		return "CONFLICT_AUTH";
	case CONFLICT_RADIO_TECH:
		return "CONFLICT_RADIO_TECH";
	case CONFLICT_DATA_PROFILE:
		return "CONFLICT_DATA_PROFILE";
	default:
		return "Unknown";
	}
}

#endif
