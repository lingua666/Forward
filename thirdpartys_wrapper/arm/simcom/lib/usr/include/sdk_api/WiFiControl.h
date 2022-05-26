#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include "simcom_sdk_common.h"

/*SIMCOM zhangwei @20210725 add for wifi support begin*/
//#include "LedControl.h"
enum {
  WIFI_PARA_AP_SSID,
  WIFI_PARA_AP_AUTH,
  WIFI_PARA_AP_BROADCAST,
  WIFI_PARA_AP_CHANNEL,
  WIFI_PARA_AP_BAND,
  WIFI_PARA_AP_MODE,
  WIFI_PARA_AP_POWER,
  WIFI_PARA_AP_ISOLATE,
  WIFI_PARA_MAX, 
  WIFI_PARA_END=0xff
};

#define AUTH_OPEN    0
#define AUTH_WPA     1
#define AUTH_WPA2    2
#define AUTH_WPAWPA2 3
#define AUTH_WEP     4
#define AUTH_SHARE   AUTH_WEP
#define AUTH_AUTO    AUTH_WPAWPA2
#define AUTH_MIN     AUTH_OPEN
#define AUTH_MAX     AUTH_WEP

#define E_TKIP       1
#define E_AES        2
#define E_AES_TKIP   3
#define E_WEP_SHARED      4
#define E_WEP_OPEN      8
#define E_WEP    E_WEP_SHARED
#define E_NULL   E_WEP_OPEN
#define E_11B  1
#define E_11G  2
#define E_11N  4
#define E_11BG 3
/*SIMCOM zhangwei @20210725 add for wifi support end*/

typedef enum
{
    AP_MODE = 0,
    APAP_MODE,
    APSTA_MODE
}wifi_mode_type;

typedef enum
{
    AP_INDEX_0 = 0,
    AP_INDEX_1,
    AP_INDEX_STA
}ap_index_type;

typedef enum 
{
    USB,
    WLAN,
} client_type;

typedef enum 
{
    CONN,
    DIS_CONN,
    BLOCK,
} client_state;

typedef enum 
{
    GET_ALL_CLIENTS_STAT,
    GET_CONN_CLIENTS,
} get_clients_type;

typedef struct client_info 
{
    unsigned char ip[6];
    unsigned char mac[8];
    char name[32];
    client_type type;
    client_state status;
    time_t last_conn_time;
    time_t last_update_time;
    long long total_conn_time;
    long long cur_conn_time;
    long long total_rx;
    long long unsave_rx;
    long long total_tx;
    long long unsave_tx;
    long long last_ndays_tx;
    long long last_ndays_rx;
    long long this_mon_tx;
    long long this_mon_rx;
    struct client_info *next;
}client_info_node,*client_info_node_ptr;


struct stat_nw_notify_data {
    int status;
    int cid;
    int iptype;
};
struct dhcp_action_msg{
    char mac[18];
    char ip[16];
    char name[32];
};

struct stat_req_msg {
    int msg_type;
    union {
        long long adjust_value;
        char mac[18];
        struct stat_nw_notify_data nw_msg;
        struct dhcp_action_msg dhcp_msg;
    } data;
};

struct client_info_msg {
    int msg_type;
    struct client_info node;
};

typedef struct 
{
    char filtermode[16];                // Indicates filtering mode
    char **allowlist;                   // All MAC addresses in allow mode
    char **denylist;                    // All MAC addresses in deny mode
    int allownum;                       // Number of MAC addresses in allow mode
    int denynum;                        // Number of MAC addresses in deny mode
    int allow_flag;                     // Check whether MAC addresses exist in the allow mode
    int deny_flag;                      // Check whether MAC addresses exist in the deny mode
}maclist_info_t;



typedef unsigned char boolean;
typedef unsigned char uint8;


/*****************************************************************************
* Function Name : get_wifi_mode
* Description   : 获取当前WIFI模式设置
* Input         : None
* Output        : 0：单AP模式     1：双AP模式     2: AP+STA模式
* Return        : wifi_mode_type
* Auther        : qjh
* Date          : 2018.04.10
*****************************************************************************/ 
wifi_mode_type get_wifi_mode();

