
#include <dllNetFrame_AsyncCmdClients/Exports.h>
#include <libNetFrame_AsyncCmdClients/CommandClients.h>
#include <iostream>

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
char*		g_RevDataBuf = NULL;
_string_type g_ServerIP;
int g_ServerPort;

void _CALLTYPE ClientRecv( NETHANDLE Node, const char* pData, int u32Size );
void _CALLTYPE ClientClose( const char* c_szIP, int uPort, NETHANDLE Node );

void _CALLTYPE ClientConnect( const char* c_szSvrIP,
					int uSvrPort,
					NETHANDLE handle )
{
	if( handle != -1 )
	{
		CASAddAndFetch(&g_Inc);
		MCmd_C_SetRecData(handle, ClientRecv);
		MCmd_C_SetClose(handle, ClientClose);
		if( g_tagNetCfg._iPacketLen > 0 )
			MCmd_C_SentData(handle, g_DataBuf, g_tagNetCfg._iPacketLen);
		g_Lock.Lock();
		g_deque.push_back(handle);
		g_Lock.UnLock();
		//printf("g_Inc:%d\r\n",g_Inc);
	}
	else
	{
		if( !g_close )
			MCmd_C_Connect(g_ServerIP.c_str(), g_ServerPort);
	}
}

void _CALLTYPE ClientRecv( NETHANDLE Node, const char* pData, int u32Size )
{
	MCmd_C_SentData(Node, pData, u32Size);
	//printf("ClientRecv(%d):%s\r\n", u32Size, pData);
}

void _CALLTYPE ClientClose( const char* c_szIP, int uPort, NETHANDLE Node )
{
	//CASSubAndFetch(&g_Inc);
	printf("ClientClose:%s:%d\r\n",c_szIP, uPort);
	if( !g_close )
		MCmd_C_Connect(g_ServerIP.c_str(), g_ServerPort);
}


unsigned short mb_crc(unsigned char * frame, int n)
{
	int i,j;
	unsigned short flag;
	unsigned short crc=0xFFFF;
	for(i=0;i<n;i++)
	{
		crc ^= frame[i];
		for(j=0;j<8;j++)
		{
			flag = crc&1;
			crc >>= 1;
			if(flag) crc^=0xA001;
		}
	}
	return crc;
}

unsigned char chartobin(char ss)
{
	if(ss>='0' && ss<='9') return ss-'0';
	if(ss>='A' && ss<='F') return ss-'A'+10;
	return 255;
}

//获取校验值
int assic_crc(char * ccmd)
{
	int i, len=strlen(ccmd);
	unsigned short crc;
	unsigned char * cmd;
	if(len < 20) return -1;
	cmd = (unsigned char *)malloc(len/2);
	if(ccmd[0]=='[' && ccmd[len-1]==']')
	{
		for(i=0;i<len/2-3;i++)
		{
			if(i==3)
			{
				cmd[i] = 0xF0+chartobin(ccmd[i*2+2]);
				if(chartobin(ccmd[i*2+2])>9)
				{
					free(cmd);
					return -2; //chartobin 有返回为 255 时
				}
			}
			else
			{
				cmd[i] = chartobin(ccmd[i*2+1])*16+chartobin(ccmd[i*2+2]);
				if(cmd[i] > 0x99)
				{
					free(cmd);
					return -3; //chartobin 有返回为 255 时
				}
			}
		}
		crc = mb_crc(cmd, i);
		sprintf(ccmd+(len-5), "%02X%02X]", crc%256, crc/256%256);
		free(cmd);
		return 1;
	}
	else
	{
		free(cmd);
		return -4;
	}
}

//检查校验是否正确
int CheckCRC(char * ccmd)
{
	int i, len=strlen(ccmd);
	unsigned short crc;
	unsigned char * cmd;
	if(len < 20) return -1;
	cmd = (unsigned char *)malloc(len/2);
	crc = (chartobin(ccmd[len-3])*16+chartobin(ccmd[len-2]))*0x100;
	crc += chartobin(ccmd[len-5])*16+chartobin(ccmd[len-4]);
	if(ccmd[0]=='[' && ccmd[len-1]==']')
	{
		for(i=0;i<len/2-3;i++)
		{
			if(i==3)
			{
				cmd[i] = 0xF0+chartobin(ccmd[i*2+2]);
				if(chartobin(ccmd[i*2+2])>9)
				{
					free(cmd);
					return -2; //chartobin 有返回为 255 时
				}
			}
			else
			{
				cmd[i] = chartobin(ccmd[i*2+1])*16+chartobin(ccmd[i*2+2]);
				if(cmd[i] > 0x99)
				{
					free(cmd);
					return -3; //chartobin 有返回为 255 时
				}
			}
		}
		if(crc == mb_crc(cmd, i))
		{
			free(cmd);
			return 1;
		}
		else
		{
			free(cmd);
			return 0;
		}
	}
	else
	{
		free(cmd);
		return -4;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	int i;
	char ccmd[] = "[010008C001201404121611230299990031FFFF]"; //校验位先用 FFFF 填充
	i = assic_crc(ccmd);
	if(i<0)
	{
		printf("协议有错误 %d\n", i);
	}
	else
	{
		//[010008C0012014041216112302999900314F7E]
		printf("%s\n", ccmd);
	}
	i = CheckCRC(ccmd);
	if(i<0) printf("协议有错\n");
	else if(i==0) printf("校验有错\n");
	else printf("校验对\n");


	_string_type s = _file_::RWIni::GetCurrentPath();
	s += _file_::CMyDirectory::GetSplit();
	s += "Config";
	s += _file_::CMyDirectory::GetSplit();
	s += "Config.ini";
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
	g_RevDataBuf = new char[1025 * 100];
	memset(g_DataBuf,'1',g_tagNetCfg._iPacketLen);

	g_Handle = new NETHANDLE[iNum];
	memset(g_Handle, 0, iNum * sizeof(NETHANDLE));

	SetDebugModule(-1);
	MCmd_C_Init();
	MCmd_C_SetConnect(ClientConnect);

	MCmd_C_Error("11111");

	while(true)
	{
		g_close = false;
		int i = 0;
		for( i = 0; i < iNum; i ++ )
		{
			if( MCmd_C_Connect( g_ServerIP.c_str(), g_ServerPort ) == -1 )
			{
				int i = 0;
			}
		}

		Sleep(g_tagNetCfg._iCloseDelay);

		g_Lock.Lock();
		g_close = true;
		while(g_deque.size())
		{
			MCmd_C_Close(g_deque.front());
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
	delete g_RevDataBuf;
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