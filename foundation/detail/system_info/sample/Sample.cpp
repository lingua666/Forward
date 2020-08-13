// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllSystem_Info/dllSystem_Info.h>


#include <windows.h>
#include <iostream>
#include <string>



#define SMART_GET_VERSION               CTL_CODE(IOCTL_DISK_BASE, 0x0020, METHOD_BUFFERED, FILE_READ_ACCESS)  
#define SMART_RCV_DRIVE_DATA            CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)  
#define DFP_SEND_DRIVE_COMMAND SMART_SEND_DRIVE_COMMAND  
#define GETVERSIONOUTPARAMS GETVERSIONINPARAMS  
#define DFP_RCV_DRIVE_DATA SMART_RCV_DRIVE_DATA  
#define DFP_GET_VERSION SMART_GET_VERSION  
#define IDENTIFY_BUFFER_SIZE        512  

const WORD IDE_ATAPI_IDENTIFY = 0xA1;       //读取ATAPI设备的命令  
const WORD IDE_ATA_IDENTIFY = 0xEC;         //读取ATA设备的命令  
const int MAX_IDE_DRIVES = 1;               //最多IDE设备数  

#include<libCommon/modafx.h>
_string_type GetDiskSerialNumber( const char* c_szDrive, int iDriveNum )
{
	HANDLE hIOCTL = CreateFileA(c_szDrive, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if( hIOCTL == INVALID_HANDLE_VALUE )
		return "";

	DWORD cbBytesReturned = 0;  
	GETVERSIONOUTPARAMS VersionParams;

	// Get the version, etc of PhysicalDrive IOCTL  
	ZeroMemory(&VersionParams, sizeof(GETVERSIONOUTPARAMS));  

	if( !DeviceIoControl(hIOCTL, DFP_GET_VERSION, NULL, 0, &VersionParams, sizeof(VersionParams), &cbBytesReturned, NULL) )
	{
		CloseHandle(hIOCTL);
		return "";
	}

	if(VersionParams.bIDEDeviceMap <= 0)
	{
		CloseHandle(hIOCTL);
		return "";
	}

	// IDE or ATAPI IDENTIFY cmd  
	SENDCMDINPARAMS InParams;  

	// Now, get the ID sector for all IDE devices in the system.  
	// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,  
	// otherwise use the IDE_ATA_IDENTIFY command  
	// 具体所得结果请参考上面的说明  
	BYTE bIDCmd = (VersionParams.bIDEDeviceMap >> iDriveNum & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;  
	BYTE IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];

	ZeroMemory(&InParams, sizeof(SENDCMDINPARAMS));  
	ZeroMemory(IdOutCmd, sizeof(IdOutCmd));  

	// Set up data structures for IDENTIFY command.  
	InParams.cBufferSize = IDENTIFY_BUFFER_SIZE;
	InParams.irDriveRegs.bFeaturesReg = 0;
	InParams.irDriveRegs.bSectorCountReg = 1;
	InParams.irDriveRegs.bSectorNumberReg = 1;
	InParams.irDriveRegs.bCylLowReg = 0;
	InParams.irDriveRegs.bCylHighReg = 0;

	//Compute the drive number.(主盘和从盘所对应的值是不一样的)  
	InParams.irDriveRegs.bDriveHeadReg = (iDriveNum & 1) ? 0xB0 : 0xA0;  

	// The command can either be IDE identify or ATAPI identify.  
	InParams.irDriveRegs.bCommandReg = bIDCmd;  
	InParams.bDriveNumber = iDriveNum;  
	InParams.cBufferSize = IDENTIFY_BUFFER_SIZE;

	if( !DeviceIoControl(hIOCTL, DFP_RCV_DRIVE_DATA, (LPVOID)&InParams, sizeof(SENDCMDINPARAMS) - 1,   
		(LPVOID)IdOutCmd, sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, &cbBytesReturned, NULL) )
	{
		CloseHandle(hIOCTL);
		return "";
	}

	DWORD DiskData[256] = {0};  
	USHORT *pIDSector = NULL;                                              // 对应结构IDSECTOR，见头文件  
	pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)IdOutCmd)->bBuffer;

	for(int i = 0; i < 256; i ++)  
		DiskData[i] = pIDSector[i];  

	CloseHandle(hIOCTL);

	char ResBuf[1024] = {0};
	int i = 0;
	for(char *p = (char*)(DiskData + 10); p <= (char*)(DiskData + 19); p += sizeof(DWORD*))
	{  
		ResBuf[i++] = *(p+1);
		ResBuf[i++] = *p;  
	}  

	return _string_type(ResBuf).strim();
}


std::string Trim( std::string str)             // cut off leading and tailing blanks  
{  
	std::string::size_type pos = str.find_last_not_of(' ');  
	if(pos != std::string::npos)  
	{  
		str.erase(pos + 1);  
		pos = str.find_first_not_of(' ');  
		if(pos != std::string::npos)   
			str.erase(0, pos);  
	}  
	else   
		str.erase(str.begin(), str.end());  

	return str;  
}  

// fetch characters what we need from DiskData[FirstIndex ... LastIndex]  
std::string ConvertToString(DWORD DiskData[256], int FirstIndex, int LastIndex)  
{  
	int i = 0;  
	char ResBuf[1024] = {0};  

	for(char *p = (char*)(DiskData + FirstIndex); p <= (char*)(DiskData + LastIndex); p += sizeof(DiskData))  
	{  
		ResBuf[i++] = *(p+1);  
		ResBuf[i++] = *p;  
	}  

	return Trim(ResBuf);            // cut off leading and tailing blanks  
}  

