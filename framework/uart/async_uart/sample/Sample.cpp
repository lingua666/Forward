
#include <dllFrameWork_AsyncUarts/Exports.h>
#include <libFrameWork_AsyncUarts/AsyncUarts.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef _string_type	_app_string;

struct tagDevice_INFO
{
	UInt8	uIchargRage;	//�������
	UInt8	uMaxVolt;		//ϵͳ��ѹ
	UInt8	uMachType;		//��Ʒ����
	UInt8	uIdischgRate;	//��ŵ����
	_string_type	ProductTypeStr;	//��Ʒ���
	_string_type	SoftWareVersion;	//����汾
	_string_type	HardWareVersion;	//Ӳ���汾
	UInt32	ProductSN;	//��Ʒ���к�
	UInt8	Rs485Addr;	//�豸�� �������� ��ַ
};

struct tagAlarm_INFO
{
	UInt16	Alarm1;
	_app_string	Message1;
	UInt16	Alarm2;
	_app_string	Message2;
};

const char* c_szRemarkMessage[32] = { "���ع���", "���س�ѹ", "Ƿѹ����", "���ض�·", "���ع��ʹ�����ع���",
"�������¶ȹ���", "��ظ��±����� �¶ȸ��ڳ�����ޣ� ��ֹ���", "������빦�ʹ���", "�������˶�· �� ������", "�������˳�ѹ",
"̫���ܰ巴�� �� ������", "̫���ܰ幤���㳬ѹ", "̫���ܰ巴��", "", "",
"","","","","",
"", "", "����״̬�� 0 ���ع��磬 1 �е繩�磩", "δ��⵽��أ� Ǧ�ᣩ", "��ظ��±����� �¶ȸ��ڷŵ����ޣ� ��ֹ�ŵ�",
"��ص��±����� �¶ȵ��ڷŵ����ޣ� ��ֹ�ŵ�", "���䱣���� ֹͣ���", "��ص��±����� �¶ȵ��ڳ�����ޣ� ֹͣ���", "���ط���", "���ݳ�ѹ �� ������",
"��Ӧ̽ͷ��(·��)", "���ؿ�·(·��)" };

char g_RevszBuf[1024] = {0};
int  g_Offset = 0;
_async_::_uarts_::AsyncUarts	Async;
int g_Inc = 0;
void ReadFun(THandle Handle, const char* szData, UInt32 uSize)
{
	g_Inc += uSize;
	printf("ReadFun(%d,%d):%s\r\n", uSize, g_Inc, szData);
	if( Handle != NULL && uSize > 0 )
	{
		if( g_Offset + uSize > 1024 )
		{
			g_Offset = 0;
		}
		else
		{
			memcpy(&g_RevszBuf[g_Offset], szData, uSize);
			g_Offset += uSize;
			if( szData[uSize - 1] == '#' )
			{
				g_RevszBuf[g_Offset] = 0;
				//Async.Send(Handle, g_RevszBuf, g_Offset);
				MAsyncUart_SentData(Handle, g_RevszBuf, g_Offset);
				g_Offset = 0;
			}
		}
	}
	//Async.Close(Handle);
}

