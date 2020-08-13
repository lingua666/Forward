
#ifndef __MACRO_VARIABLE_20160419111505_1438860785_H__
#define __MACRO_VARIABLE_20160419111505_1438860785_H__


# if defined(USE_BOOST_MACRO)
	#include <boost/preprocessor/seq/for_each_i.hpp>
	#include <boost/preprocessor/variadic/to_seq.hpp>
	#include <boost/preprocessor/seq/seq.hpp>

	# define BOOST_PP_COMMA_STRING()	","
	# define SEQ_FOR_EACH_I			BOOST_PP_SEQ_FOR_EACH_I
	# define VARIADIC_TO_SEQ		BOOST_PP_VARIADIC_TO_SEQ
	# define PP_CAT					BOOST_PP_CAT
	# define PP_COMMA_IF			BOOST_PP_COMMA_IF
	# define PP_SEQ_HEAD			BOOST_PP_SEQ_HEAD
	# define PP_COMMA_STRING(cond)\
			BOOST_PP_IF(cond, BOOST_PP_COMMA_STRING, BOOST_PP_EMPTY)()

# else
	#include "Macro_for.hpp"

	# define SEQ_FOR_EACH_I			MACRO_PP_SEQ_FOR_EACH_I
	# define VARIADIC_TO_SEQ		MACRO_PP_VARIADIC_TO_SEQ
	# define PP_CAT					MACRO_PP_CAT
	# define PP_IF					MACRO_PP_IF
	# define PP_EMPTY				MACRO_PP_EMPTY
	# define PP_COMMA_IF			MACRO_PP_COMMA_IF
	# define PP_SEQ_HEAD			MARCO_PP_SEQ_HEAD
	# define PP_COMMA_STRING(cond)	MACRO_PP_COMMA_STRING_IF(cond)

#endif

# define PP_COMMA_STRING_IF		PP_COMMA_STRING

# define INTEGER_DEFAULT			0

# define MACRO_DEFAULT_Int8		INTEGER_DEFAULT
# define MACRO_DEFAULT_UInt8	INTEGER_DEFAULT
# define MACRO_DEFAULT_Int16	INTEGER_DEFAULT
# define MACRO_DEFAULT_UInt16	INTEGER_DEFAULT
# define MACRO_DEFAULT_Int32	INTEGER_DEFAULT
# define MACRO_DEFAULT_UInt32	INTEGER_DEFAULT
# define MACRO_DEFAULT_Int64	INTEGER_DEFAULT
# define MACRO_DEFAULT_UInt64	INTEGER_DEFAULT
# define MACRO_DEFAULT_int		INTEGER_DEFAULT
# define MACRO_DEFAULT_short	INTEGER_DEFAULT
# define MACRO_DEFAULT_long		INTEGER_DEFAULT

# define MACRO_DEFAULT_pchar		NULL
# define MACRO_DEFAULT_char			''
# define MACRO_DEFAULT_string		""
# define MACRO_DEFAULT_const_char	""

# define DECLARE_VARIABLE_DEFAULT(r, data, i, field)\
		DECLARE_VARIABLE_NAME(field) = PP_CAT(MACRO_DEFAULT_,DECLARE_VARIABLE_TYPE(field));

# define DECLARE_VARIABLE_COPY(r, data, i, field)\
		DECLARE_VARIABLE_NAME(field) = data.DECLARE_VARIABLE_NAME(field);

# define DECLARE_VARIABLE_COPY_II(r, data, i, field)\
		self->DECLARE_VARIABLE_NAME(field) = DECLARE_VARIABLE_NAME(field);

# define DECLARE_VARIABLE_TYPE(field)\
		DECLARE_VARIABLE_GET_I(MACRO_PP_GET_0,(PP_VI field))

# define DECLARE_VARIABLE_TYPE_TOSTRING(field)\
		DECLARE_VARIABLE_TYPE_TOSTRING_I(DECLARE_VARIABLE_GET_I(MACRO_PP_GET_0,(PP_VI field)))

# define DECLARE_VARIABLE_TYPE_TOSTRING_I(type)\
		DECLARE_VARIABLE_TYPE_TOSTRING_II(type)

# define DECLARE_VARIABLE_TYPE_TOSTRING_II(type)	# type

