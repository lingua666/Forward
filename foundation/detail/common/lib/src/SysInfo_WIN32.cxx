#include <libCommon/SysInfo.h>
#include <libCommon/os/Types.h>
#include <libCommon/os/Platform.h>
#include <libCommon/os/windowsi.h>

#include <comdef.h>
#include <atlbase.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

static int GetDiskInfo_WMI_20170823(fpnGetDiskDetailInfo pFunc)
{
	HRESULT hres;

	// Initialize COM. ------------------------------------------
	hres = CoInitialize(0);//CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		return -1;                  // Program has failed.
	}
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------
	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);

	if (FAILED(hres))
	{
		CoUninitialize();
		return -1;                    // Program has failed.
	}

	// Obtain the initial locator to WMI -------------------------
	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		return -1;                 // Program has failed.
	}

	// Connect to WMI through the IWbemLocator::ConnectServer method
	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
	);

	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		return -1;                // Program has failed.
	}

	// Set security levels on the proxy -------------------------
	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
	);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -1;               // Program has failed.
	}
	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----
	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_DiskDrive"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -1;               // Program has failed.
	}
	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	tagDisk_Detail_INFO DiskINFO = { 0 };
	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);
		if (0 == uReturn)
		{
			break;
		}

		USES_CONVERSION;

		VARIANT vtProp;
		// Get the value of the Name property
		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		_string_type sName = W2T(vtProp.bstrVal);

		hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
		_string_type sManufacturer = _string_type(W2T(vtProp.bstrVal)).strim();
		DiskINFO.pManufacturer = (char*)sManufacturer.c_str();

		hr = pclsObj->Get(L"Model", 0, &vtProp, 0, 0);
		_string_type sModel = _string_type(W2T(vtProp.bstrVal)).strim();
		DiskINFO.pModel = (char*)sModel.c_str();

		hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		_string_type sSerialNumber = _string_type(W2T(vtProp.bstrVal)).strim();
		DiskINFO.pSerialNumber = (char*)sSerialNumber.c_str();

		hr = pclsObj->Get(L"InterfaceType", 0, &vtProp, 0, 0);
		_string_type sInterfaceType = _string_type(W2T(vtProp.bstrVal)).strim();
		DiskINFO.pInterfaceType = (char*)sInterfaceType.c_str();

		hr = pclsObj->Get(L"MediaType", 0, &vtProp, 0, 0);
		_string_type sMediaType = _string_type(W2T(vtProp.bstrVal)).strim();

		hr = pclsObj->Get(L"Size", 0, &vtProp, 0, 0);
		_string_type sSize = _string_type(W2T(vtProp.bstrVal)).strim();
		DiskINFO.uSize = _string_type::StringToInt64(sSize);
		VariantClear(&vtProp);

		if (pFunc)
			pFunc(&DiskINFO);
	}
	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	pclsObj->Release();
	CoUninitialize();
	return 1;   // Program successfully completed.
}

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <TLHELP32.H>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��

	/// ʱ��ת��
	static UInt64_t file_time_2_utc( const FILETIME* ftime )
	{
		LARGE_INTEGER li;

		assert(ftime);
		li.LowPart = ftime->dwLowDateTime;
		li.HighPart = ftime->dwHighDateTime;
		return li.QuadPart;
	}


	/// ���CPU�ĺ���
	int get_processor_number( void )
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return (int)info.dwNumberOfProcessors;
	}

	DWORD num_logical_cores(ULONG_PTR mask)
	{
		DWORD     left_shift    = sizeof(ULONG_PTR) * 8 - 1;
		DWORD     bit_set_count = 0;
		ULONG_PTR bit_test      = (ULONG_PTR)1 << left_shift;

		for (DWORD i = 0; i <= left_shift; ++i) {
			bit_set_count += ((mask & bit_test) ? 1 : 0);
			bit_test      /= 2;
		}

		return bit_set_count;
	}

	int get_physical_processor_number( void )
	{
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION info = NULL, temp = NULL;
		DWORD len = 0;
		int physical_cores = 0, logical_cores = 0;

		GetLogicalProcessorInformation(info, &len);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return -1;

		info = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(len);

		if (GetLogicalProcessorInformation(info, &len)) {
			DWORD num = len / sizeof(*info);
			temp = info;

			for (DWORD i = 0; i < num; i++) {
				if (temp->Relationship == RelationProcessorCore) {
					ULONG_PTR mask = temp->ProcessorMask;

					physical_cores ++;
					logical_cores += num_logical_cores(mask);
				}

				temp++;
			}
		}

		free(info);

		return physical_cores;
	}


	/// ���CPUռ���� ��λ/�ٷֱ�
	int get_process_cpu_usage( void )
	{
		//cpu����
		static int processor_count_ = -1;
		//��һ�ε�ʱ��
		static UInt64_t last_time_ = 0;
		static UInt64_t last_system_time_ = 0;


		FILETIME now;
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;
		UInt64_t system_time;
		UInt64_t time;
		UInt64_t system_time_delta;
		UInt64_t time_delta;

		int cpu = -1;

		if(processor_count_ == -1)
		{
			processor_count_ = get_processor_number();
		}

	gt_start:
		GetSystemTimeAsFileTime(&now);

		if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
			&kernel_time, &user_time))
		{
			// We don't assert here because in some cases (such as in the Task Manager)
			// we may call this function on a process that has just exited but we have
			// not yet received the notification.
			return -1;
		}

		system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
		time = file_time_2_utc(&now);

		if ((last_system_time_ == 0) || (last_time_ == 0))
		{
			// First call, just set the last values.
			last_system_time_ = system_time;
			last_time_ = time;
			Sleep(200);
			goto gt_start;
		}

		system_time_delta = system_time - last_system_time_;
		time_delta = time - last_time_;

		assert(time_delta != 0);

		if (time_delta == 0)
			return -1;

		// We add time_delta / 2 so the result is rounded.
		cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
		last_system_time_ = system_time;
		last_time_ = time;
		return cpu;
	}

