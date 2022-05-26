
#ifndef ___STRING_V30_20160418173307_1438860785_H__
#define ___STRING_V30_20160418173307_1438860785_H__

#include "share_count.hpp"
#include <libBase/modafx.h>

#if defined(_MSC_VER)
	typedef signed __int64         int64_tt;
	typedef unsigned __int64       uint64_tt;
#elif defined(__GNUC__)
	//
	// Unix/GCC
	//
	#include <errno.h>
	#if defined(__LP64__)
		typedef signed long        int64_tt;
		typedef unsigned long      uint64_tt;
	#else
		typedef signed long long   int64_tt;
		typedef unsigned long long uint64_tt;
	#endif
#endif

namespace	_string_v30_ {

	#define _MY_STRING_CONST_ITERATOR(ptr)		const_iterator(ptr)
	#define _MY_STRING_ITERATOR(ptr)			iterator(ptr)

	typedef char		_Elem;
	typedef UInt32		size_type;
	typedef Int32		difference_type;

	class _My_String_const_iterator
	{	// iterator for nonmutable string
	public:
		typedef _My_String_const_iterator _Myiter;

		_My_String_const_iterator()
		{	// construct with null pointer
			this->_Ptr = 0;
		}

		_My_String_const_iterator(const _Elem* _Parg)
		{	// construct with pointer _Parg
			this->_Ptr = _Parg;
		}

		const _Elem& operator*() const
		{	// return designated object
			return (*this->_Ptr);
		}

		const _Elem*  operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		_Myiter& operator++()
		{	// preincrement
			++this->_Ptr;
			return (*this);
		}

