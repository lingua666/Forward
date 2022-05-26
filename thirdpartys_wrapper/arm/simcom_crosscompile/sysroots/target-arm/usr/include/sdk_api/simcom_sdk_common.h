#ifndef _SIMCOM_SDK_COMMON_H
#define _SIMCOM_SDK_COMMON_H


#ifndef __int8_t_defined
# define __int8_t_defined
typedef signed char     int8_t;
typedef short int       int16_t;
typedef int             int32_t;
# if __WORDSIZE == 64
typedef long int        int64_t;
# else
__extension__
typedef long long int   int64_t;
# endif
#endif

/* Unsigned. */
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;

#if __WORDSIZE == 64
typedef unsigned long int   uint64_t;
#else
__extension__
typedef unsigned long long int uint64_t;
#endif

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint8_t  boolean;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAIL -1

#ifndef UNUSED
#define UNUSED(a)    (void)(a)
#endif

typedef enum
{
    SIMCOM_EVENT_SERVING_SYSTEM_IND = 0,
    SIMCOM_EVENT_SMS_PP_IND,
    SIMCOM_EVENT_VOICE_CALL_IND,
    SIMCOM_EVENT_VOICE_RECORD_IND,
    SIMCOM_EVENT_NETWORK_IND,
    SIMCOM_EVENT_DATACALL_CONNECTED_IND,
    SIMCOM_EVENT_DATACALL_DISCONNECTED_IND,
    SIMCOM_EVENT_NMEA_IND,
    SIMCOM_EVENT_LOC_IND,
    SIMCOM_EVENT_AUDIO,
    SIMCOM_EVENT_LOC_NMEA_IND,
    SIMCOM_EVENT_LOC_INFO_IND,
    SIMCOM_EVENT_MAX
}simcom_event_e;

#define MAX_STR_LEN 128
#define WDS_MAX_STR_LEN 128

typedef enum {
    WDS_PDP_IP_TYPE_IPV4V6 = 0x00, /* PDP-IPv4 and IPv6 */
    WDS_PDP_IP_TYPE_IPV4   = 0x01, /* PDP-IP (IPv4) */
    WDS_PDP_IP_TYPE_IPV6   = 0x02, /* PDP-IPv6 */
}wds_pdp_ip_type;

typedef enum {
    WDS_PDP_AUTH_TYPE_NONE      = 0x00, /* no auth */
    WDS_PDP_AUTH_TYPE_PAP       = 0x01, /* PAP */
    WDS_PDP_AUTH_TYPE_CHAP      = 0x02, /* CHAP */
}wds_pdp_auth_type;

typedef struct{
    char apn[WDS_MAX_STR_LEN];
    char username[WDS_MAX_STR_LEN];
    char password[WDS_MAX_STR_LEN];
    wds_pdp_ip_type ip_type;
    wds_pdp_auth_type auth_type;
}wds_apn_profile_info;

typedef struct 
{
    int  ip_addr;
    int  primary_dns;
    int  secondary_dns;
    int  gateway;
    int  netmask;
} dc_ipv4_info;

typedef struct 
{
    int  ipv6_addr[4];
    int  primary_dns[4];
    int  secondary_dns[4];
    int  gateway[4];
    int  netmask[4];
} dc_ipv6_info;

typedef struct 
{
    int status;
    int suggested_retry_time;
    int cid;
    int v6_partner_cid;
    int ip_type;        /*for IPTYPE, 0--IPV4V6, 1--IPV4, 2 -- IPV6 */
    int mtu;
    dc_ipv4_info ipv4_info;
    dc_ipv6_info ipv6_info;
    char apn[MAX_STR_LEN];
    char netif[MAX_STR_LEN];
}data_call_info_t;


#define RIL_UBUS_REQUEST_NAME "ril"

/******************************************************************************
 *  Defines
 ******************************************************************************/
#define SDK_ERR(format,...)    syslog(0,"[SDK_ERR]:%d:"format,__LINE__,##__VA_ARGS__)
#define SDK_LOG(format,...)    syslog(0,"[SDK_LOG]:%d:"format,__LINE__,##__VA_ARGS__)
#define SDK_INFO(format,...)   syslog(0,"[SDK_INFO]:%d:"format,__LINE__,##__VA_ARGS__)
#define SDK_DBG(format,...)    syslog(0,"[SDK_DBG]:%d:"format,__LINE__,##__VA_ARGS__)


#endif /* _SIMCOM_SDK_COMMON_H */
