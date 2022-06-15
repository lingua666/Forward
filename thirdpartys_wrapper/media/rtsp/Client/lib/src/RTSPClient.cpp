
#include <libThirdWrap_RtspClient/RTSPClient.h>
#include <libThirdWrap_RtspClient/DummySink.h>

#define RTSP_CLIENT_VERBOSITY_LEVEL 1 //默认情况下,从每个“RTSPClient”打印详细的输出

//默认情况下,我们请求服务器使用RTP / UDP流数据。
//如果,相反,你想请求RTP / TCP流数据,服务器通过RTP-over-TCP流,改变以下为真:
#define REQUEST_STREAMING_OVER_TCP False

void MyRTSPClient::ContinueAfterDESCRIBE( RTSPClient* rtspClient,
										int resultCode, char* resultString )
{
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(rtspClient);
	if( !pClient->_isClose )
		pClient->_pList->push_back(
						function20_bind(&MyRTSPClient::ContinueAfterDESCRIBE_Impl,
										rtspClient, resultCode, resultString) );
}

void MyRTSPClient::ContinueAfterDESCRIBE_Impl( RTSPClient* rtspClient,
	int resultCode, char* resultString )
{
	do
	{
		UsageEnvironment& env = rtspClient->envir(); // alias
		StreamClientState& scs = reinterpret_cast<MyRTSPClient*>(rtspClient)->_StreamState; // alias

		if (resultCode != 0) 
		{
			printf("Url(%s):Failed to get a SDP description: %s\r\n", rtspClient->url(), resultString);
			delete[] resultString;
			break;
		}

		//创建一个媒体会话对象从这个SDP描述:
		scs._pSession = MediaSession::createNew(env, resultString);

		delete[] resultString; //因为我们不需要它了
		if (scs._pSession == NULL) 
		{
			printf("Url(%s) Failed to create a MediaSession object from the SDP description: %s\r\n", rtspClient->url(), env.getResultMsg()); //创建媒体会话失败
			break;
		} 
		else if (!scs._pSession->hasSubsessions()) 
		{
			printf("This session has no media subsessions (i.e., no \"m=\" lines)\n");
			break;
		}

		//然后,创建和设置数据源对象的会话。我们通过遍历会话的“子”,
		//调用“MediaSubsession::初始化()”,然后发送一个RTSP“设置”命令,在每一个。
		//(每个“子”将有自己的数据源)。

		scs._pIter = new MediaSubsessionIterator(*scs._pSession);    //根据现有的会话创建一个子会话
		SetupNextSubsession_Impl(rtspClient);
		return;
	} while (0);

	// 发生了一个不可恢复的错误流。关闭流
	//ShutdownStream_Impl(rtspClient);
}

void MyRTSPClient::SetupNextSubsession( RTSPClient* rtspClient )
{
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(rtspClient);
	if( !pClient->_isClose )
		pClient->_pList->push_back(
			function20_bind(&MyRTSPClient::SetupNextSubsession_Impl,
						rtspClient) );
}

void MyRTSPClient::SetupNextSubsession_Impl( RTSPClient* rtspClient )
{
	UsageEnvironment& env = rtspClient->envir(); // 赋值
	StreamClientState& scs = reinterpret_cast<MyRTSPClient*>(rtspClient)->_StreamState; // 赋值

	scs._pSubsession = scs._pIter->next();
	if (scs._pSubsession != NULL) 
	{
		if (!scs._pSubsession->initiate())
		{
			printf("Url(%s) Failed to initiate the \%s/%s\ subsession: %s\r\n", rtspClient->url(), *scs._pSubsession->mediumName(), *scs._pSubsession->codecName(), env.getResultMsg());
			SetupNextSubsession_Impl(rtspClient); //放弃这子会话,去获取下一个
		} 
		else
		{
			scs._pSubsession->rtcpIsMuxed();
			// 继续建立这个子,通过发送一个RTSP“SETUIP”命令:
			rtspClient->sendSetupCommand(*scs._pSubsession, &MyRTSPClient::ContinueAfterSETUP, False, REQUEST_STREAMING_OVER_TCP);
		}
		return;
	}

	/*scs._pSession->_absStartTime() = "20161110T030000Z";
	// 我们已经完成了设置所有的子会话。现在,发送一个RTSP“PALY”命令来启动流:
	if (scs._pSession->absStartTime() != NULL)
	{
		//特殊情况:流被“absolute”时间,所以发送一个适当的“PLAY”命令:
		scs._Duration = scs._pSession->playEndTime() - scs._pSession->playStartTime();	
		rtspClient->sendPlayCommand(*scs._pSession,
			&MyRTSPClient::ContinueAfterPLAY,
			scs._pSession->absStartTime(),
			scs._pSession->absEndTime());
	} 
	else
	{*/
		rtspClient->sendPlayCommand(*scs._pSession, &MyRTSPClient::ContinueAfterPLAY);
	//}
}

