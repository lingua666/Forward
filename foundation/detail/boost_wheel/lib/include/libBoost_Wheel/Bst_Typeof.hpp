#ifndef __BST_TYPEOF_20191021232104_1442199360_H__
#define __BST_TYPEOF_20191021232104_1442199360_H__

#include <libBoost_Wheel/Bst_Macro.hpp>

#if defined( PLATFORM_OS_FAMILY_WINDOWS )

namespace boost_wheel{

	template<int N> struct the_counter;

	template<typename T,int N = 5>
	struct encode_counter
	{
		__if_exists(the_counter<N + 256>)
		{
			static const unsigned count = encode_counter<T,N + 257>::count;
		}
		__if_not_exists(the_counter<N + 256>)
		{
			__if_exists(the_counter<N + 64>)
			{
				static const unsigned count = encode_counter<T,N + 65>::count;
			}
			__if_not_exists(the_counter<N + 64>)
			{
				__if_exists(the_counter<N + 16>)
				{
					static const unsigned count = encode_counter<T,N + 17>::count;
				}
				__if_not_exists(the_counter<N + 16>)
				{
					__if_exists(the_counter<N + 4>)
					{
						static const unsigned count = encode_counter<T,N + 5>::count;
					}
					__if_not_exists(the_counter<N + 4>)
					{
						__if_exists(the_counter<N>)
						{
							static const unsigned count = encode_counter<T,N + 1>::count;
						}
						__if_not_exists(the_counter<N>)
						{
							static const unsigned count = N;
							typedef the_counter<N> type;
						}
					}
				}
			}
		}
	};

	# define BOOST_TYPEOF_INDEX(T) (encode_counter<T>::count)

	struct msvc_extract_type_default_param {};

	template<typename ID, typename T = msvc_extract_type_default_param>
	struct msvc_extract_type;

	template<typename ID>
	struct msvc_extract_type<ID, msvc_extract_type_default_param> {
		template<bool>
		struct id2type_impl;

		typedef id2type_impl<true> id2type;
	};

	template<typename ID, typename T>
	struct msvc_extract_type : msvc_extract_type<ID,msvc_extract_type_default_param>
	{
		template<>
		struct id2type_impl<true>  //VC8.0 specific bugfeature
		{
			typedef T type;
		};
		template<bool>
		struct id2type_impl;

		typedef id2type_impl<true> id2type;
	};

	template<typename T, typename ID>
	struct msvc_register_type : msvc_extract_type<ID, T>
	{
	};

	template<int ID>
	struct msvc_typeid_wrapper {
		typedef typename msvc_extract_type<mpl::int_<ID> >::id2type id2type;
		typedef typename id2type::type type;
	};
	//Workaround for ETI-bug for VC6 and VC7
	template<>
	struct msvc_typeid_wrapper<1> {
		typedef msvc_typeid_wrapper<1> type;
	};
	//Workaround for ETI-bug for VC7.1
	template<>
	struct msvc_typeid_wrapper<4> {
		typedef msvc_typeid_wrapper<4> type;
	};

	//Tie it all together
	template<typename T>
	struct encode_type
	{
		static const unsigned value = BOOST_TYPEOF_INDEX(T);
		typedef typename msvc_register_type< T,mpl::int_<value> >::id2type type;
	};

	template<class T>
	struct sizer
	{
		typedef char(*type)[encode_type<T>::value];
	};

	template<typename T> 
	typename sizer<T>::type encode_start(T const&);

	template<typename T> 
	typename sizer<T>::type encode_start(T&); 

}

# define BOOST_WHEEL_TYPEOF(expr) \
	boost_wheel::msvc_typeid_wrapper<sizeof(*boost_wheel::encode_start(expr))>::type

#elif defined( PLATFORM_OS_FAMILY_UNIX )
	# define BOOST_WHEEL_TYPEOF(expr) typeof(expr)

#endif

#endif

