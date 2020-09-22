#ifndef __BST_MACRO_20191021232104_1442199360_H__
#define __BST_MACRO_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Bst_Common.hpp"
#include "Bst_Typeof.hpp"

#define BOOST_JOIN( X, Y ) BOOST_DO_JOIN( X, Y )
#define BOOST_DO_JOIN( X, Y ) BOOST_DO_JOIN2(X,Y)
#define BOOST_DO_JOIN2( X, Y ) X##Y

template <bool x> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };
template<int x> struct static_assert_test{};

#define BOOST_STATIC_ASSERT(B)	\
	typedef static_assert_test<	\
		sizeof(STATIC_ASSERTION_FAILURE< (bool)( B ) >)> \
		BOOST_JOIN(boost_static_assert_typedef_, __COUNTER__) //VC*/
	//BOOST_JOIN(boost_static_assert_typedef_, __COUNTER__) //∆‰À˚±‡“Î∆˜”√

#define MULTI_INDEX_RAW_I(macro)		macro

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
	#define MULTI_INDEX_RAW(macro)			MULTI_INDEX_RAW_I(macro)
#elif defined( PLATFORM_OS_FAMILY_UNIX )
	#define MULTI_INDEX_RAW(macro)			macro
#endif

#define MULTI_INDEX_TAG(name)			name##_tag
#define MULTI_INDEX_MEMBER(obj, name)	&obj::name

#define MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)\
	boost_wheel::multi_index::ordered_unique< boost_wheel::tag< MULTI_INDEX_TAG(memb) >,\
	boost_wheel::member< cla, ret, MULTI_INDEX_MEMBER(cla, memb) > >

#define MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)\
	boost_wheel::multi_index::ordered_non_unique< boost_wheel::tag< MULTI_INDEX_TAG(memb) >,\
	boost_wheel::member< cla, ret, MULTI_INDEX_MEMBER(cla, memb) > >


#define MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MACRO_PP_SEPARATOR_IF(i) MULTI_INDEX_RAW( MACRO_PP_CAT(MULTI_, field) )

#define MULTI_INDEX_DECL(dst, ...) \
	boost_wheel::multi_index::multi_index_container<\
		dst,\
		boost_wheel::vector<\
			MACRO_PP_SEQ_FOR_EACH_I(MULTI_INDEX_MEMBER_GET_FIELD, _, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
		>\
	>

#define TAG_MULTI_INDEX(name)	name##_tag

#define TAG_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)\
	struct TAG_MULTI_INDEX(memb) {};

#define TAG_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)\
	 TAG_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)

#define TAG_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW( MACRO_PP_CAT(TAG_MULTI_, field) )


#define TYPE_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)\
	struct memb##_tag{};

#define TYPE_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)\
	TYPE_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)


#define NAMESPACE_MEMBER_TYPE_MACRO_II(name,member)		name::member::type
#define NAMESPACE_MEMBER_TYPE_MACRO_I(name,member)		NAMESPACE_MEMBER_TYPE_MACRO_II(name, member)
#define NAMESPACE_MEMBER_TYPE_MACRO(name,member)		NAMESPACE_MEMBER_TYPE_MACRO_I(name, member)	

#define NAMESPACE_MEMBER_MACRO_II(name,member)			name::member
#define NAMESPACE_MEMBER_MACRO_I(name,member)			NAMESPACE_MEMBER_MACRO_II(name, member)
#define NAMESPACE_MEMBER_MACRO(name,member)				NAMESPACE_MEMBER_MACRO_I(name, member)


#define INDEX_MULTI_INDEX(name)										name##Index
#define FIELD_INDEX_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)		INDEX_MULTI_INDEX(memb)
#define FIELD_INDEX_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)	FIELD_INDEX_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)


#define FIELD_TAG_INDEX_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)		index< TAG_MULTI_INDEX(memb) >
#define FIELD_TAG_INDEX_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)	FIELD_TAG_INDEX_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)


