
#ifndef __TIMEREVENT_V20_20160504200104_1462363264_H__
#define __TIMEREVENT_V20_20160504200104_1462363264_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libContainer/CycleList.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libTimestamp/Timestamp.h>
#include <vector>

/*!
* @class  TimerEvent_V20
* @brief   TimerEvent_V20
* @{
*/

class TimerEvent_V20
{
public:
	typedef	ThreadWrap	Thread_type;
	typedef SmartPTR<Thread_type>	Thread_ptr;
	typedef std::vector<Thread_ptr>	ThreadList;
	typedef function20_handle<void (void)>	Event_type;

	struct tagEvent_INFO
	{
		Event_type		_Begin;
		Event_type		_Event;
		Event_type		_End;
		Timestamp_type	_Timer;
		UInt32			_uInterval;	//微秒
	};

	typedef _container_::CycleList<tagEvent_INFO,CNoneLock>	EventList_type;
public:
	/*!
	* @function   TimerEvent_V20
	* @brief    
	* @return  
	*/
	TimerEvent_V20( void );

	//注意: 该构造函数不可用于全局变量，否则会导致由于主线程还未启动，导致出现错误
	TimerEvent_V20( UInt8 uThread ); 

	/*!
	* @function   ~TimerEvent_V20
	* @brief    
	* @return  
	*/
	~TimerEvent_V20( void );

	void	Init( UInt8 uThread );

	void	CloseAll( void );

	void	push_back( const Event_type& Event, UInt32 uInterval/*毫秒*/,
					const Event_type& Begin = Event_type(), const Event_type& _End = Event_type() );

	void	Release( void );

	void	Clear( void );
	
protected:

	void	WorkThread( void );

private:
	CLock		_Lock;
	EventList_type	_EventList;
	ThreadList	_Threads;
};
/** @} end TimerEvent_V20 */

TimerEvent_V20* GetTimerEvent_V20Instance(void);

#endif//__TIMEREVENT_20160504200104_1462363264_H__
