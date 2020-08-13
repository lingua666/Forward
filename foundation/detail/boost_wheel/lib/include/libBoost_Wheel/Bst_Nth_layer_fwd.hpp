#ifndef __BST_NTH_LAYER_20191021232104_1442199360_H__
#define __BST_NTH_LAYER_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libBoost_Wheel/Bst_Index_base.hpp>

namespace boost_wheel{

	namespace multi_index{

		template< typename T >
		struct msvc_never_true
		{
			enum { value = false };
		};

		template<typename IndexSpecifier>
		struct msvc_index_specifier
		{
			template<bool,typename T1 = na> struct fake_index_type:IndexSpecifier{};
			template<typename T1> struct fake_index_type<true,T1>
			{
				template<typename Super>
				struct node_class{};

				template<typename Super>
				struct index_class{};
			};

			template<typename Super>
			struct result_node_class:
				fake_index_type<msvc_never_true<IndexSpecifier>::value>::
				template node_class<Super>
			{
			};

			template<typename Super>
			struct result_index_class:
				fake_index_type<msvc_never_true<IndexSpecifier>::value>::
				template index_class<Super>
			{
			};
		};

		struct index_applier
		{
			template<typename IndexSpecifierMeta,typename SuperMeta>
			struct apply
			{
				typedef typename IndexSpecifierMeta::type            index_specifier;
				typedef typename index_specifier::
					template index_class<SuperMeta>::type type;
			}; 
		};

		template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
		struct nth_layer
		{
			typedef typename  eval_if_c<
				N==IndexSpecifierList::value,
				identity<index_base<Value,IndexSpecifierList,Allocator> >,
				index_applier::apply< at_c<IndexSpecifierList,N>, nth_layer<N+1,Value,IndexSpecifierList,Allocator> >
			>::type type;
		};

	}

}

#endif
