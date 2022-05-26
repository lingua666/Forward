#include "nkboard.h"
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
#include "ATControl.h"
#include "ADC.h"
#include "simcom_loc.h"
#include "SSP.h"
#include "DEVControl.h"
#include "Util.h"
#include "vs1053.h"
#include "RDA5807M.h"

/*****************************************************************
/*原型声明：	设置LED状态
/*参数说明：	Index: LED索引  ,  0:netLED   1:statusLED   2:alarmLED
				OpenInterval:  打开时长 0: 不使能  >0:时长   单位/毫秒
				CloseInterval:  关闭时长 0: 不使能  >0:时长   单位/毫秒
status:
上线：亮1500ms，关500ms
离线：亮500ms，关1500ms
播放：亮500ms，光500ms

alarm:
报警灯
慢闪：1500/1000
中闪：1000/500
快闪：500/300
关闭：0/500
常亮 500/0
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int SetLed(int Index, int OpenInterval, int CloseInterval)
{
	char *ledname = LED_NET;

	if (Index == 1)
	{
		ledname = LED_STATUS;
	}
	else if (Index == 2)
	{
		ledname = LED_ALARM;
	}

	char ledpath[50] = {0};
	sprintf(ledpath, "/sys/class/leds/%s/trigger", ledname);

	if (OpenInterval == 0)
	{
		int a = file_w_string(ledpath, "none");
		if (a > 0)
		{
			return 0;
		}
	}
	else if (CloseInterval == 0)
	{
		int a = file_w_string(ledpath, "default-on");
		if (a > 0)
		{
			return 0;
		}
	}
	else if (file_w_string(ledpath, "timer") > 0)
	{

		int a = led_setblink(ledname, 1, OpenInterval);
		int b = led_setblink(ledname, 0, CloseInterval);
		if (a > 0 && b > 0)
		{
			return 0;
		}
	};
	return -1;
}

/*****************************************************************
/*原型声明：	获取4G模块IMEI
/*参数说明：	szOutIMEI:  接收IMEI内容缓冲区
/*				Size: 接收缓冲区长度
/*返回值：		>0: 实际IMEI内容长度  <0: 错误码
*****************************************************************/
char *_imei;
int _imei_len;
int GetIMEI(char *szOutIMEI, int Size)
{

	if (_imei != NULL)
	{
		strcpy(szOutIMEI, _imei);
		return _imei_len;
	}

	int ret = atctrl_init();
	if (ret != 0)
	{
		printf("atctrl_init failed %d\n", ret);
		return -1;
	}
	ret = getIMEI(szOutIMEI, Size);
	if (ret == 0)
	{
		ret = strlen(szOutIMEI);
		printf("GetIMEI %d:::%s\n", ret, szOutIMEI);
		_imei = (char *)malloc((ret + 1) * sizeof(char));
		strcpy(_imei, szOutIMEI);
		_imei_len = ret;
	}
	else
	{
		printf("GetIMEI failed %d\n", ret);
	}
	atctrl_deinit();
	return ret;
}
/*****************************************************************
/*原型声明：	获取4G模块ICCID
/*参数说明：	szOutICCID:  接收ICCID内容缓冲区
/*				Size: 接收缓冲区长度
/*返回值：		>0: 实际IMEI内容长度  <0: 错误码
*****************************************************************/
char *_iccid;
int _iccid_len;
int GetICCID(char *szOutICCID, int Size)
{
	if (_iccid != NULL)
	{
		strcpy(szOutICCID, _iccid);
		return _iccid_len;
	}
	int ret = atctrl_init();
	if (ret != 0)
	{
		printf("at init failed \n");
		return -1;
	}
	ret = getICCID(szOutICCID, Size);
	int count = 0;
	while (count < 5 && ret != 0)
	{
		ret = getICCID(szOutICCID, Size);
		count++;
	}

	if (ret == 0)
	{
		ret = strlen(szOutICCID);
		printf("get iccid %d:::%s\n", ret, szOutICCID);
		_iccid = (char *)malloc((ret + 1) * sizeof(char));

		strcpy(_iccid, szOutICCID);
		_iccid_len = ret;
	}
	else
	{
		printf("get iccid failed %d\n", ret);
	}
	atctrl_deinit();
	return ret;
}
/*****************************************************************
/*原型声明：	获取4G模块IMEI
/*参数说明：	pOutSignal: 接收4G模块信号强度数值
/*返回值：		0: 成功  <0: 错误码
*****************************************************************/
int GetSignal(int *pOutSignal)
{
	int ret = atctrl_init();
	if (ret != 0)
	{
		return -1;
	}
	uint8_t rssi;
	getCSQ(&rssi);
	printf("getCSQ: %d\n", rssi);
	*pOutSignal = rssi;
	atctrl_deinit();
	return 0;
}

