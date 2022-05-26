
#include <libThirdWrap_RtmpClient/External.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "librtmp_send264.h"
#include "simplest_librtmp_sendflv.h"


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

int main(int argc, char* argv[])
{
	FlvTest();

	/*fp_send1 = fopen("cuc_ieschool.h264", "rb");

	//初始化并连接到服务器
	RTMP264_Connect("rtmp://localhost/publishlive/livestream");*/

	fp_send1 = fopen("C:\\Users\\2020\\Desktop\\智慧垃圾亭系统设计\\监控\\rtmp\\simplest_librtmp_example\\资源\\cuc_ieschool.h264", "rb");

	//初始化并连接到服务器
	RTMP264_Connect("rtmp://192.168.1.188:1935/live/livestream");

	//发送
	RTMP264_Send(read_buffer1);

	//断开连接并释放相关资源
	RTMP264_Close();

	getchar();
	return 0;
}




