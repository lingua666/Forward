
#ifndef __TIMEREVENT_20160504200104_1462363264_H__
#define __TIMEREVENT_20160504200104_1462363264_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libContainer/CycleList.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libTimestamp/Timestamp.h>
#include <vector>

/*!
* @class  TimerEvent
* @brief   TimerEvent
* @{
*/

class TimerEvent
{
public:
	typedef	ThreadWrap	Thread_type;
	typedef SmartPTR<Thread_type>	Thread_ptr;
	typedef std::vector<Thread_ptr>	ThreadList;
	typedef function20_handle<void (void)>	Event_type;

	struct tagEvent_INFO
	{
		Event_type		_Event;
		Timestamp_type	_Timer;
		UInt32			_uInterval;	//΢��
		bool			_isExec;	//ִ��
	};

	typedef _container_::CycleList<tagEvent_INFO,CNoneLock>	EventList_type;
public:
	/*!
	* @function   TimerEvent
	* @brief    
	* @return  
	*/
	TimerEvent( void );

	//ע��: �ù��캯����������ȫ�ֱ���������ᵼ���������̻߳�δ���������³��ִ���
	TimerEvent( UInt8 uThread ); 

	/*!
	* @function   ~TimerEvent
	* @brief    
	* @return  
	*/
	~TimerEvent( void );

	void	Init( UInt8 uThread = 1 );

	void	CloseAll( void );

	void	push_back( const Event_type& Event, UInt32 uInterval/*����*/ );

	int		push_back_sync( const Event_type& Event, UInt32 uInterval/*����*/ );

	void	Release( void );

	void	Clear( void );

	int		size( void );
	
protected:

	void	WorkThread( void );

	void	HandleSync( const Event_type& Event, Int8* iRet );

private:
	CLock		_Lock;
	EventList_type	_EventList;
	ThreadList	_Threads;
	bool		_isRun;
};
/** @} end TimerEvent */

TimerEvent* GetTimerEventInstance(void);

#endif//__TIMEREVENT_20160504200104_1462363264_H__