/*****************************************************************************
* Function Name : set_wifi_mode
* Description   : 设置WIFI模式
* Input         : 0：单AP模式     1：双AP模式     2: AP+STA模式
* Output        : TRUE or FALSE
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.10
*****************************************************************************/
boolean set_wifi_mode(wifi_mode_type mode);

/*****************************************************************************
* Function Name : wifi_power
* Description   : WIFI开关
* Input         : act 非0： 打开      0： 关闭
* Output        : None
* Return        : int
* Auther        : qjh
* Date          : 2018.04.10
*****************************************************************************/	
boolean wifi_power(int act);

/*****************************************************************************
* Function Name : get_wifi_status
* Description   : 获取WIFI 状态
* Input         : act 1： 打开      0： 关闭
* Output        : None
* Return        : int
* Auther        : qjh
* Date          : 2018.04.10
*****************************************************************************/	
boolean get_wifi_status(uint8 *flag);

/*****************************************************************************
* Function Name : set_auth
* Description   : 设置AP auth类型, 加密模式, 密码

输出参数组合限制如下表所示

条件1：长度为8到63位的ASCII编码字符串
条件2：长度为5或者13的ASCII编码字符串，长度为10或者26的十六进制编码字符串，四选一
注："/"代表参数值不影响函数功能,建议写0
_______________________________________________________________________
Auth+Encrypt       |auth type      |encrypt mode     |passwored format 
___________________|_______________|_________________|_________________
OPEN               |     0         |      /          |       /         
___________________|_______________|_________________|_________________
WPA2+AES           |     2         |      2          |                 
___________________|_______________|_________________|                 
WPA2+AESTKIP       |     2         |      3          |                 
___________________|_______________|_________________|                 
WPA2+AES           |     1         |      2          |                 
___________________|_______________|_________________|                 
WPA+TKIP           |     1         |      1          |      条件1      
___________________|_______________|_________________|                 
WPA+AESTKIP        |     1         |      3          |                 
___________________|_______________|_________________|                 
WPAWPA2+AES        |     3         |      2          |                 
___________________|_______________|_________________|                 
WPAWPA2+AESTKIP    |     3         |      3          |                 
___________________|_______________|_________________|_________________
WEP+SHARED         |     4         |      4          |                 
___________________|_______________|_________________|      条件2      
WEP+OPEN           |     4         |      8          |                 
___________________|_______________|_________________|_________________

* Input         : int authType, int encryptMode, char *password
* Output        : None
* Return        : true or false
* Auther        : caihaichuan
* Date          : 2022.1.25
*****************************************************************************/	
boolean set_auth(char *str_pwd, int auth_type, int  encrypt_mode, ap_index_type ap_index);


/*****************************************************************************
* Function Name : wifi_get_ap_auth
* Description   : 获取AP auth类型, 加密模式, 密码
* Input			: int *authType, int *encryptMode, char *pwd_str,
      ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean get_auth(int *auth_type_ptr, int *encrypt_mode_ptr, char *pwd_str, ap_index_type ap_index);

/*****************************************************************************
* Function Name : set_ssid 
* Description   : 设置WIFI 热点名称
* Input			: char *ssid,
      ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean set_ssid(char *SSID, ap_index_type ap_index);
/*****************************************************************************
* Function Name : get_ssid 
* Description   : 获取WIFI 热点名称
* Input			: char *ssid,
      ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean get_ssid(char *str_SSID, ap_index_type ap_index);

/*****************************************************************************
* Function Name : set_bcast 
* Description   : 设置WIFI 广播开关
* Input			: int broadcast，
      ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean set_bcast(int broadcast, ap_index_type ap_index);
/*****************************************************************************
* Function Name : get_bcast 
* Description   : 获取WIFI 广播设定
* Input			: int broadcast,
      ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean get_bcast(int *broadcast,ap_index_type ap_index);


/*****************************************************************************
* Function Name : get_dhcp 
* Description   : 获取DHCP设定
* Input			: ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : host_ip_str： 主机地址 start_ip_str：开始地址 end_ip_str：结束地址
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean get_dhcp(char *host_ip_str, char *start_ip_str, char *end_ip_str, char *time_str);

/*****************************************************************************
* Function Name : get_client_count 
* Description   : 获取连接的客户端数
* Input			: ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : 连接总数
* Return        : 0:success, -1:failed
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
int get_client_count(ap_index_type ap_index);

/*****************************************************************************
* Function Name : get_mac_addr 
* Description   : 获取WIFI物理地址
* Input			: ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : *mac_addr
* Return        : true or false
* Auther        : caihaichuan
* Date          : 2021.10.28
*****************************************************************************/		
boolean get_mac_addr(char *mac_addr, ap_index_type ap_index);


