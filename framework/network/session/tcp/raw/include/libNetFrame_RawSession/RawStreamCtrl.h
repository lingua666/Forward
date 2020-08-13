
#ifndef __RAWSTREAMCTRL_20150915159858_1442303938_H__
#define __RAWSTREAMCTRL_20150915159858_1442303938_H__

#include <libNetFrame_RawSession/External.h>

namespace	_session_{

	namespace	_tcp_{

		using namespace _io_net_::_tcp_;
		typedef _io_net_::StreamBuf_ptr		StreamBuf_ptr;
		typedef _io_net_::_buf_hdr			_buf_hdr;
		typedef _io_net_::StreamBuf			StreamBuf;
		typedef _io_net_::_scoket_info		_scoket_info;

		/*!
		* @class  RawStreamCtrl
		* @brief   RawStreamCtrl
		* @{
		*/

		class RawStreamCtrl
		{
		public:
			typedef	LinkList_type<_buf_hdr*>	List_type;
			typedef	PLList< _buf_hdr*, CNoneLock >	PKBList;
		public:
			/*!
			* @function   RawStreamCtrl
			* @brief    
			* @return  
			*/
			RawStreamCtrl( UInt16 uSendSize,
						UInt16 ReservedSize );

			/*!
			* @function   ~RawStreamCtrl
			* @brief    
			* @return  
			*/
			~RawStreamCtrl( void );


			int	Send( const char* pData, UInt32 u32Size );

			void	SendQue( void );

			void	HandleSend( const STREAM_HANDLE& Stream, const char* pData, UInt32 u32Size );

			void	HandleRecv( const STREAM_HANDLE& Stream, const StreamBuf_ptr& Buf_ptr );

			UInt32	GetBufSize( void ) const
			{
				return _Pool.GetLength();
			}

		protected:
			_buf_hdr*	Clone( const char* pData, UInt32 u32Size );

			int	MorePack( const char* pData, UInt32 u32Size );

			int	OnePack( const char* pData, UInt32 u32Size );

			void	SendData( void );

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()
				PERMISSION_READ(const STREAM_HANDLE&, const StreamBuf_ptr&)
			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(SockStream)
				EVENT_REGISTER_READ(&RawStreamCtrl::HandleRecv)
				EVENT_REGISTER_WRITE(&RawStreamCtrl::HandleSend)
			EVENT_REGISTER_LIST_END()
			/********************事件注册*****************************/

			/********************声明事件*****************************/
			EVENT_DECL_LIST_BEGIN()
				EVENT_DECL(SendEvent, const char*, UInt32)
			EVENT_DECL_LIST_END()
			/********************声明事件*****************************/

			/********************关联事件*****************************/
			EVENT_ATTACH_LIST_BEGIN(SockStream)
				EVENT_ATTACH_HANDLE(SendEvent,&SockStream::PostSend)
			EVENT_ATTACH_LIST_END()
			/********************关联事件*****************************/

		private:
			MemPool_type		_Pool;
			MemPool_type		_ListPool;
			bool			_isSending;
			CLock			_Lock;
			CLock			_SendLock;
			List_type		_SendList;
			UInt16			_uSendSize;
		};
		/** @} end RawStreamCtrl */

	} //namespace	_tcp_

}	//namespace	_client_


#endif//__RAWSTREAMCTRL_20150915159858_1442303938_H__
