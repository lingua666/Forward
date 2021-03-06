
#include <libMessage/TimerEvent_V20.h>

TimerEvent_V20* GetTimerEvent_V20Instance(void)
{
	return Singleton<TimerEvent_V20>::instance();
}

TimerEvent_V20::TimerEvent_V20( void )
{

}

//注意: 该构造函数不可用于全局变量，否则会导致由于主线程还未启动，导致出现错误
TimerEvent_V20::TimerEvent_V20( UInt8 uThread )
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

void	TimerEvent_V20::push_back( const Event_type& Event, UInt32 uInterval,
	const Event_type& Begin, const Event_type& _End )
{
	tagEvent_INFO	tagInfo;
	tagInfo._Begin = Begin;
	tagInfo._Event = Event;
	tagInfo._End = _End;
	tagInfo._uInterval = uInterval * 1000;
	tagInfo._Timer = Timestamp_type();
	_Lock.Lock();
	_EventList.push_back(tagInfo);
	_Lock.UnLock();
}

void	TimerEvent_V20::WorkThread( void )
{
	tagEvent_INFO tagInfo;

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
					}
					else
					{
						_EventList.next();
						_Lock.UnLock();
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
				Sleep(10);
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