/*****************************************************************************
* Function Name : get_sta_ip
* Description   : 设置STA连接外部热点后获取到的IP
* Input         : None
* Output        : ip_str
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	    
boolean get_sta_ip(char *ip_str, int len);


/*****************************************************************************
* Function Name : set_sta_cfg
* Description   : 设置STA连接外部AP的 SSID 和密码
* Input         : char *SSID, char *psk_value（密码）
* Output        : None
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean set_sta_cfg(char *ssid_str, char *psk_value);


/*****************************************************************************
* Function Name : get_sta_cfg
* Description   : 获取STA设置的SSID 和 密码
* Input         : char *SSID, char *psk_value（密码）
* Output        : None
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean get_sta_cfg(char *ssid_str, char *psk_value);	

/*****************************************************************************
* Function Name : sta_scan
* Description   : wifi sta 扫描可用热点
* Input         : None
* Output        : list_str
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean sta_scan(char *list_str);

/*****************************************************************************
* Function Name : sta_scan_with_sig
* Description   : wifi sta 扫描可用热点
* Input         : None
* Output        : list_str
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/
boolean sta_scan_with_sig(char *list_str);

/*****************************************************************************
* Function Name : set_user_name_pwd
* Description   : 设置cdma模式拨号的用户名和密码
* Input         : sz_usrname sz_usrpwd
* Output        : None
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean set_user_name_pwd(char *sz_usrname, char *sz_usrpwd);

/*****************************************************************************
* Function Name : get_user_name_pwd
* Description   : 获取cdma模式拨号的用户名和密码
* Input         : None
* Output        : sz_usrname sz_usrpwd
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean get_user_name_pwd(char *sz_usrname, int len_name, char *sz_usrpwd, int len_pwd);

/*****************************************************************************
* Function Name : get_net_status
* Description   : 获取网络状态
* Input         : None
* Output        : net_enable_str to_be_save
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
boolean get_net_status(char *net_enable_str, int *to_be_save);

/*****************************************************************************
* Function Name : restore_wifi
* Description   : 恢复wifi设定
* Input         : None
* Output        : None
* Return        : void
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/		
void restore_wifi();

boolean set_ssid_and_auth(char *SSID, char *str_pwd, int auth_type, int  encrypt_mode, ap_index_type ap_index);


boolean wifi_is_sta_mode();

/*****************************************************************************
* Function Name : sta_init
* Description   : open/close STA mode for W58L
* Input         : int: sta_enable
* Output        : None
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean sta_init(int sta_enable);


/*****************************************************************************
* Function Name : get_sta_cfg
* Description   : get the STA state for W58L
* Input         : None
* Output        : flag
* Return        : boolean
* Auther        : qjh
* Date          : 2018.04.11
*****************************************************************************/	
boolean get_sta_status(uint8 *flag);

/*****************************************************************************
* Function Name : wifi_net_cnct
* Description   : connect or disconnect the net;
                  if save the net status for later reboot
* Input         : act  to_be_dave
* Output        : None
* Return        : boolean
* Auther        : qjh
* Date          : 2018.10.26
*****************************************************************************/	
boolean wifi_net_cnct(int act, int to_be_save);

/*****************************************************************************
* Function Name : wifi_get_net_status
* Description	: get the net connected or disconnected status;
* Input 		: None
* Output		: act
* Return		: boolean
* Auther		: dyy
* Date			: 2018.12.27
*****************************************************************************/	
boolean wifi_get_net_status(int *act);

/*****************************************************************************

* Function Name : wifi_get_err_code
* Description   : get the latest error code
* Input         : None
* Output        : None
* Return        : unit8
* Auther        : qjh
* Date          : 2018.10.26
*****************************************************************************/	
uint8 wifi_get_err_code();

