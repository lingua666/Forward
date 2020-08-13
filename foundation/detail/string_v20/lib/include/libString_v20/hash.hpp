
#ifndef __HASH_STRING_V20_20160418173307_1438860785_H__
#define __HASH_STRING_V20_20160418173307_1438860785_H__

	#if defined(_WIN32) || defined(_WIN64)

		#include <xhash>

		_STD_BEGIN

		template<>
		class hash<_string_v20_::string>
			: public unary_function<_string_v20_::string, size_t>
		{	// hash functor
		public:
			typedef _string_v20_::string _Kty;

			size_t operator()(const _Kty& _Keyval) const
			{	// hash _Keyval to size_t value by pseudorandomizing transform
				size_t _Val = 2166136261U;
				size_t _First = 0;
				size_t _Last = _Keyval.size();
				size_t _Stride = 1 + _Last / 10;

				for(; _First < _Last; _First += _Stride)
					_Val = 16777619U * _Val ^ (size_t)_Keyval[_First];
				return (_Val);
			}
		};

		_STD_END


		namespace stdext {

			template <class _InIt>
			inline size_t __Hash_value(_InIt _Begin, _InIt _End)
			{	// hash range of elements
				size_t _Val = 2166136261U;

				while (_Begin != _End)
					_Val = 16777619U * _Val ^ (size_t)*_Begin++;
				return (_Val);
			}

			inline size_t hash_value(const _string_v20_::string& _Str)
			{	// hash string to size_t value
				const _string_v20_::_Elem *_Ptr = _Str.c_str();

				return (__Hash_value(_Ptr, _Ptr + _Str.size()));
			}

		}

	#endif

#endif // __MODAFX_20160418173305_1438860785_H__
