#ifndef _SIMCOM_SDK_INDICATION_H
#define _SIMCOM_SDK_INDICATION_H
#include "simcom_sdk_common.h"

typedef enum
{
    SIMCOM_IND_ID_SIM = 0,
    SIMCOM_IND_ID_NAS,
    SIMCOM_IND_ID_SMS,
    SIMCOM_IND_ID_VOICE,
    SIMCOM_IND_ID_PB,
    SIMCOM_IND_ID_WIFI,
    SIMCOM_IND_ID_ALL
}simcom_ind_id_type;

typedef void (*simcom_event_ind_cb)(simcom_event_e simcom_event, void *event_info);


/*****************************************************************************
 * FUNCTION
 *  sdk_indication_init
 *
 * DESCRIPTION
 *  init indication
 *
 * PARAMETERS
 *  simcom_event_ind_cb: register call_back to process received
 *
 * RETURNS
 *  -1, FAIL
 *  0, SUCCESS
 *
 * NOTE:If init, it will fork a child process to monitor and handle incation msg
 *
 *****************************************************************************/
int sdk_indication_init(simcom_event_ind_cb ind_cb);


/*****************************************************************************
 * FUNCTION
 *  sdk_indication_deinit
 *
 * DESCRIPTION
 *  deinit indication
 *
 * PARAMETERS
 *  void
 *
 * RETURNS
 *  void
 *
 * NOTE:
 *
 *****************************************************************************/
void sdk_indication_deinit();

/*****************************************************************************
 * FUNCTION
 *  sdk_indication_register
 *
 * DESCRIPTION
 *  register indication
 *
 * PARAMETERS
 *  simcom_ind_id_type id, simcom_event_ind_cb ind_cb
 *
 * RETURNS
 *  0:OK  -1:Failed
 *
 * NOTE:
 *
 *****************************************************************************/
int sdk_indication_register(simcom_ind_id_type id, simcom_event_ind_cb ind_cb);

/*****************************************************************************
 * FUNCTION
 *  sdk_indication_deregister
 *
 * DESCRIPTION
 *  deregister indication
 *
 * PARAMETERS
 *  simcom_ind_id_type id
 *
 * RETURNS
 *  0:OK  -1:Failed
 *
 * NOTE:
 *
 *****************************************************************************/
void sdk_indication_deregister(simcom_ind_id_type id);

int on_new_sms(char *data);


#endif /* _SIMCOM_SDK_INDICATION_H */
