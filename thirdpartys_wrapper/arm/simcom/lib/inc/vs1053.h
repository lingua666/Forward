#ifndef VS1053_H
#define VS1053_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 /*******************************************************************************

 *******************************************************************************/
/**
 *  vs1053
 */
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PIN_DEF.h>

//VS10xx SCI Registers
#define SCI_MODE 0x00
#define SCI_STATUS 0x01
#define SCI_BASS 0x02
#define SCI_CLOCKF 0x03
#define SCI_DECODE_TIME 0x04
#define SCI_AUDATA 0x05
#define SCI_WRAM 0x06
#define SCI_WRAMADDR 0x07
#define SCI_HDAT0 0x08
#define SCI_HDAT1 0x09
#define SCI_AIADDR 0x0A
#define SCI_VOL 0x0B
#define SCI_AICTRL0 0x0C
#define SCI_AICTRL1 0x0D
#define SCI_AICTRL2 0x0E
#define SCI_AICTRL3 0x0F

//VS10xx SCI_MODE bitmasks
#define SM_CANCEL 0x08
#define SM_RESET 0x04

#define TRUE 1
#define FALSE 0

//tag location offsets
#define TRACK_TITLE 3
#define TRACK_ARTIST 33
#define TRACK_ALBUM 63








void initDREQ();

void activeXCS();
void disableXCS();
 void waitForDREQ();

void Mp3Write32(uint8_t address, uint8_t high, uint8_t low);

uint32_t Mp3Read32(uint8_t addressbyte);
void vs1053HardReset(void);

void vs1053SoftReset(void);
void vs1053SetVol(int vol);
void sciWriteRawVol(int vol);
void sciWriteVol(int vol);
void vs1053down();
int vs1053Init(int vol);
static void refill(int fd_file);

uint8_t stopPlay();
uint8_t playFile(char *fileName);
uint8_t playBuf(uint8_t *buf, int len);

 

#endif