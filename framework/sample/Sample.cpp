
#include <libNetFrame_AsyncCmdClients/CommandClients.h>
#include <libNetFrame_AsyncRawClients/RawClients.h>
#include <libNetFrame_AsyncStreamClients/StreamClients.h>
#include <iostream>

_client_::_async_::RawClients		g_RawClients;
_client_::_async_::CommandClients	g_CmdClients;


void _CALLTYPE ClientCmdConnect( const _string_type& sIP,
								UInt16 u16Port,
								NETHANDLE handle )
{
	g_CmdClients.Connect(sIP.c_str(), u16Port,
		function20_bind(ClientCmdConnect, sIP, u16Port, _function_::_1));
}

void _CALLTYPE RecvCmdBackFun( NETHANDLE Node, const char* pData, UInt32 u32Size )
{
	
}

void _CALLTYPE CloseCmdBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{
	
}

void _CALLTYPE ClientRawConnect( const _string_type& sIP,
								UInt16 u16Port,
								NETHANDLE handle )
{
	g_RawClients.Connect(sIP.c_str(), u16Port,
		function20_bind(ClientRawConnect, sIP, u16Port, _function_::_1));
}

void _CALLTYPE RecvRawBackFun( NETHANDLE Node, const char* pData, UInt32 u32Size )
{

}

void _CALLTYPE CloseRawBackFun( const char* c_szIP, UInt16 uPort, NETHANDLE Node )
{

}

int main(int argc, char* argv[])
{
	g_RawClients.Init(get_processor_number() * 2 + 2);
	g_CmdClients.Init(get_processor_number() * 2 + 2);

	_string_type sCmdIp = "192.168.8.166";
	g_CmdClients.Connect(sCmdIp.c_str(), 8000,
		function20_bind(ClientCmdConnect, sCmdIp, 8000, _function_::_1));


	_string_type sRawIp = "127.0.0.1";
	g_RawClients.Connect(sRawIp.c_str(), 8080,
		function20_bind(ClientRawConnect, sRawIp, 8080, _function_::_1));

	std::cin.get();
	return 1;
}