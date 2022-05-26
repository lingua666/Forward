#ifndef _NAS_CONTROL_H_
#define _NAS_CONTROL_H_
#define MAX_SIM_INFO_STR_LEN 32
#define MAX_NETWORK_INFO_STR_LEN 24
#define NETWORK_ACT_MIN 0
#define NETWORK_ACT_MAX 8

#include "ril.h"
#include <libubox/blobmsg_json.h>
#include <libubox/ustream.h>
#include <libubus.h>
#include "simcom_sdk_common.h"

typedef enum
{
    CS_STATE_NOT_REGISTERED = 0,
    CS_STATE_REGISTERED = 1,
    CS_STATE_NOT_REGISTERED_SEARCHING = 2,
    CS_STATE_REGISTRATION_DENIED = 3,
    CS_STATE_UNKNOWN = 4,
    CS_STATE_REGISTERED_ROAMING = 5
} CS_attatch_state;

typedef enum
{
    PS_STATE_NOT_REGISTERED = 0,
    PS_STATE_REGISTERED = 1,
    PS_STATE_NOT_REGISTERED_SEARCHING = 2,
    PS_STATE_REGISTRATION_DENIED = 3,
    PS_STATE_UNKNOWN = 4,
    PS_STATE_REGISTERED_ROAMING = 5
} PS_attatch_state;

typedef struct
{
    CS_attatch_state csState;
    PS_attatch_state psState;
    RIL_RadioTechnology netState;
} SIM_NetState;

typedef enum
{
    SDK_PNHOE_MODE_ERROR,         // error
    LTE_WCDMA_GSM_PRE_LTE,        // 4G/3G/2G prefer 4G
    LTE_WCDMA_GSM_PRE_WCDMA,      // 4G/3G/2G prefer 3G
    LTE_WCDMA_GSM_PRE_GSM,        // 4G/3G/2G prefer 2G
    LTE_ONLY,                     // only 4G
    LTE_WCDMA_PRE_LTE,            // 4G/3G prefer 4G
    LTE_WCDMA_PRE_WCDMA,          // 4G/3G prefer 3G
    WCDMA_GSM_PRE_WCDMA,          // 3G/2G prefer 3G
    WCDMA_ONLY,                   // only 3G
    GSM_ONLY,                     // only 2G
    SDK_NETWORK_MODE_MAX,
}SDK_network_mode;

typedef enum
{
    UBUS_NETWORK_PHONE_MODE_ERROR,     // error
    UBUS_NETWORK_LTE_WCDMA_GSM,        // 4G/3G/2G
    UBUS_NETWORK_LTE_ONLY,             // only 4G
    UBUS_NETWORK_LTE_WCDMA,            // 4G/3G
    UBUS_NETWORK_WCDMA_GSM_PRE_WCDMA,  // 3G/2G prefer 3G
    UBUS_NETWORK_WCDMA_ONLY,           // only 3G
    UBUS_NETWORK_GSM_ONLY,             // only 2G
    UBUS_NETWORK_MAX,
}UBUS_network_mode;

typedef enum
{
    UBUS_NETWORK_LTE_PREFER_IN_LTE = 1,           // 4G/3G/2G prefer 4G
    UBUS_NETWORK_WCDMA_PREFER_IN_LTE = 2,         // 4G/3G/2G prefer 3G
    UBUS_NETWORK_GSM_PREFER_IN_LTE = 8,           // 4G/3G/2G prefer 2G
    UBUS_NETWORK_LTE_PREFER_IN_WCDMA = 3,         // 4G/3G    prefer 3G
    UBUS_NETWORK_WCDMA_PREFER_IN_WCDMA = 4,       // 4G/3G    prefer 2G
}UBUS_network_prefer_mode;


typedef enum
{
    NET_MODE_TABLE,
    NET_MODE_TABLE_MAX,
}UBUS_blob_table;

typedef enum
{
    NW_MODE,
    PREFER_MODE,
    PREFER_LTE_MODE,
    NET_MDOE_MAX,
}UBUS_blob_subtable;

