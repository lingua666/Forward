#ifndef __BST_VECTOR_FWD_20191021232104_1442199360_H__
#define __BST_VECTOR_FWD_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <dllLock/Exports.h>
#include <libLock/Lock.hpp>
#include <libCommon/macro/Macro_for.hpp>

namespace boost_wheel{

template<class V, class Increase_BOOST_TYPEOF_LIMIT_SIZE> struct v_iter;
template<class V> struct v_iter<V, mpl::int_<0> > { typedef typename V::item0 type; typedef v_iter<V, mpl::int_<0 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<1> > { typedef typename V::item1 type; typedef v_iter<V, mpl::int_<1 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<2> > { typedef typename V::item2 type; typedef v_iter<V, mpl::int_<2 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<3> > { typedef typename V::item3 type; typedef v_iter<V, mpl::int_<3 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<4> > { typedef typename V::item4 type; typedef v_iter<V, mpl::int_<4 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<5> > { typedef typename V::item5 type; typedef v_iter<V, mpl::int_<5 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<6> > { typedef typename V::item6 type; typedef v_iter<V, mpl::int_<6 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<7> > { typedef typename V::item7 type; typedef v_iter<V, mpl::int_<7 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<8> > { typedef typename V::item8 type; typedef v_iter<V, mpl::int_<8 + 1> > next; };
template<class V> struct v_iter<V, mpl::int_<9> > { typedef typename V::item9 type; typedef v_iter<V, mpl::int_<9 + 1> > next; };


template< long N > struct vector_tag;

template< class T = void> struct vector0 { enum{ value = 0 }; typedef vector_tag<0> tag; typedef v_iter<vector0<>, mpl::int_<0> > begin; };
template< class P0 > struct vector1 { enum{ value = 1 }; typedef vector_tag<1> tag; typedef v_iter<vector1< P0>, mpl::int_<0> > begin; typedef P0 item0; };
template< class P0 , class P1 > struct vector2 { enum{ value = 2 }; typedef vector_tag<2> tag; typedef v_iter<vector2< P0 , P1>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; };
template< class P0 , class P1 , class P2 > struct vector3 { enum{ value = 3 }; typedef vector_tag<3> tag; typedef v_iter<vector3< P0 , P1 , P2>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; };
template< class P0 , class P1 , class P2 , class P3 > struct vector4 { enum{ value = 4 }; typedef vector_tag<4> tag; typedef v_iter<vector4< P0 , P1 , P2 , P3>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; };
template< class P0 , class P1 , class P2 , class P3 , class P4 > struct vector5 { enum{ value = 5 }; typedef vector_tag<5> tag; typedef v_iter<vector5< P0 , P1 , P2 , P3 , P4>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 > struct vector6 { enum{ value = 6 }; typedef vector_tag<6> tag; typedef v_iter<vector6< P0 , P1 , P2 , P3 , P4 , P5>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 > struct vector7 { enum{ value = 7 }; typedef vector_tag<7> tag; typedef v_iter<vector7< P0 , P1 , P2 , P3 , P4 , P5 , P6>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef P6 item6;};
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 > struct vector8 { enum{ value = 8 }; typedef vector_tag<8> tag; typedef v_iter<vector8< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef P6 item6; typedef P7 item7; };
template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 > struct vector9 { enum{ value = 9 }; typedef vector_tag<9> tag; typedef v_iter<vector9< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8>, mpl::int_<0> > begin; typedef P0 item0; typedef P1 item1; typedef P2 item2; typedef P3 item3; typedef P4 item4; typedef P5 item5; typedef P6 item6; typedef P7 item7; typedef P8 item8; };


#define VECTOR_AT_DECL(n)\
	template< typename V >\
	struct at_c< V,n >\
	{\
		typedef typename V::item##n type;\
	}

template< typename T, int n >
struct at_c{};

VECTOR_AT_DECL(0);
VECTOR_AT_DECL(1);
VECTOR_AT_DECL(2);
VECTOR_AT_DECL(3);
VECTOR_AT_DECL(4);
VECTOR_AT_DECL(5);
VECTOR_AT_DECL(6);
VECTOR_AT_DECL(7);
VECTOR_AT_DECL(8);

template <DEFAULT_TEMPLATE_DEF(9)>
struct vector : public vector9<MACRO_ARGLIST_DEF(9)>
{
	typedef vector9<MACRO_ARGLIST_DEF(9)> type;
};

#define	VECTOR_DECLAR(n)\
	template<MACRO_TARGLIST_DEF(n)>\
	struct vector<MACRO_ARGLIST_DEF(n)> : public vector##n<MACRO_ARGLIST_DEF(n)> \
	{\
		typedef vector##n<MACRO_ARGLIST_DEF(n)> type;\
	}

VECTOR_DECLAR(0);
VECTOR_DECLAR(1);
VECTOR_DECLAR(2);
VECTOR_DECLAR(3);
VECTOR_DECLAR(4);
VECTOR_DECLAR(5);
VECTOR_DECLAR(6);
VECTOR_DECLAR(7);
VECTOR_DECLAR(8);

template<typename Sequence>
struct size
{
	typedef typename Sequence::type	type;
};

struct tag_marker{};

template<typename T = na>
struct tag : private tag_marker
{
	typedef vector<T> type;

	/*typedef typename mpl::transform<
		mpl::vector<BOOST_PP_ENUM_PARAMS(BOOST_MULTI_INDEX_TAG_SIZE,T)>,
		mpl::identity<mpl::_1>
	>::type type;*/
};

template<typename T>
struct is_tag
{
	static const bool value = is_base_of<tag_marker, T>::value;
};

template< typename Sequence = na >
struct sequence_tag
{
	typedef typename Sequence::tag type;
};

template< typename Tag = na >
struct sequence_size
{
	enum{ value = -1 };
};

template<long N>
struct sequence_size< vector_tag<N> >
{
	enum{ value = N };
};

#define VECTOR_ITEM_MEMBER(vector, N) typename vector::item##N

#define VECTOR_ITEM_MEMBER_GET_FIELD(r, data, i, field)\
	MACRO_PP_SEPARATOR_IF(i) VECTOR_ITEM_MEMBER(data,i)

#define VECTOR_ITEM_IMPL(vector, N) \
	MACRO_PP_SEQ_FOR_EACH_I(VECTOR_ITEM_MEMBER_GET_FIELD,vector,MACRO_PP_VARIADIC_TO_SEQ(MACRO_TARGLIST_DEF(N)))

#define VECTOR_N_I_INC(vector,N)	 MACRO_PP_CAT(vector, N)
#define VECTOR_N_INC(vector,N)	 VECTOR_N_I_INC(vector, MACRO_NUMBER_INC(N))

#define VECTOR_N_II_DECL(vector,T1,...) vector<T1,__VA_ARGS__>
#define VECTOR_N_I_DECL(vector,T1,...) VECTOR_N_II_DECL(vector,T1,__VA_ARGS__) 
#define VECTOR_PUSH_FRONT_DECL(dst,N,T,src) VECTOR_N_I_DECL(VECTOR_N_INC(dst,N),T,VECTOR_ITEM_IMPL(src, N))

#define PUSH_FRONT_IMPL_DECL(N)\
	template<>\
	struct push_front_impl< vector_tag<N> >\
	{\
		template< typename Vector, typename T > struct apply\
		{\
			typedef VECTOR_PUSH_FRONT_DECL(vector, N, T, Vector) type;\
		};\
	};


template< typename Tag = na >
struct push_front_impl
{
	template< typename Sequence, typename T > 
	struct apply
	{

	};
};

template<>
struct push_front_impl< vector_tag<0> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector1<T> type;
	};
};

PUSH_FRONT_IMPL_DECL(1)
PUSH_FRONT_IMPL_DECL(2)
PUSH_FRONT_IMPL_DECL(3)
PUSH_FRONT_IMPL_DECL(4)
PUSH_FRONT_IMPL_DECL(5)
PUSH_FRONT_IMPL_DECL(6)
PUSH_FRONT_IMPL_DECL(7)
PUSH_FRONT_IMPL_DECL(8)

#define VECTOR_PUSH_BACK_DECL(dst,N,T,src) VECTOR_N_I_DECL(VECTOR_N_INC(dst,N),VECTOR_ITEM_IMPL(src, N),T)
#define PUSH_BACK_IMPL_DECL(N)\
	template<>\
	struct push_back_impl< vector_tag<N> >\
	{\
		template< typename Vector, typename T > struct apply\
		{\
			typedef VECTOR_PUSH_BACK_DECL(vector, N, T, Vector) type;\
		};\
	};

template< typename Tag = na >
struct push_back_impl
{
	template< typename Sequence, typename T > 
	struct apply
	{

	};
};

template<>
struct push_back_impl< vector_tag<0> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector1<T> type;
	};
};

PUSH_BACK_IMPL_DECL(1)
PUSH_BACK_IMPL_DECL(2)
PUSH_BACK_IMPL_DECL(3)
PUSH_BACK_IMPL_DECL(4)
PUSH_BACK_IMPL_DECL(5)
PUSH_BACK_IMPL_DECL(6)
PUSH_BACK_IMPL_DECL(7)
PUSH_BACK_IMPL_DECL(8)


#define VECTOR_N_I_DEC(vector,N)	MACRO_PP_CAT(vector, N)
#define VECTOR_N_DEC(vector,N)		VECTOR_N_I_DEC(vector, MACRO_NUMBER_DEC(N))

#define VECTOR_I_II_DECL(vector,...) vector<__VA_ARGS__>
#define VECTOR_I_I_DECL(vector,...) VECTOR_I_II_DECL(vector,__VA_ARGS__)

#define VECTOR_POP_FRONT_ITEM_MEMBER_I(vector, N) typename vector::item##N
#define VECTOR_POP_FRONT_ITEM_MEMBER(vector, N) VECTOR_POP_FRONT_ITEM_MEMBER_I(vector, N)

#define VECTOR_POP_FRONT_ITEM_MEMBER_GET_FIELD(r, data, i, field)\
	MACRO_PP_SEPARATOR_IF(i) VECTOR_POP_FRONT_ITEM_MEMBER(data, MACRO_NUMBER_INC(i))

#define VECTOR_POP_FRONT_ITEM_IMPL(vector, N) \
	MACRO_PP_SEQ_FOR_EACH_I(VECTOR_POP_FRONT_ITEM_MEMBER_GET_FIELD,vector,MACRO_PP_VARIADIC_TO_SEQ(MACRO_TARGLIST_DEF(N)))

#define VECTOR_POP_FRONT_DECL(dst,N,src) VECTOR_I_I_DECL(VECTOR_N_DEC(dst,N),VECTOR_POP_FRONT_ITEM_IMPL(src, MACRO_NUMBER_DEC(N)))

#define POP_FRONT_IMPL_DECL(N)\
	template<>\
	struct pop_front_impl< vector_tag<N> >\
	{\
		template< typename Vector > struct apply\
		{\
			typedef VECTOR_POP_FRONT_DECL(vector, N, Vector) type;\
		};\
	};

template< typename Tag = na >
struct pop_front_impl
{
	template< typename Sequence, typename T >
	struct apply
	{

	};
};

template<>
struct pop_front_impl< vector_tag<1> >
{
	template< typename Vector > struct apply
	{
		typedef vector0<> type;
	};
};

POP_FRONT_IMPL_DECL(2)
POP_FRONT_IMPL_DECL(3)
POP_FRONT_IMPL_DECL(4)
POP_FRONT_IMPL_DECL(5)
POP_FRONT_IMPL_DECL(6)
POP_FRONT_IMPL_DECL(7)
POP_FRONT_IMPL_DECL(8)



#define VECTOR_POP_BACK_DECL(dst,N,src) VECTOR_I_I_DECL(VECTOR_N_DEC(dst,N),VECTOR_ITEM_IMPL(src, MACRO_NUMBER_DEC(N)))
#define POP_BACK_IMPL_DECL(N)\
	template<>\
	struct pop_back_impl< vector_tag<N> >\
	{\
		template< typename Vector > struct apply\
		{\
			typedef VECTOR_POP_BACK_DECL(vector, N, Vector) type;\
		};\
	};

template< typename Tag = na >
struct pop_back_impl
{
	template< typename Sequence, typename T > 
	struct apply
	{

	};
};

template<>
struct pop_back_impl< vector_tag<1> >
{
	template< typename Vector > struct apply
	{
		typedef vector0<> type;
	};
};

POP_BACK_IMPL_DECL(2)
POP_BACK_IMPL_DECL(3)
POP_BACK_IMPL_DECL(4)
POP_BACK_IMPL_DECL(5)
POP_BACK_IMPL_DECL(6)
POP_BACK_IMPL_DECL(7)
POP_BACK_IMPL_DECL(8)



/*template<>
struct push_front_impl< vector_tag<1> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector2<T, typename Vector::item0> type;
	};
};

template<>
struct push_front_impl< vector_tag<2> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector3<T, typename Vector::item0, typename Vector::item1> type;
	};
};


template<>
struct push_front_impl< vector_tag<3> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector4<T, typename Vector::item0, typename Vector::item1, typename Vector::item2> type;
	};
};

template<>
struct push_front_impl< vector_tag<4> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector5<T, typename Vector::item0, typename Vector::item1, typename Vector::item2, typename Vector::item3> type;
	};
};

template<>
struct push_front_impl< vector_tag<5> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector6<T, typename Vector::item0, typename Vector::item1
			, typename Vector::item2, typename Vector::item3, typename Vector::item4> type;
	};
};

template<>
struct push_front_impl< vector_tag<6> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector7<T, typename Vector::item0, typename Vector::item1
			, typename Vector::item2, typename Vector::item3, typename Vector::item4
			, typename Vector::item5> type;
	};
};

template<>
struct push_front_impl< vector_tag<7> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector8<T, typename Vector::item0, typename Vector::item1
			, typename Vector::item2, typename Vector::item3, typename Vector::item4
			, typename Vector::item5, typename Vector::item6> type;
	};
};

template<>
struct push_front_impl< vector_tag<8> >
{
	template< typename Vector, typename T > struct apply
	{
		typedef vector9<T, typename Vector::item0, typename Vector::item1
			, typename Vector::item2, typename Vector::item3, typename Vector::item4
			, typename Vector::item5, typename Vector::item6, typename Vector::item7> type;
	};
};*/

template< typename Sequence = na, typename T = na>
struct push_front
	: push_front_impl< typename sequence_tag<Sequence>::type >
	::template apply< Sequence,T >
{
	
};

template< typename Sequence = na>
struct pop_front
	: pop_front_impl< typename sequence_tag<Sequence>::type >
	::template apply< Sequence >
{

};

template< typename Sequence = na, typename T = na>
struct push_back
	: push_back_impl< typename sequence_tag<Sequence>::type >
	::template apply< Sequence,T >
{

};

template< typename Sequence = na>
struct pop_back
	: pop_back_impl< typename sequence_tag<Sequence>::type >
	::template apply< Sequence >
{

};

}

#endif