/*****************************************************************************
* Function Name : set_isolate
* Description   : 设置WIFI 隔离开关
* Input			: int isolate，0 ----disable
                               1 ----enable
                ap_index_type: 0-> ap
                               1-> ap & ap
                               2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.10.28
*****************************************************************************/		
boolean set_isolate(int isolate, ap_index_type ap_index);

/*****************************************************************************
* Function Name : get_isolate 
* Description   : 获取WIFI 隔离状态
* Input			:
            ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : int *isolate_state_ptr
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.10.28
*****************************************************************************/	
boolean get_isolate(int *isolate_state_ptr,ap_index_type ap_index);



/*****************************************************************************
* Function Name : set_channel
* Description   : 设置WIFI信道
* Input			: int channel 0~11
                ap_index_type: 0-> ap
                               1-> ap & ap
                               2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.10.28
*****************************************************************************/
boolean set_channel(int channel, ap_index_type ap_index);


/*****************************************************************************
* Function Name : get_channel 
* Description   : 获取WIFI 当前所处信道
* Input			:
            ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : int *channel
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.10.28
*****************************************************************************/	
boolean get_channel(int *channel,ap_index_type ap_index);



/*****************************************************************************
* Function Name : set_netmode
* Description   : 设置WIFI 802.11模式
* Input			: int netmode E_11B---1  
                              E_11G---2  
                              E_11BG---3  
                              E_11N---4)
                ap_index_type: 0-> ap
                               1-> ap & ap
                               2-> ap & sta
* Output        : None
* Return        : true or false
* Auther        : caihaichuan
* Date          : 2021.11.05
*****************************************************************************/
boolean set_netmode(int netmode, ap_index_type ap_index);



/*****************************************************************************
* Function Name : set_band
* Description   : 设置WIFI 802.11ng 模式下的带宽
* Input			: int band:
                           1 ----HT40
                   default 0 ----HT20
                ap_index_type: 0-> ap
                               1-> ap & ap
                               2-> ap & sta
* Output        : None
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.11.05
*****************************************************************************/		
boolean set_band(int  band, ap_index_type ap_index);


/*****************************************************************************
* Function Name : get_netmode 
* Description   : 获取WIFI WIFI 802.11模式
* Input			:
            ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : int *mode
                        E_11B---1  
                        E_11G---2  
                        E_11BG---3  
                        E_11N---4
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.11.05
*****************************************************************************/	
boolean get_netmode(int *mode, ap_index_type ap_index);


/*****************************************************************************
* Function Name : get_band 
* Description   : 获取WIFI 802.11ng 模式下的带宽
* Input			:
            ap_index_type: 0-> ap
		             1-> ap & ap
		             2-> ap & sta
* Output        : int *band
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.11.05
*****************************************************************************/	
boolean get_band(int *band, ap_index_type ap_index);



/*****************************************************************************
* Function Name : get_clients_info 
* Description   : 获取client的信息
* Input         : type 
                   GET_ALL_CLIENTS_STAT -----0
                   GET_CONN_CLIENTS    ------1
                   default 0
* Output        : client_info_node_ptr * client_info_head_node
                   以单向链表的形式获取
* Return        : 0:success, -1:failed
* Auther        : caihaichuan
* Date          : 2021.11.11
*****************************************************************************/	
int get_clients_info(int type,client_info_node_ptr *client_info_head_node);


/*****************************************************************************
* Function Name : wifi_connect_num_config 
* Description   : 配置wifi最大连接个数
* Input         : int num    范围[5,32]
* Return        : true :success, false :failed
* Auther        : caihaichuan
* Date          : 2021.12.28
*****************************************************************************/	
boolean  wifi_connect_num_config(int num);



/*****************************************************************************
* Function Name : enable_wps_pushbutton 
* Description   : wps功能开关
* Input         : opt  1--enable  0--disable
* Return        : true :success, false :failed
* Auther        : caihaichuan
* Date          : 2022.1.11
*****************************************************************************/	
boolean enable_wps_pushbutton(int opt);

