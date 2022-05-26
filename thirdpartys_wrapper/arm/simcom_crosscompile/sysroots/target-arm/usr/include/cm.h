#ifndef __CM__
#define __CM__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#define MaxStringLen	20

#define MaxMtPDPNum   10
#define MaxMoPDPNum   10

#define MAX_STR_LEN	128
#define MAX_APN_NUM	10
#define MAX_IP_TYPE_LEN 8

#define TRUE 1
#define FALSE 0

enum 
{
	REQID,
	ERRNO,
	DATA,
	_MAX
};

enum {
	CM_OK,
	CM_ERR_MEM,
	CM_ERR_INVAL,
	CM_ERR_NOTFOUND,
	CM_ERR_IO,
	CM_ERR_PARSE,
	CM_ERR_DUPLICATE,
	CM_ERR_UNKNOWN
};
enum {
	CONNECTION_NUM,
	ENABLE,
	PDP_NAME,
	IP_TYPE,
	QCI,
	APN,
	LTE_APN,
	USR_2G3G,
	PSWD_2G3G,
	AUTHTYPE_2G3G,
	USR_4G,
	PSWD_4G,
	AUTHTYPE_4G,
	TYPE,
	AUTO_APN,
	CONNECT_MODE,
	LTE_DEFAULT,
	DATA_ON_ROAMING,
	MTU,
	_PARAM_MAX
};

typedef struct {
        int  IPAddr;
        int  PrimaryDNS;
        int  SecondaryDNS;
        int  GateWay;
        int  Mask;
} Ipv4Info;

typedef struct {
        int  IPV6Addr[4];
        int  PrimaryDNS[4];
        int  SecondaryDNS[4];
        int  GateWay[4];
        int  Mask[4];
} Ipv6Info;

typedef struct {
        unsigned int  min;           //low limit
        unsigned int  max;          //max limit
} numeric_range_t;

typedef struct {
    int connectionNum;
    char PDP_name[64];
    int  PDP_Type;              // 1 -Primary, 0--secondary
    int  IP_Type;                //0-IPV4V6; 1---IPV4; 2-IPV6
    int  primaryCid;
    int  secondaryCid;
    int  QCI;
    int  mtu;
    char  APN[MAX_STR_LEN];
    char  LteAPN[MAX_STR_LEN];
    char  Usr2G3G[MAX_STR_LEN];
    char  PASWD2G3G[MAX_STR_LEN];
    char  Authtype2G3G[MAX_STR_LEN];
    char  Usr4G[MAX_STR_LEN];
    char  PASWD4G[MAX_STR_LEN];
    char  Authtype4G[MAX_STR_LEN];
    char  mmsc[MAX_STR_LEN];
    char  mmsproxy[MAX_STR_LEN];
    char  mmsport[MAX_STR_LEN];
    char  netif[MAX_STR_LEN];
    int  autoapn;
    bool autoconnect;
    bool data_on_roaming;
    bool is_default;
    bool all_try_fail;
    bool always_on;
    unsigned char try_num_ESM_false;
} PdpInfo;

typedef struct{
	int direction;
	bool directionPresent;
	numeric_range_t localPortRange;
	numeric_range_t remotePortRange;
	bool localPortRangePresent;
	bool remotePortRangePresent;
	char secondaryCid;
	char PfIdx;
	char EpIdx;
	bool pnNextHdrPresent;
	char pnNextHdr;
	bool TosPresent;
	char Tos;
	char TosMask;
	bool SpiPresent;
	bool FlowLablePresent;
	int  SPI;
	int FlowLable;
	int IPType;
	char RemoteAddress[16];
	bool RemoteAddressPresent;
	char SubnetMask[16];
	bool SubnetMaskPresent;
	void * next;
}PacketFilterInfo;

typedef struct {
    PdpInfo * pdpinfo;
    PacketFilterInfo * pf_info;
} Connection_Setup_Context;

