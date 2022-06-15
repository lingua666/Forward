
#ifndef __FTPSERVER_20171226154204_3344199361_H__
#define __FTPSERVER_20171226154204_3344199361_H__

#include "FtpCmdSession.h"
#include "libFrameWork_FtpSvr/FtpDataPorts.h"

namespace _ftp_ {

	class FtpServer
	{
	public:
		typedef FtpCmdSession::self_sptr		FtpCmdSession_sptr;

		TYPEDEF_LOCK_MULTI_INDEX_DECL(Map_type, _Map, FtpCmdSession_sptr,
			FUNC_INDEX_UNIQUE_ELEMENT(FtpCmdSession, _ftp_string, GetUser),
			FUNC_INDEX_UNIQUE_ELEMENT(FtpCmdSession, NETHANDLE, GetNet));

		typedef Container_HashMap_type<_ftp_string, FtpCmdSession_sptr>	FtpCmdSessionMap;

		typedef struct tagServer_INFO
		{
			UInt16	uCmdPort;		//指令端口
			_ftp_string	sUrl;		//Ftp Server IP
			_ftp_string	sRootDir;	//启动目录
		}tagServer_INFO;

	public:
		FtpServer(void)
		{
			
		}

		~FtpServer(void)
		{
			Release();
		}

		int Init(const _ftp_string& sUrl, const UInt16& uCmdPort,
			UInt16 uSDataPort, UInt16 uEDataPort, const _ftp_string& sRootDir)
		{
			//test
			LOG_Print_SetLog(libFrameWork_FtpServer, MLog_GetAllLevel());
			//test

			_Ftp.Init();
			GetTimerEventKMInstance()->Init();
			GetStreamSvrInstance()->Init(3);
			GetStreamSvrInstance_V20()->Init(3);
			GetAsyncFilesInstance()->Init();

			FtpDataPorts::GetFtpDataPortsInstance()->Init(uSDataPort, uEDataPort);

			_Info.sUrl = sUrl;
			_Info.uCmdPort = uCmdPort;
			_Info.sRootDir = sRootDir;
			return 1;
		}

		void Release(void)
		{
			GetStreamSvrInstance()->Release();
			GetTimerEventKMInstance()->Release();
			_Map.clear();
			_Ftp.Release();
		}

		int Run(UInt8 uIOThreadNum = 0, UInt8 uProcThreadNum = 0)
		{
			if (!GetStreamSvrInstance()->Listen(_Info.uCmdPort,
				function20_bind_Ex(&FtpServer::HandleAccept, this)))
				return -1;

			GetStreamSvrInstance()->Run(uIOThreadNum, true);
			return 1;
		}

		void	HandleAccept(const char* c_szIP, UInt16 u16Port, NETHANDLE hNet)
		{
			GetStreamSvrInstance()->SetRecvHandle(hNet, function20_bind_Ex(&FtpServer::HandleToken, this));
			if (GetStreamSvrInstance()->SetDestroyHandle(hNet,
				function20_bind_Ex(&FtpServer::HandleDestroy, this)) == 1)
			{
				_ftp_string s = _Ftp.Proto_Welcome();
				Send(hNet, s.c_str(), s.size());

				GetTimerEventKMInstance()->push_back(function20_bind(&FtpServer::HandleCheckHandle, this, hNet),
					5000);
			}
		}

		void HandleDestroy(NETHANDLE hNet)
		{
			FtpCmdSession_sptr sptr = MULTI_INDEX_FIND_FUNCTION(GetNet, hNet);
			if (sptr)
			{
				if(sptr->IsTemporary())
					MULTI_INDEX_REMOVE_FUNCTION(GetNet, hNet);
				sptr->Close();
			}
		}

		int Send(NETHANDLE hNet, const char* c_szData, UInt32 uSize)
		{
			return GetStreamSvrInstance()->Send(hNet, c_szData, uSize);
		}

		void HandleCheckHandle(NETHANDLE hNet)
		{
			FtpCmdSession_sptr sptr = MULTI_INDEX_FIND_FUNCTION(GetNet, hNet);
			if (!sptr)
			{//登录超时
				Close(hNet);
			}
		}

		void Close(NETHANDLE hNet)
		{
			GetStreamSvrInstance()->Close(hNet);
		}

		void HandleToken(NETHANDLE hNet, const StreamBuf_ptr& Stream_sptr)
		{
			if (Stream_sptr->payload > 0)
			{
				//test
				LOG_Print_Info(libFrameWork_FtpServer, "FtpServer::HandleToken(%d) Data:%s",
					Stream_sptr->payload, Stream_sptr->data);
				//test

				tagHead_INFO HeadInfo = { 0 };
				int iRet = _Ftp.Parse_Head(Stream_sptr->data, Stream_sptr->payload, &HeadInfo);
				if (iRet > 0)
				{
					if (HeadInfo.isReq)
					{//请求
						switch (HeadInfo.CodeType)
						{
						case FTP_MEMBER_GET_ENUM(USER):
							{
								_ftp_string UserName = _Ftp.ParseUSER(&Stream_sptr->data[iRet], Stream_sptr->payload - iRet);
								FtpCmdSession_sptr sptr = MULTI_INDEX_FIND_FUNCTION(GetUser, UserName);
								if (sptr)
								{
									if (!sptr->IsLogin())
									{//未登陆
										//替换会话
										FtpCmdSession_sptr Tmp_sptr = FtpCmdSession::Alloc();
										*(Tmp_sptr.get()) = *(sptr.get());
										Tmp_sptr->Open(hNet, _Info.sRootDir);
										MULTI_INDEX_REPLACE_FUNCTION(GetUser, UserName, Tmp_sptr);

										//必须密码
										_ftp_string sRemark = "Password required for ";
										sRemark += UserName;

										_ftp_string s = _Ftp.Proto_331(sRemark.c_str());
										Send(hNet, s.c_str(), s.size());
									}
									else
									{//已经登录
										_ftp_string s = _Ftp.Proto_230("Already logged - in.");
										Send(hNet, s.c_str(), s.size());

										//断开连接
										Close(hNet);
									}
								}
								else
								{//用户不存在
									_ftp_string s = _Ftp.Proto_530("Invalid user name");
									Send(hNet, s.c_str(), s.size());

									//断开连接
									Close(hNet);
								}
							}
							break;
						}
					}
				}
			}
		}

		int Stop(void)
		{
			return 1;
		}

		FtpServer::FtpCmdSession_sptr NewSession(const _ftp_string& SvrUrl,
			const _ftp_string& sUser, const _ftp_string& sPWD, bool isTemporary)
		{
			FtpCmdSession_sptr sptr = FtpCmdSession::Alloc();
			if (sptr)
			{
				sptr->Init(SvrUrl, sUser, sPWD, isTemporary);
				MULTI_INDEX_INSERT_FUNCTION(sptr);

				if (isTemporary)
				{//临时会话
					GetTimerEventKMInstance()->push_back(function20_bind(&FtpServer::HandleCheckSession, this, sUser),
						5000);
				}
			}

			return sptr;
		}

		void HandleCheckSession(const _ftp_string& sUser)
		{
			FtpCmdSession_sptr sptr = MULTI_INDEX_FIND_FUNCTION(GetUser, sUser);
			if (sptr)
			{
				if (sptr->GetNet() == -1)
				{
					RemoveSession(sUser);
				}
			}
		}

		void RemoveSession(const _ftp_string& sUser)
		{
			MULTI_INDEX_REMOVE_FUNCTION(GetUser, sUser);
		}

	protected:



	private:
		tagServer_INFO		_Info;
		Ftp					_Ftp;
	};

}

#endif

