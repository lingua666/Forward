
#ifndef __ISOCKET_20150820171514_1440062114_H__
#define __ISOCKET_20150820171514_1440062114_H__

#include <libNetFrame_Core/External.h>

namespace _io_net_{

	/*!
	* @class  ISocket
	* @brief   ISocket
	* @{
	*/

	class ISocket
	{
	public:
		/*!
		* @function   ~ISocket
		* @brief    
		* @return  
		*/
		virtual	~ISocket( void )
		{

		}

		virtual bool	Connect( const char* c_szIP, UInt16 u16Port) = 0;
		virtual void	Close( void ) = 0;
		//virtual endpoint_type	Bind( const char* c_szIP, UInt16 u16Port ) = 0;
		virtual const char*		GetRemoteIP( void ) = 0;
		virtual unsigned short	GetRemotePort( void ) = 0;
		virtual const char*		GetLocalIP( void ) = 0;
		virtual unsigned short	GetLocalPort( void ) = 0;
	};
	/** @} end ISocket */

} //_io_net_

#endif//__ISOCKET_20150820171514_1440062114_H__
