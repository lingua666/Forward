
/** @file
* @brief ����ͳ����Ϣ������ʵ��
* @author ������
* @date 2009/05/03
* @version 0.1
*
* ���ִ�������MSDN������
* ���ִ�������google chromium��Ŀ
*
* ��Ҫ���ӵ�psapi.lib
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

	/// ���CPU�ĺ���
	int get_processor_number( void )
	{
		return sysconf(_SC_NPROCESSORS_ONLN);   //GNU fuction  
	}

	int get_physical_processor_number( void )
	{
		return -1;
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
		ssize_t c, i = 0;//i����¼�ַ������ȣ�c���洢�ַ�
		if( buf == NULL || *n == 0 )
		{
			*line = (char*)malloc(BUF_SIZE / 3);
			buf = *line;
			*n = BUF_SIZE / 3;
		}

		//bufΪ��nΪ0ʱ��̬Ϊ�ڷ���ռ�
		while( (c = fgetc(fp)) != '\n' )
		{
			if( c == EOF )
			{
				if( i > 0 )
					return i; // ������һ��
				return -1;
			}

			if( i < *n - 2 )//��2���ռ��\n��\0
			{
				*(buf + i) = c;
				i ++;
			}
			else
			{
				*n = *n + BUF_SIZE / 3;
				buf = (char*)realloc(buf, *n);//�ռ䲻��ʱ�����½��з���
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
