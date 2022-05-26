/*******************************************************************************

 *******************************************************************************/
/**
 *  vs1053
 */
#include "vs1053.h"
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

#include <dirent.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "Util.h"

// spi
#define SSPCONTROL_SSP_BUS "/dev/spidev1.0"

typedef enum _SSP_MODE_E
{
	SSP_MODE_0 = 0,
	SSP_MODE_1,
	SSP_MODE_2,
	SSP_MODE_3,
} SSP_MODE_E;

typedef struct _SSPCONTROL_MODE_S
{
	SSP_MODE_E emode : 2;
	uint8_t isCsHigh : 1;
	uint8_t isLsb : 1;
	uint8_t is3wire : 1;
	uint8_t isLoop : 1;
	uint8_t isNoCs : 1;
	uint8_t isReady : 1;
} SSPCONTROL_MODE_S;

typedef union _SSPCONTROL_MODE_U
{
	uint8_t byte;
	SSPCONTROL_MODE_S smode;
} SSPCONTROL_MODE_U;

typedef struct _SSPCONTROL_S
{
	SSPCONTROL_MODE_U umode;
	uint8_t bits;
	uint32_t speed;
	uint16_t delay;
} SSPCONTROL_S;
int spi_fd = -1;

SSPCONTROL_S ssp_lowspeed = {
	.umode = {{0}},
	.bits = 8,
	.speed = 800000,
	.delay = 0,
};
SSPCONTROL_S ssp_highspeed = {
	.umode = {{0}},
	.bits = 8,
	.speed = 13000000,
	.delay = 0,
};
SSPCONTROL_S *currentSSPCTL = NULL;
int lastVol = 0;

static uint8_t playing;
static uint8_t isDown;

static uint8_t mp3DataBuffer[32];

static uint8_t mp3DataBufferRX[32];
int currentVOL = -1;

// #define SM_EARSPEAKER_LO  0x10
// #define SM_EARSPEAKER_HI 0x80
uint8_t SM_EARSPEAKER_LO = 0x00;
uint8_t SM_EARSPEAKER_HI = 0x00;

uint8_t ST_AMP = 0x7A;
uint8_t SB_AMP = 0x3A;
uint16_t SC_FREQ = 0;

int fd_DREQ = -1;
struct pollfd fds[1];

static inline int sim_ssp_open(void)
{
	spi_fd = open(SSPCONTROL_SSP_BUS, O_RDWR);
	if (spi_fd < 0)
	{
		printf("sim_ssp_open fail! please check spidev1.0 dts\r\n");
		return -2;
	}
	return spi_fd;
}

static inline void sim_ssp_close()
{
	printf("sim_ssp_close\n");
	int ret = close(spi_fd);
	spi_fd = -1;
	return ret;
}

static int sim_ssp_setMode(int fd, SSPCONTROL_S *ctl)
{
	int ret;

	if ((fd <= 0) || !ctl)
	{
		return -1;
	}

	if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &ctl->umode.byte))
	{
		return -2;
	}
	if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &ctl->umode.byte))
	{
		return -2;
	}

	if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &ctl->bits))
	{
		return -2;
	}
	if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &ctl->bits))
	{
		return -2;
	}

	if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &ctl->speed))
	{
		return -2;
	}
	if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &ctl->speed))
	{
		return -2;
	}

	return 0;
}

// static int sim_ssp_transfer(int fd, SSPCONTROL_S *ctl, uint8_t *send, uint8_t *recv, uint32_t len)
// {
// 	if ((fd <= 0) || !ctl || !send || !recv || !len)
// 	{
// 		return -1;
// 	}

// 	struct spi_ioc_transfer trans =
// 		{
// 			.tx_buf = (unsigned long)send,
// 			.rx_buf = (unsigned long)recv,
// 			.len = len,
// 			.delay_usecs = ctl->delay,
// 			.speed_hz = ctl->speed,
// 			.bits_per_word = ctl->bits,
// 		};

// 	if (ioctl(fd, SPI_IOC_MESSAGE(1), &trans) < 1)
// 	{
// 		//   printf("sim_ssp ioctl fail \r\n");
// 		return -2;
// 	}

// 	return 0;
// }

