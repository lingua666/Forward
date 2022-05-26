 /** 
* @file         simcom_loc.h 
* @brief        SIMCom  Location API
* @author       xuxiaohui
* @date         2021/6/29
* @version      V1.0.0 
* @par Copyright (c):  2021 SIMCom Wireless Solutions Co.,Ltd, All Rights Reserved
*/ 
#ifndef SIMCOM_LOC_H
#define SIMCOM_LOC_H

#include "simcom_sdk_common.h"
#include "simcom_sdk_indication.h"


/** Milliseconds since January 1, 1970 */
typedef int64_t GpsUtcTime;

#define false	0
#define true	1

#ifndef bool
#define bool int
#endif


/** Location info. WGS-84 coordinate system*/
typedef struct {
     /** set to sizeof(GpsLocation) */
    uint32_t          size;
    /** Contains GpsLocationFlags bits. */
    uint16_t        flags;
    /** Represents latitude in degrees. */
    double          latitude;
    /** Represents longitude in degrees. */
    double          longitude;
    /** Represents altitude in meters above the WGS 84 reference
     * ellipsoid. */
    double          altitude;
    /** Represents speed in meters per second. */
    float           speed;
    /** Represents heading in degrees. */
    float           bearing;
    /** Represents expected accuracy in meters. */
    float           accuracy;
    /** Timestamp for the location fix. */
    GpsUtcTime      timestamp;
    /** Y-M-D H-M-S*/
    uint16_t  time[6];
} simcom_loc_info;


/** 
* @brief
*   Initialize Location function
* @param
*   [in] loc_ind_cb: Receive Location info callback
* @return 
*   0  : SUCCESS \n
*  -1  : FAIL
* @note  
*   The function must be called before all other loc api
*/  
int simcom_loc_init(simcom_event_ind_cb loc_ind_cb);

/** 
* @brief
*   deinit Location function
* @param
*   NONE
* @return 
*   0  : SUCCESS \n
*  -1  : FAIL
* @note  
*   NONE
*/  
void simcom_loc_deinit();


/** 
* @brief
*   Cold Start Location
* @param
*   NONE
* @return 
*   0  : SUCCESS \n
*  -1  : FAIL
* @note  
*   
*/  
int simcom_loc_coldstart();
/** 
* @brief
*   Hot Start Location
* @param
*   NONE
* @return 
*   0  : SUCCESS \n
*  -1  : FAIL
* @note  
*   
*/  
int simcom_loc_hotstart();

/** 
* @brief
*   Stop Location engine
* @param
*   NONE
* @return 
*   0  : SUCCESS \n
*  -1  : FAIL
* @note  
*   NONE
*/  
int simcom_loc_stop();

void simcom_loc_agnss_enable();

void simcom_loc_agnss_disable();

int simcom_loc_poweroff(void);

#endif /* SIMCOM_LOC_H */