/*****************************************************************
/*原型声明：	打开音频
/*参数说明：	Volume: 音量
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int currentVolume = 0;

int Audio_Open(int Volume)
{
	currentVolume = Volume;
	int ret = vs1053Init(Volume);
	return ret;
}

/*****************************************************************
/*原型声明：	播放音频
/*参数说明：	pAudioData: 音频数据
				DataSize: 数据长度
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int Audio_Play(uint8_t *pAudioData, int DataSize)
{
	return playBuf(pAudioData, DataSize);
}

/*****************************************************************
/*原型声明：	关闭音频
/*参数说明：
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int Audio_Close(void)
{

	vs1053down();
	return 0;
}

/*****************************************************************
/*原型声明：	打开收音机
/*参数说明：	ch：FM频道*1000，(例子：FM92.4MHz，ch=92400)
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int FM_Open(int ch, int Volume)
{

	openRadio();
	mute(0);
	vol(Volume);
	chanle(ch);
	return 0;
}

/*****************************************************************
/*原型声明：	关闭收音机
/*参数说明：
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int FM_Close(void)
{

	closeRadio();
	return 0;
}

/*****************************************************************
/*原型声明：	对时
/*参数说明：	 ip:ntp服务器地址，port，端口
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int Ntp_time(const char *ip, int port, int64_t *_time)
{

	char command[256];

	memset(command, 0, sizeof(command));
	sprintf(command, "%s %s:%d", "ntpd -d -n -N -q -p ", ip, port);
	int ret = system(command);
	printf("ntp return %d", ret);
	struct timeval ttt;

	gettimeofday(&ttt, NULL);
	*_time = (int64_t)ttt.tv_sec * 1000 + ttt.tv_usec / 1000;
	if (ret != 0)
		return -1;
	return 0;
}

double gpslon = 0;
double gpslat = 0;
int64_t gpstime = 0;
int gpsStart = 0;
int isStop = 0;
void callback_simcom_event_loc(simcom_event_e event_id, void *data)
{
	if (isStop)
	{
		printf("\n callback_simcom_event_loc already stop\n");
		return;
	}
	printf("\n callback_simcom_event_loc:%d\n", event_id);
	switch (event_id)
	{
	case SIMCOM_EVENT_LOC_INFO_IND:
	{
		simcom_loc_info *info = (simcom_loc_info *)data;
		printf("\n new LATITUDE:%f  LONGITUDE: %f  ALTITUDE: %f  SPEED: %f  TIME:%jd,  %02d-%02d-%02d %02d:%02d:%02d\n",
			   info->latitude, info->longitude, info->altitude, info->speed, info->timestamp, info->time[0], info->time[1], info->time[2], info->time[3], info->time[4], info->time[5]);

		if (gpstime == 0 || gpslat == 0.0 || gpslon == 0.0)
		{

			gpslat = info->latitude;
			gpslon = info->longitude;
			gpstime = info->timestamp;
		}
	}
	break;
	case SIMCOM_EVENT_LOC_NMEA_IND:
		printf("%s", (char *)data);
		break;
	default:
		break;
	}
}

int stopGNNS()
{
	// isStop = 1;
	// printf("stopGNNS.....\n");
	// int ret = simcom_loc_poweroff();

	// printf("simcom_loc_poweroff.....%d\n", ret);
	// if (ret == 0)
	// {
	// 	sleep(1);
	// 	disableGNNS();
	// 	simcom_loc_deinit();
	// 	printf("stopGNNS!!!!\n");
	// }
	// else
	// {
	// 	printf("simcom_loc_poweroff failed.....%d\n", ret);
	// }

	isStop = 1;
	printf("stopGNNS.....\n");
	int ret = simcom_loc_stop();

	printf("simcom_loc_stop.....%d\n", ret);
	// if (ret == 0)
	// {
	// 	sleep(1);
	// 	disableGNNS();
	// 	simcom_loc_deinit();
	// 	printf("stopGNNS!!!!\n");
	// }
	// else
	// {
	// 	printf("simcom_loc_poweroff failed.....%d\n", ret);
	// }
	sleep(1);
	disableGNNS();
	simcom_loc_deinit();
	printf("stopGNNS!!!!\n");
}

/*****************************************************************
/*原型声明：	获取定位
/*参数说明：	 lat：纬度，lon：精度
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int GPS_loc(double *lat, double *lon, int64_t *time, int maxTry)
{
	if (maxTry < 90)
	{
		maxTry = 90;
	}

	activeGNNS();
	sleep(1);

	int pine_p[2];
	int pine_c[2];
	pid_t pid;

	int ret = pipe(pine_c);
	if (ret == -1)
	{
		perror("fd_parent error:");
		return -1;
	}
	ret = pipe(pine_p);
	if (ret == -1)
	{
		perror("fd_child error:");
		return -1;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork error:");
		return -1;
	}
	else if (pid == 0)
	{
		close(pine_p[1]);
		close(pine_c[0]);
		printf("child maxT:%d\n", maxTry);
		isStop = 0;
		gpslat = 0.0;
		gpslon = 0.0;
		gpstime = 0;

		double _lat = 0;
		double _lon = 0;
		int64_t _gpstime = 0;
		if (simcom_loc_init(callback_simcom_event_loc) != EXIT_SUCCESS)
		{
			printf("simcom loc init failed\n");
			stopGNNS();

			return -1;
		}
		else
		{
			printf("simcom loc init \n");
		}

		if (simcom_loc_track_start() != EXIT_SUCCESS)
		{
			printf("Location track Failed!\n");
			stopGNNS();

			return -1;
		}
		else
		{
			printf("simcom_loc_track_start \n");
		}

		char buf[1024]; //读数据的缓冲区
		char w_buf[256];

		memset(w_buf, 0, sizeof(w_buf));
		w_buf[0] = 0x00;
		while (1)
		{

			int rea = read(pine_p[0], buf, sizeof(buf));
			if (rea > 0 && buf[0] == 0x01)
			{
				if (gpstime == 0 || gpslat == 0.0 || gpslon == 0.0)
				{
					// int w = write(pine_c[1], w_buf, 1);
					// printf("child write: 0x0,%d\n", w);
				}
				else
				{

					_lat = gpslat;
					_lon = gpslon;
					_gpstime = gpstime;

					printf("child: LATITUDE:%f  LONGITUDE: %f    TIME:%jd, \n", _lat, _lon, _gpstime);

					break;
				}
			}
			else
			{

				printf("child: no more\n");
				break;
			}
		}

		close(pine_p[0]);
		stopGNNS();

		int doubleLen = sizeof(double);
		int int64_tLen = sizeof(int64_t);

		memcpy(w_buf, (char *)&_lat, doubleLen);
		memcpy(w_buf + doubleLen, (char *)&_lon, doubleLen);
		memcpy(w_buf + doubleLen + doubleLen, (char *)&_gpstime, int64_tLen);

	 

		ret=write(pine_c[1], w_buf, doubleLen * 2 + int64_tLen); // hello pipe
		printf("child write loc %d\nS",ret);
		//sleep(5);
		close(pine_c[1]);

		exit(1);
	}
	else
	{
		close(pine_c[1]);
		close(pine_p[0]);
		gpsStart = 1;
		int count = 0;
		int ret = 0;
		char w_buf[1];
		w_buf[0] = 0x01;
		while (gpsStart && count < maxTry)
		{
			ret = write(pine_p[1], w_buf, 1);
			if (ret < 0)
			{
				printf("parent write:error %d\n", ret);
				break;
			}

			count += 1;
			sleep(1);
		}
		close(pine_p[1]);

		int doubleLen = sizeof(double);
		int int64_tLen = sizeof(int64_t);

		char buf[doubleLen * 2 + int64_tLen]; //读数据的缓冲区

		int rea = read(pine_c[0], buf, sizeof(buf));
		int gpsCode = 0;
		if (rea == 0)
		{
			perror("read finish\n");
			gpsCode = -1;
		}
		else
		{
			printf("read loc : %d\n", rea);
			// char latbuf[doubleLen];
			// memcpy(latbuf,buf,doubleLen);
			// *lat = *((double *)latbuf);

			// char lonbuf[doubleLen];
			// memcpy(lonbuf,buf+doubleLen,doubleLen);
			// *lon = *((double *)lonbuf);

			// char timebuf[int64_tLen];
			// memcpy(timebuf,buf+doubleLen+doubleLen,int64_tLen);
			// *time = *((int64_t *)timebuf);

			memcpy(lat, buf, doubleLen);
			memcpy(lon, buf + doubleLen, doubleLen);
			memcpy(time, buf + doubleLen + doubleLen, int64_tLen);

			if (*lat == 0 || *lon == 0 || *time == 0)
			{
				gpsCode = -1;
			}
			printf("parent: LATITUDE:%f  LONGITUDE: %f    TIME:%jd, \n", *lat, *lon, *time);
		}

		close(pine_c[0]);
		return gpsCode;
	}
}
int GPS_stop()
{
	gpsStart = 0;
	// stopGNNS();
	return 0;
}
/*****************************************************************
/*原型声明：	切换以太网模式网络
/*参数说明：	  0 wan ，1 lan
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int SwitchEthMode(int mode)
{

	if (mode == 0)
	{

		system("uci set network.lan.ifname=\"usbnet0 hsicnet0\"");
		system("uci set network.lan.ipaddr='192.168.254.1'");
		system("uci set network.autowan=\"interface\"");
		system("uci set network.autowan.ifname=\"eth0\"");
		system("uci set network.autowan.proto=\"dhcp\"");
		system("uci set network.autowan.force_link=\"1\"");
		system("uci commit network");
		system("/etc/init.d/network restart");
	}
	else
	{
		system("uci set network.lan.ifname=\"usbnet0 hsicnet0 eth0\"");
		system("uci set network.lan.ipaddr='192.168.254.1'");
		system("uci delete network.autowan");
		system("uci commit network");
		system("/etc/init.d/network restart");
	}
	return 0;
}

/*****************************************************************
/*原型声明：	切换wifi模式
/*参数说明：	0：sta，  1：ap ，-1：关闭
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int SwitchWifiMode(int mode, char *ssid, char *pass)
{

	if (mode == -1)
	{
		disableWIFI();
	}
	else if (mode == 0)
	{
		activeWIFI();
		system("uci set dhcp.@dnsmasq[0].rebind_protection='0'");

		system("uci set network.wwan=interface");
		system("uci set network.wwan.proto='dhcp'");

		system("uci add_list firewall.@zone[1].network='wwan'");

		char command[50];

		system("uci set wireless.AP0_2G_if.network='wwan'");
		system("uci set wireless.AP0_2G_if.mode='sta'");
		memset(command, 0, 50);
		sprintf(command, "uci set wireless.AP0_2G_if.ssid='%s'", ssid);
		system(command);
		memset(command, 0, 50);
		sprintf(command, "uci set wireless.AP0_2G_if.key='%s'", pass);
		system(command);

		system("uci commit");
		system("reboot");
	}
	else if (mode == 1)
	{

		char command[50];

		system("uci set wireless.AP0_2G_if.network='lan'");
		system("uci set wireless.AP0_2G_if.mode='ap'");
		memset(command, 0, 50);
		sprintf(command, "uci set wireless.AP0_2G_if.ssid='%s'", ssid);
		system(command);
		memset(command, 0, 50);
		sprintf(command, "uci set wireless.AP0_2G_if.key='%s'", pass);
		system(command);

		system("uci commit");
		system("reboot");
	}
}

// adc 校准
int voltageArray[32] = {787, 793, 799, 805, 811, 817, 823, 832, 835, 841, 847, 853, 859, 900, 906, 912, 918, 924, 930, 936, 942, 951, 954, 960, 969, 972, 978, 984, 993, 999, 1005, 1011};
int voltage_add_Array[32] = {28, 29, 31, 32, 33, 35, 36, 34, 39, 40, 42, 43, 45, 11, 13, 14, 15, 16, 18, 19, 21, 19, 23, 25, 23, 28, 29, 30, 29, 31, 33, 34};
/*****************************************************************
/*原型声明：	获取电量（电压）
/*参数说明：	  voltage 电压值，单位mv
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int Get_ADC(int *voltage)
{

	int temp;

	int mVal = get_adc_voltage(&temp);

	if (mVal < 0)
	{
		printf("ADC READ Failed\r\n");
		return -1;
	}
	else
	{
		temp = temp + 28;
		int i;
		for (i = 0; i < 32; i = i + 1)
		{
			if (temp <= voltageArray[i])
			{

				temp = voltage_add_Array[i] + temp;

				break;
			}
		}

		*voltage = temp * 1102 / 82;

		return 0;
	}
}

/*****************************************************************
/*原型声明：	获取ip地址
/*参数说明：	ip，掩码，网关
/*
/*返回值：		1: 成功，  <0: 错误码
*****************************************************************/
int Get_IP(char *ip, char *mask, char *gate);