#define TYPE_MULTI_INDEX(name)										name##_type
#define FIELD_NAME_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)		TYPE_MULTI_INDEX(memb)
#define FIELD_NAME_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)	FIELD_NAME_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)


#define ITERATOR_MULTI_INDEX(name)								name##_iterator
#define ITERATOR_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)		ITERATOR_MULTI_INDEX(memb)
#define ITERATOR_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)	ITERATOR_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)

#define TYPE_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	typedef NAMESPACE_MEMBER_TYPE_MACRO(data, nth_index<i>) MULTI_INDEX_RAW( MACRO_PP_CAT(FIELD_INDEX_MULTI_, field) );\
	typedef NAMESPACE_MEMBER_TYPE_MACRO(data, MULTI_INDEX_RAW( MACRO_PP_CAT(FIELD_TAG_INDEX_MULTI_, field) )) MULTI_INDEX_RAW( MACRO_PP_CAT(FIELD_NAME_MULTI_, field) );\
	typedef NAMESPACE_MEMBER_MACRO( MULTI_INDEX_RAW( MACRO_PP_CAT(FIELD_NAME_MULTI_, field) ), iterator) MULTI_INDEX_RAW( MACRO_PP_CAT(ITERATOR_MULTI_, field) );

#define PARAMETER_MULTI_INDEX_UNIQUE_ELEMENT(cla, ret, memb)		cla, ret, memb
#define PARAMETER_MULTI_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)	cla, ret, memb

#define PARAMETER_MULTI_INDEX_MACRO_II(...)				__VA_ARGS__
#define PARAMETER_MULTI_INDEX_MACRO_I(...)				PARAMETER_MULTI_INDEX_MACRO_II(__VA_ARGS__)
#define PARAMETER_MULTI_INDEX_MACRO(...)				PARAMETER_MULTI_INDEX_MACRO_I(__VA_ARGS__)


#define TAG_MULTI_INDEX_DECL(...)					MACRO_PP_SEQ_FOR_EACH_I(TAG_MULTI_INDEX_MEMBER_GET_FIELD, _, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define FIELD_TYPEDEF_MULTI_INDEX_DECL(tp, ...)		MACRO_PP_SEQ_FOR_EACH_I(TYPE_MULTI_INDEX_MEMBER_GET_FIELD, tp, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define GET_PARAMETER1_MULTI_INDEX_MACRO(p1, p2, p3) p1
#define GET_PARAMETER2_MULTI_INDEX_MACRO(p1, p2, p3) p2
#define GET_PARAMETER3_MULTI_INDEX_MACRO(p1, p2, p3) p3


#define MULTI_FUNC_INDEX_UNIQUE_ELEMENT(cla, ret, memb)\
	boost_wheel::multi_index::ordered_unique< boost_wheel::tag< MULTI_INDEX_TAG(memb) >,\
	boost_wheel::member_func< cla, ret, MULTI_INDEX_MEMBER(cla, memb) > >

#define MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT(cla, ret, memb)\
	boost_wheel::multi_index::ordered_non_unique< boost_wheel::tag< MULTI_INDEX_TAG(memb) >,\
	boost_wheel::member_func< cla, ret, MULTI_INDEX_MEMBER(cla, memb) > >

