#ifndef __BST_MULTI_INDEX_CONTAINER_20191021232104_1442199360_H__
#define __BST_MULTI_INDEX_CONTAINER_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libBoost_Wheel/Bst_Macro.hpp>
#include <libBoost_Wheel/Bst_Member_fwd.hpp>
#include <libBoost_Wheel/Bst_Member_func_fwd.hpp>
#include <libBoost_Wheel/Bst_Find_if.hpp>
#include <libBoost_Wheel/Bst_Nth_layer_fwd.hpp>
#include <libBoost_Wheel/Bst_Ordered_index.hpp>
#include <libBoost_Wheel/Bst_Node_type.hpp>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#pragma warning(disable:4503)
#endif

namespace boost_wheel{

	namespace multi_index{

		template<typename NodeTypePtr,typename Final>
		struct header_holder:private noncopyable
		{
			header_holder():member(final().allocate_node()){}
			~header_holder(){final().deallocate_node(&*member);}

			NodeTypePtr* member;

		private:
			Final& final(){return *static_cast<Final*>(this);}
		};

		template<typename Value,typename IndexSpecifierList,typename Allocator>
		struct multi_index_base_type:nth_layer<0,Value,IndexSpecifierList,Allocator>
		{

		};

		template<typename Value,typename IndexSpecifierList,typename Allocator = na>
		class multi_index_container : public header_holder< typename multi_index_node_type<Value,IndexSpecifierList,Allocator>::type,
			multi_index_container<Value, IndexSpecifierList, Allocator> >,
			public multi_index_base_type<Value,IndexSpecifierList,Allocator>::type
		{
		public:
			template <typename,typename,typename> friend class  index_base;
			template <typename,typename>          friend struct header_holder;

		public:
			typedef typename multi_index_base_type<
				Value,IndexSpecifierList,Allocator>::type   super;
			typedef typename super::node_type				node_type;
			typedef typename super::index_type_list         index_type_list;
			typedef typename super::iterator                iterator;
			typedef typename super::const_iterator          const_iterator;
			typedef typename super::value_type              value_type;

			typedef header_holder< 
				typename multi_index_node_type<
				Value,IndexSpecifierList,Allocator>::type,
				multi_index_container>						bfm_header;

		protected:
			static MemPool_type* MultiIndex_GetPoolInstance( void )
			{
				static MemPool_type s_Pool( sizeof(node_type) );
				return &s_Pool;
			}

		public:
			multi_index_container():
			  node_count(0)
			  {

			  } 

			  ~multi_index_container()
			  {
				  clear();
			  }

			  template<int N>
			  struct nth_index
			  {
				  BOOST_STATIC_ASSERT(N>=0&&N<index_type_list::value);
				  typedef typename at_c<index_type_list,N>::type type;
			  };

			  template<int N>
			  typename nth_index<N>::type& get( void )
			  {
				  BOOST_STATIC_ASSERT(N>=0&&N<index_type_list::value);
				  return *this;
			  }

			  template<int N>
			  const typename nth_index<N>::type& get( void )const
			  {
				  BOOST_STATIC_ASSERT(N>=0&&N<index_type_list::value);
				  return *this;
			  }

			  template<typename Tag>
			  struct index
			  {
				  typedef typename  eval_if<
					  is_tag<Tag>::value,
					  find_if< index_type_list, Tag >,
					  find_if< index_type_list, tag<Tag> >
				  >::type type;
			  };

			  template<typename Tag>
			  typename index<Tag>::type& get( void )
			  {
				  return *this;
			  }

			  template<typename Tag>
			  const typename index<Tag>::type& get( void ) const
			  {
				  return *this;
			  }

			  std::size_t size()const
			  {
				  return size_();
			  }

			  void clear()
			  {
				  super::clear();
			  }

		protected:

			node_type* header()const
			{
				return &*bfm_header::member;
			}

			node_type* allocate_node()
			{
				return MultiIndex_GetPoolInstance()->template AllocObj<node_type>();
			}

			void deallocate_node(node_type* x)
			{
				MultiIndex_GetPoolInstance()->FreeObj(x);
			}

			bool empty_()const
			{
				return node_count==0;
			}

			std::size_t size_()const
			{
				return node_count;
			}

			std::size_t max_size_()const
			{
				return static_cast<std::size_t >(-1);
			}

			std::pair<node_type*,bool> insert_(const Value& v)
			{
				node_type* x=allocate_node();
				node_type* res=super::insert_(v,x);
				if(res==x)
				{
					++node_count;
					return std::pair<node_type*,bool>(res,true);
				}
				deallocate_node(x);
				return std::pair<node_type*,bool>(res,false);
			}

			std::pair<node_type*,bool> insert_(const Value& v,node_type* position)
			{
				node_type* x=allocate_node();
				node_type* res=super::insert_(v,position,x);
				if(res==x)
				{
					++node_count;
					return std::pair<node_type*,bool>(res,true);
				}
				deallocate_node(x);
				return std::pair<node_type*,bool>(res,false);
			}

			void erase_(node_type* x)
			{
				--node_count;
				super::erase_(x);
				deallocate_node(x);
			}

			void delete_node_(node_type* x)
			{
				super::delete_node_(x);
				deallocate_node(x);
			}

			void delete_all_nodes_()
			{
				super::delete_all_nodes_();
			}

			void clear_()
			{
				delete_all_nodes_();
				super::clear_();
				node_count=0;
			}

			bool replace_(const Value& k,node_type* x)
			{
				return super::replace_(k,x);
			}

			template<typename Modifier>
			bool modify_(Modifier& mod,node_type* x)
			{
				mod(const_cast<value_type&>(x->value()));

				try{
					if(!super::modify_(x)){
						deallocate_node(x);
						--node_count;
						return false;
					}
					else return true;
				}
				catch(...){
					deallocate_node(x);
					--node_count;
					throw;
				}
			}

			template<typename Modifier,typename Rollback>
			bool modify_(Modifier& mod,Rollback& back,node_type* x)
			{
				mod(const_cast<value_type&>(x->value()));

				bool b;
				try{
					b=super::modify_rollback_(x);
				}
				catch(...){
					try{
						back(const_cast<value_type&>(x->value()));
						throw;
					}
					catch(...){
						this->erase_(x);
						throw;
					}
				}

				try{
					if(!b){
						back(const_cast<value_type&>(x->value()));
						return false;
					}
					else return true;
				}
				catch(...){
					this->erase_(x);
					throw;
				}
			}

		private:
			std::size_t node_count;
		};

	}

}

#endif

