// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <libFrameWork_RtmpCommon/RtmpSession.h>
//#include "librtmp_send264.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <libFrameWork_Rtsp/H264Info.h>
#include <libFrameWork_RtmpCommon/H264Session.h>
#include <libFrameWork_RtmpCommon/FlvSession.h>

#define RTMP_PACKET_TYPE_AUDIO 0x08
#define RTMP_PACKET_TYPE_VIDEO 0x09
#define RTMP_PACKET_TYPE_INFO  0x12

FILE *fp_send1;

//读文件的回调函数
//we use this callback function to read data from buffer
int read_buffer1(unsigned char *buf, int buf_size) {
	if (!feof(fp_send1)) {
		int true_size = fread(buf, 1, buf_size, fp_send1);
		return true_size;
	}
	else {
		return -1;
	}
}

void HandleEvent(_rtmp_::RtmpSession::RTMPStatus Status, const _rtmp_::RtmpSession::self_sptr& Session, void* pParam)
{

}

void HandleEventTest(_rtmp_::IRtmpSession::RTMPStatus Status, const _rtmp_::IRtmpSession::self_sptr& Session, void* pParam)
{

}

void RtmpFlvTest(void)
{
	GetAsynStreamClientsInstance()->Init(0, 0, true);
	_rtmp_::IRtmpSession::self_sptr Rtmp_sptr = _rtmp_::FlvSession::Alloc();

	while (true)
	{
		printf("RtmpFlvTest Start\r\n");
		fp_send1 = fopen("C:\\Users\\2020\\Desktop\\智慧垃圾亭系统设计\\监控\\rtmp\\simplest_librtmp_example\\资源\\cuc_ieschool.h264", "rb");
		if (fp_send1 == NULL)
			continue;

		//fp_send1 = fopen("J:\\个人项目\\库封装\\h264_analysis-master\\h264_analysis-master\\cuc_ieschool.h264", "rb");

		//初始化并连接到服务器
		((_rtmp_::FlvSession*)Rtmp_sptr.get())->Init(HandleEventTest);
		Rtmp_sptr->Connect("rtmp://192.168.1.188:1935/live/livestream");
		Sleep(500);

		unsigned int tick = 0;
		unsigned int tick_gap = 1000 / 15;
		uint32_t now, last_update;

		std::string sData;

		const char szHead[] = { 0x00, 0x00, 0x01 };
		char szBuffer[2] = { 0 };

		tagVideoPlayload_INFO PlayloadInfo;
		H264Info Info;
		int iRet = 0, iOffset = 0;
		do
		{
			iRet = fread(szBuffer, 1, sizeof(szBuffer), fp_send1);
			Rtmp_sptr->SendPacket(szBuffer, iRet);
		} while (iRet > 0);

		if (fp_send1 != NULL)
		{
			fclose(fp_send1);
			fp_send1 = NULL;
		}

		Rtmp_sptr->Close();
	}

	std::cin.get();
}

void RtmpH264Test(void)
{
	GetAsynStreamClientsInstance()->Init(0, 0, true);
	_rtmp_::IRtmpSession::self_sptr Rtmp_sptr = _rtmp_::H264Session::Alloc();

	while (true)
	{
		printf("RtmpH264Test Start\r\n");
		fp_send1 = fopen("C:\\Users\\2020\\Desktop\\智慧垃圾亭系统设计\\监控\\rtmp\\simplest_librtmp_example\\资源\\cuc_ieschool.h264", "rb");
		if (fp_send1 == NULL)
			continue;

		//fp_send1 = fopen("J:\\个人项目\\库封装\\h264_analysis-master\\h264_analysis-master\\cuc_ieschool.h264", "rb");

		//初始化并连接到服务器
		((_rtmp_::H264Session*)Rtmp_sptr.get())->Init(HandleEventTest);
		Rtmp_sptr->Connect("rtmp://192.168.1.188:1935/live/livestream");
		Sleep(500);

		unsigned int tick = 0;
		unsigned int tick_gap = 1000 / 15;
		uint32_t now, last_update;

		std::string sData;

		const char szHead[] = { 0x00, 0x00, 0x01 };
		char szBuffer[2] = { 0 };

		tagVideoPlayload_INFO PlayloadInfo;
		H264Info Info;
		int iRet = 0, iOffset = 0;
		do
		{
			iRet = fread(szBuffer, 1, sizeof(szBuffer), fp_send1);
			Rtmp_sptr->SendPacket(szBuffer, iRet);
		} while (iRet > 0);

		if (fp_send1 != NULL)
		{
			fclose(fp_send1);
			fp_send1 = NULL;
		}

		Rtmp_sptr->Close();
	}

	std::cin.get();
}

int main(int argc, char* argv[])
{
	RtmpFlvTest();

	//RtmpH264Test();

	GetAsynStreamClientsInstance()->Init(0, 0, true);

	while (true)
	{
		printf("main Start\r\n");

		_rtmp_::RtmpSession::self_sptr Rtmp_sptr = _rtmp_::RtmpSession::Alloc();
		fp_send1 = fopen("C:\\Users\\2020\\Desktop\\智慧垃圾亭系统设计\\监控\\rtmp\\simplest_librtmp_example\\资源\\cuc_ieschool.h264", "rb");
		if (fp_send1 == NULL)
			continue;

		//fp_send1 = fopen("J:\\个人项目\\库封装\\h264_analysis-master\\h264_analysis-master\\cuc_ieschool.h264", "rb");

		//初始化并连接到服务器
		Rtmp_sptr->Init(HandleEvent);
		Rtmp_sptr->Connect("rtmp://192.168.1.188:1935/live/livestream");
		Sleep(500);

		unsigned int tick = 0;
		unsigned int tick_gap = 1000 / 15;
		uint32_t now, last_update;

		std::string sData;

		const char szHead[] = { 0x00, 0x00, 0x01 };
		char szBuffer[2] = { 0 };

		tagVideoPlayload_INFO PlayloadInfo;
		H264Info Info;
		int iRet = 0, iOffset = 0;
		do
		{
			iRet = fread(szBuffer, 1, sizeof(szBuffer), fp_send1);
			Rtmp_sptr->SendH264Packet(szBuffer, iRet);
		} while (iRet > 0);
		
		if (fp_send1 != NULL)
		{
			fclose(fp_send1);
			fp_send1 = NULL;
		}

		Rtmp_sptr->Close();
	}


	std::cin.get();
	return 0;
}