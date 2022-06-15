
#ifndef __FTPLIST_20201009172804_1442199364_H__
#define __FTPLIST_20201009172804_1442199364_H__

#include "External.h"

namespace _ftp_ {

	class FtpList
	{
	public:
		/*typedef struct tagTime_INFO
		{
			UInt16	Year;
			UInt8	Month;
			UInt8	Day;
			UInt8	Hour;
			UInt8	Minute;
			UInt8	Second;
		}tagTime_INFO;

		typedef struct tagPermissions_INFO
		{
			bool isRead;			//��Ȩ��
			bool isWrite;			//дȨ��
			bool isExec;			//ִ��Ȩ��
		}tagPermissions_INFO;

		typedef struct tagFile_INFO
		{
			bool isDir;						//�Ƿ�Ŀ¼
			tagPermissions_INFO	Permissions;//�ļ�Ȩ��
			_ftp_string			HardTime;	//Ӳ���Ӵ���
			_ftp_string			Ower;		//�ļ�ӵ����
			UInt64				Size;		//�ļ���С
			tagTime_INFO		Time;		//�ļ���������
			_ftp_string			Name;		//�ļ���
		}tagFile_INFO;*/

		typedef _file_::CMyFile::tagFile_INFO tagFile_INFO;

		typedef SmartPTR<tagFile_INFO>			File_INFO_sptr;
		typedef LinkList_type<File_INFO_sptr>	FileList_tpye;

		static File_INFO_sptr Alloc( void )
		{
			static MemPool_type	s_Pool(sizeof(tagFile_INFO));
			return File_INFO_sptr(s_Pool.AllocObj<tagFile_INFO>(),
				function20_bind_Ex(&MemPool_type::FreeObj<tagFile_INFO>, &s_Pool));
		}

		static _string_type GenerateString(const tagFile_INFO& Info);
			
	public:
		FtpList(void);

		~FtpList(void);

		int Init(void);

		void Release(void);

		File_INFO_sptr ParseFile(const char* szData, int iDataSize);

		File_INFO_sptr ParseLine(const char* szData, int iDataSize, int* pReadLen);

		int Parse(const char* c_szData, UInt32 uSize, FileList_tpye* pList);
	};

}

#endif

