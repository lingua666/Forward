
#include <libMessage/TimerEventKM.h>

TimerEventKM* GetTimerEventKMInstance(void)
{
	return Singleton<TimerEventKM>::instance();
}

TimerEventKM::tagEvent_sptr TimerEventKM::Alloc(void)
{
	static MemPool_type s_Pool(sizeof(tagEvent_INFO));
	return tagEvent_sptr(s_Pool.AllocObj<tagEvent_INFO>(),
		function20_bind_Ex(&MemPool_type::FreeObj<tagEvent_INFO>, &s_Pool));
}

TimerEventKM::TimerEventKM( void )
	: _isRun( false )
	, _uMillisecond(10)
{

}

//注意: 该构造函数不可用于全局变量，否则会导致由于主线程还未启动，导致出现错误
TimerEventKM::TimerEventKM( UInt8 uThread )
	: _isRun( false )
	, _uMillisecond(10)
{
	Init(uThread);
}

TimerEventKM::~TimerEventKM( void )
{
	Release();
}

void	TimerEventKM::Init( UInt8 uThread )
{
	if( _Threads.size() <= 0 )
	{
		for(std::size_t i = 0; i < uThread; ++ i)
		{
			Thread_ptr thread( new Thread_type( function20_bind(&TimerEventKM::WorkThread, this) ) );
			_Threads.push_back(thread);
			thread->wait_run();
		}
	}
}

void	TimerEventKM::CloseAll( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
	}

	Clear();
}

TimerEventKM::_string_	TimerEventKM::push_back( const Event_type& Event, UInt64 uInterval )
{
	if (_Threads.size() <= 0)
		return "";

	tagEvent_sptr sptr = Alloc();
	sptr->_Event = Event;
	sptr->_uInterval = uInterval * 1000;
	sptr->_Uuid = _uuid_::UUIDGenerator().UUIDToString().c_str();
	sptr->_isRun = true;

	_Lock.Lock();
	_EventList.push_back(sptr);
	_EventMap.insert(std::make_pair(sptr->_Uuid, sptr));
	sptr->_Timer = Timestamp_type();
	_Lock.UnLock();

	return sptr->_Uuid;
}

int	TimerEventKM::push_back_sync( const Event_type& Event, UInt64 uInterval/*毫秒*/ )
{
	Int8 iRet = 0;

	tagEvent_sptr sptr = Alloc();
	sptr->_Event = function20_bind(&TimerEventKM::HandleSync, this, Event, &iRet);
	sptr->_uInterval = uInterval * 1000;
	sptr->_Uuid = _uuid_::UUIDGenerator().UUIDToString().c_str();
	sptr->_isRun = true;
	
	_Lock.Lock();
	_EventList.push_back(sptr);
	_EventMap.insert(std::make_pair(sptr->_Uuid, sptr));
	sptr->_Timer = Timestamp_type();
	_Lock.UnLock();

	while (iRet == 0 && _isRun)
	{
		Sleep(1);
	};

	if( iRet == 0 )
		return -1;

	return 1;
}

void TimerEventKM::Remove(const _string_& sUuid)
{
	bool isRun = false;
	tagEvent_sptr sptr;
	_Lock.Lock();
	EventMap::iterator iter = _EventMap.find(sUuid);
	if (iter != _EventMap.end())
	{
		sptr = iter->second;
		isRun = sptr->_isRun;
		sptr->_isRun = false;
	}
	_Lock.UnLock();

	if(isRun)
		sptr->_Event.reset();
}

void	TimerEventKM::HandleSync( const Event_type& Event, Int8* iRet )
{
	if( Event )
		Event();
	*iRet = 1;
}

void	TimerEventKM::WorkThread( void )
{
	bool isEventRun = false;
	int iInc = 0;

	_isRun = true;

	while( true )
	{
		try
		{
			ThreadWrap::interruption_point();
			if (_EventList.size() > 0)
			{
				_Lock.Lock();
				if (_EventList.size() > 0)
				{
					tagEvent_sptr sptr = _EventList.current();
					if (Timestamp_type().epochMicroseconds() - sptr->_Timer.epochMicroseconds() >= sptr->_uInterval)
					{
gt_remove:
						_EventList.pop();
						_EventMap.erase(sptr->_Uuid);
						isEventRun = sptr->_isRun;
						sptr->_isRun = false;
						_Lock.UnLock();
						if (isEventRun)
						{
							sptr->_Event();
							sptr->_Event.reset();
						}

						iInc = -1;
					}
					else
					{
						if (!sptr->_Event)
						{//已被移除
							goto gt_remove;
						}
						else
						{
							_EventList.next();
							_Lock.UnLock();
						}
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
	_isRun = false;
	return ;
}

void	TimerEventKM::Release( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Threads[i]->join();
	}
	_Threads.clear();
	
	Clear();
}

void	TimerEventKM::Clear( void )
{
	_Lock.Lock();
	_EventList.clear();
	_EventMap.clear();
	_Lock.UnLock();
}

int		TimerEventKM::size( void )
{
	return _EventMap.size();
}

void	TimerEventKM::Lock(void)
{
	_Lock.Lock();
}

void	TimerEventKM::UnLock(void)
{
	_Lock.UnLock();
}

void	TimerEventKM::UpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime)
{
	tagEvent_sptr sptr;
	for(EventMap::iterator iter = _EventMap.begin(); iter != _EventMap.end(); iter++)
	{
		sptr = iter->second;
		if (sptr && sptr->_isRun)
		{
			sptr->_Timer = NewTime - (LocalTime - sptr->_Timer);
		}
	}
}

//单位毫秒
void	TimerEventKM::SetSleepStep(UInt32 uMillisecond)
{
	_uMillisecond = uMillisecond;
}

