#ifndef __BST_FIND_IF_20191021232104_1442199360_H__
#define __BST_FIND_IF_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libBoost_Wheel/Bst_Common.hpp>
#include <libBoost_Wheel/Bst_Vector_fwd.hpp>

namespace boost{

struct find_if_same
{
	template<typename IndexSpecifierMeta, typename Tag, typename SuperMeta>
	struct apply
	{
		typedef typename IndexSpecifierMeta::type IndexSpecifierMeta_type;
		typedef typename IndexSpecifierMeta_type::tag_type tag_type;
		typedef typename Tag::type	dstTag;

		typedef typename  eval_if_c<
			is_same<tag_type, dstTag>::value,
			IndexSpecifierMeta,
			SuperMeta
		>::type type;
	}; 
};

template< typename Sequence = na, typename Tag  = na, int I = 0 >
struct find_if_impl
{
	typedef typename sequence_tag<Sequence>::type sequence_tag_type;
	
	typedef typename  eval_if_c<
		sequence_size< sequence_tag_type >::value==I,
		tag<>,
		find_if_same::apply< at_c<Sequence,I>, Tag,
		find_if_impl< Sequence, Tag, I + 1 > 
		>
	>::type type;
};

template< typename Sequence = na, typename Tag = na >
struct find_if : find_if_impl< Sequence, Tag, 0 >
{
	 
};

}

#endif
