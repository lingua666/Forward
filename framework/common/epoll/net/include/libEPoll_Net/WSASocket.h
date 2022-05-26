#ifndef  __CUSTOM_WSASOCKET_HEAD_DEF__
#define  __CUSTOM_WSASOCKET_HEAD_DEF__

#include "CMyInitSock.h"
#include "External.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	#ifdef __cplusplus
	extern "C" {
	#endif

	#define SD_RECEIVE 0x00
	#define SD_SEND 0x01
	#define SD_BOTH 0x02

	#define SIO_KEEPALIVE_VALS IOC_IN | IOC_VENDOR | 4

	typedef	int SOCKET;

	SOCKET	APIWSACreate( int iSocketType, int iRevSize, int iSendSize, const char* c_szIP = NULL );
	int APIWSASetnonBlocking( SOCKET Sock );
	int	APIWSAShutDown( SOCKET Sock );
	int	APIWSAClose( SOCKET Sock );
	int	APIWSABind( SOCKET Sock, unsigned short usPort, const char* c_szIP = NULL );
	int	APIWSAListen( SOCKET Sock, unsigned short ListenNum );

	int	APIWSAConnect( SOCKET Sock, const char* c_szIP, unsigned short usPort );

	SOCKET	APIWSAAccept( SOCKET Sock, struct sockaddr_in* pAddr );

	int	APIWSASend( SOCKET Sock, const char* c_szDataBuf, unsigned long ulDataSize );

	int	APIWSARecv( SOCKET Sock, char* szDataBuf, unsigned long ulDataSize );

	int	APIWSASendto( SOCKET Sock, const char* c_szDataBuf,
		unsigned long ulDataSize, struct sockaddr_in* pAddr );

	int	APIWSARecvFrom( SOCKET Sock, char* szDataBuf,
		unsigned long ulDataSize, struct sockaddr_in* pAddr );

	int	APIWSASetSockopt( SOCKET Sock, int Level, int Optname, 
						 const char* c_szOptval, int Optlen );
	int	APIGetWSASocketLocalIP( SOCKET Sock, char* szIP, unsigned long uiSize );
	int	APIGetWSASocketLocalPort( SOCKET Sock, unsigned short& usPort );

	int	APIGetWSASocketRemoteIP( SOCKET Sock, char* szIP, unsigned short usSize );
	int	APIGetWSASocketRemotePort( SOCKET Sock, unsigned short& usPort );
	int	APIWSASetKeepalive( SOCKET Sock,int iTime,int iTimerNum );
	
	#ifdef  __cplusplus
	}
	#endif

#endif

#endif
