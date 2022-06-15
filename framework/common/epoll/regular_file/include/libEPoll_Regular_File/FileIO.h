
#ifndef __EPOLL_REGULAR_FILE_FILEIO_20151202154125_1449042085_H__
#define __EPOLL_REGULAR_FILE_FILEIO_20151202154125_1449042085_H__

#include "io_service.h"

namespace	_epoll_regular_file_{

	#define INVALID_FILE	(-1)
	#define FILE_ERROR		(-1)

	#define FILE_INVALID(file, ret)\
		{\
			if(file == INVALID_FILE) \
				return ret; \
		}

	#define SOCKET_INVALID_ZERO(file)\
		{\
			if(file == INVALID_FILE) \
				return 0; \
		}

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

		int Bind( const HFILE& hFile );

		int Attach( const HFILE& hFile );

		int async_read( char* c_szBuf,
			UInt32 uSize, const HFNComplete& handle, UInt64 uOffset = 0 );

		int async_read( tagOverlapped* pOverlapped, UInt64 uOffset = 0 );

		int async_read( tagOverlapped* pOverlapped, char* pData, UInt32 u32Size, UInt64 uOffset = 0 );

		int async_write( const char* c_szBuf,
			UInt32 uSize, const HFNComplete& handle, UInt64 uOffset = 0 );

		int async_write( tagOverlapped* pOverlapped, UInt64 uOffset = 0 );

		void close( void );

		inline HFILE GetHandle( void ) const
		{
			return _hFile;
		}

		inline bool is_open( void ) const
		{
			return _hFile != INVALID_FILE;
		}

	protected:
		void	FreeOverlapped( tagOverlapped* p )
		{
			_OverlappedManage.Free(p);
		}

	private:
		HFILE		_hFile;
		io_service::io_service_sptr	_IOService_sptr;
		tagOverlapped*	_pRecv;
		CLock			_Lock;
		OverlappedManage<>	_OverlappedManage;
	};
	/** @} end FileIO */

} //_iocp_net_

#endif//__EPOLL_REGULAR_FILE_FILEIO_20151202154125_1449042085_H__
