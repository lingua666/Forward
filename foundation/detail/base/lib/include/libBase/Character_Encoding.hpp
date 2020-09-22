
#ifndef __BASE_CHARACTER_ENCODING_20160419111505_1438860785_H__
#define __BASE_CHARACTER_ENCODING_20160419111505_1438860785_H__

#include <cctype>

# if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iconv.h>
# endif

namespace _base_{

	struct Charset
	{

# if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
		static bool gcc_utf8_gbk(char* pInBuf, size_t inLen, char* pOutBuf)
		{
			iconv_t cd = iconv_open("gbk", "utf-8");
			if (cd == 0)
			{
				printf("utf8_gbk iconv_open failed!! errorcode:%d\r\n", errno);
				return false;
			}

			size_t	outLen = inLen * 3;
			memset(pOutBuf, 0, outLen);

			bool isResult = true;
			if (iconv(cd, &pInBuf, &inLen, &pOutBuf, &outLen) == -1)
			{
				printf("utf8_gbk iconv failed!! errorcode:%d\r\n", errno);
				isResult = false;
			}
			iconv_close(cd);

			return isResult;
		}

		static bool gcc_gbk_utf8(char* pInBuf, size_t inLen, char* pOutBuf)
		{
			iconv_t cd = iconv_open("utf-8", "gbk");
			if (cd == 0)
			{//
				printf("gbk_utf8 iconv_open failed!! errorcode:%d\r\n", errno);
				return false;
			}

			size_t	outLen = inLen * 3;
			memset(pOutBuf, 0, outLen);

			bool isResult = true;
			if (iconv(cd, &pInBuf, &inLen, &pOutBuf, &outLen) == -1)
			{
				printf("gbk_utf8 iconv failed!! errorcode:%d\r\n", errno);
				isResult = false;
			}
			iconv_close(cd);

			return isResult;
		}
# endif

	};
}


#endif

