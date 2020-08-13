// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <libTimestamp/Timestamp.h>
#include <libSmart_ptr_v20/Shared_ptr.h>

#define STRING_RESERVED_BUF_SIZE	16

class string
{
public:
	typedef UInt32			size_type;
	typedef	string			self_type;
	typedef char&			reference;
	typedef const char&		const_reference;
	typedef char			_Elem;
	typedef SmartPTR<_Elem>	_Alty ;

private:
	size_type		_Mysize;
	size_type		_Myres;
	_Alty			_Alval;	// allocator object for strings

public:
	string( void )
		: _Mysize(0)
		, _Myres(0)
	{

	}

	string( const self_type& t1 )
		: _Mysize(t1._Mysize)
		, _Myres(t1._Myres)
		, _Alval(t1._Alval)
	{

	}

	string( const _Elem* _Ptr )
		: _Mysize(0)
		, _Myres(0)
	{
		assign(_Ptr, strlen(_Ptr));
	}

	string( const _Elem* _Ptr, size_type _Count )
		: _Mysize(0)
		, _Myres(0)
	{
		assign(_Ptr, _Count);
	}

	string( size_type _Count, char _Ch )
		: _Mysize(0)
		, _Myres(0)
	{
		assign(_Count, _Ch);
	}

	string& append( const string& _Right )
	{	
		assign(_Right.c_str(), _Right.size());
		return *this;
	}

	string& append( const _Elem* _Ptr, size_type _Count )
	{
		assign(_Ptr, _Count);
		return *this;
	}

	string& append( const _Elem* _Ptr )
	{
		assign(_Ptr, strlen(_Ptr));
		return *this;
	}

	string& append( size_type _Count, _Elem _Ch )
	{
		assign(_Count, _Ch);
		return *this;
	}