void MyRTSPClient::ContinueAfterSETUP( RTSPClient* rtspClient,
								int resultCode, char* resultString )
{
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(rtspClient);

	if( !pClient->_isClose )
		pClient->_pList->push_back(
					function20_bind(&MyRTSPClient::ContinueAfterSETUP_Impl, rtspClient,
						resultCode, resultString) );
}

void MyRTSPClient::ContinueAfterSETUP_Impl( RTSPClient* rtspClient,
	int resultCode, char* resultString )
{
	do 
	{
		UsageEnvironment& env = rtspClient->envir(); // alias
		StreamClientState& scs = reinterpret_cast<MyRTSPClient*>(rtspClient)->_StreamState; // alias

		if (resultCode != 0) 
		{
			printf("Url(%s) Failed to set up the \%s/%s\ subsession: %s\r\n", rtspClient->url(), *scs._pSubsession->mediumName(), *scs._pSubsession->codecName(), resultString);
			break;
		}

		scs._pSubsession->rtcpIsMuxed();

		//成功设置子连接,创建一个数据接收器,并称之为“startPlaying()”。
		//(这将准备数据接收器接收数据,实际的数据流从客户机之后,才开始发生
		//后我们发送一个RTSP“PLAY”命令。)

		scs._pSubsession->sink = new DummySink(env, *scs._pSubsession, rtspClient->url());
		// 应许使用自己的自定义“MediaSink”子类
		if (scs._pSubsession->sink == NULL) 
		{
			printf("Url(%s) Failed to create a data sink for the \%s/%s\ subsession: %s\r\n", rtspClient->url(), *scs._pSubsession->mediumName(), *scs._pSubsession->codecName(), env.getResultMsg());
			break;
		}

		reinterpret_cast<DummySink*>(scs._pSubsession->sink)->SetRealDataCallBack(reinterpret_cast<MyRTSPClient*>(rtspClient)->GetRealDataCallBack());
		scs._pSubsession->miscPtr = rtspClient;                                  // a hack to let subsession handle functions get the "RTSPClient" from the subsession 
		scs._pSubsession->sink->startPlaying(*(scs._pSubsession->readSource()),
			&MyRTSPClient::SubsessionAfterPlaying, scs._pSubsession);
		// 还设置一个处理程序被称为如果服务器“bye”到这个子:
		if (scs._pSubsession->rtcpInstance() != NULL)
		{
			scs._pSubsession->rtcpInstance()->setByeHandler(&MyRTSPClient::SubsessionByeHandler, scs._pSubsession);
		}
	} while (0);

	delete[] resultString;

	// 设置下一个子会话，如果可能:
	SetupNextSubsession_Impl(rtspClient);
}

void MyRTSPClient::ContinueAfterPLAY( RTSPClient* rtspClient,
	int resultCode, char* resultString )
{
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(rtspClient);

	if( !pClient->_isClose )
		pClient->_pList->push_back(
			function20_bind(&MyRTSPClient::ContinueAfterPLAY_Impl, rtspClient,
				resultCode, resultString) );
}

void MyRTSPClient::ContinueAfterPLAY_Impl( RTSPClient* rtspClient,
	int resultCode, char* resultString )
{
	Boolean success = False;

	do {
		UsageEnvironment& env = rtspClient->envir(); // alias
		StreamClientState& scs = ((MyRTSPClient*)rtspClient)->_StreamState; // alias

		if (resultCode != 0)
		{
			printf("Url(%s) Failed to start playing session:%s\r\n", rtspClient->url(), resultString);
			break;
		}

		//设置一个定时器处理结束时流的预期持续时间(如果流尚未结束的信号
		//使用一个服务器“bey”)。这是可选的。相反,如果你想保持流活动——如。以后,你可以
		//“寻求”在它,做一个学习“PLAY”,那么您可以省略这段代码。
		//(或者,如果您不想接收整个流,你可以设置这个定时器对一些短值)。
		if (scs._Duration > 0) 
		{
			unsigned const delaySlop = 2; // 流的预期持续时间。(这是可选的。)
			scs._Duration += delaySlop;
			unsigned uSecsToDelay = (unsigned)(scs._Duration * 1000000);
			scs._StreamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)StreamTimerHandler, rtspClient);
		}

		success = True;   // true = 1

	} while (0);
	delete[] resultString;

	if (!success)   //如果等于0的话，关闭流
	{
		// 
		//ShutdownStream_Impl(rtspClient);
	}
}

void MyRTSPClient::SubsessionAfterPlaying( void* clientData )
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(subsession->miscPtr);

	if( !pClient->_isClose )
		pClient->_pList->push_back(
			function20_bind(&MyRTSPClient::SubsessionAfterPlaying_Impl, clientData) );
}

