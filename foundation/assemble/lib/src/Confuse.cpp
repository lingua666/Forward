
#include <libFoundation/Confuse.h>

namespace	_foundation_ {

	Confuse::Confuse(void)
	{
		memset(_Letter, 0, sizeof(_Letter));
	}

	Confuse::~Confuse(void)
	{
		Release();
	}

	int Confuse::Init(void)
	{
		int i = 0;

		//´óÐ´×ÖÄ¸
		for (i = 0; i < 26; i++)
		{
			_Letter[i] = 'A' + i;
		}

		//Ð¡Ð´×ÖÄ¸
		for (i = 26; i < 52; i++)
		{
			_Letter[i] = 'a' + (i - 26);
		}

		//Êý×Ö
		for (i = 52; i < 52 + 10; i++)
			_Letter[i] = '0' + (i - 52);

		//·ûºÅ
		_Letter[i] = '@';
		_Letter[i + 1] = '#';

		return 1;
	}

	void Confuse::Release(void)
	{
		memset(_Letter, 0, sizeof(_Letter));
	}

	_string_t	Confuse::Generate(const char* c_szData, const ConfuseList& List, _string_t& Positon)
	{
		_string_t sRet;
		int i = 0;
		for (i = 0; i < strlen(c_szData); i++)
		{
			for (int j = 0; j < strlen(_Letter); j++)
			{
				if (c_szData[i] == _Letter[j])
				{
					for (int k = 0; k < List[i]; k++)
					{
						_uuid_::RandomInputStream ris;
						UInt32 uValue = 0;
						ris.read((char*)&uValue, sizeof(uValue));
						char c = uValue % 16;
						if (c > 9)
							sRet += 'a' + (c - 10);
						else
							sRet += '0' + c;
					}

					if (i != 0)
						Positon += ",";

					Positon += _string_t::NumberToStr(sRet.size());
					sRet += _string_t::lFixation(_string_t::NumberToStr(j), 2, '0');
					break;
				}
			}
		}

		return sRet;
	}

	_string_t	Confuse::Decoder(const char* c_szData, const char* c_szPostion)
	{
		_string_t sRet;
		_foundation_::ScanChar::ContentList	List;
		_foundation_::ScanChar::FindValues(c_szPostion, ",", &List);

		while (List.size() > 0)
		{
			sRet += _Letter[_string_t::StringToInt(_string_t(&c_szData[_string_t::StringToInt(List.pop_front().c_str())], 2))];
		};

		return sRet;
	}

} //_foundation

