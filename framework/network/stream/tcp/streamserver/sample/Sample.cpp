// MediaServerSample.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libNetFrame_StreamServer/StreamServer.h>
#include <libNetFrame_Packet/Packet.h>

typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;


/*****************************************************************
/*����˵����	�������
/*����˵����	szData: ��������
/*				uSize: ��Ч���س���
/*				out_uSize: �������������
/*����ֵ��		������������
*****************************************************************/
const char* Packages_Impl( const char* szData, unsigned short uSize, unsigned short& out_uSize )
{
	static char s_szBuf[1500] = {0};
	//��ͷ
	s_szBuf[0] = 0xAA;
	s_szBuf[1] = 0xBB;
	//��Ч���س���
	memcpy(&s_szBuf[2], (char*)&uSize, sizeof(uSize));
	//��������
	memcpy(&s_szBuf[4], szData, uSize);
	//��������������
	out_uSize = uSize + 4;
	s_szBuf[out_uSize] = 0;
	//����������������
	return s_szBuf;
}

/////////////////////////////////SQL SERVER 2008//////////////////////////////////////////////

long g_AcceptNum = 0;
_server_::_tcp_::StreamServer	g_Server;

void RecvBackFun( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr, long lRemain )
{
	printf("Recv Data(%d):%s\r\n",Buf_ptr->payload, Buf_ptr->data);

	//printf("Recv Data(%d):%s\r\n",u32Size, pData);
	//GetServer()->Post(boost::bind(PostRecv, Node, Buf_ptr));
	//GetServer()->Send(Node, pData, u32Size);

	//printf("Send Data: %d\r\n",StreamBuf::GetSize(Buf_ptr.get()));
	//g_Server.Send(Node, Buf_ptr->data, Buf_ptr->payload);

	/*Buf_ptr->data[Buf_ptr->payload] = 0;
	printf("recv data(%d):%s\r\n", Buf_ptr->payload, Buf_ptr->data);

	g_Packet.Parse(Buf_ptr->data, Buf_ptr->payload);*/
}

void CloseBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	CASSubAndFetch(&g_AcceptNum);
	printf("Client Close(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

void AcceptBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	g_Server.SetRecvHandle(Node, function20_bind_Ex(RecvBackFun));
	g_Server.SetCloseHandle(Node, function20_bind(CloseBackFun, c_szIP, uPort,
												_function_::_1));
	CASAddAndFetch(&g_AcceptNum);
	printf("Client Into(%d): %s:%d\r\n", g_AcceptNum, c_szIP, uPort);
}

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#include <signal.h>
void ExitHande(int n)
{ 
	if( n == SIGINT ) 
	{
		printf("\n�㰴����ctrl+c");
		signal(SIGINT, ExitHande);
		g_Server.Release();
		return;
	} 

	if( n==SIGBREAK )
	{
		printf("\n�㰴����ctrl+break, �����˹رհ�ť�������˳�����\n");
		g_Server.Release();
		exit(0);
	}
}
#endif

int main(int argc, char* argv[])
{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	signal(SIGINT, ExitHande);  /*ע��ctrl+c�źŲ�����*/
	signal(SIGBREAK, ExitHande); /*ע��ctrl+break�źŲ�����*/
#endif

	//g_Server.Init(3);

	_string_type s = _file_::RWIni::GetCurrentPath();
	s.append("\\Config\\Stream_Config.ini");
	const int iPort = 21000;//_file_::RWIni::ReadInt("SERVER","Port",s.c_str());

	if( g_Server.Listen(iPort, function20_bind_Ex(AcceptBackFun)) )
	{
		int iThreadNum = get_processor_number();
		g_Server.Run( __max(iThreadNum / 2, 2), __max(iThreadNum / 2, 2) );
		printf("=====================================================\r\n");
		printf("Server is Runing,\r\nHost:%s, "
			"Listen Port:%d\r\n%d Thread is Runing\r\n", GetHostIP().c_str(),iPort, iThreadNum);
		printf("=====================================================\r\n");
	}

	/*char szData[100] = {0};
	szData[0] = 0xAA;
	szData[1] = 0xBB;
	UInt16 iLen = strlen("MyUIDCode:7798889898001;A�����:15.12;B�����:35.98;C�����:65.98;A���й�����:1500;B���й�����:220;C���й�����:1000");
	memcpy(&szData[2], &iLen, sizeof(UInt16));
	memcpy(&szData[4], "MyUIDCode:7798889898001;A�����:15.12;B�����:35.98;C�����:65.98;A���й�����:1500;B���й�����:220;C���й�����:1000", iLen);
	
	g_Packet.Parse(szData, iLen + 4);*/

	while(true)
	{
		Sleep(10);
	};
}