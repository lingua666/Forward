
#ifndef __TIMEREVENTKM_20160504200104_1462363264_H__
#define __TIMEREVENTKM_20160504200104_1462363264_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libContainer/CycleList.hpp>
#include <libContainer/CMyUnordered_map.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libTimestamp/Timestamp.h>
//#include <libString_v30/string.hpp>
#include <libUUID/UUIDGenerator.h>
#include <vector>

/*!
* @class  TimerEvent
* @brief   TimerEvent
* @{
*/

class TimerEventKM
{
public:
	typedef _string_type	_string_;
	typedef TimerEventKM	self_type;
	typedef	ThreadWrap	Thread_type;
	typedef SmartPTR<Thread_type>	Thread_ptr;
	typedef std::vector<Thread_ptr>	ThreadList;
	typedef function20_handle<void (void)>	Event_type;

	typedef struct tagEvent_INFO
	{
		Event_type		_Event;
		Timestamp_type	_Timer;
		Int64			_uInterval;	//微秒
		_string_		_Uuid;
		bool			_isRun;
	}tagEvent_INFO;

	typedef SmartPTR<tagEvent_INFO>	tagEvent_sptr;

	typedef _container_::CycleList<tagEvent_sptr,CNoneLock>	EventList_type;
	typedef Container_HashMap_type<_string_, tagEvent_sptr>	EventMap;

private:
	static tagEvent_sptr Alloc(void);

public:
	/*!
	* @function   TimerEvent
	* @brief    
	* @return  
	*/
	TimerEventKM( void );

	//注意: 该构造函数不可用于全局变量，否则会导致由于主线程还未启动，导致出现错误
	TimerEventKM( UInt8 uThread );

	/*!
	* @function   ~TimerEvent
	* @brief    
	* @return  
	*/
	~TimerEventKM( void );

	void	Init( UInt8 uThread = 1 );

	void	CloseAll( void );

	TimerEventKM::_string_	push_back( const Event_type& Event, UInt64 uInterval/*毫秒*/ );

	int	push_back_sync( const Event_type& Event, UInt64 uInterval/*毫秒*/ );

	void Remove(const _string_& sUuid);

	void	Release( void );

	void	Clear( void );

	int		size( void );

	void	Lock(void);

	void	UnLock(void);

	void	UpdateTime(const Timestamp_type& LocalTime, const Timestamp_type& NewTime);

	//单位毫秒
	void	SetSleepStep(UInt32 uMillisecond);
	
protected:

	void	WorkThread( void );

	void	HandleSync( const Event_type& Event, Int8* iRet );

private:
	CLock		_Lock;
	EventList_type	_EventList;
	EventMap		_EventMap;
	ThreadList	_Threads;
	bool		_isRun;
	UInt32 		_uMillisecond;
};
/** @} end TimerEvent */

TimerEventKM* GetTimerEventKMInstance(void);

#endif//__TIMEREVENT_20160504200104_1462363264_H__
