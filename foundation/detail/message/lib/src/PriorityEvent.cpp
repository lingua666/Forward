
#include <libMessage/PriorityEvent.h>

PriorityEvent::PriorityEvent( void )
{

}

PriorityEvent::~PriorityEvent( void )
{

}

void	PriorityEvent::Init( UInt8 uThread )
{
	for(std::size_t i = 0; i < uThread; ++ i)
	{
		Thread_ptr thread( new Thread_type( function20_bind(&PriorityEvent::WorkThread, this) ) );
		_Threads.push_back(thread);
		thread->wait_run();
	}
}

void	PriorityEvent::Release( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Threads[i]->join();
	}
	_Threads.clear();

	_MesList.clear();
}

void	PriorityEvent::push_back( const Message_type& Message, int iPriority )
{
	_Lock.Lock();
	_MesList.push_back(Message, iPriority);
	_Lock.UnLock();
}

void	PriorityEvent::WorkThread( void )
{
	Message_type	Message;
	while( true )
	{
		try
		{
			ThreadWrap::interruption_point();
			if( _MesList.size() > 0 )
			{
				_Lock.Lock();
				if( _MesList.size() > 0 )
				{
					Message = _MesList.front();
					_MesList.pop_front();
					_Lock.UnLock();
					Message();
					Message.reset();
					goto gt_sleep;
				}
				else
				{
					_Lock.UnLock();
				}
			}
			else
			{
gt_sleep:
#if defined(LOW_POWER_FLAG_DEL)
				Sleep(10);
#else
				Sleep(1);
#endif
			}
		}
		catch (const thread_interrupted& e)
		{
			switch( e.get_code() )
			{
			case ThreadData::en_INTERRUPTED_EXIT:	//Ïß³ÌÍË³ö
				goto exit;
				break;
			}
			break;
		}
	}

exit:
	return ;
}

