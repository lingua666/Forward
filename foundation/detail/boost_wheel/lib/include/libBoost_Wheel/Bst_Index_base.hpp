#ifndef __BST_INDEX_BASE_20191021232104_1442199360_H__
#define __BST_INDEX_BASE_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libBoost_Wheel/Bst_Vector_fwd.hpp>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libMempool/MemPool.h>
#include <libBoost_Wheel/Bst_Node_type.hpp>

namespace boost_wheel{

	namespace multi_index{

		template<typename Value,typename IndexSpecifierList,typename Allocator>
		class index_base
		{
		public:
			typedef index_node_base<Value,Allocator>    node_type;

			typedef Value								value_param_type;

			typedef vector0<>			index_type_list;
			typedef vector0<>			iterator_type_list;
			typedef vector0<>			const_iterator_type_list;
			typedef tag< >				tag_type;

			typedef multi_index_container<
				Value,IndexSpecifierList,Allocator>       final_type;

			typedef typename multi_index_node_type<
				Value,IndexSpecifierList,Allocator>::type final_node_type;

		public:
			explicit index_base(){}

		protected:
			node_type* insert_(value_param_type v,node_type* x)
			{
				if( x )
					new (&x->value()) value_param_type(v);
				return x;
			}

			node_type* insert_(value_param_type v,node_type*,node_type* x)
			{
				if( x )
					new (&x->value()) value_param_type(v);
				return x;
			}

			void erase_(node_type* x)
			{	
				(x->value()).~value_param_type();
			}

			void delete_node_(node_type* x)
			{
				(x->value()).~value_param_type();
			}

			void clear_(){}

			bool replace_(const value_param_type& v,node_type* x)
			{
				x->value()=v;
				return true;
			}

			bool modify_(node_type*){return true;}

			bool modify_rollback_(node_type*){return true;}

		protected:
			final_type&       final(){return *static_cast<final_type*>(this);}
			const final_type& final()const{return *static_cast<const final_type*>(this);}

			final_node_type* final_header()const{return final().header();}

			bool        final_empty_()const{return final().empty_();}
			std::size_t final_size_()const{return final().size_();}
			std::size_t final_max_size_()const{return final().max_size_();}

			std::pair<final_node_type*,bool> final_insert_(value_param_type x)
			{return final().insert_(x);}
			std::pair<final_node_type*,bool> final_insert_(
				value_param_type x,final_node_type* position)
			{return final().insert_(x,position);}

			void final_erase_(final_node_type* x){final().erase_(x);}

			void final_delete_node_(final_node_type* x){final().delete_node_(x);}

			void final_clear_(){final().clear_();}

			bool final_replace_(const value_param_type& k,final_node_type* x)
			{return final().replace_(k,x);}

			template<typename Modifier>
			bool final_modify_(Modifier& mod,final_node_type* x)
			{return final().modify_(mod,x);}

			template<typename Modifier,typename Rollback>
			bool final_modify_(Modifier& mod,Rollback& back,final_node_type* x)
			{return final().modify_(mod,back,x);}
		};
	}
}

#endif
