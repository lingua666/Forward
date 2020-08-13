
#include <libCommon/CMyException.h>
  
//custom exceptions  
/// /brief Create exception object as copy of another
CMyException::CMyException(int iCode) throw() 
	: _sMessage("")
	, _iCode(iCode)
{

}

CMyException::CMyException(const std::string& sMessage, int iCode) throw() 
	: _sMessage(sMessage)
	, _iCode(iCode)
{

}

CMyException::~CMyException( void ) throw() 
{

}

/// Creates an exception.

/// /brief Assign another exception object's contents to this one  
CMyException& CMyException::operator=( const CMyException& rhs ) throw()  
{  
	_sMessage = rhs._sMessage;
	_iCode = rhs._iCode;
	return *this;  
}  

/// /brief Returns explanation of why exception was thrown  
const char* CMyException::what( void ) const throw()  
{  
	return _sMessage.c_str();
}

const char* CMyException::name( void ) const throw()
{
	return "CMyException";
}

const std::string& CMyException::message( void ) const
{
	return _sMessage;
}

std::string CMyException::display_message( void ) const
{
	std::string sText = std::string(name());
	if (!_sMessage.empty())
	{
		sText.append(": ");
		sText.append(_sMessage);
	}
	return sText;
}

const int	CMyException::get_code( void ) const
{
	return _iCode;
} 

std::string	get_message_string( const char *format, ... )
{
	EXCEPT_VA_ARGS_LIST(format);
	return std::string(log_buffer);
}
