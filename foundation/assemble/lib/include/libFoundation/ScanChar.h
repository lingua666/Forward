
#ifndef __SCANCHAR_20160418173310_1438860785_H__
#define __SCANCHAR_20160418173310_1438860785_H__

#include <libString_v30/string.hpp>
#include <libMempool/MemPool.h>

namespace	_foundation_{

	typedef _string_v30_::string< MemPool_macro<400> >	_string_t;
	typedef _string_v30_::string< MemPool_macro<1600> >	_string_big_t;

	class ScanChar
	{
	public:
		static _string_t FindValueAndInc(char** szBuf, const _string_t& Start, const _string_t& End);

	public:
		ScanChar(void);

		~ScanChar(void);

		int Init(void);

		void Release(void);

		void AddString(const _string_t& sValue);

		void AddNumberCharacter(void);

		void AddHexNumberCharacter(void);

		void AddLetterCharacter(void);

		char GetChar(char** pSrc, bool isStop = false);

		_string_t GetString(char** pSrc, bool isStop = false);

	private:
		void AddRange(char cStart, char cEnd);

		void AddOne(char cStart);

		bool isMatch(char c);

	private:
		bool	_bMask[255];
	};


} //_foundation


#endif // __MODAFX_20160418173310_1438860785_H__
