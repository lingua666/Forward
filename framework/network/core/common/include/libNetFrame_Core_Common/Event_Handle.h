
#ifndef __EVENT_HANDLE_20150721155112_1437465072_H__
#define __EVENT_HANDLE_20150721155112_1437465072_H__

#include <libFoundation/modafx.h>
#include <libNetFrame_Core_Common/Typeof.h>

namespace	_event_{

	typedef void* INVOKE_HANDLE;
	typedef void* NET_HANDLE;

	enum ENVENT_MASK
	{
		READ_MASK = (1 << 0),
		WRITE_MASK = (1 << 1),
		EXCEPT_MASK = (1 << 2),
		ACCEPT_MASK = (1 << 3),
		CONNECT_MASK = (1 << 4),
		TIMER_MASK = (1 << 5),
		CLOSE_MASK = (1 << 6)
		//RWE_MASK = READ_MASK | WRITE_MASK | EXCEPT_MASK | CLOSE_MASK
	};

	class Event_Handle;

#define FUNC_HANDLE			function10_handle

#define EVENT_PERMISSION_LIST_BEGIN()

#define EVENT_PERMISSION_LIST_END()

#define EVENT_NAME_SUFFIX(name) name##__

#define	EVENT_SET_HANDLE(mask)	PP_CAT(_event_::Event_Handle _ev,EVENT_NAME_SUFFIX(mask))
#define	EVENT_GET_HANDLE(mask)	PP_CAT(_ev, EVENT_NAME_SUFFIX(mask))

#define EVENT_DECL(mask, ...)\
		public:\
			typedef FUNC_HANDLE< void (__VA_ARGS__) > ev_type_##mask;\
		protected:\
			EVENT_SET_HANDLE(mask);

#define EVENT_DECL_LIST_BEGIN()

#define EVENT_DECL_LIST_END()

//test
#define EVENT_TYPE_MASK(mask)		ev_type_##mask
#define EVENT_MASK_I(func, mask)	func<mask>
#define EVENT_MASK(func, mask)		EVENT_MASK_I( func, EVENT_TYPE_MASK(mask) )

#define EVENT_PERMISSION_DECL(mask, ...)\
	EVENT_DECL(mask, __VA_ARGS__)\
public:\
	template<typename _EVT>\
	inline void EVENT_##mask( void (_EVT::*pfnFun)(__VA_ARGS__), const _EVT* pthis )\
	{\
		EVENT_OBJ_MEMBER(EVENT_GET_HANDLE(mask), EVENT_MASK_FUNCTION1(Attach, pfnFun, pthis));\
	}

//test


/*#define EVENT_PERMISSION_DECL(mask, ...)\
	EVENT_DECL(mask, __VA_ARGS__)\
	public:\
		template<typename T>\
		inline void EVENT_##mask##( void (T::*pfnFun)(__VA_ARGS__), const T* pthis )\
		{\
			EVENT_GET_HANDLE(mask) ## .Attach(pfnFun, pthis);\
		}*/


#define PERMISSION_READ(...)			EVENT_PERMISSION_DECL(Read, __VA_ARGS__)
#define PERMISSION_WRITE(...)			EVENT_PERMISSION_DECL(Write, __VA_ARGS__)
#define PERMISSION_PREPARE_WRITE(...)	EVENT_PERMISSION_DECL(PrepareWrite, __VA_ARGS__)
#define PERMISSION_ACCEPT(...)			EVENT_PERMISSION_DECL(Accept, __VA_ARGS__)
#define PERMISSION_CONNECT(...)			EVENT_PERMISSION_DECL(Connect, __VA_ARGS__)
#define PERMISSION_TIMER(...)			EVENT_PERMISSION_DECL(Timer, __VA_ARGS__)
#define PERMISSION_CLOSE(...)			EVENT_PERMISSION_DECL(Close, __VA_ARGS__)
#define PERMISSION_EXCEPT(...)			EVENT_PERMISSION_DECL(Except, __VA_ARGS__)

//test
#define EVENT_PTR_MEMBER(ptr, member)				ptr->member
#define EVENT_OBJ_MEMBER(obj, member)				obj.member
#define EVENT_TEMPLATE_PTR_MEMBER(ptr, member)		ptr-> template member
#define EVENT_TEMPLATE_OBJ_MEMBER(obj, member)		obj. template member
#define EVENT_MASK_FUNCTION1(mask_func, func, name)	mask_func(func, name)
#define EVENT_MASK_FUNCTION(mask, func, name)		EVENT_MASK_FUNCTION1( PP_CAT(EVENT_, mask),  func, name)
#define EVENT_IMPL_DECL(mask, func)					EVENT_PTR_MEMBER(ptr, EVENT_MASK_FUNCTION(mask, func, this));
//test

//#define EVENT_IMPL_DECL(mask, func) ptr->EVENT_##mask##( func,this );

#define EVENT_REGISTER_READ(func)			EVENT_IMPL_DECL(Read, func)
#define EVENT_REGISTER_WRITE(func)			EVENT_IMPL_DECL(Write, func)
#define EVENT_REGISTER_PREPARE_WRITE(func)	EVENT_IMPL_DECL(PrepareWrite, func)
#define EVENT_REGISTER_EXCEPT(func)			EVENT_IMPL_DECL(Except, func)
#define EVENT_REGISTER_ACCEPT(func)			EVENT_IMPL_DECL(Accept, func)
#define EVENT_REGISTER_CONNECT(func)		EVENT_IMPL_DECL(Connect, func)
#define EVENT_REGISTER_TIMER(func)			EVENT_IMPL_DECL(Timer, func)
#define EVENT_REGISTER_CLOSE(func)			EVENT_IMPL_DECL(Close, func)

#define EVENT_TEMPLATE_EXEC(mask)			EVENT_TEMPLATE_OBJ_MEMBER(EVENT_PTR_MEMBER(this, EVENT_GET_HANDLE(mask)), EVENT_MASK(Invoke, mask))
#define EVENT_EXEC(mask)					EVENT_OBJ_MEMBER(EVENT_PTR_MEMBER(this, EVENT_GET_HANDLE(mask)), EVENT_MASK(Invoke, mask))
//#define EVENT_EXEC(mask)		EVENT_EXEC1(PP_CAT(this->,EVENT_GET_HANDLE(mask)), EVENT_MASK(Invoke, mask))
//#define EVENT_EXEC(mask)		PP_CAT(this->,EVENT_GET_HANDLE(mask))##.Invoke<ev_type_##mask>

#define EVENT_TEMPLATE_EXEC_READ			EVENT_TEMPLATE_EXEC(Read)
#define EVENT_TEMPLATE_EXEC_WRITE			EVENT_TEMPLATE_EXEC(Write)
#define EVENT_TEMPLATE_EXEC_PREPARE_WRITE	EVENT_TEMPLATE_EXEC(PrepareWrite)
#define EVENT_TEMPLATE_EXEC_EXCEPT			EVENT_TEMPLATE_EXEC(Except)
#define EVENT_TEMPLATE_EXEC_ACCEPT			EVENT_TEMPLATE_EXEC(Accept)
#define EVENT_TEMPLATE_EXEC_CONNECT			EVENT_TEMPLATE_EXEC(Connect)
#define EVENT_TEMPLATE_EXEC_TIMER			EVENT_TEMPLATE_EXEC(Timer)
#define EVENT_TEMPLATE_EXEC_CLOSE			EVENT_TEMPLATE_EXEC(Close)

#define EVENT_EXEC_READ				EVENT_EXEC(Read)
#define EVENT_EXEC_WRITE			EVENT_EXEC(Write)
#define EVENT_EXEC_PREPARE_WRITE	EVENT_EXEC(PrepareWrite)
#define EVENT_EXEC_EXCEPT			EVENT_EXEC(Except)
#define EVENT_EXEC_ACCEPT			EVENT_EXEC(Accept)
#define EVENT_EXEC_CONNECT			EVENT_EXEC(Connect)
#define EVENT_EXEC_TIMER			EVENT_EXEC(Timer)
#define EVENT_EXEC_CLOSE			EVENT_EXEC(Close)

#define EVENT_DETACH(mask)		EVENT_OBJ_MEMBER(EVENT_PTR_MEMBER(this, EVENT_GET_HANDLE(mask), Detach)();//PP_CAT(this->,EVENT_GET_HANDLE(mask))##.Detach();


#define EVENT_REGISTER_LIST_BEGIN( target )\
	public:\
		inline void	RegisterAllEvent( target* ptr )\
		{

#define EVENT_REGISTER_LIST_END()\
		}

#define	EVENT_REGISTER(ptr_target, ptr_src)	(ptr_src)->RegisterAllEvent(ptr_target);


#define EVENT_CUSTOM(mask, ...)	EVENT_PERMISSION_DECL(mask, __VA_ARGS__)
#define	EVENT_EXEC_CUSTOM(mask)	EVENT_EXEC(mask)

#define	EVENT_CUSTOM_LIST_BEGIN()
#define	EVENT_CUSTOM_LIST_END()

#define EVENT_BIND_REGISTER_DEL(mask, target)\
		EVENT_PERMISSION_DECL(mask, target*)\
		public:\
			template<typename T>\
			void	Ev_##mask##Bind_Register( const T* pthis )\
			{\
				EVENT_GET_HANDLE(mask) ## .Attach(&T::RegisterAllEvent, pthis);\
			}

#define EVENT_BIND_REGISTER(mask, ptr_src)	Ev_##mask##Bind_Register(ptr_src)
#define EVENT_EXEC_BIND(mask, ptr_target)	EVENT_EXEC(mask)(ptr_target);


#define EVENT_ATTACH_LIST_BEGIN( target )\
		public:\
			inline void Event_Attach_List( const target* ptr )\
			{

#define EVENT_ATTACH_LIST_END()\
			}

#define	EVENT_EXEC_ATTACH(ptr_target, ptr_src)	(ptr_src)->Event_Attach_List(ptr_target)

#define EVENT_ATTACH_HANDLE_I(ev_handle, mask_func, handle, ptr) EVENT_OBJ_MEMBER(ev_handle, EVENT_MASK_FUNCTION1(mask_func, handle, ptr));
#define EVENT_ATTACH_HANDLE(mask, handle)	EVENT_ATTACH_HANDLE_I(EVENT_GET_HANDLE(mask), EVENT_MASK(Attach, mask), handle, ptr)
//#define EVENT_ATTACH_HANDLE(mask, handle)	EVENT_GET_HANDLE(mask) ## .Attach<ev_type_##mask>(handle, ptr);

