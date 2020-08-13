
#ifndef __UDP_SOCKSTREAM_20150721142643_1437460003_H__
#define __UDP_SOCKSTREAM_20150721142643_1437460003_H__

#include <libNetFrame_UDP_Core/Socket.h>
#include <libNetFrame_Core_Common/Event_Handle.h>

namespace _io_net_{

	namespace _udp_{

		class SockStream;
		typedef SmartPTR<SockStream> STREAM_HANDLE;

		/*!
		* @class  SockStream
		* @brief   SockStream
		* @{
		*/

		class SockStream : public _enable_shared_from_this<SockStream>
		{
		public:
			typedef SockStream		self_type;

		public:
			static const STREAM_HANDLE Create( io_service* pIOService,
				UInt32 u32BufSize,
				UInt16 u16PostSize )
			{
				return STREAM_HANDLE( new SockStream(pIOService, u32BufSize, u16PostSize) );
			}
		public:
			SockStream( io_service* pIOService, UInt32 u32BufSize, UInt16 u16PostSize );

			~SockStream( void );

	#if defined( PLATFORM_OS_FAMILY_UNIX )
			inline void	Close( bool isDestory = false, bool isPassivity = false )
	#elif defined( PLATFORM_OS_FAMILY_WINDOWS )
			inline void	Close( bool isDestory = false )
	#endif
			{
				_Lock.Lock();
				if( _isOpen )
				{
					_isOpen = false;
					_Lock.UnLock();
				
				#if defined( PLATFORM_OS_FAMILY_UNIX )
					_Socket.Close(isPassivity);
				#elif defined( PLATFORM_OS_FAMILY_WINDOWS )
					_Socket.Close();
				#endif
					if( !isDestory )
					{
						EVENT_EXEC_CLOSE(shared_from_this());
					}
				}
				else
					_Lock.UnLock();
			}

			inline bool is_valid( void )
			{
				return _Socket.isOpen();
			}

			inline void Init( void )
			{
				_Socket.DefaultConfig();
				ReadScoketInfo();
				_isOpen = true;
			}

			inline int Run( void )
			{
				return PostRecv();
			}

			inline void ReadScoketInfo( void )
			{
				if( _Socket.isOpen() )
				{
					_Info.local_ip = _Socket.GetLocalIP();
					_Info.local_port = _Socket.GetLocalPort();
					_Info.local_node = _Socket.GetLocalNetNode();
					_Info.remote_ip = _Socket.GetRemoteIP();
					_Info.remote_port = _Socket.GetRemotePort();
					_Info.remote_node = _Socket.GetRemoteNetNode();
				}
			}

			inline const _scoket_info& GetSocketInfo( void ) const
			{
				return _Info;
			}

			inline _SOCKET_&	GetSocket( void )
			{
				return _Socket;
			}

			int		Bind( const _SOCKET_::HSOCKET& Socket );

			int		Attach( const _SOCKET_::HSOCKET& Socket );

			int		PostRecv( tagOverlapped* pOverlapped = NULL );

			void	PostSend( sockaddr_in* pAddr, const char* c_szData, UInt32 u32Size );

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()

				PERMISSION_READ(const STREAM_HANDLE&, sockaddr_in*, const StreamBuf_ptr&)
				PERMISSION_WRITE(const STREAM_HANDLE&, sockaddr_in*, const char*, UInt32)
				PERMISSION_EXCEPT(const STREAM_HANDLE&, const char* )
				PERMISSION_CLOSE(const STREAM_HANDLE& )

			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/

		protected:
			void	HandleRecv( tagIOData_INFO* pIOData );

			void	HandleSend( const char* pBuf, tagIOData_INFO* pIOData );


		private:
			const UInt32	_uBufSize;
			const UInt16	_uPostSize;
			const char*		_pRevBuf;
			_SOCKET_		_Socket;
			bool			_isOpen;
			MemPool_type		_Pool;
			_scoket_info	_Info;
			CLock			_Lock;
		};

		/** @} end SockStream */

	} //namespace _tcp_

} //_io_net_

#endif//__SOCKSTREAM_20150721142643_1437460003_H__

