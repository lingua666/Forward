// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#include <windows.h>

// for WSALookupServiceBegin() WSALookupServiceNext etc.
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <ws2bth.h>

// for BluetoothFindFirstDevice() BluetoothFindNextDevice() etc.
#include <bthsdpdef.h>
#include <BluetoothAPIs.h>
#pragma comment ( lib, "Bthprops.lib" )
#pragma comment ( lib, "Irprops.lib" )
#include <libFoundation/modafx.h>

_string_type g_Bluetooth = "X.Pen";//"Honor 9";//

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

	sprintf(
		pstrMac, "%02X:%02X:%02X:%02X:%02X:%02X",
		pbAddr[5], pbAddr[4], pbAddr[3],
		pbAddr[2], pbAddr[1], pbAddr[0]
	);
}

BOOL CGGBlueTooth::StartSearchBthDev(IN DWORD dwControlFlags)
{
	WSAQUERYSETA wsaq;
	ZeroMemory(&wsaq, sizeof(wsaq));
	wsaq.dwSize = sizeof(wsaq);
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

	while( true )
	{
		nResult = WSALookupServiceNextA(m_hLookup, dwControlFlags, &dwSize, pwsaResults);
		if (nResult == ERROR_SUCCESS)
		{
			BOOL bHaveName = pwsaResults->lpszServiceInstanceName && *(pwsaResults->lpszServiceInstanceName);
			printf("WSALookupServiceNextA() Name:%s\r\n", pwsaResults->lpszServiceInstanceName);
			if (bHaveName && g_Bluetooth == pwsaResults->lpszServiceInstanceName )
			{
				//test
				SOCKADDR_BTH *pp = (SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr;
				//test
				pcBthDevInfo->btAddr = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
				pcBthDevInfo->iPort = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->port;
				memcpy(&pcBthDevInfo->serviceClassId,  &((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->serviceClassId, sizeof(GUID));
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
	if( sa.port == 0 )
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

void ServerTest( void )
{
	BOOL isRet = FALSE;
	CGGBlueTooth Blue;
	BTHDEV_INFO BthDevInfo;
	/*while(true)
	{
		isRet = Blue.StartSearchBthDev(LUP_CONTAINERS | LUP_FLUSHCACHE);
		isRet = Blue.GetNextBthDev(&BthDevInfo);
		isRet = Blue.EndSearchBthDev();
		if(BthDevInfo.btAddr != 0)
		{
			break;
		}
	}*/

	isRet = Blue.Create();
	isRet = Blue.Bind(0, 22);
	isRet = Blue.Listen(10);
	isRet = Blue.SetBthService("11111");
	int iErr = GetLastError();

	Blue.Accept(&Blue);
	std::cin.get();
}

// Authentication callback
BOOL WINAPI BluetoothAuthCallback(LPVOID pvParam,
	PBLUETOOTH_AUTHENTICATION_CALLBACK_PARAMS pAuthCallbackParams)
{
	DWORD dwRet;
	BLUETOOTH_AUTHENTICATE_RESPONSE AuthRes;
	AuthRes.authMethod = pAuthCallbackParams->authenticationMethod;
	// Check to make sure we are using numeric comparison (Just Works)
	if (AuthRes.authMethod == BLUETOOTH_AUTHENTICATION_METHOD_NUMERIC_COMPARISON) 
	{
		printf("Numeric Comparison supported\n");
	}
	AuthRes.bthAddressRemote = pAuthCallbackParams->deviceInfo.Address;
	AuthRes.negativeResponse = FALSE;

	// Commented out code is used for pairing using the BLUETOOTH_AUTHENTICATION_METHOD_PASSKEY method
	//memcpy_s(AuthRes.pinInfo.pin, sizeof(AuthRes.pinInfo.pin), L"1234", 0);
	//AuthRes.pinInfo.pinLength = 0;
	// Respond with numerical value for Just Works pairing
	AuthRes.numericCompInfo.NumericValue = 1;

	// Send authentication response to authenticate device
	dwRet = BluetoothSendAuthenticationResponseEx(NULL, &AuthRes);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("BluetoothSendAuthenticationResponseEx Result(%d,%d)\n", dwRet, GetLastError());
		if (dwRet == ERROR_CANCELLED)
		{
			printf("Bluetooth device denied passkey response or communicatino problem.\n");
		}
		else if (dwRet == E_FAIL)
		{
			printf("Device returned a failure code during authentication.\n");
		}
		else if (dwRet == 1244)
		{
			printf("Not Authenticated\n");
		}
	}
	else
	{
		printf("BluetoothAuthCallback finish\n");
	}

	return 1; // This value is ignored
}

void RegisterBluetoothAuthCallback( BTH_ADDR btAddr )
{
	HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle = 0;
	DWORD dwRet;

	BLUETOOTH_DEVICE_INFO  btdi = { 0 };
	// setup device info
	btdi.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
	btdi.Address.ullLong = btAddr;
	btdi.ulClassofDevice = 0;
	btdi.fConnected = FALSE;
	btdi.fRemembered = FALSE;
	btdi.fAuthenticated = FALSE;

	// register authentication callback. this prevents UI from showing up.
	dwRet = BluetoothRegisterForAuthenticationEx(&btdi, &hRegHandle, &BluetoothAuthCallback, NULL);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("BluetoothRegisterForAuthenticationEx Result(%d,%d)\n", dwRet, GetLastError());
		return ;
	}

	// authenticate device (will call authentication callback)
	AUTHENTICATION_REQUIREMENTS authreqs = MITMProtectionNotRequired;
	dwRet = BluetoothAuthenticateDeviceEx(NULL, NULL, &btdi, NULL, authreqs);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("BluetoothAuthenticateDevice Result(%d,%d)\n", dwRet, GetLastError());
		if (dwRet == ERROR_CANCELLED)
		{
			printf("Cancelled");
		}
		else if (dwRet == ERROR_INVALID_PARAMETER)
		{
			printf("Invalid Parameter");
		}
		else if (dwRet == ERROR_NO_MORE_ITEMS)
		{
			printf("Already paired!");
		}
		else if (dwRet == 1244)
		{
			printf("Not authenticated\n");
		}
		return ;
	}

	printf("Pairing finished\n");
}

void ClientTest( void )
{
	BOOL isRet = FALSE;
	CGGBlueTooth Blue;
	BTHDEV_INFO BthDevInfo;
	while(true)
	{
		isRet = Blue.StartSearchBthDev(LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_ADDR);
		isRet = Blue.GetNextBthDev(&BthDevInfo, LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_NAME | LUP_RETURN_ADDR);
		isRet = Blue.EndSearchBthDev();
		if(BthDevInfo.btAddr != 0)
		{
			break;
		}
	}

	RegisterBluetoothAuthCallback(BthDevInfo.btAddr);
	isRet = Blue.Create();

	int iPort = 0;
	while( true )
	{
		isRet = Blue.Connect(BthDevInfo.btAddr, iPort);
		int iErr = GetLastError();
		if( iErr == 0 )
			break;
		Sleep(1000);
		printf("iErr:%d,%d\r\n", iErr, iPort);
		if( iPort < 32 )
			iPort ++;
	}

	char cData[100] = {0};
	while ( true )
	{
		int iRet = Blue.Recv(cData, sizeof(cData));
		printf("Recvice(%d,%d):%s\r\n", GetLastError(), iRet, _string_type::HexToStr(_string_type(cData, iRet)).c_str());
		Sleep(1);
	}

	std::cin.get();
}


int main333(int argc, char* argv[])
{
	WSADATA wsd;
	WSAStartup (MAKEWORD(1,0), &wsd);

	ClientTest();
	//ServerTest();

	std::cin.get();
	WSACleanup();
	return 0;
}

#include <conio.h>    
#include <iostream>    
#include <string>    
#include <locale>
using namespace std;
int main111(void)
{
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
	}
	BluetoothFindRadioClose(hbf);//使用完后记得关闭HBLUETOOTH_RADIO_FIND句柄hbf。    
	cout << "All listed";
	_getch();
	return 0;
}

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <BluetoothAPIs.h>
#pragma comment(lib, "SetupAPI")

//this function works to get a handle for a BLE device based on its GUID
//copied from http://social.msdn.microsoft.com/Forums/windowshardware/en-US/e5e1058d-5a64-4e60-b8e2-0ce327c13058/erroraccessdenied-error-when-trying-to-receive-data-from-bluetooth-low-energy-devices?forum=wdk
//credits to Andrey_sh
HANDLE GetBLEHandle12(__in GUID AGuid)
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

	int iNum = SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, 0, &did);
	int iErr = GetLastError();

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

GUID USB_EX = { 0x36FC9E60, 0xC465, 0x11CF, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 };

int enum_usb_device_info(GUID uid)
{        
	int i = 0;
	int res = 0;
	HDEVINFO hDevInfo;  
	SP_DEVINFO_DATA DeviceInfoData = {sizeof(DeviceInfoData)};   

	// get device class information handle
	hDevInfo = SetupDiGetClassDevs(&uid,0, 0, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)     
	{         
		res = GetLastError();     
		return res;
	}

	// enumerute device information
	DWORD required_size = 0;
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
	{        
		DWORD DataT;         
		char friendly_name[2046] = {0};         
		DWORD buffersize = 2046;        
		DWORD req_bufsize = 0;   

		SP_DEVICE_INTERFACE_DATA did;
		DWORD size = 0;
		if(!SetupDiGetDeviceInterfaceDetail(hDevInfo, &did, NULL, 0, &size, 0) )
		{
			int err = GetLastError();

			if( err == ERROR_NO_MORE_ITEMS ) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR , size);

			pInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			SP_DEVINFO_DATA dd;
			dd.cbSize = sizeof(SP_DEVINFO_DATA);
			if( !SetupDiGetDeviceInterfaceDetail(hDevInfo, &did, pInterfaceDetailData, size, &size, &dd) )
				break;
		}

		// get device description information
		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, &DataT, (LPBYTE)friendly_name, buffersize, &req_bufsize))
		{
			res = GetLastError();
			continue;
		}

		char temp[512] = {0};
		sprintf_s(temp, 512, "USB device %d: %s", i, friendly_name);
		puts(temp);
	}

	return 0;
}

//串口传输GUID
GUID ServiceClass_UUID_EX = { 0xEEC5AD98, 0x8080, 0x425F, 0x92, 0x2A, 0xDA, 0xBF, 0x3D, 0xE3, 0xF6, 0x9A };

int main(void)
{
	//获取USB类设备的GUID
	GUID Uid;
	DWORD dwLen = sizeof(GUID);
	SetupDiClassGuidsFromName("USB", &Uid, dwLen, &dwLen); 
	enum_usb_device_info(Uid);
	
	GetBLEHandle12(Uid);
}