	/*!
	* @class  Event_Handle
	* @brief   Event_Handle
	* @{
	*/

	class Event_Handle
	{
	public:
#define ATTACH1_BODY_MACRO(n, p1)\
		new(reinterpret_cast<void*>(_Type)) Typeof< FUNC_HANDLE< void (MACRO_ARGLIST_DEF(n)) > >();\
		new(reinterpret_cast<void*>(_handle)) FUNC_HANDLE< void (MACRO_ARGLIST_DEF(n)) >(p1);\
		_isInit = true;

#define ATTACH2_BODY_MACRO(n, p1, p2)\
		new(reinterpret_cast<void*>(_Type)) Typeof< FUNC_HANDLE< void (MACRO_ARGLIST_DEF(n)) > >();\
		new(reinterpret_cast<void*>(_handle)) FUNC_HANDLE< void (MACRO_ARGLIST_DEF(n)) >(p1, p2);\
		_isInit = true;

#define ATTACH3_BODY_MACRO(p1, p2, p3)\
		new(reinterpret_cast<void*>(_Type)) Typeof< p1 >();\
		new(reinterpret_cast<void*>(_handle)) p1(p2, p3);\
		_isInit = true;

#define EVENT_ATTACH_DECLAR(n)\
		template<typename T PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		inline void Attach( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis )\
		{\
			ATTACH2_BODY_MACRO(n, pfnFun, pthis)\
		}\
		template<MACRO_TARGLIST_DEF(n)>\
		inline void Attach( void (*pfnFun)(MACRO_ARGLIST_DEF(n)) )\
		{\
			ATTACH1_BODY_MACRO(n, pfnFun)\
		}\
		template<typename C,typename T PP_COMMA_IF(n) MACRO_TARGLIST_DEF(n)>\
		inline void Attach( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis )\
		{\
			ATTACH3_BODY_MACRO(C, pfnFun, pthis)\
		}\
		template<MACRO_TARGLIST_DEF(n)>\
		inline void Invoke( MACRO_TPARAMLIST_DEF(n) )\
		{\
			if(_isInit)\
			{\
				FUNC_HANDLE< void (MACRO_ARGLIST_DEF(n)) >* ptr = any_cast< FUNC_HANDLE< void (MACRO_ARGLIST_DEF(n)) > >();\
				(*ptr)(MACRO_PARAMLIST_DEF(n));\
			}\
		}\
		template<typename T,MACRO_TARGLIST_DEF(n)>\
		inline  void Invoke( MACRO_TPARAMLIST_DEF(n) )\
		{\
			if(_isInit)\
			{\
				T* ptr = (reinterpret_cast<T*>(_handle));\
				(*ptr)(MACRO_PARAMLIST_DEF(n));\
			}\
		 }

