#ifndef _VOICE_CALL_H_
#define _VOICE_CALL_H_

#include <libubox/ustream.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include "ril.h"
#include "simcom_sdk_indication.h"

typedef enum {
    VOICE_CALL_HANDUP = 1,
    VOICE_CALL_ANSWER,
    VOICE_CALL_HOLD,
    VOICE_END_ALL,
    VOICE_END_BG,
    VOICE_END_FG
}voice_call_option;

#define VOICE_QMI_TIMEOUT_VALUE 10000
#define VOICE_NUMBER_MAX        81
#define VOICE_CALL_INFO_MAX     7

typedef enum {
    VOICE_CALL_STATE_ACTIVE =   0,
    VOICE_CALL_STATE_HOLDING =  1,
    VOICE_CALL_STATE_DIALING =  2,  /* MO call only */
    VOICE_CALL_STATE_ALERTING = 3,  /* MO call only */
    VOICE_CALL_STATE_INCOMING = 4,  /* MT call only */
    VOICE_CALL_STATE_WAITING =  5,  /* MT call only */
    VOICE_CALL_STATE_OFFERING = 6,  /*MT call offering (call setup)*/
    VOICE_CALL_STATE_DISCONNECTING = 7,/*call in disconnect procedure*/
    VOICE_CALL_STATE_DISCONNECTED = 8, /*call is disconnected*/
} voice_call_state;

typedef enum {
    VOICE_CALL_DIRECTION_MO = 0x00,
    VOICE_CALL_DIRECTION_MT = 0x01,
}voice_call_direction;

typedef enum {
    CALL_VOLUME_CONFIG_TX,
    CALL_VOLUME_CONFIG_RX
}call_volume_config_type;

typedef enum {
    AUDIO_DEVICE_EARPIECE,
    AUDIO_DEVICE_SPEAKER,
    AUDIO_DEVICE_HEADSET,
}audio_device_type;


typedef struct {
    voice_call_state   state;
    int             index;      /* Connection Index for use with, eg, AT+CHLD */
    int             toa;        /* type of address, eg 145 = intl */
    char            is_mpty;    /* nonzero if is mpty call */
    int             direction;  /* refer voice_call_direction 0:MO 1:MT*/
    char            als;        /* ALS line indicator if available (0 = line 1) */
    char            is_voice;   /* nonzero if this is is a voice call */
    char            number[VOICE_NUMBER_MAX];     /* Remote party number */
}voice_call_info;

typedef struct {
    int call_num;
    voice_call_info call_info_list[VOICE_CALL_INFO_MAX];
}simcom_voice_call_info;

typedef struct
{
    int status;                   /* 0: not active, 1:active */
    int reason;                   /* see CallForward_Reason */
    int serviceClass;             /*is a sum of integers each representing a
                                    class of information (default 7):
                                    1: voice (telephony)
                                    2: data (refers to all bearer services; with
                                    <mode>=2 this may refer only to some bearer
                                    service if TA does not support values 16, 32, 64
                                    and 128)
                                    4: fax (facsimile services)
                                    8: short message service
                                    16: data circuit sync
                                    32: data circuit async
                                    64: dedicated packet access
                                    128: dedicated PAD access
                                  */
    char number[VOICE_NUMBER_MAX];
}CallForward_Info;

typedef enum
{
    CALL_FORWARD_REASON_UNCONDITIONAL,
    CALL_FORWARD_REASON_BUSY,
    CALL_FORWARD_REASON_NO_REPLY,
    CALL_FORWARD_REASON_NOT_REACHABLE,
    CALL_FORWARD_REASON_MAX
}CallForward_Reason;

typedef enum
{
    CALL_FORWARD_MODE_NOT_ACTIVE,
    CALL_FORWARD_MODE_ACTIVE,
    CALL_FORWARD_MODE_QUERY_STATUS,
    CALL_FORWARD_MODE_REGISTRATION,
    CALL_FORWARD_MODE_ERASURE,
    CALL_FORWARD_MODE_MAX,
}CallForward_Mode;

void get_call_info_msg(void);

/*****************************************************************************
 * FUNCTION
 *  voice_init
 *
 * DESCRIPTION
 *  init voice
 *
 * PARAMETERS
 *  simcom_event_ind_cb: register call_back to process received sms
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 *
 *****************************************************************************/
int voice_init(simcom_event_ind_cb ind_cb);

/*****************************************************************************
 * FUNCTION
 *  voice_deinit
 *
 * DESCRIPTION
 *  release sms
 *
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  if quit voice ,call this
 *****************************************************************************/
void voice_deinit();

