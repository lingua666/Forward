
#ifndef __FTPCMDSESSION_20171226154204_3344199361_H__
#define __FTPCMDSESSION_20171226154204_3344199361_H__

#include "External.h"
#include "FtpDataSession.h"

namespace _ftp_ {

	class FtpCmdSession : public _enable_shared_from_this<FtpCmdSession>
	{
	public:
		typedef FtpCmdSession				self_type;
		typedef SmartPTR<FtpCmdSession>		self_sptr;
		typedef FtpDataSession::self_sptr	FtpDataSession_sptr;

		typedef FtpDataSession::HFNReady	HFNDataReady;
		typedef FtpDataSession::HFNRecv		HFNDataRecv;

		typedef struct tagSession_INFO
		{
			_ftp_string	sUser;
			_ftp_string	sPassword;
			_ftp_string	_sRootDir;		//windows下为windwos路径，linux下为unix路径
			_ftp_string	_sCurDir;		//无论为windows,linux都为unix路径
			bool		_isTemporary;	//是否临时会话
			NETHANDLE	hNet;
		}tagSession_INFO;

		static self_sptr Alloc(void);

	public:
		FtpCmdSession(const self_type& t1);

		FtpCmdSession(void);

		self_type& operator=(const self_type& t1);

		~FtpCmdSession(void);

		int Init(const _ftp_string& sLocalUrl, const _ftp_string& sUser,
			const _ftp_string& sPWD, bool isTemporary);

		void Release(void);

		bool Authentication(const _ftp_string& sUser, const _ftp_string& sPWD);

		int Open(NETHANDLE hNet, const _ftp_string& sRootDir);

		void Close(void);

		int Send(const char* c_szData, UInt32 uSize);

		_ftp_string GetUser(void);

		NETHANDLE	GetNet(void);

		bool IsLogin(void) const;

		bool IsTemporary(void) const;

	protected:
		void HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr);

		void HandleDataComplete(const FtpDataSession_sptr& sptr,
			const _ftp_string& sPath, FtpDataSession::enFTP_ACTION_TYPE Type);

		//void HandleSearchFile(_ftp_string* sData, bool isDir, const char* c_szPath);

		void HandleDataRecv(const _ftp_string& sPath, const FtpDataSession_sptr& sptr,
			NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr);

		_ftp_string NormcasePath(const _ftp_string& sDir);

	private:
		Ftp					_Ftp;
		FtpDataSession_sptr	_Data_sptr;
		tagSession_INFO		_SessionInfo;
		_ftp_string			_sLocalUrl;
		bool				_isLogin;
	};

}

#endif