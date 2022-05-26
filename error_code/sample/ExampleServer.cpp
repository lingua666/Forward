// Link to ws2_32.lib
/*#include <winsock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <stdio.h>
 
#define DEFAULT_BUFLEN 512

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "irprops.lib")

int main2(int argc, char **argv)
{
	WORD wVersionRequested = 0x202;
	WSADATA m_data;
	if (0 == WSAStartup(wVersionRequested, &m_data))
	{
		SOCKET s = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		const DWORD lastError = ::GetLastError();

		if (s == INVALID_SOCKET)
		{
			printf("Failed to get bluetooth socket! %d\n",       
				GetLastError());
			exit(1);
		}
		WSAPROTOCOL_INFO protocolInfo;
		int protocolInfoSize = sizeof(protocolInfo);

		if (0 != getsockopt(s, SOL_SOCKET, SO_PROTOCOL_INFO, 
			(char*)&protocolInfo, &protocolInfoSize))
		{
			exit(1);
		}
		SOCKADDR_BTH address;
		address.addressFamily = AF_BTH;
		address.btAddr = 0;
		address.serviceClassId = GUID_NULL;
		address.port = BT_PORT_ANY;
		sockaddr *pAddr = (sockaddr*)&address;

		if (0 != bind(s, pAddr, sizeof(SOCKADDR_BTH)))
		{
			printf("%d\n", GetLastError());
		}
		else
		{
			printf("\nBinding Successful....\n");
			int length = sizeof(SOCKADDR_BTH) ;
			getsockname(s,(sockaddr*)&address,&length);
			wprintf (L"Local Bluetooth device is %04x%08x \nServer channel = %d\n", 
				GET_NAP(address.btAddr), GET_SAP(address.btAddr), address.port);
		}

		int size = sizeof(SOCKADDR_BTH);
		if (0 != getsockname(s, pAddr, &size))
		{
			printf("%d\n", GetLastError());
		}
		if (0 != listen(s, 10))
		{
			printf("%d\n", GetLastError());
		}

		WSAQUERYSET service;
		memset(&service, 0, sizeof(service));
		service.dwSize = sizeof(service);
		service.lpszServiceInstanceName = "Accelerometer Data...";
		service.lpszComment = "Pushing data to PC";

		GUID serviceID = OBEXFileTransferServiceClass_UUID;

		service.lpServiceClassId = &serviceID;
		service.dwNumberOfCsAddrs = 1;
		service.dwNameSpace = NS_BTH;

		CSADDR_INFO csAddr;
		memset(&csAddr, 0, sizeof(csAddr));
		csAddr.LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
		csAddr.LocalAddr.lpSockaddr = pAddr;
		csAddr.iSocketType = SOCK_STREAM;
		csAddr.iProtocol = BTHPROTO_RFCOMM;
		service.lpcsaBuffer = &csAddr;

		if (0 != WSASetService(&service, RNRSERVICE_REGISTER, 0))
		{
			printf("Service registration failed....");
			printf("%d\n", GetLastError());
		}
		else
		{    
			printf("\nService registration Successful....\n");
		}
		printf("\nBefore accept.........");
		SOCKADDR_BTH sab2;
		int ilen = sizeof(sab2);
		SOCKET s2 = accept (s,(sockaddr*)&sab2, &ilen);
		if (s2 == INVALID_SOCKET)
		{
			wprintf (L"Socket bind, error %d\n", WSAGetLastError ());
		}
		wprintf (L"\nConnection came from %04x%08x to channel %d\n",
			GET_NAP(sab2.btAddr), GET_SAP(sab2.btAddr), sab2.port);
		wprintf (L"\nAfter Accept\n");

		char buffer[1024] = {0}; 
		memset(buffer, 0, sizeof(buffer));
		int r = recv(s2,(char*)buffer, sizeof(buffer), 0);
		printf("%s\n",buffer);

		closesocket(s2);
		if (0 != WSASetService(&service, RNRSERVICE_DELETE, 0))
		{
			printf("%d\n", GetLastError());
		}
		closesocket(s);
		WSACleanup();
	}

	return 0;
}
 
int main1(int argc, char **argv)
{
            WSADATA wsd;
            SOCKET s, s2;
            SOCKADDR_BTH sab, sab2;
// NULL_GUID
            GUID nguiD = {00000000-0000-0000-0000-000000000000};
            int ilen, iResult;
 
            // This should be const void * type for non-char data
            char *sendbuf = "Test data from receiver...";
            int recvbuflen = DEFAULT_BUFLEN;
            // Change the type accordingly for non-char data
            char recvbuf[DEFAULT_BUFLEN] = "";
 
            if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
            {
                        printf("Unable to load Winsock! Error code is %d\n", WSAGetLastError());
                        return 1;
            }
            else
                        printf("WSAStartup() is OK, Winsock lib loaded!\n");
 
            s = socket (AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
            if (s == INVALID_SOCKET)
            {
                        printf ("Socket creation failed, error %d\n", WSAGetLastError());
                        return 1;
            }
            else
                        printf ("socket() looks fine!\n");
 
            memset (&sab, 0, sizeof(sab));
            sab.addressFamily  = AF_BTH;
            // We hardcoded it
            sab.port = 2;
 
            if (0 != bind(s, (SOCKADDR *) &sab, sizeof(sab)))
            {
                        printf ("bind() failed with error code %d\n", WSAGetLastError());
                        closesocket (s);
                        return 0;
            }
            else
                        printf ("bind() looks fine!\n");
 
            if(listen (s, 5) == 0)
                        printf("listen() is OK! Listening for connection...\n");
            else
                        printf("listen() failed with error code %d\n", WSAGetLastError());
 
            for ( ; ; )
            {
                        // Get information on the port assigned
                        ilen = sizeof(sab2);
                        s2 = accept (s, (SOCKADDR *)&sab2, &ilen);
                        if (s2 == INVALID_SOCKET)
                        {
                                    printf ("accept() failed with error code %d\n", WSAGetLastError ());
                                    break;
                        }
                        else
                                    printf ("accept(), is OK buddy!\n");
 
                        // Print the info
                        printf ("Connection came from %04x%08x to channel %d\n", GET_NAP(sab2.btAddr), GET_SAP(sab2.btAddr), sab2.port);
            }
 
    // Receive until the peer closes the connection
    do {
        iResult = recv(s2, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf(" %d Bytes received from sender\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed by peer!\n");
        else
            printf("recv() failed with error code %d\n", WSAGetLastError());
 
    } while(iResult > 0);
 
    // Echo back the data
    iResult = send(s2, recvbuf, recvbuflen, 0 );
    if (iResult == SOCKET_ERROR)
    {
        printf("send() failed with error code %d\n", WSAGetLastError());
        closesocket(s2);
        WSACleanup();
        return 1;
    }
            else
            {
                        printf("send() is OK!\n");
                        printf("Bytes Sent: %d\n", iResult);
            }
 
            if(closesocket(s) == 0)
                        printf("closesocket() pretty fine!\n");
            if(WSACleanup () == 0)
                        printf("WSACleanup() is OK!\n");
            return 0;
}*/

// BluetoothTest.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <BthLEDef.h>
#include <BluetoothAPIs.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothAPIs.lib")
#define TO_SEARCH_DEVICE_UUID "{0000180D-0000-1000-8000-00805F9B34FB}" //we use UUID for an HR BLE device 


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
	SP_DEVICE_INTERFACE_DATA did;
	SP_DEVINFO_DATA dd;
	GUID BluetoothInterfaceGUID = AGuid;
	HANDLE hComm = NULL;

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

			hComm = CreateFile(
				pInterfaceDetailData->DevicePath,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

			GlobalFree(pInterfaceDetailData);
		}
	}

	SetupDiDestroyDeviceInfoList(hDI);
	return hComm;
}


int main( int argc, char *argv[ ], char *envp[ ] )
{
	//Step 1: find the BLE device handle from its GUID
	GUID AGuid;
	//GUID can be constructed from "{xxx....}" string using CLSID
	CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID), &AGuid);

	//now get the handle 
	HANDLE hLEDevice = GetBLEHandle(AGuid);


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

	PBTH_LE_GATT_CHARACTERISTIC pCharBuffer;
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
	PBTH_LE_GATT_CHARACTERISTIC currGattChar;
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

