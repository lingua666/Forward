#ifndef _SSP_H_
#define _SSP_H_

#include <stdint.h>

typedef enum _SSP_MODE_E
{
    SSP_MODE_0 = 0,
    SSP_MODE_1,
    SSP_MODE_2,
    SSP_MODE_3,
}
SSP_MODE_E;

typedef struct _SSPCONTROL_MODE_S
{
    SSP_MODE_E emode : 2;
    uint8_t isCsHigh : 1;
    uint8_t isLsb : 1;
    uint8_t is3wire : 1;
    uint8_t isLoop : 1;
    uint8_t isNoCs : 1;
    uint8_t isReady : 1;
}
SSPCONTROL_MODE_S;

typedef union _SSPCONTROL_MODE_U
{
    uint8_t byte;
    SSPCONTROL_MODE_S smode;
}
SSPCONTROL_MODE_U;

typedef struct _SSPCONTROL_S
{
    SSPCONTROL_MODE_U umode;
    uint8_t bits;
    uint32_t speed;
    uint16_t delay;
}
SSPCONTROL_S;


int sim_ssp_bytes(SSPCONTROL_S *ctl, uint8_t *send, uint8_t *recv, uint32_t len);

#endif