void MyRTSPClient::SubsessionAfterPlaying_Impl( void* clientData )
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);

	// 首先关闭这个子流:
	Medium::close(subsession->sink);
	subsession->sink = NULL;

	// 接下来,检查是否* *子的流已经被关闭:
	MediaSession& session = subsession->parentSession();
	MediaSubsessionIterator iter(session);
	while ((subsession = iter.next()) != NULL) 
	{
		if (subsession->sink != NULL) 
			return; //这个子仍然活跃
	}

	//关闭流
	//ShutdownStream_Impl(rtspClient);
}

void MyRTSPClient::SubsessionByeHandler( void* clientData )
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(subsession->miscPtr);

	if( !pClient->_isClose )
		pClient->_pList->push_back(
			function20_bind(&MyRTSPClient::SubsessionByeHandler_Impl, clientData) );
}

void MyRTSPClient::SubsessionByeHandler_Impl( void* clientData )
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	RTSPClient* rtspClient = (RTSPClient*)subsession->miscPtr;

	// 现在充当如果子连接已经关闭:
	SubsessionAfterPlaying_Impl(subsession);
}

void MyRTSPClient::StreamTimerHandler( void* clientData )
{
	MyRTSPClient* pClient = reinterpret_cast<MyRTSPClient*>(clientData);

	if( !pClient->_isClose )
		pClient->_pList->push_back(
			function20_bind(&MyRTSPClient::StreamTimerHandler_Impl, clientData) );
}

void MyRTSPClient::StreamTimerHandler_Impl( void* clientData )
{
	MyRTSPClient* rtspClient = (MyRTSPClient*)clientData;
	StreamClientState& scs = rtspClient->_StreamState; // 赋值

	scs._StreamTimerTask = NULL;

	// Shut down the stream:关闭流
	ShutdownStream(rtspClient);
}

void MyRTSPClient::ShutdownStream( RTSPClient* rtspClient, int exitCode )
{
	((MyRTSPClient*)rtspClient)->_isClose = true;
	reinterpret_cast<MyRTSPClient*>(rtspClient)->_pList->push_back(
		function20_bind(&MyRTSPClient::ShutdownStream2, rtspClient, exitCode) );
}

void MyRTSPClient::ShutdownStream2( RTSPClient* rtspClient, int exitCode )
{
	reinterpret_cast<MyRTSPClient*>(rtspClient)->_pList->push_back(
		function20_bind(&MyRTSPClient::ShutdownStream_Impl, rtspClient, exitCode) );
}

void MyRTSPClient::ShutdownStream_Impl( RTSPClient* rtspClient, int exitCode )
{
	MyRTSPClient* pClient = ((MyRTSPClient*)rtspClient);
	StreamClientState& scs = pClient->_StreamState; // 赋值

	// 首先,检查是否有子仍被关闭:
	if (scs._pSession != NULL)
	{ 
		Boolean someSubsessionsWereActive = False;
		MediaSubsessionIterator iter(*scs._pSession);
		MediaSubsession* subsession;

		while ((subsession = iter.next()) != NULL) 
		{
			if (subsession->sink != NULL) 
			{
				Medium::close(subsession->sink);
				subsession->sink = NULL;

				if (subsession->rtcpInstance() != NULL) 
				{
					subsession->rtcpInstance()->setByeHandler(NULL, NULL); //  "TEARDOWN"如果服务器发送一个媒体服务器处理“TEARDOWN”“BYE”,
				}

				someSubsessionsWereActive = True;
			}
		}

		if ( someSubsessionsWereActive ) 
		{
			//发送一个RTSP“TEARDOWN”命令,告诉服务器关闭流。
			//别烦处理应对“TEARDOWN”
			rtspClient->sendTeardownCommand(*scs._pSession, NULL);
		}

		Medium::close(scs._pSession);
	}

	pClient->reset();
	//Medium::close(rtspClient);

	pClient->_pList->push_back(
		function20_bind(&Medium::close, rtspClient) );
}

MyRTSPClient::MyRTSPClient( UsageEnvironment& Env, Event_list* pList, char const* pRtspURL )
	: RTSPClient( Env, pRtspURL, RTSP_CLIENT_VERBOSITY_LEVEL, "live555client", 0, -1)
	, _pList( pList )
	, _isClose( false )
{

}

MyRTSPClient::~MyRTSPClient( void )
{
	
}

void MyRTSPClient::SetRealDataCallBack( const HFNRealData& hRealData )
{
	_hRealData = hRealData;
}

unsigned int MyRTSPClient::SendDescribeCommand( void )
{
	return sendDescribeCommand(&MyRTSPClient::ContinueAfterDESCRIBE);   //使用回调函数的格式
}

int	MyRTSPClient::Close( void )
{
	ShutdownStream(this);
	return 1;
}