
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
#include <sys/resource.h>
//#include <cpuid.h>
#endif

//#include <cpuid.h>
#include <linux/hdreg.h>
#include <linux/if_ether.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define VMRSS_LINE 22
#define PROCESS_ITEM 14

	static const char* get_items(const char* buffer, unsigned int item)
	{
		// read from buffer by offset
		const char* p = buffer;

		int len = strlen(buffer);
		int count = 0;

		for (int i = 0; i < len; i++)
		{
			if (' ' == *p)
			{
				count++;
				if (count == item - 1)
				{
					p++;
					break;
				}
			}
			p++;
		}

		return p;
	}

	//��ȡ��CPUʱ��
	typedef struct CPUInfo_INFO20220516
	{
		UInt64	user_time;
		UInt64	nice_time;
		UInt64	system_time;
		UInt64	idle_time;
	}CPUInfo_INFO20220516;

	static inline UInt64_t get_cpu_total_occupy(CPUInfo_INFO20220516* pCPUInfo)
	{
		// get total cpu use time

		// different mode cpu occupy time
		UInt64_t user_time = 0;
		UInt64_t nice_time = 0;
		UInt64_t system_time = 0;
		UInt64_t idle_time = 0;

		FILE* fd;
		char buff[1024] = { 0 };

		fd = fopen("/proc/stat", "r");
		if (NULL == fd)
			return 0;

		fgets(buff, sizeof(buff), fd);
		char name[64] = { 0 };
		sscanf(buff, "%s %lld %lld %lld %lld", name, &user_time, &nice_time, &system_time, &idle_time);
		fclose(fd);

		if (pCPUInfo != NULL)
		{
			pCPUInfo->user_time = user_time;
			pCPUInfo->nice_time = nice_time;
			pCPUInfo->system_time = system_time;
			pCPUInfo->idle_time = idle_time;
		}

		return (user_time + nice_time + system_time + idle_time);
	}

	//��ȡ����CPUʱ��
	static inline UInt64_t get_cpu_proc_occupy(int pid)
	{
		// get specific pid cpu use time
		unsigned int tmp_pid = 0;
		UInt64_t utime = 0;  // user time
		UInt64_t stime = 0;  // kernel time
		UInt64_t cutime = 0; // all user time
		UInt64_t cstime = 0; // all dead time

		char file_name[64] = { 0 };
		FILE* fd;
		char line_buff[1024] = { 0 };
		sprintf(file_name, "/proc/%d/stat", pid);

		fd = fopen(file_name, "r");
		if (NULL == fd)
			return 0;

		fgets(line_buff, sizeof(line_buff), fd);

		sscanf(line_buff, "%u", &tmp_pid);
		const char* q = get_items(line_buff, PROCESS_ITEM);
		sscanf(q, "%lld %lld %lld %lld", &utime, &stime, &cutime, &cstime);
		fclose(fd);

		return (utime + stime + cutime + cstime);
	}

	/// ���CPU�ĺ���
	int get_processor_number(void)
	{
		return get_nprocs();   //GNU fuction  
	}

	int get_physical_processor_number(void)
	{
		return get_processor_number();
	}

	//��ȡcpu���к�
	int get_cpu_serialnumber(char* pserial, int isize)
	{
		if (pserial == NULL || isize < 16)
			return -1;

		unsigned int level = 1;
		unsigned int eax = 3 /* processor serial number */, ebx = 0, ecx = 0, edx = 0;

		//__get_cpuid(level, &eax, &ebx, &ecx, &edx);

		// byte swap
		int first = ((eax >> 24) & 0xff) | ((eax << 8) & 0xff0000) | ((eax >> 8) & 0xff00) | ((eax << 24) & 0xff000000);
		int last = ((edx >> 24) & 0xff) | ((edx << 8) & 0xff0000) | ((edx >> 8) & 0xff00) | ((edx << 24) & 0xff000000);
		sprintf(pserial, "%08X%08X", first, last);
		return 16;
	}

	//��ȡcpu����
	int get_cpu_vendor(char* pvendor, int isize)
	{
		return -1;
	}

	//��ȡcpu����
	int get_cpu_brand(char* pbrand, int isize)
	{
		return -1;
	}
	
	/// ��ȡcpuʹ���ʣ�����-1ʧ�� ��λ/�ٷֱ�
	int get_sys_cpu_usage( void )
	{
		pid_t pid = getpid();
		UInt64_t totalcputime1 = 0, totalcputime2 = 0;
		UInt64_t idlecputime1 = 0, idlecputime2 = 0;

		CPUInfo_INFO20220516 CPUInfo1;
		totalcputime1 = get_cpu_total_occupy(&CPUInfo1);

		// FIXME: the 200ms is a magic number, works well
		usleep(1000000); // sleep 200ms to fetch two time point cpu usage snapshots sample for later calculation

		CPUInfo_INFO20220516 CPUInfo2;
		totalcputime2 = get_cpu_total_occupy(&CPUInfo2);
		
		float value = (float(CPUInfo2.system_time - CPUInfo1.system_time) + float(CPUInfo2.user_time - CPUInfo1.user_time));
		float total = float(totalcputime2 - totalcputime1);
		float pcpu = 0.0;
		if (0 != totalcputime2 - totalcputime1)
			pcpu = 100 * value / total; // float number

		int cpu_num = get_processor_number();
		pcpu *= cpu_num; // should multiply cpu num in multiple cpu machine

		return pcpu;
	}

	/// ��ȡ��ǰ���̵�cpuʹ���� �ٷֱȣ�����-1ʧ�� ��λ/�ٷֱ�
	int get_process_cpu_usage(void)
	{//
		pid_t pid = getpid();
		UInt64_t totalcputime1 = 0, totalcputime2 = 0;
		UInt64_t procputime1 = 0, procputime2 = 0;

		totalcputime1 = get_cpu_total_occupy(NULL);
		procputime1 = get_cpu_proc_occupy(pid);

		// FIXME: the 200ms is a magic number, works well
		usleep(200000); // sleep 200ms to fetch two time point cpu usage snapshots sample for later calculation

		totalcputime2 = get_cpu_total_occupy(NULL);
		procputime2 = get_cpu_proc_occupy(pid);

		float pcpu = 0.0;
		if (0 != totalcputime2 - totalcputime1)
			pcpu = 100 * (procputime2 - procputime1) / float(totalcputime2 - totalcputime1); // float number

		int cpu_num = get_processor_number();
		pcpu *= cpu_num; // should multiply cpu num in multiple cpu machine

		return pcpu;
	}

	/// ��ȡ��ǰ�����ڴ�������ڴ�ʹ����������-1ʧ�ܣ�0�ɹ� ��λ/K
	int get_process_memory_usage(UInt64_t* mem, UInt64_t* vmem)
	{//
		pid_t p = getpid();
		char file[64] = { 0 };	//�ļ���
		char line_buff[256] = { 0 };  //��ȡ�еĻ�����
		sprintf(file, "/proc/%d/status", p);
		fprintf(stderr, "current pid:%d\n", p);
		FILE *fd = fopen(file, "r"); //��R���ķ�ʽ���ļ��ٸ���ָ��fd
		if (fd == NULL)
			return -1;

		//��ȡvmrss:ʵ�������ڴ�ռ��

		char name[32] = { 0 };//�����Ŀ����
		int value = { 0 };	//�����ֵ

							//��ȡVmRSS��һ�е�����
		for (int i = 0; i < VMRSS_LINE - 1; i++)
		{
			char* ret = fgets(line_buff, sizeof(line_buff), fd);
			sscanf(line_buff, "%s %d", name, &value);
			if (strncmp(name, "VmRSS", strlen("VmRSS")) == 0)
			{
				*mem = value;
			}

			if (strncmp(name, "VmSize", strlen("VmSize")) == 0)
			{
				*vmem = value;
			}

			//fprintf(stderr, "====%s��%d====\n", name, value);
		}

		fclose(fd);     //�ر��ļ�fd
		return 1;
	}

	/// ��ȡ��ǰ�����ܹ�����д��IO�ֽ���������-1ʧ�ܣ�0�ɹ�
	int get_process_io_bytes(UInt64_t* read_bytes, UInt64_t* write_bytes)
	{//δʵ��
		return -1;
	}

	//��ȡ�ڴ���Ϣ ��λ/K
	void get_memory_info(UInt64_t* total, UInt64_t* free)
	{
		struct sysinfo s_info;
		int error = sysinfo(&s_info);
		if (error == -1)
		{
			printf("get_memory_info error:%d\r\n", error);
			return;
		}

		*total = s_info.totalram / 1024;
		*free = s_info.freeram / 1024;
	}

	//ͨ����������ȡ����ID
	bool get_processid_by_name(const char* task_name, unsigned long& processId)
	{
		DIR *dir = NULL;
		struct dirent *ptr = NULL;
		FILE *fp = NULL;
		char filepath[50] = { 0 };//��С���⣬��װ��cmdline�ļ���·������
		char cur_task_name[BUF_SIZE] = { 0 };//��С���⣬��װ��Ҫʶ����������ı�����
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
					if (fgets(cur_task_name, BUF_SIZE - 1, fp) == NULL)
					{
						fclose(fp);
						continue;
					}

					//����ļ���������Ҫ�����ӡ·�������֣������̵�PID��
					char* name = strrchr(cur_task_name, '/');
					if (name != NULL)
						++name;

					if (!strcmp(task_name, cur_task_name) ||
						(name != NULL && !strcmp(task_name, name)))
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

	bool get_name_by_processid(unsigned long pid, char *task_name)
	{
		char proc_pid_path[BUF_SIZE] = { 0 };
		char buf[BUF_SIZE] = { 0 };
		sprintf(proc_pid_path, "/proc/%d/cmdline", pid);
		FILE* fp = fopen(proc_pid_path, "r");
		if (NULL != fp)
		{
			if (fgets(buf, BUF_SIZE - 1, fp) == NULL)
			{
				fclose(fp);
			}
			fclose(fp);

			char* name = strrchr(buf, '/');
			if (name != NULL)
				++name;
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

	int	get_networks_info(pfnNetworkInfo pFun, void* pUser)
	{
		struct if_ipv4_info* if_info = 0;
		struct if_ipv4_info* pTmp = 0;

		struct ifaddrs* ifa = 0;
		struct ifaddrs* ifa_h = 0;
		struct ifaddrs* ifap = 0;
		struct sockaddr_ll* s = 0;
		int family;  /* protocl family */
		char szIP[20] = { 0 };

		if (getifaddrs(&ifa) == -1) {
			perror("get_networks_info failed!!\n");
			return -1;
		}

		ifa_h = ifa;

		for (ifap = ifa; ifap != 0; ifap = ifap->ifa_next)
		{
			if (strcmp(ifap->ifa_name, "lo") == 0) {
				continue; /* skip the lookback card */
			}

			if (ifap->ifa_addr == NULL)
				continue; /* if addr is NULL, this must be no ip address */

			tagNetworks_INFO	Info = { 0 };
			Info.szName = ifap->ifa_name;
			if (strstr(ifap->ifa_name, "wlan") > 0)
			{//��������
				Info.Type = enNETWORKS_TYPE_WIRELESS;
			}
			else if (strstr(ifap->ifa_name, "eth") > 0)
			{//��������
				Info.Type = enNETWORKS_TYPE_WIRE;
			}

			if (ifap->ifa_addr->sa_family == AF_PACKET)
			{//�����ַ
				s = (struct sockaddr_ll *)ifap->ifa_addr;
				memcpy(Info.Mac, s->sll_addr, sizeof(s->sll_addr));

				//��ȡIP��ַ
				struct ifaddrs* temp;
				for (temp = ifa; temp != 0; temp = temp->ifa_next)
				{
					if (strcmp(temp->ifa_name, Info.szName) == 0 && temp->ifa_addr->sa_family == AF_INET)
					{
						memset(szIP, 0, sizeof(szIP));

						inet_ntop(temp->ifa_addr->sa_family, &((struct sockaddr_in *)temp->ifa_addr)->sin_addr,
							(char*)szIP, sizeof(struct sockaddr_in));
						Info.szIP = szIP;
						break;
					}
				}
			}
			else
			{
				continue;
			}

			/*char szIP[20] = { 0 };
			switch (ifap->ifa_addr->sa_family)
			{
			case AF_INET:
			{
			inet_ntop(ifap->ifa_addr->sa_family, &((struct sockaddr_in *)ifap->ifa_addr)->sin_addr,
			(char*)szIP, sizeof(struct sockaddr_in));
			Info.szIP = szIP;
			}
			break;
			case AF_PACKET:
			{
			s = (struct sockaddr_ll *)ifap->ifa_addr;
			memcpy(Info.Mac, s->sll_addr, sizeof(s->sll_addr));
			}
			break;
			}*/

			if (pFun)
				pFun(&Info, pUser);
		}

		if (ifa_h != 0)
			freeifaddrs(ifa_h);
	}

	int get_system_resource(const char* szFilePath)
	{
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		char szData[30] = { 0 };
		FILE * fp = fopen(szFilePath, "r");
		if (fp == NULL)
			return -1;

		if (read = getline(&line, &len, fp) != -1)
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

	//����ϵͳ�ں�ת���ļ�����󳤶ȺͿ��Դ򿪵�socket����(linuxϵͳ��Ч)
	int set_core_and_file_max(int iCore, int iFile)
	{
		//����ϵͳ�ں�ת���ļ�����󳤶�
		struct rlimit rlim = { 0 }, rlim_new = { 0 };
		if (getrlimit(RLIMIT_CORE, &rlim) == 0)
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = iCore == -1 ? RLIM_INFINITY : iCore;
			if (setrlimit(RLIMIT_CORE, &rlim_new) != 0)
			{
				rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
				(void)setrlimit(RLIMIT_CORE, &rlim_new);
			}
		}

		//����ϵͳ���Դ򿪵�socket����
		if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = iFile == -1 ? RLIM_INFINITY : iFile;
			if (setrlimit(RLIMIT_NOFILE, &rlim_new) != 0)
			{
				rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
				(void)setrlimit(RLIMIT_NOFILE, &rlim_new);
			}
		}

		return 1;
	}

	bool is_admin_auth(void)
	{
		return false;
	}
	
	int get_disk_space(const char* c_szDir, tagSys_Disk_INFO* pInfo)
	{
		struct statfs diskInfo;

		statfs(c_szDir, &diskInfo);
		unsigned long long blocksize = diskInfo.f_bsize;    //ÿ��block��������ֽ���  
		pInfo->uTotalSize = blocksize * diskInfo.f_blocks;   //�ܵ��ֽ�����f_blocksΪblock����Ŀ  
		pInfo->uFreeSize = diskInfo.f_bfree * blocksize; //ʣ��ռ�Ĵ�С  

														 //unsigned long long availableDisk = diskInfo.f_bavail * blocksize;   //���ÿռ��С
		return 1;
	}
	
	//��ȡ������ϸ��Ϣ
	int get_disk_detail(fpnGetDiskDetailInfo pFunc)
	{
		int fd = -1;
		struct hd_driveid hid;
		FILE *fp = NULL;
		char line[0x100], *disk, *root, *p;
	 
		fp = fopen ("/etc/mtab", "r");
		if (fp == NULL)
		{
			fprintf (stderr, "No /etc/mtab file.\n");
			return -1;
		}
	 
		while (fgets (line, sizeof line, fp) != NULL)
		{
			disk = strtok (line, " ");
			if (disk == NULL)
			{
				continue;
			}
	 
			root = strtok (NULL, " ");
			if (root == NULL)
			{
				continue;
			}
	 
			if (strcmp (root, "/") == 0)
			{
				for (p = disk + strlen (disk) - 1; isdigit (*p); p --)
				{
					*p = '\0';
				}

				fd = open (disk, O_RDONLY);
				if(fd > 0)
					break;
			}
		}
	 
		fclose(fp);
	 
		if(fd < 0)
		{
			fprintf(stderr, "open hard disk device failed.\n");
			return -1;
		}
	 
		if(ioctl(fd, HDIO_GET_IDENTITY, &hid) < 0)
		{
			fprintf (stderr, "ioctl error:%d.\n", errno);
			return -1;
		}
	 
		close (fd);
		
		tagDisk_Detail_INFO DiskINFO = { 0 };
		DiskINFO.pSerialNumber = (char*)hid.serial_no;
		
		if (pFunc)
			pFunc(&DiskINFO);
		
		char id[100] = {0};
		snprintf (id, sizeof(id), "%s", hid.serial_no);
		fprintf (stdout, "get hard disk serial number: %s/n", id);
	 
		return 1;
	}

#ifdef  __cplusplus
}
#endif

