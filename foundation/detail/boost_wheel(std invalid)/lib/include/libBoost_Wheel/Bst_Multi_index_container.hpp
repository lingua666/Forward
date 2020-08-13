#ifndef __BST_MULTI_INDEX_CONTAINER_20191021232104_1442199360_H__
#define __BST_MULTI_INDEX_CONTAINER_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libBoost_Wheel/Bst_Macro.hpp>
#include <libBoost_Wheel/Bst_Member_fwd.hpp>
#include <libBoost_Wheel/Bst_Find_if.hpp>
#include <libBoost_Wheel/Bst_Nth_layer_fwd.hpp>
#include <libBoost_Wheel/Bst_Ordered_index.hpp>

namespace boost{

	template<typename Value = na, typename Allocator = na>
	struct ordered_node_type
	{
		typedef typename SmartPTR<Value> Value_sptr;
		typedef Value_sptr	type;

		static MemPool_type* GetPoolInstance( void )
		{
			static MemPool_type s_Pool( sizeof(Value) );
			return &s_Pool;
		}

		static Value_sptr Alloc( void )
		{
			MemPool_type* ptr = GetPoolInstance();
			return Value_sptr(ptr->AllocObj<Value>(), 
				function20_bind_Ex(&MemPool_type::FreeObj<Value>, ptr ));
		}

		static Value_sptr Alloc( const Value& Val )
		{
			MemPool_type* ptr = GetPoolInstance();
			return Value_sptr(ptr->AllocObj<Value>(Val), 
				function20_bind_Ex(&MemPool_type::FreeObj<Value>, ptr ));
		}
	};

	template<typename Value>
	struct ordered_node_type<Value*>
	{
		typedef typename SmartPTR<Value> Value_sptr;
		typedef Value_sptr		 type;

		static MemPool_type* GetPoolInstance( void )
		{
			static MemPool_type s_Pool( sizeof(Value) );
			return &s_Pool;
		}

		static Value_sptr Alloc( void )
		{
			MemPool_type* ptr = GetPoolInstance();
			return Value_sptr(ptr->AllocObj<Value>(), 
				function20_bind_Ex(&MemPool_type::FreeObj<Value>, ptr));
		}

		static Value_sptr Alloc( const Value& Val )
		{
			MemPool_type* ptr = GetPoolInstance();
			return Value_sptr(ptr->AllocObj<Value>(Val), 
				function20_bind_Ex(&MemPool_type::FreeObj<Value>, ptr ));
		}
	};

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct multi_index_base_type:nth_layer<0,Value,IndexSpecifierList,Allocator>
{
	
};

template<typename Value,typename IndexSpecifierList,typename Allocator = na>
class multi_index_container : public multi_index_base_type<Value,IndexSpecifierList,Allocator>::type
{
public:
	typedef typename multi_index_base_type<
		Value,IndexSpecifierList,Allocator>::type   super;
	typedef typename super::node_type				node_type;
	typedef typename super::node_type_sptr			node_type_sptr;
	typedef typename super::index_type_list         index_type_list;
	typedef typename super::iterator                iterator;
	typedef typename super::const_iterator          const_iterator;

public:
	explicit multi_index_container():
		node_count(0)
	{
		
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

	std::pair<iterator,bool> insert_(const Value& v)
	{
		node_type_sptr x=allocate_node();
		iterator res=super::insert_(v,x);
		if(!res)
		{
			return std::pair<iterator,bool>(res,false);
		}

		++node_count;
		return std::pair<iterator,bool>(res,true);
	}
	
	int erase_(const Value& v, const node_type_sptr& x)
	{
		if( super::erase_(v,x) > 0 )
			--node_count;
		return node_count;
	}

protected:
	node_type_sptr allocate_node()
	{
		return ordered_node_type<node_type>::Alloc();
	}

	/*void deallocate_node(node_type* x)
	{
		typedef typename node_allocator::pointer node_pointer;
		bfm_allocator::member.deallocate(static_cast<node_pointer>(x),1);
	}*/

private:
	std::size_t node_count;
};

}

#endif