		_Myiter operator++(int)
		{	// postincrement
			_Myiter _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_Myiter& operator--()
		{	// predecrement
			--this->_Ptr;
			return (*this);
		}

		_Myiter operator--(int)
		{	// postdecrement
			_Myiter _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		_Myiter& operator+=(difference_type _Off)
		{	// increment by integer
			_Ptr += _Off;
			return (*this);
		}

		_Myiter operator+(difference_type _Off) const
		{	// return this + integer
			_Myiter _Tmp = *this;
			return (_Tmp += _Off);
		}

		_Myiter& operator-=(difference_type _Off)
		{	// decrement by integer
			return (*this += -_Off);
		}

		_Myiter operator-(difference_type _Off) const
		{	// return this - integer
			_Myiter _Tmp = *this;
			return (_Tmp -= _Off);
		}

		size_type operator-(const _Myiter& _Right) const
		{	// return difference of iterators
			return (this->_Ptr - _Right._Ptr);
		}

		const _Elem& operator[](size_type _Off) const
		{	// subscript
			return (*(*this + _Off));
		}

		bool operator==(const _Myiter& _Right) const
		{	// test for iterator equality
			return (this->_Ptr == _Right._Ptr);
		}

		bool operator!=(const _Myiter& _Right) const
		{	// test for iterator inequality
			return (!(*this == _Right));
		}

		bool operator<(const _Myiter& _Right) const
		{	// test if this < _Right
			return (this->_Ptr < _Right._Ptr);
		}

		bool operator>(const _Myiter& _Right) const
		{	// test if this > _Right
			return (_Right < *this);
		}

		bool operator<=(const _Myiter& _Right) const
		{	// test if this <= _Right
			return (!(_Right < *this));
		}

		bool operator>=(const _Myiter& _Right) const
		{	// test if this >= _Right
			return (!(*this < _Right));
		}

		const _Elem* _Ptr;	// pointer to element in string
	};

	class _My_String_iterator : public _My_String_const_iterator
	{	// iterator for mutable string
	public:
		typedef _My_String_iterator _Myiter;
		typedef _My_String_const_iterator _Mybase;

		_My_String_iterator()
		{	// construct with null string pointer
		}

		_My_String_iterator(const _Elem* _Parg)
			: _Mybase(_Parg)
		{	// construct with pointer _Parg
		}

		_Elem& operator*() const
		{	// return designated object
			return ((_Elem&)**(_Mybase *)this);
		}

		const _Elem* operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		_Myiter& operator++()
		{	// preincrement
			++*(_Mybase *)this;
			return (*this);
		}

		_Myiter operator++(int)
		{	// postincrement
			_Myiter _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_Myiter& operator--()
		{	// predecrement
			--*(_Mybase *)this;
			return (*this);
		}

		_Myiter operator--(int)
		{	// postdecrement
			_Myiter _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		_Myiter& operator+=(difference_type _Off)
		{	// increment by integer
			*(_Mybase *)this += _Off;
			return (*this);
		}

		_Myiter operator+(difference_type _Off) const
		{	// return this + integer
			_Myiter _Tmp = *this;
			return (_Tmp += _Off);
		}

		_Myiter& operator-=(difference_type _Off)
		{	// decrement by integer
			return (*this += -_Off);
		}

		_Myiter operator-(difference_type _Off) const
		{	// return this - integer
			_Myiter _Tmp = *this;
			return (_Tmp -= _Off);
		}

		size_type operator-(const _Mybase& _Right) const
		{	// return difference of iterators
			return ((_Mybase)*this - _Right);
		}

		_Elem& operator[](size_type _Off) const
		{	// subscript
			return (*(*this + _Off));
		}
	};

	template<typename Allocate>
	class string
	{
	public:
		typedef	string					self_type;
		typedef _Elem&					reference;
		typedef const _Elem&			const_reference;
		typedef shared_count<Allocate>	_Alty ;

		typedef _My_String_iterator			iterator;
		typedef _My_String_const_iterator	const_iterator;

	public:
		static size_type FindData(const _Elem *_Myptr, size_type _Mysize, const _Elem *_Ptr,
			size_type _Off, size_type _Count) 
		{
			if (_Count == 0 && _Off <= _Mysize)
				return (_Off);	// null string always matches (if inside string)

			size_type _Nm;
			if (_Off < _Mysize && _Count <= (_Nm = _Mysize - _Off))
			{	// room for match, look for it
				const _Elem *_Uptr, *_Vptr;
				for (_Nm -= _Count - 1, _Vptr = _Myptr + _Off;
					(_Uptr = (const _Elem *)memchr(_Vptr, *_Ptr, _Nm)) != 0;
					_Nm -= _Uptr - _Vptr + 1, _Vptr = _Uptr + 1)
					if (memcmp(_Uptr, _Ptr, _Count) == 0)
						return (_Uptr - _Myptr);	// found a match
			}

			return (-1);	// no match
		}

		static size_type rFindData(const _Elem *_Myptr, size_type _Mysize, const _Elem *_Ptr,
			size_type _Off, size_type _Count) 
		{	// look for [_Ptr, _Ptr + _Count) beginning before _Off

			if (_Count == 0)
				return (_Off < _Mysize ? _Off
					: _Mysize);	// null always matches

			if (_Count <= _Mysize)
			{	// room for match, look for it
				const _Elem *_Head = _Myptr;
				const _Elem *_Uptr = _Head +
					(_Off < _Mysize - _Count ? _Off
						: _Mysize - _Count);

				for (; ; --_Uptr)
					if (*_Uptr == *_Ptr
						&& memcmp(_Uptr, _Ptr, _Count) == 0)
						return (_Uptr - _Head);	// found a match
					else if (_Uptr == _Head)
						break;	// at beginning, no more chance for match
			}

			return (-1);	// no match
		}

		static int64_tt StrHexToInt( const string& s )
		{
			int64_tt iValue = 0, t = 1;
			for( int i = s.size() - 1; i >= 0; i -- )
			{
				if( (s[i] >= '0') && (s[i] <= '9') )
				{
					iValue += (s[i] - '0') * t;
				}
				else if( (s[i] >= 'A') && (s[i] <= 'F') )
				{
					iValue += (s[i] - 'A' + 10) * t;
				}
				else if( (s[i] >= 'a') && (s[i] <= 'f') )
				{
					iValue += (s[i] - 'a' + 10) * t;
				}
				else
					return iValue;

				t *= 16;
			}

			return iValue;
		}

		static string	HexToStr( const char* szHex, size_type Size )
		{
			string s;
			s.reserve((Size << 1) + 1);

			unsigned char High = 0;
			unsigned char Low = 0;
			for( int i = 0 ; i < Size; i ++ )
			{
				High = (szHex[i] & 0xF0) >> 4;
				Low = szHex[i] & 0x0F;

				if(High < 10)
					s.append(1, '0' + High);
				else
					s.append(1, 'A' + (High - 10) );
				
				if(Low < 10)
					s.append(1, '0' + Low);
				else
					s.append(1, 'A' + (Low - 10));
			}

			return s;
		}

		static string	HexToStr( const string& s )
		{
			return HexToStr(s.c_str(), s.size());
		}

		static string	StrToHex( const char* szData, size_type Size )
		{
			string s;
			s.reserve(Size + 1);

			int i = 0;
			unsigned char c = 0;
			if( Size - ((Size >> 1) << 1) > 0 )
			{
				if( szData[0] >= '0' && szData[0] <= '9' )
					c = szData[0] - '0';
				else if( szData[0] >= 'A' && szData[0] <= 'F' )
					c = szData[0] - 'A' + 10;
				else if( szData[0] >= 'a' && szData[0] <= 'f' )
					c = szData[0] - 'a' + 10;

				s.append(1, c);
				i = 1;
			}

			for(; i < Size;)
			{
				if( szData[i] >= '0' && szData[i] <= '9' )
					c = (szData[i] - '0') << 4;
				else if( szData[i] >= 'A' && szData[i] <= 'F' )
					c = (szData[i] - 'A' + 10) << 4;
				else if( szData[i] >= 'a' && szData[i] <= 'f' )
					c = (szData[i] - 'a' + 10) << 4;

				if( szData[i + 1] >= '0' && szData[i + 1] <= '9' )
					c += szData[i + 1] - '0';
				else if( szData[i + 1] >= 'A' && szData[i + 1] <= 'F' )
					c += szData[i + 1] - 'A' + 10;
				else if( szData[i + 1] >= 'a' && szData[i + 1] <= 'f' )
					c += szData[i + 1] - 'a' + 10;

				s.append(1, c);

				i += 2;
			}

			return s;
		}

		static string	StrToHex( const string& s )
		{
			return StrToHex(s.c_str(), s.size());
		}

		static string	NumberToStr( int64_tt iValue )
		{
			char szBuf[30] = {0};
			sprintf(szBuf, "%lld", iValue);
			return reinterpret_cast<char*>(szBuf);
		}

		static string	FloatToStr( double dValue, unsigned short uPrecision = 0)
		{
			char szBuf[50] = { 0 };
			if (uPrecision == 0)
				sprintf(szBuf, "%lf", dValue);
			else
			{
				string s = "%.";
				s += NumberToStr(uPrecision);
				s += "lf";
				sprintf(szBuf, s.c_str(), dValue);
			}
			return reinterpret_cast<char*>(szBuf);
		}

		static int64_tt	StringToInt64( const string& s )
		{
			int64_tt Value;
			sscanf(s.c_str(), "%lld", &Value);
			return Value;
		}

		static int	StringToInt( const string& s )
		{
			return atoi(s.c_str());
		}

		static long	StringToLong( const string& s )
		{
			return atol(s.c_str());
		}

		static float StringToFloat( const string& s )
		{
			return atof(s.c_str());
		}

		static int	StringToInt( const char* pStr )
		{
			return atoi(pStr);
		}

		static long	StringToLong( const char* pStr )
		{
			return atol(pStr);
		}

		static float StringToFloat( const char* pStr )
		{
			return atof(pStr);
		}

		static int stringCompareIgnoreCase( const string& lhs, const string& rhs )
		{
			//转换小写比较
			string tmlhs = string(lhs.tolowers().c_str()).strim();
			string tmrhs = string(rhs.tolowers().c_str()).strim();
			return tmlhs.compare(tmrhs);
		}

		static string lFixation( const string& s, int iLen, char c )
		{
			int Num = iLen - s.size();
			if( Num > 0 )
			{
				string New;
				New.append(Num, c);
				New.append(s);
				return New;
			}
			
			return s;
		}

		static string rFixation(const string& s, int iLen, char c)
		{
			int Num = iLen - s.size();
			if (Num > 0)
			{
				string New;
				New.append(s);
				New.append(Num, c);
				return New;
			}

			return s;
		}

	private:
		_Elem			_Buf[STRING_OBJ_SIZE_V30 + STRING_BUF_SIZE + 1];
		size_type		_Mysize;
		size_type		_Myres;
		_Alty			_Alval;	// allocator object for strings

	public:
		string( void )
			: _Mysize(0)
			, _Myres(STRING_BUF_SIZE)
			, _Alval(_Buf, STRING_BUF_SIZE)
		{
			_Alval.get()[0] = 0;
		}

		string( const self_type& t1 )
			: _Mysize(t1._Mysize)
			, _Myres(t1._Myres)
			, _Alval(_Buf, STRING_BUF_SIZE)
		{
			_Alval = t1._Alval;
			_Myptr()[_Mysize] = 0;
		}

		string( const _Elem* _Ptr )
			: _Mysize(0)
			, _Myres(0)
		{
			assign(_Ptr, strlen(_Ptr), true);
		}

		string( const _Elem* _Ptr, size_type _Count )
			: _Mysize(0)
			, _Myres(0)
		{
			assign(_Ptr, _Count, true);
		}

		string( size_type _Count, _Elem _Ch )
			: _Mysize(0)
			, _Myres(0)
		{
			assign(_Count, _Ch, true);
		}

		reference operator[]( size_type _Off )
		{
			if(_Alval.use_count() > 1)
				Clone(this->_Myres);

			return _Myptr()[_Off];
		}

		const_reference operator[]( size_type _Off ) const
		{
			return _Myptr()[_Off];
		}

		string& operator=( const string& _Right )
		{
			if( !_Inside(_Right._Myptr()) )
			{
				_Myres = _Right._Myres;
				if( _Right._Myres > STRING_BUF_SIZE )
				{
					_Mysize = _Right._Mysize;
					_Alval = _Right._Alval;
					_Myptr()[_Mysize] = 0;
				}
				else
				{
					std::memcpy(_GetDataBuf(), _Right._Myptr(), _Right._Mysize);
					_Alty Aty = _Alty(_Buf, STRING_BUF_SIZE);
					_Alval.swap( Aty );
					_Mysize = 0;
					_Eos(_Right._Mysize);
				}
			}

			return *this;
		}

#if defined(_SUPPORTED_CXX11_)
		string& operator=( string&& _Right  )
		{
			if (!_Inside(_Right._Myptr()))
			{
				_Myres = _Right._Myres;
				if (_Right._Myres > STRING_BUF_SIZE)
				{
					_Mysize = _Right._Mysize;
					_Alval.swap(reinterpret_cast<string&&>(_Right)._Alval);
					_Myptr()[_Mysize] = 0;
				}
				else
				{
					std::memcpy(_GetDataBuf(), _Right._Myptr(), _Right._Mysize);
					_Alty Aty = _Alty(_Buf, STRING_BUF_SIZE);
					_Alval.swap(Aty);
					_Mysize = 0;
					_Eos(_Right._Mysize);
				}
			}

			return *this;
		}
#endif

		string& operator=( const _Elem* _Ptr )
		{	
			int iLen = strlen(_Ptr);
			if( _Alval.use_count() <= 1 && iLen < _Myres )
			{
				_Mysize = 0;
				std::memcpy(_Myptr(), _Ptr, iLen);
				_Eos( iLen );
				return *this;
			}

			assign(_Ptr, iLen, true);

			return *this;
		}

		string& operator=( _Elem _Ch )
		{	
			if(_Alval.use_count() <= 1)
			{
				_Mysize = 0;
				assign(1, _Ch);
			}
			else
				assign(1, _Ch, true);
			return *this;
		}

		string& operator+=( const string& _Right )
		{	
			append(_Right);
			return *this;
		}

		string& operator+=( const _Elem* _Ptr )
		{	
			append(_Ptr);
			return *this;
		}

		string& operator+= ( _Elem _Ch )
		{	
			append(1, _Ch);
			return *this;
		}

		string& append( const string& _Right )
		{	
			append_n(_Right._Myptr(), _Right.size());
			return *this;
		}

		string& append( const _Elem* _Ptr, size_type _Count )
		{
			append_n(_Ptr, _Count);
			return *this;
		}

		string& append( const _Elem* _Ptr )
		{
			append_n(_Ptr, strlen(_Ptr));
			return *this;
		}

		string& append( size_type _Count, _Elem _Ch )
		{
			append_n(_Count, _Ch);
			return *this;
		}

		string& append_n( const _Elem* _Ptr, size_type _Count )
		{
			if( _Alval.use_count() <= 1 )
			{
				std::memcpy( _Grow(_Count), _Ptr, _Count );
				_Eos( _Count );
			}
			else
			{
				Clone( __max(this->_Myres, this->_Mysize + _Count) + STRING_RESERVED_BUF_SIZE );
				std::memcpy( &_Myptr()[this->_Mysize], _Ptr, _Count );
				_Eos( _Count );
			}
			return *this;
		}

		string& append_n( size_type _Count, _Elem _Ch )
		{
			if(_Alval.use_count() <= 1)
			{
				std::memset(_Grow(_Count), _Ch, _Count);
				_Eos( _Count );
			}
			else
			{
				Clone( __max(this->_Myres, this->_Mysize + _Count) + STRING_RESERVED_BUF_SIZE );
				std::memset( &_Myptr()[this->_Mysize], _Ch, _Count );
				_Eos( _Count );
			}
			return *this;
		}

		string& assign( const string& _Right )
		{
			if( !_Inside(_Right._Myptr()) )
			{
				std::memcpy( _Grow(_Right.size()), _Right._Myptr(), _Right.size() );
				_Eos( _Right.size() );
			}
			return *this;
		}

		string& assign( const _Elem* _Ptr, size_type _Count )
		{
			if( !_Inside(_Ptr) )
			{
				std::memcpy( _Grow(_Count), _Ptr, _Count );
				_Eos( _Count );
			}

			return *this;
		}

		string& assign( const _Elem* _Ptr, size_type _Count, bool _Trim )
		{
			if( !_Inside(_Ptr) )
			{
				std::memcpy( _Grow(_Count, _Trim), _Ptr, _Count );
				_Eos( _Count );
			}

			return *this;
		}

		string& assign( size_type _Count, _Elem _Ch )
		{
			std::memset(_Grow(_Count), _Ch, _Count);
			_Eos( _Count );
			return *this;
		}

		string& assign( size_type _Count, _Elem _Ch, bool _Trim )
		{
			std::memset(_Grow(_Count, _Trim), _Ch, _Count);
			_Eos( _Count );
			return *this;
		}

		void clear( void )
		{
			erase();
		}

		_Elem *_Myptr()
		{
			return _Alval.get();
		}

		_Elem *_Myptr() const
		{
			return _Alval.get();
		}

		const _Elem* c_str() const
		{	
			return _Alval.get();
		}

		const _Elem* data( void ) const
		{
			return _Alval.get();
		}

		size_type length( void ) const
		{
			return _Mysize;
		}

		size_type size( void ) const
		{
			return _Mysize;
		}

		void resize( size_type _Newsize )
		{
			resize(_Newsize,'\0');
		}

		void resize( size_type _Newsize, _Elem _Ch )
		{
			if( _Newsize <= _Mysize )
				erase(0, _Newsize == 0 ? _Mysize : _Mysize - _Newsize);
			else
				append(_Newsize - _Mysize, _Ch);
		}

		bool update_size( size_type _Newsize )
		{
			if( _Newsize < _Myres )
			{
				_Mysize = 0;
				_Eos(_Newsize);
			}
			
			return false;
		}

		bool empty( void ) const
		{
			return _Mysize == 0;
		}

		void swap( string& _Right )
		{
			size_type tmp = 0;
			if( _Myres > STRING_BUF_SIZE ||  _Right._Myres > STRING_BUF_SIZE )
			{
				_Alval.swap(_Right._Alval); 
				if( _Myres <= STRING_BUF_SIZE )
				{
					std::memcpy(_Right._GetDataBuf(), _GetDataBuf(), _Mysize + 1);
					_Alty Aty = _Alty(_Right._Buf, STRING_BUF_SIZE);
					_Right._Alval.swap( Aty );
				}

				if( _Right._Myres <= STRING_BUF_SIZE )
				{
					std::memcpy(_GetDataBuf(), _Right._GetDataBuf(), _Right._Mysize + 1);
					_Alty Aty = _Alty(_Buf, STRING_BUF_SIZE);
					_Alval.swap( Aty );
				}
			}
			else
			{
				char buf[STRING_BUF_SIZE + 1] = {0};
				std::memcpy(buf, _Right._GetDataBuf(), _Right._Mysize + 1);
				std::memcpy(_Right._GetDataBuf(), _GetDataBuf(), _Mysize + 1);
				std::memcpy(_GetDataBuf(), buf, _Right._Mysize + 1);
			}

			tmp = _Right._Myres;
			_Right._Myres = _Myres;
			_Myres = tmp;
			tmp = _Right._Mysize;
			_Right._Mysize = _Mysize;
			_Mysize = tmp;
		}

#if defined(_SUPPORTED_CXX11_)
		void swap( string&& _Right )
		{
			//reinterpret_cast<string&&>(_Right)
			if( _Right._Myres > STRING_BUF_SIZE )
			{
				_Alval.swap(_Right._Alval); 
			}
			else
			{
				std::memcpy(_GetDataBuf(), _Right._GetDataBuf(), _Right._Mysize + 1);
				if( _Myres > STRING_BUF_SIZE )
				{
					_Alty Aty = _Alty(_Buf, STRING_BUF_SIZE);
					_Alval.swap( Aty );
				}
			}

			_Myres = _Right._Myres;
			_Mysize = _Right._Mysize;
		}
#endif

		int compare( const string& _Right ) const
		{	
			return compare(0, _Mysize, _Right._Myptr(), _Right.size());
		}

		int compare(const _Elem* _Ptr) const
		{
			return compare(0, _Mysize, _Ptr, strlen(_Ptr));
		}

		int compare(size_type _Off,
			size_type _N0, const _Elem* _Ptr, size_type _Count) const
		{
			if( _N0 == _Count )
				return memcmp(&(_Myptr()[_Off]), _Ptr, __min(_N0, _Count));

			return ( _N0 < _Count ? -1 : 1);
		}

		void reserve( size_type _Newcap )
		{	
			if ( this->_Myres < _Newcap )
			{
				if( _Grow(_Newcap, true, 0) )
				{
					_Eos(0);
				}
			}
		}

		size_type find(const _Elem *_Ptr, size_type _Off = 0) const
		{	// look for [_Ptr, <null>) beginnng at or after _Off
			return find(_Ptr, _Off, strlen(_Ptr));
		}

		size_type find(const string& _Right, size_type _Off = 0) const
		{
			return find(_Right._Myptr(), _Off, _Right.size());
		}

		size_type find(_Elem _Ch, size_type _Off = 0) const
		{
			return find((const _Elem *)&_Ch, _Off, 1);
		}

		size_type find(const _Elem *_Ptr,
			size_type _Off, size_type _Count) const
		{
			if (_Count == 0 && _Off <= this->_Mysize)
				return (_Off);	// null string always matches (if inside string)

			size_type _Nm;
			if (_Off < this->_Mysize && _Count <= (_Nm = this->_Mysize - _Off))
			{	// room for match, look for it
				const _Elem *_Uptr, *_Vptr;
				for (_Nm -= _Count - 1, _Vptr = _Myptr() + _Off;
					(_Uptr = (const _Elem *)memchr(_Vptr, *_Ptr, _Nm)) != 0;
					_Nm -= _Uptr - _Vptr + 1, _Vptr = _Uptr + 1)
					if (memcmp(_Uptr, _Ptr, _Count) == 0)
						return (_Uptr - _Myptr());	// found a match
			}

			return (-1);	// no match
		}

		size_type rfind(const _Elem *_Ptr, size_type _Off = -1) const
		{	// look for [_Ptr, <null>) beginning before _Off
			return rfind(_Ptr, _Off, strlen(_Ptr));
		}

		size_type rfind(const string& _Right, size_type _Off = -1) const
		{	// look for _Right beginning before _Off
			return rfind(_Right._Myptr(), _Off, _Right.size());
		}

		size_type rfind(_Elem _Ch, size_type _Off = -1) const
		{	// look for _Ch before _Off
			return rfind((const _Elem *)&_Ch, _Off, 1);
		}

		size_type rfind(const _Elem *_Ptr,
			size_type _Off, size_type _Count) const
		{	// look for [_Ptr, _Ptr + _Count) beginning before _Off

			if (_Count == 0)
				return (_Off < this->_Mysize ? _Off
				: this->_Mysize);	// null always matches

			if (_Count <= this->_Mysize)
			{	// room for match, look for it
				const _Elem *_Head = _Myptr();
				const _Elem *_Uptr = _Head +
					(_Off < this->_Mysize - _Count ? _Off
					: this->_Mysize - _Count);

				for (; ; --_Uptr)
					if (*_Uptr == *_Ptr
						&& memcmp(_Uptr, _Ptr, _Count) == 0)
						return (_Uptr - _Head);	// found a match
					else if (_Uptr == _Head)
						break;	// at beginning, no more chance for match
			}

			return (-1);	// no match
		}

		string& replace(size_type _Off, size_type _N0, const string& _Right)
		{	// replace [_Off, _Off + _N0) with _Right
			return (replace(_Off, _N0, _Right._Myptr(), 0, _Right.size()));
		}

		string& replace(size_type _Off, size_type _N0, const _Elem *_Ptr)
		{	// replace [_Off, _Off + _N0) with [_Ptr, <null>)
			return (replace(_Off, _N0, _Ptr, 0, strlen(_Ptr) ));
		}

		string& replace(size_type _Off,
			size_type _N0, size_type _Count, _Elem _Ch)
		{	// replace [_Off, _Off + _N0) with _Count * _Ch
			return (replace(_Off, _N0, (const _Elem*)&_Ch, 0, 1 ));
		}

		string& replace(size_type _Off,
			size_type _N0, const _Elem* _Right, size_type _Roff, size_type _Count)
		{	
			if ( _Inside(_Right) || this->_Mysize < _Off /*|| _Count < _Roff ||
				!_Right || _Myptr() == _Right*/ )
				return *this;

			_N0 = __min(this->_Mysize - _Off, _N0);

			size_type _Newsize = this->_Mysize + _Count - _N0;
			size_type _Nm = size() - _N0 - _Off;	// length of kept tail

			if (this->_Myres < _Newsize)
			{
				_Alty _tmp;
				_tmp.swap(_Alval);
				_Grow(_Newsize, true);
				_Elem* _Uptr = _Myptr();

				if( _Off > 0 )
					std::memcpy(_Uptr, _tmp.get(), _Off);	// fill hole

				std::memcpy(_Uptr + _Off, _Right + _Roff, _Count);	// fill hole

				if( _Nm > 0 )
					std::memcpy(_Uptr + _Off + _Count, _tmp.get() + _Off + _N0, _Nm);	// fill hole

				_Eos(_Newsize);
			}
			else
			{
				if(_Alval.use_count() > 1)
					Clone(this->_Myres);

				_Elem* _Uptr = _Myptr();

				// no overlap, just move down and copy in new stuff
				if( _Off > 0 )
					std::memmove(_Uptr + _Off + _Count, _Uptr + _Off + _N0, _Nm);	// fill hole

				if(_Nm > 0)
					std::memmove(_Uptr + _Off + _Count, _Uptr + _Off + _N0, this->_Mysize - (_Off + _N0));	// fill hole

				std::memcpy(_Uptr + _Off, _Right + _Roff, _Count);	// fill hole
				_Mysize = 0;
				_Eos(_Newsize);
			}
			return (*this);
		}

		size_type find_first_of(const string& _Right,
			size_type _Off = 0) const
		{	// look for one of _Right at or after _Off
			return ( find_first_of(_Right._Myptr(), _Off, _Right.size()) );
		}

		size_type find_first_of(const _Elem *_Ptr, size_type _Off = 0) const
		{	// look for one of [_Ptr, <null>) at or after _Off
			return ( find_first_of(_Ptr, _Off, strlen(_Ptr)) );
		}

		size_type find_first_of(_Elem _Ch, size_type _Off = 0) const
		{	// look for _Ch at or after _Off
			return ( find((const _Elem *)&_Ch, _Off, 1) );
		}

		size_type find_first_of(const _Elem *_Ptr,
			size_type _Off, size_type _Count) const
		{	// look for one of [_Ptr, _Ptr + _Count) at or after _Off

			if (0 < _Count && _Off < this->_Mysize)
			{	// room for match, look for it
				const _Elem *const _Vptr = _Myptr() + this->_Mysize;
				for (const _Elem *_Uptr = _Myptr() + _Off; _Uptr < _Vptr; ++_Uptr)
					if (std::memchr(_Ptr, *_Uptr, _Count) != 0)
						return (_Uptr - _Myptr());	// found a match
			}

			return (-1);	// no match
		}

		size_type find_last_of(const string& _Right,
			size_type _Off = -1) const
		{	// look for one of _Right before _Off
			return (find_last_of(_Right._Myptr(), _Off, _Right.size()));
		}

		size_type find_last_of(const _Elem *_Ptr,
			size_type _Off, size_type _Count) const
		{	// look for one of [_Ptr, _Ptr + _Count) before _Off

			if (0 < _Count && 0 < this->_Mysize)
			{	// worth searching, do it
				const _Elem *_Uptr = _Myptr()
					+ (_Off < this->_Mysize ? _Off : this->_Mysize - 1);
				for (; ; --_Uptr)
					if (std::memchr(_Ptr, *_Uptr, _Count) != 0)
						return (_Uptr - _Myptr());	// found a match
					else if (_Uptr == _Myptr())
						break;	// at beginning, no more chance for match
			}

			return (-1);	// no match
		}

		size_type find_last_of(const _Elem *_Ptr,
			size_type _Off = -1) const
		{	// look for one of [_Ptr, <null>) before _Off
			return (find_last_of(_Ptr, _Off, strlen(_Ptr)));
		}

		size_type find_last_of(_Elem _Ch, size_type _Off = -1) const
		{	// look for _Ch before _Off
			return (rfind((const _Elem *)&_Ch, _Off, 1));
		}

		size_type find_first_not_of(const string& _Right,
			size_type _Off = 0) const
		{	// look for none of _Right at or after _Off
			return (find_first_not_of(_Right._Myptr(), _Off,
				_Right.size()));
		}

		size_type find_first_not_of(const _Elem *_Ptr,
			size_type _Off = 0) const
		{	// look for one of [_Ptr, <null>) at or after _Off
			return (find_first_not_of( _Ptr, _Off, __max(strlen(_Ptr), 1) ));
		}

		size_type find_first_not_of(_Elem _Ch, size_type _Off = 0) const
		{	// look for non _Ch at or after _Off
			return (find_first_not_of((const _Elem *)&_Ch, _Off, 1));
		}

		size_type find_first_not_of(const _Elem *_Ptr,
			size_type _Off, size_type _Count) const
		{	// look for none of [_Ptr, _Ptr + _Count) at or after _Off
			if (_Off < this->_Mysize)
			{	// room for match, look for it
				const _Elem *const _Vptr = _Myptr() + this->_Mysize;
				for (const _Elem *_Uptr = _Myptr() + _Off; _Uptr < _Vptr; ++_Uptr)
					if (std::memchr(_Ptr, *_Uptr, _Count) == 0)
						return (_Uptr - _Myptr());
			}
			return (-1);
		}

		size_type find_last_not_of(const string& _Right,
			size_type _Off = -1) const
		{	// look for none of _Right before _Off
			return (find_last_not_of(_Right._Myptr(), _Off, _Right.size()));
		}

		size_type find_last_not_of(const _Elem *_Ptr,
			size_type _Off = -1) const
		{	// look for none of [_Ptr, <null>) before _Off
			return (find_last_not_of( _Ptr, _Off, __max( strlen(_Ptr), 1 ) ));
		}

		size_type find_last_not_of(_Elem _Ch, size_type _Off = -1) const
		{	// look for non _Ch before _Off
			return (find_last_not_of((const _Elem *)&_Ch, _Off, 1));
		}

		size_type find_last_not_of(const _Elem *_Ptr,
			size_type _Off, size_type _Count) const
		{	// look for none of [_Ptr, _Ptr + _Count) before _Off
			if (0 < this->_Mysize)
			{	// worth searching, do it
				const _Elem *_Uptr = _Myptr()
					+ (_Off < this->_Mysize ? _Off : this->_Mysize - 1);
				for (; ; --_Uptr)
					if (std::memchr(_Ptr, *_Uptr, _Count) == 0)
						return (_Uptr - _Myptr());
					else if (_Uptr == _Myptr())
						break;
			}
			return (-1);
		}

		string substr(size_type _Off = 0, size_type _Count = -1) const
		{	// return [_Off, _Off + _Count) as new string
			return (string(&(_Myptr()[_Off]),  _Count == -1 ? this->_Mysize - _Off : _Count));
		}

		string& insert(size_type _Off, const string& _Right)
		{	// insert _Right at _Off
			return (insert(_Off, _Right._Myptr(), 0, _Right.size()));
		}

		string& insert(size_type _Off, const _Elem *_Ptr)
		{	// insert [_Ptr, <null>) at _Off
			return (insert(_Off, _Ptr, 0, strlen(_Ptr)));
		}

		string& insert(size_type _Off,
			const _Elem *_Ptr, size_type _Count)
		{	// insert [_Ptr, _Ptr + _Count) at _Off
			return (insert(_Off, _Ptr, 0, _Count));
		}

		string& insert(size_type _Off,
			const _Elem* _Right, size_type _Roff, size_type _Count)
		{	// insert _Right [_Roff, _Roff + _Count) at _Off
			if (this->_Mysize < _Off)
				return *this;

			if (0 < _Count)
			{	// make room and insert new stuff

				size_type _Num = this->_Mysize + _Count;
				if( this->_Myres < _Num || _Alval.use_count() > 1)
				{
					_Alty tmp(_Num + STRING_RESERVED_BUF_SIZE);
					if( _Off > 0 )
						std::memcpy(tmp.get(), _Myptr(), _Off);	// empty out hole

					std::memcpy(tmp.get() + _Off, _Right + _Roff, _Count);	// empty out hole
					std::memcpy(tmp.get() + _Off + _Count, _Myptr() + _Off, this->_Mysize - _Off);	// empty out hole
					this->_Myres = tmp.get_buffer_size();// _Num + STRING_RESERVED_BUF_SIZE;
					_Alval.swap(tmp);
				}
				else
				{
					std::memmove(_Myptr() + _Off + _Count,
						_Myptr() + _Off, this->_Mysize - _Off);	// empty out hole

					std::memcpy(_Myptr() + _Off,
						_Right + _Roff, _Count);	// fill hole
				}

				this->_Mysize = 0;
				_Eos(_Num);
			}
			return (*this);
		}

		string& insert(size_type _Off,
			size_type _Count, _Elem _Ch)
		{	// insert _Count * _Ch at _Off
			if (this->_Mysize < _Off)
				return *this;

			if (0 < _Count)
			{	// make room and insert new stuff

				size_type _Num = this->_Mysize + _Count;
				if( this->_Myres < _Num || _Alval.use_count() > 1)
				{
					_Alty tmp(_Num + STRING_RESERVED_BUF_SIZE);
					if( _Off > 0 )
						std::memcpy(tmp.get(), _Myptr(), _Off);	// empty out hole

					std::memset(tmp.get() + _Off, _Ch, _Count);	// empty out hole
					std::memcpy(tmp.get() + _Off + _Count, _Myptr() + _Off, this->_Mysize - _Off);	// empty out hole
					this->_Myres = tmp.get_buffer_size();// _Num + STRING_RESERVED_BUF_SIZE;
					_Alval.swap(tmp);
				}
				else
				{
					std::memmove(_Myptr() + _Off + _Count,
						_Myptr() + _Off, this->_Mysize - _Off);	// empty out hole

					std::memset(_Myptr() + _Off,
						_Ch, _Count);	// fill hole
				}

				this->_Mysize = 0;
				_Eos(_Num);
			}
			return (*this);
		}

		string& erase( size_type _Off = 0,
			size_type _Count = -1 )
		{
			if (this->_Mysize < _Off)
				return *this;

			_Count = __min(this->_Mysize - _Off, _Count);

			if( 0 < _Count )
			{
				size_type Size = this->_Mysize - _Off - _Count;

				if( _Alval.use_count() > 1 )
				{
					_Alty	tmp( this->_Myres );
					std::memcpy(tmp.get(), _Myptr(), _Off);
					std::memcpy(tmp.get(), _Myptr() + _Off + _Count, Size);
					_Alval.swap(tmp);
				}
				else
				{
					if( Size > 0 )
					{
						std::memmove(_Myptr() + _Off, _Myptr() + _Off + _Count, Size);
					}
				}

				_Mysize = 0;
				_Eos(Size + _Off);
			}
			return *this;
		}

		iterator begin()
		{	// return iterator for beginning of mutable sequence
			return (_MY_STRING_ITERATOR(_Myptr()));
		}

		const_iterator begin() const
		{	// return iterator for beginning of nonmutable sequence
			return (_MY_STRING_CONST_ITERATOR(_Myptr()));
		}

		iterator end()
		{	// return iterator for end of mutable sequence
			return (_MY_STRING_ITERATOR(_Myptr() + this->_Mysize));
		}

		const_iterator end() const
		{	// return iterator for end of nonmutable sequence
			return (_MY_STRING_CONST_ITERATOR(_Myptr() + this->_Mysize));
		}

		string replace( const string& _Old, const string& _New, size_type _Timer = -1 ) 
		{	// replace [_Off, _Off + _N0) with _Count * _Ch

			size_type   _Pos = 0;
			for(size_type i = 0; i < _Timer; i ++)
			{
				_Pos = 0;
				if( ( _Pos = find(_Old) ) != size_type(-1) )
				{
					replace(_Pos, _Old.size(), _New);
				}
				else   
				{
					break;
				}
			}
			return *this;
		}

		string replace( const _Elem* _Old, const _Elem* _New, size_type _Timer = -1 ) 
		{	// replace [_Off, _Off + _N0) with _Count * _Ch

			size_type   _Pos = 0;
			for(int i = 0; i < _Timer; i ++)
			{
				_Pos = 0;
				if( ( _Pos = find(_Old) ) != size_type(-1) )
				{
					replace(_Pos, strlen(_Old), _New);
				}
				else   
				{
					break;
				}
			}
			return *this;
		}

		string touppers( void ) const
		{
			string sOut;
			sOut.reserve( this->_Myres );

			for( size_type i = 0; i < this->_Mysize; i ++ )
			{
				sOut[i] = std::toupper(*(_Myptr() + i));
			}
			
			sOut._Eos(this->_Mysize);
			return sOut;
		}

		string tolowers( void ) const 
		{
			string sOut;
			sOut.reserve( this->_Myres );

			for( size_type i = 0; i < this->_Mysize; i ++ )
			{
				sOut[i] = std::tolower(*(_Myptr() + i));
			}

			sOut._Eos(this->_Mysize);
			return sOut;
		}

		string strim( const _Elem *_Ptr = " " ) const
		{
			string sOut(*this);

			sOut.erase(0,sOut.find_first_not_of(_Ptr));
			sOut.erase(sOut.find_last_not_of(_Ptr) + 1);
			return sOut;
		}

		string lstrim( const _Elem *_Ptr = " " ) const
		{
			string sOut(*this);

			sOut.erase(0,sOut.find_first_not_of(_Ptr));
			return sOut;
		}

		string rstrim( const _Elem *_Ptr = " " ) const
		{
			string sOut(*this);
			sOut.erase(sOut.find_last_not_of(_Ptr) + 1);
			return sOut;
		}

#if defined(_WIN32) || defined(_WIN64)
		string wchar2char( void ) const
		{
			// wide char to multi char
			string sOut;
			int iLen = WideCharToMultiByte( CP_ACP,
				0,
				(LPWSTR)_Myptr(),
				-1,
				NULL,
				0,
				NULL,
				NULL );

			if( iLen < 1 )
				return false;

			sOut.resize(iLen);

			::WideCharToMultiByte( CP_ACP,
				0,
				(LPWSTR)_Myptr(),
				-1,
				sOut._Myptr(),
				iLen,
				NULL,
				NULL );
			return sOut;
		}

		string char2wchar( void ) const
		{
			string sOut;

			int iLen = ::MultiByteToWideChar( CP_ACP,
				0,
				_Myptr(),
				-1,
				NULL,
				0 ); 

			if( iLen < 1 )
				return false;

			sOut.resize(iLen * sizeof(wchar_t));

			::MultiByteToWideChar( CP_ACP,
				0,
				_Myptr(),
				-1,
				(LPWSTR)sOut._Myptr(),
				iLen );

			return sOut;
		}
#endif

	string utf8_gbk( void ) const
	{
# if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)

		size_t	inLen = size();
		size_t	outLen = inLen * 3;

		char*	pInBuf = _Myptr();
		char*	pOutBuf = (char*)MemoryAllocator::Alloc(outLen);
		char*	pMalloc = pOutBuf;
		memset(pOutBuf,0,outLen);
		if( !_base_::Charset::gcc_utf8_gbk(pInBuf, inLen, pOutBuf) )
			return "";

		string sOut(pOutBuf);
		MemoryAllocator::Free(pMalloc);

		return sOut;
# elif defined(_WIN32) || defined(_WIN64)
		unsigned short *wszGBK;
		char *szGBK;
		//长度
		int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)_Myptr(), -1, NULL, 0);
		wszGBK = (unsigned short*)MemoryAllocator::Alloc((len + 1) * sizeof(unsigned short));
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)_Myptr(), -1, (LPWSTR)wszGBK, len);

		//长度
		len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
		szGBK = (char*)MemoryAllocator::Alloc(len+1);
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

		//存空间将被释放，此时将得不到szOut的内容
		string sOut(szGBK);

		MemoryAllocator::Free(szGBK);
		MemoryAllocator::Free(wszGBK);

		return sOut;
#endif
	}

