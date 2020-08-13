#ifndef __BST_INDEX_BASE_20191021232104_1442199360_H__
#define __BST_INDEX_BASE_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libBoost_Wheel/Bst_Vector_fwd.hpp>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libMempool/MemPool.h>

namespace boost{

template<typename Value,typename IndexSpecifierList,typename Allocator>
class multi_index_container;

template <
	std::size_t size_
	, std::size_t alignment_ = std::size_t(-1)
>
struct aligned_storage
{
	typedef aligned_storage type;

	union data_t
	{
		char buf[size_];
	} data_;

	//void* address() const { return const_cast<aligned_storage_imp*>(this); }
};

template<typename Value>
struct pod_value_holder
{
	typename aligned_storage<
		sizeof(Value)
	>::type                      space;
};

template<typename Value,typename Allocator>
struct index_node_base:private pod_value_holder<Value>
{
  typedef index_node_base base_type; /* used for serialization purposes */
  typedef Value           value_type;
  typedef Allocator       allocator_type;

  value_type& value()
  {
    return *static_cast<value_type*>(
      static_cast<void*>(&this->space));
  }

  const value_type& value()const
  {
    return *static_cast<const value_type*>(
      static_cast<const void*>(&this->space));
  }
};


template<typename Value,typename IndexSpecifierList,typename Allocator>
class index_base
{
public:
	typedef index_node_base<Value,Allocator>    node_type;
	typedef SmartPTR<node_type>					node_type_sptr;
	typedef Value								value_param_type;

	typedef vector0<>			index_type_list;
	typedef vector0<>			iterator_type_list;
	typedef vector0<>			const_iterator_type_list;
	typedef tag< >				tag_type;

	typedef multi_index_container<
		Value,IndexSpecifierList,Allocator>       final_type;

public:
	explicit index_base(){}

protected:
	node_type_sptr insert_(const value_param_type& v, const node_type_sptr& x)
	{
		if( x )
			new (&x->value()) Value(v);
		return x;
	}

	int erase_(const Value& v, const node_type_sptr& x)
	{	
		(x->value()).~Value();
		return 1;
	}

protected:
	final_type&       final(){return *static_cast<final_type*>(this);}
	const final_type& final()const{return *static_cast<const final_type*>(this);}

	template<typename iterator>
	std::pair<iterator,bool> final_insert_(const value_param_type& x)
	{return final().insert_(x);}

	int final_erase_(const Value& v, const node_type_sptr& x){ return final().erase_(v,x); }
};

}

#endif
