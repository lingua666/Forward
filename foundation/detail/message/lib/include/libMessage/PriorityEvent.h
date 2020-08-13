
#ifndef __PRIORITYEVENT_20160504200104_1462363264_H__
#define __PRIORITYEVENT_20160504200104_1462363264_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libMessage/PriorityQueue.h>
#include <libFunction_v20/CMyFunction.hpp>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <vector>

/*!
* @class  PriorityEvent
* @brief   PriorityEvent
* @{
*/

class PriorityEvent
{
public:
	typedef	ThreadWrap	Thread_type;
	typedef SmartPTR<Thread_type>	Thread_ptr;
	typedef std::vector<Thread_ptr>	ThreadList;
	typedef function20_handle<void (void)>	Message_type;
	typedef PriorityQueue<Message_type>	MessageList_type;
public:
	/*!
	* @function   PriorityEvent
	* @brief    
	* @return  
	*/
	PriorityEvent( void );

	/*!
	* @function   ~PriorityEvent
	* @brief    
	* @return  
	*/
	~PriorityEvent( void );

	void	Init( UInt8 uThread );

	void	Release( void );

	void	push_back( const Message_type& Message, int iPriority = 0 );
	
protected:
	void	WorkThread( void );

private:
	CLock		_Lock;
	MessageList_type	_MesList;
	ThreadList	_Threads;
};
/** @} end PriorityEvent */

#endif//__PRIORITYEVENT_20160504200104_1462363264_H__
