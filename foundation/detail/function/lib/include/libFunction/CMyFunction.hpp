
#ifndef __CMYFUNCTION_20151118115435_1447818875_H__
#define __CMYFUNCTION_20151118115435_1447818875_H__

////////////////////////////////////////////////////////////////
//
//使用原则：尽可能多用CMyFunction对象替代AnyFunc对象
//
///////////////////////////////////////////////////////////////

#include <libFunction/IFunction.h>
#include <libFunction/list_av.hpp>
#include <libFunction/FunctionAddress.hpp>
#include <libCommon/CMyException.h>

using namespace _functionaddress_;

namespace	_function_ {

	DECLARE_EXCEPTION(_myfunctionException,CMyException)

#define DECLARE_PARAMS(...) __VA_ARGS__ 
#define DECLARE_ARGS(...) __VA_ARGS__ 

	typedef	void* ImplObjHandle;
	template <typename T>
	struct  ImplObj
	{
		inline	static	ImplObjHandle	Construction( const ImplObjHandle& handle )
		{
			if(handle == NULL)
				return NULL;
			return (ImplObjHandle)(new T(*((T*)(handle))));
		}

		inline	static	ImplObjHandle	Construction_v1( const T& handle )
		{
			return (ImplObjHandle)(new T(handle));
		}

		inline	static	void	Destroy( ImplObjHandle& handle )
		{
			if(handle != NULL)
				delete reinterpret_cast<T*>(handle);
		}
	};

