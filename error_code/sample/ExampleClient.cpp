// BluetoothTest.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// for WSALookupServiceBegin() WSALookupServiceNext etc.
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <ws2bth.h>
#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <BthLEDef.h>
#include <BluetoothAPIs.h>
#include <bluetoothleapis.h>
//#include <Cfgmgr32.h>

#pragma comment(lib, "SetupAPI")
#pragma comment(lib,"Bthprops.lib")  
#pragma comment(lib, "BluetoothAPIs.lib")
//#pragma comment(lib, "Cfgmgr32.lib")

//#define TO_SEARCH_DEVICE_UUID "{0000180D-0000-1000-8000-00805F9B34FB}" //we use UUID for an HR BLE device 

//GUID查看注册表HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\DeviceClasses
GUID g_BluetoothInterfaceGUID = { 0x00001800, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };

//GUID g_BluetoothInterfaceGUID = { 0x0000180A, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };

//this is the notification function
//the way ValueChangedEventParameters is utilized is shown in
//HealthHeartRateService::HeartRateMeasurementEvent()
//a function in Windows Driver Kit (WDK) 8.0 Samples.zip\C++\WDK 8.0 Samples\Bluetooth Low Energy (LE) Generic Attribute (GATT) Profile Drivers\Solution\WpdHealthHeartRate\HealthHeartRateService.cpp
void SomethingHappened( BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
	printf("notification obtained ");
	PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;    

	HRESULT hr;
	if (0 == ValueChangedEventParameters->CharacteristicValue->DataSize) {
		hr = E_FAIL;
		printf("datasize 0\n");
	} else {
		printf("HR ");
		//for(int i=0; i<ValueChangedEventParameters->CharacteristicValue->DataSize;i++) {
		//	printf("%0x",ValueChangedEventParameters->CharacteristicValue->Data[i]);
		//}
		// if the first bit is set, then the value is the next 2 bytes.  If it is clear, the value is in the next byte
		//The Heart Rate Value Format bit (bit 0 of the Flags field) indicates if the data format of 
		//the Heart Rate Measurement Value field is in a format of UINT8 or UINT16. 
		//When the Heart Rate Value format is sent in a UINT8 format, the Heart Rate Value 
		//Format bit shall be set to 0. When the Heart Rate Value format is sent in a UINT16 
		//format, the Heart Rate Value Format bit shall be set to 1
		//from this PDF https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=239866
		unsigned heart_rate;
		if (0x01 == (ValueChangedEventParameters->CharacteristicValue->Data[0] & 0x01)) {
			heart_rate = ValueChangedEventParameters->CharacteristicValue->Data[1]*256 + ValueChangedEventParameters->CharacteristicValue->Data[2];
		} else {
			heart_rate = ValueChangedEventParameters->CharacteristicValue->Data[1];
		}
		printf("%d\n", heart_rate);
	}
}

//this function works to get a handle for a BLE device based on its GUID
//copied from http://social.msdn.microsoft.com/Forums/windowshardware/en-US/e5e1058d-5a64-4e60-b8e2-0ce327c13058/erroraccessdenied-error-when-trying-to-receive-data-from-bluetooth-low-energy-devices?forum=wdk
//credits to Andrey_sh
HANDLE GetBLEHandle(__in GUID AGuid)
{
	HDEVINFO hDI;
	SP_DEVICE_INTERFACE_DATA did = {0};
	SP_DEVINFO_DATA dd = {0};
	GUID BluetoothInterfaceGUID = AGuid;
	HANDLE hComm = NULL;

	//hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_ALLCLASSES/*DIGCF_DEVICEINTERFACE*/ | DIGCF_PRESENT);
	//GUID TmpGUID = { 0x00001812, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };
	hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	if( hDI == INVALID_HANDLE_VALUE ) return NULL;

	did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	dd.cbSize = sizeof(SP_DEVINFO_DATA);

	for(DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, i, &did); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD size = 0;

		if(!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0) )
		{
			int err = GetLastError();

			if( err == ERROR_NO_MORE_ITEMS ) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR , size);

			pInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);

			if( !SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd) )
				break;

			hComm = CreateFileA(
				pInterfaceDetailData->DevicePath,
				0/*GENERIC_WRITE | GENERIC_READ*/,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

			GlobalFree(pInterfaceDetailData);

			int iErr = GetLastError();
			printf("iErr:%d\r\n", iErr);
			iErr ++;
		}
	}

	SetupDiDestroyDeviceInfoList(hDI);
	return hComm;
}