	string gbk_utf8( void ) const
	{
# if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
		size_t	inLen = size();
		size_t	outLen = inLen * 3;

		char*	pInBuf = _Myptr();
		char*	pOutBuf = (char*)MemoryAllocator::Alloc(outLen);
		char*	pMalloc = pOutBuf;
		memset(pOutBuf,0,outLen);

		if( !_base_::Charset::gcc_gbk_utf8(pInBuf, inLen, pOutBuf) )
			return "";
		string sOut(pMalloc);
		MemoryAllocator::Free(pMalloc);

		return sOut;
# elif defined(_WIN32) || defined(_WIN64)
		char* strGBK = _Myptr();
		int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, NULL,0);
		unsigned short* wszUtf8 = (unsigned short*)MemoryAllocator::Alloc((len + 1) * sizeof(unsigned short));
		memset(wszUtf8, 0, len * 2 + 2);
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

		len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
		char *szUtf8 = (char*)MemoryAllocator::Alloc(len + 1);
		memset(szUtf8, 0, len + 1);
		WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

		string sOut(szUtf8);

		MemoryAllocator::Free(szUtf8);
		MemoryAllocator::Free(wszUtf8);

		return sOut;
#endif
	}

	protected:
		bool _Inside( const _Elem *_Ptr ) const 
		{
			if (!_Alval || _Ptr < _Myptr() || _Myptr() + _Mysize <= _Ptr)
				return (false);	// don't ask
			else
				return (true);
		}

