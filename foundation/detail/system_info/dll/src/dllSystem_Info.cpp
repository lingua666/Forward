
#include <libCommon/SysInfo.h>
#include <libCommon/os/Platform.h>
#include <libString/string.hpp>
#include <libString/string.hpp>
#include <dllSystem_Info/dllSystem_Info.h>

tagCpu_INFO g_20170724tagCpu_INFO = {0};

/*****************************************************************
/*函数说明：	获取CPU信息
/*参数说明：	
/*				
/*返回值：		CPU信息结构体
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS tagCpu_INFO* _CALLTYPE GetCpuInfo( void )
{
	if( g_20170724tagCpu_INFO.pSerialNumber == NULL )
	{
		g_20170724tagCpu_INFO.pSerialNumber = new char[50];
		memset(g_20170724tagCpu_INFO.pSerialNumber, 0, 50);

		char Serial[50] = {0};
		get_cpu_serialnumber(Serial, 50);

		//strcpy(g_20170724tagCpu_INFO.pSerialNumber, _string_type::HexToStr(Serial, 8).c_str());

		sprintf(g_20170724tagCpu_INFO.pSerialNumber, "%08X%08X", *((Int32*)&Serial[0]), *((Int32*)&Serial[sizeof(Int32)]));
	}

	if( g_20170724tagCpu_INFO.pVendor == NULL )
	{
		g_20170724tagCpu_INFO.pVendor = new char[50];
		memset(g_20170724tagCpu_INFO.pVendor, 0, 50);
		get_cpu_vendor(g_20170724tagCpu_INFO.pVendor, 50);
	}

	if( g_20170724tagCpu_INFO.pBrand == NULL )
	{
		g_20170724tagCpu_INFO.pBrand = new char[50];
		memset(g_20170724tagCpu_INFO.pBrand, 0, 50);
		get_cpu_brand(g_20170724tagCpu_INFO.pBrand, 50);
	}

	return &g_20170724tagCpu_INFO;
}

///////////////////硬盘信息////////////////////

#if defined(PLATFORM_OS_FAMILY_WINDOWS)

#include <comdef.h>
#include <atlbase.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

int GetDiskInfo_WMI_20170823( fpnMGetDiskInfo pFunc )
{
	HRESULT hres;

	// Initialize COM. ------------------------------------------
	hres =  CoInitialize(0);//CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		return -1;                  // Program has failed.
	}
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------
	hres =  CoInitializeSecurity(
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
		IID_IWbemLocator, (LPVOID *) &pLoc);

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

	tagDisk_INFO DiskINFO = {0};
	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);
		if(0 == uReturn)
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

		if( pFunc )
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
#else
	int GetDiskInfo_WMI_20170823( fpnMGetDiskInfo pFunc )
	{ return -1; }
#endif

/*****************************************************************
/*函数说明：	获取硬盘信息
/*参数说明：	pFunc:  硬盘信息回调函数
/*				
/*返回值：		CPU信息结构体
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MGetDiskInfo( fpnMGetDiskInfo pFunc )
{
	GetDiskInfo_WMI_20170823(pFunc);
}