int enum_usb_device_info(GUID uid)
{
	int i = 0;
	int res = 0;
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData = { sizeof(DeviceInfoData) };

	// get device class information handle
	hDevInfo = SetupDiGetClassDevs(&uid, 0, 0, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		res = GetLastError();
		return res;
	}

	HDEVINFO hDevInfo1;
	// get device class information handle
	hDevInfo1 = SetupDiGetClassDevs(&uid, 0, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (hDevInfo1 == INVALID_HANDLE_VALUE)
	{
		res = GetLastError();
		return res;
	}

	// enumerute device information
	DWORD required_size = 0;
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
	{
		SP_DEVICE_INTERFACE_DATA did = { 0 };
		did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		int iRet = SetupDiEnumDeviceInterfaces(hDevInfo1, &DeviceInfoData, NULL, 0, &did);
		int iErr = GetLastError();
		

		DWORD DataT;
		char friendly_name[2046] = { 0 };
		char desc[2046] = { 0 };
		char mac[2046] = { 0 };
		char path[2046] = { 0 };
		DWORD buffersize = 2046;
		DWORD req_bufsize = 0;

		/*SP_DEVICE_INTERFACE_DATA did;
		DWORD size = 0;
		if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &did, NULL, 0, &size, 0))
		{
			int err = GetLastError();

			if (err == ERROR_NO_MORE_ITEMS) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

			pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			SP_DEVINFO_DATA dd;
			dd.cbSize = sizeof(SP_DEVINFO_DATA);
			if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &did, pInterfaceDetailData, size, &size, &dd))
				break;
		}*/

		// get device description information
		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, &DataT, (LPBYTE)desc, buffersize, &req_bufsize))
		{
			res = GetLastError();
			continue;
		}

		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, &DataT, (LPBYTE)friendly_name, buffersize, &req_bufsize))
		{
			res = GetLastError();
			//continue;
		}

		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_CHARACTERISTICS, &DataT, (LPBYTE)mac, buffersize, &req_bufsize))
		{
			res = GetLastError();
			//continue;
		}

		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_LOCATION_PATHS, &DataT, (LPBYTE)path, buffersize, &req_bufsize))
		{
			res = GetLastError();
			//continue;
		}

		char temp[512] = { 0 };
		sprintf_s(temp, 512, "device %d: desc:%s, Name:%s, mac:%s, path:%s", i, desc, friendly_name, mac, path);
		puts(temp);
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	return 0;
}

