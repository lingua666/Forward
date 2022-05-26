
#ifndef __TIMEREVENTVSYNC_20160504200104_1462363264_H__
#define __TIMEREVENTVSYNC_20160504200104_1462363264_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libContainer/CycleList.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libTimestamp/Timestamp.h>
#include <vector>

template<typename R>
class ITimerEventSync
{
public:
	typedef	ThreadWrap	Thread_type;
	typedef SmartPTR<Thread_type>	Thread_ptr;
	typedef std::vector<Thread_ptr>	ThreadList;
	typedef function20_handle<R (void)>	Event_type;

	struct tagEvent_INFO
	{
		Event_type		_Event;
		Timestamp_type	_Timer;
		UInt64			_uInterval;	//微秒
	};

	typedef _container_::CycleList<tagEvent_INFO,CNoneLock>	EventList_type;

public:
	virtual void	Init( UInt8 uThread )
	{
		if( _Threads.size() <= 0 )
		{
			for(std::size_t i = 0; i < uThread; ++ i)
			{
				Thread_ptr thread( new Thread_type( function20_bind(&ITimerEventSync::WorkThread, this) ) );
				_Threads.push_back(thread);
				thread->wait_run();
			}
		}
	}

	virtual void	CloseAll( void )
	{
		for(std::size_t i = 0; i < _Threads.size(); ++ i)
		{
			_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
		}
	}

	virtual void	Release( void )
	{
		for(std::size_t i = 0; i < _Threads.size(); ++ i)
		{
			_Threads[i]->interrupt(ThreadData::en_INTERRUPTED_EXIT);
			_Threads[i]->join();
		}
		_Threads.clear();

		this->_EventList.clear();
	}

	virtual void	Clear( void )
	{
		this->_Lock.Lock();
		this->_EventList.clear();
		this->_Lock.UnLock();
	}

	virtual void	WorkThread( void )
	{
		tagEvent_INFO	tagInfo;
		int iInc = 0;

		while( true )
		{
			try
			{
				ThreadWrap::interruption_point();
				if( this->_EventList.size() > 0 )
				{
					this->_Lock.Lock();
					if( this->_EventList.size() > 0 )
					{
						tagInfo = this->_EventList.current();
						if( Timestamp_type().epochMicroseconds() - tagInfo._Timer.epochMicroseconds() >= tagInfo._uInterval )
						{
							this->_EventList.pop();
							this->_Lock.UnLock();
							tagInfo._Event();
							tagInfo._Event.reset();
							iInc = -1;
						}
						else
						{
							this->_EventList.next();
							this->_Lock.UnLock();
						}
					}
					else
					{
						this->_Lock.UnLock();
						goto gt_sleep;
					}

					if ((++iInc) >= _EventList.size())
					{//轮询一轮
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

	//单位毫秒
	void	SetSleepStep(UInt32 uMillisecond)
	{
		_uMillisecond = uMillisecond;
	}

protected:
	CLock		_Lock;
	EventList_type	_EventList;
	ThreadList	_Threads;
	UInt32 		_uMillisecond;
};


/*!
* @class  TimerEventSync
* @brief   TimerEventSync
* @{
*/
template<typename R>
class TimerEventSync : public ITimerEventSync<R>
{
public:
	typedef typename ITimerEventSync<R>::Event_type		Event_type;
	typedef typename ITimerEventSync<R>::tagEvent_INFO	tagEvent_INFO;
	
public:
	/*!
	* @function   TimerEventSync
	* @brief    
	* @return  
	*/
	TimerEventSync( void )
	{
		this->_uMillisecond = 10;
	}

	/*!
	* @function   ~TimerEventSync
	* @brief    
	* @return  
	*/
	~TimerEventSync( void )
	{
		this->Release();
	}

	R	push_back( const Event_type& Event, UInt64 uInterval/*毫秒*/ )
	{
		R Ret;
		int iFinish = -1;
		tagEvent_INFO	tagInfo;
		tagInfo._Event = function20_bind(&TimerEventSync::HandleProcess, this, &Ret, Event, &iFinish);
		tagInfo._uInterval = uInterval * 1000;
		this->_Lock.Lock();
		this->_EventList.push_back(tagInfo);
		this->_Lock.UnLock();
		tagInfo._Timer = Timestamp_type();

		bool isStart = false;
		while( iFinish != 1 )
		{
			if( !isStart )
			{//第一次执行
				isStart = true;
				Sleep(uInterval);
			}
			else
			{
				Sleep(this->_uMillisecond);
			}
		}

		return Ret;
	}

private:
	R	HandleProcess( R* pRet, const Event_type& Event, int* pFinish )
	{
		*pRet = Event();
		*pFinish = 1;
		return R();
	}
};
/** @} end TimerEventSync */


/*!
* @class  TimerEventSync
* @brief   TimerEventSync
* @{
*/
template<>
class TimerEventSync<void> : public ITimerEventSync<void>
{
public:
	/*!
	* @function   TimerEventSync
	* @brief    
	* @return  
	*/
	TimerEventSync( void )
	{
		this->_uMillisecond = 10;
	}

	/*!
	* @function   ~TimerEventSync
	* @brief    
	* @return  
	*/
	~TimerEventSync( void )
	{
		Release();
	}

	void	push_back( const Event_type& Event, UInt64 uInterval/*毫秒*/ )
	{
		int iFinish = -1;
		tagEvent_INFO	tagInfo;
		tagInfo._Event = function20_bind(&TimerEventSync::HandleProcess, this, Event, &iFinish);
		tagInfo._Timer = Timestamp_type();
		tagInfo._uInterval = uInterval * 1000;
		this->_Lock.Lock();
		this->_EventList.push_back(tagInfo);
		this->_Lock.UnLock();

		bool isStart = false;
		while( iFinish != 1 )
		{
			if( !isStart )
			{//第一次执行
				isStart = true;
				Sleep(uInterval);
			}
			else
			{
				Sleep(this->_uMillisecond);
			}
		}
	}

private:
	void	HandleProcess( const Event_type& Event, int* pFinish )
	{
		Event();
		*pFinish = 1;
	}
};
/** @} end TimerEventSync */

#endif//__TIMEREVENTVSYNC_20160504200104_1462363264_H__
