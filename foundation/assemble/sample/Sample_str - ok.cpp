// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <libTimestamp/Timestamp.h>

#define STRING_RESERVED_BUF_SIZE	16
#define	_OBJ_SIZE					24
#define _BUF_SIZE					16

#if defined(PLATFORM_OS_FAMILY_UNIX)
	#include <pthread.h>
	#define CASCompare		__sync_bool_compare_and_swap
	#define	CASFetchAdd		__sync_fetch_and_add
	#define	CASAddFetch		__sync_add_and_fetch
	#define	CASFetchSub		__sync_fetch_and_sub
	#define	CASSubFetch		__sync_sub_and_fetch
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <windows.h>
	#include <intrin.h>
	#define CASCompare		_InterlockedCompareExchange
	#define	CASFetchAdd		_InterlockedExchangeAdd
	#define CASAddFetch     _InterlockedIncrement
	#define	CASFetchSub		_InterlockedExchangeAdd
	#define CASSubFetch     _InterlockedDecrement
#endif

class sp_counted_base
{
protected:
	long _use_count;        // #shared
	char* _px;

	sp_counted_base( sp_counted_base const & );
	sp_counted_base & operator= ( sp_counted_base const & );

public:
	sp_counted_base( char* px ): _use_count( 1 ), _px( px )
	{
	}

	virtual bool add_ref_copy( sp_counted_base* pi ) = 0;

	virtual void release() = 0; // nothrow

	long use_count() const // nothrow
	{
		return static_cast<long const volatile &>( _use_count );
	}

	char*	get( void ) const
	{
		return _px;
	}
};

class sp_counted_impl_p : public sp_counted_base
{
private:
	sp_counted_impl_p( sp_counted_impl_p const & );
	sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

public:
	static sp_counted_base* GetInstance( UInt32 uSize )
	{
		void* p = malloc( sizeof(sp_counted_impl_p) + uSize + 1 );
		return new (p) sp_counted_impl_p( reinterpret_cast<char*>(p) + sizeof(sp_counted_impl_p) );
	}

public:
	sp_counted_impl_p( char* px ): sp_counted_base( px )
	{

	}

	bool add_ref_copy( sp_counted_base* pi )
	{
		CASAddFetch( &_use_count );
		return true;
	}

	void release() // nothrow
	{
		if( CASSubFetch( &_use_count ) == 0 )
		{
			free(_px - sizeof(sp_counted_impl_p));
		}
	}
};

class sp_counted_impl_buf : public sp_counted_base
{
private:
	sp_counted_impl_buf( sp_counted_impl_buf const & );
	sp_counted_impl_buf & operator= ( sp_counted_impl_buf const & );

public:
	static sp_counted_base* GetInstance( void* pBuf )
	{
		return new(pBuf) sp_counted_impl_buf( reinterpret_cast<char*>(pBuf) + _OBJ_SIZE );
	}

public:
	sp_counted_impl_buf( char* px ): sp_counted_base( px )
	{

	}

	bool add_ref_copy( sp_counted_base* pi )
	{
		memcpy(pi->get(), _px, _BUF_SIZE);
		return false;
	}

	void release() // nothrow
	{

	}
};

class shared_count
{
private:
	sp_counted_base * _pi;

public:

	shared_count(): _pi(0) // nothrow
	{
	}

	explicit shared_count( UInt32 uSize )
	{
		_pi = sp_counted_impl_p::GetInstance( uSize );
	}

	explicit shared_count( void* pBuf )
	{
		_pi = sp_counted_impl_buf::GetInstance( pBuf );
	}

	~shared_count() // nothrow
	{
		if( _pi != 0 ) _pi->release();
	}

	shared_count(shared_count const & r): _pi(r._pi) // nothrow
	{
		if( _pi != 0 ) _pi->add_ref_copy(_pi);
	}

	shared_count&	operator=( const shared_count& r )
	{
		if( r._pi != _pi )
		{
			if( _pi != 0 ) _pi->release();
			if( r._pi != 0 )
			{
				if( r._pi->add_ref_copy(_pi) )
					_pi = r._pi;
			}
		}

		return *this;
	}

