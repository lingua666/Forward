
#include <liveMedia.hh>
#include <libThirdWrap_RtspClient/RTSPClient.h>

//默认情况下,我们请求服务器使用RTP / UDP流数据。
//如果,相反,你想请求RTP / TCP流数据,服务器通过RTP-over-TCP流,改变以下为真:
#define REQUEST_STREAMING_OVER_TCP False
#define RTSP_CLIENT_VERBOSITY_LEVEL 1 //默认情况下,从每个“RTSPClient”打印详细的输出

class RTSPClientManager;
class MyRTSPClient; 

class MyTaskScheduler
{
public:
	friend	class RTSPClientManager;

protected:
	MyTaskScheduler( void );

	~MyTaskScheduler( void );

	void doEventUnLoop( void );

	void doEventLoop( char* watchVariable );

private:
	TaskScheduler* _pTask;
};

class RTSPClientManager
{
public:
	typedef MyRTSPClient::Event_list Event_list;

private:
	static DWORD WINAPI WorkerThread( void* pParam );

public:
	RTSPClientManager( void );

	~RTSPClientManager( void );

	int Init( void );

	void Release( void );

	MyRTSPClient* Open( char const* pRtspURL );

	int Close( MyRTSPClient* pClient );

private:
	HANDLE	_hThread;
	Event_list	_evList;
	bool	_isWorker;
	MyTaskScheduler _Scheduler;
	UsageEnvironment* _pEnv;
	long			_UserCounter;
};