/*****************************************************************
/*原型声明：	设置ip
/*参数说明：	ip，掩码，网关（如使用dhcp，三个参数传null）
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int Set_IP(char *ip, char *mask, char *gate);

/*****************************************************************
/*原型声明：	开关adb
/*参数说明：	 status: 0 关闭，1开启
/*
/*返回值：		0: 成功，  <0: 错误码
*****************************************************************/
int Switch_ADB(int status)
{

	//return exec_cusbadb_cmd(status);
	return 0;
}

/*****************************************************************
/*原型声明：	 打开功放
/*参数说明：
/*
/*返回值：
*****************************************************************/
void OpenAMP(void)
{
	activeAMP();
}

/*****************************************************************
/*原型声明：	 关闭功放
/*参数说明：
/*
/*返回值：
*****************************************************************/
void CloseAMP(void)
{
	disableAMP();
}

/*****************************************************************
/*原型声明：	 打开音频外部输入
/*参数说明：
/*
/*返回值：
*****************************************************************/
void OpenAUX(void)
{
	activeAUX();
}

/*****************************************************************
/*原型声明：	 关闭音频外部输入
/*参数说明：
/*
/*返回值：
*****************************************************************/
void CloseAUX(void)
{
	disableAUX();
}

/*****************************************************************
/*原型声明：	 ota升级
/*参数说明：	 size: bin镜像大小（字节） path：bin文件路径
/*
/*返回值：
*****************************************************************/
void OTAUpdate(int64_t size, char *path)
{

	char command[500];

	memset(command, 0, 500);
	sprintf(command, "ubus call ota download'{ \"type\":2,\"size\":%d,\"url\":\"%s\",\"username\":\"\",\"password\":\"\",\"sync\":1 }'", size, path);
	system(command);
}