		typedef Event_Handle self_type;
	public:
		/*!
		* @function   Event_Handle
		* @brief    
		* @return  
		*/
		Event_Handle( void );

		Event_Handle( const Event_Handle& ev );

		/*!
		* @function   ~Event_Handle
		* @brief    
		* @return  
		*/
		~Event_Handle( void );

		Event_Handle& operator=( const Event_Handle& ev );

		bool Detach( void );

		template<typename T>
		inline void Attach( void (T::*pfnFun)( void ), const T* pthis )
		{
			ATTACH2_BODY_MACRO(0, pfnFun, pthis)
		}

		template<typename C,typename T>
		inline void Attach( void (T::*pfnFun)( void ), const T* pthis )
		{
			ATTACH3_BODY_MACRO(C, pfnFun, pthis)
		}

		void Attach( void (*pfnFun)( void ) );

		void Invoke( void );

		template<typename T>
		void Invoke( void )
		{
			if(_isInit)
			{
				T* ptr = (reinterpret_cast<T*>(_handle));
				(*ptr)();
			}
		}

		EVENT_ATTACH_DECLAR(1)
		EVENT_ATTACH_DECLAR(2)
		EVENT_ATTACH_DECLAR(3)
		EVENT_ATTACH_DECLAR(4)
		EVENT_ATTACH_DECLAR(5)
		EVENT_ATTACH_DECLAR(6)
		EVENT_ATTACH_DECLAR(7)

	protected:
		template<typename ValueType>
		ValueType* any_cast( void )
		{
			try
			{
				if( &((reinterpret_cast<ITypeof*>(_Type))->type()) == &(typeid(ValueType)))
					return (reinterpret_cast<ValueType*>(_handle));
				else
					throw("error");
			}
			catch(...)
			{//error
				printf("error\r\n");
				//error_code = -1;
			}

			return NULL;
		}

	private:
		__M_ALIGNED__(4) char _Type[10];
		__M_ALIGNED__(4) char _handle[50];
		bool	_isInit;

	};
	/** @} end Event_Handle */


} //namespace	_event_

#endif//__EVENT_HANDLE_20150721155112_1437465072_H__
