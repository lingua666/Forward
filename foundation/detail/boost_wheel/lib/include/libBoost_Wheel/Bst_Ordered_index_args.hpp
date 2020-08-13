#ifndef __BST_ORDERED_INDEX_ARGS_20191021232104_1442199360_H__
#define __BST_ORDERED_INDEX_ARGS_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libBoost_Wheel/Bst_Common.hpp>

namespace boost_wheel{

	namespace multi_index{

		template<typename KeyFromValue>
		struct index_args_default_compare
		{
			typedef std::less<typename KeyFromValue::result_type> type;
		};

		template<>
		struct index_args_default_compare<na>
		{
			typedef na type;
		};

		template<typename Arg1,typename Arg2,typename Arg3>
		struct ordered_index_args
		{
			typedef is_tag<Arg1> full_form;

			typedef typename if_<
				full_form::value,
				Arg1,
				tag< > >::type                                   tag_list_type;
			typedef typename if_<
				full_form::value,
				Arg2,
				Arg1>::type                                      key_from_value_type;
			typedef typename if_<
				full_form::value,
				Arg3,
				Arg2>::type                                      supplied_compare_type;
			typedef typename eval_if<
				is_na<supplied_compare_type>::value,
				index_args_default_compare<key_from_value_type>,
				identity<supplied_compare_type>
			>::type                                            compare_type;
		};

	}

}

#endif