static inline int sim_ssp_bytes(SSPCONTROL_S *ctl, uint8_t *send, uint8_t *recv, uint32_t len)
{
	int ret;

	// if (!ctl || !send || !recv || !len)
	// {
	// 	return -1;
	// }

	if (currentSSPCTL != ctl)
	{
		ret = sim_ssp_setMode(spi_fd, ctl);
		if (ret)
		{
			printf("sim_ssp_setMode fail \r\n");
			sim_ssp_close(spi_fd);
			return ret;
		}
		currentSSPCTL = ctl;
	}

	// ret = sim_ssp_transfer(spi_fd, ctl, send, recv, len);
	//  printf("sim_ssp_transfer ok \r\n");
	//  sim_ssp_close(spi_fd);

	struct spi_ioc_transfer trans =
		{
			.tx_buf = (unsigned long)send,
			.rx_buf = (unsigned long)recv,
			.len = len,
			.delay_usecs = ctl->delay,
			.speed_hz = ctl->speed,
			.bits_per_word = ctl->bits,
		};
	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &trans);
	if (ret < 1)
	{
		//   printf("sim_ssp ioctl fail \r\n");
		return -2;
	}

	return 0;

	// return ret;
}

void initDREQ()
{
	initGpio(MP3_DREQ);
	setGpioDirection(MP3_DREQ, "in");
	setGpioEdge(MP3_DREQ, "both");

	fd_DREQ = open(MP3_DREQ_PATH, O_RDONLY);
	if (fd_DREQ < 0)
	{
		perror("open  dreq/value' failed!\n");
		return -1;
	}
	fds[0].fd = fd_DREQ;
	fds[0].events = POLLPRI;
}

void activeXCS()
{

	setGpioValue(XCS, 0);
}
void disableXCS()
{

	setGpioValue(XCS, 1);
}

void waitForDREQ()
{
	int timeoutCount = 0;
	// Wait for DREQ to go high indicating IC is available
	char value_str[3];
	int value = 0;
	int ret = 0;

	if (lseek(fd_DREQ, 0, SEEK_SET) == -1)
	{
		perror("lseek failed!\n");
		return -1;
	}

	if (read(fd_DREQ, value_str, 3) < 0)
	{
		perror("Failed to read value!");
		return -1;
	}
	value = atoi(value_str);
	// printf("get value:%d\n", value);
	if (value == 1)
	{
		return;
	}

#define debug 0
#if debug
	int count = 0;
#endif
	while (1)
	{
		ret = poll(fds, 1, 1000);
		if (ret == -1)
		{
			perror("poll failed!\n");
			return -1;
		}
		else if (ret == 0)
		{
			perror("poll timeout!\n");

			if (lseek(fd_DREQ, 0, SEEK_SET) == -1)
			{
				perror("lseek failed!\n");
				return -1;
			}
			if (read(fd_DREQ, value_str, 3) < 0)
			{
				perror("Failed to read value!");
				return -1;
			}
			value = atoi(value_str);

			// printf("poll value:%d\n", value);
			if (value == 1)
			{
				break;
			}
			timeoutCount += 1;
			if (timeoutCount > 5)
			{
				vs1053HardReset();
				vs1053SoftReset();
				sciWriteVol(lastVol);
 			}
		}
		else
		{
			if (fds[0].revents & POLLPRI)
			{

				if (lseek(fd_DREQ, 0, SEEK_SET) == -1)
				{
					perror("lseek failed!\n");
					return -1;
				}
				if (read(fd_DREQ, value_str, 3) < 0)
				{
					perror("Failed to read value!");
					return -1;
				}
				value = atoi(value_str);

				// printf("poll value:%d\n", value);
				if (value == 1)
				{
					break;
				}
				else
				{
#if debug
					count += 1;
#endif
					usleep(100);
				}
			}
		}
	}
#if debug
	if (count > 0)
	{
		printf("count:%d \n", count);
	}
#endif
}

void Mp3Write32(uint8_t address, uint8_t high, uint8_t low)
{

	// struct aw_spi_trans_config cfg_sci;
	// cfg_sci.bits_per_word = 8;
	// cfg_sci.mode = AW_SPI_MODE_0;
	// cfg_sci.cs_index = 0;
	// cfg_sci.speed_hz = 0;
	// cfg_sci.is_lsb_first = 0;

	// Wait for DREQ to go high indicating IC is available
	waitForDREQ();
	// Wait for DREQ to go high indicating IC is available
	// Select control

	// SCI consists of instruction byte, address byte, and 16-bit data word.

	uint8_t txbuf[4];
	uint8_t rxbuf[4] = {0};
	txbuf[3] = low;
	txbuf[2] = high;
	txbuf[1] = address;
	txbuf[0] = 0x02;
	activeXCS();
	if (sim_ssp_bytes(&ssp_lowspeed, txbuf, rxbuf, 4))
	{
		printf("SPI DATA Transfer failed\r\n");
	}
	disableXCS();
}

