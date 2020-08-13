
#include <libNetFrame_SyncRawClients/RawClients.h>
#include <libFoundation/modafx.h>

#include <iostream>

_client_::_sync_::RawClients	Clients;

void ClientRecv( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	Clients.Send(Node, pData, u32Size);
	printf("ClientRecv(%d):%s\r\n", u32Size, pData);
}

void ClientClose( const _string_type& sIP, UInt16 uPort, NETHANDLE Node )
{
	printf("ClientClose:%s:%d\r\n",sIP.c_str(), uPort);
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
	int iNum = 100;
	char sz[5120] = {0};
	memset(sz,'1',sizeof(sz));

	NETHANDLE	Handle[100] = {0};

	while(true)
	{
		int i = 0;
		for( i = 0; i < iNum; i ++ )
		{
			Handle[i] = Clients.Connect( GetHostIP().c_str(),60000 );
			if( Handle[i] != 0 )
			{
				Clients.SetRecvHandle(Handle[i], function20_bind_Ex(ClientRecv));
				Clients.SetCloseHandle(Handle[i], function20_bind(ClientClose,
															GetIPv4ToString(Handle[i]), GetPort(Handle[i]),
															_function_::_1));
			}
		}

		for( i = 0; i < iNum; i ++ )
		{
			if( Handle[i] != NULL )
			{
				Clients.Send(Handle[i], sz, sizeof(sz));
			}
		}

		Sleep(1000);
		for( i = 0; i < iNum; i ++ )
		{
			if( Handle[i] != NULL )
			{
				Clients.Close(Handle[i]);
			}
		}

		Sleep(1000);
	}

	std::cin.get();
	return 0;
}
