

#ifndef	__BST_NODE_TYPE_20191021232104_1442199360_H__
#define __BST_NODE_TYPE_20191021232104_1442199360_H__

#include <libBoost_Wheel/Bst_Index_node_base.hpp>

namespace boost_wheel{

	namespace multi_index{

		struct index_node_applier
		{
			template<typename IndexSpecifierMeta,typename SuperMeta>
			struct apply
			{
				typedef typename IndexSpecifierMeta::type            index_specifier;
				typedef typename index_specifier::
					template node_class<SuperMeta>::type type;
			}; 
		};

		template< typename IndexSpecifierMeta, typename SuperMeta >
		struct multi_index_node_type_impl_  
		{
			typedef typename SuperMeta::type            SuperMeta_type;
			typedef typename index_node_applier::apply<IndexSpecifierMeta, SuperMeta_type>::type type;
		};

		template< int N, typename Value, typename IndexSpecifierList, typename Allocator >
		struct multi_index_node_type_impl
		{
			typedef typename  eval_if_c<
				N==IndexSpecifierList::value,
				identity< index_node_base<Value,Allocator> >,
				multi_index_node_type_impl_< at_c<IndexSpecifierList,N>, multi_index_node_type_impl<N+1,Value,IndexSpecifierList,Allocator> >
			>::type type;
		};

		template<typename Value, typename IndexSpecifierList,typename Allocator = na>
		struct multi_index_node_type : multi_index_node_type_impl<0, Value, IndexSpecifierList, Allocator>
		{

		};

	}

} /* namespacValue,  */

#endif
