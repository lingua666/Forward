
#ifndef __EXTERNAL_20171124162404_1442199360_H__
#define __EXTERNAL_20171124162404_1442199360_H__

#include <libFoundation/modafx.h>
#include <vector>

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#include <libIOCP_Net/acceptor.h>
	#include <libIOCP_Net/connector.h>
	#include <libIOCP_Net/TcpSocket.h>
	#include <libIOCP_Net/UdpSocket.h>
	#include <libCommon/os/winsock2i.h>
#elif defined(PLATFORM_OS_FAMILY_UNIX)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <libEPoll_Net/acceptor.h>
	#include <libEPoll_Net/connector.h>
	#include <libEPoll_Net/TcpSocket.h>
	#include <libEPoll_Net/UdpSocket.h>
#endif

#include <libNetFrame_Core_Common/Event_Handle.h>
#include <libNetFrame_Core_Common/NetNode.h>

namespace _io_net_{

#pragma pack(push, 1)
	struct _buf_hdr
	{
		UInt32		payload;	//数据大小
		char		data[1];	//数据内容地址
	};
#pragma pack(pop)

	typedef SmartPTR<_buf_hdr>	 StreamBuf_ptr;

	struct _scoket_info
	{
		_string_type local_ip;
		UInt16		local_port;
		NETHANDLE	local_node;
		_string_type remote_ip;
		UInt16		remote_port;
		NETHANDLE	remote_node;
	};

	struct StreamBuf
	{
		inline static int GetHdr( void )
		{
			return sizeof(_buf_hdr) - 1;
		}

		inline static char*	GetData( _buf_hdr* pDest )
		{
			return (char*)&pDest->data;
		}

		inline static	UInt32	GetSize( _buf_hdr* pDest )
		{
			return pDest->payload;
		}

		inline static	void	SetSize( _buf_hdr* pDest, UInt32 u32Size )
		{
			pDest->payload = u32Size;
		}

		inline static	_buf_hdr*	GetHdrToData( const char* pData )
		{
			return (_buf_hdr*)(pData - GetHdr());
		}

		inline static	UInt32	GetSizeToData( const char* pData ) 
		{
			return ((_buf_hdr*)(pData - GetHdr()))->payload;
		}

		inline static	void	SetSizeToData( const char* pData, UInt32 u32Size )
		{
			((_buf_hdr*)(pData - GetHdr()))->payload = u32Size;
		}

		inline static	_buf_hdr*	GetHeadToData( const char* pData )
		{
			return (_buf_hdr*)(pData - GetHdr());
		}
	};

} //namespace _io_net_

#endif//__EXTERNAL_20150914105604_1442199360_H__