BOOL __fastcall DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, byte bIDCmd, byte bDriveNum, PDWORD lpcbBytesReturned)  
{  
	// Set up data structures for IDENTIFY command.  
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;  
	pSCIP->irDriveRegs.bFeaturesReg = 0;  
	pSCIP->irDriveRegs.bSectorCountReg = 1;  
	pSCIP->irDriveRegs.bSectorNumberReg = 1;  
	pSCIP->irDriveRegs.bCylLowReg = 0;  
	pSCIP->irDriveRegs.bCylHighReg = 0;  

	//Compute the drive number.(主盘和从盘所对应的值是不一样的)  
	pSCIP->irDriveRegs.bDriveHeadReg = (bDriveNum & 1) ? 0xB0 : 0xA0;  

	// The command can either be IDE identify or ATAPI identify.  
	pSCIP->irDriveRegs.bCommandReg = bIDCmd;  
	pSCIP->bDriveNumber = bDriveNum;  
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;  

	return DeviceIoControl(hPhysicalDriveIOCTL, DFP_RCV_DRIVE_DATA, (LPVOID)pSCIP, sizeof(SENDCMDINPARAMS) - 1,   
		(LPVOID)pSCOP, sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, lpcbBytesReturned, NULL);  
}  

// 获取硬盘型号、序列号、修订版本号  
void __fastcall ReadPhysicalDriveOnNT()  
{  
	//IDENTIFY_BUFFER_SIZE = 512  
	byte IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];  
	char serialNo[21], modelNo[21], revisionNo[21];  

	for(int Drive=0; Drive < 1/*4*/; Drive++)  
	{  
		HANDLE hPhysicalDriveIOCTL;  
		char DriveName[32];  
		sprintf(DriveName, "\\\\.\\PhysicalDrive%d", Drive);
		hPhysicalDriveIOCTL = CreateFileA(DriveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);  

		if(hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)  
		{  
			DWORD cbBytesReturned = 0;  
			GETVERSIONOUTPARAMS VersionParams;  

			// Get the version, etc of PhysicalDrive IOCTL  
			ZeroMemory(&VersionParams, sizeof(GETVERSIONOUTPARAMS));  

			if(!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION, NULL, 0, &VersionParams, sizeof(VersionParams), &cbBytesReturned, NULL))  
			{  
				continue;  
			}  

			if(VersionParams.bIDEDeviceMap > 0)  
			{  
				// IDE or ATAPI IDENTIFY cmd  
				byte bIDCmd = 0;  
				SENDCMDINPARAMS InParams;  

				// Now, get the ID sector for all IDE devices in the system.  
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,  
				// otherwise use the IDE_ATA_IDENTIFY command  
				// 具体所得结果请参考上面的说明  
				bIDCmd = (VersionParams.bIDEDeviceMap >> Drive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;  

				ZeroMemory(&InParams, sizeof(SENDCMDINPARAMS));  
				ZeroMemory(IdOutCmd, sizeof(IdOutCmd));  

				if(DoIdentify(hPhysicalDriveIOCTL, &InParams, (PSENDCMDOUTPARAMS)IdOutCmd, (BYTE)bIDCmd,(BYTE)Drive, &cbBytesReturned))  
				{  
					DWORD DiskData[256];  
					USHORT *pIDSector;                                              // 对应结构IDSECTOR，见头文件  
					pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)IdOutCmd)->bBuffer;  

					for(int i=0; i < 256; i++)  
						DiskData[i] = pIDSector[i];

					FILE *fp = NULL;  
					if(NULL != (fp = fopen("a.log", "w")))  
					{
						fwrite(DiskData, sizeof(DiskData), 1, fp);  
						fclose(fp);  
						//cout<<"Write file successful"<<endl;  
					}  

					strcpy(serialNo, ConvertToString(DiskData, 10, 19).c_str());    // get serial number  
					/*strcpy(modelNo, ConvertToString(DiskData, 27, 46).c_str());     // get model type  
					strcpy(revisionNo, ConvertToString (DiskData, 23, 26).c_str()); // 获取修订版本号  */

					printf("SerialNo=%s\nModelNo=%s\nRevisionNo=%s\n", serialNo, modelNo, revisionNo);  
				}  

				int Erro = GetLastError();
				int i = 0;
			}  
			CloseHandle(hPhysicalDriveIOCTL);  
		}  
	}  
} 



void _CALLTYPE MGetDiskInfo_test(tagDisk_INFO* pInfo)
{
	printf("pManufacturer:%s\r\n", pInfo->pManufacturer);
	printf("pModel:%s\r\n", pInfo->pModel);
	printf("pSerialNumber:%s\r\n", pInfo->pSerialNumber);
	printf("pInterfaceType:%s\r\n", pInfo->pInterfaceType);
	printf("Size:%lld\r\n", pInfo->uSize);
}

int main(int argc, char* argv[])
{
	/*ReadPhysicalDriveOnNT(); 
	printf("main ser:%s\r\n", GetDiskSerialNumber("\\\\.\\PhysicalDrive0", 0).c_str());

	tagCpu_INFO* p = GetCpuInfo();
	printf("pBrand:%s, pSerialNumber:%s, pVendor:%s\r\n",p->pBrand, p->pSerialNumber, p->pVendor);
	MGetDiskInfo(MGetDiskInfo_test);*/

	MGetPicFormDllEx("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\图片流指针测试BufV4.0\\ClientBG.png",
		"D:\\ClientBG.png");

	std::cin.get();
	return 0;
}