int main( int argc, char *argv[ ], char *envp[ ] )
{
	//WORD iw = CM_Get_Version();

	//Step 1: find the BLE device handle from its GUID
	GUID AGuid = g_BluetoothInterfaceGUID;

	DWORD dwLen = sizeof(GUID);
	SetupDiClassGuidsFromName("Bluetooth", &AGuid, dwLen, &dwLen);
	enum_usb_device_info(g_BluetoothInterfaceGUID);
	//now get the handle 
	HANDLE hLEDevice = GetBLEHandle(g_BluetoothInterfaceGUID);// GetBLEHandle(AGuid);


	//Step 2: Get a list of services that the device advertises
	// first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
	// services in serviceBufferCount
	USHORT serviceBufferCount;
	////////////////////////////////////////////////////////////////////////////
	// Determine Services Buffer Size
	////////////////////////////////////////////////////////////////////////////

	HRESULT hr = BluetoothGATTGetServices(
		hLEDevice,
		0,
		NULL,
		&serviceBufferCount,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
		printf("BluetoothGATTGetServices - Buffer Size %d", hr);
	}

	PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)
		malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);

	if (NULL == pServiceBuffer) {
		printf("pServiceBuffer out of memory\r\n");
	} else {
		RtlZeroMemory(pServiceBuffer, 
			sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
	}

	////////////////////////////////////////////////////////////////////////////
	// Retrieve Services
	////////////////////////////////////////////////////////////////////////////

	USHORT numServices;
	hr = BluetoothGATTGetServices(
		hLEDevice,
		serviceBufferCount,
		pServiceBuffer,
		&numServices,
		BLUETOOTH_GATT_FLAG_NONE);

	if (S_OK != hr) {
		printf("BluetoothGATTGetServices - Buffer Size %d", hr);
	}


	//Step 3: now get the list of charactersitics. note how the pServiceBuffer is required from step 2
	////////////////////////////////////////////////////////////////////////////
	// Determine Characteristic Buffer Size
	////////////////////////////////////////////////////////////////////////////

	USHORT charBufferSize;
	hr = BluetoothGATTGetCharacteristics(
		hLEDevice,
		pServiceBuffer,
		0,
		NULL,
		&charBufferSize,
		BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
		printf("BluetoothGATTGetCharacteristics - Buffer Size %d", hr);
	}

	PBTH_LE_GATT_CHARACTERISTIC pCharBuffer = {0};
	if (charBufferSize > 0) {
		pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)
			malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));

		if (NULL == pCharBuffer) {
			printf("pCharBuffer out of memory\r\n");
		} else {
			RtlZeroMemory(pCharBuffer, 
				charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
		}

		////////////////////////////////////////////////////////////////////////////
		// Retrieve Characteristics
		////////////////////////////////////////////////////////////////////////////
		USHORT numChars;
		hr = BluetoothGATTGetCharacteristics(
			hLEDevice,
			pServiceBuffer,
			charBufferSize,
			pCharBuffer,
			&numChars,
			BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr) {
			printf("BluetoothGATTGetCharacteristics - Actual Data %d", hr);
		}

		if (numChars != charBufferSize) {
			printf("buffer size and buffer size actual size mismatch\r\n");
		}
	}


	//Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
	//descriptors are required as we descriptors that are notification based will have to be written
	//once IsSubcribeToNotification set to true, we set the appropriate callback function
	//need for setting descriptors for notification according to
	//http://social.msdn.microsoft.com/Forums/en-US/11d3a7ce-182b-4190-bf9d-64fefc3328d9/windows-bluetooth-le-apis-event-callbacks?forum=wdk
	PBTH_LE_GATT_CHARACTERISTIC currGattChar = {0};
	for (int ii=0; ii <charBufferSize; ii++) {
		currGattChar = &pCharBuffer[ii];
		USHORT charValueDataSize;
		PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;


		///////////////////////////////////////////////////////////////////////////
		// Determine Descriptor Buffer Size
		////////////////////////////////////////////////////////////////////////////
		USHORT descriptorBufferSize;
		hr = BluetoothGATTGetDescriptors(
			hLEDevice,
			currGattChar,
			0,
			NULL,
			&descriptorBufferSize,
			BLUETOOTH_GATT_FLAG_NONE);

		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
			printf("BluetoothGATTGetDescriptors - Buffer Size %d", hr);
		}

		PBTH_LE_GATT_DESCRIPTOR pDescriptorBuffer;
		if (descriptorBufferSize > 0) {
			pDescriptorBuffer = (PBTH_LE_GATT_DESCRIPTOR)
				malloc(descriptorBufferSize 
				* sizeof(BTH_LE_GATT_DESCRIPTOR));

			if (NULL == pDescriptorBuffer) {
				printf("pDescriptorBuffer out of memory\r\n");
			} else {
				RtlZeroMemory(pDescriptorBuffer, descriptorBufferSize);
			}

			////////////////////////////////////////////////////////////////////////////
			// Retrieve Descriptors
			////////////////////////////////////////////////////////////////////////////

			USHORT numDescriptors;
			hr = BluetoothGATTGetDescriptors(
				hLEDevice,
				currGattChar,
				descriptorBufferSize,
				pDescriptorBuffer,
				&numDescriptors,
				BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr) {
				printf("BluetoothGATTGetDescriptors - Actual Data %d", hr);
			}

			if (numDescriptors != descriptorBufferSize) {
				printf("buffer size and buffer size actual size mismatch\r\n");
			}

			for(int kk=0; kk<numDescriptors; kk++) {
				PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[kk];
				////////////////////////////////////////////////////////////////////////////
				// Determine Descriptor Value Buffer Size
				////////////////////////////////////////////////////////////////////////////
				USHORT descValueDataSize;
				hr = BluetoothGATTGetDescriptorValue(
					hLEDevice,
					currGattDescriptor,
					0,
					NULL,
					&descValueDataSize,
					BLUETOOTH_GATT_FLAG_NONE);

				if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
					printf("BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
				}

				PBTH_LE_GATT_DESCRIPTOR_VALUE pDescValueBuffer = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);

				if (NULL == pDescValueBuffer) {
					printf("pDescValueBuffer out of memory\r\n");
				} else {
					RtlZeroMemory(pDescValueBuffer, descValueDataSize);
				}

				////////////////////////////////////////////////////////////////////////////
				// Retrieve the Descriptor Value
				////////////////////////////////////////////////////////////////////////////

				hr = BluetoothGATTGetDescriptorValue(
					hLEDevice,
					currGattDescriptor,
					(ULONG)descValueDataSize,
					pDescValueBuffer,
					NULL,
					BLUETOOTH_GATT_FLAG_NONE);
				if (S_OK != hr) {
					printf("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
				}
				//you may also get a descriptor that is read (and not notify) andi am guessing the attribute handle is out of limits
				// we set all descriptors that are notifiable to notify us via IsSubstcibeToNotification
				if(currGattDescriptor->AttributeHandle < 255) {
					BTH_LE_GATT_DESCRIPTOR_VALUE newValue;

					RtlZeroMemory(&newValue, sizeof(newValue));

					newValue.DescriptorType = ClientCharacteristicConfiguration;
					newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

					hr = BluetoothGATTSetDescriptorValue(
						hLEDevice,
						currGattDescriptor,
						&newValue,
						BLUETOOTH_GATT_FLAG_NONE);
					if (S_OK != hr) {
						printf("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
					} else {
						printf("setting notification for serivice handle %d\n", currGattDescriptor->ServiceHandle);
					}

				}

			}
		} 


		//set the appropriate callback function when the descriptor change value
		BLUETOOTH_GATT_EVENT_HANDLE EventHandle;

		if (currGattChar->IsNotifiable) {
			printf("Setting Notification for ServiceHandle %d\n",currGattChar->ServiceHandle);
			BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;

			BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn;
			EventParameterIn.Characteristics[0] = *currGattChar;
			EventParameterIn.NumCharacteristics = 1;
			hr = BluetoothGATTRegisterEvent(
				hLEDevice,
				EventType,
				&EventParameterIn,
				SomethingHappened,
				NULL,
				&EventHandle,
				BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr) {
				printf("BluetoothGATTRegisterEvent - Actual Data %d", hr);
			}
		}


		if (currGattChar->IsReadable) {//currGattChar->IsReadable
			////////////////////////////////////////////////////////////////////////////
			// Determine Characteristic Value Buffer Size
			////////////////////////////////////////////////////////////////////////////
			hr = BluetoothGATTGetCharacteristicValue(
				hLEDevice,
				currGattChar,
				0,
				NULL,
				&charValueDataSize,
				BLUETOOTH_GATT_FLAG_NONE);

			if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
				printf("BluetoothGATTGetCharacteristicValue - Buffer Size %d", hr);
			}

			pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)malloc(charValueDataSize);

			if (NULL == pCharValueBuffer) {
				printf("pCharValueBuffer out of memory\r\n");
			} else {
				RtlZeroMemory(pCharValueBuffer, charValueDataSize);
			}

			////////////////////////////////////////////////////////////////////////////
			// Retrieve the Characteristic Value
			////////////////////////////////////////////////////////////////////////////

			hr = BluetoothGATTGetCharacteristicValue(
				hLEDevice,
				currGattChar,
				(ULONG)charValueDataSize,
				pCharValueBuffer,
				NULL,
				BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr) {
				printf("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
			}

			//print the characeteristic Value
			//for an HR monitor this might be the body sensor location
			printf("\n Printing a read (not notifiable) characterstic (maybe) body sensor value");
			for(int iii=0; iii< pCharValueBuffer->DataSize; iii++) {// ideally check ->DataSize before printing
				printf("%d",pCharValueBuffer->Data[iii]); 
			}
			printf("\n");


			// Free before going to next iteration, or memory leak.
			free(pCharValueBuffer);
			pCharValueBuffer = NULL;
		}

	}

	//go into an inf loop that sleeps. you will ideally see notifications from the HR device
	while(1){
		Sleep(1000);
		//printf("look for notification");
	}

	CloseHandle(hLEDevice);



	if ( GetLastError()!=NO_ERROR &&
		GetLastError()!=ERROR_NO_MORE_ITEMS )
	{
		// Insert error handling here.
		return 1;
	}

	return 0;
}