typedef enum
{
    RIL_GET_CFUN_ONLINE = 1,
    RIL_GET_CFUN_LOWPOWER = 0,
    RIL_GET_CFUN_FACTORY_TEST_MODE = 5,
    RIL_GET_CFUN_PERSISTENT_LOW_POWER = 4,
    RIL_GET_CFUN_OFFLINE = 7,
}RIL_phone_get_mode;

typedef enum
{
    RIL_SET_CFUN_ONLINE = 0,
    RIL_SET_CFUN_LOWPOWER = 1,
    RIL_SET_CFUN_FACTORY_TEST_MODE = 2,
    RIL_SET_CFUN_PERSISTENT_LOW_POWER = 3,
    RIL_SET_CFUN_RESET = 4,
    RIL_SET_CFUN_OFFLINE = 6,
}RIL_phone_set_mode;


typedef enum
{
    SDK_CFUN_ONLINE,
    SDK_CFUN_LOWPOWER,
    SDK_CFUN_FACTORY_TEST_MODE,
    SDK_CFUN_PERSISTENT_LOW_POWER,
    SDK_CFUN_OFFLINE,
    SDK_CFUN_RESET,
}SDK_phone_mode;


typedef struct
{
    int rssi;
    int gsmSigValid;
    int lteSigValid;
} SIM_sigInfo;

typedef struct
{
    int SigType;       //1:LTE  0:GSM
    int rssi;
    int rsrp;
    int rsrq;
    int sinr;
} LTE_sigInfo;

typedef struct
{
    int SigType;      //1:LTE  0:GSM
    int rssi;
    int ecio;
    int rscp;
} GSM_sigInfo;

typedef union
{
    LTE_sigInfo lte_siginfo;
    GSM_sigInfo gsm_siginfo;
}SIM_ExtSigInfo;

typedef struct CellInfo_Node
{
    RIL_CellInfo cellinfo;
    struct CellInfo_Node *next;
}CellInfo_Node;

typedef struct
{
    char long_formal_alphanumeric[MAX_SIM_INFO_STR_LEN];
    char short_formal_alphanumeric[MAX_SIM_INFO_STR_LEN];
    char numeric[MAX_SIM_INFO_STR_LEN];
}PLMN_formal;

typedef struct Network_information
{
    int id; //network id
    char isp_name[MAX_NETWORK_INFO_STR_LEN]; //network name
    char plmn[MAX_NETWORK_INFO_STR_LEN]; // Network Name Abbreviation
    char status[MAX_NETWORK_INFO_STR_LEN]; // network status
    char act[MAX_NETWORK_INFO_STR_LEN]; // Access technology selected
    struct Network_information *next;
}Network_list;

/*****************************************************************************
 * FUNCTION
 *  alsa_init
 *
 * DESCRIPTION
 *  init nas
 *
 * PARAMETERS
 *  simcom_event_ind_cb: register call_back to process received
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE:
 *
 *****************************************************************************/
int nas_init();


/*****************************************************************************
 * FUNCTION
 *  alsa_deinit
 *
 * DESCRIPTION
 *  deinit nas
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 *
 *****************************************************************************/
void nas_deinit();


/*****************************************************************************
* Function Name : get_NetworkType
* Description   : 获取网络状态
* Input         : None
* Return        : regState
* Date          : 2020.08.31
*****************************************************************************/
int get_NetworkType(SIM_NetState *network_type);


/*****************************************************************************
* Function Name : get_SignalStrength
* Description   : 获取信号强度
* Input         : None
* Return        : sigInfo
* Date          : 2020.08.31
*****************************************************************************/
SIM_sigInfo get_SignalStrength(void);


/*****************************************************************************
* Function Name : get_NetworkPreferredMode
* Description   : 获取网络模式
* Output        : nw_mode:enum SDK_network_mode
* Return        : 0:true
                  -1:false
* Date          : 2020.08.31
*****************************************************************************/
int get_NetworkPreferredMode(int *nw_mode);

/*****************************************************************************
* Function Name : set_NetworkPreferredMode
* Description   : 设置网络模式
* Input         : nw_mode:enum SDK_network_mode
* Return        : 0:true
                  -1:false
* Date          : 2020.08.31
*****************************************************************************/
int set_NetworkPreferredMode(int nw_mode);

