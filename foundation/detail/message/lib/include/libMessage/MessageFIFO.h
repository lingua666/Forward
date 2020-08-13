
#ifndef __MESSAGEFIFO_20160504200104_1462363264_H__
#define __MESSAGEFIFO_20160504200104_1462363264_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libContainer/LinkList.h>
#include <libFunction_v20/CMyFunction.hpp>

#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <vector>

/*!
* @class  MessageFIFO
* @brief   MessageFIFO
* @{
*/

class MessageFIFO
{
public:
	typedef	ThreadWrap	Thread_type;
	typedef SmartPTR<Thread_type>	Thread_ptr;
	typedef std::vector<Thread_ptr>	ThreadList;
	typedef function20_handle<void (void)>	Message_type;
	typedef _container_::LinkList<Message_type,CNoneLock>	MessageList_type;
public:
	/*!
	* @function   MessageFIFO
	* @brief    
	* @return  
	*/
	MessageFIFO( void );

	/*!
	* @function   ~MessageFIFO
	* @brief    
	* @return  
	*/
	~MessageFIFO( void );

	void	Init( UInt8 uThread );

	void	Release( void );

	void	push_back( const Message_type& Message );
	
protected:
	void	WorkThread( void );

private:
	CLock		_Lock;
	MessageList_type	_MesList;
	ThreadList	_Threads;
};
/** @} end MessageFIFO */

#endif//__MESSAGEFIFO_20160504200104_1462363264_H__
