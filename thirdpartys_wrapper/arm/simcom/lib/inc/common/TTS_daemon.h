#ifndef _BT_DAEMON_H_
#define _BT_DAEMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdarg.h>

#define key_length     16
#define OUT_PCM_FILE                  "/data/synthResult.pcm"

#ifdef FEATURE_DEFAULT_MODVOL
#define lyTTS_VOLUME_MIN               0
#define lyTTS_VOLUME_MAX               8
#define lyTTS_VOLUME_NORMAL      6
#else
#define lyTTS_VOLUME_MIN               0
#define lyTTS_VOLUME_MAX               9
#define lyTTS_VOLUME_NORMAL      5
#endif

#define lyTTS_SPEED_MIN               0
#define lyTTS_SPEED_MAX               9
#define lyTTS_SPEED_NORMAL      4
#define lyTTS_PITCH_MIN               0
#define lyTTS_PITCH_MAX               9
#define lyTTS_PITCH_NORMAL      5

#ifdef FEATURE_DEFAULT_LETTER
#define lyTTS_ENGMODE_DEFAULT      0
#else
#define lyTTS_ENGMODE_DEFAULT      0
#endif 

typedef enum 
{
    SYNC_NORMAL_END,
    SYNC_STOP_END,
    SYNC_CONTINUE,
    SYNC_UNKNOW_ERROR
} syncErrCode;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef  unsigned char     byte;    

extern unsigned char lyTTS_digitMode[4][20];
extern unsigned char lyTTS_engMode[4][20];

typedef struct Account_info
{
    unsigned char   app_key_[key_length];
    unsigned char   developer_key_[50];
    unsigned char   cloud_url_[30];
    unsigned char   auth_path_[30];
    unsigned char   cap_key_[key_length];
    unsigned char   data_path_[30];
    unsigned char   log_file_path_[30];
    unsigned char   res_Prefix_[30];
    unsigned char   audio_format_[20];
    int volume;
    int speed;
    int pitch;
    unsigned char digitMode_[20];
    unsigned char engMode_[20];
}Account_info;

typedef struct ctts_args{
    unsigned char    tts_text[1024];
    unsigned char    tts_filename[1024];
    int      len;
    int      mode;
    int             demopid;
}ctts_args;

typedef enum thread_working{
	NO_WORKING,
	AMR_WORKING,
	MP3_WORKING,
	AAC_WORKING,
	WAV_WORKING,
	TTS_WORKING,
	MID_WORKING,
	EXT_WORKING
}thread_working;

// error return code
typedef enum 
{
	jtTTS_ERR_NONE,				//	无错误
	
	jtTTS_ERR_TIME_EXPIRED,		//	授权时间过期
	jtTTS_ERR_LICENCE,			//	授权错误

	jtTTS_ERR_INPUT_PARAM,		//	传入参数错误
	jtTTS_ERR_TOO_MORE_TEXT,	//	输入文本太长
	jtTTS_ERR_NOT_INIT,			//	引擎没有初始化，或者没有正确初始化
	jtTTS_ERR_OPEN_DATA,		//	打开资源数据错误
	jtTTS_ERR_NO_INPUT,			//	没有输入文本
	jtTTS_ERR_MORE_TEXT,		//	文本没有合成完毕
	jtTTS_ERR_INPUT_MODE,		//	输入方式错误
	jtTTS_ERR_ENGINE_BUSY		//  引擎在工作中

} jtErrCode;


void setUserPara(void *para);
int initAccont(Account_info *account_info);
UINT8 UCS2toUTF8Code(UINT16 ucs2_code, UINT8* utf8_code);
void qcmap_gbk2utf8(char *p_gbkStr, char *p_utf8Str);
//void Start_trans(char * data,int len);
//void Start_trans_utf8(char * data);
//int poc_getDeviceInfo(char * imei);


#endif

