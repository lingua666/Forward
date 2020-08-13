
#include <libNetFrame_Core_Common/Event_Handle.h>

namespace	_event_{

	Event_Handle::Event_Handle( void )
	{
		memset(_Type, 0, sizeof(_Type));
		memset(_handle, 0, sizeof(_handle));
		_isInit = false;
	}

	Event_Handle::Event_Handle( const Event_Handle& ev )
	{
		memcpy(_Type, ev._Type, sizeof(_Type));
		memcpy(_handle, ev._handle, sizeof(_handle));
		_isInit = ev._isInit;
	}

	Event_Handle::~Event_Handle( void )
	{

	}

	Event_Handle& Event_Handle::operator=( const Event_Handle& ev )
	{
		memcpy(_Type, ev._Type, sizeof(_Type));
		memcpy(_handle, ev._handle, sizeof(_handle));
		_isInit = ev._isInit;
		return *this;
	}

	bool Event_Handle::Detach( void )
	{
		memset(_Type, 0, sizeof(_Type));
		memset(_handle, 0, sizeof(_handle));
		_isInit = false;
		return true;
	}

	void Event_Handle::Attach( void (*pfnFun)( void ) )
	{
		ATTACH1_BODY_MACRO(0, pfnFun)
	}

	void Event_Handle::Invoke( void )
	{
		if(_isInit)
		{
			function10_handle< void (void) >* ptr = any_cast< function10_handle< void (void) > >();
			(*ptr)();
		}
	}
} //namespace	_event_