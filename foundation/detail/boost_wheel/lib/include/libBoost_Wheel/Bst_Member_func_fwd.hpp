#ifndef __BST_MEMBER_FUNC_20191021232104_1442199360_H__
#define __BST_MEMBER_FUNC_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libBoost_Wheel/Bst_Common.hpp>

namespace boost_wheel{

template<class Class,typename Type,Type (Class::*PtrToMember)()>
struct const_member_func_base
{
	typedef Type result_type;

	template<typename ChainedPtr>
	Type operator()(const ChainedPtr& x)const
	{
		return operator()(*x);
	}

	Type operator()(const Class& x)const
	{
		return (x.*PtrToMember)();
	}

	Type operator()(const reference_wrapper<const Class>& x)const
	{
		return operator()(x.get());
	}

	Type operator()(const reference_wrapper<Class>& x,int=0)const
	{ 
		return operator()(x.get());
	}

};

template<class Class,typename Type,Type (Class::*PtrToMember)()>
struct non_const_member_func_base
{
	typedef Type result_type;

	template<typename ChainedPtr>
	Type operator()(const ChainedPtr& x)const
	{
		return operator()(*x);
	}

	const Type operator()(const Class& x,int=0)const
	{
		return (x.*PtrToMember)();
	}

	Type operator()(Class& x)const
	{ 
		return (x.*PtrToMember)();
	}

	const Type operator()(const reference_wrapper<const Class>& x,int=0)const
	{
		return operator()(x.get());
	}

	Type operator()(const reference_wrapper<Class>& x) const
	{ 
		return operator()(x.get());
	}
};


template<class Class,typename Type,Type (Class::*PtrToMember)()>
struct member_func:
	if_<
	is_const<Type>::value,
	const_member_func_base<Class,Type,PtrToMember>,
	non_const_member_func_base<Class,Type,PtrToMember>
	>::type
{
};

}

#endif