typedef struct
{
	bool		autoconnect;
	bool		always_on;
	bool		lte_default;
	bool		config_only;
	bool		tetherable;
	bool		is_host;
	bool		data_on_roaming;
	int           mtu;
}Extra_Apn_Info;

typedef struct
{
	char	carrier[MAX_STR_LEN];
	char	pdp_name[MAX_STR_LEN];
	char	apn[MAX_STR_LEN];
	char	lte_apn[MAX_STR_LEN];
	char	type[MAX_STR_LEN];	//APN type
	char	usrname[MAX_STR_LEN];
	char lte_usrname[MAX_STR_LEN];
	char    paswd[MAX_STR_LEN];
	char    lte_paswd[MAX_STR_LEN];
	char    server[MAX_STR_LEN];
	char    proxy[MAX_STR_LEN];
	char    port[MAX_STR_LEN];
	char    mmsproxy[MAX_STR_LEN];
	char    mmsport[MAX_STR_LEN];
	char    mmsc[MAX_STR_LEN];
	char    authtype[MAX_STR_LEN]; //0-no auth; 1-PAP; 2-CHAP; 3-CHAP\PAP
	char    lte_authtype[MAX_STR_LEN];
	char    protocol[MAX_STR_LEN]; //0-IPV4V6; 1---IPV4; 2-IPV6
	char	roamingProtocol[MAX_STR_LEN];
	char	bearer[MAX_STR_LEN];
	char    pco[MAX_STR_LEN]; /* PCO options, see Ref [4], section 10.5.6.3 */
	char    vendorSpecific[MAX_STR_LEN]; /* PCO options, see Ref [4], section 10.5.6.3 */
	char	netif[MAX_STR_LEN];
	Extra_Apn_Info	Extra_params;
	int basic_set;
	int extra_set;
	int enable;
	int auto_apn;
	int connection_num;
	int mtu;
	int qci;
        int cid;
	unsigned char	 nwtype; /*0 -- common;	1 -- 2/3g;	2 -- lte;	3 -- 2g;	4 -- 3g*/
	int iptype; /*for IPTYPE, 0--IPV4V6, 1--IPV4, 2 -- IPV6 */
	int autoapn_iptype; /*1--IPV4, 2 -- IPV6, 3-- IPV4V6 */
	bool autoapn_iptype_present;
	unsigned char	 fail_num;/*updated for several APN*/
	unsigned char	 lte_fail_num;
	int apn_class;/*0-others, 1-vzwims, 2-vzwadmin, 3-vzwinternet, 4-vzwapp, 5-vzw800*/
	int config_by;
}Apn_Info;

typedef struct
{
	int iptype;
	char apn[MAX_STR_LEN];
	char username[MAX_STR_LEN];
	char passwd[MAX_STR_LEN];
	char authtype[MAX_STR_LEN];
}AttachApn_Info;

typedef struct
{
	char mcc[MAX_STR_LEN];
	char mnc[MAX_STR_LEN];
	Apn_Info apn_info_list[MAX_APN_NUM];
	bool	all_try_fail;
	unsigned char	try_num_ESM_false;
}Mnc_Apn;

typedef struct {
    int             status;     /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int             suggestedRetryTime; /* If status != 0, this fields indicates the suggested retry
                                           back-off timer value RIL wants to override the one
                                           pre-configured in FW.
                                           The unit is miliseconds.
                                           The value < 0 means no value is suggested.
                                           The value 0 means retry should be done ASAP.
                                           The value of INT_MAX(0x7fffffff) means no retry. */
    int             cid;        /* Context ID, uniquely identifies this call */
    int	             v6_parnter_cid;      /*ipv6 parnter cid if ipv4v6 retrace */                    
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char           type[8];       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". If status is
                                   PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                                   such as "IP" or "IPV6" */
    char          ifname[12];     /* The network interface name */
    char          addresses[128];  
    char          dnses[256];
    char          gateways[128];	
    char          pcscf[100];
    int           mtu;
    char          apn[MAX_STR_LEN];	
	char		  subnetmask[128];
}Ubus_Data_Call_Response;


