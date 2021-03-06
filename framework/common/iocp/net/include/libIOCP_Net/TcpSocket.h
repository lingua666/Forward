
#ifndef __TCPSOCKET_20151202154125_1449042085_H__
#define __TCPSOCKET_20151202154125_1449042085_H__

#include "tcp_io_service.h"
#include "TcpOverlappedManage.hpp"

namespace	_iocp_net_{

	namespace _tcp_{

#pragma pack(push,1)
		struct tagSOCKET_INFO
		{
			char	_szRemote[20];
			UInt16	_u16RemotePort;
			char	_szLocal[20];
			UInt16	_u16LocalPort;
		};
#pragma pack(pop)

		/*!
		* @class  TcpSocket
		* @brief   TcpSocket
		* @{
		*/

		class TcpSocket
		{
		public:
			typedef SOCKET				HSOCKET;
			typedef OverlappedManage<>	OverlappedManage_type;

		public:
			/*!
			* @function   TcpSocket
			* @brief    
			* @return  
			*/
			TcpSocket( const io_service& service );

			/*!
			* @function   ~TcpSocket
			* @brief    
			* @return  
			*/
			~TcpSocket( void );

			TcpSocket::HSOCKET Connect( const _string_type& sIP, UInt16 u16Port );

			int Bind( const HSOCKET& Sock );

			int Attach( const HSOCKET& Sock );

			int async_read( const WSABUF& wBuf, const HFNIOData& handle );

			int async_read( tagOverlapped* pOverlapped );

			int async_read( tagOverlapped* pOverlapped, char* pData, UInt32 u32Size );

			int async_write( const WSABUF& wBuf, const HFNIOData& handle );
		
			int async_write( tagOverlapped* pOverlapped );

			void close( void );

			inline HSOCKET GetSocket( void ) const
			{
				return _Sock;
			}

			inline const tagSOCKET_INFO* GetInfo( void ) const
			{
				return &_tagInfo;
			}

			inline bool is_open( void ) const
			{
				return _Sock != INVALID_SOCKET;
			}
		protected:
			int	ReadInfo( const HSOCKET& Sock );

			void	FreeOverlapped( tagOverlapped* p )
			{
				_OverlappedManage.Free(p);
			}
		private:
			HSOCKET					_Sock;
			io_service				_Service;
			tagSOCKET_INFO			_tagInfo;
			OverlappedManage_type	_OverlappedManage;
		};
		/** @} end TcpSocket */

	} //namespace _tcp_

} //_iocp_net_

#endif//__TCPSOCKET_20151202154125_1449042085_H__