uint32_t Mp3Read32(uint8_t addressbyte)
{

	// Wait for DREQ to go high indicating IC is available
	waitForDREQ();
	// Wait for DREQ to go high indicating IC is available
	// Select control

	// SCI consists of instruction byte, address byte, and 16-bit data word.

	uint8_t txbuf[4];
	uint8_t rxbuf[4] = {0};
	txbuf[3] = 0xFF;
	txbuf[2] = 0xFF;
	txbuf[1] = addressbyte;
	txbuf[0] = 0x03;
	activeXCS();
	if (sim_ssp_bytes(&ssp_lowspeed, txbuf, rxbuf, 4))
	{
		printf("SPI DATA Transfer failed\r\n");
	}
	else
	{
		printf("Mp3ReadRegister aw_spi_read 0x%X: 0x%X ,0x%X \r\n", addressbyte, rxbuf[2], rxbuf[3]);
	}
	disableXCS();
	return rxbuf[2] << 8 | rxbuf[3];
}
void vs1053HardReset(void)
{
	initGpio(RESET_PIN);
	setGpioDirection(RESET_PIN, "out");

	setGpioValue(RESET_PIN, 0);
	usleep(100000);
	setGpioValue(RESET_PIN, 1);
}

void vs1053SoftReset(void)
{
	printf("vs1053SoftReset\r\n");
	Mp3Write32(SCI_MODE, 0x48, SM_RESET | SM_EARSPEAKER_HI | SM_EARSPEAKER_LO);
	usleep(100000);
	uint16_t SC_MULT = 0x8000;
	uint16_t SC_ADD = 0x800;
	uint16_t clock = SC_MULT | SC_ADD | SC_FREQ;

	Mp3Write32(SCI_CLOCKF, (clock & 0xFF00) >> 8, clock & 0x00FF);

	Mp3Write32(SCI_BASS, ST_AMP, SB_AMP);

	Mp3Write32(SCI_AUDATA, 0xAC, 0x44);
	Mp3Write32(SCI_DECODE_TIME, 0x00, 0x00);
	Mp3Write32(SCI_DECODE_TIME, 0x00, 0x00);

	uint32_t MP3Mode = Mp3Read32(SCI_MODE);

	printf("sci mode %d \r\n", MP3Mode);
	uint32_t MP3Clock = Mp3Read32(SCI_CLOCKF);

	printf("MP3Clock %d \r\n", MP3Clock);
}
void vs1053SetVol(int vol)
{

	if (playing)
	{
		currentVOL = vol;
		return;
	}

	sciWriteVol(vol);
}
// void sciWriteRawVol(int vol)
// {
// 	Mp3Write32(SCI_VOL, vol, vol);
// }
void sciWriteVol(int vol)
{

	if (vol == 0)
	{
		Mp3Write32(SCI_VOL, 254, 254);
	}
	else if (vol <= 10)
	{
		Mp3Write32(SCI_VOL, 130, 130);
	}
	else if (vol <= 20)
	{
		Mp3Write32(SCI_VOL, 110, 110);
	}
	else if (vol <= 30)
	{
		Mp3Write32(SCI_VOL, 90, 90);
	}
	else if (vol <= 40)
	{
		Mp3Write32(SCI_VOL, 70, 70);
	}
	else if (vol <= 50)
	{
		Mp3Write32(SCI_VOL, 60, 60);
	}
	else if (vol <= 60)
	{
		Mp3Write32(SCI_VOL, 50, 50);
	}
	else if (vol <= 70)
	{
		Mp3Write32(SCI_VOL, 40, 40);
	}
	else if (vol <= 80)
	{
		Mp3Write32(SCI_VOL, 30, 30);
	}
	else if (vol <= 90)
	{
		Mp3Write32(SCI_VOL, 10, 10);
	}
	else
	{
		Mp3Write32(SCI_VOL, 2, 2);
	}
}

void vs1053down()
{

	printf("vs1053down............\n");
	// Mp3Write32(SCI_VOL, 255, 255);
	isDown = 1;
	while (playing)
	{
		printf("playing,waiting!!\n");
		sleep(1);
	}

	// vs1053SoftReset();
	initGpio(RESET_PIN);
	setGpioDirection(RESET_PIN, "out");
	setGpioValue(RESET_PIN, 0);

	sim_ssp_close();
	printf("vs1053down!!!!!\n");
}