#include <conio.h>    
#include <iostream>    
#include <string>    
#include <locale>
using namespace std;

void CollectUSBInfo()
{
	char *pstr;
	int idindex = 0;
	// pstr=T2A(str);
	DWORD dwFlag = (DIGCF_ALLCLASSES | DIGCF_PRESENT);
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, dwFlag);
	if (INVALID_HANDLE_VALUE == hDevInfo)
	{
		printf("获取系统设备列表失败");

		return;
	}

	// 准备遍历所有设备查找USB
	SP_DEVINFO_DATA sDevInfoData;
	sDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	char szDIS[MAX_PATH]; // Device Identification Strings, 
	DWORD nSize = 0;
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &sDevInfoData); i++)
	{
		nSize = 0;
		if (!SetupDiGetDeviceInstanceId(hDevInfo, &sDevInfoData, szDIS, sizeof(szDIS), &nSize))
		{
			printf("获取设备识别字符串失败");

			break;
		}

		printf("%s\r\n", szDIS);
	}

	// 释放设备
	SetupDiDestroyDeviceInfoList(hDevInfo);
}

int main222(void)
{
	//CollectUSBInfo();

	HBLUETOOTH_RADIO_FIND hbf = NULL;
	HANDLE hbr = NULL;
	HBLUETOOTH_DEVICE_FIND hbdf = NULL;
	BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) }; //调用BluetoothFindFirstDevice搜索本机蓝牙收发器所需要的搜索参数对象   
	BLUETOOTH_RADIO_INFO bri = { sizeof(BLUETOOTH_RADIO_INFO) }; //初始化一个储存蓝牙收发器信息（BLUETOOTH_RADIO_INFO）的对象bri  
	BLUETOOTH_DEVICE_SEARCH_PARAMS btsp = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };//调用BluetoothFindFirstDevice搜索本所需要的搜索参数对象   
	BLUETOOTH_DEVICE_INFO btdi = { sizeof(BLUETOOTH_DEVICE_INFO) };  //初始化一个远程蓝牙设备信息（BLUETOOTH_DEVICE_INFO）对象btdi，以储存搜索到的蓝牙设备信息  
	hbf = BluetoothFindFirstRadio(&btfrp, &hbr); //得到第一个被枚举的蓝牙收发器的句柄hbf可用于BluetoothFindNextRadio，hbr可用于BluetoothFindFirstDevice。若没有找到本机的蓝牙收发器，则得到的句柄hbf=NULL，具体可参考https://msdn.microsoft.com/en-us/library/aa362786(v=vs.85).aspx   

	bool brfind = hbf != NULL;
	while (brfind)
	{
		if (BluetoothGetRadioInfo(hbr, &bri) == ERROR_SUCCESS)//获取蓝牙收发器的信息，储存在bri中    
		{
			cout << "Class of device: 0x" << uppercase << hex << bri.ulClassofDevice << endl;
			wcout << "Name:" << bri.szName << endl;  //蓝牙收发器的名字  
			cout << "Manufacture:0x" << uppercase << hex << bri.manufacturer << endl;
			cout << "Subversion:0x" << uppercase << hex << bri.lmpSubversion << endl;
			//    
			btsp.hRadio = hbr;  //设置执行搜索设备所在的句柄，应设为执行BluetoothFindFirstRadio函数所得到的句柄  
			btsp.fReturnAuthenticated = TRUE;//是否搜索已配对的设备    
			btsp.fReturnConnected = FALSE;//是否搜索已连接的设备    
			btsp.fReturnRemembered = TRUE;//是否搜索已记忆的设备    
			btsp.fReturnUnknown = TRUE;//是否搜索未知设备    
			btsp.fIssueInquiry = TRUE;//是否重新搜索，True的时候会执行新的搜索，时间较长，FALSE的时候会直接返回上次的搜索结果。  
			btsp.cTimeoutMultiplier = 30;//指示查询超时的值，以1.28秒为增量。 例如，12.8秒的查询的cTimeoutMultiplier值为10.此成员的最大值为48.当使用大于48的值时，调用函数立即失败并返回   
			hbdf = BluetoothFindFirstDevice(&btsp, &btdi);//通过找到第一个设备得到的HBLUETOOTH_DEVICE_FIND句柄hbdf来枚举远程蓝牙设备，搜到的第一个远程蓝牙设备的信息储存在btdi对象中。若没有远程蓝牙设备，hdbf=NULL。    
			bool bfind = hbdf != NULL;
			while (bfind)
			{
				wcout << "[Name]:" << btdi.szName;  //远程蓝牙设备的名字  
				cout << ",[Address]:0x" << uppercase << hex << btdi.Address.ullLong << endl;
				bfind = BluetoothFindNextDevice(hbdf, &btdi);//通过BluetoothFindFirstDevice得到的HBLUETOOTH_DEVICE_FIND句柄来枚举搜索下一个远程蓝牙设备，并将远程蓝牙设备的信息储存在btdi中    
			}
			BluetoothFindDeviceClose(hbdf);//使用完后记得关闭HBLUETOOTH_DEVICE_FIND句柄hbdf。    
		}
		CloseHandle(hbr);
		brfind = BluetoothFindNextRadio(hbf, &hbr);//通过BluetoothFindFirstRadio得到的HBLUETOOTH_RADIO_FIND句柄hbf来枚举搜索下一个本地蓝牙收发器，得到可用于BluetoothFindFirstDevice的句柄hbr。      
		if (!brfind)
		{
			hbf = BluetoothFindFirstRadio(&btfrp, &hbr); //得到第一个被枚举的蓝牙收发器的句柄hbf可用于BluetoothFindNextRadio，hbr可用于BluetoothFindFirstDevice。若没有找到本机的蓝牙收发器，则得到的句柄hbf=NULL，具体可参考https://msdn.microsoft.com/en-us/library/aa362786(v=vs.85).aspx   
			brfind = true;
		}
	}
	BluetoothFindRadioClose(hbf);//使用完后记得关闭HBLUETOOTH_RADIO_FIND句柄hbf。    
	cout << "All listed";
	_getch();
	return 0;
}

