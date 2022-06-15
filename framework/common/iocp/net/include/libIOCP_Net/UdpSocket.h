
#ifndef __UDPSOCKET_20151202154125_1449042085_H__
#define __UDPSOCKET_20151202154125_1449042085_H__

#include "udp_io_service.h"
#include "UdpOverlappedManage.hpp"

namespace	_iocp_net_{

	namespace _udp_{

		#pragma pack(push,1)
		struct tagUdpSOCKET_INFO
		{
			char	_szRemote[20];
			UInt16	_u16RemotePort;
			char	_szLocal[20];
			UInt16	_u16LocalPort;
		};
		#pragma pack(pop)

		/*!
		* @class  UdpSocket
		* @brief   UdpSocket
		* @{
		*/

		class UdpSocket
		{
		public:
			typedef SOCKET	HSOCKET;
			typedef OverlappedManage<>	OverlappedManage_type;

		public:
			/*!
			* @function   UdpSocket
			* @brief    
			* @return  
			*/
			UdpSocket( const io_service& service );

			/*!
			* @function   ~UdpSocket
			* @brief    
			* @return  
			*/
			~UdpSocket( void );

			UdpSocket::HSOCKET Listen( UInt16 u16Port );

			int Bind( const HSOCKET& Sock );

			int Attach( const HSOCKET& Sock );

			int async_read( const WSABUF& wBuf, const HFNIOData& handle );

			int async_read( tagOverlapped* pOverlapped );

			int async_read( tagOverlapped* pOverlapped, char* pData, UInt32 u32Size );

			int async_write( sockaddr_in* pAddr, const WSABUF& wBuf, const HFNIOData& handle );
		
			int async_write( sockaddr_in* pAddr, tagOverlapped* pOverlapped );

			void close( void );

			inline HSOCKET GetSocket( void ) const
			{
				return _Sock;
			}

			inline const tagUdpSOCKET_INFO* GetInfo( void ) const
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
			HSOCKET		_Sock;
			io_service	_Service;
			tagUdpSOCKET_INFO	_tagInfo;
			OverlappedManage<>	_OverlappedManage;
		};
		/** @} end UdpSocket */

	} //_udp_

} //_iocp_net_

#endif//__UDPSOCKET_20151202154125_1449042085_H__
