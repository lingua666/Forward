
#ifndef __CMYEXCEPTION_20150806193305_1438860785_H__
#define __CMYEXCEPTION_20150806193305_1438860785_H__

//
// Exception.h
// reference :
// $Id: //poco/1.3/Foundation/include/Poco/Exception.h#2 $
// Library: Foundation
// Package: Core
// Module:  Exception
//

#include <stdio.h>
#include <exception>
#include <stdarg.h>
#include <string>
  
//custom exceptions  
  
class CMyException : public std::exception 
{  
public:  
    /// /brief Create exception object as copy of another
	CMyException( int iCode = 0 ) throw() ;

	explicit CMyException( const std::string& sMessage, int iCode = 0 ) throw() ;
	/// Creates an exception.

	virtual ~CMyException( void ) throw() ;
  
    /// /brief Assign another exception object's contents to this one  
    CMyException& operator=( const CMyException& rhs ) throw() ;
  
    /// /brief Returns explanation of why exception was thrown  
    virtual const char* what( void ) const throw() ;

	virtual const char* name( void ) const throw() ;

	const std::string& message( void ) const;
  
	std::string display_message( void ) const;

	const int	get_code( void ) const;

private:
    /// /brief explanation of why exception was thrown  
    std::string _sMessage; 
	int			_iCode;
};

#define DECLARE_EXCEPTION(exception_obj, exception_base)									\
class exception_obj: public exception_base													\
{																							\
public:																						\
	exception_obj( int iCode = 0 ) throw() :												\
	exception_base(iCode)	{}																\
																							\
	explicit exception_obj( const std::string& sMessage, int iCode = 0 ) throw() :			\
	exception_base(sMessage,iCode)	{}														\
																							\
	exception_obj& operator=(const exception_obj& rhs) throw()								\
	{																						\
		exception_base::operator=(rhs);														\
		return *this;																		\
	}																						\
	inline const char* name( void ) const throw()	{ return #exception_obj; }				\
};

DECLARE_EXCEPTION(SystemException,CMyException)
DECLARE_EXCEPTION(CommonException,CMyException)
DECLARE_EXCEPTION(LibException,CMyException)


#define except_ansi_snprintf	snprintf
#define except_ansi_vsprintf	vsprintf
#define except_ansi_vsnprintf	vsnprintf

#define	EXCEPT_BUF_MAX_SIZE	1024

#define EXCEPT_VA_ARGS_LIST(format)\
	char log_buffer[EXCEPT_BUF_MAX_SIZE] = {0};\
	va_list args;\
	va_start(args, format);\
	int iSize = except_ansi_vsprintf(log_buffer, format, args);\
	va_end(args);

std::string	get_message_string( const char *format, ... );

#define EXCEPTION_THROW(exception,format, ...)\
		throw exception(get_message_string(format,##__VA_ARGS__));
#endif