// for BluetoothFindFirstDevice() BluetoothFindNextDevice() etc.
#include <bthsdpdef.h>
#include <BluetoothAPIs.h>
#pragma comment ( lib, "Bthprops.lib" )
#pragma comment ( lib, "Irprops.lib" )

char* g_Bluetooth = "X.Pen";//"Honor 9";//

#ifndef BTH_ADDR
typedef ULONGLONG BTH_ADDR;
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(pObj) {if(pObj) delete pObj; pObj = NULL;}
#endif

#ifndef SAFE_DELETERG
#define SAFE_DELETERG(pObj) {if(pObj) delete [] pObj; pObj = NULL;}
#endif

#define RECEIVE_OVER_COMMAND	0x00001000

typedef struct _tag_BthDev_Info
{
	BTH_ADDR btAddr;
	GUID     serviceClassId;
	int iPort;
	char szAddr[32];
	char szName[128];

	_tag_BthDev_Info()
	{
		memset(this, 0, sizeof(this));
	}
}
BTHDEV_INFO;

class CGGBlueTooth
{
public:
	CGGBlueTooth();
	~CGGBlueTooth();

	BOOL StartSearchBthDev(IN DWORD dwControlFlags);
	BOOL GetNextBthDev(
		IN OUT BTHDEV_INFO *pcBthDevInfo,
		IN DWORD dwControlFlags = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_FLUSHCACHE
	);
	BOOL EndSearchBthDev();

