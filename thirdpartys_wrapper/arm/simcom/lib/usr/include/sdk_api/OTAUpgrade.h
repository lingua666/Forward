#ifndef _OAT_UPGRADE_H_
#define _OAT_UPGRADE_H_

#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <syscall.h>
#include <sys/ioctl.h>
#include <linux/reboot.h>
#include <libubox/blob.h>
#include <libubox/blobmsg.h>
#include <libubox/ustream.h>
#include <libubus.h>


/*****************************************************************************
* Function Name : upgrade_through_sd
* Description   : SD方式OTA升级
* Input         : size 升级包大小         url 升级包存放的路径
* Output        : None
* Return        : 0: success     其他: fail
* Auther        : zhurui
* Date          : 2022.01.11
*****************************************************************************/
int upgrade_through_sd(char *url);

#endif
