
#ifndef __SIGNALS_20160602113712_1464838632_H__
#define __SIGNALS_20160602113712_1464838632_H__

#include <libSignalsSlots/SignalsConnect.h>
#include <list>

namespace _signals_slots_{

	enum connect_position 
	{ 
		at_back, 
		at_front 
	};

	template <typename R, DEFAULT_TEMPLATE_DEF(9)>
	class signals_impl;

	#define	SIGNALS_IMPL_HANDLE_DECLAR(n)\
		template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		class signals_impl<R (MACRO_ARGLIST_DEF(n))>\
		{\
		public:\
			typedef	function20_handle<R (MACRO_ARGLIST_DEF(n))>		slot_type;\
			typedef	connection_body<slot_type>						connection_body_type;\
			typedef	SmartPTR<connection_body_type>					connection_body_sptr;\
			typedef std::list<connection_body_sptr>					connection_body_list;\
			static	MemPool_type* GetMemPoolInstance( void )\
			{\
				static MemPool_type	s_Pool( sizeof(connection_body_type) );\
				return &s_Pool;\
			}\
			static	connection_body_type* MemAlloc( const slot_type& t1 )\
			{\
				return GetMemPoolInstance()->template AllocObj<connection_body_type>(t1);\
			}\
			static	void MemFree( void* p )\
			{\
				GetMemPoolInstance()->FreeObj( reinterpret_cast<connection_body_type*>(p) );\
			}\
		public:\
			signals_impl( void )\
			{}\
			connection connect( const slot_type& t1, connect_position position = at_back )\
			{\
				connection_body_sptr body_sptr = connection_body_sptr( MemAlloc(t1), function20_bind_Ex(MemFree) );\
				_lock.Lock();\
				if( position == at_back )\
					_list.push_back( body_sptr );\
				else\
					_list.push_front( body_sptr );\
				_lock.UnLock();\
				return connection(body_sptr);\
			}\
			connection connect_one( const slot_type& t1, connect_position position = at_back )\
			{\
				typename connection_body_list::iterator it;\
				connection_body_sptr body_sptr = connection_body_sptr( MemAlloc(t1), function20_bind_Ex(MemFree) );\
				bool exist = false;\
				_lock.Lock();\
				for(it = _list.begin(); it != _list.end(); ++it)\
				{\
					if( (*it)->slot_function() == t1 )\
						exist = true;\
				}\
				if( !exist )\
				{\
					if( position == at_back )\
						_list.push_back( body_sptr );\
					else\
						_list.push_front( body_sptr );\
				}\
				_lock.UnLock();\
				return connection(body_sptr);\
			}\
			void disconnect( const slot_type& t1 )\
			{\
				typename connection_body_list::iterator it;\
				_lock.Lock();\
				for(it = _list.begin(); it != _list.end(); ++it)\
				{\
					if( (*it)->slot_function() == t1 )\
						(*it)->disconnect();\
				}\
				_lock.UnLock();\
			}\
			void disconnect_all( void )\
			{\
				typename connection_body_list::iterator it;\
				_lock.Lock();\
				for(it = _list.begin(); it != _list.end(); ++it)\
				{\
					(*it)->disconnect();\
				}\
				_list.clear();\
				_lock.UnLock();\
			}\
			R operator()(MACRO_TPARAMLIST_DEF(n))\
			{\
				typename connection_body_list::iterator it;\
				_lock.Lock();\
				for(it = _list.begin(); it != _list.end(); )\
				{\
					if( !(*it)->is_connect() )\
						it = _list.erase(it);\
					else\
					{\
						(*it)->slot_function()( MACRO_PARAMLIST_DEF(n) );\
						it ++;\
					}\
				}\
				_lock.UnLock();\
			}\
		private:\
			connection_body_list	_list;\
			CLock					_lock;\
		};

