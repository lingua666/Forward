
#ifndef  __CUSTOM_FUNCTIONADDRESS_HEAD_DEF__
#define  __CUSTOM_FUNCTIONADDRESS_HEAD_DEF__

#include <libCommon/modafx.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////
//
//使用原则：尽可能多用CMyFunction对象替代AnyFunc对象
//
///////////////////////////////////////////////////////////////

namespace	_functionaddress_{

//空父类
//class __single_inheritance GenericClass;
class GenericClass {};

const int SINGLE_MEMFUNCPTR_SIZE = sizeof(void (GenericClass::*)());
const int SINGLE_MEMFUNCPTR_SIZE_INT = SINGLE_MEMFUNCPTR_SIZE + sizeof(int);
const int SINGLE_MEMFUNCPTR_SIZE_2INT = SINGLE_MEMFUNCPTR_SIZE_INT + sizeof(int);


/*////////////////////////////////////////注释//////////////////////////////////
/*第一种情况：
/*typedef void (B1::*FPB1)();   
{
	mem function address;  // 4 bytes，成员函数地址
}

第二种情况：
typedef void (D1::*FPB1)();   
{
	mem function address; // 4 bytes;，成员函数地址
	delta  added to this pointer; // 4 bytes，this指针偏移
}

第三种情况：
typedef void (D2::*FPB1)();   
{
	mem function address; // 4 bytes;，成员函数地址
	delta  added to this pointer; // 4 bytes，this指针偏移
	index in virtual base class table; // 4 byptes ,虚基类列表中的序号
}
//*/
//

//成员函数指针转换模板
template <int N>  
struct SimplifyMemFunc   
{  
	template <typename X, typename XFuncType, typename GenericMemFuncType>  
	inline static GenericClass *Convert(X *pthis, XFuncType& function_to_bind,   
									GenericMemFuncType &bound_func) { }
}; 

//第一种情况
template <>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE>  
{	
	template <typename X, typename XFuncType, typename GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType& function_to_bind, 
		GenericMemFuncType &bound_func) 
	{
			bound_func = reinterpret_cast<GenericMemFuncType>(function_to_bind);
			return reinterpret_cast<GenericClass *>(pthis);
	}
};

//第二种情况
template<>
struct SimplifyMemFunc< SINGLE_MEMFUNCPTR_SIZE + sizeof(int) >  
{
	template <typename X, typename XFuncType, typename GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType& function_to_bind, 
		GenericMemFuncType &bound_func) 
	{ 
			// We need to use a horrible_cast to do this conversion.
			// In MSVC, a multiple inheritance member pointer is internally defined as:
			union {
				XFuncType func;
				struct {	 
					GenericMemFuncType funcaddress; // points to the actual member function
					int delta;	     // #BYTES to be added to the 'this' pointer
				}s;
			} u;
			u.func = function_to_bind;
			bound_func = u.s.funcaddress;
			return reinterpret_cast<GenericClass *>(reinterpret_cast<char *>(pthis) + u.s.delta); 
	}
};

struct MicrosoftVirtualMFP 
{  
	void (GenericClass::*codeptr)(); // points to the actual member function  
	int delta;      // #bytes to be added to the 'this' pointer  
	int vtable_index; // or 0 if no virtual inheritance  
};  

struct GenericVirtualClass : virtual public GenericClass  
{  
	typedef GenericVirtualClass * (GenericVirtualClass::*ProbePtrType)();  
	GenericVirtualClass * GetThis() { return this; }  
};  

//第三情况
template <>  
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + 2*sizeof(int) >  
{  
	template <typename X, typename XFuncType, typename GenericMemFuncType>  
	inline static GenericClass *Convert(X *pthis, XFuncType& function_to_bind,   
		GenericMemFuncType &bound_func) 
	{  
			union 
			{  
				XFuncType func;
				GenericClass* (X::*ProbeFunc)(); 
				MicrosoftVirtualMFP s;
			} u;  
			u.func = function_to_bind;
			bound_func = reinterpret_cast<GenericMemFuncType>(u.s.codeptr);  
			union 
			{  
				GenericVirtualClass::ProbePtrType virtfunc;
				MicrosoftVirtualMFP s;  
			} u2;  

			u2.virtfunc = &GenericVirtualClass::GetThis;
			u.s.codeptr = u2.s.codeptr;  
			return (pthis->*u.ProbeFunc)();
	}  
};

