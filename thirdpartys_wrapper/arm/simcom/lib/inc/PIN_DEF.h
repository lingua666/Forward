#ifndef _PINDEF_H_
#define _PINDEF_H_

 //GPIO25/PCM_IN
#define MP3_DREQ 28
//GPIO24/PCM_OUT
#define RESET_PIN 27
//GPIO26/PCM_SYNC
#define XCS 26
//GPIO27/PCM_CLK
#define MP3_DREQ_PATH "/sys/class/gpio/gpio28/value"

#define AMP_MUTE 19
#define AMP_EN 13

#define PIN_AUX_CTRL 20
#define PIN_IN 23
#define PIN_OUT 31
#define PIN_GNSS_SW 22
#define LED_NET "net"
#define LED_STATUS "status"
#define LED_ALARM "alarm"
#endif