/*****************************************************************************
* Function Name : get_wps_pushbutton_state 
* Description   : 获取wps功能开关状态
* output        : int* opt  1--enable  0--disable
* Return        : true :success, false :failed
* Auther        : caihaichuan
* Date          : 2022.1.11
*****************************************************************************/	
boolean get_wps_pushbutton_state(int *opt);


/*****************************************************************************
* Function Name : wifi_call_wps_pbc 
* Description   : wps的pbc模式调用，
                  设备启动wps连接时，调用此函数
* Return        : true :success, false :failed
* Auther        : caihaichuan
* Date          : 2022.1.11
*****************************************************************************/	
boolean wifi_call_wps_pbc(void);


/*****************************************************************************
* Function Name : get_wps_pbc_state 
* Description   : 查看wps的pbc状态
* output         int * state
                  1----Active  已触发wps_pbc,正在等待连接
                  2----Disable 未触发wps_pbc,上次连接成功
                  3----Timeout 未触发wps_pbc,上次连接超时
* Return        : true :success, false :failed
* note          : 可从wps的pbc模式调用时的Active状态到结束的Disable状态
                  判断本次wps_pbc连接成功
* Auther        : caihaichuan
* Date          : 2022.3.30
*****************************************************************************/
boolean get_wps_pbc_state(int *state);

/*****************************************************************************
* Function Name : wifi_call_wps_pin 
* Description   : wps的pin模式调用，
                  设备启动wps_pin连接时，调用此函数
* Input         : pin: 设备设备启动wps_pin连接时，产出的pin码
* Return        : true :success, false :failed
* Auther        : caihaichuan
* Date          : 2022.1.11
*****************************************************************************/	
boolean wifi_call_wps_pin(const char *pin);


/*****************************************************************************
* Function Name : wifi_call_wps_cancel 
* Description   : 取消悬而未决的wps操作
* Return        : true :success, false :failed
* Auther        : caihaichuan
* Date          : 2022.1.11
*****************************************************************************/	
boolean wifi_call_wps_cancel(void);


/*****************************************************************************
* Function Name : wifi_set_mac_address_filter_mode
* Description   : Set MAC address filtering mode,(disable,allow,deny)
* Input         : filter(Only the disable allow deny mode can be selected)
* Output        : None
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.3.24
*****************************************************************************/
int wifi_set_mac_filter_mode(char *filter);


/*****************************************************************************
* Function Name : wifi_add_allow_mac_address_filter
* Description   : Add a MAC address in allow mode
                  The MAC address format must be XX:XX:XX:XX:XX:XX
* Input         : allowfilter(MAC address in allow mode)
* Output        : None
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.3.24
*****************************************************************************/
int wifi_add_allow_mac_address_filter(char *allowfilter);


/*****************************************************************************
* Function Name : wifi_add_deny_mac_address_filter
* Description   : Add a MAC address in deny mode
                  The MAC address format must be XX:XX:XX:XX:XX:XX
* Input         : denyfilter(MAC address in deny mode)
* Output        : None
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.3.24
*****************************************************************************/
int wifi_add_deny_mac_address_filter(char *denyfilter);


/*****************************************************************************
* Function Name : wifi_del_allow_mac_address_filter
* Description   : Delete a MAC address in allow mode
                  The MAC address format must be XX:XX:XX:XX:XX:XX
* Input         : allowfilter(MAC address in allow mode)
* Output        : None
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.3.24
*****************************************************************************/
int wifi_del_allow_mac_address_filter(char *allowfilter);


/*****************************************************************************
* Function Name : wifi_del_deny_mac_address_filter
* Description   : Delete a MAC address in deny mode
                  The MAC address format must be XX:XX:XX:XX:XX:XX
* Input         : denyfilter(MAC address in deny mode)
* Output        : None
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.3.24
*****************************************************************************/
int wifi_del_deny_mac_address_filter(char *denyfilter);


/*****************************************************************************
* Function Name : wifi_get_mac_filter_info
* Description   : Obtain information about MAC address filtering
* Input         : maclist_info_t
* Output        : Filtering mode(disable,allow,deny)
                  Allow list
                  Deny  list
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.3.24
*****************************************************************************/
int wifi_get_mac_filter_info(maclist_info_t *mac_info);


#endif

