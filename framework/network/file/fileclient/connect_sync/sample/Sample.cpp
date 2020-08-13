// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libNetFrame_SyncFileClient/FileClients.h>

_client_::_sync_::FileClients	Clients;
long g_iNumber = 0;

void PostRecv( NETHANDLE Node, const MediaDBuf_ptr& Buf_ptr )
{
	int is = Buf_ptr->_uPayload;
	printf("Send Data:%d\r\n", Buf_ptr->_uPayload);
	Clients.Send(Node, Buf_ptr->_Data, Buf_ptr->_uPayload);
}

void ClientRecv( NETHANDLE Node, const StreamBuf_ptr& Buf_ptr )
{
	Clients.Send(Node, StreamBuf::GetData(Buf_ptr.get()),
		StreamBuf::GetSize(Buf_ptr.get()));
}

void ClientClose( NETHANDLE Node, const char* c_szIP, UInt16 uPort )
{
	CASSubAndFetch(&g_iNumber);
	printf("ClientClose(%d):%s:%d\r\n",g_iNumber,c_szIP, uPort);
}

/*
_string_type GetHostIP( void )
{
	char ip[20] = {0};
	struct hostent *phostinfo = gethostbyname("");  
	char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
	strcpy(ip, p);  
	return _string_type(ip);
}*/

int main(int argc, char* argv[])
{
	const int iNum = 100;

	NETHANDLE*	Handle = new NETHANDLE[iNum];
	memset(Handle, 0, iNum * sizeof(NETHANDLE));

	while(true)
	{
		int i = 0;
		for( i = 0; i < iNum; i ++ )
		{
			Handle[i] = Clients.Connect( GetHostIP().c_str(), 11000 );
			if( Handle[i] != 0 )
			{
				CASAddAndFetch(&g_iNumber);
				Clients.SetRecvHandle(Handle[i], function20_bind_Ex(ClientRecv));
				Clients.SetCloseHandle(Handle[i], function20_bind_Ex(ClientClose));
			}
		}

		char szData[1024] = {0};
		memset(szData,'1',sizeof(szData) - 1);

		for( i = 0; i < iNum; i ++ )
		{
			if( Handle[i] != NULL )
			{
				Clients.Send(Handle[i], szData, sizeof(szData));
			}
		}

		Sleep(1000);//Sleep(500);//Sleep(50000);

		for( i = 0; i < iNum; i ++ )
		{
			if( Handle[i] != NULL )
			{
				Clients.Close(Handle[i]);
			}
		}

		Sleep(100);

		/*if( Client.Connect("192.168.3.104",8080) )
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
		}*/
	}

	delete Handle;

	std::cin.get();
	return 0;
}