/*****************************************************************************
* Function Name : voice_dial
* Description   : 拨打电话
* Input         : phoneNum  电话号码
* Output        : None
* Return        : 0：success   -1： fail
* Auther        : qjh
* Date          : 2018.04.19
*****************************************************************************/
void voice_dial(char *phoneNum);

/*****************************************************************************
* Function Name : voice_mtcall_process
* Description   : 处理通话或者来电
* Input         : voice_call_option：   
            ：  VOICE_CALL_HANDUP   挂断
            ：  VOICE_CALL_ANSWER   接听
            ：  VOICE_CALL_HOLD     挂起当前电话，接入新来电
            ：  VOICE_END_BG        挂断后台，保持当前通话
            ：  VOICE_END_FG        挂断当前电话， 后台电话转为接入
            ：call_id  标记每一路通话的ID
* Output        : None
* Return        : 0：success   -1： fail
* Auther        : qjh
* Date          : 2018.04.19
*****************************************************************************/
void voice_mtcall_process(voice_call_option option);

/*****************************************************************************
* Function Name : get_all_call_info_msg
* Description   : 获取所有通话信息
* Input         : None
* Output        : call_infos
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2018.04.19
*****************************************************************************/
int get_all_call_info_msg(simcom_voice_call_info *call_infos);

/*****************************************************************************
* Function Name : set_call_waiting
* Description   : set call wait action
* Input         : action
                    0:deactive call waiting
                    1:active waiting
* Output        : None
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2021.12.29
*****************************************************************************/
int set_call_waiting(int action);

/*****************************************************************************
* Function Name : set_call_waiting
* Description   : set call wait action
* Input         : 
* Output        : action
                  0:deactive call waiting
                  1:active waiting
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2022.01.05
*****************************************************************************/
int query_call_waiting(int *action);

/*****************************************************************************
* Function Name : set_volume
* Description   : set volume
* Input         : call_volume_config_type type:0-TX  1-RX
                  int volume:range[-36db~12db]
* Output        : 
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2022.01.05
*****************************************************************************/
int set_volume(call_volume_config_type type, int volume);

/*****************************************************************************
* Function Name : set_mute
* Description   : set mute
* Input         : call_volume_config_type type:0-TX  1-RX
                  int action: 0-unmute  1:mute
* Output        : 
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2022.01.05
*****************************************************************************/
int set_mute(call_volume_config_type type, int action);

/*****************************************************************************
* Function Name : send_dtmf
* Description   : send dtmf
* Input         : char code: 0~9, *, #, A, B, C, D
* Output        : 
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2022.01.05
*****************************************************************************/
int send_dtmf(char code);

/*****************************************************************************
* Function Name : set_audio_device
* Description   : send audio device
* Input         : audio_device_type type
                  0 -- EARPIECE
                  1 -- SPEAKER
                  2 -- HEADSET
* Output        : 
* Return        : 0：success   -1： fail
* Auther        : 
* Date          : 2022.03.02
*****************************************************************************/
int set_audio_device(audio_device_type type);

/*****************************************************************************
* Function Name : get_callforward_info
* Description   : 获取呼叫转移的设置信息
* Input         : reason:
                    0: unconditional
                    1: mobile busy
                    2: no reply
                    3: not reachable
* Output        : status:
                    1:actice
                    0:not active
                  number:
* Return        : 0：success   -1： fail
* Auther        :
* Note          : 当status = 0(未激活)时, number为空, 哪怕已经注册了号码
* Date          : 2022.03.11
*****************************************************************************/
int get_callforward_info(int reason, CallForward_Info *info);

/*****************************************************************************
* Function Name : dective_callforward
* Description   : 去激活呼叫转移
* Input         : reason
* Return        : 0：success   -1： fail
* Auther        :
* Note          : Can be actived by "active_callforward" after dective
* Date          : 2022.03.11
*****************************************************************************/
int dective_callforward(int reason);

/*****************************************************************************
* Function Name : active_callforward
* Description   : 激活呼叫转移
* Input         : reason
* Return        : 0：success   -1： fail
* Auther        :
* Date          : 2022.03.11
*****************************************************************************/
int active_callforward(int reason);

/*****************************************************************************
* Function Name : regist_callforward
* Description   : 注册呼叫转移
* Input         : reason
                  number
* Return        : 0：success   -1： fail
* Auther        :
* Note          : Overwrite if you have previously registered
* Date          : 2022.03.11
*****************************************************************************/
int regist_callforward(int reason, char *number);

/*****************************************************************************
* Function Name : regist_callforward
* Description   : 擦除呼叫转移
* Input         : reason
* Return        : 0：success   -1： fail
* Auther        :
* Note          : Can not active after erase
* Date          : 2022.03.11
*****************************************************************************/
int erase_callforward(int reason);

#endif

