
#ifndef __CONFUSE_20210118173310_1438860785_H__
#define __CONFUSE_20210118173310_1438860785_H__

#include <libFoundation/ScanChar.h>
#include <libUUID/RandomStream.h>
#include <vector>

namespace	_foundation_{

	class Confuse
	{
	public:
		typedef std::vector<int>				ConfuseList;

	public:
		Confuse(void);

		~Confuse(void);

		int Init(void);

		void Release(void);

		_string_t	Generate(const char* c_szData, const ConfuseList& List, _string_t& Positon);

		_string_t	Decoder(const char* c_szData, const char* c_szPostion);

	private:
		char	_Letter[255];			//26个大写字母+26个小写字母+2个字符
	};


} //_foundation


#endif // __MODAFX_20160418173310_1438860785_H__