	//Socket Api
	BOOL Create(int af = AF_BTH, int type = SOCK_STREAM, int protocol = BTHPROTO_RFCOMM);
	BOOL Close();

	//If no error occurs, this function returns zero. If an error occurs, it returns SOCKET_ERROR
	int Bind(BTH_ADDR sktAddr = 0, ULONG port = BT_PORT_ANY);
	int Listen(IN int backlog);
	int GetSockName(BTH_ADDR *psktAddr, ULONG *port);
	int SetBthService(char *lpServiceName);
	int Accept(CGGBlueTooth *pcBth);
	int Connect(BTH_ADDR sktAddr, ULONG port = 0, int nMSecond = -1);
	int Send(LPVOID lpData, int len, int flags = 0);
	int Recv(LPVOID lpData, int len, int flags = 0);
	int SendAll(LPVOID lpData, int len, int flags = 0);
	int RecvAll(LPVOID lpData, int len, int flags = 0);

private:
	void HexMac2String(BTH_ADDR dw64Addr, char *pstrMac);

private:
	HANDLE m_hLookup;
	SOCKET m_hSocket;
	SOCKADDR_BTH m_sktAddr;
};


CGGBlueTooth::CGGBlueTooth()
	: m_hLookup(NULL)
	, m_hSocket(NULL)
{
	memset(&m_sktAddr, 0, sizeof(m_sktAddr));
}

CGGBlueTooth::~CGGBlueTooth()
{
	EndSearchBthDev();
	Close();
}

void CGGBlueTooth::HexMac2String(BTH_ADDR dw64Addr, char *pstrMac)
{
	BYTE *pbAddr = (BYTE*)&dw64Addr;

	sprintf(pstrMac, "%02X:%02X:%02X:%02X:%02X:%02X",
		pbAddr[5], pbAddr[4], pbAddr[3],
		pbAddr[2], pbAddr[1], pbAddr[0]
	);
}

BOOL CGGBlueTooth::StartSearchBthDev(IN DWORD dwControlFlags)
{
	WSAQUERYSETA wsaq;
	ZeroMemory(&wsaq, sizeof(WSAQUERYSETA));
	wsaq.dwSize = sizeof(WSAQUERYSETA);
	wsaq.dwNameSpace = NS_BTH;
	wsaq.lpcsaBuffer = NULL;

	return WSALookupServiceBeginA(&wsaq, dwControlFlags, &m_hLookup) == ERROR_SUCCESS ? TRUE : FALSE;
}


