
#ifndef __IO_SERVICE_REGULAR_20151201220617_1448978777_H__
#define __IO_SERVICE_REGULAR_20151201220617_1448978777_H__

#include "OverlappedManage.hpp"
#include "External.h"

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace _epoll_regular_file_{
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
			typedef io_service	self_type;
			typedef	SmartPTR<io_service>	io_service_sptr;

			typedef	LinkList_type<UInt64>	LList;
			typedef	SmartPTR< LList >		LList_sptr;
			typedef	LinkList_type<tagOverlapped*, CNoneLock>	OPList;
			typedef	SmartPTR< OPList >		OPList_sptr;
			
			typedef	SmartPTR<CLock>			CLock_sptr;
			
			typedef	OverlappedManage<>				OverlappedManage_type;
			typedef	SmartPTR< OverlappedManage_type >	OverlappedManage_sptr;

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
		
			//int post( const function_handle& f );

			int post_read( HANDLE Handle, tagOverlapped* pOverlapped );

			int post_write( HANDLE Handle, tagOverlapped* pOverlapped );

			int post_cancel( tagOverlapped* pOverlapped );

			int post_delete( HANDLE Handle );

			int post( HANDLE Handle, int iType, int iValue, tagOverlapped* pOverlapped );

			void process( tagIOData_INFO* pInfo, tagOverlapped* lpOverlapped, UInt32 Event = 0 );
		
			void run( void );

			int stop(void);

			int	bind( HFILE Handle );
			
			tagOverlapped* Alloc( void );

			void Free( tagOverlapped* pOverlapped );
			
			void CloseOverlapped( tagOverlapped* pOverlapped );
			
			int post( tagOverlapped* pOverlapped );
			
			int Immediate_post( tagOverlapped* pOverlapped );
			
			const OverlappedManage_sptr GetOverlappedManage( void )
			{
				return _OverlappedManage;
			}
			
			bool is_run( void ) const
			{
				return _is_run;
			}

			bool is_open( void ) const
			{
				return _io_handle != NULL ? true : false;
			}

		protected:
			/*int post( tagOverlapped* pOverlapped );

			int post( SOCKET sock, const WSABUF& wBuf, const function_handle& f );*/
			
		
			void	FreeOverlapped( tagOverlapped* p )
			{
				_OverlappedManage->Free(p);
			}

		private:
			bool		_is_run;
			IOHANDLE	_io_handle;
			LList_sptr		_ThreadList;
			OPList_sptr		_List;
			CLock_sptr		_Lock;
			CLock_sptr		_FreeLock;
			OverlappedManage_sptr	_OverlappedManage;
		};
		/** @} end io_service */

	} // _iocp_net_

#endif

#endif//__IO_SERVICE_20151201220617_1448978777_H__
