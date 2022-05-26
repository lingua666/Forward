
#include <libMessage/TimerEvent.h>

TimerEvent* GetTimerEventInstance(void)
{
	return Singleton<TimerEvent>::instance();
}

TimerEvent::TimerEvent( void )
	: _isRun( false )
	, _uMillisecond(10)
{

}

//注意: 该构造函数不可用于全局变量，否则会导致由于主线程还未启动，导致出现错误
TimerEvent::TimerEvent( UInt8 uThread )
	: _isRun( false )
	, _uMillisecond(10)
{
	Init(uThread);
}

TimerEvent::~TimerEvent( void )
{
	Release();
}

void	TimerEvent::Init( UInt8 uThread )
{
	if( _Threads.size() <= 0 )
	{
		for(std::size_t i = 0; i < uThread; ++ i)
		{
			Thread_ptr thread( new Thread_type( function20_bind(&TimerEvent::WorkThread, this) ) );
			_Threads.push_back(thread);
			thread->wait_run();
		}
	}
}

void	TimerEvent::CloseAll( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
	}

	Clear();
}

int	TimerEvent::push_back( const Event_type& Event, UInt64 uInterval )
{
	if (_Threads.size() <= 0)
		return -1;

	tagEvent_INFO	tagInfo;
	tagInfo._Event = Event;
	tagInfo._uInterval = uInterval * 1000;
	tagInfo._isExec = true;
	_Lock.Lock();
	_EventList.push_back(tagInfo);
	tagInfo._Timer = Timestamp_type();
	_Lock.UnLock();

	return 1;
}

int	TimerEvent::push_back_sync( const Event_type& Event, UInt64 uInterval/*毫秒*/ )
{
	if (_Threads.size() <= 0)
		return -1;

	Int8 iRet = 0;
	tagEvent_INFO	tagInfo;
	tagInfo._Event = function20_bind(&TimerEvent::HandleSync, this, Event, &iRet);
	tagInfo._uInterval = uInterval * 1000;
	tagInfo._isExec = true;
	_Lock.Lock();
	_EventList.push_back(tagInfo);
	tagInfo._Timer = Timestamp_type();
	_Lock.UnLock();
	while( iRet == 0 && _isRun )
	{
		Sleep(1);
	}

	if( iRet == 0 )
		return -1;

	return 1;
}

void	TimerEvent::HandleSync( const Event_type& Event, Int8* iRet )
{
	if( Event )
		Event();
	*iRet = 1;
}

void	TimerEvent::WorkThread( void )
{
	tagEvent_INFO tagInfo;
	int iInc = 0;
	_isRun = true;

	while( true )
	{
		try
		{
			ThreadWrap::interruption_point();
			if( _EventList.size() > 0 )
			{
				_Lock.Lock();
				if( _EventList.size() > 0 )
				{
					tagInfo = _EventList.current();
					if( Timestamp_type().epochMicroseconds() - tagInfo._Timer.epochMicroseconds() >= tagInfo._uInterval )
					{
						_EventList.pop();
						_Lock.UnLock();
						tagInfo._Event();
						tagInfo._Event.reset();
						iInc = -1;
					}
					else
					{
						_EventList.next();
						_Lock.UnLock();
					}

					if ((++ iInc) >= _EventList.size())
					{//轮询一轮
						goto gt_sleep;
					}
				}
				else
				{
					_Lock.UnLock();
					goto gt_sleep;
				}
			}
			else
			{
gt_sleep:
				iInc = 0;
				Sleep(_uMillisecond);
			}
		}
		catch (const thread_interrupted& e)
		{
			switch( e.get_code() )
			{
			case ThreadData::en_INTERRUPTED_EXIT:	//线程退出
				goto exit;
				break;
			}
			break;
		}
	}

exit:
	//Release();
	_isRun = false;
	return ;
}

void	TimerEvent::Release( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Threads[i]->join();
	}
	_Threads.clear();

	_EventList.clear();
}

void	TimerEvent::Clear( void )
{
	_Lock.Lock();
	_EventList.clear();
	_Lock.UnLock();
}

int		TimerEvent::size( void )
{
	return _EventList.size();
}

void	TimerEvent::Lock(void)
{
	_Lock.Lock();
}

void	TimerEvent::UnLock(void)
{
	_Lock.UnLock();
}

void	TimerEvent::UpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime)
{
	tagEvent_INFO tagInfo;
	for (int i = 0; i < _EventList.size(); i++)
	{
		tagInfo = _EventList.current();
		tagInfo._Timer = NewTime - (LocalTime - tagInfo._Timer);
		_EventList.next();
	}
}

//单位毫秒
void	TimerEvent::SetSleepStep(UInt32 uMillisecond)
{
	_uMillisecond = uMillisecond;
}
