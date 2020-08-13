#ifndef __BST_MACRO_20191021232104_1442199360_H__
#define __BST_MACRO_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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

#endif