#define TAG_MULTI_FUNC_INDEX_UNIQUE_ELEMENT					TAG_MULTI_INDEX_UNIQUE_ELEMENT
#define TAG_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT				TAG_MULTI_INDEX_NON_UNIQUE_ELEMENT
#define TYPE_MULTI_FUNC_INDEX_UNIQUE_ELEMENT				TYPE_MULTI_INDEX_UNIQUE_ELEMENT
#define TYPE_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT			TYPE_MULTI_INDEX_NON_UNIQUE_ELEMENT
#define FIELD_INDEX_MULTI_FUNC_INDEX_UNIQUE_ELEMENT			FIELD_INDEX_MULTI_INDEX_UNIQUE_ELEMENT
#define FIELD_INDEX_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT		FIELD_INDEX_MULTI_INDEX_NON_UNIQUE_ELEMENT
#define FIELD_TAG_INDEX_MULTI_FUNC_INDEX_UNIQUE_ELEMENT		FIELD_TAG_INDEX_MULTI_INDEX_UNIQUE_ELEMENT
#define FIELD_TAG_INDEX_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT	FIELD_TAG_INDEX_MULTI_INDEX_NON_UNIQUE_ELEMENT
#define FIELD_NAME_MULTI_FUNC_INDEX_UNIQUE_ELEMENT			FIELD_NAME_MULTI_INDEX_UNIQUE_ELEMENT
#define FIELD_NAME_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT		FIELD_NAME_MULTI_INDEX_NON_UNIQUE_ELEMENT
#define ITERATOR_MULTI_FUNC_INDEX_UNIQUE_ELEMENT			ITERATOR_MULTI_INDEX_UNIQUE_ELEMENT
#define ITERATOR_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT		ITERATOR_MULTI_INDEX_NON_UNIQUE_ELEMENT
#define PARAMETER_MULTI_FUNC_INDEX_UNIQUE_ELEMENT			PARAMETER_MULTI_INDEX_UNIQUE_ELEMENT
#define PARAMETER_MULTI_FUNC_INDEX_NON_UNIQUE_ELEMENT		PARAMETER_MULTI_INDEX_NON_UNIQUE_ELEMENT


#define INSERT_MULTI_INDEX_FUNCTION_DECL(tp, tp_name, dst, ...)\
	int MultiIndex_Insert(reference_type<dst>::type Val)\
	{\
		std::pair< NAMESPACE_MEMBER_MACRO(tp, iterator),bool> Ret = tp_name.insert(Val);\
		return Ret.second ? 1 : -1;\
	}

#define FIND_MULTI_INDEX_ELEMENT(function, tp_name, dst, cla, ret, memb)\
	dst MultiIndex_FindFrom_##memb(reference_type<ret>::type Val)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Val );\
		if( iter == indexOf.end() )\
		{\
			return ret_init<dst>::init();\
		}\
		BOOST_WHEEL_TYPEOF(*iter) ptr = *iter;\
		return ptr;\
	}\
	UInt64 MultiIndex_FindsFrom_##memb(reference_type<ret>::type Val, const function& func)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		UInt64 uCount = indexOf.count(Val);\
		if( uCount > 0 )\
		{\
			ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Val );\
			if( func )\
			{\
				for(int i = 0; i < uCount; i ++)\
				{\
					if( !func(uCount, i, (*iter)) )\
						break;\
					++ iter;\
				}\
			}\
		}\
		return uCount;\
	}

#define FIND_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(FIND_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define FIND_MULTI_INDEX_FUNCTION_DECL(function, tp_name, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(FIND_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(function, tp_name, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define REPLACE_MULTI_INDEX_ELEMENT(tp_name, dst, cla, ret, memb)\
	bool MultiIndex_Replace_##memb(reference_type<ret>::type Key, reference_type<dst>::type Val)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Key );\
		if(iter != indexOf.end())\
		{\
			return indexOf.replace(iter, Val);\
		}\
		return false;\
	}

#define REPLACE_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(REPLACE_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define REPLACE_MULTI_INDEX_FUNCTION_DECL(tp_name, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(REPLACE_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(tp_name, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define REMOVE_MULTI_INDEX_ELEMENT(tp_name, dst, cla, ret, memb)\
	dst MultiIndex_RemoveFrom_##memb(reference_type<ret>::type Val)\
	{\
		dst t;\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Val );\
		if(iter != indexOf.end())\
		{\
			t = *iter;\
			indexOf.erase(Val); \
		}\
		return t;\
	}

#define REMOVE_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(REMOVE_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define REMOVE_MULTI_INDEX_FUNCTION_DECL(tp_name, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(REMOVE_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(tp_name, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))


