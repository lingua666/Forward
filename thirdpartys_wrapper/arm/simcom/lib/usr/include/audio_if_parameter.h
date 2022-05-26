
/******************************************************************************
*(C) Copyright 2014 Marvell International Ltd.
* All Rights Reserved
******************************************************************************/
/* -------------------------------------------------------------------------------------------------------------------
 *
 *  Filename: audio_if_parameter.h
 *
 *  Authors:  tzahi stern
 *
 *  Description: audio_if interface to ubus
 *
 *  HISTORY:
 *
 *
 *  Notes:
 *
 ******************************************************************************/
#ifndef AUDIO_IF_PARAMETER_H
#define AUDIO_IF_PARAMETER_H

/****************************************************/
/**   define                                       **/
/****************************************************/

#define AUDIO_IF_UBUS_AUDIO_MODE_SET	"audio_mode_set"
#define AUDIO_IF_UBUS_AUDIO_MODE_STATUS	"audio_mode_status"
#define AUDIO_IF_UBUS_AUDIO_DEVICE_SET	"audio_device_set"
#define AUDIO_IF_UBUS_AUDIO_DEVICE_STATUS	"audio_device_status"
#define AUDIO_IF_UBUS_TTY_SET	"tty_set"
#define AUDIO_IF_UBUS_VOLUME_SET	"volume_set"
#define AUDIO_IF_UBUS_VOLUME_STATUS	"volume_status"
#define AUDIO_IF_UBUS_MUTE_SET	"mute_set"
#define AUDIO_IF_UBUS_MUTE_STATUS	"mute_status"
#define AUDIO_IF_UBUS_ECALL_DATA_SET "ecall_data_set"
#define AUDIO_IF_UBUS_ECALL_DATA_STATUS "ecall_data_status"
#define AUDIO_IF_UBUS_ECALL_VOICE_SET "ecall_voice_set"
#define AUDIO_IF_UBUS_ECALL_VOICE_STATUS "ecall_voice_status"
#define AUDIO_IF_UBUS_SWITCH_PCM	"switch_pcm"
#define AUDIO_IF_UBUS_DTMFDETECTION	"dtmf_detection"

#define AUDIO_IF_UBUS_VCM_REC_START "rec_start"
#define AUDIO_IF_UBUS_VCM_REC_STOP "rec_stop"
#define AUDIO_IF_UBUS_VCM_PLAY_START "play_start"
#define AUDIO_IF_UBUS_VCM_PLAY_STOP "play_stop"
#define AUDIO_IF_UBUS_CP_LOOPBACK_ENABLE "loopback_enable"
#define AUDIO_IF_UBUS_CP_LOOPBACK_DISABLE "loopback_disable"
#define AUDIO_IF_UBUS_VCM_CONFIG "vcm_configure"

#define AUDIO_IF_UBUS_PCMLOOPBACK_START "pcmloopback_start"
#define AUDIO_IF_UBUS_PCMLOOPBACK_STOP "pcmloopback_stop"

#define AUDIO_IF_UBUS_CONFIG_PCM "config_pcm"
#define AUDIO_IF_UBUS_CONFIG_DSPGAIN "config_dspgain"
#define AUDIO_IF_UBUS_CONFIG_PCMEXPERT "config_pcmexpert"
#define AUDIO_IF_UBUS_DTMFCONTROL "dtmf_control"
#define AUDIO_IF_UBUS_REPORT_VOICE_STATUS	"report_voice_status"

#define ACIPC_VCM_ECALL_DATA_BUFFER_MAX_LEN 140
#define AUDIO_IF_AUDIO_STUB_CTRL_BUFF_MAX_LEN 256

#define PCM_NB_BUF_SIZE 320
#define PCM_WB_BUF_SIZE 640


/****************   parameters in policy            ************/
#define PARAM_1_INT 1
#define PARAM_2_INT 2
#define PARAM_3_INT 3
#define PARAM_4_INT 4
#define PARAM_5_INT 5

/******************************************************************************
 * AP to CP Codes
 *****************************************************************************/
#define ACIPC_AUDIO_REQ_BASE                                0x0000

#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_SET                 (ACIPC_AUDIO_REQ_BASE+30)
#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_GET                 (ACIPC_AUDIO_REQ_BASE+31)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_SET                (ACIPC_AUDIO_REQ_BASE+32)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_GET                (ACIPC_AUDIO_REQ_BASE+33)


