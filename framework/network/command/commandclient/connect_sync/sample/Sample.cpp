
#include <dllNetFrame_SyncCmdClients/Exports.h>
#include <libFoundation/modafx.h>
#include <iostream>

#include <deque>
std::deque<NETHANDLE>		g_deque;
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

void _CALLTYPE ClientConnect( NETHANDLE handle,
					const char* c_szSvrIP,
					UInt16 uSvrPort )
{
	if( handle != -1 )
	{
		CASAddAndFetch(&g_Inc);
		MCmd_SynC_SetRecData(handle, ClientRecv);
		MCmd_SynC_SetClose(handle, ClientClose);
		MCmd_SynC_SentData(handle, g_DataBuf, g_tagNetCfg._iPacketLen);
		g_Lock.Lock();
		g_deque.push_back(handle);
		g_Lock.UnLock();
		//printf("g_Inc:%d\r\n",g_Inc);
	}
	else
	{
		if( !g_close )
			MCmd_SynC_Connect(g_ServerIP.c_str(), g_ServerPort);
	}
}

void _CALLTYPE ClientRecv( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	MCmd_SynC_SentData(Node, pData, u32Size);
	//printf("ClientRecv(%d):%s\r\n", u32Size, pData);
}

void _CALLTYPE ClientClose( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	//CASSubAndFetch(&g_Inc);
	printf("ClientClose:%s:%d\r\n",c_szIP, uPort);
}

int main(int argc, char* argv[])
{
	_string_type s = _file_::RWIni::GetCurrentPath();
	s.append("\\Config.ini");
	g_ServerIP = _file_::RWIni::Read("SERVER","IP",s.c_str());
	g_ServerPort = _file_::RWIni::ReadInt("SERVER","Port",s.c_str());

	g_tagNetCfg._iClientNum = _file_::RWIni::ReadInt("NET","ClientNum",s.c_str());
	g_tagNetCfg._iCloseDelay = _file_::RWIni::ReadInt("NET","CloseDelay",s.c_str());
	g_tagNetCfg._iLoopDelay = _file_::RWIni::ReadInt("NET","LoopDelay",s.c_str());
	g_tagNetCfg._iPacketLen = _file_::RWIni::ReadInt("NET","PacketLen",s.c_str());

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

	MCmd_SynC_Init();

	while(true)
	{
		g_close = false;
		int i = 0;
		for( i = 0; i < iNum; i ++ )
		{
			if( MCmd_SynC_Connect( g_ServerIP.c_str(), g_ServerPort ) == -1 )
			{
				int i = 0;
			}
		}

		Sleep(g_tagNetCfg._iCloseDelay);

		g_Lock.Lock();
		g_close = true;
		while(g_deque.size())
		{
			MCmd_SynC_Close(g_deque.front());
			g_deque.pop_front();
		}
		g_Lock.UnLock();
		/*for( i = 0; i < iNum; i ++ )
		{
			if( g_Handle[i] != NULL )
			{
				CMD_Clients_MClose(g_Handle[i]);
			}
		}*/

		Sleep(g_tagNetCfg._iLoopDelay);
	}

	delete g_DataBuf;
}

/*
_client_::CommandClients	Clients;

void ClientRecv( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	Clients.Send(Node, pData, u32Size);
	printf("ClientRecv(%d):%s\r\n", u32Size, pData);
}

void ClientClose( NETHANDLE Node, const char* c_szIP, UInt16 uPort )
{
	printf("ClientClose:%s:%d\r\n",c_szIP, uPort);
}

int main(int argc, char* argv[])
{
	struct  
	{
		int _iCloseDelay;
		int _iClientNum;
		int _iLoopDelay;
		int _iPacketLen;
	}tagNetCfg;

	_string_type s = Foundation::RWIni::GetCurrentPath();
	s.append("\\Config.ini");
	_string_type ServerIP = Foundation::RWIni::Read("SERVER","IP",s.c_str());
	int ServerPort = Foundation::RWIni::ReadInt("SERVER","Port",s.c_str());

	tagNetCfg._iClientNum = Foundation::RWIni::ReadInt("NET","ClientNum",s.c_str());
	tagNetCfg._iCloseDelay = Foundation::RWIni::ReadInt("NET","CloseDelay",s.c_str());
	tagNetCfg._iLoopDelay = Foundation::RWIni::ReadInt("NET","LoopDelay",s.c_str());
	tagNetCfg._iPacketLen = Foundation::RWIni::ReadInt("NET","PacketLen",s.c_str());

	printf("tagNetCfg._iClientNum:%d,tagNetCfg._iCloseDelay:%d,"
		"tagNetCfg._iLoopDelay:%d,tagNetCfg._iPacketLen:%d\r\n",
		tagNetCfg._iClientNum,
		tagNetCfg._iCloseDelay,
		tagNetCfg._iLoopDelay,
		tagNetCfg._iPacketLen
		);

	//Sleep(5000);

	const int iNum = tagNetCfg._iClientNum;//10000;
	char* szs = new char[tagNetCfg._iPacketLen];
	memset(szs,'1',tagNetCfg._iPacketLen);

	NETHANDLE*	Handle = new NETHANDLE[iNum];
	memset(Handle, 0, iNum * sizeof(NETHANDLE));

	while(true)
	{
		int i = 0;
		for( i = 0; i < iNum; i ++ )
		{
			Handle[i] = Clients.Connect( ServerIP.c_str(),ServerPort );
			if( Handle[i] != 0 )
			{
				Clients.SetRecvHandle(Handle[i], function20_bind_Ex(ClientRecv));
				Clients.SetCloseHandle(Handle[i], function20_bind_Ex(ClientClose));
			}
		}

		for( i = 0; i < iNum; i ++ )
		{
			if( Handle[i] != NULL )
			{
				Clients.Send(Handle[i], szs, tagNetCfg._iPacketLen);
			}
		}

		Sleep(tagNetCfg._iCloseDelay);//Sleep(500);//Sleep(50000);

		for( i = 0; i < iNum; i ++ )
		{
			if( Handle[i] != NULL )
			{
				Clients.Close(Handle[i]);
			}
		}

		Sleep(tagNetCfg._iLoopDelay);

		/ *if( Client.Connect("192.168.3.104",8080) )
		{
			Client.SetRecvHandle(&ClientRecv);
			Client.SetCloseHandle(&ClientClose);
			/ *while(true)
			{
				char ptr[1024] = {0};
				gets(ptr);
				Client.Send(ptr, strlen(ptr));
			}* /
			Client.Close();
			Sleep(1000);
		}* /
	}

	delete szs;
}*/