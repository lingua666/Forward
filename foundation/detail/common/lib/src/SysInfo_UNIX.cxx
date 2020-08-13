
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

	/// ���CPU�ĺ���
	int get_processor_number( void )
	{
		return get_nprocs();   //GNU fuction  
	}

	int get_physical_processor_number( void )
	{

	}

	/// ��ȡ��ǰ���̵�cpuʹ���ʣ�����-1ʧ��
	int get_process_cpu_usage( void )
	{//δʵ��
		return -1;
	}

	/// ��ȡ��ǰ�����ڴ�������ڴ�ʹ����������-1ʧ�ܣ�0�ɹ�
	int get_process_memory_usage( UInt64_t* mem, UInt64_t* vmem )
	{//δʵ��
		return -1;
	}

	/// ��ȡ��ǰ�����ܹ�����д��IO�ֽ���������-1ʧ�ܣ�0�ɹ�
	int get_process_io_bytes( UInt64_t* read_bytes, UInt64_t* write_bytes )
	{//δʵ��
		return -1;
	}

	//��ȡ�ڴ���Ϣ
	void get_memory_info( UInt64_t* total, UInt64_t* free )
	{

	}

	//ͨ����������ȡ����ID
	bool get_processid_by_name( const char* task_name, unsigned long& processId )
	{
		DIR *dir = NULL;
		struct dirent *ptr = NULL;
		FILE *fp = NULL;
		char filepath[50] = {0};//��С���⣬��װ��cmdline�ļ���·������
		char cur_task_name[BUF_SIZE] = {0};//��С���⣬��װ��Ҫʶ����������ı�����
		dir = opendir("/proc"); //��·��
		if (NULL != dir)
		{
			while ((ptr = readdir(dir)) != NULL) //ѭ����ȡ·���µ�ÿһ���ļ�/�ļ���
			{
				//�����ȡ������"."����".."����������ȡ���Ĳ����ļ�������Ҳ����
				if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))             
					continue;

				if (DT_DIR != ptr->d_type) 
					continue;

				//sprintf(filepath, "/proc/%s/status", ptr->d_name);//����Ҫ��ȡ���ļ���·��
				sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);//����Ҫ��ȡ���ļ���·��
				fp = fopen(filepath, "r");//���ļ�
				if (NULL != fp)
				{
					memset(cur_task_name, 0, sizeof(cur_task_name));
					if( fgets(cur_task_name, BUF_SIZE - 1, fp)== NULL )
					{
						fclose(fp);
						continue;
					}

					//����ļ���������Ҫ�����ӡ·�������֣������̵�PID��
					char* name = strrchr(cur_task_name,  '/');
					if( name != NULL )
						++ name;

					if( !strcmp(task_name, cur_task_name) || 
						(name != NULL && !strcmp(task_name, name)) )
					{
						processId = _string_type::StringToInt(ptr->d_name);
						fclose(fp);
						closedir(dir);//�ر�·��
						return true;
					}

					fclose(fp);
				}
			}

			closedir(dir);//�ر�·��
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
			{//��������
				pTmp->type = enNETWORKS_TYPE_WIRELESS;
			}
			else if( strstr(ifap->ifa_name, "eth") > 0 )
			{//��������
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
		unsigned long long blocksize = diskInfo.f_bsize;    //ÿ��block��������ֽ���  
		pInfo->uTotalSize = blocksize * diskInfo.f_blocks;   //�ܵ��ֽ�����f_blocksΪblock����Ŀ  
		pInfo->uFreeSize = diskInfo.f_bfree * blocksize; //ʣ��ռ�Ĵ�С  

		//unsigned long long availableDisk = diskInfo.f_bavail * blocksize;   //���ÿռ��С
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

