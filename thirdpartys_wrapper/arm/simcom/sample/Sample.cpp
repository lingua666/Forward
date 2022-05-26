#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <nkboard.h>

#ifdef __cplusplus
}
#endif


int main()  
{
	//应用灯
   int iRet = SetLed(1, 500, 500);
   if (iRet != 0)
   {
	   printf("SetLed() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   //告警灯
   iRet = SetLed(2, 500, 300);
   if (iRet != 0)
   {
	   printf("SetLed() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   /*char szIMEI[100] = { 0 };
   iRet = GetIMEI(szIMEI, sizeof(szIMEI));
   if (iRet <= 0)
   {
	   printf("GetIMEI() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("GetIMEI() IMEI:%s\r\n", szIMEI);*/

   char szCCID[100] = { 0 };
   iRet = GetICCID(szCCID, sizeof(szCCID));
   if (iRet < 0)
   {
	   printf("GetICCID() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("4G_GetICCID() CCID:%s\r\n", szCCID);

   int iSignal = 0;
   iRet = GetSignal(&iSignal);
   if (iRet != 0)
   {
	   printf("GetSignal() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("GetSignal() Signal:%d\r\n", iSignal);

   int64_t _time = 0;
   //Ntp_time("cn.ntp.org.cn", 123, &_time);
   iRet = Ntp_time("time.nist.gov", 123, &_time);//time.nist.gov
   if (iRet != 0)
   {
	   printf("Ntp_time() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("Ntp_time() _time:%lld\r\n", _time);

   //播放音频
   OpenAMP();
   iRet = Audio_Open(50);
   if (iRet != 0)
   {
	   printf("Audio_Open() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   FILE* fp = fopen("/data/test.mp3", "rb");
   if (fp == NULL)
   {
	   printf("Not Found MP3!\r\n");
	   return -1;
   }

   char szBuffer[1024 * 10] = {0};

   do 
   {
	   int iLen = fread(szBuffer, 1, sizeof(szBuffer), fp);
	   if (iLen <= 0)
		   break;

	   printf("Playing!\r\n");

	   int iRet = Audio_Play((UInt8*)szBuffer, iLen);
	   if (iRet != 0)
	   {
		   printf("Audio_Play() Failed, error:%d!\r\n", iRet);
		   return -1;
	   }
   } while (true);

   CloseAMP();
   Audio_Close();

   //打开功放
   /*OpenAMP();
   iRet = FM_Open(924 * 100, 2);
   if (iRet != 0)
   {
	   printf("FM_Open() Failed！！, error:%d", iRet);
	   return -1;
   }

   sleep(30);
   iRet = FM_Close();
   if (iRet != 0)
   {
	   printf("FM_Close() Failed！！, error:%d", iRet);
	   return -1;
   }

   CloseAMP();*/

   iRet = SwitchEthMode(0);
   if (iRet != 0)
   {
	   printf("SwitchEthMode() Failed！！, error:%d", iRet);
	   return -1;
   }

   int Vol;
   iRet = Get_ADC(&Vol);
   if (iRet != 0)
   {
	   printf("Get_ADC() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("Get_ADC() Vol:%d\r\n", Vol);

   double lat, lon;
   int64_t ttt;
   iRet = GPS_loc(&lat, &lon, &ttt);
   if (iRet != 0)
   {
	   printf("GPS_loc() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("GPS_loc:%f , %f ,%lld  \n", lat, lon, ttt);

   sleep(10);

   iRet = GPS_loc(&lat, &lon, &ttt);
   if (iRet != 1)
   {
	   printf("GPS_loc() Failed, error:%d!\r\n", iRet);
	   return -1;
   }

   printf("GPS_loc:%f , %f ,%lld  \n", lat, lon, ttt);

    getchar();
    return 0;  
}