/******************************************************************************
 * CP to AP Codes
 *****************************************************************************/
#define ACIPC_AUDIO_IND_BASE                                0x0100

#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_IND                 (ACIPC_AUDIO_IND_BASE+30)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_IND                (ACIPC_AUDIO_IND_BASE+32)

#define ACIPC_AUDIO_CNF_BASE                                0x0200

#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_GET_CNF             (ACIPC_AUDIO_CNF_BASE+31)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_GET_CNF            (ACIPC_AUDIO_CNF_BASE+33)

//ATC cmd ID for audio
#define ATC_AUDIO_CMD  9

/* CP to AP */
#define CMD_TYPE_CONFIRM 0x1
#define CMD_TYPE_INDICATION 0x2

//ATC audio commands
#define ATC_HANDSHAKE 0x0
#define ATC_VOLUMECTL 0x1
#define ATC_MUTECTL 0x2
#define ATC_PATHCTL 0x3
#define ATC_EQCTL 0x4
#define ATC_LOOPBACKCTL 0x5
#define ATC_PCMRECCTL 0x6
#define ATC_PCMPLAYBACKCTL 0x7
#define ATC_PCMRECSTREAM 0x8
#define ATC_PCMPLAYSTREAM 0x9
#define ATC_ECALLCTL 0xa
#define ATC_PCMCTL    0xb
#define ATC_DTMFDETECTIONCTL    0xc
#define ATC_PCMCFG    0xd
#define ATC_PCMCFGEXPERT    0xe
#define ATC_DTMFCONTROL 0xf
#define ATC_INVALIDMSG 0xff

#define ECALL_DATA_SIZE 160

/******************************************************************************
 * structures
 *****************************************************************************/
struct atc_header {
	unsigned char cmd_code;
	unsigned char sub_cmd;
	unsigned char cmd_type;
	unsigned char data_len;
} __packed;


/*atc_ecall_msg is without atc-header: use in communication between user-space and kernel */
typedef struct _atc_ecall_msg{
	char	ecall_data[ECALL_DATA_SIZE];
	unsigned long ecall_data_len;
	unsigned long msg_id;
} __packed atc_ecall_msg;

/*ATCMsg is with atc-header: use in communication AP and CP */
struct ATCMsg {
    struct atc_header   header;
    union {
        unsigned short DTMF_code;
        atc_ecall_msg eCallPayload;
    };
}__packed;


/******************************************************************************
 * AP to CP structures
 *****************************************************************************/

// ECall
typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_SET {
    unsigned long command;
    unsigned long op;
    unsigned long param1;
    unsigned char data[ACIPC_VCM_ECALL_DATA_BUFFER_MAX_LEN];
} ACIPC_AUDIO_VCM_ECALL_DATA_SET, *P_ACIPC_AUDIO_VCM_ECALL_DATA_SET;

typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_GET {
    unsigned long command;
    unsigned long op;
    unsigned long param1;
} ACIPC_AUDIO_VCM_ECALL_DATA_GET, *P_ACIPC_AUDIO_VCM_ECALL_DATA_GET;

typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_SET {
    unsigned long command;
    unsigned long cmd_id;
    unsigned long res_id;
    unsigned long param2;
} ACIPC_AUDIO_VCM_ECALL_VOICE_SET, *P_ACIPC_AUDIO_VCM_ECALL_VOICE_SET;

typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_GET {
    unsigned long command;
    unsigned long cmd_id;
    unsigned long res_id;
} ACIPC_AUDIO_VCM_ECALL_VOICE_GET, *P_ACIPC_AUDIO_VCM_ECALL_VOICE_GET;

/******************************************************************************
 * CP to AP structures
 *****************************************************************************/

// ECall
typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_IND {
	unsigned long command;
	unsigned long urc_id;
	unsigned long data;
} ACIPC_AUDIO_VCM_ECALL_DATA_IND, *P_ACIPC_AUDIO_VCM_ECALL_DATA_IND;

typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_IND {
	unsigned long command;
	unsigned long res_id;
	unsigned long res_state;
} ACIPC_AUDIO_VCM_ECALL_VOICE_IND, *P_ACIPC_AUDIO_VCM_ECALL_VOICE_IND;

typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_GET_CNF {
    unsigned long command;
    unsigned long op;
    unsigned long param1;
    unsigned long value1;
    unsigned long value2;
} ACIPC_AUDIO_VCM_ECALL_DATA_GET_CNF, *P_ACIPC_AUDIO_VCM_ECALL_DATA_GET_CNF;

typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_GET_CNF {
    unsigned long command;
    unsigned long cmd_id;
    unsigned long res_id;
    unsigned long param2;
} ACIPC_AUDIO_VCM_ECALL_VOICE_GET_CNF, *P_ACIPC_AUDIO_VCM_ECALL_VOICE_GET_CNF;


/****************************************************/
/** enums                                         **/
/****************************************************/

typedef enum {
    PARAM_INT_POLICY_0,
    PARAM_INT_POLICY_1,
    PARAM_INT_POLICY_2,
    PARAM_INT_POLICY_3,
    PARAM_INT_POLICY_MAX,
} PARAM_INT_POLICY;

typedef enum {
    PARAM_ECALL_DATA_SET_POLICY_0,
    PARAM_ECALL_DATA_SET_POLICY_1,
    PARAM_ECALL_DATA_SET_POLICY_2,
    PARAM_ECALL_DATA_SET_POLICY_3,
    PARAM_ECALL_DATA_SET_POLICY_MAX,
} PARAM_ECALL_DATA_SET_POLICY;

typedef enum {
    PARAM_ECALL_DATA_GET_POLICY_0,
    PARAM_ECALL_DATA_GET_POLICY_1,
    PARAM_ECALL_DATA_GET_POLICY_2,
    PARAM_ECALL_DATA_GET_POLICY_MAX,
} PARAM_ECALL_DATA_GET_POLICY;

typedef enum {
    PARAM_ECALL_DATA_GET_REPLY_POLICY_0,
    PARAM_ECALL_DATA_GET_REPLY_POLICY_1,
    PARAM_ECALL_DATA_GET_REPLY_POLICY_2,
    PARAM_ECALL_DATA_GET_REPLY_POLICY_3,
    PARAM_ECALL_DATA_GET_REPLY_POLICY_MAX,
} PARAM_ECALL_DATA_GET_REPLY_POLICY;

typedef enum {
    PARAM_ECALL_VOICE_SET_POLICY_0,
    PARAM_ECALL_VOICE_SET_POLICY_1,
    PARAM_ECALL_VOICE_SET_POLICY_2,
    PARAM_ECALL_VOICE_SET_POLICY_MAX,
} PARAM_ECALL_VOICE_SET_POLICY;

typedef enum {
    PARAM_ECALL_VOICE_GET_POLICY_0,
    PARAM_ECALL_VOICE_GET_POLICY_1,
    PARAM_ECALL_VOICE_GET_POLICY_MAX,
} PARAM_ECALL_VOICE_GET_POLICY;

typedef enum {
    PARAM_ECALL_VOICE_GET_REPLY_POLICY_0,
    PARAM_ECALL_VOICE_GET_REPLY_POLICY_1,
    PARAM_ECALL_VOICE_GET_REPLY_POLICY_2,
    PARAM_ECALL_VOICE_GET_REPLY_POLICY_MAX,
} PARAM_ECALL_VOICE_GET_REPLY_POLICY;

typedef enum {
    PARAM_VCM_PLAY_REC_POLICY_0,
    PARAM_VCM_PLAY_REC_POLICY_MAX,
} PARAM_VCM_PLAY_REC_POLICY;

typedef enum {
    PARAM_VCM_CONFIG_POLICY_0,
    PARAM_VCM_CONFIG_POLICY_1,
    PARAM_VCM_CONFIG_POLICY_2,
    PARAM_VCM_CONFIG_POLICY_3,
    PARAM_VCM_CONFIG_POLICY_4,
    PARAM_VCM_CONFIG_POLICY_MAX,
} PARAM_VCM_CONFIG_POLICY;

typedef enum {
    PARAM_PCMEXPERT_CONFIG_POLICY_0,
    PARAM_PCMEXPERT_CONFIG_POLICY_1,
    PARAM_PCMEXPERT_CONFIG_POLICY_2,
    PARAM_PCMEXPERT_CONFIG_POLICY_3,
    PARAM_PCMEXPERT_CONFIG_POLICY_4,
    PARAM_PCMEXPERT_CONFIG_POLICY_MAX,
} PARAM_PCMEXPERT_CONFIG_POLICY;
#endif	//AUDIO_IF_PARAMETER_H
