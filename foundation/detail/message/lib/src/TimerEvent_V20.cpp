
#include <libMessage/TimerEvent_V20.h>

TimerEvent_V20* GetTimerEvent_V20Instance(void)
{
	return Singleton<TimerEvent_V20>::instance();
}

TimerEvent_V20::TimerEvent_V20( void )
		: _uMillisecond(10)
{

}

//注意: 该构造函数不可用于全局变量，否则会导致由于主线程还未启动，导致出现错误
TimerEvent_V20::TimerEvent_V20( UInt8 uThread )
	: _uMillisecond(10)
{
	Init(uThread);
}

TimerEvent_V20::~TimerEvent_V20( void )
{
	Release();
}

void	TimerEvent_V20::Init( UInt8 uThread )
{
	if( _Threads.size() <= 0 )
	{
		for(std::size_t i = 0; i < uThread; ++ i)
		{
			Thread_ptr thread( new Thread_type( function20_bind(&TimerEvent_V20::WorkThread, this) ) );
			_Threads.push_back(thread);
			thread->wait_run();
		}
	}
}

void	TimerEvent_V20::CloseAll( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
	}

	Clear();
}

int	TimerEvent_V20::push_back( const Event_type& Event, UInt64 uInterval,
	const Event_type& Begin, const Event_type& _End )
{
	if (_Threads.size() <= 0)
		return -1;

	tagEvent_INFO	tagInfo;
	tagInfo._Begin = Begin;
	tagInfo._Event = Event;
	tagInfo._End = _End;
	tagInfo._uInterval = uInterval * 1000;
	_Lock.Lock();
	_EventList.push_back(tagInfo);
	tagInfo._Timer = Timestamp_type();
	_Lock.UnLock();
	return 1;
}

void	TimerEvent_V20::WorkThread( void )
{
	tagEvent_INFO tagInfo;
	int iInc = 0;

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
						if( tagInfo._Begin )
						{
							tagInfo._Begin();
						}
						_Lock.UnLock();
						tagInfo._Event();
						tagInfo._Event.reset();
						if( tagInfo._End )
						{
							tagInfo._End();
						}

						iInc = -1;
					}
					else
					{
						_EventList.next();
						_Lock.UnLock();
					}

					if ((++iInc) >= _EventList.size())
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
	return ;
}

void	TimerEvent_V20::Release( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Threads[i]->join();
	}
	_Threads.clear();

	_EventList.clear();
}

void	TimerEvent_V20::Clear( void )
{
	_Lock.Lock();
	_EventList.clear();
	_Lock.UnLock();
}

void	TimerEvent_V20::Lock(void)
{
	_Lock.Lock();
}

void	TimerEvent_V20::UnLock(void)
{
	_Lock.UnLock();
}

void	TimerEvent_V20::UpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime)
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
void	TimerEvent_V20::SetSleepStep(UInt32 uMillisecond)
{
	_uMillisecond = uMillisecond;
}
