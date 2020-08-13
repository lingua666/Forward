#ifndef  __CUSTOM_WSASOCKET_HEAD_DEF__
#define  __CUSTOM_WSASOCKET_HEAD_DEF__

#include "CMyInitSock.h"
#include "External.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SD_RECEIVE 0x00
#define SD_SEND 0x01
#define SD_BOTH 0x02

#define SIO_KEEPALIVE_VALS IOC_IN | IOC_VENDOR | 4

#pragma pack(push,1)

typedef struct tagWSAtcp_keepalive
{
	unsigned long onoff;
	unsigned long keepalivetime;			//首次KeepAlive探测前的TCP空闭时间
	unsigned long keepaliveinterval;		//每次检测的间隔 （单位毫秒）
}WSATCP_KEEPALIVE,*LPWSATCP_KEEPALIVE;

typedef struct tagWSAOverlapped
{
	WSAOVERLAPPED		wsaOverlapped;
	WSABUF				wsaBuf;
}SWSAOverlapped,LP_SWSAOverlapped;

#pragma pack(pop)


SOCKET	APIWSACreate( int iSocketType, int iRevSize, int iSendSize );
BOOL	APIWSAClose( SOCKET Sock );
int		APIWSABind( SOCKET Sock, u_short usPort, const char* c_szIP = NULL );
int		APIWSAListen( SOCKET Sock, u_int ListenNum, LPFN_ACCEPTEX& lpfnAcceptEx,
					   LPFN_GETACCEPTEXSOCKADDRS& lpfnGetAcceptExSockAddrs );

int		APIWSAConnect( SOCKET Sock, const char* c_szIP, u_short usPort );
int		APIWSAConnectExInit( SOCKET Sock, LPFN_CONNECTEX& lpfnConnectEx );
int		APIWSAConnectEx( LPFN_CONNECTEX& lpfnConnectEx,
						SOCKET	Sock, sockaddr_in& Addr,
						SWSAOverlapped* pSWSAOverlapped,
						unsigned short usBufSize );

SOCKET	APIWSAAccept( SOCKET Sock, sockaddr* pAddr );

BOOL	APIWSAacceptEx( LPFN_ACCEPTEX& lpfnAcceptEx, SOCKET ListenSock,
						 SOCKET& NewSock, SWSAOverlapped* pSWSAOverlapped, 
						 unsigned short usBufSize = 0 );
BOOL	APIWSAGetacceptAddr( LPFN_GETACCEPTEXSOCKADDRS& lpfnGetAcceptExSockAddrs, 
							SWSAOverlapped* pSWSAOverlapped, sockaddr** lpLocaladdr, 
							sockaddr** lpRemoteaddr );

int		APIWSASend( SOCKET Sock, const char* c_szDataBuf, u_long ulDataSize,
		         WSAOVERLAPPED* pOverlapped );

int		APIWSARecv( SOCKET Sock, char* szDataBuf, u_long ulDataSize,
		         WSAOVERLAPPED* pOverlapped );

int		APIWSASendto( SOCKET Sock, const char* c_szDataBuf, u_long ulDataSize,
		           sockaddr* pAddr ,WSAOVERLAPPED* pOverlapped );

int		APIWSARecvFrom( SOCKET Sock, char* szDataBuf, u_long ulDataSize,
		                sockaddr* pAddr,WSAOVERLAPPED* pOverlapped );

int		APIWSASetSockopt( SOCKET Sock, int Level, int Optname, 
		                 const char* c_szOptval, int Optlen );
BOOL	APIGetWSASocketLocalIP( SOCKET Sock, char* szIP, u_int uiSize );
BOOL	APIGetWSASocketLocalPort( SOCKET Sock, u_short& usPort );

BOOL	APIGetWSASocketRemoteIP( SOCKET Sock, char* szIP, u_short usSize );
BOOL	APIGetWSASocketRemotePort( SOCKET Sock, u_short& usPort );
BOOL	APIWSASetKeepalive( SOCKET Sock,int iTime,int iTimernum );
	
#ifdef  __cplusplus
}
#endif

#endif
