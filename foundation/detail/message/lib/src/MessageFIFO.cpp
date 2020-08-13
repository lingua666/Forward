
#include <libMessage/MessageFIFO.h>

MessageFIFO::MessageFIFO( void )
{

}

MessageFIFO::~MessageFIFO( void )
{

}

void	MessageFIFO::Init( UInt8 uThread )
{
	for(std::size_t i = 0; i < uThread; ++ i)
	{
		Thread_ptr thread( new Thread_type( function20_bind(&MessageFIFO::WorkThread, this) ) );
		_Threads.push_back(thread);
		thread->wait_run();
	}
}

void	MessageFIFO::Release( void )
{
	for(std::size_t i = 0; i < _Threads.size(); ++ i)
	{
		_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		_Threads[i]->join();
	}
	_Threads.clear();

	_MesList.clear();
}

void	MessageFIFO::push_back( const Message_type& Message )
{
	_Lock.Lock();
	_MesList.push_back(Message);
	_Lock.UnLock();
}

void	MessageFIFO::WorkThread( void )
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
				Message = _MesList.front();
				_MesList.pop();
				_Lock.UnLock();
				Message();
				Message.reset();
			}
			else
			{
				Sleep(1);
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

