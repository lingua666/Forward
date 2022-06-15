
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
#if PLATFORM_OS == PLATFORM_OS_ANDROID
#include <libCommon/ifaddrs.h>
#else
#include <ifaddrs.h>
#include <linux/if_packet.h>
#endif

#include <linux/if_ether.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#define BUF_SIZE 1024

	/// 获得CPU的核数
	int get_processor_number( void )
	{
		return get_nprocs();   //GNU fuction  
	}

	int get_physical_processor_number( void )
	{

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
		DIR *dir = NULL;
		struct dirent *ptr = NULL;
		FILE *fp = NULL;
		char filepath[50] = {0};//大小随意，能装下cmdline文件的路径即可
		char cur_task_name[BUF_SIZE] = {0};//大小随意，能装下要识别的命令行文本即可
		dir = opendir("/proc"); //打开路径
		if (NULL != dir)
		{
			while ((ptr = readdir(dir)) != NULL) //循环读取路径下的每一个文件/文件夹
			{
				//如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
				if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))             
					continue;

				if (DT_DIR != ptr->d_type) 
					continue;

				//sprintf(filepath, "/proc/%s/status", ptr->d_name);//生成要读取的文件的路径
				sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);//生成要读取的文件的路径
				fp = fopen(filepath, "r");//打开文件
				if (NULL != fp)
				{
					memset(cur_task_name, 0, sizeof(cur_task_name));
					if( fgets(cur_task_name, BUF_SIZE - 1, fp)== NULL )
					{
						fclose(fp);
						continue;
					}

					//如果文件内容满足要求则打印路径的名字（即进程的PID）
					char* name = strrchr(cur_task_name,  '/');
					if( name != NULL )
						++ name;

					if( !strcmp(task_name, cur_task_name) || 
						(name != NULL && !strcmp(task_name, name)) )
					{
						processId = _string_type::StringToInt(ptr->d_name);
						fclose(fp);
						closedir(dir);//关闭路径
						return true;
					}

					fclose(fp);
				}
			}

			closedir(dir);//关闭路径
		}

		return false;
	}

	bool get_name_by_processid( unsigned long pid, char *task_name )
	{
		char proc_pid_path[BUF_SIZE] = {0};
		char buf[BUF_SIZE] = {0};
		sprintf(proc_pid_path, "/proc/%d/cmdline", pid);
		FILE* fp = fopen(proc_pid_path, "r");
		if(NULL != fp)
		{
			if( fgets(buf, BUF_SIZE - 1, fp) == NULL )
			{
				fclose(fp);
			}
			fclose(fp);

			char* name = strrchr(buf,  '/');
			if( name != NULL )
				++ name;
			else
				name = buf;

			strcpy(task_name, name);
			return true;
		}

		return false;
	}

	/* define a get if info struct */
	struct if_ipv4_info {
		char if_name[10];
		unsigned char ip_addr[20];
		unsigned char hd_addr[20];
		unsigned char type;
		struct if_ipv4_info *next;
	};

	int	get_networks_info( pfnNetworkInfo pFun, void* pUser )
	{
		struct if_ipv4_info* if_info = 0;
		struct if_ipv4_info* pTmp = 0;

		struct ifaddrs* ifa = 0;
		struct ifaddrs* ifap = 0;
		struct sockaddr_ll* s = 0;
		int family;  /* protocl family */

		if (getifaddrs (&ifa) == -1) {
			perror ("get_networks_info failed!!\n");
			return -1;
		}

		for (ifap = ifa; ifap != 0; ifap = ifap->ifa_next)
		{  
			if (strcmp (ifap->ifa_name, "lo") == 0) {  
				continue; /* skip the lookback card */  
			}  

			if (ifap->ifa_addr == NULL)
				continue; /* if addr is NULL, this must be no ip address */

			if( if_info == 0 )
			{
				if_info = (if_ipv4_info*)malloc (sizeof (struct if_ipv4_info));
				memset(if_info, 0, sizeof(struct if_ipv4_info));
				strcpy(if_info->if_name, ifap->ifa_name);
				pTmp = if_info;
			}
			else
			{
				for (pTmp = if_info; pTmp != 0; pTmp = pTmp->next)
				{
					if (strcmp (pTmp->if_name, ifap->ifa_name) == 0)
						break;
				}
			}

			if( pTmp == 0 )
			{
				pTmp = (if_ipv4_info*)malloc (sizeof (struct if_ipv4_info));
				memset(pTmp, 0, sizeof(struct if_ipv4_info));
				strcpy(pTmp->if_name, ifap->ifa_name);
			}

			switch (ifap->ifa_addr->sa_family) 
			{
			case AF_INET:
				{
					inet_ntop (ifap->ifa_addr->sa_family, &((struct sockaddr_in *)ifap->ifa_addr)->sin_addr,  
						(char*)pTmp->ip_addr, sizeof (struct sockaddr_in));
				}
				break;  
			case AF_PACKET:  
				{
					s = (struct sockaddr_ll *)ifap->ifa_addr;
					memcpy(pTmp->hd_addr, s->sll_addr, sizeof(s->sll_addr));
				}
				break;  
			}

			if( strstr(ifap->ifa_name, "wlan") > 0 )
			{//无线网卡
				pTmp->type = enNETWORKS_TYPE_WIRELESS;
			}
			else if( strstr(ifap->ifa_name, "eth") > 0 )
			{//有线网卡
				pTmp->type = enNETWORKS_TYPE_WIRE;
			}
		}  

		/* printf all of them */  
		for (pTmp = if_info; pTmp != NULL; pTmp = pTmp->next)
		{  
			tagNetworks_INFO	Info = {0};
			Info.szName = (char*)pTmp->if_name;
			Info.szIP = (char*)pTmp->ip_addr;
			memcpy(Info.Mac, (char*)pTmp->hd_addr, sizeof(Info.Mac));
			Info.Type = pTmp->type;

			if( pFun )
				pFun(&Info, pUser);
		}
	}

	int get_disk_space( const char* c_szDir, tagSys_Disk_INFO* pInfo )
	{
		struct statfs diskInfo;

		statfs(c_szDir, &diskInfo);  
		unsigned long long blocksize = diskInfo.f_bsize;    //每个block里包含的字节数  
		pInfo->uTotalSize = blocksize * diskInfo.f_blocks;   //总的字节数，f_blocks为block的数目  
		pInfo->uFreeSize = diskInfo.f_bfree * blocksize; //剩余空间的大小  

		//unsigned long long availableDisk = diskInfo.f_bavail * blocksize;   //可用空间大小
		return 1;  
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

		if( read = getline(&line, &len, fp) != -1 )
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

	bool is_admin_auth( void )
	{
		return false;
	}

#ifdef  __cplusplus
}
#endif

