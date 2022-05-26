#ifndef _SMS_MESSAGE_BASE_H_
#define _SMS_MESSAGE_BASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simcom_sdk_common.h"

//#include "ms_sms_message_header.h"

#define SMS_MESSAGE_ENCODING_UNKNOWN 0
#define SMS_MESSAGE_ENCODING_7BIT    1
#define SMS_MESSAGE_ENCODING_8BIT    2
#define SMS_MESSAGE_ENCODING_16BIT   3
#define SMS_MESSAGE_MAX_USER_DATA_BYTES     140
#define SMS_MESSAGE_MAX_USER_DATA_BYTES_WITH_HEADER 134
#define SMS_MESSAGE_MAX_USER_DATA_SEPTES    160
#define SMS_MESSAGE_MAX_USER_DATA_SEPTES_WITH_HEADER    153


int is_ascii(char *data, int length);
int is_utf8(char *data, int length);
int ascii_to_hex(const char *ascii, int ascii_length, char *hex, int *hex_length);
int hex_to_ascii(const char *hex, int hex_length, char *ascii, int *ascii_length);
int ascii_to_hex_low(char *ascii, int ascii_length, char *hex, int *hex_length);
int hex_to_ascii_low(char *hex, int hex_length, char *ascii, int *ascii_length);
int gsm_to_ascii(char *in, int in_length, char *out, int *out_length);
int ascii_to_gsm(char *in, int in_length, char *out, int *out_length);
int encode_7bit(char *in, int in_length, int padding_length, char *out, int *out_length);
int encode_8bit(char *in, int in_length, char *out, int *out_length);
int encode_ucs2(char *in, int in_length, char *out, int *out_length);        
int decode_7bit(char *in, int in_length, int padding_length, char *out, int *out_length);
int decode_8bit(char *in, int in_length, char *out, int *out_length);
int decode_ucs2(char *in, int in_length, char *out, int *out_length);

int convert_get_utf8_size(uint8 *in);
int convert_utf8_to_ucs2(uint8 *in, uint16 *out, int *outsize);
int convert_ucs2_to_utf8(uint8* in, int inlen, char* out, int *out_length);

int ascii_gsm_length(char ascii);
int calculate_gsm_length(char *text);
int calculate_ascii_length(char *text, int pos, int limit);
int calculate_utf8_length(char *text, int pos, int limit);

//list<string> *fragment_text(string &text);


#endif /*_SMS_MESSAGE_BASE_H_*/
