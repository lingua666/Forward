
#ifndef __FTPDATAPORTS_20171226154204_3344199361_H__
#define __FTPDATAPORTS_20171226154204_3344199361_H__

#include "External.h"

namespace _ftp_ {

	class FtpDataPorts
	{
	public:
		typedef struct tagConfig_INFO
		{
			UInt16	uStart;			//起始端口
			UInt16	uEnd;			//结束端口
			UInt16	uCurrent;		//当前端口
		};

		typedef _server_::_tcp_::StreamServer_V20::Stream_HAccept	Stream_HAccept;
		typedef LinkList_type<UInt16> Container_type;

		static FtpDataPorts* GetFtpDataPortsInstance(void)
		{
			static FtpDataPorts s_FtpDatas;
			return &s_FtpDatas;
		}

	public:
		FtpDataPorts(void);

		~FtpDataPorts(void);

		int Init(UInt16 uStart, UInt16 uEnd);

		void Release(void);

		NETHANDLE Open(const Stream_HAccept& hAccept);

		int Close(NETHANDLE hNet);

		UInt16 AllocPort(void);

		void FreePort(UInt16 uPort);

		UInt16 GetPort(NETHANDLE hNet);

	private:
		Container_type	_Container;
		CLock			_Lock;
	};

}

#endif
