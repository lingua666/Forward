
#include <libMessage/TimerEventSmart.h>

TimerEventSmart* GetTimerEventSmartInstance(void)
{
	return Singleton<TimerEventSmart>::instance();
}

TimerEventSmart::TimerEventSmart( void )
	: _uMillisecond(10)
{

}

//ע��: �ù��캯����������ȫ�ֱ���������ᵼ���������̻߳�δ���������³��ִ���
TimerEventSmart::TimerEventSmart( UInt8 uThread )
		: _uMillisecond(10)
{
	Init(uThread);
}

TimerEventSmart::~TimerEventSmart( void )
{
	Release();
}

void	TimerEventSmart::Init( UInt8 uThread )
{
	if( _Threads.size() <= 0 )
	{
		for(std::size_t i = 0; i < uThread; ++ i)
		{
			Thread_ptr thread( new Thread_type( function20_bind(&TimerEventSmart::WorkThread, this) ) );
			_Threads.push_back(thread);
			thread->wait_run();
		}
	}
}

void	TimerEventSmart::CloseAll( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
	}
}

int	TimerEventSmart::push_back( const Event_type& Event, UInt64 uInterval,
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

void	TimerEventSmart::WorkThread( void )
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
					{//��ѯһ��
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
			case ThreadData::en_INTERRUPTED_EXIT:	//�߳��˳�
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

void	TimerEventSmart::Release( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Threads[i]->join();
	}
	_Threads.clear();

	_EventList.clear();
}

void	TimerEventSmart::Clear( void )
{
	_Lock.Lock();
	_EventList.clear();
	_Lock.UnLock();
}

//��λ����
void	TimerEventSmart::SetSleepStep(UInt32 uMillisecond)
{
	_uMillisecond = uMillisecond;
}

