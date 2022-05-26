/*******************************************************************************

 *******************************************************************************/
/**
 *  RDA5807
 */
#include <stdint.h>
 
#define SLAVE_ADDR_SEQ 0x10
#define SLAVE_ADDR_INDEX 0x11

 
int powerupFm();
int resetFM();
int openRadio();
int closeRadio();
int chanle(int chanelnum);
int mute(int isMute);
int vol(int vol);

 