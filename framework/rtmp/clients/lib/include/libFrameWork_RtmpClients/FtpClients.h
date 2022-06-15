
#ifndef __FTPCLIENTS_20180104162504_3344199361_H__
#define __FTPCLIENTS_20180104162504_3344199361_H__

#include "FtpClient_Impl.h"

namespace _ftp_ {

	class FtpClients
	{
	public:
		struct UrlHost_INFO
		{
			_string_type IP;
			_string_type Port;
		};

		typedef SmartPTR<FtpClient_Impl>	FtpClient_Impl_sptr;
		typedef Container_Map_type<THandle, FtpClient_Impl_sptr>	ClientsMap;
		typedef FtpClient_Impl::FileList_tpye		FileList_tpye;
		typedef FtpClient_Impl::HFNSubscribe		HFNSubscribe;
		typedef FtpClient_Impl::HFNFilesSubscribe	HFNFilesSubscribe;

		static FtpClients::FtpClient_Impl_sptr Alloc(void);

	public:
		FtpClients(void);

		~FtpClients(void);

		int Init(const HFNSubscribe& hSubscribe, const HFNFilesSubscribe& hSubFiles);

		void Release(void);

		FtpClients::FtpClient_Impl_sptr	Open(const _ftp_string& sFtpIP, const UInt16 uFtpPort,
			const _ftp_string& sUserName, const _ftp_string& sPwd, bool isSync = false);

		int Close(const FtpClient_Impl_sptr& sptr);
		
		int CloseAll(void);

	protected:
		void HandleSubscribe(const FtpClient_Impl_sptr& sptr , int StatusCode);

	private:
		ClientsMap		_ClientsMap;
		MemPool_type	_Pool;
		CLock			_Lock;
		UInt32			_uSeq;
		HFNSubscribe	_hSubscribe;
		HFNFilesSubscribe _hSubFiles;
	};

}

#endif