BOOL CGGBlueTooth::GetNextBthDev(
	IN OUT BTHDEV_INFO *pcBthDevInfo,
	IN DWORD dwControlFlags /* = LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_FLUSHCACHE*/
)
{
	if (!m_hLookup || !pcBthDevInfo)
	{
		return FALSE;
	}

	memset(pcBthDevInfo->szAddr, 0, sizeof(pcBthDevInfo->szAddr));
	memset(pcBthDevInfo->szName, 0, sizeof(pcBthDevInfo->szName));

	union
	{
		CHAR buf[5000];
		double __unused; // ensure proper alignment
	};
	LPWSAQUERYSETA pwsaResults = (LPWSAQUERYSETA)buf;
	DWORD dwSize = sizeof(buf);
	int nResult;

	ZeroMemory(pwsaResults, sizeof(WSAQUERYSETA));
	pwsaResults->dwSize = sizeof(WSAQUERYSETA);
	pwsaResults->dwNameSpace = NS_BTH;
	pwsaResults->lpBlob = NULL;

	while (true)
	{
		nResult = WSALookupServiceNextA(m_hLookup, dwControlFlags, &dwSize, pwsaResults);
		if (nResult == ERROR_SUCCESS)
		{
			BOOL bHaveName = pwsaResults->lpszServiceInstanceName && *(pwsaResults->lpszServiceInstanceName);
			printf("WSALookupServiceNextA() Name:%s\r\n", pwsaResults->lpszServiceInstanceName);
			if (bHaveName && g_Bluetooth == pwsaResults->lpszServiceInstanceName)
			{
				//test
				SOCKADDR_BTH *pp = (SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr;
				//test
				pcBthDevInfo->btAddr = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
				pcBthDevInfo->iPort = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->port;
				memcpy(&pcBthDevInfo->serviceClassId, &((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->serviceClassId, sizeof(GUID));
				HexMac2String(pcBthDevInfo->btAddr, pcBthDevInfo->szAddr);
				strcpy(pcBthDevInfo->szName, pwsaResults->lpszServiceInstanceName);
				return TRUE;
			}
		}
		else
		{
			break;
		}
		Sleep(0);
	}

	return FALSE;
}

BOOL CGGBlueTooth::EndSearchBthDev()
{
	if (m_hLookup)
	{
		WSALookupServiceEnd(m_hLookup);
		m_hLookup = NULL;
		return TRUE;
	}

	return FALSE;
}

///===============================

BOOL CGGBlueTooth::Create(int af/* = AF_BTH*/, int type/* = SOCK_STREAM*/, int protocol/* = BTHPROTO_RFCOMM*/)
{
	if (m_hSocket)
	{
		return FALSE;
	}

	m_hSocket = socket(af, type, protocol);
	m_sktAddr.addressFamily = af;
	m_sktAddr.serviceClassId = GUID_NULL;
	return m_hSocket == INVALID_SOCKET ? FALSE : TRUE;
}

BOOL CGGBlueTooth::Close()
{
	if (m_hSocket)
	{
		closesocket(m_hSocket);
		return TRUE;
	}
	return FALSE;
}

int CGGBlueTooth::Bind(BTH_ADDR sktAddr/* = 0*/, ULONG port/* = BT_PORT_ANY*/)
{
	m_sktAddr.btAddr = sktAddr;
	m_sktAddr.port = port;
	//m_sktAddr.serviceClassId = GUID_NULL;
	return bind(m_hSocket, (SOCKADDR *)&m_sktAddr, sizeof(m_sktAddr));
}

int CGGBlueTooth::Listen(int backlog)
{
	return listen(m_hSocket, backlog);
}

int CGGBlueTooth::GetSockName(BTH_ADDR *psktAddr, ULONG *port)
{
	int nLen = sizeof(m_sktAddr);
	int nResult = getsockname(m_hSocket, (SOCKADDR *)&m_sktAddr, &nLen);
	if (nResult == 0)
	{
		*psktAddr = m_sktAddr.btAddr;
		*port = m_sktAddr.port;
	}
	return nResult;
}

//串口传输GUID
GUID OBEXFileTransferServiceClass_UUID_EX = { 0x00001101, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };

//文件传输GUID
//GUID OBEXFileTransferServiceClass_UUID_EX = { 0x00001106, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB };
int CGGBlueTooth::SetBthService(char * lpServiceName)
{
	CSADDR_INFO sockInfo = { 0 };
	sockInfo.iProtocol = BTHPROTO_RFCOMM;
	sockInfo.iSocketType = SOCK_STREAM;
	sockInfo.LocalAddr.lpSockaddr = (SOCKADDR *)&m_sktAddr;
	sockInfo.LocalAddr.iSockaddrLength = sizeof(m_sktAddr);
	/*sockInfo.RemoteAddr.lpSockaddr = (SOCKADDR *)&m_sktAddr;
	sockInfo.RemoteAddr.iSockaddrLength = sizeof(m_sktAddr);*/

	WSAQUERYSETA svcInfo = { 0 };
	svcInfo.dwSize = sizeof(svcInfo);
	svcInfo.dwNameSpace = NS_BTH;
	svcInfo.lpszServiceInstanceName = "My Service";
	svcInfo.lpszComment = "Description of My Service";
	svcInfo.lpServiceClassId = &OBEXFileTransferServiceClass_UUID_EX;
	svcInfo.dwNumberOfCsAddrs = 1;
	svcInfo.lpcsaBuffer = &sockInfo;


	return WSASetServiceA(&svcInfo, RNRSERVICE_REGISTER, 0);
}

int CGGBlueTooth::Accept(CGGBlueTooth *pcBth)
{
	SOCKADDR_BTH ca;
	int size = sizeof(ca);
	pcBth->m_hSocket = accept(m_hSocket, (SOCKADDR *)&ca, &size);
	pcBth->m_sktAddr = ca;
	return pcBth->m_hSocket == INVALID_SOCKET ? SOCKET_ERROR : 0;
}

int CGGBlueTooth::Connect(BTH_ADDR sktAddr, ULONG port, int nMSecond/* = -1*/)
{
	SOCKADDR_BTH sa = { 0 };
	sa.addressFamily = AF_BTH;
	sa.btAddr = sktAddr;
	sa.port = port;
	if (sa.port == 0)
		sa.serviceClassId = OBEXFileTransferServiceClass_UUID_EX;

	if (nMSecond == -1)
	{
		return connect(m_hSocket, (SOCKADDR *)&sa, sizeof(SOCKADDR_BTH));
	}

	ULONG non_blocking = 1;
	ULONG blocking = 0;

	int nResult = ioctlsocket(m_hSocket, FIONBIO, &non_blocking);
	if (nResult == SOCKET_ERROR)
	{
		return nResult;
	}

	nResult = SOCKET_ERROR;
	if (connect(m_hSocket, (LPSOCKADDR)&sa, sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
	{
		struct timeval tv;
		fd_set writefds;

		// 设置连接超时时间
		tv.tv_sec = nMSecond / 1000; // 秒数
		tv.tv_usec = nMSecond % 1000; // 毫秒

		FD_ZERO(&writefds);
		FD_SET(m_hSocket, &writefds);

		nResult = select((int)m_hSocket + 1, NULL, &writefds, NULL, &tv);
		if (nResult > 0)
		{
			if (FD_ISSET(m_hSocket, &writefds))
			{
				int error = 0;
				int len = sizeof(error);
				//下面的一句一定要，主要针对防火墙 
				if (!(getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (char *)&error, &len) != 0 || error != 0))
				{
					nResult = 0;
				}
			}
		}
		else if (nResult == 0)
		{
			nResult = -2;
		}
	}

	if (ioctlsocket(m_hSocket, FIONBIO, &blocking) == SOCKET_ERROR)
	{
		nResult = SOCKET_ERROR;
	}
	return nResult;
}

int CGGBlueTooth::Send(LPVOID lpData, int len, int flags/* = 0*/)
{
	return send(m_hSocket, (char *)lpData, len, flags);
}

int CGGBlueTooth::Recv(LPVOID lpData, int len, int flags/* = 0*/)
{
	return recv(m_hSocket, (char *)lpData, len, flags);
}

int CGGBlueTooth::SendAll(LPVOID lpData, int len, int flags/* = 0*/)
{
	int nCount = send(m_hSocket, (char *)lpData, len, flags);
	if (nCount == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return SOCKET_ERROR;
	}

	int nCommand = 0;
	nCount = recv(m_hSocket, (char *)&nCommand, 4, flags);
	if (nCount != 4 || nCommand != RECEIVE_OVER_COMMAND)
	{
		return SOCKET_ERROR;
	}

	return ERROR_SUCCESS;
}

int CGGBlueTooth::RecvAll(LPVOID lpData, int len, int flags/* = 0*/)
{
	int nCount = -1, nCurRecv = 0, nMaxRead = 32 * 1024;

	while (nCurRecv < len)
	{
		if (len - nCurRecv < nMaxRead)
		{
			nMaxRead = len - nCurRecv;
		}

		nCount = recv(m_hSocket, (char *)lpData + nCurRecv, nMaxRead, flags);

		if (nCount == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				Sleep(1);
				continue;
			}
			else
			{
				return SOCKET_ERROR;
			}
		}

		nCurRecv += nCount;
	}

	int nCommand = RECEIVE_OVER_COMMAND;
	nCount = send(m_hSocket, (char *)&nCommand, 4, flags);
	if (nCount == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		return SOCKET_ERROR;
	}

	return ERROR_SUCCESS;
}

void ClientTest(void)
{
	BOOL isRet = FALSE;
	CGGBlueTooth Blue;
	BTHDEV_INFO BthDevInfo;
	while (true)
	{
		isRet = Blue.StartSearchBthDev(LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_ADDR);
		isRet = Blue.GetNextBthDev(&BthDevInfo, LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_ADDR);
		isRet = Blue.EndSearchBthDev();
		/*if (BthDevInfo.btAddr != 0)
		{
			break;
		}*/
	}

	isRet = Blue.Create();

	int iPort = 0;
	while (true)
	{
		isRet = Blue.Connect(BthDevInfo.btAddr, iPort);
		int iErr = GetLastError();
		if (iErr == 0)
			break;
		Sleep(1000);
		printf("iErr:%d,%d\r\n", iErr, iPort);
		if (iPort < 32)
			iPort++;
	}

	char cData[100] = { 0 };
	while (true)
	{
		int iRet = Blue.Recv(cData, sizeof(cData));
		//printf("Recvice(%d,%d):%s\r\n", GetLastError(), iRet, _string_type::HexToStr(_string_type(cData, iRet)).c_str());
		Sleep(1);
	}

	std::cin.get();
}

int main333(int argc, char* argv[])
{
	WSADATA wsd;
	WSAStartup(MAKEWORD(1, 0), &wsd);

	ClientTest();
	//ServerTest();

	std::cin.get();
	WSACleanup();
	return 0;
}