/*****************************************************************
/*原型声明：	 获取IO值
/*参数说明：	 Pin: 引脚位置
/*			 Direct: 引脚反向 0: 输入    1:输出
/*
/*返回值：	
*****************************************************************/
int GetGpio(int Pin, int Direct)
{
	int iRet = 0;
	initGpio(Pin);
	setGpioDirection(Pin, Direct == 0 ? "in" : "out");
	iRet = getGpioValue(Pin);
	deInitGpio(iRet);
	return iRet;
}


void ledtest()
{
	SetLed(0, 0, 2000);
	SetLed(1, 200, 0);

	char imei[100] = {0};
	memset(imei, 0, sizeof(imei));

	GetIMEI(imei, 100);
	printf("imei:%s  \n", imei);
	memset(imei, 0, sizeof(imei));

	GetIMEI(imei, 100);
	printf("imei:%s  \n", imei);
	memset(imei, 0, sizeof(imei));

	GetIMEI(imei, 100);
	printf("imei:%s  \n", imei);

	char iccid[100] = {0};
	memset(iccid, 0, sizeof(iccid));
	GetICCID(iccid, 100);
	int singal = -1;
	GetSignal(&singal);
	printf("imei:%s,iccid:%s,singal:%d\n", imei, iccid, singal);

	int v;
	Get_ADC(&v);
	printf("adv:%dmv\n", v);

	double lat, lon;
	int64_t time;

	while (1)
	{
		GPS_loc(&lat, &lon, &time, 120);
		printf("GPS_loc:%f %f %jd\n", lat, lon, time);
		sleep(10);
	}
}
void mp3test()
{

	OpenAMP();
	Audio_Open(100);

	int fd_src = open("/data/test1.mp3", O_RDONLY);
	uint8_t bufp[1024];
	uint8_t *mp3DataBuffer;
	mp3DataBuffer = (uint8_t *)malloc(1024 * 5 * sizeof(uint8_t));
	struct timeval t1;
	struct timeval t2;
	printf("start....\n");

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
			Audio_Close();
			free(mp3DataBuffer);
			break;
		}
		Audio_Play(mp3DataBuffer, datasize);
	}
}
void InitSys()
{
	printf("disableAMP\n");
	initGpio(AMP_EN);
	initGpio(AMP_MUTE);
	setGpioDirection(AMP_EN, "out");
	setGpioDirection(AMP_MUTE, "out");
	disableAMP();

	printf("setLED\n");
	SetLed(0, 2000, 500);

	disableGNNS();

	printf("tryNtp...\n");

	// int64_t _time = 0;
	// Ntp_time("ntp.aliyun.com", 123, &_time);
	// printf("current time %jd\n", _time);
	// Ntp_time("ntp.aliyun.com", 123, &_time);
	// printf("current time %jd\n", _time);

	disableWIFI();
}