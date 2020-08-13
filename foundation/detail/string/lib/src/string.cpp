
#include <libString/string.hpp>

namespace	_string_{

	void	string::Clone( size_type _Newsize )
	{
		_Alty	tmp( _Newsize );
		std::memcpy(tmp.get(), _Myptr(), this->_Mysize + 1);
		_Alval.swap(tmp);
		this->_Myres = _Newsize;
	}

	_Elem* string::_Grow( size_type _Newsize )
	{
		if( _Mysize + _Newsize > _Myres )
		{//空间不足，重新分配
			_Myres = _Mysize + _Newsize + STRING_RESERVED_BUF_SIZE ;
gt_alloc:
			_Alty AVal(_Myres);
			if( _Mysize != 0 )
			{
				std::memcpy(AVal.get(), _Myptr(), _Mysize);
			}

			_Alval.swap(AVal);
		}
		else if( _Alval.use_count() > 1 )
		{
			goto gt_alloc;
		}

		return &(_Myptr()[_Mysize]);
	}

	_Elem* string::_Grow( size_type _Newsize, bool _Trim, int iReserved )
	{
		_Mysize = 0;
		if( _Newsize > STRING_BUF_SIZE )
		{
			_Myres = _Newsize + iReserved;

			_Alty Aty = _Alty(_Myres);
			_Alval.swap( Aty );
		}
		else
		{
			_Myres = STRING_BUF_SIZE;
			_Alty Aty = _Alty(_Buf);
			_Alval.swap( Aty );
		}

		return _Myptr();
	}

	char* string::_GetDataBuf( void )
	{
		return reinterpret_cast<char*>(_Buf) + STRING_OBJ_SIZE;
	}

}// namespace	_foundation_
