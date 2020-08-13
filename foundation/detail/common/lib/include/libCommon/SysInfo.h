/*****************************************************************************
*  *Copyright (C)															 *
*  @file     Example.h                                                       *
*  @brief    获取系统信息                                                    *
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
		enNETWORKS_TYPE_UNKNOW,			//未知
		enNETWORKS_TYPE_BLUETOOTH,		//蓝牙
		enNETWORKS_TYPE_WIRE,			//有线
		enNETWORKS_TYPE_WIRELESS,		//无线
		enNETWORKS_TYPE_VIRTUAL			//虚拟
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

	/// 获得CPU的核数
	int get_processor_number( void );

	int get_physical_processor_number( void );

	//获取cpu序列号
	int get_cpu_serialnumber( char* pserial, int isize );

	//获取cpu厂家
	int get_cpu_vendor( char* pvendor, int isize );

	//获取cpu类型
	int get_cpu_brand( char* pbrand, int isize );

	/// 获取cpu使用率，返回-1失败
	int get_cpu_usage( void );

	//获取内存信息
	void get_memory_info( UInt64_t* total, UInt64_t* free );

	/// 获取当前进程总共读和写的IO字节数，返回-1失败，0成功
	int get_process_io_bytes( UInt64_t* read_bytes, UInt64_t* write_bytes );

	/// 获取当前进程内存和虚拟内存使用量，返回-1失败，0成功
	int get_process_memory_usage( UInt64_t* mem, UInt64_t* vmem );

	/// 获取当前进程的cpu使用率，返回-1失败
	int get_process_cpu_usage( void );

	//通过进程名获取进程ID
	bool get_processid_by_name( const char* task_name, unsigned long& processId );
	
	//通过进程ID获取进程名
	bool get_name_by_processid( unsigned long pid, char *task_name );

	//获取多网卡信息
	int	get_networks_info( pfnNetworkInfo pFun, void* pUser );

	//是否管理权限
	bool is_admin_auth( void );

	//获取一年中某个月的天数
	int get_days_to_month( unsigned short uYear, unsigned char uMonth );

	//获取某年天数
	int get_days_to_year( unsigned short uYear );

	//获取磁盘空间
	int get_disk_space( const char* c_szDir, tagSys_Disk_INFO* pInfo );

	//获取系统配置资源
	int get_system_resource( const char* szFilePath );

#ifdef  __cplusplus
	}
#endif

#endif/*PROCESS_STAT_H*/