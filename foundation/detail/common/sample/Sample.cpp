// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libCommon/modafx.h>

union time_data {
	FILETIME           ft;
	unsigned long long val;
};

struct os_cpu_usage_info {
	union time_data last_time, last_sys_time, last_user_time;
	DWORD core_count;
};

typedef struct os_cpu_usage_info os_cpu_usage_info_t;

os_cpu_usage_info_t *os_cpu_usage_info_start(void)
{
	struct os_cpu_usage_info *info = (os_cpu_usage_info*)malloc(sizeof(*info));
	memset(info, 0, sizeof(*info));

	SYSTEM_INFO           si;
	FILETIME              dummy;

	GetSystemInfo(&si);
	GetSystemTimeAsFileTime(&info->last_time.ft);
	GetProcessTimes(GetCurrentProcess(), &dummy, &dummy,
		&info->last_sys_time.ft, &info->last_user_time.ft);
	info->core_count = si.dwNumberOfProcessors;

	return info;
}

double os_cpu_usage_info_query( os_cpu_usage_info_t *info )
{
	union time_data cur_time, cur_sys_time, cur_user_time;
	FILETIME        dummy;
	double          percent;
	//os_cpu_usage_info_t *info = os_cpu_usage_info_start();

	if (!info)
		return 0.0;

	GetSystemTimeAsFileTime(&cur_time.ft);
	GetProcessTimes(GetCurrentProcess(), &dummy, &dummy,
		&cur_sys_time.ft, &cur_user_time.ft);

	percent = (double)(cur_sys_time.val - info->last_sys_time.val +
		(cur_user_time.val - info->last_user_time.val));
	percent /= (double)(cur_time.val - info->last_time.val);
	percent /= (double)info->core_count;

	info->last_time.val      = cur_time.val;
	info->last_sys_time.val  = cur_sys_time.val;
	info->last_user_time.val = cur_user_time.val;

	return percent * 100.0;
}


double os_cpu_usage( void )
{
	union time_data cur_time, cur_sys_time, cur_user_time;
	struct os_cpu_usage_info info = {0};
	SYSTEM_INFO           si;
	FILETIME              dummy;
	double				  percent;

	GetSystemInfo(&si);
	GetSystemTimeAsFileTime(&info.last_time.ft);
	GetProcessTimes(GetCurrentProcess(), &dummy, &dummy,
		&info.last_sys_time.ft, &info.last_user_time.ft);
	info.core_count = si.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&cur_time.ft);
	GetProcessTimes(GetCurrentProcess(), &dummy, &dummy,
		&cur_sys_time.ft, &cur_user_time.ft);

	percent = (double)(cur_sys_time.val - info.last_sys_time.val +
		(cur_user_time.val - info.last_user_time.val));
	percent /= (double)(cur_time.val - info.last_time.val);
	percent /= (double)info.core_count;

	info.last_time.val      = cur_time.val;
	info.last_sys_time.val  = cur_sys_time.val;
	info.last_user_time.val = cur_user_time.val;

	return percent * 100.0;
}

int main(int argc, char* argv[])
{
	os_cpu_usage_info_t* psw = os_cpu_usage_info_start();

	{
		printf("addmin:%s\r\n", is_admin_auth() ? "true" : "false");

		HANDLE Handle = GetCurrentProcess();

		HANDLE hProcess = NULL;
		DuplicateHandle(GetCurrentProcess(),
			GetCurrentProcess(),
			GetCurrentProcess(),
			&hProcess,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS
			);

		int ierror = GetLastError();

		union time_data cur_time, cur_sys_time, cur_user_time;
		FILETIME        dummy;
		double          percent;
		GetProcessTimes(GetCurrentProcess(), &dummy, &dummy,
			&cur_sys_time.ft, &cur_user_time.ft);


		GetProcessTimes(hProcess, &dummy, &dummy,
			&cur_sys_time.ft, &cur_user_time.ft);

		int i ;
	}
	
	int iCo = GetTickCount();
	usleep(5000 * 1000);
	int lw323 = GetTickCount() - iCo;
	printf("11111:%d\r\n", lw323);
	double dl123 = os_cpu_usage_info_query(psw);

	usleep(10);
	struct timeval_t tv;
	gettimeofday(&tv, NULL);

	usleep(5000 * 1000);

	struct timeval_t tv1;
	gettimeofday(&tv1, NULL);

	long lw3 = (( tv1.tv_sec * 1000000 + tv1.tv_usec ) - (tv.tv_sec * 1000000 + tv.tv_usec) ) / 1000;

	char szl[1024] = {0};
	//������
	{
		int yl = get_process_cpu_usage();
		Sleep(1000);
	}
	get_cpu_serialnumber(szl, sizeof(szl));

	char szvendor[1024] = {0};
	get_cpu_vendor(szvendor, sizeof(szvendor));

	char szbrand[1024] = {0};
	get_cpu_brand(szbrand, sizeof(szbrand));

	int num = get_processor_number();

	int ro = get_physical_processor_number();

	UInt64 uTotal = 0, uFree = 0;
	get_process_memory_usage(&uTotal, &uFree);

	get_memory_info(&uTotal, &uFree);

	std::cin.get();
	return 0;
}