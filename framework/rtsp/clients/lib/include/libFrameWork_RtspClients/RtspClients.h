
#ifndef __RTSPCLIENTS_20180104162504_3344199361_H__
#define __RTSPCLIENTS_20180104162504_3344199361_H__

#include "RtspClient_Impl.h"

class RtspClients
{
public:

	struct UrlHost_INFO
	{
		_string_type IP;
		_string_type Port;
	};

	typedef SmartPTR<RtspClient_Impl>	RtspClient_Impl_sptr;
	typedef Container_Map_type<_string_type, RtspClient_Impl_sptr>	ClientsMap;

	typedef RtspClient_Impl::HFNExceptNT	HFNExceptNT;
	typedef RtspClient_Impl::HFNSdpNT		HFNSdpNT;
	typedef RtspClient_Impl::HFNSuccessNT	HFNSuccessNT;
	typedef RtspClient_Impl::HFNRtpDataNT	HFNRtpDataNT;

	static int GetUrlHost( const _string_type& sUrl, UrlHost_INFO& HostInfo )
	{
		int iStart = sUrl.find("rtsp://");
		if( iStart == -1 )
			return -1;

		iStart = iStart + strlen("rtsp://");
		int iEnd = sUrl.find('@', iStart);
		if( iEnd != -1 )
		{//标准格式的用户名密码
			iStart = iEnd;
			iStart += 1;
		}

		iEnd = sUrl.find('/', iStart);
		_string_type sHost = sUrl.substr(iStart, iEnd - iStart);
		_string_type sIP;
		_string_type sPort;
		iStart = sHost.find(":");

		if(iStart == -1)
		{
			HostInfo.IP = sHost;
			HostInfo.Port = "554";
		}
		else
		{
			HostInfo.IP = sHost.substr(0, iStart);
			HostInfo.Port = sHost.substr(iStart + 1);
		}
		
		return 1;
	}

public:
	RtspClients( void );

	~RtspClients( void );

	void Init( UInt8 uRtspIOThreadNum = 1, UInt8 uRtspProcThreadNum = 1,
		UInt8 uRtpIOThreadNum = 1, UInt8 uRtpProcThreadNum = 1 );

	void Release( void );

	_string_type Open( const _string_type& sRtspPath, UInt8 uStreamType,
		UInt16 uRtpPort,HFNExceptNT hExcepNT = HFNExceptNT(), HFNSdpNT hSdpNT = HFNSdpNT(),
		HFNSuccessNT hSuccessNT = HFNSuccessNT(), HFNRtpDataNT hRtpDataNT = HFNRtpDataNT() );

	int Play( const char* c_szSessionID );

	int Stop( const char* c_szSessionID );

	void Close( const char* c_szSessionID );

	void SetResultEvent( const char* c_szSessionID, const HFNExceptNT& hExcepNT,
				const HFNSuccessNT& hSuccessNT = HFNSuccessNT() );

	void SetDataEvent( const char* c_szSessionID, const HFNRtpDataNT& hRtpDataNT, const HFNSdpNT& hSdpNT = HFNSdpNT() );

	_string_type GetMediaVideoHeadBuffer( const char* c_szSessionID );

	tagMediaVideoINFO* GetMediaVideoInfo( const char* c_szSessionID );

	TimerEvent* GetTimerEventPtr( void )
	{
		return GetTimerEventInstance();
	}

protected:
	void HandleExcept( const HFNExceptNT& hExcepNT,
		const char* c_szSessionID, Int32 iErrCode );

	void HandleSuccess( const HFNSuccessNT& hSuccess, 
		const char* c_szSessionID );

private:
	ClientsMap		_ClientsMap;
	MemPool_type	_Pool;
	CLock			_Lock;
	UInt32			_uSeq;
};

#endif
