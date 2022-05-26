#ifndef _DMZ_CONTROL_H_
#define _DMZ_CONTROL_H_

#include "simcom_sdk_common.h"



/*****************************************************************************
* Function Name : set_dmz
* Description   : Set the IP address of the DMZ
* Input         : host (Enter the IP address of the DMZ)
* Output        : host (DMZ ip)
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.1.18
*****************************************************************************/
int set_dmz_ip(char *host);

/*****************************************************************************
* Function Name : get_dmz_ip
* Description   : Gets the DMZ IP
* Input         : host
* Output        : host (DMZ ip，Failure will be displayed if there is no IP)
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.1.18
*****************************************************************************/
int get_dmz_ip(char *host);

/*****************************************************************************
* Function Name : close_dmz
* Description   : Disable the DMZ function
* Input	        : None
* Output        : None
* Return        : 0:EXIT_SUCCESS ,-1:EXIT_FAIL
* Auther        : linhuanzhong
* Date          : 2022.1.18
*****************************************************************************/
 int close_dmz();

#endif
