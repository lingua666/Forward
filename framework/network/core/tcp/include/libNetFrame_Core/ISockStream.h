
#ifndef __ISOCKSTREAM_20150821091049_1440119449_H__
#define __ISOCKSTREAM_20150821091049_1440119449_H__

namespace _io_net_{

	namespace _tcp_{

		/*!
		* @class  ISockStream
		* @brief   ISockStream
		* @{
		*/

		class ISockStream
		{
		public:
			/*!
			* @function   ~ISockStream
			* @brief    
			* @return  
			*/
			virtual ~ISockStream( void )
			{}
	
			virtual void	Close( void ) = 0;
	
			virtual _SOCKET_*	GetHandle( void ) const = 0;

			virtual bool	PostRecv( void ) = 0;

			virtual bool	PostSend( const char* c_szData, UInt32 u32Size ) = 0;

			virtual NETHANDLE	GetNetNode( void ) = 0;

		};
		/** @} end ISockStream */

	} //namespace _tcp_

} //_io_net_



#endif//__ISOCKSTREAM_20150821091049_1440119449_H__
