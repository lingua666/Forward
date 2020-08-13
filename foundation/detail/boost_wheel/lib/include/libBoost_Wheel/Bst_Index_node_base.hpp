#ifndef __BST_INDEX_NODE_BASE_20191021232104_1442199360_H__
#define __BST_INDEX_NODE_BASE_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace boost_wheel{

	namespace multi_index{

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

	}

}

#endif
