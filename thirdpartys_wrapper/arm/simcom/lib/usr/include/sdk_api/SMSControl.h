#ifndef _SMSCONTROL_H_
#define _SMSCONTROL_H_

#include "simcom_sdk_common.h"

/////////////////////////////////////////////////////////////
// MACRO CONSTANTS DEFINITIONS
///////////////////////////////////////////////////////////// 
#define TRUE   1
#define FALSE  0

#define SMS_MAX_ADDRESS_LENGTH  48
#define SMS_MAX_CONTENT_LENGTH  255
#define SMS_SMSC_ADDRESS_LENGTH 64
#define RIL_REQUEST_SET_CSCS (RIL_REQUEST_EXT_BASE + 15)
#define RIL_REQUEST_EXT_BASE 500

#define LOG printf

/////////////////////////////////////////////////////////////
// ENUM TYPE DEFINITIONS
/////////////////////////////////////////////////////////////
typedef enum
{
    SMS_MSG_FORMAT_TEXT_NULL,
    SMS_MSG_FORMAT_TEXT_ASCII   = 1,
    SMS_MSG_FORMAT_TEXT_UTF8    = 2,
    SMS_MSG_FORMAT_TEXT_UNICODE = 3,
    SMS_MSG_FORMAT_TEXT_MAX
}sms_format;

/////////////////////////////////////////////////////////////
// STRUCT TYPE DEFINITIONS
/////////////////////////////////////////////////////////////

typedef unsigned char uint8;
typedef unsigned char boolean;

typedef struct
{
    boolean     is_concate;
    uint8       msg_ref;
    uint8       seq_num;
    uint8       total_sm;
    sms_format  format;
    int         message_len;
    char        message_content[SMS_MAX_CONTENT_LENGTH];
    char        source_address[SMS_MAX_ADDRESS_LENGTH];
}sms_info_type;

typedef void (*sms_ind_cb_fcn)(simcom_event_e simcom_event, sms_info_type *sms_info);

/////////////////////////////////////////////////////////////
// GLOBAL DATA DEFINITION
/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS
/////////////////////////////////////////////////////////////

/*****************************************************************************
 * FUNCTION
 *  sms_init
 *
 * DESCRIPTION
 *  init sms 
 *  
 * PARAMETERS
 *  sms_ind_cb_fcn: register call_back to process received sms
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 *  
 *****************************************************************************/
int sms_init(sms_ind_cb_fcn sms_ind_cb);

/*****************************************************************************
 * FUNCTION
 *  sms_deinit
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
 *  if quit sms ,call this
 *****************************************************************************/
void sms_deinit();


/*****************************************************************************
 * FUNCTION
 *  send_message
 *
 * DESCRIPTION
 *  send sms api
 
 * PARAMETERS
 *  smsMode:                  1:ASCII  2:UTF8 3:UCS2
 *  phoneNumber:            destination address
 *  content:                     send message content
 *  content_len:               send message length
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 *  
 *****************************************************************************/
int send_message(char *phoneNumber, unsigned char *content, int content_len);

/*****************************************************************************
* FUNCTION
*  get_smsc_addr
*
* DESCRIPTION
*  get smsc address

* PARAMETERS
*  smsc_addr
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
*  
*****************************************************************************/
int get_smsc_addr(char *smsc_addr);

/*****************************************************************************
* FUNCTION
*  set_cscs
*
* DESCRIPTION
*  set cscs

* PARAMETERS
*  chest
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
*  chest must be "GSM","UCS2" OR "IRA".
*****************************************************************************/
int set_cscs(char *chest);

/*****************************************************************************
* FUNCTION
*  set_smsc_addr
*
* DESCRIPTION
*  set smsc addr

* PARAMETERS
*  smsc_addr
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
*  smsc_addr must start with +.
*  must be number behind + and at least one number.
*****************************************************************************/
int set_smsc_addr(char *smsc_addr);


#endif