#define COUNT_MULTI_INDEX_ELEMENT(tp_name, dst, cla, ret, memb)\
	UInt64 MultiIndex_Count_##memb(reference_type<ret>::type Val)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		return indexOf.count(Val);\
	}

#define CLEAR_MULTI_INDEX_ELEMENT(tp_name)\
	void MultiIndex_Clear( void )\
	{\
		tp_name.clear();\
	}

#define SIZE_MULTI_INDEX_ELEMENT(tp_name)\
	std::size_t MultiIndex_Size( void )\
	{\
		return tp_name.size();\
	}

#define COUNT_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(COUNT_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define COUNT_MULTI_INDEX_FUNCTION_DECL(tp_name, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(COUNT_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(tp_name, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define TYPEDEF_MULTI_INDEX_DECL(tp, tp_name, dst, ...) \
	TAG_MULTI_INDEX_DECL(__VA_ARGS__)\
	typedef MULTI_INDEX_DECL(dst, __VA_ARGS__) tp;\
	tp tp_name;\
	FIELD_TYPEDEF_MULTI_INDEX_DECL(tp, __VA_ARGS__)\
	INSERT_MULTI_INDEX_FUNCTION_DECL(tp, tp_name, dst, __VA_ARGS__)\
	REPLACE_MULTI_INDEX_FUNCTION_DECL(tp_name, dst, __VA_ARGS__)\
	REMOVE_MULTI_INDEX_FUNCTION_DECL(tp_name, dst, __VA_ARGS__)\
	COUNT_MULTI_INDEX_FUNCTION_DECL(tp_name, dst, __VA_ARGS__)\
	typedef function20_handle<bool (UInt64, UInt64, dst)>	HFNFind;\
	FIND_MULTI_INDEX_FUNCTION_DECL(HFNFind,tp_name, dst, __VA_ARGS__)\
	CLEAR_MULTI_INDEX_ELEMENT(tp_name)\
	SIZE_MULTI_INDEX_ELEMENT(tp_name)

#define INSERT_LOCK_MULTI_INDEX_FUNCTION_DECL(tp, tp_name, lock, dst, ...)\
	int MultiIndex_Insert(reference_type<dst>::type Val)\
	{\
		lock.Lock();\
		std::pair< NAMESPACE_MEMBER_MACRO(tp, iterator),bool> Ret = tp_name.insert(Val);\
		lock.UnLock();\
		return Ret.second ? 1 : -1;\
	}


#define FIND_LOCK_MULTI_INDEX_ELEMENT(function, tp_name, lock, dst, cla, ret, memb)\
	dst MultiIndex_FindFrom_##memb(reference_type<ret>::type Val)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		lock.Lock();\
		ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Val );\
		if( iter == indexOf.end() )\
		{\
			lock.UnLock();\
			return ret_init<dst>::init();\
		}\
		BOOST_WHEEL_TYPEOF(*iter) ptr = *iter;\
		lock.UnLock();\
		return ptr;\
	}\
	UInt64 MultiIndex_FindsFrom_##memb(reference_type<ret>::type Val, const function& func)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		lock.Lock();\
		UInt64 uCount = indexOf.count(Val);\
		if( uCount > 0 )\
		{\
			ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Val );\
			if( func )\
			{\
				for(int i = 0; i < uCount; i ++)\
				{\
					if( !func(uCount, i, (*iter)) )\
						break;\
					++ iter;\
				}\
			}\
		}\
		lock.UnLock();\
		return uCount;\
	}
	
#define FIND_LOCK_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(FIND_LOCK_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define FIND_LOCK_MULTI_INDEX_FUNCTION_DECL(function, tp_name, lock, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(FIND_LOCK_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(function, tp_name, lock, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define REPLACE_LOCK_MULTI_INDEX_ELEMENT(tp_name, lock, dst, cla, ret, memb)\
	bool MultiIndex_Replace_##memb(reference_type<ret>::type Key, reference_type<dst>::type Val )\
	{\
		bool isRet = false;\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		lock.Lock();\
		ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Key );\
		if(iter != indexOf.end())\
		{\
			isRet = indexOf.replace(iter, Val);\
		}\
		lock.UnLock(); \
		return isRet;\
	}

