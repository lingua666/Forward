
#ifndef __IOCPFILE_IO_SERVICE_20151201220617_1448978777_H__
#define __IOCPFILE_IO_SERVICE_20151201220617_1448978777_H__

#include "OverlappedManage.hpp"
#include "External.h"

namespace _iocp_file_{

	/*!
	* @class  io_service
	* @brief   io_service
	* @{
	*/
	typedef	HANDLE	HFILE;
	typedef	HANDLE	IOHANDLE;

	class io_service
	{
	public:
		typedef io_service				self_type;
		typedef	SmartPTR<io_service>	io_service_sptr;

	public:
		/*!
		* @function   io_service
		* @brief    
		* @return  
		*/
		io_service( void );

		io_service( const self_type& self );

		/*!
		* @function   ~io_service
		* @brief    
		* @return  
		*/
		~io_service( void );
		
		io_service& operator=( const self_type& self );

		int Init(void);

		void Release(void);

		int open( void );

		int close( void );
		
		int post( const function_handle& f );
		
		void run( void );

		int stop(void);

		int	bind( HFILE Handle );

		tagOverlapped* AllocOverlapped( void );

		void	FreeOverlapped( tagOverlapped* pOverlapped );

		bool is_run( void ) const
		{
			return _is_run;
		}

		bool is_open( void ) const
		{
			return _io_handle != NULL ? true : false;
		}

	protected:
		int post( tagOverlapped* pOverlapped );

		int post( HFILE hFile, const tagIOWSABuf& wBuf, const function_handle& f );

	private:
		bool		_is_run;
		IOHANDLE	_io_handle;
		OverlappedManage<>	_OverlappedManage;
	};
	/** @} end io_service */
} // _iocp_net_

#endif//__IO_SERVICE_20151201220617_1448978777_H__
