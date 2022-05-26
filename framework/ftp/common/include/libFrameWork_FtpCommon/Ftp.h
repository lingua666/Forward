
#ifndef __FTP_20201009172804_1442199364_H__
#define __FTP_20201009172804_1442199364_H__

#include "External.h"

namespace _ftp_ {

#define FTP_STRING_MEMBER_DECL(name)		#name
#define FTP_MEMBER_GET_ENUM(name)			EN_CODE_ ## name
#define FTP_ENUM_MEMBER_DECL(name)			FTP_MEMBER_GET_ENUM(name)
#define FTP_ENUM_EVAL_MEMBER_DECL(name)		FTP_MEMBER_GET_ENUM(name) = name

#define FTP_MEMBER_RAW_I(macro)		macro

#if defined( PLATFORM_OS_FAMILY_WINDOWS )
#define FTP_MEMBER_RAW(macro)			FTP_MEMBER_RAW_I(macro)
#elif defined( PLATFORM_OS_FAMILY_UNIX )
#define FTP_MEMBER_RAW(macro)			macro
#endif

#define FTP_MEMBER_GET_FIELD(r, data, i, field)\
	MACRO_PP_SEPARATOR_IF(i) FTP_MEMBER_RAW( MACRO_PP_CAT(data, field) )

#define FTP_COMMAND_REQ_DECL(...)\
		static _ftp_string	g_ftp_req_name202010091137[] = {\
			MACRO_PP_SEQ_FOR_EACH_I(FTP_MEMBER_GET_FIELD, FTP_STRING_, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
		};\
		enum CmdCodeReq{\
			MACRO_PP_SEQ_FOR_EACH_I(FTP_MEMBER_GET_FIELD, FTP_ENUM_, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
		};


#define FTP_COMMAND_ACK_DECL(...)\
		static _ftp_string	g_ftp_ack_name202010091137[] = {\
			MACRO_PP_SEQ_FOR_EACH_I(FTP_MEMBER_GET_FIELD, FTP_STRING_, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
		};\
		enum CmdCodeAck{\
			MACRO_PP_SEQ_FOR_EACH_I(FTP_MEMBER_GET_FIELD, FTP_ENUM_EVAL_, MACRO_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
		};


	FTP_COMMAND_REQ_DECL(\
		MEMBER_DECL(NUL)/*系统登录的用户名*/, \
		MEMBER_DECL(USER)/*系统登录的用户名*/, \
		MEMBER_DECL(PASS)/*系统登录密码*/, \
		MEMBER_DECL(SYST)/*返回服务器使用的操作系统*/, \
		MEMBER_DECL(FEAT)/*请求FTP服务器列出它的所有的扩展命令与扩展功能*/, \
		MEMBER_DECL(PWD)/*显示当前工作目录*/, \
		MEMBER_DECL(CWD)/*改变服务器上的工作目录*/, \
		MEMBER_DECL(PASV)/*请求服务器等待数据连接(被动模式)*/, \
		MEMBER_DECL(PORT)/*IP地址和两字节的端口号(主动模式)*/, \
		MEMBER_DECL(LIST)/*获取文件列表*/, \
		MEMBER_DECL(DELE)/*删除服务器上的指定文件*/, \
		MEMBER_DECL(MKD)/*在服务器上建立指定目录*/, \
		MEMBER_DECL(RMD)/*在服务器上删除指定目录*/, \
		MEMBER_DECL(TYPE)/*数据类型(A=ASCII，E=EBCDIC，I=binary)*/, \
		MEMBER_DECL(RETR)/*从服务器上找回（复制）文件*/, \
		MEMBER_DECL(STOR)/*储存（复制）文件到服务器上*/, \
		MEMBER_DECL(QUIT)/*从 FTP 服务器上退出登录*/, \
		MEMBER_DECL(RNFR)/*对旧路径重命名*/, \
		MEMBER_DECL(RNTO)/*对新路径重命名*/, \
		MEMBER_DECL(CDUP)/*改变到父目录*/\
	);

	FTP_COMMAND_ACK_DECL(\
		MEMBER_DECL(150)/*打开连接*/, \
		MEMBER_DECL(200)/*系统状态回复*/, \
		MEMBER_DECL(202)/*系统状态回复*/, \
		MEMBER_DECL(211)/*系统状态回复*/, \
		MEMBER_DECL(215)/*系统类型回复*/, \
		MEMBER_DECL(220)/*Welcome*/, \
		MEMBER_DECL(221)/*退出网络*/, \
		MEMBER_DECL(226)/*结束数据连接*/, \
		MEMBER_DECL(227)/*进入被动模式（IP地址(h1,h2,h3,h4)，端口(p1, p2)）端口计算方式: p1*256+p2*/, \
		MEMBER_DECL(230)/*登录因特网*/, \
		MEMBER_DECL(250)/*文件行为完成*/, \
		MEMBER_DECL(257)/*路径名建立*/, \
		MEMBER_DECL(331)/*要求密码*/, \
		MEMBER_DECL(350)/*文件行为暂停*/, \
		MEMBER_DECL(421)/*服务关闭*/, \
		MEMBER_DECL(425)/*无法打开数据连接*/, \
		MEMBER_DECL(426)/*结束连接*/, \
		MEMBER_DECL(450)/*文件不可用*/, \
		MEMBER_DECL(451)/*遇到本地错误*/, \
		MEMBER_DECL(452)/*磁盘空间不足*/, \
		MEMBER_DECL(500)/*文件行为暂停*/, \
		MEMBER_DECL(501)/*错误参数*/, \
		MEMBER_DECL(502)/*命令没有执行*/, \
		MEMBER_DECL(503)/*错误指令序列*/, \
		MEMBER_DECL(504)/*无效命令参数*/, \
		MEMBER_DECL(530)/*未登录网络*/, \
		MEMBER_DECL(532)/*存储文件需要帐号*/, \
		MEMBER_DECL(550)/*文件不可用*/, \
		MEMBER_DECL(551)/*不知道的页类型*/, \
		MEMBER_DECL(552)/*超过存储分配*/, \
		MEMBER_DECL(553)/*错文件名不允许*/ \
	);

	typedef struct tagHead_INFO
	{
		bool	isReq;				//是否请求
		UInt16	CodeType;			//指令类型
	}tagHead_INFO;

	typedef struct tagHost_INFO
	{
		_ftp_string	sIP;
		UInt32		uPort;
	};

	class Ftp
	{
	public:
		Ftp(void);

		~Ftp(void);

		int Init(void);

		void Release(void);

		int Parse_Head(const char* c_szData, UInt32 uSize, tagHead_INFO* pOut_Info);

		_ftp_string	FillData(const tagHead_INFO& Info, const char* c_szParam = NULL, const char* c_szRemark = NULL);

		int Parse_Content(const char* c_szData, UInt32 uSize);

		_ftp_string ParseUSER(const char* c_szData, UInt32 uSize);

		_ftp_string ParsePASS(const char* c_szData, UInt32 uSize);

		_ftp_string ParsePWD(const char* c_szData, UInt32 uSize);

		_ftp_string ParseCWD(const char* c_szData, UInt32 uSize);

		_ftp_string ParseTYPE(const char* c_szData, UInt32 uSize);

		_ftp_string ParseMKD(const char* c_szData, UInt32 uSize);

		_ftp_string ParseRMD(const char* c_szData, UInt32 uSize);

		_ftp_string ParseDELE(const char* c_szData, UInt32 uSize);

		_ftp_string ParseCDUP(const char* c_szData, UInt32 uSize);

		_ftp_string ParseRETR(const char* c_szData, UInt32 uSize);

		_ftp_string ParseSTOR(const char* c_szData, UInt32 uSize);

		int ParsePASV(const char* c_szData, UInt32 uSize, tagHost_INFO* pOut_Host);

		_ftp_string ParseAckCWD(const char* c_szData, UInt32 uSize);

		bool isVaild(const char* c_szData, UInt32 uSize);

		_ftp_string Proto_Welcome(void);

		_ftp_string	Proto_USER(const char* c_szUserName);

		_ftp_string	Proto_PASS(const char* c_szPwd);

		_ftp_string	Proto_SYST(void);

		_ftp_string	Proto_FEAT(void);

		_ftp_string	Proto_PWD(void);

		_ftp_string	Proto_CWD(const char* c_szDir);

		_ftp_string	Proto_PASV(void);

		_ftp_string	Proto_PORT(void);

		_ftp_string	Proto_CDUP(void);

		_ftp_string	Proto_RMD(const char* c_szDir);
	
		_ftp_string	Proto_DELE(const char* c_szPath);

		_ftp_string	Proto_TYPE(const char* c_szType);

		//下载
		_ftp_string	Proto_RETR(const char* c_szPath);

		//上传
		_ftp_string	Proto_STOR(const char* c_szPath);

		//列表
		_ftp_string	Proto_LIST(void);

		//操作成功
		_ftp_string	Proto_150(const char* c_szRemark);

		//
		_ftp_string	Proto_200(const char* c_szRemark);

		//
		_ftp_string	Proto_202(const char* c_szRemark);

		//系统类型回复
		_ftp_string	Proto_215(const char* c_szRemark);

		//结束数据连接
		_ftp_string	Proto_226(const char* c_szRemark);

		//PASV 响应
		_ftp_string	Proto_227(tagHost_INFO* pOut_Host);

		//
		_ftp_string	Proto_230(const char* c_szRemark);

		//文件行为完成
		_ftp_string	Proto_250(const char* c_szRemark);

		//
		_ftp_string	Proto_257(const char* c_szRemark);

		//要求密码
		_ftp_string	Proto_331(const char* c_szRemark);

		//无法打开数据连接
		_ftp_string	Proto_425(const char* c_szRemark);

		// 文件不可用
		_ftp_string	Proto_450(const char* c_szRemark);

		// 该操作不允许
		_ftp_string	Proto_530(const char* c_szRemark);

		// 文件不可用
		_ftp_string	Proto_550(const char* c_szRemark);

	};

}

#endif