#define REPLACE_LOCK_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(REPLACE_LOCK_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define REPLACE_LOCK_MULTI_INDEX_FUNCTION_DECL(tp_name, lock, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(REPLACE_LOCK_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(tp_name, lock, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define REMOVE_LOCK_MULTI_INDEX_ELEMENT(tp_name, lock, dst, cla, ret, memb)\
	dst MultiIndex_RemoveFrom_##memb(reference_type<ret>::type Val)\
	{\
		dst t;\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		lock.Lock();\
		ITERATOR_MULTI_INDEX(memb) iter = indexOf.find( Val );\
		if(iter != indexOf.end())\
		{\
			t = *iter;\
			indexOf.erase(Val); \
		}\
		lock.UnLock(); \
		return t;\
	}

#define REMOVE_LOCK_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(REMOVE_LOCK_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define REMOVE_LOCK_MULTI_INDEX_FUNCTION_DECL(tp_name, lock, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(REMOVE_LOCK_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(tp_name, lock, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define COUNT_LOCK_MULTI_INDEX_ELEMENT(tp_name, lock, dst, cla, ret, memb)\
	UInt64 MultiIndex_Count_##memb(reference_type<ret>::type Val)\
	{\
		TYPE_MULTI_INDEX(memb) & indexOf = tp_name.get< TAG_MULTI_INDEX(memb) >();\
		lock.Lock();\
		UInt64 uCount = indexOf.count(Val);\
		lock.UnLock();\
		return uCount;\
	}

#define CLEAR_LOCK_MULTI_INDEX_ELEMENT(tp_name, lock)\
	void MultiIndex_Clear( void )\
	{\
		lock.Lock();\
		tp_name.clear();\
		lock.UnLock();\
	}

#define SIZE_LOCK_MULTI_INDEX_ELEMENT(tp_name, lock)	SIZE_MULTI_INDEX_ELEMENT(tp_name)


#define COUNT_LOCK_MULTI_INDEX_MEMBER_GET_FIELD(r, data, i, field)\
	MULTI_INDEX_RAW(MACRO_PP_CAT(COUNT_LOCK_, MULTI_INDEX_ELEMENT( MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_,data)), MULTI_INDEX_RAW(MACRO_PP_CAT(PARAMETER_MULTI_, field)) ) ))