	SIGNALS_IMPL_HANDLE_DECLAR(0)
	SIGNALS_IMPL_HANDLE_DECLAR(1)
	SIGNALS_IMPL_HANDLE_DECLAR(2)
	SIGNALS_IMPL_HANDLE_DECLAR(3)
	SIGNALS_IMPL_HANDLE_DECLAR(4)
	SIGNALS_IMPL_HANDLE_DECLAR(5)
	SIGNALS_IMPL_HANDLE_DECLAR(6)
	SIGNALS_IMPL_HANDLE_DECLAR(7)
	SIGNALS_IMPL_HANDLE_DECLAR(8)
	SIGNALS_IMPL_HANDLE_DECLAR(9)

	template <typename R, DEFAULT_TEMPLATE_DEF(9)>
	class signal;

	#define	SIGNALS_HANDLE_DECLAR(n)\
		template<typename R PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		class signal<R (MACRO_ARGLIST_DEF(n))>\
		{\
		public:\
			typedef	function20_handle<R (MACRO_ARGLIST_DEF(n))>		slot_type;\
			typedef	signals_impl<R (MACRO_ARGLIST_DEF(n))>			impl_type;\
			typedef	SmartPTR<impl_type>								impl_type_sptr;\
			typedef	signal<R (MACRO_ARGLIST_DEF(n))>				self_type;\
			static	MemPool_type* GetMemPoolInstance( void )\
			{\
				static MemPool_type	s_Pool( sizeof(impl_type) );\
				return &s_Pool;\
			}\
			static	impl_type* MemAlloc( void )\
			{\
				return GetMemPoolInstance()->template AllocObj<impl_type>();\
			}\
			static	void MemFree( void* p )\
			{\
				GetMemPoolInstance()->FreeObj( reinterpret_cast<impl_type*>(p) );\
			}\
		public:\
			signal( void )\
				: _impl_sptr( MemAlloc(), function20_bind_Ex(MemFree) )\
			{}\
			signal( const self_type& t1 ) \
			{\
				_impl_sptr = t1._impl_sptr;\
			}\
			inline self_type& operator=( const self_type& t1 )\
			{\
				_impl_sptr = t1._impl_sptr;\
				return *this;\
			}\
			connection connect( const slot_type& t1, connect_position position = at_back )\
			{\
				return _impl_sptr->connect(t1, position);\
			}\
			connection connect_one( const slot_type& t1, connect_position position = at_back )\
			{\
				return _impl_sptr->connect_one(t1, position);\
			}\
			void disconnect( const slot_type& t1 )\
			{\
				_impl_sptr->disconnect(t1);\
			}\
			void disconnect_all( void )\
			{\
				_impl_sptr->disconnect_all();\
			}\
			inline R operator()( MACRO_TPARAMLIST_DEF(n) )\
			{\
				return (*_impl_sptr)( MACRO_PARAMLIST_DEF(n) );\
			}\
			inline R operator()( MACRO_TPARAMLIST_DEF(n) ) const\
			{\
				return (*_impl_sptr)( MACRO_PARAMLIST_DEF(n) );\
			}\
			operator bool () \
			{\
				return !_impl_sptr ? false : true;\
			}\
			bool operator!()\
			{ \
				return !_impl_sptr;\
			}\
			operator bool () const\
			{\
				return !_impl_sptr ? false : true;\
			}\
			bool operator!() const\
			{ \
				return !_impl_sptr;\
			}\
		private:\
			impl_type_sptr	_impl_sptr;\
		};

	SIGNALS_HANDLE_DECLAR(0)
	SIGNALS_HANDLE_DECLAR(1)
	SIGNALS_HANDLE_DECLAR(2)
	SIGNALS_HANDLE_DECLAR(3)
	SIGNALS_HANDLE_DECLAR(4)
	SIGNALS_HANDLE_DECLAR(5)
	SIGNALS_HANDLE_DECLAR(6)
	SIGNALS_HANDLE_DECLAR(7)
	SIGNALS_HANDLE_DECLAR(8)
	SIGNALS_HANDLE_DECLAR(9)

	/** @} end Signals */

} //_signals_slots_

#endif//__SIGNALS_20160602113712_1464838632_H__
