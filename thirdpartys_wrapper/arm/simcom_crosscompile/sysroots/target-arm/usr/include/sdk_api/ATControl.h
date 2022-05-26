#ifndef _AT_CONTROL_H_
#define _AT_CONTROL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <uci.h>

#define SIMCOM_AT_PORT   "/tmp/atcmdni"
#define ARRAY_SIZE         100
#define BAND_CONFIG_FILE "/etc/config/wan"

#define ICCID_LENGTH 20
#define IMEI_LENGTH  15
#define CIMI_LENGTH  15

typedef enum
{
    ACT_GSM = 0,
    ACT_UMTS_TD,
    ACT_UMTS_WB,
    ACT_LTE,
}ACCESS_TECH;

typedef enum
{
    GSM_BAND = 0,
    UMTS_BAND,
    LTEH_BAND,
    LTEL_BAND,
    MAX_BAND,
}BAND_INFO;

typedef enum {
    SDK_SEARCH_NET_2G = 0,
    SDK_SEARCH_NET_3G = 1,
    SDK_SEARCH_NET_4G = 2,
    SDK_SEARCH_NET_5G = 3,
    SDK_SEARCH_NET_NONE = 4,
}simcom_search_net_type;

struct simcom_search_net_info{
    int mcc;
    int mnc;
    simcom_search_net_type net;
    int rssi;
    struct simcom_search_net_info *next;
};


typedef enum {
    CPSI_ACCESS_TECH,
    CPSI_MCC,
    CPSI_MNC,
    CPSI_AREA_CODE,
    CPSI_CELL_ID,
    CPSI_BAND,
    CPSI_FREQUENCY,
    CPSI_FIELD_MAX,
}CPSI_FIELD;

typedef enum {
    CPSI_ACT_NO_SERVICE,
    CPSI_ACT_GSM,
    CPSI_ACT_WCDMA,
    CPSI_ACT_LTE,
    CPSI_ACT_MAX,
}CPSI_ACT;

typedef enum {
    PGSM,
    DCS_GSM,
    PCS_GSM,
    EGSM,
    GSM,
    GSM_BAND_FIELD_MAX,
}GSM_BAND_FIELD;

typedef enum{
    WCDMA_IMT_2000,
    WCDMA_PCS_1900,
    WCDMA_1700,
    WCDMA_850,
    WCDMA_800,
    WCDMA_2600,
    WCDMA_900,
    WCDMA_BAND_FIELD_MAX,
}WCDMA_BAND_FIELD;

typedef struct registed_network_info{
    int access_tech;               // LTE, WCDMA, GSM, NO_SERVICE, reference to CPSI_ACT
    int mcc;
    int mnc;
    long area_code;
    int cell_id;
    int freq_band;                 //reference to BAND_FIELD
    int earfcn;
}registed_network_info;

int atctrl_init();
void atctrl_deinit();

/***************************************************************************
* Function Name: sendATCmd 					
* Function Introduction: send at command to LTE module
* Parameter description:
*     atCmd    : at command
*     finalRsp : at command's response result.
*     buff     : if need function return result, you should pass the buff,
*                then the function will store the result into buff 
*     buffSize : buff size, At least 100bits.
*     timeout_ms: time out
* Function return value:  -1: failed; 0: return error,
*                         other: return data length.
* Data : 2017.09.08									
****************************************************************************/
int sendATCmd(char* atCmd, char* finalRsp, char *buff, uint32_t buffSize, long timeout_ms);
int getModuleRevision(char *pBuff, int size);
int getCPIN();
int getCSQ(uint8_t *rssi);
int getCREG();
int getICCID(char *pBuff, int size);
int getIMEI(char *pBuff, int size);
int getCIMI(char *pBuff, int size);
int audioPlayTTS(char *tts);

/*****************************************************************************
 * Function Name : enable_or_disable_volte
 * Description   : 使能VoLTE功能
 * Input         : int enable_value
                   0:disable volte
                   1:enable volte
 * Output        : None
 * Return        : 0:success, -1:faild
 * Auther        : simcom
 * Note          : 设置后需要重启才能生效 
 *****************************************************************************/
int enable_or_disable_volte(int enable_value);

/*****************************************************************************
 * Function Name : manual_search_network
 * Description   : manual search network 
 * Input         : 
 * Output        : net_info_list_head：the list head of search network list
 * Return        : 0:success, -1:faild
 * Auther        : simcom
 * Note          : Must be free list,otherwise will be memory leak
 *****************************************************************************/
int manual_search_network(struct simcom_search_net_info **net_info_list_head);

