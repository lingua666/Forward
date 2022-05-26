#ifndef _DMS_CONTROL_H_
#define _DMS_CONTROL_H_

#include "simcom_sdk_common.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*****************************************************************************
* Function Name : dms_init
* Description   : device manager module init
* Input         : 无
* Output        : 
* Return        : 0： succes     -1： Fail
* Date          : 2018.04.26
* Note          : 
*****************************************************************************/
int dms_init();

/*****************************************************************************
* Function Name : dms_deinit
* Description   : device manager module deinit
* Input         : 无
* Output        : 
* Return        : 0： succes     -1： Fail
* Date          : 2018.04.26
* Note          : 
*****************************************************************************/
void dms_deinit();


/*****************************************************************************
* Function Name : get_imei
* Description   : 查询sim卡 IMSI
* Input         : 无
* Output        : pImsi
* Return        : 0： succes     -1： Fail
* Auther        : qjh
* Date          : 2018.04.26
*****************************************************************************/
int get_imei(char *pImei);


/****************************************************************************
* Function Name : get_rev_id
* Description   : get firmware revision identification
* Input         : pointer to buffer 
* Output        : firmware revision identification
* Return        : 0 succes   1 Fail
* Auther        : lhy
* Date          : 2018.08.16
*****************************************************************************/		
int get_rev_id(char *pRev_id);


/*****************************************************************************
* Function Name : dms_set_operating_mode
* Description	: set UE operating mode
* Input         : mode value
                       0 Online            (equal  at+cfun=1)
                       1 Low power         (equal  at+cfun=0)
                       2 Factory Test mode 
                       3 Persistent low power
                       4 Resetting
                       5 Shutting down
                       6 Offline 
                       
* Output        : NONE
* Return        : 0 succes     1 Fail
* Auther        : lhy
* Date          : 2018.08.16
*****************************************************************************/
int dms_set_operating_mode(unsigned char mode);


/*****************************************************************************
* Function Name : dms_get_operating_mode
* Description	: get UE operating mode
* Input         : NONE
* Output        : mode value
                       0 Online            (equal  at+cfun=1)
                       1 Low power         (equal  at+cfun=0)
                       2 Factory Test mode 
                       3 Persistent low power
                       4 Resetting
                       5 Shutting down
                       6 Offline 

                       
* Return        : 0 succes     1 Fail
* Auther        : zxs
* Date          : 2019.07.25
*****************************************************************************/
int dms_get_operating_mode(unsigned char* mode);


/*****************************************************************************
* Function Name : time_get_modem_cclk
* Description	: get system time
* Input         : NONE
* Output        : return current time format with milli second(from 1970-1-1)
* Return        : 0 succes     1 Fail
*****************************************************************************/
int time_get_modem_cclk(uint64_t *msecs);

#endif


