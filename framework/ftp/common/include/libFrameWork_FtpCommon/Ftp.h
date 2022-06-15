
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
		MEMBER_DECL(NUL)/*ϵͳ��¼���û���*/, \
		MEMBER_DECL(USER)/*ϵͳ��¼���û���*/, \
		MEMBER_DECL(PASS)/*ϵͳ��¼����*/, \
		MEMBER_DECL(SYST)/*���ط�����ʹ�õĲ���ϵͳ*/, \
		MEMBER_DECL(FEAT)/*����FTP�������г��������е���չ��������չ����*/, \
		MEMBER_DECL(PWD)/*��ʾ��ǰ����Ŀ¼*/, \
		MEMBER_DECL(CWD)/*�ı�������ϵĹ���Ŀ¼*/, \
		MEMBER_DECL(PASV)/*����������ȴ���������(����ģʽ)*/, \
		MEMBER_DECL(PORT)/*IP��ַ�����ֽڵĶ˿ں�(����ģʽ)*/, \
		MEMBER_DECL(LIST)/*��ȡ�ļ��б�*/, \
		MEMBER_DECL(DELE)/*ɾ���������ϵ�ָ���ļ�*/, \
		MEMBER_DECL(MKD)/*�ڷ������Ͻ���ָ��Ŀ¼*/, \
		MEMBER_DECL(RMD)/*�ڷ�������ɾ��ָ��Ŀ¼*/, \
		MEMBER_DECL(TYPE)/*��������(A=ASCII��E=EBCDIC��I=binary)*/, \
		MEMBER_DECL(RETR)/*�ӷ��������һأ����ƣ��ļ�*/, \
		MEMBER_DECL(STOR)/*���棨���ƣ��ļ�����������*/, \
		MEMBER_DECL(QUIT)/*�� FTP ���������˳���¼*/, \
		MEMBER_DECL(RNFR)/*�Ծ�·��������*/, \
		MEMBER_DECL(RNTO)/*����·��������*/, \
		MEMBER_DECL(CDUP)/*�ı䵽��Ŀ¼*/\
	);

	FTP_COMMAND_ACK_DECL(\
		MEMBER_DECL(150)/*������*/, \
		MEMBER_DECL(200)/*ϵͳ״̬�ظ�*/, \
		MEMBER_DECL(202)/*ϵͳ״̬�ظ�*/, \
		MEMBER_DECL(211)/*ϵͳ״̬�ظ�*/, \
		MEMBER_DECL(215)/*ϵͳ���ͻظ�*/, \
		MEMBER_DECL(220)/*Welcome*/, \
		MEMBER_DECL(221)/*�˳�����*/, \
		MEMBER_DECL(226)/*������������*/, \
		MEMBER_DECL(227)/*���뱻��ģʽ��IP��ַ(h1,h2,h3,h4)���˿�(p1, p2)���˿ڼ��㷽ʽ: p1*256+p2*/, \
		MEMBER_DECL(230)/*��¼������*/, \
		MEMBER_DECL(250)/*�ļ���Ϊ���*/, \
		MEMBER_DECL(257)/*·��������*/, \
		MEMBER_DECL(331)/*Ҫ������*/, \
		MEMBER_DECL(350)/*�ļ���Ϊ��ͣ*/, \
		MEMBER_DECL(421)/*����ر�*/, \
		MEMBER_DECL(425)/*�޷�����������*/, \
		MEMBER_DECL(426)/*��������*/, \
		MEMBER_DECL(450)/*�ļ�������*/, \
		MEMBER_DECL(451)/*�������ش���*/, \
		MEMBER_DECL(452)/*���̿ռ䲻��*/, \
		MEMBER_DECL(500)/*�ļ���Ϊ��ͣ*/, \
		MEMBER_DECL(501)/*�������*/, \
		MEMBER_DECL(502)/*����û��ִ��*/, \
		MEMBER_DECL(503)/*����ָ������*/, \
		MEMBER_DECL(504)/*��Ч�������*/, \
		MEMBER_DECL(530)/*δ��¼����*/, \
		MEMBER_DECL(532)/*�洢�ļ���Ҫ�ʺ�*/, \
		MEMBER_DECL(550)/*�ļ�������*/, \
		MEMBER_DECL(551)/*��֪����ҳ����*/, \
		MEMBER_DECL(552)/*�����洢����*/, \
		MEMBER_DECL(553)/*���ļ���������*/ \
	);

	typedef struct tagHead_INFO
	{
		bool	isReq;				//�Ƿ�����
		UInt16	CodeType;			//ָ������
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

		//����
		_ftp_string	Proto_RETR(const char* c_szPath);

		//�ϴ�
		_ftp_string	Proto_STOR(const char* c_szPath);

		//�б�
		_ftp_string	Proto_LIST(void);

		//�����ɹ�
		_ftp_string	Proto_150(const char* c_szRemark);

		//
		_ftp_string	Proto_200(const char* c_szRemark);

		//
		_ftp_string	Proto_202(const char* c_szRemark);

		//ϵͳ���ͻظ�
		_ftp_string	Proto_215(const char* c_szRemark);

		//������������
		_ftp_string	Proto_226(const char* c_szRemark);

		//PASV ��Ӧ
		_ftp_string	Proto_227(tagHost_INFO* pOut_Host);

		//
		_ftp_string	Proto_230(const char* c_szRemark);

		//�ļ���Ϊ���
		_ftp_string	Proto_250(const char* c_szRemark);

		//
		_ftp_string	Proto_257(const char* c_szRemark);

		//Ҫ������
		_ftp_string	Proto_331(const char* c_szRemark);

		//�޷�����������
		_ftp_string	Proto_425(const char* c_szRemark);

		// �ļ�������
		_ftp_string	Proto_450(const char* c_szRemark);

		// �ò���������
		_ftp_string	Proto_530(const char* c_szRemark);

		// �ļ�������
		_ftp_string	Proto_550(const char* c_szRemark);

	};

}

#endif