void _CALLTYPE MAsyncUart_RecData( void* pUser, THandle Handle,
	const char* szData, UInt32 uSize )
{
	//MAsyncUart_SentData(Handle, c_szData, u32Size);
	g_Inc += uSize;
	printf("ReadFun(%d,%d):%s\r\n", uSize, g_Inc, _string_type::HexToStr(szData, uSize).c_str());

	const char* c_szData = &szData[3];

	tagDevice_INFO	DeviceINFO;
	DeviceINFO.uMaxVolt = (UInt8)c_szData[0];
	DeviceINFO.uIchargRage = (UInt8)c_szData[1];
	DeviceINFO.uIdischgRate = (UInt8)c_szData[2];
	DeviceINFO.uMachType = (UInt8)c_szData[3];
	DeviceINFO.ProductTypeStr.append(&c_szData[4], 16);
	DeviceINFO.SoftWareVersion =  _string_type::HexToStr(&c_szData[21], 1) + "." + _string_type::HexToStr(&c_szData[22], 1) + "." + _string_type::HexToStr(&c_szData[23], 1);// *((UInt32*)&c_szData[20]);
	DeviceINFO.HardWareVersion = _string_type::HexToStr(&c_szData[25], 1) + "." + _string_type::HexToStr(&c_szData[26], 1) + "." + _string_type::HexToStr(&c_szData[27], 1);// *((UInt32*)&c_szData[24]);
	DeviceINFO.ProductSN = *((UInt32*)&c_szData[28]);
	DeviceINFO.Rs485Addr = *((UInt16*)&c_szData[32]);

	if( Handle != NULL && uSize > 0 )
	{
		if( g_Offset + uSize > 1024 )
		{
			g_Offset = 0;
		}
		else
		{
			memcpy(&g_RevszBuf[g_Offset], szData, uSize);
			g_Offset += uSize;
			if( szData[uSize - 1] == '#' )
			{
				g_RevszBuf[g_Offset] = 0;
				//Async.Send(Handle, g_RevszBuf, g_Offset);
				MAsyncUart_SentData(Handle, g_RevszBuf, g_Offset);
				g_Offset = 0;
			}
		}
	}
	//Async.Close(Handle);
}

void WriteFun(THandle Handle, UInt32 uSize )
{
	printf("WriteFun(%d)\r\n", uSize);
	//Async.Close(Handle);
	/*char sz[] = {"www.baidu.con#"};
	Async.Send(Handle, sz, sizeof(sz));*/
}

void DestroyFun(THandle Handle)
{
	printf("DestroyFun\r\n");
	//Async.Close(Handle);
}

int main(int argc, char* argv[])
{
	THandle Handle;
	MAsyncUart_Init();
	//Async.Init(3);

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	std::string sName;
	MAsyncUart_GetWinComName(3, sName);
	const char* szCom = sName.c_str();// "COM1";
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	char szCom[] = "/dev/ttySP1";
#endif
	
	while(true)
	{
		Handle = MAsyncUart_Open(szCom, 9600, 8, 1, false);
		if( Handle == -1 )
		{
			printf("Open Uart failed!!\r\n");
		}
		else
		{
			printf("Open Uart:%s success!!\r\n", szCom);
			//Async.SetCallBack(Handle, ReadFun, DestroyFun, WriteFun);
			
			MAsyncUart_SetRecData(Handle, MAsyncUart_RecData, NULL);
			//char sz[] = {"www.baidu.con#"};
			//unsigned char sz[] = { 0x01, 0x03, 0x00, 0x0A, 0x00, 0x01, 0xA4, 0x08 };
			//unsigned char sz[] = { 0x01, 0x03, 0x00, 0x0A, 0x00, 0x11, 0xA5, 0xC4 };
			//unsigned char sz[] = { 0x01, 0x03, 0x00, 0x14, 0x00, 0x04, 0x04, 0x0D };
			//unsigned char sz[] = { 0x01, 0x03, 0x00, 0x18, 0x00, 0x02, 0x44, 0x0C };
			//unsigned char sz[] = { 0x01, 0x03, 0x00, 0x1A, 0x00, 0x01, 0xA5, 0xCD };
			//unsigned char sz[] = { 0x01, 0x10, 0x00, 0x1A, 0x00, 0x01, 0x02, 0x00, 0x01, 0x65, 0xAA };
			//unsigned char sz[] = { 0x01, 0x03, 0x01, 0x21, 0x00, 0x02, 0x95, 0xFD };
			//unsigned char sz[] = { 0x02, 0x03, 0x00, 0xFD, 0x00, 0x31, 0x15, 0xEE };
			unsigned char sz[] = { 0x02, 0x03, 0x00, 0x01, 0x00, 0x02, 0x95, 0xF8 }; //0xA4, 0x08;
			MAsyncUart_SentData(Handle, (char*)sz, sizeof(sz));//Async.Send(Handle, sz, sizeof(sz));
		}
		std::cin.get();
		int iTm = 100000;// rand() % 2000 + 1000;
		Sleep(iTm);
		MAsyncUart_Close(Handle);
		//Async.Close(Handle);
	}

	std::cin.get();
	//Async.Close(Handle);
	MAsyncUart_Close(Handle);
	return 0;
}

