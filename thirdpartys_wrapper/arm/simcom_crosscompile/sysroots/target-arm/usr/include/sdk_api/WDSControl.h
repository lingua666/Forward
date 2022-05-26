#ifndef _WDS_CONTROL_H_
#define _WDS_CONTROL_H_

#include "simcom_sdk_common.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char boolean;
typedef unsigned char uint8;

//int get_apnInfo(int profile_index, int* pdp_type, char* apn_str, char *username, char *password);
//int set_apnInfo(int profile_index, int pdp_type, char* apn_str, char *username, char *password);

/******************************NOTE*******************************************
 ******************************NOTE*******************************************
  This interface document is the interface description for the cm dialer. 
  If you do not need the cm dialer, you can use the datacall.h interface and you need to remove the cm dialer.
 ******************************NOTE*******************************************
*******************************NOTE********************************************/

/*****************************************************************************
* FUNCTION
*  get_apn_profile_info
*
* DESCRIPTION
*  get apn profile infomation

* PARAMETERS
*  apn_info
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int get_apn_profile_info(wds_apn_profile_info *apn_info);

/*****************************************************************************
* FUNCTION
*  set_apn_profile_info
*
* DESCRIPTION
*  set apn profile infomation
*  If use the set_apn_profile_info,auto_apn will be set to 0.
*  You can run active_auto_apn to set auto apn to 1

* PARAMETERS
*  apn_info
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int set_apn_profile_info(wds_apn_profile_info apn_info);

/*****************************************************************************
* FUNCTION
*  active_auto_apn
*
* DESCRIPTION
*  active auto apn

* PARAMETERS
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int active_auto_apn();

/*****************************************************************************
* FUNCTION
*  set_auto_connect_config
*
* DESCRIPTION
*  set auto connect config
   0--manual connect
   1--auto connect

* PARAMETERS
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int set_auto_connect_config(int auto_connect);


/*****************************************************************************
* FUNCTION
*  active_default_pdp_context
*
* DESCRIPTION
*  active default pdp context

* PARAMETERS
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int active_default_pdp_context();

/*****************************************************************************
* FUNCTION
*  deactive_default_pdp_context
*
* DESCRIPTION
*  deactive default pdp context

* PARAMETERS
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int deactive_default_pdp_context();


/*****************************************************************************
* FUNCTION
*  get_default_pdp_context
*
* DESCRIPTION
*  get default pdp context
   data_call_info_t *data_call_info

* PARAMETERS
*
* RETURNS
*  -1, FAIL
*  0, SUCCESS
*
* NOTE
* 
*****************************************************************************/
int get_default_pdp_context(data_call_info_t *data_call_info);
#endif