	shared_count&	operator=( const shared_count&& r )
	{
		if( r._pi != _pi )
		{
			if( _pi != 0 ) _pi->release();
			if( r._pi != 0 ) 
			{
				if( r._pi->add_ref_copy(_pi) )
					_pi = r._pi;
			}
		}

		return *this;
	}

	void reset( void )
	{
		if( _pi != 0 ) 
		{
			_pi->release();
			_pi = 0;
		}
	}

	void	swap( shared_count& r )
	{
		sp_counted_base * tmp = r._pi;
		r._pi = _pi;
		_pi = tmp;
	}

	char*	get( void ) const
	{
		return _pi != 0 ? _pi->get(): 0;
	}

	long use_count() const // nothrow
	{
		return _pi != 0 ? _pi->use_count(): 0;
	}

	bool empty() const // nothrow
	{
		return _pi == 0;
	}

	operator bool ()
	{
		return empty() ? false : true;
	}

	bool operator!()
	{ 
		return empty();
	}

	operator bool () const
	{
		return empty() ? false : true;
	}

	bool operator!() const
	{ 
		return empty();
	}
};


class string
{
public:
	typedef UInt32			size_type;
	typedef	string			self_type;
	typedef char&			reference;
	typedef const char&		const_reference;
	typedef char			_Elem;
	typedef shared_count	_Alty ;

private:
	size_type		_Mysize;
	size_type		_Myres;
	_Alty			_Alval;	// allocator object for strings
	char*			_Buf[_OBJ_SIZE + _BUF_SIZE + 1];

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
		assign(_Ptr, strlen(_Ptr), true);
	}

	string( const _Elem* _Ptr, size_type _Count )
		: _Mysize(0)
		, _Myres(0)
	{
		assign(_Ptr, _Count, true);
	}

	string( size_type _Count, char _Ch )
		: _Mysize(0)
		, _Myres(0)
	{
		assign(_Count, _Ch);
	}

	reference operator[]( size_type _Off )
	{
		return _Alval.get()[_Off];
	}

	const_reference operator[]( size_type _Off ) const
	{
		return _Alval.get()[_Off];
	}

	string& operator=( const string& _Right )
	{
		if( !_Inside(_Right.c_str()) )
		{
			_Mysize = _Right._Mysize;
			_Myres = _Right._Myres;

			if( !_Alval )
			{
				_Alty Aty = _Alty(_Buf);
				_Alval.swap(Aty);	
			}

			_Alval = _Right._Alval;
		}

		return *this;
	}

	string& operator=( string&& _Right  )
	{
		if( !_Inside(_Right.c_str()) )
		{
			_Mysize = _Right._Mysize;
			_Myres = _Right._Myres;
			_Alval.swap( reinterpret_cast<string&&>(_Right)._Alval);
		}

		return *this;
	}

	string& operator=( const _Elem* _Ptr )
	{	
		int iLen = strlen(_Ptr);
		if(_Alval.use_count() == 1)
		{
			if( iLen < _Myres )
			{
				_Mysize = 0;
				std::memcpy(_Alval.get(), _Ptr, iLen);
				return *this;
			}
		}

		assign(_Ptr, iLen, true);
		return *this;
	}

	string& operator=( _Elem _Ch )
	{	
		if(_Alval.use_count() == 1)
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
			erase(0, _Newsize);
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
		if( _N0 == _Count )
			return memcmp(&(_Alval.get()[_Off]), _Ptr, __min(_N0, _Count));

		return ( _N0 < _Count ? -1 : 1);
	}

	void reserve( size_type _Newcap )
	{	
		if ( this->_Mysize <= _Newcap && this->_Myres != _Newcap )
		{
			if( _Grow(_Newcap - STRING_RESERVED_BUF_SIZE, true) )
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
		return find(_Right.c_str(), _Off, _Right.size());
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
			for (_Nm -= _Count - 1, _Vptr = c_str() + _Off;
				(_Uptr = (const _Elem *)memchr(_Vptr, *_Ptr, _Nm)) != 0;
				_Nm -= _Uptr - _Vptr + 1, _Vptr = _Uptr + 1)
				if (memcmp(_Uptr, _Ptr, _Count) == 0)
					return (_Uptr - c_str());	// found a match
		}

		return (-1);	// no match
	}

	size_type rfind(const _Elem *_Ptr, size_type _Off = -1) const
	{	// look for [_Ptr, <null>) beginning before _Off
		return rfind(_Ptr, _Off, strlen(_Ptr));
	}

	size_type rfind(const string& _Right, size_type _Off = -1) const
	{	// look for _Right beginning before _Off
		return rfind(_Right.c_str(), _Off, _Right.size());
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
			const _Elem *_Uptr = c_str() +
				(_Off < this->_Mysize - _Count ? _Off
				: this->_Mysize - _Count);

			for (; ; --_Uptr)
				if (*_Uptr == *_Ptr
					&& memcmp(_Uptr, _Ptr, _Count) == 0)
					return (_Uptr - c_str());	// found a match
				else if (_Uptr == c_str())
					break;	// at beginning, no more chance for match
		}

		return (-1);	// no match
	}

	string& replace(size_type _Off, size_type _N0, const string& _Right)
	{	// replace [_Off, _Off + _N0) with _Right
		return (replace(_Off, _N0, _Right.c_str(), 0, _Right.size()));
	}

	string& replace(size_type _Off, size_type _N0, const _Elem *_Ptr)
	{	// replace [_Off, _Off + _N0) with [_Ptr, <null>)
		return (replace(_Off, _N0, _Ptr, 0, strlen(_Ptr) ));
	}

	string& replace(size_type _Off,
		size_type _N0, size_type _Count, _Elem _Ch)
	{	// replace [_Off, _Off + _N0) with _Count * _Ch
		return (replace(_Off, _N0, (const char*)&_Ch, 0, 1 ));
	}

	string& replace(size_type _Off,
		size_type _N0, const char* _Right, size_type _Roff, size_type _Count)
	{	
		if (this->_Mysize < _Off || _Count < _Roff ||
			_Right == NULL || c_str() == _Right )
			return *this;

		_N0 = __min(this->_Mysize - _Off, _N0);

		size_type _Newsize = this->_Mysize + _Count - _N0;

		size_type _Nm = size() - _N0 - _Off;	// length of kept tail

		if (this->_Myres < _Newsize)
		{
			_Alty _tmp;
			_tmp.swap(_Alval);
			_Grow(_Newsize, true);

			if( _Off > 0 )
				std::memcpy(_Myptr(), _tmp.get(), _Off);	// fill hole

			std::memcpy(_Myptr() + _Off, _Right + _Roff, _Count);	// fill hole

			if( _Nm > 0 )
				std::memcpy(_Myptr() + _Off + _Count, _tmp.get() + _Off + _N0, _Nm);	// fill hole

			_Eos(_Newsize);
		}
		else
		{
			// no overlap, just move down and copy in new stuff
			if( _Off > 0 )
				std::memmove(_Myptr() + _Off + _Count, _Myptr() + _Off + _N0, _Off);	// fill hole

			std::memcpy(_Myptr() + _Off, _Right + _Roff, _Count);	// fill hole
			_Mysize = 0;
			_Eos(_Newsize);
		}
		return (*this);
	}

	size_type find_first_of(const string& _Right,
						size_type _Off = 0) const
	{	// look for one of _Right at or after _Off
		return ( find_first_of(_Right.c_str(), _Off, _Right.size()) );
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
			const _Elem *const _Vptr = c_str() + this->_Mysize;
			for (const _Elem *_Uptr = c_str() + _Off; _Uptr < _Vptr; ++_Uptr)
				if (std::memchr(_Ptr, *_Uptr, _Count) != 0)
					return (_Uptr - c_str());	// found a match
		}

		return (-1);	// no match
	}

	size_type find_last_of(const string& _Right,
		size_type _Off = -1) const
	{	// look for one of _Right before _Off
		return (find_last_of(_Right.c_str(), _Off, _Right.size()));
	}

	size_type find_last_of(const _Elem *_Ptr,
		size_type _Off, size_type _Count) const
	{	// look for one of [_Ptr, _Ptr + _Count) before _Off

		if (0 < _Count && 0 < this->_Mysize)
		{	// worth searching, do it
			const _Elem *_Uptr = c_str()
				+ (_Off < this->_Mysize ? _Off : this->_Mysize - 1);
			for (; ; --_Uptr)
				if (std::memchr(_Ptr, _Count, *_Uptr) != 0)
					return (_Uptr - c_str());	// found a match
				else if (_Uptr == c_str())
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
		return (find_first_not_of(_Right.c_str(), _Off,
			_Right.size()));
	}

	size_type find_first_not_of(const _Elem *_Ptr,
		size_type _Off = 0) const
	{	// look for one of [_Ptr, <null>) at or after _Off
		return (find_first_not_of(_Ptr, _Off, strlen(_Ptr)));
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
			const _Elem *const _Vptr = c_str() + this->_Mysize;
			for (const _Elem *_Uptr = c_str() + _Off; _Uptr < _Vptr; ++_Uptr)
				if (std::memchr(_Ptr, _Count, *_Uptr) == 0)
					return (_Uptr - c_str());
		}
		return (-1);
	}

	size_type find_last_not_of(const string& _Right,
		size_type _Off = -1) const
	{	// look for none of _Right before _Off
		return (find_last_not_of(_Right.c_str(), _Off, _Right.size()));
	}

	size_type find_last_not_of(const _Elem *_Ptr,
		size_type _Off = -1) const
	{	// look for none of [_Ptr, <null>) before _Off
		return (find_last_not_of(_Ptr, _Off, strlen(_Ptr)));
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
			const _Elem *_Uptr = c_str()
				+ (_Off < this->_Mysize ? _Off : this->_Mysize - 1);
			for (; ; --_Uptr)
				if (std::memchr(_Ptr, _Count, *_Uptr) == 0)
					return (_Uptr - c_str());
				else if (_Uptr == c_str())
					break;
		}
		return (-1);
	}

	string substr(size_type _Off = 0, size_type _Count = -1) const
	{	// return [_Off, _Off + _Count) as new string
		return (string(&(c_str()[_Off]), _Count));
	}

	string& erase( size_type _Off = 0,
		size_type _Count = -1 )
	{
		if (this->_Mysize < _Off)
			return *this;

		//_Count = __min(this->_Mysize - _Off, _Count);
		if( this->_Mysize - _Off < _Count )
			_Count = this->_Mysize - _Off;

		if( 0 < _Count )
		{
			size_type Size = this->_Mysize - _Off - _Count;
			if( Size > 0 )
				std::memmove(_Myptr() + _Off, _Myptr() + _Off + _Count, Size);

			_Mysize = 0;
			_Eos(Size - _Off);
		}
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
			_Myres = _Mysize + _Newsize + STRING_RESERVED_BUF_SIZE ;
gt_alloc:
			try
			{
				_Alty AVal(_Myres);

				if( _Alval )
				{
					std::memcpy(AVal.get(), _Alval.get(), _Mysize);
				}

				_Alval.swap(AVal);
			}
			catch(std::bad_alloc &)
			{
				return NULL;
			}
		}
		else if( _Alval.use_count() > 1 )
		{
			goto gt_alloc;
		}

		return &(_Alval.get()[_Mysize]);
	}

	_Elem* _Grow( size_type _Newsize, bool _Trim )
	{
		_Mysize = 0;
		if( _Newsize > _BUF_SIZE )
		{
			_Myres = _Newsize + STRING_RESERVED_BUF_SIZE;
			try
			{
				_Alty Aty = _Alty(_Myres);
				_Alval.swap( Aty );
			}
			catch(std::bad_alloc &)
			{
				return NULL;
			}
		}
		else
		{
			_Myres = _BUF_SIZE;
			_Alval.swap( _Alty(_Buf) );
		}

		return _Alval.get();
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

string operator+( string&& _Left, const string::_Elem* _Right )
{
	return (std::move(_Left.append(_Right)));
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

const string& str_Replace_ex( string& str, const string& old_value,
	const string& new_value )
{
	while(true) 
	{   
		string::size_type   pos(0);   
		if( (pos = str.find(old_value)) != std::string::npos )   
		{
			str.replace(pos,old_value.length(),new_value); 
		}
		else   
		{
			break;
		}
	}   
	return   str;  
}

int main(int argc, char* argv[])
{	
	int iii = sizeof(sp_counted_impl_buf);

	std::string sss1("1234");
	sss1.append("12");
	sss1.append("888");

	sss1.replace(0, 2, "888888");

	int num = 100000000;

	while(true)
	{
		/*{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s("123456789123456789789789");
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
			string s1;
			string s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s1 = s;
			}
			printf("string4 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s1;
			std::string s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s1 = s;
			}
			printf("std::string4 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s;
			string s1("123456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = string("123465679897987985646546321321");
			}
			printf("string5 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s;
			std::string s1("123456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = std::string("123465679897987985646546321321");
			}
			printf("std::string5 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s;
			string s1("123456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = "123465679897987985646546321321";
			}
			printf("string6 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s;
			std::string s1("123456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = "123465679897987985646546321321";
			}
			printf("std::string6 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s("12346557989621313132165465432131313123");
				s.append("123465579896");
				s.append("123465579896");
			}
			printf("string7 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{	
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s("12346557989621313132165465432131313123");
				s.append("123465579896");
				s.append("123465579896");
			}
			printf("std::string7 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
			}
			printf("string8 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
			}
			printf("std::string8 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s;
				s += "123465679897987985646546321321";
			}
			printf("string9 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s;
				s += "123465679897987985646546321321";
			}
			printf("std::string9 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s;
				s += string("123465679897987985646546321321");
			}
			printf("string10 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s;
				s += std::string("123465679897987985646546321321");
			}
			printf("std::string10 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		//while(true)
		{
			{
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					string s;
					s.reserve(100);
				}
				printf("string11 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}

			{
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					std::string s;
					s.reserve(100);
				}
				printf("std::string11 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s;
				s.resize(100);
				s.resize(50);
			}
			printf("string12 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s;
				s.resize(100);
				s.resize(50);
			}
			printf("std::string12 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s;
			string s1("12346557989621313132165465432131313123");
			string s2("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = s1 + s2;
			}
			printf("string13 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s;
			std::string s1("12346557989621313132165465432131313123");
			std::string s2("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = s1 + s2;
			}
			printf("std::string13 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}*/

		{
			string s;
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = string("12346557989621313132165465432131313123") + 
					"12346557989621313132165465432131313123";
			}
			printf("string14 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s;
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = std::string("12346557989621313132165465432131313123") +
					"12346557989621313132165465432131313123";
			}
			printf("std::string14 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == "12346557989621313132165465432131313123" )
				{
					int i = 0;
				}
			}
			printf("string15 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == "12346557989621313132165465432131313123" )
				{
					int i = 0;
				}
			}
			printf("std::string15 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == string("12346557989621313132165465432131313123") )
				{

				}
			}
			printf("string16 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == std::string("12346557989621313132165465432131313123") )
				{

				}
			}
			printf("std::string16 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s != string("123465579896213131321654654321313131231") )
				{

				}
			}
			printf("string17 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s != std::string("123465579896213131321654654321313131231") )
				{

				}
			}
			printf("std::string17 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.find("131");
				int y = 0;
			}
			printf("string18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.find("131");
				int y = 0;
			}
			printf("std::string18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string s("88846557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.rfind("888");
				int y = 0;
			}
			printf("string18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("88846557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.rfind("888");
				int y = 0;
			}
			printf("std::string18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, 6, "888888");
			}
			printf("string19 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, 6, "888888");
			}
			printf("std::string19 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			char* sz = "888888888888888888888888888888";
			string ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, strlen(sz), string(sz));
			}
			printf("string20 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			char* sz = "888888888888888888888888888888";
			std::string ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, strlen(sz), std::string(sz));
			}
			printf("std::string20 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string ss = "123456798794654643311123456789645674878978";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.substr(10, 30);
			}
			printf("string21 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string ss = "123456798794654643311123456789645674878978";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.substr(10, 30);
			}
			printf("std::string21 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string ss = "1234567890";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string s = ss;
			}
			printf("string22 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string ss = "1234567890";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s = ss;
			}
			printf("std::string22 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}
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