
#include <libMessage/TimerEvent.h>

TimerEvent* GetTimerEventInstance(void)
{
	return Singleton<TimerEvent>::instance();
}

TimerEvent::TimerEvent( void )
	: _isRun( false )
{

}

//ע��: �ù��캯����������ȫ�ֱ���������ᵼ���������̻߳�δ���������³��ִ���
TimerEvent::TimerEvent( UInt8 uThread )
	: _isRun( false )
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

void	TimerEvent::push_back( const Event_type& Event, UInt32 uInterval )
{
	tagEvent_INFO	tagInfo;
	tagInfo._Event = Event;
	tagInfo._uInterval = uInterval * 1000;
	tagInfo._Timer = Timestamp_type();
	_Lock.Lock();
	_EventList.push_back(tagInfo);
	_Lock.UnLock();
}

int	TimerEvent::push_back_sync( const Event_type& Event, UInt32 uInterval/*����*/ )
{
	Int8 iRet = 0;
	tagEvent_INFO	tagInfo;
	tagInfo._Event = function20_bind(&TimerEvent::HandleSync, this, Event, &iRet);
	tagInfo._uInterval = uInterval * 1000;
	tagInfo._Timer = Timestamp_type();
	_Lock.Lock();
	_EventList.push_back(tagInfo);
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
				Sleep(1);
			}
		}
		catch (const thread_interrupted& e)
		{
			switch( e.get_code() )
			{
			case ThreadData::en_INTERRUPTED_EXIT:	//�߳��˳�
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
