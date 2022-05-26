#ifndef __STREAMCLIENTSTATE_20170213155004_1444199361_H__
#define __STREAMCLIENTSTATE_20170213155004_1444199361_H__

#include "External.h"
#include <liveMedia.hh>

class MyRTSPClient;

class StreamClientState 
{
public:
	friend class MyRTSPClient;

	static void Relase(MediaSubsessionIterator* _pIter,
					MediaSession* _pSession,
					MediaSubsession* _pSubsession,
					TaskToken _StreamTimerTask );

protected:
	StreamClientState( void );

	~StreamClientState( void );

protected:
	MediaSubsessionIterator* _pIter;
	MediaSession* _pSession;
	MediaSubsession* _pSubsession;
	TaskToken _StreamTimerTask;
	double _Duration;
};

#endif

