/*****************************************************************************
*  *Copyright (C)															 *
*  @file     Example.h                                                       *
*  @brief    ��ȡϵͳ��Ϣ                                                    *
*  @Details.                                                                 *
*                                                                            *
*  @author   Lin.Sheng														 *
*  @email    lys515800@163.com                                               *
*  @version  3.0.0.1														 *
*  @date	 2015.08.21														 *
*  @license																	 *
/****************************************************************************/
#ifndef __SYSINFO_20151127152350_1448609030_H__
#define __SYSINFO_20151127152350_1448609030_H__

#ifdef __cplusplus
	extern "C" {
#endif

	typedef long long          	Int64_t;
	typedef unsigned long long	UInt64_t;

	enum NetworksType
	{
		enNETWORKS_TYPE_UNKNOW,			//δ֪
		enNETWORKS_TYPE_BLUETOOTH,		//����
		enNETWORKS_TYPE_WIRE,			//����
		enNETWORKS_TYPE_WIRELESS,		//����
		enNETWORKS_TYPE_VIRTUAL			//����
	};

	struct tagNetworks_INFO
	{
		char*	szName;
		char*	szIP;
		char*	szDescribe;
		unsigned char	Mac[6];
		unsigned char	Type;
	};

	struct tagSys_Disk_INFO
	{
		UInt64_t uTotalSize;
		UInt64_t uFreeSize;
	};

	typedef	void (*pfnNetworkInfo)( tagNetworks_INFO* pInfo, void* pUser );

	/// ���CPU�ĺ���
	int get_processor_number( void );

	int get_physical_processor_number( void );

	//��ȡcpu���к�
	int get_cpu_serialnumber( char* pserial, int isize );

	//��ȡcpu����
	int get_cpu_vendor( char* pvendor, int isize );

	//��ȡcpu����
	int get_cpu_brand( char* pbrand, int isize );

	/// ��ȡcpuʹ���ʣ�����-1ʧ��
	int get_cpu_usage( void );

	//��ȡ�ڴ���Ϣ
	void get_memory_info( UInt64_t* total, UInt64_t* free );

	/// ��ȡ��ǰ�����ܹ�����д��IO�ֽ���������-1ʧ�ܣ�0�ɹ�
	int get_process_io_bytes( UInt64_t* read_bytes, UInt64_t* write_bytes );

	/// ��ȡ��ǰ�����ڴ�������ڴ�ʹ����������-1ʧ�ܣ�0�ɹ�
	int get_process_memory_usage( UInt64_t* mem, UInt64_t* vmem );

	/// ��ȡ��ǰ���̵�cpuʹ���ʣ�����-1ʧ��
	int get_process_cpu_usage( void );

	//ͨ����������ȡ����ID
	bool get_processid_by_name( const char* task_name, unsigned long& processId );
	
	//ͨ������ID��ȡ������
	bool get_name_by_processid( unsigned long pid, char *task_name );

	//��ȡ��������Ϣ
	int	get_networks_info( pfnNetworkInfo pFun, void* pUser );

	//�Ƿ����Ȩ��
	bool is_admin_auth( void );

	//��ȡһ����ĳ���µ�����
	int get_days_to_month( unsigned short uYear, unsigned char uMonth );

	//��ȡĳ������
	int get_days_to_year( unsigned short uYear );

	//��ȡ���̿ռ�
	int get_disk_space( const char* c_szDir, tagSys_Disk_INFO* pInfo );

	//��ȡϵͳ������Դ
	int get_system_resource( const char* szFilePath );

#ifdef  __cplusplus
	}
#endif

#endif/*PROCESS_STAT_H*/