
#ifndef __SUBSCRIBE_EVENT_20200727094622_3442199361_H__
#define __SUBSCRIBE_EVENT_20200727094622_3442199361_H__

#include <libLock/Lock.hpp>
#include <libThread/ThreadWrap.h>
#include <libContainer/CycleList.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libTimestamp/Timestamp.h>
#include <vector>

template<typename R, typename A1 = void, typename A2 = void, typename A3 = void, typename A4 = void\
	, typename A5 = void, typename A6 = void, typename A7 = void, typename A8 = void, typename A9 = void>
	class SubscribeEvents;

#define	SUBSCRIBE_EVENT_DECLAR(n)\
	template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
	class SubscribeEvents<R (MACRO_ARGLIST_DEF(n))>\
	{\
	public:\
		typedef function20_handle<R (UInt32 PP_COMMA_IF(n) MACRO_ARGLIST_DEF(n))>	HFNEvent;\
		typedef std::vector<HFNEvent>						Events_Vec;\
	public:\
		SubscribeEvents(void)\
			: _uStart(0)\
		{}\
		~SubscribeEvents(void)\
		{ Release(); }\
		int Init(UInt32 uStart, UInt32 uEnd)\
		{\
			if (uStart > uEnd)\
				return -1;\
			_Vec.clear();\
			_Vec.resize((uEnd - uStart) + 1);\
			_uStart = uStart;\
		}\
		void Release(void)\
		{\
			_Vec.clear();\
		}\
		int AddConsumer(UInt32 uType, const HFNEvent& hEvent)\
		{\
			if (_uStart > uType || _uStart + _Vec.size() <= uType)\
				return -1;\
			_Vec[uType - _uStart] = hEvent;\
			return 1;\
		}\
		int AddAllConsumer(const HFNEvent& hEvent)\
		{\
			for(UInt32 i = 0; i < _Vec.size(); i ++)\
			{\
				_Vec[i] = hEvent;\
			}\
			return 1;\
		}\
		void Publish(UInt32 uType PP_COMMA_IF(n) MACRO_CREF_PARAMLIST_DEF(n))\
		{\
			if (_uStart <= uType && _uStart + _Vec.size() > uType)\
			{\
				try\
				{\
					HFNEvent hEv = _Vec.at(uType - _uStart);\
					if(hEv)\
						hEv(uType PP_COMMA_IF(n) MACRO_PARAMLIST_DEF(n));\
				}\
				catch (const std::exception& e)\
				{\
					printf("SubscribeEvents::Publish(%d,%d) Error\r\n", _uStart, uType);\
				}\
			}\
		}\
		private:\
			UInt32 _uStart;\
			Events_Vec	_Vec;\
		};


SUBSCRIBE_EVENT_DECLAR(0)
SUBSCRIBE_EVENT_DECLAR(1)
SUBSCRIBE_EVENT_DECLAR(2)
SUBSCRIBE_EVENT_DECLAR(3)
SUBSCRIBE_EVENT_DECLAR(4)
SUBSCRIBE_EVENT_DECLAR(5)
SUBSCRIBE_EVENT_DECLAR(6)
SUBSCRIBE_EVENT_DECLAR(7)
SUBSCRIBE_EVENT_DECLAR(8)
SUBSCRIBE_EVENT_DECLAR(9)

#endif//__SUBSCRIBE_EVENT_20200727094622_3442199361_H__
