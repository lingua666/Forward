#ifndef _DATA_CALL_H_
#define _DATA_CALL_H_

#include <math.h>
#include "DnsResolv.h"
#include "simcom_sdk_common.h"


typedef enum 
{
    IF_DOWN = 0,
    IF_UP   = 1
} if_command;


typedef struct {
  int handle;
  //app_tech_e tech;
  int family;
  int profile;
  //datacall_status_type status;
  char if_name[32];
  char ip_str[16];
  char pri_dns_str[16];
  char sec_dns_str[16];
  char gw_str[16];
  unsigned int mask;
  int end_reason; 
}datacall_info_type;


/*****************************************************************************
    此套拨号接口专门用于脱离cm拨号程序的接口,如不需要cm辅助拨号,则可以使用此套
    拨号接口用于自己管理拨号
 *****************************************************************************/

/*****************************************************************************
 * FUNCTION
 *  datacall_init
 *
 * DESCRIPTION
 *  init datacall
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 * 
 *****************************************************************************/
int datacall_init();

/*****************************************************************************
 * FUNCTION
 *  datacall_deinit
 *
 * DESCRIPTION
 *  deinit
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 * 
 *****************************************************************************/
int datacall_deinit();

/*****************************************************************************
 * FUNCTION
 *  set_initial_attach_apn
 *
 * DESCRIPTION
 *  set initial attach apn
 *
 * PARAMETERS
 *  wds_apn_profile_info *apn_info
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 * 
 *****************************************************************************/
int set_initial_attach_apn(wds_apn_profile_info apn_info);

/*****************************************************************************
 * FUNCTION
 *  active_data_call
 *
 * DESCRIPTION
 *  active data call
 *
 * PARAMETERS
 *  cid: default Bearer = 1
 *  apn_info
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 * 
 *****************************************************************************/
int active_data_call(int cid, wds_apn_profile_info apn_info);

/*****************************************************************************
 * FUNCTION
 *  deactive_data_call
 *
 * DESCRIPTION
 *  deactive data call
 *
 * PARAMETERS
 *  cid: default Bearer = 1
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 * 
 *****************************************************************************/
int deactive_data_call(int cid);

/*****************************************************************************
 * FUNCTION
 *  query_data_call
 *
 * DESCRIPTION
 *  query data call
 *
 * PARAMETERS
 *  
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE
 * 
 *****************************************************************************/
int query_data_call(data_call_info_t *data_call_info);


#endif /* _DATA_CALL_H_ */
