
#include <libFrameWork_FtpCommon/FtpList.h>

namespace _ftp_ {

	static char* g_szWeek20171221[] = { "Sun","Mon", "Tus", "Wed", "Thu", "Fri", "Sat" };
	static char* g_szMonth20171221[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	_string_type FtpList::GenerateString(const tagFile_INFO& Info)
	{
		_string_type s;
		if (Info.isDir)
			s += "d";
		else
			s += "-";

		Int8 iTimer = 3;
		do
		{
			if (Info.Permissions.isRead)
				s += "r";
			else
				s += "-";

			if (Info.Permissions.isWrite)
				s += "w";
			else
				s += "-";

			if (Info.Permissions.isExec)
				s += "x";
			else
				s += "-";

		} while (--iTimer > 0);

		s += "   ";
		s += _string_type::NumberToStr(Info.HardTime) + " ";
		s += Info.Ower;
		s += "      ";
		s += Info.Ower;
		s += "            ";
		s += _string_type::NumberToStr(Info.Size);
		s += " ";
		s += g_szMonth20171221[Info.Time.Month - 1];
		s += " ";
		s += _string_type::NumberToStr(Info.Time.Day);
		s += " ";
		s += _string_type::NumberToStr(Info.Time.Hour);
		s += ":";
		s += _string_type::NumberToStr(Info.Time.Minute);
		s += " ";
		s += Info.Name.gbk_utf8();

		return s;
	}

	FtpList::FtpList(void)
	{

	}

	FtpList::~FtpList(void)
	{
		Release();
	}

	int FtpList::Init(void)
	{
		return 1;
	}

	void FtpList::Release(void)
	{

	}

	FtpList::File_INFO_sptr FtpList::ParseFile(const char* szData, int iDataSize)
	{
		File_INFO_sptr	sptr = FtpList::Alloc();
		_string_type sFile = _string_type(szData, iDataSize).strim();
		if (sFile[0] == 'd')
			sptr->isDir = true;
		else
			sptr->isDir = false;

		if (sFile[1] == 'r')
			sptr->Permissions.isRead = true;
		else
			sptr->Permissions.isRead = false;

		if (sFile[2] == 'w')
			sptr->Permissions.isWrite = true;
		else
			sptr->Permissions.isWrite = false;

		if (sFile[3] == 'x')
			sptr->Permissions.isExec = true;
		else
			sptr->Permissions.isExec = false;

		_string_type sTmp = sFile.substr(sFile.find(' ', 4)).strim(" ");
		int iOffset = sTmp.find(' ');
		sptr->HardTime = _string_type::StringToInt(sTmp.substr(0, iOffset));
		sTmp = sTmp.substr(iOffset + 1).strim(" ");
		iOffset = sTmp.find(' ');
		sptr->Ower = sTmp.substr(0, iOffset);
		sTmp = sTmp.substr(iOffset + 1).strim(" ");
		iOffset = sTmp.find(' ');
		sTmp = sTmp.substr(iOffset + 1).strim(" ");
		iOffset = sTmp.find(' ');
		sptr->Size = _string_type::StringToInt(sTmp.substr(0, iOffset));

		sptr->Time.Year = 0;
		sTmp = sTmp.substr(iOffset + 1).strim(" ");
		iOffset = sTmp.find(' ');

		_string_type sMonth = sTmp.substr(0, iOffset);

		for (int i = 0; i < 12; i++)
		{
			if (sMonth == g_szMonth20171221[i])
			{
				sptr->Time.Month = i;
				break;
			}
		}

		sTmp = sTmp.substr(iOffset + 1).strim(" ");
		iOffset = sTmp.find(' ');
		sptr->Time.Day = _string_type::StringToInt(sTmp.substr(0, iOffset));

		sTmp = sTmp.substr(iOffset + 1).strim(" ");
		iOffset = sTmp.find(' ');
		_string_type sTime = sTmp.substr(0, iOffset);
		int iPos = sTime.find(':');
		sptr->Time.Hour = _string_type::StringToInt(sTime.substr(0, iPos));
		sptr->Time.Minute = _string_type::StringToInt(sTime.substr(iPos + 1));

		sptr->Name = sTmp.substr(iOffset + 1).strim(" ").utf8_gbk();

		//test
		printf("sptr->Name(%d):%s\r\n", sptr->Size, sptr->Name.c_str());
		//test

		return sptr;
	}

	FtpList::File_INFO_sptr FtpList::ParseLine(const char* szData, int iDataSize, int* pReadLen)
	{
		File_INFO_sptr	sptr;
		int iRet = -1;
		char* pStart = (char*)szData;
		char* pCur = strstr((char*)pStart, "\r\n");
		*pReadLen = -1;
		if (pCur != NULL)
		{
			sptr = ParseFile(pStart, (int)(pCur - pStart));
			*pReadLen = pCur - szData + 2;// 加上最后的"\r\n"结束符
		}

		return sptr;
	}

	int FtpList::Parse(const char* c_szData, UInt32 uSize, FileList_tpye* pList)
	{
		const char* pCur = c_szData;
		int iLen = uSize;
		int iPos = -1;
		do
		{
			File_INFO_sptr sptr = ParseLine(pCur, iLen, &iPos);
			iLen -= iPos;
			if (!sptr || iPos == -1 || iLen <= 0)
			{
				break;
			}
			else
			{
				pList->push_back(sptr);
			}
			pCur += iPos;
		} while (true);

		return 1;
	}

}