/*****************************************************************************
 * Function Name : set_usb_at_switch
 * Description   : set usb at function on/off
 * Input         : 0: close usb at 1:open usb at
 * Output        : 
 * Return        : 0:success, -1:faild
 * Auther        : simcom
 * Note          : 
*****************************************************************************/
int set_usb_at_switch(int on_off);

/*****************************************************************************
 * Function Name : get_usb_at_switch
 * Description   : get usb at switch state
 * Input         : 
 * Output        : 0: close usb at 1:open usb at
 * Return        : 0:success, -1:faild
 * Auther        : simcom
 * Note          : 
*****************************************************************************/
int get_usb_at_switch(int *on_off);

/*****************************************************************************
 * Function Name : get_current_band
 * Description   : 获取当前频段锁配置
 * Input         : NULL
 * Output        : GSM_band
 *                 UMTS_band
 *                 LTEH_band
 *                 LTEL_band
 * Return        : 0:success, -1:faild
 * Note          : 返回值对应的二进制的每一位即对应的band
 *****************************************************************************/
int get_current_band(int *GSM_band, int *UMTS_band, int *LTEH_band, int *LTEL_band);

/*****************************************************************************
 * Function Name : lock_band
 * Description   : 锁频段
 * Input         : GSM_band
 *                 UMTS_band
 *                 LTEH_band
 *                 LTEL_band
 * Output        : NULL
 * Return        : 0:success, -1:faild
 * Note          : 输入0,对应制式频段锁保持不变
 *                 GSM_band = 8(EGSM_900)时,默认修改为9(PGSM_900,EGSM_900)
 *****************************************************************************/
int lock_band(int GSM_band, int UMTS_band, int LTEH_band, int LTEL_band);


/*****************************************************************************
 * Function Name : reset_band
 * Description   : 重置频段锁
 * Input         : NULL
 * Output        : NULL
 * Return        : 0:success, -1:faild
 * Note          : 将频段锁重置为默认状态
 *****************************************************************************/
int reset_band();

/*****************************************************************************
 * Function Name : get_frequency_cell_lock_info
 * Description   : 获取小区锁及频点锁信息
 * Input         : NULL
 * Output        : mode:0 ------ 无
 *                      1 ------ 锁频点
 *                      2 ------ 锁小区
 *                 act: 接入技术
 *                      0 ------ GSM
 *                      1 ------ UMTS-TD
 *                      2 ------ UMTS-WB
 *                      3 ------ LTE
 *                 freq:频点
 *                 cellid:小区识别码
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int get_frequency_cell_lock_info(int *mode, int *act, int *freq, int *cellid);

/*****************************************************************************
 * Function Name : lock_cell
 * Description   : 在freq频段上锁定cellid小区
 * Input         : act:接入技术
 *                      0 ------ GSM
 *                      1 ------ UMTS-TD
 *                      2 ------ UMTS-WB
 *                      3 ------ LTE
 *                 freq:频点
 *                 cellid:小区识别码
 * Output        : NULL
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int lock_cell(int act, int freq, int cellid);

/*****************************************************************************
 * Function Name : lock_frequency
 * Description   : 锁频点
 * Input         : act:接入技术
 *                      0 ------ GSM
 *                      1 ------ UMTS-TD
 *                      2 ------ UMTS-WB
 *                      3 ------ LTE
 *                 freq:频点
 * Output        : NULL
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int lock_frequency(int act, int freq);

/*****************************************************************************
 * Function Name : close_frequency_cell_lock
 * Description   : 关闭频点锁以及小区锁
 * Input         : NULL
 * Output        : NULL
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int close_frequency_cell_lock();

/*****************************************************************************
 * Function Name : set_gain_NITZtime_enable
 * Description   : 设置开启或关闭开机获取NITZ时间功能
 * Input         : 0 close ctzu    1 open ctzu
 * Output        : NULL
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int set_gain_NITZtime_enable(int act);

/*****************************************************************************
 * Function Name : query_gain_NITZtime_enable
 * Description   : 查询开启或关闭开机获取NITZ时间功能
 * Input         : NULL
 * Output        : 0: close ctzu 1:open ctzu
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int query_gain_NITZtime_enable(char *pBuff,int size);

/*****************************************************************************
 * Function Name : query_registe_network_information
 * Description   : 查询驻网信息
 * Input         : NULL
 * Output        : p_out_info
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int query_registe_network_information(registed_network_info *p_out_info);

/*****************************************************************************
 * Function Name : query_SIM_number
 * Description   : 查询SIM卡电话号码
 * Input         : NULL
 * Output        : p_out_number
 * Return        : 0:success, -1:faild
 * Note          : NULL
 *****************************************************************************/
int query_SIM_number(char *p_out_number);

#endif