/*****************************************************************************
* Function Name : get_ExtSignalInfo
* Description   : 获取信号信息
* Input         : pointer to get the info

* Return        : 0:true
                  -1:false
* Date          : 2020.08.31
*****************************************************************************/
int get_ExtSignalInfo(SIM_ExtSigInfo *siginfo);


/*****************************************************************************
* Function Name : get_PhoneMode
* Description   : 获取cfun模式
* Input         : pointer to get the mode
                  phone_mode:enum SDK_phone_mode
                  0 Online            (equal  at+cfun=1)
                  1 Low power         (equal  at+cfun=0)
                  2 Factory Test mode
                  3 Persistent low power
                  4 Offline

* Return        : 0:true
                  -1:false
* Date          : 2020.08.31
*****************************************************************************/
int get_PhoneMode(int *phone_mode);


/*****************************************************************************
* Function Name : set_PhoneMode
* Description   : 设置cfun模式
* Input         : integer to set the mode
                  phone_mode:enum SDK_phone_mode
                  0 Online            (equal  at+cfun=1)
                  1 Low power         (equal  at+cfun=0)
                  2 Factory Test mode
                  3 Persistent low power
                  4 Offline
                  5 Reset             (only use in offline and will reboot)

* Return        : 0:true
                  -1:false
* Date          : 2020.08.31
*****************************************************************************/
int set_PhoneMode(int phone_mode);

/*****************************************************************************
* Function Name : get_CeLLInfo
* Description   : 获取小区信息
* Output        : integer to get the mode
                  cell_info_list:
                  serving cell and neighber cell list

* Return        : 0:true
                  -1:false
* note          : 得到的链表需要手动释放,可通过free_cellinfo_list接口释放
* Date          : 2020.08.31
*****************************************************************************/
int get_CellInfo(CellInfo_Node **cell_info_list);

/*****************************************************************************
* Function Name : free_cellinfo_list
* Description   : 释放小区信息链表
* Input         : head:链表表头

* Return        : 0:true
                  -1:false
* note          :
* Date          : 2020.08.31
*****************************************************************************/
int free_cellinfo_list(CellInfo_Node *head);

/*****************************************************************************
* Function Name : get_ImsRegistrationState
* Description   : 获取IMS注册状态
* Output        : integer to get the state
*                 0 - Not registered
*                 1 - Registered
* Return        : 0:true
                  -1:false
* note          :
* Date          :
*****************************************************************************/
int get_ImsRegistrationState(int *state);

/*****************************************************************************
* Function Name : get_register_plmn
* Description   : 获取注网运营商
* Output        : long_formal_alphanumeric, short_formal_alphanumeric, numeric
                  respectively correspond to an array which type is char.
                  the length of them can not more than 32
                  'long_formal_alphanumeric' gets long formal alphanumeric of plmn
                  'long_formal_alphanumeric' gets short formal alphanumeric of plmn
                  'numeric' gets the numeric type of plmn

* Return        : 0:true
                  -1:false
* Date          : 2020.12.28
*****************************************************************************/

int get_register_plmn(char *long_formal_alphanumeric, char *short_formal_alphanumeric, char *numeric);

/*****************************************************************************
* Function Name : search_network
* Description   : 搜索网络
* Output        : network_list:
                  Save Network Information
* Return        : 0:true
                  -1:false
* note          : You must release the space which *network_head point to after use.
                  Search_network takes a while.
* Date          : 2020.1.25
*****************************************************************************/
int search_network(Network_list **network_head);

/*****************************************************************************
* Function Name : select_network
* Description   : 选择网络
* Input         : plmn:numeric of network
                  act:Access technology selected
                    0:GSM
                    1:GSM_COMPACT
                    2:UTRAN
                    3:GSM_wEGPRS
                    4:UTRAN_wHSDPA
                    5:UTRAN_wHSUPA
                    6:UTRAN_wHSDPA_HSUPA
                    7:E_UTRAN
                    8:UTRAN_HSPA+
* Return        : 0:true
                  -1:false
* note          : You must search network before select network.
* Date          : 2020.1.25
*****************************************************************************/
int select_network(char *plmn, int act);


#endif
