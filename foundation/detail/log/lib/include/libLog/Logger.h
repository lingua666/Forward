
#ifndef __LOGGER_20151102222110_1446474070_H__
#define __LOGGER_20151102222110_1446474070_H__

#include <libLog/LogImpl.hpp>
#include <libLog/ConsoleLog.h>
#include <libTimestamp/Timestamp.h>
#include <libFile/CMyFile.h>
#include <libFile/CMyDirectory.h>
#include <libCommon/os/Types.h>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>

namespace	_log_{

	class LogIOStd
	{
	public:
		LogIOStd( void );

		~LogIOStd( void );

		_string_type GetTimer( void );

		void Print( const char* c_szMesage, const char* sz_Remark = NULL );

	private:
		CLock		_Lock;
	};

	class LogFile_Date
	{
	public:
		typedef LogFile_Date	self_type;

	public:
		LogFile_Date( void );

		LogFile_Date( const char* szFile );

		~LogFile_Date( void );

		void	Close( void );

		int OpenLog( const char* szFileName );

		int Open( const char* c_szFullPath );

		bool isOpen( void ) const
		{
			return _File.is_open();
		}

		void Print( const char* c_szMessage, const char* sz_Remark = NULL );

		bool CheckFile( void );

	private:
		_string_type GetTimer( void );

	private:
		File_type	_File;
		CLock		_Lock;
		struct tm			_CreateTime;
		_string_type	_sFilePath;
	};

	class LogFile_Capacity
	{
	public:
		typedef	LogFile_Capacity	self_type;

	public:
		LogFile_Capacity( UInt32 u32MaxCapacity );

		~LogFile_Capacity( void );

		void SetCapacity( UInt32 u32MaxCapacity );

		int Open( const char* c_szSavePath );

		int Close( void );

		void Print( const char* c_szMesage, const char* sz_Remark );

		bool CheckFile( void );

	private:
		File_type	_File;
		_string_type	_sFilePath;
		CLock	_Lock;
		UInt32	_u32Capacity;
		UInt32	_u32MaxCapacity;
	};

#define LOG_PRINF_DECL(n,remark)\
	if( _uLevel <= n )\
	{\
		VA_ARGS_LIST(format)\
		Print(log_buffer,remark);\
	}

	struct ILogger
	{
		virtual ~ILogger( void ) {}

		virtual void Debug(const char *format, ...) = 0;

		virtual void Info( const char *format, ... ) = 0;

		virtual void Warning( const char *format, ... ) = 0;

		virtual void Error( const char *format, ... ) = 0;

		virtual void FATAL( const char *format, ... ) = 0;
	};

	/*!
	* @class  Logger
	* @brief   Logger
	* @{
	*/

	template<typename T, int N = 0>
	class Logger : public ILogger
	{
	public:
		typedef Logger					self_type;
		typedef	SmartPTR<T>				Log_type_sptr;

		static	ILogger* GetInstance( const T& t1 )
		{
			return self_type(t1);
		}
	public:
		/*!
		* @function   Logger
		* @brief    
		* @return  
		*/
		Logger( const self_type& t1 )
		{
			_Log = t1._Log;
			_uLevel = t1._uLevel;
		}

		Logger( void )
			: _Log( new T() )
			, _uLevel(N)
		{

		}
		
		template<typename P>
		Logger( P p1 )
			: _Log( new T( p1 ) )
			, _uLevel(N)
		{
		
		}

		/*!
		* @function   ~Logger
		* @brief    
		* @return  
		*/
		~Logger( void )
		{

		}

		void SetLevel ( UInt8 uLevel )
		{
			_uLevel = uLevel;
		}

		int	Open( const char* szPath )
		{
			return _Log->Open(szPath);
		}

		void Close( void )
		{
			_Log->Close();
		}

		void Debug(const char *format, ...)
		{
			LOG_PRINF_DECL(1, "[Debug]")
		}

		void Info( const char *format, ... )
		{
			LOG_PRINF_DECL(2,"[Info]")
		}

		void Warning( const char *format, ... )
		{
			LOG_PRINF_DECL(3,"[Warning]")
		}

		void Error( const char *format, ... )
		{
			LOG_PRINF_DECL(4,"[Error]")
		}

		void FATAL( const char *format, ... )
		{
			LOG_PRINF_DECL(5,"[FATAL]")
		}

	private:
		void	Print( const char *format, const char* szRemark )
		{
			_Log->Print(format,szRemark);
		}
	private:
		UInt8			_uLevel;
		Log_type_sptr	_Log;
	};

	/** @} end Logger */

}

#endif//__LOGGER_20151102222110_1446474070_H__
