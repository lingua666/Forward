#include "Util.h"
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
#include <stdarg.h>

#include <dirent.h>

#include "SSP.h"
#include "PIN_DEF.h"

void initGpio(int n)
{
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0)
	{
		printf("%s open filed", "/sys/class/gpio/export");
		return;
	}
	FILE *fp = fdopen(fd, "w");
	if (fp == NULL)
		printf("export %d open filed\n", n);
	else
	{
		fprintf(fp, "%d", n);
		fclose(fp);
	}
}
void deInitGpio(int n)
{
	int fd = open("/sys/class/gpio/unexport", O_WRONLY);
	FILE *fp = fdopen(fd, "w");
	if (fp == NULL)
		perror("export open filed\n");
	else
	{
		fprintf(fp, "%d", n);
		fclose(fp);
	}
}
void setGpioDirection(int n, char *direction)
{
	char path[100] = {0};
	sprintf(path, "/sys/class/gpio/gpio%d/direction", n);
	int fd = open(path, O_WRONLY);
	if (fd < 0)
	{
		printf("%s open filed\n", path);
		return;
	}
	FILE *fp = fdopen(fd, "w");
	if (fp == NULL)
		perror("direction open filed\n");
	else
	{
		fprintf(fp, "%s", direction);
		fclose(fp);
	}
}
void setGpioEdge(int n, char *edge)
{
	char path[100] = {0};
	sprintf(path, "/sys/class/gpio/gpio%d/edge", n);
	int fd = open(path, O_WRONLY);
	FILE *fp = fdopen(fd, "w");
	if (fp == NULL)
		perror("setGpioEdge open filed\n");
	else
	{
		fprintf(fp, "%s", edge);
		fclose(fp);
	}
}
int setGpioValue(int n, int val)
{
	char path[64];
	char value_str[3];
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", n);
	int fd = open(path, O_WRONLY);
	FILE *fp = fdopen(fd, "w");
	if (fp == NULL)
		perror("ailed to open gpio value file\n");
	else
	{
		fprintf(fp, "%d", val);
		fclose(fp);
	}
}

int getGpioValue(int n)
{
	char path[64];
	char value_str[3];
	int fd;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", n);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open gpio value for reading!\n");
		return -1;
	}

	if (read(fd, value_str, 3) < 0)
	{
		perror("Failed to read value!");
		return -1;
	}

	close(fd);
	return (atoi(value_str));
}

int file_w_string(const char *path, char *str)
{
	int fd;
	int vlen = 0;
	fd = open(path, O_WRONLY);
	if (fd < 0)
	{
		printf("%s,error %d\n", path, fd);
		return;
	}
	ssize_t s = write(fd, str, strlen(str));
	close(fd);

	return s;
}
int led_setblink(const char *led, int mode, int light_t)
{
	int fd;
	char ledname[50] = {0};
	char ledlight_t[10] = {0};
	if (mode == 1)
	{
		sprintf(ledname, "/sys/class/leds/%s/delay_on", led);
	}
	else
	{
		sprintf(ledname, "/sys/class/leds/%s/delay_off", led);
	}
	sprintf(ledlight_t, "%d", light_t);
	int s = file_w_string(ledname, ledlight_t);
	return s;
}

int endswith(const char *str, const char *p)
{
	int len1, len2;
	len1 = strlen(str);
	len2 = strlen(p);

	if (len2 <= 0)
		return 0;

	if (strncmp(str + len1 - len2, p, len2) == 0)
		return 1;

	return 0;
}

void activeAMP()
{

	initGpio(AMP_EN);
	setGpioDirection(AMP_EN, "out");
	setGpioValue(AMP_EN, 0);
	deInitGpio(AMP_EN);

	usleep(50000);

	initGpio(AMP_MUTE);
	setGpioDirection(AMP_MUTE, "out");
	setGpioValue(AMP_MUTE, 1);
	deInitGpio(AMP_MUTE);
}
void disableAMP()
{

	initGpio(AMP_MUTE);
	setGpioDirection(AMP_MUTE, "out");
	setGpioValue(AMP_MUTE, 0);
	deInitGpio(AMP_MUTE);

	usleep(50000);

	initGpio(AMP_EN);
	setGpioDirection(AMP_EN, "out");
	setGpioValue(AMP_EN, 1);
	deInitGpio(AMP_EN);
}

void activeAUX()
{
	initGpio(PIN_AUX_CTRL);
	setGpioDirection(PIN_AUX_CTRL, "out");

	setGpioValue(PIN_AUX_CTRL, 0);
	deInitGpio(PIN_AUX_CTRL);
}
void disableAUX()
{
	initGpio(PIN_AUX_CTRL);
	setGpioDirection(PIN_AUX_CTRL, "out");

	setGpioValue(PIN_AUX_CTRL, 1);
	deInitGpio(PIN_AUX_CTRL);
}

void activeGNNS()
{
	printf("activeGNNS....\n");

	initGpio(PIN_GNSS_SW);
	setGpioDirection(PIN_GNSS_SW, "out");

	setGpioValue(PIN_GNSS_SW, 1);
	deInitGpio(PIN_GNSS_SW);
	// sleep(2);
	printf("activeGNNS !!!!\n");
}

void disableGNNS()
{
	printf("disableGNNS  ....\n");
	initGpio(PIN_GNSS_SW);
	setGpioDirection(PIN_GNSS_SW, "out");

	setGpioValue(PIN_GNSS_SW, 0);
	deInitGpio(PIN_GNSS_SW);
	// sleep(2);
	printf("disableGNNS   !!!!\n");
}

int checkWifiMod()
{
	int fd = open("/lib/modules/3.10.33/rtl8189es.ko", O_RDONLY);
	if (fd < 0)
	{
		printf("not rtl8189es\n");
	}
	else
	{
		close(fd);
		return 0;
	}
	fd = open("/lib/modules/3.10.33/rtl8192fs.ko", O_RDONLY);
	if (fd < 0)
	{
		printf("not rtl8192fs\n");
	}
	else
	{
		close(fd);
		return 1;
	}
	fd = open("/lib/modules/3.10.33/ssv6x5x.ko", O_RDONLY);
	if (fd < 0)
	{
		printf("not ssv6x5x\n");
	}
	else
	{
		close(fd);
		return 2;
	}
	return -1;
}

void activeWIFI()
{

	system("echo 1 > /sys/devices/asr-rfkill.0/pwr_ctrl");
	int wifiType = checkWifiMod();
	if (wifiType == 0)
	{
		system("insmod /lib/modules/3.10.33/rtl8189es.ko");
	}
	else if (wifiType == 1)
	{
		system("insmod /lib/modules/3.10.33/rtl8192fs.ko");
	}
	else if (wifiType == 2)
	{
		system("insmod /lib/modules/3.10.33/ssv6x5x.ko");
	}
	system("/sbin/wifi");
}

void disableWIFI()
{
	printf("disableWIFI...\n");
	int wifiType = checkWifiMod();
	if (wifiType == 0)
	{
		system("rmmod rtl8189es.ko");
	}
	else if (wifiType == 1)
	{
		system("rmmod rtl8192fs.ko");
	}
	else if (wifiType == 2)
	{
		system("rmmod ssv6x5x.ko");
	}
	system("echo 0 > /sys/devices/asr-rfkill.0/pwr_ctrl");
}