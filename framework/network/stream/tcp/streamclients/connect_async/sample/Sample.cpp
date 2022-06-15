// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libNetFrame_AsyncStreamClients/StreamClients.h>
#include <dllNetFrame_AsyncStreamClients/Exports.h>
_client_::_async_::StreamClients	Clients;
long g_iNumber = 0;

#include <deque>
std::deque<NETHANDLE>	g_deque;
CLock					g_Lock;
bool					g_close;

struct  tagNetCfg
{
	int _iCloseDelay;
	int _iClientNum;
	int _iLoopDelay;
	int _iPacketLen;
};

tagNetCfg	g_tagNetCfg;
NETHANDLE*	g_Handle = NULL;
long		g_Inc = 0;
char*		g_DataBuf = NULL;
_string_type g_ServerIP;
int g_ServerPort;

void _CALLTYPE ClientRecv( NETHANDLE Node, const char* pData, UInt32 u32Size );
void _CALLTYPE ClientClose( const char* c_szIP, UInt16 uPort, NETHANDLE Node );

void _CALLTYPE ClientConnect( const char* c_szSvrIP,
					UInt16 uSvrPort,
					NETHANDLE handle )
{
	if( handle != -1 )
	{
		CASAddAndFetch(&g_Inc);
		MStream_C_SetRecData(handle, ClientRecv);
		MStream_C_SetClose(handle, ClientClose);
		MStream_C_SentData(handle, g_DataBuf, g_tagNetCfg._iPacketLen);
		g_Lock.Lock();
		g_deque.push_back(handle);
		g_Lock.UnLock();
		//printf("g_Inc:%d\r\n",g_Inc);
	}
	else
	{
		if( !g_close )
			MStream_C_Connect(g_ServerIP.c_str(), g_ServerPort);
	}
}

void _CALLTYPE ClientRecv( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	MStream_C_SentData(Node, pData, u32Size);
	//printf("ClientRecv(%d):%s\r\n", u32Size, pData);
}

void _CALLTYPE ClientClose( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	//CASSubAndFetch(&g_Inc);
	printf("ClientClose:%s:%d\r\n",c_szIP, uPort);
	if( !g_close )
		MStream_C_Connect(g_ServerIP.c_str(), g_ServerPort);
}

int main(int argc, char* argv[])
{
	_string_type s = _file_::RWIni::GetCurrentPath();
	s.append("\\Config\\Stream_Config.ini");
	g_ServerIP = "192.168.1.246";// _file_::RWIni::Read("SERVER", "IP", s.c_str());
	g_ServerPort = 60000;// _file_::RWIni::ReadInt("SERVER", "Port", s.c_str());

	g_tagNetCfg._iClientNum = 10;// _file_::RWIni::ReadInt("NET", "ClientNum", s.c_str());
	g_tagNetCfg._iCloseDelay = 10000000;// _file_::RWIni::ReadInt("NET", "CloseDelay", s.c_str());
	g_tagNetCfg._iLoopDelay = _file_::RWIni::ReadInt("NET","LoopDelay",s.c_str());
	g_tagNetCfg._iPacketLen = 100;// _file_::RWIni::ReadInt("NET", "PacketLen", s.c_str());

	printf("g_tagNetCfg._iClientNum:%d,g_tagNetCfg._iCloseDelay:%d,"
		"g_tagNetCfg._iLoopDelay:%d,g_tagNetCfg._iPacketLen:%d\r\n",
		g_tagNetCfg._iClientNum,
		g_tagNetCfg._iCloseDelay,
		g_tagNetCfg._iLoopDelay,
		g_tagNetCfg._iPacketLen
		);

	//Sleep(5000);

	const int iNum = g_tagNetCfg._iClientNum;//10000;
	g_DataBuf = new char[g_tagNetCfg._iPacketLen];
	memset(g_DataBuf,'1',g_tagNetCfg._iPacketLen);

	g_Handle = new NETHANDLE[iNum];
	memset(g_Handle, 0, iNum * sizeof(NETHANDLE));

	MStream_C_Init();
	SetDebugModule(-1);
	MStream_C_SetConnect(ClientConnect);

	while(true)
	{
		g_close = false;
		int i = 0;
		for( i = 0; i < iNum; i ++ )
		{
			if( MStream_C_Connect( g_ServerIP.c_str(), g_ServerPort ) == -1 )
			{
				int i = 0;
			}
		}

		Sleep(g_tagNetCfg._iCloseDelay);

		g_Lock.Lock();
		g_close = true;
		while(g_deque.size())
		{
			MStream_C_Close(g_deque.front());
			g_deque.pop_front();
		}
		g_Lock.UnLock();
		Sleep(g_tagNetCfg._iLoopDelay);
	}

	delete g_DataBuf;
}