		void	Clone( size_type _Newsize )
		{
			_Alty	tmp( _Newsize );
			std::memcpy(tmp.get(), _Myptr(), this->_Mysize + 1);
			_Alval.swap(tmp);
			this->_Myres = tmp.get_buffer_size();// _Newsize;
		}

		_Elem* _Grow( size_type _Newsize )
		{
			if( _Mysize + _Newsize > _Myres )
			{//空间不足，重新分配
gt_alloc:
				_Alty AVal(_Mysize + _Newsize + STRING_RESERVED_BUF_SIZE);
				if( _Mysize != 0 )
				{
					std::memcpy(AVal.get(), _Myptr(), _Mysize);
				}

				_Myres = AVal.get_buffer_size();
				_Alval.swap(AVal);
			}
			else if( _Alval.use_count() > 1 )
			{
				goto gt_alloc;
			}

			return &(_Myptr()[_Mysize]);
		}

		_Elem* _Grow( size_type _Newsize, bool _Trim, int iReserved = STRING_RESERVED_BUF_SIZE )
		{
			_Mysize = 0;
			if( _Newsize > STRING_BUF_SIZE )
			{
				_Alty Aty = _Alty(_Newsize + iReserved);
				_Myres = Aty.get_buffer_size();
				_Alval.swap( Aty );
			}
			else
			{
				_Alty Aty = _Alty(_Buf, STRING_BUF_SIZE);
				_Myres = Aty.get_buffer_size();
				_Alval.swap( Aty );
			}

			return _Myptr();
		}

