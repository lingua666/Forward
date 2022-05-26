
#ifndef _SMS_MESSAGE_HEADER_H_
#define _SMS_MESSAGE_HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "ms_debug.h"

typedef struct
{
    int ref_number;
    int msg_count;
    int seq_number;
}sms_message_header;

#define SMS_HEADER_ID_CONCATENATED_8BIT_REFERENCE   0x00
#define SMS_HEADER_ID_SPECIAL_SMS_INDICATION        0x02
#define SMS_HEADER_ID_CONCATENATED_16BIT_REFERENCE  0x08

sms_message_header *parse_udh(char *udh, int udhl);
void encode_udh(sms_message_header *sms_header, char *udh, int *udhl);
    

#endif /*_SMS_MESSAGE_HEADER_H_*/
