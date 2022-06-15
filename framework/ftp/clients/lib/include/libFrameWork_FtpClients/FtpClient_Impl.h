
#ifndef __FTPCLIENT_IMPL_20201009174304_3344199361_H__
#define __FTPCLIENT_IMPL_20201009174304_3344199361_H__

#include "External.h"
#include "Simulation_Coroutine.h"

namespace _ftp_ {

	typedef enum enFtpCode
	{
		EN_CODE_NULL,					//��ʼ״̬
		EN_LOGIN_SUCCESS,				//��¼�ɹ�
		EN_CMD_SESSION_DESTROY,			//�Ự����

		EN_ERR_CMD_CONNECT = 0x8000,	//ָ��Ự����Connnect�ӿ�ʧ��
		EN_ERR_CMD_HCONNECT,			//ָ��Ự����Ŀ��ʧ��
		EN_ERR_CMD_SESSION_INIT,		//ָ��Ự��ʼ��ʧ��
		EN_ERR_CMD_SESSION_TIMEOUT,		//ָ��Ự��ʱ
		EN_ERR_DATA_CONNECT,			//���ݻỰ����Connnect�ӿ�ʧ��
		EN_ERR_DATA_HCONNECT,			//���ݻỰ����Ŀ��ʧ��
		EN_ERR_GET_FILES,				//��ȡ�ļ��б�ʧ��
		EN_ERR_UPLOAD,					//�ϴ��ļ�ʧ��
		EN_ERR_DOWNLOAD,				//�����ļ�ʧ��

	}enFtpCode;

	class FtpClient_Impl : public _enable_shared_from_this<FtpClient_Impl>
	{
	public:
		typedef struct tagLogin_INFO
		{
			_ftp_string sFtpIP;
			UInt16 uFtpPort;
			_ftp_string sUserName;
			_ftp_string sPwd;
		}tagLogin_INFO;

		typedef FtpClient_Impl				self_type;
		typedef SmartPTR<FtpClient_Impl>	self_sptr;
		typedef _session_::_tcp_::StreamBuf_ptr	StreamBuf_ptr;
		typedef FtpList::FileList_tpye		FileList_tpye;

		typedef function20_handle<void (const self_sptr&, int)>	HFNSubscribe;
		typedef function20_handle<void(const self_sptr&, const FileList_tpye&)>	HFNFilesSubscribe;
		typedef function20_handle<void(const char*, UInt32)>	HFNParse;
		typedef function20_handle<void (const _ftp_string&, const UInt16)>		HFNPASVExec;

	public:
		FtpClient_Impl(void);

		~FtpClient_Impl(void);

		int Init(const HFNSubscribe& hSubscribe, const HFNFilesSubscribe& hSubFiles);

		void Release(void);

		int Open(const _ftp_string& sFtpIP, const UInt16 uFtpPort,
			const _ftp_string& sUserName, const _ftp_string& sPwd);

		void Close(void);

		int Send(const char* c_szData, UInt32 uSize);

		int GetFiles(const _ftp_string& sDir);

		int UpLoad(const _ftp_string& sSrcPath, const _ftp_string& sSaveDir/*������·��*/);

		int DownLoad(const _ftp_string& sSrcPath, const _ftp_string& sSavePath/*����·��*/);

		int RemoveDir(const _ftp_string& sSrcDir);

		int DeleteFile(const _ftp_string& sSrcPath);

		enFtpCode	GetStatus(void);

		THandle	Get(void);

	protected:
		int GetFiles_Impl(const _ftp_string& sDir, const _ftp_string& sFtpIP, const UInt16 uFtpPort);

		int UpLoad_Impl(const _ftp_string& sSrcPath, const _ftp_string& sSaveDir/*������·��*/,
			const _ftp_string& sFtpIP, const UInt16 uFtpPort);

		int DownLoad_Impl(const _ftp_string& sSrcPath, const _ftp_string& sSavePath/*����·��*/,
			const _ftp_string& sFtpIP, const UInt16 uFtpPort);

		void CloseData(void);

		void Connect(const _ftp_string& sFtpIP, const UInt16 uFtpPort,
			const _ftp_string& sUserName, const _ftp_string& sPwd);

		void HandleConnect(NETHANDLE hNet);

		void HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain);

		void HandleRecv(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr, long lRemain);

		void HandleCheck(void);

		void HandleDestroy(NETHANDLE hNet);

		void DataConnect(const _ftp_string& sSvrIP, const UInt16 uSvrPort,
			const _ftp_string& sPath, CmdCodeReq Code);

		void HandleDataConnect(const _ftp_string& sSvrIP, UInt16 uSvrPort,
			const _ftp_string& sPath, CmdCodeReq Code, NETHANDLE hNet);

		void HandleDataRecv(CmdCodeReq Code, NETHANDLE hNet,
			const StreamBuf_ptr& Stream_sptr, long lRemain);

		void HandleDataDestroy(THandle hFile, NETHANDLE hNet);

		int HandleFileRead(NETHANDLE hNet, THandle Handle, const char* szData, UInt32 uSize);

		void HandleDownLoad(THandle hWrite, NETHANDLE hNet,
			const StreamBuf_ptr& Stream_sptr, long lRemain);

		void Publish(enFtpCode Code);

	private:
		enFtpCode		_Status;	//�Ƿ��Ѿ���¼
		CmdCodeReq		_CmdCode;	//ָ����
		NETHANDLE		_NetCmd;	//ָ����
		NETHANDLE		_NetData;	//���ݾ��
		tagLogin_INFO	_tagLogin;	//��¼��Ϣ
		_ftp_string		_CurDir;	//��ǰĿ¼
		HFNSubscribe	_hSubscribe;//�����¼�
		HFNFilesSubscribe _hSubFiles;	//�ļ��б����¼�
		HFNPASVExec		_Exec;		//��ִ��
		Simulation_Coroutine	_Coroutine; //ģ��Э��
		Ftp	_Ftp;
	};

}

#endif
