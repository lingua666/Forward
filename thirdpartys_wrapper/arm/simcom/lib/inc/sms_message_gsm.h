
#ifndef _SMS_MESSAGE_GSM_H_
#define _SMS_MESSAGE_GSM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sms_message_base.h"
#include "sms_message_header.h"

#define SMS_MSG_SMSC_ADDR_LEN       32
#define SMS_MSG_PHONE_NUMBER_LEN    32
#define SMS_MSG_TIMESTAMP_LEN       32
#define SMS_MSG_CONTEXT_LEN         256
#define SMS_MSG_PDU_LEN             512


typedef enum
{
    SMS_DCS_TYPE_7BIT = 1,
    SMS_DCS_TYPE_8BIT = 2,
    SMS_DCS_TYPE_UCS2 = 3,
}sms_dcs_type;

typedef struct
{
    sms_dcs_type dcs_type;
    char smsc_address[SMS_MSG_SMSC_ADDR_LEN];
    char number_address[SMS_MSG_PHONE_NUMBER_LEN];
    char time_stamp[SMS_MSG_TIMESTAMP_LEN];
    int  message_content_len;
    char message_content[SMS_MSG_CONTEXT_LEN];
    char message_pdu[SMS_MSG_PDU_LEN];

    int is_concate_sms;
    sms_message_header sms_header;
}sms_message_gsm;


int decode_pdu(const char *pdu, sms_message_gsm *sms_message);
void encode_pdu(char *number, char *content, sms_message_header *sms_header, sms_message_gsm *sms_message);
void decode_normal_pdu(char *pdu_hex, int pdu_type, sms_message_gsm *sms_message);
void decode_sms_receipt(char *pdu_hex, sms_message_gsm *sms_message);

//int encode_msc(char *msc, char *pdu, int *pdu_length);
//int decode_msc(char *pdu, int pdu_length, char *msc);
int encode_number(char *number, char *pdu, int *pdu_length);
int decode_number(char *pdu, int pdu_length, char *number);
int encode_vaild_period(long vaild_period, char *pdu, int *pdu_length);
int decode_time_stamp(char *pdu, int pdu_length, char *time_stamp);

sms_message_gsm *create_from_pdu(const char *pdu);
sms_message_gsm *create_from_content(char *number, char *content);
sms_message_gsm *create_from_content_multipart(char *number, char *content, sms_message_header *sms_header);
int is_sms_pdu_format(const char *pdu);

#endif /*_SMS_MESSAGE_GSM_H_*/
