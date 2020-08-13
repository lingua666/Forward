
#ifndef __COMMANDSTREAMCTRL_20150915255858_1442303938_H__
#define __COMMANDSTREAMCTRL_20150915255858_1442303938_H__

#include <libNetFrame_CmdSession/External.h>

namespace	_session_{

	namespace	_tcp_{

		using namespace _io_net_::_tcp_;
		typedef _io_net_::StreamBuf_ptr		StreamBuf_ptr;
		typedef _io_net_::_buf_hdr			_buf_hdr;
		typedef _io_net_::StreamBuf			StreamBuf;
		typedef _io_net_::_scoket_info		_scoket_info;

		typedef	function20_handle<void (void)>		HSendComplete;

		/*!
		* @class  CommandStreamCtrl
		* @brief   CommandStreamCtrl
		* @{
		*/

		class CommandStreamCtrl
		{
		public:
			typedef	LinkList_type<CmdDBuf_ptr>	List_type;

		public:
			/*!
			* @function   CommandStreamCtrl
			* @brief    
			* @return  
			*/
			CommandStreamCtrl( UInt16 uSendSize,
							UInt16 ReservedSize );

			/*!
			* @function   ~CommandStreamCtrl
			* @brief    
			* @return  
			*/
			~CommandStreamCtrl( void );
		
			int	Send( void* pAddtion, UInt8 u8AddSize,
						const char* pData, UInt16 u16Size );

			int	Send( const char* pData, UInt16 u16Size );

			void	SendQue( void );

			void	HandleSend( const STREAM_HANDLE& Stream,
								const char* pData, UInt32 u32Size );

			void	HandleRecv( const STREAM_HANDLE& Stream,
								const StreamBuf_ptr& Buf_ptr );

			UInt32	GetBufSize( void ) const
			{
				return _Pack.GetPoolSize();
			}

			void	Set_SendCompleteNotify( const HSendComplete& func )
			{
				_SendComplete = func;
			}

		protected:
			void	FullPacket( const CmdDBuf_ptr& sptr );

			bool	MorePack( const char* pData, UInt16 u16Size );

			bool	OnePack( const char* pData, UInt16 u16Size );

			bool	MorePack( void* pAddtion, UInt8 u8AddSize,
						const char* pData, UInt16 u16Size );

			bool	OnePack( void* pAddtion, UInt8 u8AddSize,
						const char* pData, UInt16 u16Size );

			void	SendData( void );

			/********************事件注册权限*****************************/
			EVENT_PERMISSION_LIST_BEGIN()
				PERMISSION_READ(const CmdDBuf_ptr&)
			EVENT_PERMISSION_LIST_END()
			/********************事件注册权限*****************************/

			/********************事件注册*****************************/
			EVENT_REGISTER_LIST_BEGIN(SockStream)
				EVENT_REGISTER_READ(&CommandStreamCtrl::HandleRecv)
				EVENT_REGISTER_WRITE(&CommandStreamCtrl::HandleSend)
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
			HSendComplete	_SendComplete;
			bool			_isSending;
			CLock			_Lock;
			CLock			_SendLock;
			List_type		_SendList;
			CmdNPacket		_Pack;
		};
		/** @} end CommandStreamCtrl */

	} //namespace	_tcp_

}	//namespace	_client_


#endif//__COMMANDSTREAMCTRL_20150915255858_1442303938_H__
