/*******************************************************************************

 *******************************************************************************/
/**
 *  RDA5807
 */
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>

#include "I2C.h"
#include "RDA5807M.h"

uint8_t DHIZ = 0 << 7;
uint8_t DMUTE = 0 << 6;
uint8_t MONO = 1 << 5;
uint8_t BASS = 1 << 4;
uint8_t RCLK_CALIBRATEMODE = 1 << 3;
uint8_t RCLK_DIRECT_INPUT_MODE = 0 << 2;
uint8_t SEEKUP = 0 << 1;
uint8_t SEEK = 0;

uint8_t SKMODE = 0 << 7;
uint8_t CLK_MODE = 0b000 << 4;
uint8_t RDS_EN = 1 << 3;
uint8_t NEW_METHOD = 1 << 2;
uint8_t SOFT_RESET = 1 << 1;
uint8_t ENABLE = 0;

int resetFM()
{

	DHIZ = 0 << 7;
	DMUTE = 0 << 6;
	uint8_t buf[2];
	buf[0] = DHIZ | DMUTE | MONO | BASS | RCLK_CALIBRATEMODE | RCLK_DIRECT_INPUT_MODE | SEEKUP |
			 SEEK;

	SOFT_RESET = 1 << 1;
	ENABLE = 0;
	buf[1] = SKMODE | CLK_MODE | RDS_EN | NEW_METHOD | SOFT_RESET | ENABLE;

	return sim_i2c_write(SLAVE_ADDR_INDEX, 0x02, buf, 2);
}

int powerupFm()
{
	// RDS/RBDS disable
	// return WriteReg(0x02, 0xF1, 0x05, 600);
	// RDS/RBDS enable
	uint8_t buf[2];
	buf[0] = DHIZ | DMUTE | MONO | BASS | RCLK_CALIBRATEMODE | RCLK_DIRECT_INPUT_MODE | SEEKUP |
			 SEEK;

	SOFT_RESET = 0 << 1;
	ENABLE = 1;
	buf[1] = SKMODE | CLK_MODE | RDS_EN | NEW_METHOD | SOFT_RESET | ENABLE;

	return sim_i2c_write(SLAVE_ADDR_INDEX, 0x02, buf, 2);
}

int openRadio()
{
	resetFM();
	return powerupFm();
}

int closeRadio()
{

	return resetFM();
}

int chanle(int chanelnum)
{
	int chan = (chanelnum - 76000) / 100;
	uint8_t buf[2];
	buf[0] = ((chan & 0xFFFF) >> 2) & 0xFF;
	buf[1] = chan << 6 & 0x00FF | 0x18;

	return sim_i2c_write(SLAVE_ADDR_INDEX, 0x03, buf, 2);
}

int mute(int isMute)
{
	if (isMute)
	{
		DMUTE = 0 << 6;
		DHIZ = 0 << 7;
	}
	else
	{
		DHIZ = 1 << 7;
		DMUTE = 1 << 6;
	}

	uint8_t buf[2];
	buf[0] = DHIZ | DMUTE | MONO | BASS | RCLK_CALIBRATEMODE | RCLK_DIRECT_INPUT_MODE | SEEKUP |
			 SEEK;
	buf[1] = SKMODE | CLK_MODE | RDS_EN | NEW_METHOD | SOFT_RESET | ENABLE;

	return sim_i2c_write(SLAVE_ADDR_INDEX, 0x02, buf, 2);
}
// vol:10-100>>>>>1-15
int vol(int vol)
{
	int vv = vol * 15 / 100;

	uint8_t buf[2];
	buf[0] = 0x88;
	buf[1] = 0x80 | (vv & 0x0F);
	return sim_i2c_write(SLAVE_ADDR_INDEX, 0x05, buf, 2);
}

int testFM()
{
	openRadio();
	mute(0);

	vol(5);
	chanle(92400);
	int opt = 0;
	char scan_string[100] = {0};

	while (1)
	{
		printf("\nPlease enter vol 0-100 , 999:exit  \n");
		printf("num > ");

		memset(scan_string, 0, sizeof(scan_string));
		if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
		{
			continue;
		}
		opt = atoi(scan_string);
		printf("%d\n", opt);
		if (opt == 999)
		{

			mute(1);
			closeRadio();
			break;
		}

		else
			vol(opt);
	}
}