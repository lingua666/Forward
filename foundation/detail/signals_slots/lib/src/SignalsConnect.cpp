
#include <libSignalsSlots/SignalsConnect.h>

namespace _signals_slots_{

	connection_body_base::connection_body_base( void )
			: _connected(true)
	{
	}

	connection_body_base::~connection_body_base( void ) 
	{

	}

	void connection_body_base::disconnect( void )
	{
		_connected = false;
	}

	bool connection_body_base::is_connect( void ) const
	{
		return _connected;
	}



	connection::connection( void )
	{

	}

	connection::connection( const self_type& t1 )
		: _weak_connection_body(t1._weak_connection_body)
	{

	}

	connection::connection( const weak_ptr_type& connectionBody )
		: _weak_connection_body(connectionBody)
	{
	
	}

	connection& connection::operator=( const self_type& t1 )
	{
		_weak_connection_body = t1._weak_connection_body;
		return *this;
	}

	void connection::disconnect( void ) const
	{
		smart_ptr_type connectionBody(_weak_connection_body.lock());
		if(	connectionBody.get() != NULL )
			connectionBody->disconnect();
	}

} //_signals_slots_