# define DECLARE_VARIABLE_NAME(field)\
		DECLARE_VARIABLE_GET_I(MACRO_PP_GET_1,(PP_VI field))

# define DECLARE_VARIABLE_NAME_TOSTRING(field)\
		DECLARE_VARIABLE_NAME_TOSTRING_I(DECLARE_VARIABLE_GET_I(MACRO_PP_GET_1,(PP_VI field)))

# define DECLARE_VARIABLE_NAME_TOSTRING_I(name)\
		DECLARE_VARIABLE_NAME_TOSTRING_II(name)

# define DECLARE_VARIABLE_NAME_TOSTRING_II(name)	# name

# define DECLARE_VARIABLE_GET_I(t, f)\
		DECLARE_VARIABLE_GET_II(t, f)

# define DECLARE_VARIABLE_GET_II(t, f)\
		PP_CAT(t f,)

# define MAROC_DECLARE_VARIABLE(r, data, i, field)\
		PP_VI field;

# define MACRO_PP_GET_0(t, f)	t
# define MACRO_PP_GET_1(t, f)	f

# define MACRO_GET_FIELD(r, data, i, field)\
		PP_V field;

# define PP_V(x)		x
# define PP_VI(x)		x,

# define MACRO_STRING_TYPE_FIELD(r, data, i, field)\
		PP_COMMA_STRING_IF(i) MACRO_TPYE(PP_SEQ_HEAD(field))

# define MACRO_STRING_NAME_FIELD(r, data, i, field)\
		PP_COMMA_STRING_IF(i) DECLARE_VARIABLE_NAME_TOSTRING(field)

# define MACRO_TPYE(n)		MACRO_TPYE_I(n)
# define MACRO_TPYE_I(n)	MACRO_TPYE_II(n)
# define MACRO_TPYE_II(n)	PP_CAT(MACRO_VARIABLE_TYPE_,n)

# define MACRO_VARIABLE_TYPE_short		"%d"
# define MACRO_VARIABLE_TYPE_int		"%d"
# define MACRO_VARIABLE_TYPE_long		"%d"
# define MACRO_VARIABLE_TYPE_int64		"%lld"
# define MACRO_VARIABLE_TYPE_float		"%f"
# define MACRO_VARIABLE_TYPE_double		"%lf"
# define MACRO_VARIABLE_TYPE_char		"%s"
# define MACRO_SVARIABLE_TYPE_string	"%s"

# define MACRO_SQL_TPYE(n)		MACRO_SQL_TPYE_I(n)
# define MACRO_SQL_TPYE_I(n)	MACRO_SQL_TPYE_II(n)
# define MACRO_SQL_TPYE_II(n)	PP_CAT(MACRO_SQL_TYPE_,n)

# define MACRO_SQL_TYPE_short		"%d"
# define MACRO_SQL_TYPE_int			"%d"
# define MACRO_SQL_TYPE_long		"%d"
# define MACRO_SQL_TYPE_int64		"%lld"
# define MACRO_SQL_TYPE_float		"%f"
# define MACRO_SQL_TYPE_double		"%lf"
# define MACRO_SQL_TYPE_char		"'%s'"
# define MACRO_SQL_TYPE_string		"'%s'"

# define MACRO_SQL_TYPE_const_char	"'%s'"
# define MACRO_SQL_TYPE_const_int	"%d"

# define MACRO_VALUE_string(o)	o.c_str()
# define MACRO_VALUE_char(o)	o
# define MACRO_VALUE_short(o)	o
# define MACRO_VALUE_int(o)		o
# define MACRO_VALUE_long(o)	o
# define MACRO_VALUE_int64(o)	o
# define MACRO_VALUE_float(o)	o
# define MACRO_VALUE_double(o)	o

# define MACRO_VARIABLE_NAME_COLLECTION(r, data, i, field)\
		PP_COMMA_IF(i) MACRO_VARIABLE_NAME_COLLECTION_I(DECLARE_VARIABLE_NAME(field),DECLARE_VARIABLE_TYPE(field))

# define MACRO_VARIABLE_NAME_COLLECTION_I(name, type)\
		MACRO_VARIABLE_NAME_COLLECTION_II(name, PP_CAT(MACRO_VALUE_,type))

# define MACRO_VARIABLE_NAME_COLLECTION_II(name, value)\
		value(name)

# define MACRO_DECLARE_CLASS(m, args)\
		m args

#endif