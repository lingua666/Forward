#ifndef __MYRTSPCLIENT_20170213155004_1444199361_H__
#define __MYRTSPCLIENT_20170213155004_1444199361_H__

#include "External.h"
#include "StreamClientState.h"
#include <BasicUsageEnvironment.hh>
#include <RTSPClient.hh>

class MyRTSPClient : public RTSPClient
{
public:
	typedef	function20_handle< void ( void )> HFNEvent;
	typedef function20_handle<void (UInt8*, UInt32)>	 HFNRealData;
	typedef	_container_::LinkList< HFNEvent > Event_list;

	static void ContinueAfterDESCRIBE_Impl( RTSPClient* rtspClient,
									int resultCode, char* resultString);

	static void SetupNextSubsession_Impl( RTSPClient* rtspClient );

	static void ContinueAfterSETUP_Impl( RTSPClient* rtspClient,
		int resultCode, char* resultString);

	static void ContinueAfterPLAY_Impl( RTSPClient* rtspClient,
		int resultCode, char* resultString);
	
	static void SubsessionAfterPlaying_Impl( void* clientData );

	static void SubsessionByeHandler_Impl( void* clientData );

	static void StreamTimerHandler_Impl( void* clientData );

	static void ShutdownStream_Impl( RTSPClient* rtspClient, int exitCode = 1 );

public:
	static void ContinueAfterDESCRIBE( RTSPClient* rtspClient,
		int resultCode, char* resultString);

	static void SetupNextSubsession( RTSPClient* rtspClient );

	static void ContinueAfterSETUP( RTSPClient* rtspClient,
		int resultCode, char* resultString);

	static void ContinueAfterPLAY( RTSPClient* rtspClient,
		int resultCode, char* resultString);

	static void SubsessionAfterPlaying( void* clientData );

	static void SubsessionByeHandler( void* clientData );

	static void StreamTimerHandler( void* clientData );

	static void ShutdownStream( RTSPClient* rtspClient, int exitCode = 1 );

	static void ShutdownStream2( RTSPClient* rtspClient, int exitCode );

public:
	MyRTSPClient( UsageEnvironment& Env, Event_list* pList, char const* pRtspURL );

	~MyRTSPClient( void );
	
	unsigned int SendDescribeCommand( void );

	void SetRealDataCallBack( const HFNRealData& hRealData );

	int	Close( void );

	const HFNRealData& GetRealDataCallBack( void ) const
	{
		return _hRealData;
	}

private:
	HFNRealData			_hRealData;
	StreamClientState	_StreamState;
	Event_list* _pList;
	bool	_isClose;
};

#endif
