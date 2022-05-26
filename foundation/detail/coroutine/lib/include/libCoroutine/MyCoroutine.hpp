
#ifndef __MY_COROUTINE_20150806193305_1438860785_H__
#define __MY_COROUTINE_20150806193305_1438860785_H__

#include <libCoroutine/External.h>

namespace _coroutine_ {

#define Coroutine_Begin(value)

#define Coroutine_End()

#define Coroutine_Open(obj, handle)	obj.Co_Open(handle)

#define Coroutine_Resume(obj)	\
		if(obj.IsVaild())\
			obj.Co_Resume();\

#define Coroutine_Begin(obj)		switch(obj.Get()) { case 0: {

#define Coroutine_Yield(obj)		obj.Co_Yield(__LINE__); printf("case:%d:", __LINE__); } break; case __LINE__:{

#define Coroutine_Close(obj)		obj.Co_Close()

#define Coroutine_IsVaild(obj)		obj.IsVaild()

#define Coroutine_End(obj)			} break; default: break; }

	class MyCoroutine
	{
	public:
		typedef function20_handle<int(void)>	HFNEvent;

	public:
		MyCoroutine(void)
			: _iSetp(0)
		{}

		~MyCoroutine(void)
		{
			Co_Release();
		}

		int Init(void)
		{
			return 1;
		}

		void Co_Release(void)
		{
			_hEvent.reset();
			_iSetp = 0;
		}

		int Co_Open(const HFNEvent& hEvent)
		{
			_hEvent = hEvent;
			_iSetp = 0;
			return 1;
		}

		void Co_Close(void)
		{
			_hEvent.reset();
			_iSetp = -1;
		}

		void Co_Resume(void)
		{
			if (_hEvent)
			{
				_hEvent();
			}
		}

		void Co_Yield(Int32 iSetp)
		{
			_iSetp = iSetp;
		}

		void Co_Running(void)
		{}

		void Co_Status(void)
		{}

		bool IsVaild(void) const
		{
			return _hEvent ? true : false;
		}

		Int32 Get(void)
		{
			return _iSetp;
		}

	private:
		HFNEvent	_hEvent;
		Int32		_iSetp;
	};

}

#endif