#if _MSC_VER >=1400	// VC2005��֧��intrin.h
#include <intrin.h>	// ����Intrinsics����
#endif

#if defined(_WIN64)
	// 64λ�²�֧���������. Ӧʹ��__cpuid��__cpuidex��Intrinsics������
#else
#if _MSC_VER < 1600	// VS2010. ��˵VC2008 SP1֮���֧��__cpuidex
	void __cpuidex(Int32 CPUInfo[4], Int32 InfoType, Int32 ECXValue)
	{
		if (NULL==CPUInfo)	return;
		_asm{
			// load. ��ȡ�������Ĵ���
			mov edi, CPUInfo;	// ׼����ediѰַCPUInfo
			mov eax, InfoType;
			mov ecx, ECXValue;
			// CPUID
			cpuid;
			// save. ���Ĵ������浽CPUInfo
			mov	[edi], eax;
			mov	[edi+4], ebx;
			mov	[edi+8], ecx;
			mov	[edi+12], edx;
		}
	}
#endif	// #if _MSC_VER < 1600	// VS2010. ��˵VC2008 SP1֮���֧��__cpuidex

#if _MSC_VER < 1400	// VC2005��֧��__cpuid
	void __cpuid(Int32 CPUInfo[4], Int32 InfoType)
	{
		__cpuidex(CPUInfo, InfoType, 0);
	}
#endif	// #if _MSC_VER < 1400	// VC2005��֧��__cpuid