typedef struct {
	int status;
	int suggestedRetryTime;
	int cid;
	int v6_partner_cid;
	int IP_Type;	/*for IPTYPE, 0--IPV4V6, 1--IPV4, 2 -- IPV6 */
	int mtu;
	Ipv4Info * ipv4info;
	Ipv6Info * ipv6info;
	char apn[MAX_STR_LEN];
	char netif[MAX_STR_LEN];
}Data_Call_Response;


typedef struct {
	Data_Call_Response PDP;
	void *next;
}Data_Call_Response_List;

typedef struct{
	char connectStatus; 	//0-disconnect, 1-connect successful, 2-connecting, 3-need to redial //when register to the network
	char first_dial;  //flag for identify whether it's the first dial
	char create_by; // 1 create by CM, 0 create by profile or webUI
	int dial_cnt;
	int dialFailureCode;    //0- no error
	int retryTime;
	PdpInfo * pdpinfo;
	Ipv4Info * ipv4info;
	Ipv6Info * ipv6info;
	PacketFilterInfo * tftinfo;
	void * next;
} CM_Connection_Context;


typedef struct{
	char connectStatus;   //0-disconnect, 1-connect successful, 2-connecting, 3-need to redial when register to the network
	int dialFailureCode;  //0-no error
	int retryTime;
	PdpInfo pdpinfo;
	Ipv4Info ipv4info;
	Ipv6Info ipv6info;
	PacketFilterInfo tftinfo;
}Dialer_Connection_Context;


typedef struct{
	int connectionNum;
	char deleteall; //1-delete all connections, 0-don't delete all
}DisconnectInfo;

typedef struct {
	int connectionNum;
	char curAPN[30];
	int dialFailurecode;
	int retryTime;
}DialerFailureResponse;

typedef struct{
	int connectionNum;
	int connectStatus;	//0-disconnect, 1-connect successful, 2-connecting, 3-need to redial when register to the network
	char create_by; // 1 create by CM, 0 create by profile or webUI
	PdpInfo pdpinfo;
	Ipv4Info ipv4info;
	Ipv6Info ipv6info;
	void * next;
}Link_Context;

typedef struct {          
     int  primaryCid;
     int  PDP_Type;              //  1-Primary, 0--secondary     	 
     int  QCI;
     char  IP_Type[8]; 	 
     char  APN[30];
     char  Usr[30];
     char  PASWD[30];
     char  Authtype[20];
} data_call_PdpInfo;

typedef struct {
    data_call_PdpInfo  pdp_info;
    PacketFilterInfo pf_info;
} data_call_Setup_Context;

typedef enum fifo_msg_type
{
	MSG_CGDCONT = 1,
	MSG_AUTH,
	MSG_ACT,
	MSG_ZACT,
	MSG_CONFEX,
	MSG_NETMASK,
	MSG_ZROAM,
	FIFO_MSG_MAX
} fifo_msg_type_e;

typedef struct {
	int cid;
	int ip_type;
	int is_default;
	char apn[MAX_STR_LEN];
	char netif[MAX_STR_LEN];
	int is_delete;
} fifo_msg_cgdcont_s;

typedef struct {
	int cid;
	int auth_type;
	char username[MAX_STR_LEN];
	char password[MAX_STR_LEN];
} fifo_msg_auth_s;

typedef struct {
	int cid;
	int state;
} fifo_msg_act_s;

typedef struct {
        int cid;
        bool data_on_roaming_present;
	bool data_on_roaming;
	bool auto_connect_present;
	bool auto_connect;
	bool data_on_boot_present;
	bool data_on_boot;
} fifo_msg_confex_s;

typedef struct {
	int mask;
} fifo_msg_netmask_s;

typedef enum pdp_config_by
{
	PDP_CONFIG_ZGDCONT = 1,
	PDP_CONFIG_MAX
} pdp_config_by_e;
#endif