#define COUNT_LOCK_MULTI_INDEX_FUNCTION_DECL(tp_name, lock, dst, ...)\
	MACRO_PP_SEQ_FOR_EACH_I(COUNT_LOCK_MULTI_INDEX_MEMBER_GET_FIELD,  MULTI_INDEX_MACRO(tp_name, lock, dst), MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define TYPEDEF_LOCK_MULTI_INDEX_DECL(tp, tp_name, dst, ...) \
	TAG_MULTI_INDEX_DECL(__VA_ARGS__)\
	typedef MULTI_INDEX_DECL(dst, __VA_ARGS__) tp;\
	tp tp_name;\
	CLock	_MultiIndexLock;\
	FIELD_TYPEDEF_MULTI_INDEX_DECL(tp, __VA_ARGS__)\
	INSERT_LOCK_MULTI_INDEX_FUNCTION_DECL(tp, tp_name, _MultiIndexLock, dst, __VA_ARGS__)\
	REPLACE_LOCK_MULTI_INDEX_FUNCTION_DECL(tp_name, _MultiIndexLock, dst, __VA_ARGS__)\
	REMOVE_LOCK_MULTI_INDEX_FUNCTION_DECL(tp_name, _MultiIndexLock, dst, __VA_ARGS__)\
	COUNT_LOCK_MULTI_INDEX_FUNCTION_DECL(tp_name, _MultiIndexLock, dst, __VA_ARGS__)\
	typedef function20_handle<bool (UInt64, UInt64, dst)>	HFNFind;\
	FIND_LOCK_MULTI_INDEX_FUNCTION_DECL(HFNFind, tp_name, _MultiIndexLock, dst, __VA_ARGS__)\
	CLEAR_LOCK_MULTI_INDEX_ELEMENT(tp_name, _MultiIndexLock)\
	SIZE_LOCK_MULTI_INDEX_ELEMENT(tp_name, _MultiIndexLock)
	


#define MULTI_INDEX_INSERT_FUNCTION(dst)					MultiIndex_Insert(dst)

#define MULTI_INDEX_FIND_FUNCTION(memb, val)				MultiIndex_FindFrom_##memb(val)

#define MULTI_INDEX_FINDS_FUNCTION(memb, val, function)		MultiIndex_FindsFrom_##memb(val, function)

#define MULTI_INDEX_REPLACE_FUNCTION(memb, key, val)		MultiIndex_Replace_##memb(key, val)

#define MULTI_INDEX_REMOVE_FUNCTION(memb, val)				MultiIndex_RemoveFrom_##memb(val)

#define MULTI_INDEX_COUNT_FUNCTION(memb, val)				MultiIndex_Count_##memb(val)

#define MULTI_INDEX_CLEAR_FUNCTION()						MultiIndex_Clear()

#define MULTI_INDEX_SIZE_FUNCTION()							MultiIndex_Size()



#define CLASS_MULTI_INDEX_INSERT_FUNCTION(obj, dst)						obj.MultiIndex_Insert(dst)

#define CLASS_MULTI_INDEX_FIND_FUNCTION(obj, memb, val)					obj.MultiIndex_FindFrom_##memb(val)

#define CLASS_MULTI_INDEX_FINDS_FUNCTION(obj, memb, val, function)		obj.MultiIndex_FindsFrom_##memb(val, function)

#define CLASS_MULTI_INDEX_REPLACE_FUNCTION(obj, memb, key, val)			obj.MultiIndex_Replace_##memb(key, val)

#define CLASS_MULTI_INDEX_REMOVE_FUNCTION(obj, memb, val)				obj.MultiIndex_RemoveFrom_##memb(val)

#define CLASS_MULTI_INDEX_COUNT_FUNCTION(obj, memb, val)				obj.MultiIndex_Count_##memb(val)

#define CLASS_MULTI_INDEX_CLEAR_FUNCTION(obj)							obj.MultiIndex_Clear()

#define CLASS_MULTI_INDEX_SIZE_FUNCTION(obj)							obj.MultiIndex_Size()




#define PTR_MULTI_INDEX_INSERT_FUNCTION(ptr, dst)						ptr->MultiIndex_Insert(dst)

#define PTR_MULTI_INDEX_FIND_FUNCTION(ptr, memb, val)					ptr->MultiIndex_FindFrom_##memb(val)

#define PTR_MULTI_INDEX_FINDS_FUNCTION(ptr, memb, val, function)		ptr->MultiIndex_FindsFrom_##memb(val, function)

#define PTR_MULTI_INDEX_REPLACE_FUNCTION(ptr, memb, key, val)			ptr->MultiIndex_Replace_##memb(key, val)

#define PTR_MULTI_INDEX_REMOVE_FUNCTION(ptr, memb, val)					ptr->MultiIndex_RemoveFrom_##memb(val)

#define PTR_MULTI_INDEX_COUNT_FUNCTION(ptr, memb, val)					ptr->MultiIndex_Count_##memb(val)

#define PTR_MULTI_INDEX_CLEAR_FUNCTION(ptr)								ptr->MultiIndex_Clear()

#define PTR_MULTI_INDEX_SIZE_FUNCTION(ptr)								ptr->MultiIndex_Size()

#endif