	/*template <typename R, DEFAULT_TEMPLATE_DEF(9)>
	class CMyFunction;*/

#define	FUNCTION_HANDLE_DECLAR(n,args)\
	template<args>\
	class CMyFunction<R (MACRO_ARGLIST_DEF(n))>\
	{\
	public:\
		typedef R (_functionaddress_::GenericClass::*GenericMemFn)(MACRO_ARGLIST_DEF(n));\
		typedef CMyFunction self_type;\
		typedef	ImplObjHandle	(*pConstructionFunc)( const ImplObjHandle& handle );\
		typedef	void	(*pDestroyFunc)( ImplObjHandle& handle );\
	public:\
		~CMyFunction( void ) \
		{ \
			if(_pDestroyFunc != NULL)\
			{\
				_pDestroyFunc(_pObjHandle);\
				_pDestroyFunc = NULL;\
			}\
			_pObjHandle = NULL;\
		}\
		CMyFunction( void) : _pObjHandle(NULL)\
				,_pConstructionFunc(NULL)\
				,_pDestroyFunc(NULL)\
				,m_pfnFun(NULL)\
				,m_pthis(NULL)\
				,m_pStaticfnFun(NULL)\
		{}\
		CMyFunction( const self_type& t1 ) \
		{\
			m_pStaticfnFun = t1.m_pStaticfnFun;\
			m_pfnFun = t1.m_pfnFun;\
			_pConstructionFunc = t1._pConstructionFunc;\
			_pDestroyFunc = t1._pDestroyFunc;\
			_pObjHandle = t1._pObjHandle != NULL ? _pConstructionFunc(t1._pObjHandle) : NULL;\
			m_pthis = m_pStaticfnFun == NULL ? t1.m_pthis : reinterpret_cast<_functionaddress_::GenericClass*>(this);\
		}\
		template<typename T>\
		CMyFunction( R (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis )\
		{\
			Bind( pfnFun, pthis );\
		}\
		template<typename T,typename C>\
		CMyFunction( R (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), C& obj ) : _pDestroyFunc(NULL) \
		{\
			Bind( pfnFun, obj );\
		}\
		/*explicit*/ CMyFunction( R (*pfnFun)(MACRO_ARGLIST_DEF(n)) )\
		{\
			Bind( pfnFun );\
		}\
		template <typename F, typename T>\
		CMyFunction( const list_av_2<F,T>& t1 ) : _pDestroyFunc(NULL) \
		{\
			Bind(MACRO_MEMBERLIST_DEF(2,t1));\
		}\
		template <typename F>\
		CMyFunction( const list_av_1<F>& t1 ) \
		{\
			Bind(MACRO_MEMBERLIST_DEF(1,t1));\
		}\
		template <typename F,typename T>\
		inline self_type& operator=( const list_av_2<F,T>& t1 ) \
		{\
			if(_pDestroyFunc != NULL)\
				_pDestroyFunc(_pObjHandle);\
			Bind(MACRO_MEMBERLIST_DEF(2,t1));\
			return *this;\
		}\
		template <typename T>\
		inline self_type& operator=( const list_av_1<T>& t1 ) \
		{\
			if(_pDestroyFunc != NULL)\
				_pDestroyFunc(_pObjHandle);\
			Bind(MACRO_MEMBERLIST_DEF(1,t1));\
			return *this;\
		}\
		inline self_type& operator=( const self_type& t1 )\
		{\
			if(_pDestroyFunc != NULL)\
				_pDestroyFunc(_pObjHandle);\
			m_pfnFun = t1.m_pfnFun;\
			m_pStaticfnFun = t1.m_pStaticfnFun;\
			_pConstructionFunc = t1._pConstructionFunc;\
			_pDestroyFunc = t1._pDestroyFunc;\
			_pObjHandle = t1._pObjHandle != NULL ? _pConstructionFunc(t1._pObjHandle) : NULL;\
			m_pthis = m_pStaticfnFun == NULL ? t1.m_pthis : reinterpret_cast<_functionaddress_::GenericClass*>(this);\
			return *this;\
		}\
		inline R	operator()(MACRO_TPARAMLIST_DEF(n))\
		{\
			return (m_pthis->*m_pfnFun)(MACRO_PARAMLIST_DEF(n));\
		}\
		operator bool () \
		{\
			return !this->empty();\
		}\
		bool operator!()\
		{ \
			return this->empty();\
		}\
		operator bool () const\
		{\
			return !this->empty();\
		}\
		bool operator!() const\
		{ \
			return this->empty();\
		}\
		template <typename T>\
		inline void Bind( R (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis )\
		{\
			_pConstructionFunc = NULL;\
			_pDestroyFunc = NULL;\
			_pObjHandle = NULL;\
			m_pStaticfnFun = NULL;\
			m_pthis = _functionaddress_::SimplifyMemFunc<sizeof(pfnFun)>\
			::Convert((T*)pthis, pfnFun, m_pfnFun);\
		}\
		template <typename T,typename A>\
		inline void Bind( R (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), A& obj )\
		{\
			if(_pDestroyFunc != NULL)\
				_pDestroyFunc(_pObjHandle);\
			_pConstructionFunc = ImplObj<A>::Construction;\
			_pDestroyFunc = ImplObj<A>::Destroy;\
			_pObjHandle = ImplObj<A>::Construction_v1(obj);\
			T* ptr = static_cast<T*>(&(*obj));\
			m_pStaticfnFun = NULL;\
			m_pthis = _functionaddress_::SimplifyMemFunc<sizeof(pfnFun)>\
			::Convert(ptr, pfnFun, m_pfnFun);\
		}\
		inline void Bind( R (*pfnFun)(MACRO_ARGLIST_DEF(n)) )\
		{\
			if( pfnFun != NULL )\
			{\
				_pConstructionFunc = NULL;\
				_pDestroyFunc = NULL;\
				_pObjHandle = NULL;\
				m_pthis = (_functionaddress_::GenericClass*)this;\
				m_pStaticfnFun = pfnFun;\
				m_pfnFun = (GenericMemFn)&self_type::StaticFun;\
			}\
		}\
		inline _functionaddress_::GenericClass*	GetThis( void ) const\
		{\
			return m_pthis;\
		}\
		inline GenericMemFn	GetFunctionptr( void ) const\
		{\
			return m_pfnFun;\
		}\
	protected:\
		inline bool empty( void ) const\
		{\
			return (m_pthis == NULL || m_pfnFun == NULL);\
		}\
		inline R	Invoke(MACRO_TPARAMLIST_DEF(n))\
		{\
			return (m_pthis->*m_pfnFun)(MACRO_PARAMLIST_DEF(n));\
		}\
		inline R StaticFun(MACRO_TPARAMLIST_DEF(n))\
		{\
			return (*m_pStaticfnFun)(MACRO_PARAMLIST_DEF(n));\
		}\
	private:\
		_functionaddress_::GenericClass* m_pthis;\
		GenericMemFn  m_pfnFun;\
		R (*m_pStaticfnFun)(MACRO_ARGLIST_DEF(n));\
		ImplObjHandle		_pObjHandle;\
		pConstructionFunc	_pConstructionFunc;\
		pDestroyFunc		_pDestroyFunc;\
	};

	template<typename F>
	inline list_av_1<F> bind(F pfun)
	{
		return list_av_1<F>(pfun);
	}

	template<typename F,typename T>
	list_av_2<F,const T*> bind(F pfun, const T* pthis)
	{
		return list_av_2<F,const T*>(pfun,pthis);
	}

	template<typename F,typename C>
	list_av_2<F,const C&> bind(F pfun, C& obj)
	{
		return list_av_2<F,const C&>(pfun,obj);
	}

#define TEMPLATE_DEF(n)			typename R
#define TEMPLATE_ARGS_DEF(n)	typename R,MACRO_TARGLIST_DEF(n)

#define LIST_AV_PARAMLIST(n)	

#define SEPARATOR_DEF	
	FUNCTION_HANDLE_DECLAR(0,TEMPLATE_DEF(0))
#undef	SEPARATOR_DEF	
#undef	LIST_AV_PARAMLIST	

#define LIST_AV_PARAMLIST(n)	(MACRO_PARAMLIST_DEF(n))
#define SEPARATOR_DEF	,	
		FUNCTION_HANDLE_DECLAR(1,TEMPLATE_ARGS_DEF(1))
		FUNCTION_HANDLE_DECLAR(2,TEMPLATE_ARGS_DEF(2))
		FUNCTION_HANDLE_DECLAR(3,TEMPLATE_ARGS_DEF(3))
		FUNCTION_HANDLE_DECLAR(4,TEMPLATE_ARGS_DEF(4))
		FUNCTION_HANDLE_DECLAR(5,TEMPLATE_ARGS_DEF(5))
		FUNCTION_HANDLE_DECLAR(6,TEMPLATE_ARGS_DEF(6))
		FUNCTION_HANDLE_DECLAR(7,TEMPLATE_ARGS_DEF(7))
		FUNCTION_HANDLE_DECLAR(8,TEMPLATE_ARGS_DEF(8))
		FUNCTION_HANDLE_DECLAR(9,TEMPLATE_ARGS_DEF(9))
#undef SEPARATOR_DEF
#undef LIST_AV_PARAMLIST

#undef TEMPLATE_ARGS_DEF
#undef TEMPLATE_DEF
#undef FUNCTION_HANDLE_DECLAR


		/////////////////////////////////////////////////////////////////////////////
		namespace	_anyfunc_ {

			typedef	void*	AnyHandle;

			template <DEFAULT_TEMPLATE_DEF(9)>
			class AnyFunction;

#define ANY_FUNCTION_HANDLE_DECLAR(n)\
			template<MACRO_TARGLIST_DEF(n)>\
			class AnyFunction<void (MACRO_ARGLIST_DEF(n))> : public CMyFunction<void (MACRO_ARGLIST_DEF(n))>\
			{\
			public:\
				typedef	CMyFunction<void (MACRO_ARGLIST_DEF(n))>	base_type;\
				typedef AnyFunction self_type;\
			public:\
				inline static	void Invoke( AnyHandle Handle )\
				{\
					(*(reinterpret_cast<self_type*>(Handle)))();\
				}\
				inline	static	void	Destroy ( AnyHandle Handle )\
				{\
					if(Handle != NULL)\
						delete (reinterpret_cast<self_type*>(Handle));\
				}\
				inline	static	AnyHandle	Clone( AnyHandle Handle )\
				{\
					if(Handle != NULL)\
						return new self_type(*reinterpret_cast<self_type*>(Handle)) ;\
					return NULL;\
				}\
			public:\
				~AnyFunction( void )\
				{}\
				AnyFunction( void ): base_type() \
				{}\
				AnyFunction( const self_type& t1 ) \
					: base_type(static_cast<const base_type&>(t1))\
					,_list_av(t1._list_av)\
				{}\
				template <typename T>\
				AnyFunction( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) ) \
					: base_type(pfnFun,pthis), _list_av(MACRO_PARAMLIST_DEF(n))\
				{\
				}\
				template <typename T,typename C>\
				AnyFunction( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), C& obj SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) ) \
					: base_type(pfnFun,obj), _list_av(MACRO_PARAMLIST_DEF(n))\
				{\
				}\
				AnyFunction( void (*pfnFun)(MACRO_ARGLIST_DEF(n)) SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
					: base_type(pfnFun), _list_av(MACRO_PARAMLIST_DEF(n))\
				{\
				}\
				inline void operator()( void )\
				{\
					base_type::Invoke(MACRO_MEMBERLIST_DEF(n,_list_av));\
				}\
				inline self_type& operator=( const self_type& t1 )\
				{\
					_list_av = t1._list_av;\
					return *this;\
				}\
			private:\
				list_av_##n<MACRO_ARGLIST_DEF(n)>		_list_av;\
			};

#define SEPARATOR_DEF	
			ANY_FUNCTION_HANDLE_DECLAR(0)
#undef	SEPARATOR_DEF	

#define SEPARATOR_DEF	,
				ANY_FUNCTION_HANDLE_DECLAR(1)
				ANY_FUNCTION_HANDLE_DECLAR(2)
				ANY_FUNCTION_HANDLE_DECLAR(3)
				ANY_FUNCTION_HANDLE_DECLAR(4)
				ANY_FUNCTION_HANDLE_DECLAR(5)
				ANY_FUNCTION_HANDLE_DECLAR(6)
				ANY_FUNCTION_HANDLE_DECLAR(7)
				ANY_FUNCTION_HANDLE_DECLAR(8)
				ANY_FUNCTION_HANDLE_DECLAR(9)
#undef	SEPARATOR_DEF


#define BIND_DECLAR(n)\
			template<typename T SEPARATOR_DEF MACRO_TARGLIST_DEF(n)>\
			inline  static stAnyFuncTable bind( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
			{\
				typedef	AnyFunction<void (MACRO_ARGLIST_DEF(n))>	InvokeHandle;\
				AnyHandle	handle = (AnyHandle)new InvokeHandle(pfnFun,pthis SEPARATOR_DEF MACRO_PARAMLIST_DEF(n));\
				return stAnyFuncTable(InvokeHandle::Destroy, InvokeHandle::Invoke, InvokeHandle::Clone, handle );\
			}\
			template<typename T,typename C SEPARATOR_DEF MACRO_TARGLIST_DEF(n)>\
			inline  static stAnyFuncTable bind( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), C& obj SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
			{\
				typedef	AnyFunction<void (MACRO_ARGLIST_DEF(n))>	InvokeHandle;\
				AnyHandle	handle = (AnyHandle)new InvokeHandle(pfnFun,obj SEPARATOR_DEF MACRO_PARAMLIST_DEF(n));\
				return stAnyFuncTable(InvokeHandle::Destroy, InvokeHandle::Invoke, InvokeHandle::Clone, handle );\
			}\
			template<MACRO_TARGLIST_DEF(n)>\
			inline  static stAnyFuncTable bind( void (*pfnFun)(MACRO_ARGLIST_DEF(n)) SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
			{\
				typedef	AnyFunction<void (MACRO_ARGLIST_DEF(n))>	InvokeHandle;\
				AnyHandle	handle = (AnyHandle)new InvokeHandle(pfnFun SEPARATOR_DEF MACRO_PARAMLIST_DEF(n));\
				return stAnyFuncTable(InvokeHandle::Destroy, InvokeHandle::Invoke, InvokeHandle::Clone, handle );\
			}

#define	ANYFUNC_DECLAR(n)\
			template<typename T SEPARATOR_DEF MACRO_TARGLIST_DEF(n)>\
			AnyFunc( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), const T* pthis SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
			{\
				typedef	AnyFunction<void (MACRO_ARGLIST_DEF(n))>	InvokeHandle;\
				_stAnyFuncTable._handle = (AnyHandle)new InvokeHandle(pfnFun,pthis SEPARATOR_DEF MACRO_PARAMLIST_DEF(n));\
				_stAnyFuncTable._pInvokeHandle = InvokeHandle::Invoke;\
				_stAnyFuncTable._pDestroyHandle = InvokeHandle::Destroy;\
				_stAnyFuncTable._pCloneHandle = InvokeHandle::Clone;\
				}\
			template<typename T,typename C SEPARATOR_DEF MACRO_TARGLIST_DEF(n)>\
			AnyFunc( void (T::*pfnFun)(MACRO_ARGLIST_DEF(n)), C& obj SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
			{\
				typedef	AnyFunction<void (MACRO_ARGLIST_DEF(n))>	InvokeHandle;\
				_stAnyFuncTable._handle = (AnyHandle)new InvokeHandle(pfnFun,obj SEPARATOR_DEF MACRO_PARAMLIST_DEF(n));\
				_stAnyFuncTable._pInvokeHandle = InvokeHandle::Invoke;\
				_stAnyFuncTable._pDestroyHandle = InvokeHandle::Destroy;\
				_stAnyFuncTable._pCloneHandle = InvokeHandle::Clone;\
			}\
			template<MACRO_TARGLIST_DEF(n)>\
			AnyFunc( void (*pfnFun)(MACRO_ARGLIST_DEF(n)) SEPARATOR_DEF MACRO_TPARAMLIST_DEF(n) )\
			{\
				typedef	AnyFunction<void (MACRO_ARGLIST_DEF(n))>	InvokeHandle;\
				_stAnyFuncTable._handle = (AnyHandle)new InvokeHandle(pfnFun SEPARATOR_DEF MACRO_PARAMLIST_DEF(n));\
				_stAnyFuncTable._pInvokeHandle = InvokeHandle::Invoke;\
				_stAnyFuncTable._pDestroyHandle = InvokeHandle::Destroy;\
				_stAnyFuncTable._pCloneHandle = InvokeHandle::Clone;\
			}\
			BIND_DECLAR(n)

			class	stAnyFuncTable
			{
			public:
				typedef	void	(*pfunDestroyHandle)( AnyHandle );
				typedef	void	(*pfunInvokeHandle)( AnyHandle );
				typedef AnyHandle	(*pfunCloneHandle)( AnyHandle );

			public:
				stAnyFuncTable( void ) : _pDestroyHandle(NULL)
					,_pInvokeHandle(NULL)
					,_pCloneHandle(NULL)
					,_handle(NULL)
				{}
				stAnyFuncTable(const pfunDestroyHandle& pDestroyHandle,
					const pfunInvokeHandle&	pInvokeHandle,
					const pfunCloneHandle&		pCloneHandle,
					const AnyHandle&			handle)
					:_pDestroyHandle(pDestroyHandle)
					,_pInvokeHandle(pInvokeHandle)
					,_pCloneHandle(pCloneHandle)
					,_handle(handle)
				{}
				~stAnyFuncTable( void )
				{}
				pfunDestroyHandle		_pDestroyHandle;
				pfunInvokeHandle		_pInvokeHandle;
				pfunCloneHandle			_pCloneHandle;
				AnyHandle				_handle;
			};

			class	AnyFunc
			{
			public:
				typedef AnyFunc self_type;
			public:
				~AnyFunc( void )
				{
					if(_stAnyFuncTable._pDestroyHandle != NULL)
					{
						_stAnyFuncTable._pDestroyHandle(_stAnyFuncTable._handle);
						_stAnyFuncTable._pDestroyHandle = NULL;
						_stAnyFuncTable._handle = NULL;
					}
				}
				AnyFunc( void )
				{ 
					_stAnyFuncTable._pInvokeHandle = NULL;
					_stAnyFuncTable._pDestroyHandle = NULL;
					_stAnyFuncTable._pCloneHandle = NULL; 
					_stAnyFuncTable._handle = NULL;
				}

				AnyFunc( const AnyFunc& t1 )
				{
					_stAnyFuncTable._pInvokeHandle = t1._stAnyFuncTable._pInvokeHandle;
					_stAnyFuncTable._pDestroyHandle = t1._stAnyFuncTable._pDestroyHandle;
					_stAnyFuncTable._pCloneHandle = t1._stAnyFuncTable._pCloneHandle; 
					if(_stAnyFuncTable._pCloneHandle != NULL)
						_stAnyFuncTable._handle = _stAnyFuncTable._pCloneHandle(t1._stAnyFuncTable._handle);
				}

				AnyFunc( const stAnyFuncTable& t1 )
				{
					_stAnyFuncTable._pInvokeHandle = t1._pInvokeHandle;
					_stAnyFuncTable._pDestroyHandle = t1._pDestroyHandle;
					_stAnyFuncTable._pCloneHandle = t1._pCloneHandle; 
					_stAnyFuncTable._handle = t1._handle;
				}

				inline AnyFunc& operator=( const AnyFunc& t1 )
				{
					if(_stAnyFuncTable._pDestroyHandle != NULL)
						_stAnyFuncTable._pDestroyHandle(_stAnyFuncTable._handle);
					_stAnyFuncTable._pInvokeHandle = t1._stAnyFuncTable._pInvokeHandle;
					_stAnyFuncTable._pDestroyHandle = t1._stAnyFuncTable._pDestroyHandle;
					_stAnyFuncTable._pCloneHandle = t1._stAnyFuncTable._pCloneHandle; 
					if(_stAnyFuncTable._pCloneHandle != NULL)
						_stAnyFuncTable._handle = _stAnyFuncTable._pCloneHandle(t1._stAnyFuncTable._handle);
					return *this;
				}

				inline AnyFunc& operator=( const stAnyFuncTable& t1 )
				{
					if(_stAnyFuncTable._pDestroyHandle != NULL)
						_stAnyFuncTable._pDestroyHandle(_stAnyFuncTable._handle);
					_stAnyFuncTable._pInvokeHandle = t1._pInvokeHandle;
					_stAnyFuncTable._pDestroyHandle = t1._pDestroyHandle;
					_stAnyFuncTable._pCloneHandle = t1._pCloneHandle; 
					_stAnyFuncTable._handle = t1._handle;
					return *this;
				}

				inline void operator()( void )
				{
					_stAnyFuncTable._pInvokeHandle(_stAnyFuncTable._handle);
				}

#define SEPARATOR_DEF	,
				ANYFUNC_DECLAR(1)
				ANYFUNC_DECLAR(2)
				ANYFUNC_DECLAR(3)
				ANYFUNC_DECLAR(4)
				ANYFUNC_DECLAR(5)
				ANYFUNC_DECLAR(6)
				ANYFUNC_DECLAR(7)
				ANYFUNC_DECLAR(8)
				ANYFUNC_DECLAR(9)
#undef	SEPARATOR_DEF

			private:
				stAnyFuncTable	_stAnyFuncTable;
			};


#undef MACRO_MEMBERLIST_DEF

#undef BIND_DECLAR

	}	// namespace _anyfunc_

}	// namespace _function_

//#define function10_handle	_function_::CMyFunction

#endif
