
#include <libCommon/CMyOperation.h>

CMyOperation::CMyOperation( const unsigned long ulLevel ) 
	: _ulLevel(ulLevel) 
{

}

CMyOperation::~CMyOperation( void )
{

}

inline const unsigned long	CMyOperation::Multiplication( const unsigned long	value ) const
{	
	return (value << _ulLevel); 
}

inline	const unsigned long	CMyOperation::Division( const unsigned long value ) const
{ 
	return (value >> _ulLevel);
}

inline	const unsigned long	CMyOperation::Mod( const unsigned long value ) const
{	
	return (value - ((value >> _ulLevel) << _ulLevel)); 
}

