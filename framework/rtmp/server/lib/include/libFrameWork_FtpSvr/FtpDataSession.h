
#ifndef __FTPDATASESSION_20171226154204_3344199361_H__
#define __FTPDATASESSION_20171226154204_3344199361_H__

#include "External.h"

namespace _ftp_ {

	class FtpDataSession : public _enable_shared_from_this<FtpDataSession>
	{
	public:
		typedef FtpDataSession		self_type;
		typedef SmartPTR<self_type>	self_sptr;
		typedef function20_handle<void(const self_sptr&)>		HFNReady;
		typedef function20_handle<void(const self_sptr&, NETHANDLE, const StreamBuf_ptr&)>	HFNRecv;

		typedef enum enFTP_ACTION_TYPE
		{
			EN_ACTION_INIT,			//初始化
			EN_ACTION_LIST,			//获取文件列表
			EN_ACTION_DOWNLOAD,		//下载
			EN_ACTION_UPLOAD,		//上传
		}enFTP_ACTION_TYPE;

		typedef function20_handle<void(const self_sptr&, const _ftp_string&, enFTP_ACTION_TYPE)>	HFNComplete;

		typedef struct tagSession_INFO
		{
			NETHANDLE hListen;
			NETHANDLE hData;
			UInt16	  uListenPort;
			enFTP_ACTION_TYPE	uType;
		}tagSession_INFO;

		static self_sptr Alloc(void);

	public:
		FtpDataSession(void);

		~FtpDataSession(void);

		int Init(void);

		void Release(void);

		UInt16 Open(void);

		void Close(void);

		int Send(const char* c_szData, UInt32 uSize);

		int Attach(const _ftp_string& sPath, const HFNComplete& hComplete, const enFTP_ACTION_TYPE Type);

		bool isVaild(void) const
		{
			return _Info.hListen == -1 ? false : true;
		}

		bool isOpen(void);

	protected:

		void CompleteNotify(const _ftp_string& sPath, FtpDataSession::enFTP_ACTION_TYPE Type);

		void HandleWaitSession(NETHANDLE hListen);

		void HandleDataCheck(NETHANDLE hListen);

		void HandleDataAccept(NETHANDLE hListen, const char* c_szClientIP,
			UInt16 uClientPort, NETHANDLE hNet);

		void HandleRecv(THandle hFile, NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr);

		void HandleDestroy(THandle hFile, NETHANDLE hNet);

		void GetFiles(const _ftp_string& sPath);

		void HandleSearchFile(_ftp_string* sData, bool isDir, const char* c_szPath);

		void DownFiles(THandle hFile, const _ftp_string& sPath);

		void HandleReadFile(THandle hFile, const char* c_szData, UInt32 uSize);

	private:
		tagSession_INFO	_Info;
		HFNReady		_hReady;
		HFNRecv			_hRecv;
		HFNComplete		_hComplete;
		Timestamp_type	_Time;
		_ftp_string		_sPath;
		TimerEventKM::_string_		_sEventUuid;
		bool			_isOpen;
	};

}

#endif