//第四种情况:只适用于Microsoft and Intel (IA32 and Itanium) 
//			 and  Microsoft Visual Studio enable /vmg option
// Nasty hack for Microsoft and Intel (IA32 and Itanium)
// unknown_inheritance classes go here 
// This is probably the ugliest bit of code I've ever written. Look at the casts!
// There is a compiler bug in MSVC6 which prevents it from using this code.
/*template <>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + 3*sizeof(int) >
{
	template <class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType function_to_bind, 
		GenericMemFuncType &bound_func) {
			// The member function pointer is 16 bytes long. We can't use a normal cast, but
			// we can use a union to do the conversion.
			union {
				XFuncType func;
				// In VC++ and ICL, an unknown_inheritance member pointer 
				// is internally defined as:
				struct {
					GenericMemFuncType m_funcaddress; // points to the actual member function
					int delta;              // #bytes to be added to the 'this' pointer
					int vtordisp;           // #bytes to add to 'this' to find the vtable
					int vtable_index; // or 0 if no virtual inheritance
				} s;
			} u;
			// Check that the horrible_cast will work
			typedef int ERROR_CantUsehorrible_cast[sizeof(XFuncType)==sizeof(u.s)? 1 : -1];
			u.func = function_to_bind;
			bound_func = u.s.funcaddress;
			int virtual_delta = 0;
			if (u.s.vtable_index) { // Virtual inheritance is used
				// First, get to the vtable. 
				// It is 'vtordisp' bytes from the start of the class.
				const int * vtable = *reinterpret_cast<const int *const*>(
					reinterpret_cast<const char *>(pthis) + u.s.vtordisp );

				// 'vtable_index' tells us where in the table we should be looking.
				virtual_delta = u.s.vtordisp + *reinterpret_cast<const int *>( 
					reinterpret_cast<const char *>(vtable) + u.s.vtable_index);
			}
			// The int at 'virtual_delta' gives us the amount to add to 'this'.
			// Finally we can add the three components together. Phew!
			return reinterpret_cast<GenericClass *>(
				reinterpret_cast<char *>(pthis) + u.s.delta + virtual_delta);
	}
};*/



//成员函数指针转换模板
template <int N>
struct GeneraldutyFunc
{
	inline static GenericClass *Convert(char* pthis, IntPtr* function_to_bind, 
		IntPtr* bound_func) 
	{
		return NULL;
	}
};

template <>
struct GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE>
{	
	inline static GenericClass *Convert(char* pthis, IntPtr* function_to_bind, 
							IntPtr* bound_func, char* pProbeFunc = NULL) 
	{
		*bound_func = *function_to_bind;
		return reinterpret_cast<GenericClass *>(pthis);
	}
};

//第二种情况
template<>
struct GeneraldutyFunc< SINGLE_MEMFUNCPTR_SIZE_INT >
{
	inline static GenericClass *Convert(char* pthis, IntPtr* function_to_bind, 
		IntPtr* bound_func, IntPtr ProbeFunc = 0 ) 
	{ 
		// We need to use a horrible_cast to do this conversion.
		// In MSVC, a multiple inheritance member pointer is internally defined as:
		union {
			__M_ALIGNED__(4) char func[SINGLE_MEMFUNCPTR_SIZE_INT];
			struct {	 
				IntPtr funcaddress; // points to the actual member function
				int delta;	     // #BYTES to be added to the 'this' pointer
			}s;
		} u;
		memcpy(u.func, reinterpret_cast<char*>(function_to_bind), sizeof(u.func) );
		*bound_func = u.s.funcaddress;
		return reinterpret_cast<GenericClass *>(pthis + u.s.delta); 
	}
};

//第三情况  出现崩溃，待改进
template <>
struct GeneraldutyFunc< SINGLE_MEMFUNCPTR_SIZE_2INT >
{  
	inline static GenericClass *Convert(char* pthis, IntPtr* function_to_bind, 
		IntPtr* bound_func, IntPtr ProbeFunc) 
	{
		union 
		{  
			__M_ALIGNED__(4) char func[SINGLE_MEMFUNCPTR_SIZE_2INT]; 
			MicrosoftVirtualMFP s;
		} u; 
		memcpy(u.func, function_to_bind, SINGLE_MEMFUNCPTR_SIZE_2INT );
		*bound_func = *reinterpret_cast<IntPtr*>(&u.s.codeptr);	
		union 
		{  
			GenericVirtualClass::ProbePtrType virtfunc;
			MicrosoftVirtualMFP s;  
		} u2;

		u2.virtfunc = &GenericVirtualClass::GetThis;
		u.s.codeptr = u2.s.codeptr;
		
		union 
		{  
			IntPtr value;
			GenericClass* (GenericClass::*ConvertProbe)( char* );
		} u3;
		u3.value = ProbeFunc;
		return (reinterpret_cast<GenericClass*>(pthis)->*u3.ConvertProbe)(reinterpret_cast<char*>(&u));
	}
};

/*typedef GenericClass* (*FunctionConvert)(char*, IntPtr*, IntPtr*, IntPtr);
const FunctionConvert	g_FunctionConvert_20150909[3] = { &GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE>::Convert,
														&GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE_INT>::Convert,
														&GeneraldutyFunc<SINGLE_MEMFUNCPTR_SIZE_2INT>::Convert };*/

}	// namespace _function_

#endif