int vs1053Init(int vol)
{
	int ret;
	ret = sim_ssp_open();
	if (ret < 0)
	{
		printf("vs1053Init failed");
		return -1;
	}

	initGpio(XCS);
	setGpioDirection(XCS, "out");
	disableXCS();

	vs1053HardReset();
	initDREQ();

	vs1053SoftReset();

	vs1053SetVol(vol);
	lastVol = vol;
	// Mp3Write32( SCI_VOL, 0x28, 0x28);
	// Mp3Write32(SCI_MODE, 0x48, 0x02);
	Mp3Write32(SCI_DECODE_TIME, 0x00, 0x00);
	Mp3Write32(SCI_DECODE_TIME, 0x00, 0x00);
	currentVOL = -1;
	isDown = FALSE;
	playing = FALSE;
	return 0;
}

uint8_t stopPlay()
{
	playing = FALSE;
}

inline uint8_t playBuf(uint8_t *buf, int len)
{
	if (isDown)
	{
		printf("isDown init first\n");
		return -1;
	}
	if (playing)
	{
		printf("playing\n");
		return -3;
	}

	playing = TRUE;

	int pos = 0;
	int datasize = 32;
	int ret = 0;
	while (playing)
	{
		// printf("playbuf %d %d\n", pos, len);
		waitForDREQ();

		if (pos >= len)
		{
			playing = FALSE;
			break;
		}
		if (len - pos < 32)
		{
			datasize = len - pos;
			pos += datasize;
		}
		else
		{
			datasize = 32;
			pos += 32;
		}
		if (isDown)
		{
			printf("isDown, end playbuf\r\n");
			ret = -1;
			break;
		}
		if (sim_ssp_bytes(&ssp_highspeed, buf, mp3DataBufferRX, datasize))
		{
			printf("SPI DATA Transfer failed,%d,%d,%d\r\n", pos, datasize, len);
			printf("SPI DATA Transfer failed\r\n");
			ret = -2;
			break;
		}
		buf = buf + datasize;
	}
	playing = FALSE;
	return ret;
}
SSPCONTROL_S ssp_config = {
	.umode = {{0}},
	.bits = 8,
	.speed = 8000000,
	.delay = 0,
};
inline uint8_t Audio_Play_32(uint8_t *buf, int len)
{

	if (sim_ssp_bytes(&ssp_config, buf, mp3DataBufferRX, 32))
	{
		printf("SPI DATA Transfer failed\r\n");
		return -1;
	}

	return 0;
}
uint8_t playFile(char *fileName)
{

	// Open the file in read mode.
	//	if (!track.open(&root, fileName, O_READ)) return 2;

	int fd_src = open(fileName, O_RDONLY);
	printf("playFile%s \r\n", fileName);
	if (fd_src < 0)
	{

		printf("aw_open file %s failed\r\n", fileName);
		return 2;
	}

	uint8_t *mp3DataBuffer;
	mp3DataBuffer = (uint8_t *)malloc(1024 * 5 * sizeof(uint8_t));
	while (1)
	{

		// gettimeofday(&t1, NULL);
		// printf("read file....");

		int datasize = read(fd_src, mp3DataBuffer, 1024 * 5 * sizeof(uint8_t));
		// gettimeofday(&t2, NULL);
		// printf("    %d ,take %dms", datasize, (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000);

		if (datasize <= 0)
		{ // Go out to SD card and try reading 32 new bytes of the song
			close(fd_src);

			printf("read file    %d,", datasize);
			// vs1053down();
			break;
		}
		if (playBuf(mp3DataBuffer, datasize) < 0)
		{
			break;
		}
	}
	printf("closeFile%s \r\n", fileName);
	free(mp3DataBuffer);
	close(fd_src);
	return 0;
}
int isSelectFile = 0;
void *thread1(void *arg)
{
	printf("enter thread1\n");

	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir("/data")) == NULL)
	{
		perror("Open dir error...");
		printf("exit thread1\n");
		pthread_exit(0);
	}
	int count = 0;
	char *playlist[100];
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) /// current dir OR parrent dir
			continue;
		else if (ptr->d_type == 8) /// file
		{
			char *format = "%s/%s";
			char *path = (char *)malloc(sizeof(char) * 500);
			;
			int requestLen = sprintf(path, format,
									 "/data",
									 ptr->d_name);
			// if (endswith(path, ".mp3"))
			// {
			printf("%d : %s\n", count, path);
			playlist[count] = path;
			count++;
			// playFile(path);
			// }
		}

		else if (ptr->d_type == 10) /// link file
			printf("d_name:%s/%s\n", "/data", ptr->d_name);
	}

	closedir(dir);
	int opt = 0;

	char scan_string[100] = {0};

	printf("\nPlease enter file num ! 99 for exit\n");
	printf("num > ");

	memset(scan_string, 0, sizeof(scan_string));
	if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
	{
		printf("exit thread1\n");
		pthread_exit(0);
	}
	opt = atoi(scan_string);
	if (opt == 99)
	{
		playing = 0;

		printf("exit thread1\n");
		pthread_exit(0);
	}
	else if (opt >= count)
	{
		isSelectFile = 1;
		while (!isDown)
		{
			printf("\n play %s ", playlist[count - 1]);
			playFile(playlist[count - 1]);
		}
	}
	else
	{
		isSelectFile = 1;
		while (!isDown)
		{
			printf("\n play %s ", playlist[opt]);

			playFile(playlist[opt]);
		}
	}

	printf("exit thread1\n");
	pthread_exit(0);
}
void test1053()
{
	printf("test 1053\r\n");
	int opt = 0;
	isSelectFile = 0;
	char scan_string[100] = {0};
	while (1)
	{
		printf("\nPlease enter ear speaker mode(0,1,2,3)\n");
		printf("mode > ");

		memset(scan_string, 0, sizeof(scan_string));
		if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
		{
			continue;
		}
		opt = atoi(scan_string);
		printf("%d\n", opt);
		if (opt == 0)
		{
			SM_EARSPEAKER_HI = 0x00;
			SM_EARSPEAKER_LO = 0x00;
		}
		if (opt == 1)
		{
			SM_EARSPEAKER_HI = 0x00;
			SM_EARSPEAKER_LO = 0x10;
		}
		if (opt == 2)
		{
			SM_EARSPEAKER_HI = 0x80;
			SM_EARSPEAKER_LO = 0x00;
		}
		if (opt == 3)
		{
			SM_EARSPEAKER_HI = 0x80;
			SM_EARSPEAKER_LO = 0x10;
		}
		break;
	}
	while (1)
	{
		printf("\nPlease enter bass treble mode(0,1,2,3)\n");
		printf("mode > ");

		memset(scan_string, 0, sizeof(scan_string));
		if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
		{
			continue;
		}
		opt = atoi(scan_string);
		printf("%d\n", opt);
		if (opt == 0)
		{
			ST_AMP = 0x00;
			SB_AMP = 0x00;
		}
		if (opt == 1)
		{
			ST_AMP = 0x00;
			SB_AMP = 0x3A;
		}
		if (opt == 2)
		{
			ST_AMP = 0x7A;
			SB_AMP = 0x00;
		}
		if (opt == 3)
		{
			ST_AMP = 0x7A;
			SB_AMP = 0x3A;
		}
		break;
	}

	while (1)
	{
		printf("\nPlease enter clock\n");
		printf("clock > ");

		memset(scan_string, 0, sizeof(scan_string));
		if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
		{
			continue;
		}
		opt = atoi(scan_string);
		printf("%d\n", opt);
		SC_FREQ = opt;
		break;
	}

	while (1)
	{
		printf("\nPlease enter vol\n");
		printf("vol > ");

		memset(scan_string, 0, sizeof(scan_string));
		if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
		{
			continue;
		}
		opt = atoi(scan_string);
		printf("%d\n", opt);
		vs1053Init(opt);
		break;
	}

	activeAMP();

	pthread_t tid1;
	int rc1 = 0;
	rc1 = pthread_create(&tid1, NULL, thread1, NULL);

	while (!isSelectFile)
	{
		// printf("\nwaiting play start!!!!\n");
		sleep(1);
	}
	// sleep(5);

	while (1)
	{
		printf("\nPlease enter raw vol 0-254 , \n999:exit \n300:open aux \n400:close aux\n500:activeAMP\n600:disAMP\n700:disWifi\n800:disGNNS\n");
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

			break;
		}
		else if (opt == 900)
		{
			printf("900 vs1053down\n");
			vs1053down();
		}

		else if (opt == 300)
		{
			activeAUX();
		}
		else if (opt == 400)
		{
			disableAUX();
		}
		else if (opt == 500)
		{
			activeAMP();
		}
		else if (opt == 600)
		{
			disableAMP();
		}
		else if (opt == 700)
		{
			disableWIFI();
		}
		else if (opt == 800)
		{
			disableGNNS();
		}
		else
			vs1053SetVol(opt);
	}
}
