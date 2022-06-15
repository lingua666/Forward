
#include <libThirdWrap_RtspClient/StreamClientState.h>
#include <liveMedia.hh>              //live555只要包含这两个头文件就可以了
#include <BasicUsageEnvironment.hh>   //live555只要包含这两个头文件就可以了，还要在项目属性里添加四个库的，和目录的路径


//实现"StreamClientState":

StreamClientState::StreamClientState( void )
	: _pIter(NULL)
	, _pSession(NULL)
	, _pSubsession(NULL)
	, _StreamTimerTask(NULL)
	, _Duration(0.0) 
{

}

StreamClientState::~StreamClientState( void )
{
	
}

void StreamClientState::Relase(MediaSubsessionIterator* pIter,
							MediaSession* pSession,
							MediaSubsession* pSubsession,
							TaskToken StreamTimerTask )
{
	if( pIter != NULL )
	{
		delete pIter;
		pIter = NULL;
	}

	if( pSession != NULL )
	{
		// 我们还需要删除“会话”,unschedule“streamTimerTask”(如果设置)
		UsageEnvironment& env = pSession->envir(); //赋值

		if( StreamTimerTask != NULL )	
			env.taskScheduler().unscheduleDelayedTask(StreamTimerTask);

		/*if( _pSubsession != NULL && _pSubsession->sink != NULL )
		{
			Medium::close(_pSubsession->sink);
			_pSubsession->sink = NULL;
		}*/

		Medium::close(pSession);
		pSession = NULL;
	}
}