		void _Eos( size_type _Newsize )
		{
			_Mysize += _Newsize;
			_Myptr()[_Mysize] = 0;
		}

		char* _GetDataBuf( void )
		{
			return reinterpret_cast<char*>(_Buf) + STRING_OBJ_SIZE_V30;
		}
	};

	template<typename Allocate>
	inline string<Allocate> operator+( const string<Allocate>& _Left, const string<Allocate>& _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(_Left.size() + _Right.size());
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

	template<typename Allocate>
	inline string<Allocate> operator+( const _Elem* _Left, const string<Allocate>& _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(strlen(_Left) + _Right.size());
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

#if defined(_SUPPORTED_CXX11_)
	template<typename Allocate>
	inline string<Allocate> operator+( const _Elem* _Left, string<Allocate>&& _Right )
	{
		return (std::move(_Right.insert(0, _Left)));
	}
#endif
	template<typename Allocate>
	inline string<Allocate> operator+( const _Elem _Left, const string<Allocate>& _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(1 + _Right.size());
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

#if defined(_SUPPORTED_CXX11_)
	template<typename Allocate>
	inline string<Allocate> operator+( const _Elem _Left, string<Allocate>&& _Right )
	{
		return (std::move(_Right.insert(0, 1, _Left)));
	}
#endif
	template<typename Allocate>
	inline string<Allocate> operator+( const string<Allocate>& _Left, const _Elem* _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(_Left.size() + strlen(_Right));
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

#if defined(_SUPPORTED_CXX11_)
	template<typename Allocate>
	inline string<Allocate> operator+(string<Allocate>&& _Left, const _Elem* _Right )
	{
		return (std::move(_Left.append(_Right)));
	}
#endif

	template<typename Allocate>
	inline string<Allocate> operator+( const string<Allocate>& _Left, const _Elem _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(1 + _Left.size());
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

#if defined(_SUPPORTED_CXX11_)
	template<typename Allocate>
	inline string<Allocate> operator+(string<Allocate>&& _Left, const _Elem _Right )
	{
		return (std::move(_Left.insert(0, 1, _Right)));
	}
#endif

	template<typename Allocate>
	inline string<Allocate> operator+(string<Allocate>& _Left, const _Elem _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(1 + _Left.size());
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

	template<typename Allocate>
	inline string<Allocate> operator+(string<Allocate>& _Left, const _Elem* _Right )
	{
		string<Allocate> _Ans;
		_Ans.reserve(strlen(_Right) + _Left.size());
		_Ans += _Left;
		_Ans += _Right;
		return (_Ans);
	}

	template<typename Allocate>
	inline bool operator==( const string<Allocate>& _Left, const string<Allocate>& _Right)
	{
		return (_Left.compare(_Right) == 0);
	}

	template<typename Allocate>
	inline bool operator==( const _Elem* _Left, const string<Allocate>& _Right )
	{
		return (_Right.compare(_Left) == 0);
	}

	template<typename Allocate>
	inline bool operator==( const string<Allocate>& _Left, const _Elem*_Right )
	{
		return (_Left.compare(_Right) == 0);
	}

	template<typename Allocate>
	inline bool operator!=( const string<Allocate>& _Left, const string<Allocate>& _Right )
	{
		return (!(_Left == _Right));
	}

	template<typename Allocate>
	inline bool operator!=( const _Elem* _Left, const string<Allocate>& _Right )
	{
		return (!(_Left == _Right));
	}

	template<typename Allocate>
	inline bool operator!=( const string<Allocate>& _Left,  const _Elem*_Right )
	{
		return (!(_Left == _Right));
	}

	template<typename Allocate>
	inline bool operator<( const string<Allocate>& _Left, const string<Allocate>& _Right )
	{
		return (_Left.compare(_Right) < 0);
	}

	template<typename Allocate>
	inline bool operator<( const _Elem* _Left, const string<Allocate>& _Right )
	{
		return (_Right.compare(_Left) > 0);
	}

	template<typename Allocate>
	inline bool operator<( const string<Allocate>& _Left,  const _Elem*_Right )
	{
		return (_Left.compare(_Right) < 0);
	}

	template<typename Allocate>
	inline bool operator>( const string<Allocate>& _Left, const string<Allocate>& _Right )
	{
		return (_Right < _Left);
	}

	template<typename Allocate>
	inline bool operator>( const _Elem* _Left, const string<Allocate>& _Right )
	{
		return (_Right < _Left);
	}

	template<typename Allocate>
	inline bool operator>( const string<Allocate>& _Left,  const _Elem*_Right )
	{
		return (_Right < _Left);
	}

	template<typename Allocate>
	inline bool operator<=( const string<Allocate>& _Left, const string<Allocate>& _Right )
	{
		return (!(_Right < _Left));
	}

	template<typename Allocate>
	inline bool operator<=( const _Elem* _Left, const string<Allocate>& _Right )
	{
		return (!(_Right < _Left));
	}

	template<typename Allocate>
	inline bool operator<=( const string<Allocate>& _Left,  const _Elem*_Right )
	{
		return (!(_Right < _Left));
	}

	template<typename Allocate>
	inline bool operator>=( const string<Allocate>& _Left, const string<Allocate>& _Right )
	{
		return (!(_Left < _Right));
	}

	template<typename Allocate>
	inline bool operator>=( const _Elem* _Left, const string<Allocate>& _Right )
	{
		return (!(_Left < _Right));
	}

	template<typename Allocate>
	inline bool operator>=( const string<Allocate>& _Left,  const _Elem*_Right )
	{
		return (!(_Left < _Right));
	}

#undef _MY_STRING_CONST_ITERATOR
#undef _MY_STRING_ITERATOR

} //namespace	_foundation_

#include <libString_v30/hash.hpp>

#endif // __MODAFX_20160418173305_1438860785_H__
