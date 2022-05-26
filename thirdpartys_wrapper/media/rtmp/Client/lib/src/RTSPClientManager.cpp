
#include <libThirdWrap_RtspClient/RTSPClientManager.h>
#include <libThread/ThreadWrap.h>

MyTaskScheduler::MyTaskScheduler( void )
{
	_pTask = reinterpret_cast<TaskScheduler*>(BasicTaskScheduler::createNew());
}

MyTaskScheduler::~MyTaskScheduler( void )
{
	if( _pTask != NULL )
	{
		delete _pTask;
		_pTask = NULL;
	}
}

void MyTaskScheduler::doEventUnLoop( void )
{
	reinterpret_cast<BasicTaskScheduler0*>(_pTask)->SingleStep();
}

void MyTaskScheduler::doEventLoop( char* watchVariable )
{
	while (1) 
	{
		if( watchVariable != NULL && *watchVariable != 0 )
			break;

		reinterpret_cast<BasicTaskScheduler0*>(_pTask)->SingleStep();
	}
}


RTSPClientManager::RTSPClientManager( void )
	: _UserCounter( 0 )
	, _pEnv( NULL )
	, _hThread( NULL )
	, _isWorker( false )
{

}

RTSPClientManager::~RTSPClientManager( void )
{
	Release();
}

int RTSPClientManager::Init( void )
{
	if( _pEnv == NULL )
	{
		if( _Scheduler._pTask == NULL )
			return -1;

		_pEnv = BasicUsageEnvironment::createNew( *_Scheduler._pTask );
		if( _pEnv == NULL )
			return -1;
	}

	if( _hThread == NULL )
	{
		UInt64	uThreadId;
		_isWorker = true;
		_hThread = _thread_::ThreadAPI::CreateThread( &RTSPClientManager::WorkerThread, this, uThreadId );
	}

	return 1;
}

void RTSPClientManager::Release( void )
{
	if( _pEnv != NULL )
	{
		_pEnv->reclaim();
		_pEnv = NULL;

		if( _hThread != NULL )
		{
			_isWorker = false;
			_thread_::ThreadAPI::Wait(_hThread, INFINITE);
			CloseHandle(_hThread);
			_hThread = NULL;
		}
	}
}

MyRTSPClient* RTSPClientManager::Open( char const* pRtspURL )
{
	if( _pEnv == NULL )
		return NULL;

	// 首先创建一个“RTSPClient”对象。请注意,有一个单独的“RTSPClient”对象为每个流,我们的愿望
	//接收(即使多流使用相同的rtsp:/ / URL)。
	MyRTSPClient* pClient = new MyRTSPClient(*_pEnv, &_evList, pRtspURL);
	if ( pClient == NULL )
	{
		printf("Failed to create a RTSP client for URL(%s), ResultMsg:%s\r\n", pRtspURL, _pEnv->getResultMsg());
		return NULL;
	}

	++ _UserCounter;

	//然后,rtspClient发送一个RTSP“DESCRIBE”命令,SDP描述流
	//注意这个命令——像所有RTSP命令发送异步;我们不阻止,等待一个响应。
	//相反,下面的函数调用立即返回,我们处理RTSP响应后,从内部事件循环:

	pClient->sendDescribeCommand(&MyRTSPClient::ContinueAfterDESCRIBE);   //使用回调函数的格式
	return pClient;
}

int RTSPClientManager::Close( MyRTSPClient* pClient )
{
	if( pClient == NULL )
		return -1;

	MyRTSPClient::ShutdownStream(pClient);
	return 1;
}


DWORD WINAPI RTSPClientManager::WorkerThread( void* pParam )
{
	RTSPClientManager* self = reinterpret_cast<RTSPClientManager*>(pParam);
	while( self->_isWorker )
	{
		if(self->_evList.size() > 0)
		{
			self->_evList.front()();
			self->_evList.pop();
		}

		self->_Scheduler.doEventUnLoop();
	}

	return 0;
}