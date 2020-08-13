
#ifndef __SIGNALSCONNECT_20160602115813_1464839893_H__
#define __SIGNALSCONNECT_20160602115813_1464839893_H__

#include <libSignalsSlots/modafx.h>

namespace _signals_slots_{

	class connection_body_base
	{
	public:
		connection_body_base( void );

		virtual ~connection_body_base( void );

		void disconnect( void );

		bool is_connect( void ) const;

	public:
		mutable bool _connected;
	};

	template <typename SlotType>
	class connection_body : public connection_body_base
	{
	public:
		typedef connection_body	self_type;
		typedef	SlotType		template_type;

	public:
		connection_body( const SlotType &slot_in )
			: _slot(slot_in)
		{}

		const SlotType& slot_function( void ) const 
		{
			return _slot;
		}

	private:
		template_type	_slot;
	};

	/*!
	* @class  SignalsConnect
	* @brief   SignalsConnect
	* @{
	*/

	class connection
	{
	public:
		typedef connection	self_type;
		typedef SmartPTR<connection_body_base> smart_ptr_type;
		typedef WeakPTR<connection_body_base> weak_ptr_type;

	public:
		connection( void );
			
		connection( const self_type& t1 );

		connection( const weak_ptr_type& connectionBody );

		connection& operator=( const self_type& t1 );

		void disconnect( void ) const;

	private:
		weak_ptr_type	_weak_connection_body;
	};
	/** @} end SignalsConnect */

} //_signals_slots_

#endif//__SIGNALSCONNECT_20160602115813_1464839893_H__
