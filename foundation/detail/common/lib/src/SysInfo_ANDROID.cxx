
/** @file
* @brief 进程统计信息函数的实现
* @author 张亚霏
* @date 2009/05/03
* @version 0.1
*
* 部分代码来自MSDN的例子
* 部分代码来自google chromium项目
*
* 需要连接到psapi.lib
*/


#include <libCommon/SysInfo.h>
#include <libCommon/os/Types.h>
#include <libCommon/os/Platform.h>
#include <libCommon/os/windowsi.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <libCommon/ifaddrs.h>
#include <linux/if_ether.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#define BUF_SIZE 1024

	/// 获得CPU的核数
	int get_processor_number( void )
	{
		return sysconf(_SC_NPROCESSORS_ONLN);   //GNU fuction  
	}

	int get_physical_processor_number( void )
	{
		return -1;
	}

	/// 获取当前进程的cpu使用率，返回-1失败
	int get_process_cpu_usage( void )
	{//未实现
		return -1;
	}

	/// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功
	int get_process_memory_usage( UInt64_t* mem, UInt64_t* vmem )
	{//未实现
		return -1;
	}

	/// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功
	int get_process_io_bytes( UInt64_t* read_bytes, UInt64_t* write_bytes )
	{//未实现
		return -1;
	}

	//获取内存信息
	void get_memory_info( UInt64_t* total, UInt64_t* free )
	{

	}

	//通过进程名获取进程ID
	bool get_processid_by_name( const char* task_name, unsigned long& processId )
	{
		return false;
	}

	bool get_name_by_processid( unsigned long pid, char *task_name )
	{
		return false;
	}


	struct sockaddr_ll {
		unsigned short sll_family;   /* Always AF_PACKET */
		unsigned short sll_protocol; /* Physical layer protocol */
		int            sll_ifindex;  /* Interface number */
		unsigned short sll_hatype;   /* Header type */
		unsigned char  sll_pkttype;  /* Packet type */
		unsigned char  sll_halen;    /* Length of address */
		unsigned char  sll_addr[8];  /* Physical layer address */
	};

	int	get_networks_info( pfnNetworkInfo pFun, void* pUser )
	{
		
	}

	int get_disk_space( const char* c_szDir, tagSys_Disk_INFO* pInfo )
	{
		return -1;  
	}

	ssize_t mygetline(char** line,size_t *n,FILE *fp)
	{
		char *buf = *line;
		ssize_t c, i = 0;//i来记录字符串长度，c来存储字符
		if( buf == NULL || *n == 0 )
		{
			*line = (char*)malloc(BUF_SIZE / 3);
			buf = *line;
			*n = BUF_SIZE / 3;
		}

		//buf为或n为0时动态为期分配空间
		while( (c = fgetc(fp)) != '\n' )
		{
			if( c == EOF )
			{
				if( i > 0 )
					return i; // 结束当一行
				return -1;
			}

			if( i < *n - 2 )//留2个空间给\n和\0
			{
				*(buf + i) = c;
				i ++;
			}
			else
			{
				*n = *n + BUF_SIZE / 3;
				buf = (char*)realloc(buf, *n);//空间不足时，重新进行分配
				*(buf + i) = c;
				i ++;
			}
		}
		*(buf + i) = '\n';
		i ++;
		*(buf + i) = '\0';

		return i;
	}

	int get_system_resource( const char* szFilePath )
	{  
		char * line = NULL;  
		size_t len = 0;  
		ssize_t read;
		char szData[30] = {0};
		FILE * fp = fopen(szFilePath, "r");  
		if (fp == NULL)  
			return -1;

		if( read = mygetline(&line, &len, fp) != -1 )
		{
			if (line)  
			{  
				sscanf(line, "%d", &len);
				free(line);
				fclose(fp);
				return len;
			}
		}

		fclose(fp);
		return -1;  
	}

#ifdef  __cplusplus
}
#endif
