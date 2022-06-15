
#ifndef __INETACCEPTOR_20150820225114_1440082274_H__
#define __INETACCEPTOR_20150820225114_1440082274_H__

#include <libNetFrame_Core_Common/Event_Handle.h>
#include <libNetFrame_Core/SockStream.h>

namespace _io_net_{

	namespace _tcp_{

		/*!
		* @class  INetAcceptor
		* @brief   INetAcceptor
		* @{
		*/

		class INetAcceptor
		{
		public:
			/*!
			* @function   ~INetAcceptor
			* @brief    
			* @return  
			*/
			virtual	~INetAcceptor( void )
			{

			}

			virtual bool	Listen( UInt16 u16Port, const char* c_szIP) = 0;

			virtual	void	Stop( void ) = 0;
			virtual void	Run( UInt8 uNum ) = 0;
			virtual void	RunLoop( UInt8 uNum ) = 0;

			/********************�¼�ע��Ȩ��*****************************/
			//�����ⲿע�ᡰ����д���쳣���ر�"���¼�Ȩ��
			EVENT_PERMISSION_LIST_BEGIN()
				//PERMISSION_ACCEPT( const STREAM_HANDLE& )
				PERMISSION_ACCEPT( const _SOCKET_::HSOCKET& )
				PERMISSION_EXCEPT( const STREAM_HANDLE&, const char* )
				//PERMISSION_CLOSE()
			EVENT_PERMISSION_LIST_END()
			/********************�¼�ע��Ȩ��*****************************/
		};
		/** @} end INetAcceptor */

	} //namespace _tcp_

} //_io_net_


#endif//__INETACCEPTOR_20150820225114_1440082274_H__
