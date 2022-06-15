
#ifndef __IOCPFILE_FILEIO_20151202154125_1449042085_H__
#define __IOCPFILE_FILEIO_20151202154125_1449042085_H__

#include "io_service.h"

namespace	_iocp_file_{

	/*!
	* @class  FileIO
	* @brief   FileIO
	* @{
	*/

	class FileIO
	{
	public:
		typedef HANDLE	HFILE;

	public:
		/*!
		* @function   FileIO
		* @brief    
		* @return  
		*/
		FileIO( const io_service::io_service_sptr& IOServer_sptr );

		/*!
		* @function   ~FileIO
		* @brief    
		* @return  
		*/
		~FileIO( void );

		int Bind( const HFILE& Handle );

		int Attach( const HFILE& Handle );

		int async_read( char* c_szBuf,
					UInt32 uSize,
					const function_handle& hFn, 
					UInt64 uOffset = 0 );

		int async_read( tagOverlapped* pOverlapped, UInt64 uOffset = 0 );

		int async_read( tagOverlapped* pOverlapped,
					char* pData, UInt32 u32Size,
					UInt64 uOffset = 0 );

		int async_write( const char* c_szBuf,
					UInt32 uSize,
					const function_handle& hFn,
					UInt64 uOffset = 0 );
		
		int async_write( tagOverlapped* pOverlapped, UInt64 uOffset = 0 );

		void close( void );

		int seek_read(Int64 uOffset);

		Int64 tell_read(void);

		inline HFILE GetHandle( void ) const
		{
			return _hFile;
		}

		inline bool is_open( void ) const
		{
			return _hFile != NULL;
		}
	private:
		HFILE		_hFile;
		io_service::io_service_sptr	_IOService_sptr;
	};
	/** @} end FileIO */

} //_iocp_net_

#endif//__TCPSOCKET_20151202154125_1449042085_H__