#endif	// #if defined(_WIN64)

	//��ȡcpu���к�
	int get_cpu_serialnumber( char* pserial, int isize )
	{
		if( pserial == NULL || isize < sizeof(Int32) * 2 )
			return -1;

		Int32 dwBuf[4];
		// Function 0: Vendor-ID and Largest Standard Function
		__cpuidex(dwBuf, 1, 0);
		memcpy(pserial, (char*)&dwBuf[3], sizeof(Int32));
		memcpy(&pserial[sizeof(Int32)], (char*)&dwBuf[0], sizeof(Int32));
		return sizeof(Int32) * 2;
	}

	//��ȡcpu����
	int get_cpu_vendor( char* pvendor, int isize )
	{
		Int32 dwBuf[4];
		if( NULL == pvendor || isize < sizeof(Int32) * 3 )
			return -1;

		// Function 0: Vendor-ID and Largest Standard Function
		__cpuid(dwBuf, 0);
		memcpy(pvendor, (char*)&dwBuf[1], sizeof(Int32));
		memcpy(&pvendor[sizeof(Int32)], (char*)&dwBuf[3], sizeof(Int32));
		memcpy(&pvendor[sizeof(Int32) * 2], (char*)&dwBuf[2], sizeof(Int32));
		return sizeof(Int32) * 3;
	}

	//��ȡcpu����
	int get_cpu_brand( char* pbrand, int isize )
	{
		Int32 dwBuf[4];
		if( NULL == pbrand || isize < 49 )
			return -1;

		// Function 0x80000000: Largest Extended Function Number
		__cpuid(dwBuf, 0x80000000);
		if (dwBuf[0] < 0x80000004)
			return -1;

		// Function 80000002h,80000003h,80000004h: Processor Brand String
		__cpuid((Int32*)&pbrand[0], 0x80000002);	// ǰ16���ַ�
		__cpuid((Int32*)&pbrand[16], 0x80000003);	// �м�16���ַ�
		__cpuid((Int32*)&pbrand[32], 0x80000004);	// ���16���ַ�
		pbrand[48] = '\0';
		return 49;
	}

	int get_sys_cpu_usage( void )
	{
		BOOL res;
		FILETIME preidleTime;
		FILETIME prekernelTime;
		FILETIME preuserTime;
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
		::Sleep(1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(&preidleTime, &idleTime);
		int kernel = CompareFileTime(&prekernelTime, &kernelTime);
		int user = CompareFileTime(&preuserTime, &userTime);
		int cpu = (kernel + user - idle) * 100 / (kernel + user);
		int cpuidle = (idle) * 100 / (kernel + user);
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
		return cpu;
	}

	/// ��ȡ��ǰ�����ڴ�������ڴ�ʹ����������-1ʧ�ܣ�0�ɹ� ��λ/K
	int get_process_memory_usage( UInt64_t* mem, UInt64_t* vmem )
	{
		PROCESS_MEMORY_COUNTERS pmc;
		if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			if(mem) *mem = pmc.WorkingSetSize / 1024;
			if(vmem) *vmem = pmc.PagefileUsage / 1024;
			return 0;
		}
		return -1;
	}

	int get_process_io_bytes( UInt64_t* read_bytes, UInt64_t* write_bytes )
	{
		IO_COUNTERS io_counter;
		if(GetProcessIoCounters(GetCurrentProcess(), &io_counter))
		{
			if(read_bytes) *read_bytes = io_counter.ReadTransferCount;
			if(write_bytes) *write_bytes = io_counter.WriteTransferCount;
			return 0;
		}
		return -1;
	}

	//��ȡ�ڴ���Ϣ ��λ/K
	void get_memory_info( UInt64_t* total, UInt64_t* free )
	{
		MEMORYSTATUSEX		ms;

		ms.dwLength = sizeof(ms);

		GlobalMemoryStatusEx(&ms);
		*total = (UInt64_t)ms.ullTotalPhys / 1024;// 1048576;
		*free = (UInt64_t)ms.ullAvailPhys / 1024;// 1048576;
	}

	//ͨ����������ȡ����ID
	bool get_processid_by_name( const char* task_name, unsigned long& processId )
	{
		processId = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof pe;

		if (Process32First(hSnapshot, &pe))
		{
			do {
				if (strcmp(task_name, pe.szExeFile) == 0)
				{
					CloseHandle(hSnapshot);
					processId = pe.th32ProcessID;
					return true;
				}
			} while(Process32Next(hSnapshot, &pe));
		}

		CloseHandle(hSnapshot);
		return false;
	}

	bool get_name_by_processid( unsigned long pid, char *task_name )
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pid);
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);

		if (Process32First(hSnapshot, &pe))
		{
			strcpy(task_name, pe.szExeFile);
			CloseHandle(hSnapshot);
			return true;
		}

		CloseHandle(hSnapshot);
		return false;
	}

	int	get_networks_info( pfnNetworkInfo pFun, void* pUser )
	{
		//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
		PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
		//�õ��ṹ���С,����GetAdaptersInfo����
		unsigned long stSize = sizeof(IP_ADAPTER_INFO);
		//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
		int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
		//��¼��������
		int netCardNum = 0;
		//��¼ÿ�������ϵ�IP��ַ����
		int IPnumPerNetCard = 0;
		if (ERROR_BUFFER_OVERFLOW == nRel)
		{
			//����������ص���ERROR_BUFFER_OVERFLOW
			//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
			//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
			//�ͷ�ԭ�����ڴ�ռ�
			delete pIpAdapterInfo;
			//���������ڴ�ռ������洢����������Ϣ
			pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
			//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
			nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);
		}

		if (ERROR_SUCCESS == nRel)
		{
			PIP_ADAPTER_INFO Tmp = pIpAdapterInfo;
			//���������Ϣ
			//�����ж�����,���ͨ��ѭ��ȥ�ж�
			while (Tmp)
			{
				tagNetworks_INFO	Info = { 0 };
				Info.szName = Tmp->AdapterName;
				Info.szDescribe = Tmp->Description;
				if( Tmp->Type == 71)
				{//��������
					Info.Type = enNETWORKS_TYPE_WIRELESS;
				}
				else if( strstr(Tmp->Description,"Bluetooth") > 0 )
				{//����
					Info.Type = enNETWORKS_TYPE_BLUETOOTH;
				}
				else if( strstr(Tmp->Description,"PCI") > 0 )
				{//��������
					Info.Type = enNETWORKS_TYPE_WIRE;
				}
				else
				{
					Info.Type = enNETWORKS_TYPE_UNKNOW;
				}

				memcpy(&Info.Mac, Tmp->Address, Tmp->AddressLength);
				//���������ж�IP,���ͨ��ѭ��ȥ�ж�
				IP_ADDR_STRING *pIpAddrString =&(Tmp->IpAddressList);
				Info.szIP = pIpAddrString->IpAddress.String;
				if( pFun )
					pFun(&Info, pUser);

				Tmp = Tmp->Next;
			}
		}

		//�ͷ��ڴ�ռ�
		if (pIpAdapterInfo)
		{
			delete pIpAdapterInfo;
		}

		return 0;
	}

	int get_system_resource( const char* szFilePath )
	{
		return -1;
	}

	//����ϵͳ�ں�ת���ļ�����󳤶ȺͿ��Դ򿪵�socket����(linuxϵͳ��Ч)
	int set_core_and_file_max(int iCore, int iFile)
	{
		return -1;
	}

	bool is_admin_auth( void )
	{
		SID_IDENTIFIER_AUTHORITY auth = SECURITY_NT_AUTHORITY;
		PSID admin_group;
		BOOL success;

		success = AllocateAndInitializeSid(&auth, 2,
			SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0, &admin_group);
		if (success) 
		{
			if (!CheckTokenMembership(NULL, admin_group, &success))
				success = false;
			FreeSid(admin_group);
		}

		return success;
	}

	//��ȡ���̿ռ�
	int get_disk_space(const char* c_szDir, tagSys_Disk_INFO* pInfo)
	{
		if (c_szDir == NULL)
			return -1;

		UInt64_t i64FreeBytesToCaller;
		UInt64_t i64TotalBytes;
		UInt64_t i64FreeBytes;
		bool fResult = GetDiskFreeSpaceEx(
			c_szDir,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);

		if (!fResult)
			return -1;

		pInfo->uTotalSize = i64TotalBytes;
		pInfo->uFreeSize = i64FreeBytes;
		return 1;
	}

	//��ȡ������ϸ��Ϣ
	int get_disk_detail(fpnGetDiskDetailInfo pFunc)
	{
		return GetDiskInfo_WMI_20170823(pFunc);
	}

#ifdef  __cplusplus
}
#endif