	string& assign( const string& _Right )
	{
		if( !_Inside(_Right.c_str()) )
		{
			std::memcpy( _Grow(_Right.size()), _Right.c_str(), _Right.size() );
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
			_Mysize = 0;
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
		_Mysize = 0;
		std::memset(_Grow(_Count, _Trim), _Ch, _Count);
		_Eos( _Count );
		return *this;
	}

	void clear( void )
	{
		erase(0);
	}

	reference operator[]( size_type _Off )
	{
		return _Alval.get()[_Off];
	}

	const_reference operator[]( size_type _Off ) const
	{
		return _Alval.get()[_Off];
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
			erase(_Newsize);
		else
			append(_Newsize - _Mysize, _Ch);
	}

	bool empty( void ) const
	{
		return _Mysize == 0;
	}

	void swap( string& _Right )
	{

	}

	int compare( const string& _Right ) const
	{	
		return compare(0, _Mysize, _Right.c_str(), _Right.size());
	}

	int compare(const _Elem* _Ptr) const
	{
		return compare(0, _Mysize, _Ptr, strlen(_Ptr));
	}

	int compare(size_type _Off,
		size_type _N0, const _Elem* _Ptr, size_type _Count) const
	{
		return memcmp(&(_Alval.get()[_Off]), _Ptr, min(_N0, _Count));
	}

	void reserve( size_type _Newcap )
	{	
		if ( this->_Mysize <= _Newcap && this->_Myres != _Newcap )
		{
			if( _Grow(_Newcap) )
			{
				_Eos(_Newcap);
			}
		}
	}

	string& operator=( const string& _Right )
	{
		if( !_Inside(_Right.c_str()) )
		{
			_Mysize = _Right._Mysize;
			_Myres = _Right._Myres;
			_Alval = _Right._Alval;
		}
			
		return *this;
	}

	string& operator=( const _Elem* _Ptr )
	{	
		assign(_Ptr, strlen(_Ptr), true);
		return *this;
	}

	string& operator=( _Elem _Ch )
	{	
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

protected:
	bool _Inside( const _Elem *_Ptr ) const 
	{
		if (!_Alval || _Ptr < _Alval.get() || _Alval.get() + _Mysize <= _Ptr)
			return (false);	// don't ask
		else
			return (true);
	}

	_Elem* _Grow( size_type _Newsize )
	{
		if( _Mysize + _Newsize > _Myres )
		{//空间不足，重新分配

			_Myres = _Mysize + _Newsize + STRING_RESERVED_BUF_SIZE;

			try
			{
				_Alty	AVal( new char[ _Myres] );
				if( _Alval )
				{
					std::memcpy(AVal.get(), _Alval.get(), _Mysize);
				}

				_Alval = AVal;
			}
			catch(std::bad_alloc &)
			{
				return NULL;
			}
		}

		return &(_Alval.get()[_Mysize]);
	}

	_Elem* _Grow( size_type _Newsize, bool _Trim )
	{
		if( _Mysize + _Newsize > _Myres )
		{//空间不足，重新分配
			_Myres = _Mysize + _Newsize + STRING_RESERVED_BUF_SIZE;
			try
			{
				_Alty	AVal( new char[_Myres] );
				_Alval = AVal;
			}
			catch(std::bad_alloc &)
			{
				return NULL;
			}
		}

		return &(_Alval.get()[_Mysize]);
	}

	string& erase( size_type _Count )
	{
		_Mysize = 0;
		_Eos(_Count);
	}

	void _Eos( size_type _Newsize )
	{
		_Mysize += _Newsize;
		_Alval.get()[_Mysize] = '\0';
	}
};

string operator+( const string& _Left, const string& _Right )
{
	string _Ans;
	_Ans.reserve(_Left.size() + _Right.size());
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

string operator+( const string::_Elem* _Left, const string& _Right )
{
	string _Ans;
	_Ans.reserve(strlen(_Left) + _Right.size());
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

string operator+( const string::_Elem _Left, const string& _Right )
{
	string _Ans;
	_Ans.reserve(1 + _Right.size());
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

string operator+( const string& _Left, const string::_Elem* _Right )
{
	string _Ans;
	_Ans.reserve(_Left.size() + strlen(_Right));
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

string operator+( const string& _Left, const string::_Elem _Right )
{
	string _Ans;
	_Ans.reserve(1 + _Left.size());
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

string operator+( string& _Left, const string::_Elem _Right )
{
	string _Ans;
	_Ans.reserve(1 + _Left.size());
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

string operator+( string& _Left, const string::_Elem* _Right )
{
	string _Ans;
	_Ans.reserve(strlen(_Right) + _Left.size());
	_Ans += _Left;
	_Ans += _Right;
	return (_Ans);
}

bool operator==( const string& _Left, const string& _Right)
{
	return (_Left.compare(_Right) == 0);
}

bool operator==( const string::_Elem* _Left, const string& _Right )
{
	return (_Right.compare(_Left) == 0);
}

bool operator==( const string& _Left, const string::_Elem*_Right )
{
	return (_Left.compare(_Right) == 0);
}

bool operator!=( const string& _Left, const string& _Right )
{
	return (!(_Left == _Right));
}

bool operator!=( const string::_Elem* _Left, const string& _Right )
{
	return (!(_Left == _Right));
}

bool operator!=( const string& _Left,  const string::_Elem*_Right )
{
	return (!(_Left == _Right));
}

bool operator<( const string& _Left, const string& _Right )
{
	return (_Left.compare(_Right) < 0);
}

bool operator<( const string::_Elem* _Left, const string& _Right )
{
	return (_Right.compare(_Left) < 0);
}

bool operator<( const string& _Left,  const string::_Elem*_Right )
{
	return (_Left.compare(_Right) < 0);
}

bool operator>( const string& _Left, const string& _Right )
{
	return (_Right < _Left);
}

bool operator>( const string::_Elem* _Left, const string& _Right )
{
	return (_Right < _Left);
}

bool operator>( const string& _Left,  const string::_Elem*_Right )
{
	return (_Right < _Left);
}

bool operator<=( const string& _Left, const string& _Right )
{
	return (!(_Right < _Left));
}

bool operator<=( const string::_Elem* _Left, const string& _Right )
{
	return (!(_Right < _Left));
}

bool operator<=( const string& _Left,  const string::_Elem*_Right )
{
	return (!(_Right < _Left));
}

bool operator>=( const string& _Left, const string& _Right )
{
	return (!(_Left < _Right));
}

bool operator>=( const string::_Elem* _Left, const string& _Right )
{
	return (!(_Left < _Right));
}

bool operator>=( const string& _Left,  const string::_Elem*_Right )
{
	return (!(_Left < _Right));
}

int main(int argc, char* argv[])
{	
	int num = 100000000;
	{
		MemPool_type	_Pool20160726( sizeof(_smart_ptr::sp_counted_impl_p<char>) );

		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			int len = strlen("123456789123456789789789");
			char* sz = new char[len + 1 + 30];
			memcpy(&sz[30], "123456789123456789789789", len - 1);
			sz[len + 30] = '\0';

			new ((void*)sz) _smart_ptr::sp_counted_impl_p<char>(&sz[30]);
			//_Pool20160726.AllocObj< _smart_ptr::sp_counted_impl_p<char> >(sz);
			//_Pool20160726.FreeObj(_Pool20160726.AllocObj< _smart_ptr::sp_counted_impl_p<char> >(sz));
			delete sz;
			//string s("123456789123456789789789");
		}
		printf("string time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			std::string s("123456789123456789789789");
		}
		printf("std::string time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		string s;
		string s1("123456789");
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			s = s1;
		}
		printf("string1 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		std::string s;
		std::string s1("123456789");
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			s = s1;
		}
		printf("std::string1 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		string s("12345678923456789456789");
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			string s1 = s;
		}
		printf("string2 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		std::string s("12345678923456789456789");
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			std::string s1 = s;
		}
		printf("std::string2 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		string s("12345678923456789456789");
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			s.c_str();
		}
		printf("string3 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		std::string s("12345678923456789456789");
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			s.c_str();
		}
		printf("std::string3 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}
	

	{
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			string s;
		}
		printf("string4 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}

	{
		_timestamp_::Timestamp	time;
		for(int i = 0; i < num; i ++)
		{
			std::string s;
		}
		printf("std::string4 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
	}
	

	/*std::string s;
	s = "1234567890311221234564578987456321";
	s.resize(100);

	if(s == std::string(""))
	{
		int i = 0;
	}

	s += "1232324324324324";
	if( s == std::string("") )
	{
		int i = 0;
	}*/

	std::cin.get();
	return 0;
}