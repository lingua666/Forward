#ifndef _SIMCOM_COMMON_H
#define _SIMCOM_COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simcom_sdk_common.h"

#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#define DEBUGLOG(format,...) printf("### SYSLOG ### %s,%s[%d] "format"\n",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#define DEBUG_NO(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUGLOG(format,...)
#define DEBUG_NO(format,...)
#endif

#ifndef __int8_t_defined
# define __int8_t_defined
typedef signed char     int8_t;
typedef short int       int16_t;
typedef int             int32_t;
# if __WORDSIZE == 64
typedef long int        int64_t;
# else
__extension__
typedef long long int   int64_t;
# endif
#endif

/* Unsigned. */
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;

#if __WORDSIZE == 64
typedef unsigned long int   uint64_t;
#else
__extension__
typedef unsigned long long int uint64_t;
#endif

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint8_t  boolean;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAIL -1

#define SIMCOM_TEST_UI


/*************************************************************
 *  input 
 *          --scan_string                     :input string
 *          --nlen_scan_string                :input string length  
 *          --nMaxLen                         :max len
 *          --nMinLen                         :min len  
 *  output
 *          --out_str                         :output string
  
 * ret
 * -1 len too short
 * 1 len too long
 * 2 error
 *************************************************************/
int simcom_handle_input_string(char* scan_string, int nlen_scan_string, char* out_str, int nMaxLen, int nMinLen);

/*************************************************************
 *  input 
 *  --scan_string                 :input string
 *  --nlen_scan_string            :input string length  
 * ret
 * 0xFFFF error
 * number  success
 *************************************************************/
int simcom_handle_input_number(char* scan_string, int nlen_scan_string);

void simcom_fgets(char *scan_string, uint32_t scan_len);

void process_simcom_ind_message(simcom_event_e event,void *cb_usr_data);

#ifdef SIMCOM_TEST_UI
void simcom_test_main();
#endif

#endif /* _SIMCOM_COMMON_H */
