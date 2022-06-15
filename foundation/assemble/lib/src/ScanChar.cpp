
#include <libFoundation/ScanChar.h>

namespace	_foundation_ {

	_string_t ScanChar::FindValueAndInc(char** szBuf, const _string_t& Start, const _string_t& End)
	{
		if (*szBuf == NULL || Start == "" || End == "")
			return "";

		const char cMask = '|';
		int iFilterCount = 0, iLen = 0;

		//�����ַ����еĽ�����������
		_string_t Filters[10];
		char* pFilterCur = (char*)End.c_str();
		char* pFilterEnd = NULL;
		do
		{
			pFilterEnd = strchr(pFilterCur, cMask);
			if (pFilterEnd == NULL)
			{
				Filters[iFilterCount] = pFilterCur;
				iFilterCount++;
				break;
			}
			else
			{
				iLen = (int)(pFilterEnd - pFilterCur);
				Filters[iFilterCount] = _string_t(pFilterCur, iLen);
				pFilterCur = pFilterCur + 1 + iLen;
				iFilterCount++;
			}

		} while (true);

		//����ƥ������
		char* pBegin = NULL;
		char* pTmp = NULL;
		if (Start == "SOF")
		{//������ʼλ��
			pBegin = *szBuf;
			pTmp = pBegin;
		}
		else
		{
			pBegin = strstr(*szBuf, Start.c_str());
			pTmp = (char*)(pBegin + Start.size());
		}

		if (pBegin != NULL)
		{
			for (int i = 0; i < iFilterCount; i++)
			{
				const char* pEnd = strstr(pTmp, Filters[i].c_str());
				if (pEnd != NULL)
				{
					const int iSize = (pEnd - pTmp);
					*szBuf = pTmp + iSize + Filters[i].size();
					return _string_t(pTmp, iSize);
				}
				else if (Filters[i] == "EOF")//��ʼ������
				{
					*szBuf += strlen(*szBuf);
					return	pTmp;
				}
			}
		}

		return "";
	}

	ScanChar::ScanChar(void)
	{
		Init();
	}

	ScanChar::~ScanChar(void)
	{
		Release();
	}

	int ScanChar::Init(void)
	{
		memset(_bMask, 0, sizeof(_bMask));
		return 1;
	}

	void ScanChar::Release(void)
	{

	}

	void ScanChar::AddString(const _string_t& sValue)
	{
		for (int i = 0; i < sValue.size(); i++)
		{
			AddOne(sValue[i]);
		}
	}

	void ScanChar::AddNumberCharacter(void)
	{
		AddRange('0', '9');
	}

	void ScanChar::AddHexNumberCharacter(void)
	{
		AddRange('0', '9');
		AddRange('a', 'f');
		AddRange('A', 'F');
	}

	void ScanChar::AddLetterCharacter(void)
	{
		AddRange('a', 'z');
		AddRange('A', 'Z');
	}

	char ScanChar::GetChar(char** pSrc, bool isStop)
	{
		if (*pSrc != NULL && *(*pSrc) != 0)
		{
			if (!isStop && isMatch(*(*pSrc)) || (isStop && !isMatch(*(*pSrc))))
			{
				return *((*pSrc)++);
			}

			(*pSrc)++;
		}

		return 0;
	}

	_string_t ScanChar::GetString(char** pSrc, bool isStop)
	{
		_string_t sRet;
		char c = 0;
		do
		{
			c = GetChar(pSrc, isStop);
			if (c == 0)
			{
				break;
			}
			else
			{
				sRet += c;
			}

		} while (true);

		return sRet;
	}

	void ScanChar::AddRange(char cStart, char cEnd)
	{
		for (char c = cStart; c <= cEnd; c++)
		{
			_bMask[c] = true;
		}
	}

	void ScanChar::AddOne(char cStart)
	{
		_bMask[cStart] = true;
	}

	bool ScanChar::isMatch(char c)
	{
		return _bMask[c];
	}

} //_foundation

