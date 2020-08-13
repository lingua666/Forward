
#ifndef __UDP_STREAMCTRL_20150915155914_2442303954_H__
#define __UDP_STREAMCTRL_20150915155914_2442303954_H__

#include <libNetFrame_UDP_StreamSession/External.h>

namespace	_session_{

	namespace	_udp_{

		using namespace _io_net_::_udp_;
		typedef _io_net_::StreamBuf_ptr		StreamBuf_ptr;
		typedef _io_net_::_buf_hdr			_buf_hdr;
		typedef _io_net_::StreamBuf			StreamBuf;
		typedef _io_net_::_scoket_info		_scoket_info;

		typedef	function20_handle<void (void)>	HSendComplete;

		/*!
		* @class  StreamCtrl
		* @brief  StreamCtrl
		* @{
		*/

		class StreamCtrl
		{
		public:
			typedef	LinkList_type<MediaDBuf_ptr>	List_type;

		public:
			/*!
			* @function   StreamCtrl
			* @brief    
			* @return  
			*/
			StreamCtrl( UInt16 uSendSize,
						UInt16 ReservedSize );

			/*!
			* @function   ~StreamCtrl
			* @brief    
			* @return  
			*/
			~StreamCtrl( void );

			UInt32	Send( sockaddr_in* pDest, void* pAddtion, UInt8 u8AddSize,
						const char* pData, UInt16 u16Size );

			UInt32	Send( sockaddr_in* pDest, const char* pData, UInt16 u16Size );

			void	SendQue( void );

			void	HandleSend( const STREAM_HANDLE& Stream, sockaddr_in* pDest, 
				const char* pData, UInt32 u32Size );

			void	HandleRecv( const STREAM_HANDLE& Stream, sockaddr_in* pSrc,
				const StreamBuf_ptr& Buf_ptr );

			UInt32	GetBufSize( void ) const
			{
				return _Pack.GetPoolSize();
			}

			void Set_SendCompleteNotify( const HSendComplete& func )
			{
				_SendComplete = func;
			}

		protected:
			void	FullPacket( void* pUser, const StreamBuf_ptr& Buf_ptr );

			int	MorePack( sockaddr_in* pDest, const char* pData, UInt16 u16Size );

			int	OnePack( sockaddr_in* pDest, const char* pData, UInt16 u16Size );

			int	MorePack( sockaddr_in* pDest, void* pAddtion, UInt8 u8AddSize,
						const char* pData, UInt16 u16Size );

			int	OnePack( sockaddr_in* pDest, void* pAddtion, UInt8 u8AddSize,
						const char* pData, UInt16 u16Size );

			void	SendData( void );

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()
				PERMISSION_READ(sockaddr_in*, const StreamBuf_ptr&)
			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(SockStream)
				EVENT_REGISTER_READ(&StreamCtrl::HandleRecv)
				EVENT_REGISTER_WRITE(&StreamCtrl::HandleSend)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			/********************声明事件*****************************/
			EVENT_DECL_LIST_BEGIN()
				EVENT_DECL(SendEvent, sockaddr_in*, const char*, UInt32)
			EVENT_DECL_LIST_END()
			/********************声明事件*****************************/


			/********************关联事件*****************************/
			EVENT_ATTACH_LIST_BEGIN(SockStream)
				EVENT_ATTACH_HANDLE(SendEvent,&SockStream::PostSend)
			EVENT_ATTACH_LIST_END()
			/********************关联事件*****************************/
		private:
			HSendComplete	_SendComplete;
			bool			_isSending;
			CLock			_Lock;
			CLock			_SendLock;
			List_type		_SendList;
			MediaNPacket	_Pack;
		};
		/** @} end StreamCtrl */

	} //namespace	_tcp_

} // client


#endif//__MEDIASTREAMCTRL_20150915155914_2442303954_H__
