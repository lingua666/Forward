
#ifndef __MODAFX_20160418173310_1438860785_H__
#define __MODAFX_20160418173310_1438860785_H__

#include <libBoost_Wheel/Bst_Multi_index_container.hpp>
#include <libLock/Lock.hpp>
#include <libTimestamp/Timestamp.h>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSmart_ptr_v20/Shared_ptr.h>
#include <libFunction/CMyFunction.hpp>
#include <libFunction_v20/CMyFunction.hpp>
#include <libDesignMode/Singleton.hpp>
#include <libMessage/modafx.h>
#include <libFile/modafx.h>
#include <libLog/Logger.h>
#include <libThread/ThreadWrap.h>
#include <libContainer/LinkList.h>
#include <libContainer/CycleList.hpp>
#include <libContainer/Queue.hpp>
#include <libSignalsSlots/Signals.h>
#include <libContainer/CMyUnordered_map.hpp>
#include <libCommon/macro/Macro_variable.h>
#include <libCommon/macro/Macro_math.hpp>
#include <libCommon/modafx.h>
#include <libCommon/CASAtomic.h>
#include <libCommon/SysInfo.h>
#include <libCommon/digest/base64.hpp>
#include <libCommon/digest/sha1.hpp>
#include <libUUID/modafx.h>
#include <libFoundation/ScanChar.h>
#include <libFoundation/Confuse.h>
#include <libFoundation/MathChar.h>
#include <libFoundation/ObjectMap.hpp>


#define	List_marco			std::list

namespace	_foundation_{

	static _function_::Arg<1> _1;
	static _function_::Arg<2> _2;
	static _function_::Arg<3> _3;
	static _function_::Arg<4> _4;
	static _function_::Arg<5> _5;
	static _function_::Arg<6> _6;
	static _function_::Arg<7> _7;
	static _function_::Arg<8> _8;
	static _function_::Arg<9> _9;

	//执行shell指令
	inline _string_type	ShellExec(const char *pCmd, int& iRet, int Timestamp /*毫秒*/)
	{
#ifdef PLATFORM_OS_FAMILY_WINDOWS
		iRet = 0;
		return "";
#elif PLATFORM_OS_FAMILY_UNIX
		_string_type sResult;
		FILE *pp = popen(pCmd, "r");
		if (!pp)
		{
			iRet = -1;
			sResult = "error, cannot popen cmd: ";
			sResult += pCmd;
			return sResult;
		}

		Timestamp_type	Time;
		char tmp[512] = { 0 };
		while (fgets(tmp, sizeof(tmp), pp) != NULL)
		{
			if (tmp[strlen(tmp) - 1] == '\n')
			{
				tmp[strlen(tmp) - 1] = '\0';
			}

			sResult += tmp;

			if (Time.isElapsed(Timestamp * 1000))
			{//超时
				break;
			}
			else
			{
				sResult += "\r\n";
			}
		}

		int rv = pclose(pp);

		if (WIFEXITED(rv))
		{
			iRet = WEXITSTATUS(rv);
		}

		return sResult;
#endif // PLATFORM_OS_FAMILY_WINDOWS
	}

	inline Timestamp_type DateTimeStrinigTo(const char* c_szData, const char cDateTimeSep = ' ',
		const char cDateSep = '-', const char cTimeSep = ':')
	{
		typedef _foundation_::_string_t	_string_t;

		_string_t sData = _string_t(c_szData).strim();
		tm Time = { 0 };
		_string_t sDate;
		_string_t sTime;
		_string_t Separate;
		char* pBuffer = NULL;
		if (cDateTimeSep == 0 || cDateTimeSep == cDateSep)
		{//取固定日期长度
			UInt8 uLen = 0;
			if (cDateSep > 0)
				uLen = 2;//分隔符数量

			sDate.append(sData.c_str(), 8 + uLen);
			uLen = cDateTimeSep != 0 ? uLen + 1 : uLen;
			sTime.append(&sData[8 + uLen]);
		}
		else
		{
			pBuffer = (char*)sData.c_str();
			Separate = _string_t(&cDateTimeSep, 1);
			sDate = _foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", Separate);
			sTime = _foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", "EOF");
		}

		if (cDateSep != 0)
		{
			pBuffer = (char*)sDate.c_str();
			Separate = _string_t(&cDateSep, 1);
			Time.tm_year = _string_t::StringToInt(_foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", Separate)) - 1900;
			Time.tm_mon = _string_t::StringToInt(_foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", Separate)) - 1;
			Time.tm_mday = _string_t::StringToInt(_foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", "EOF"));
		}
		else
		{
			Time.tm_year = _string_t::StringToInt(_string_t(&sDate[0], 4)) - 1900;
			Time.tm_mon = _string_t::StringToInt(_string_t(&sDate[4], 2)) - 1;
			Time.tm_mday = _string_t::StringToInt(_string_t(&sDate[6]));
		}

		if (cTimeSep != 0)
		{
			pBuffer = (char*)sTime.c_str();
			Separate = _string_t(&cTimeSep, 1);
			Time.tm_hour = _string_t::StringToInt(_foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", Separate));
			Time.tm_min = _string_t::StringToInt(_foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", Separate));
			Time.tm_sec = _string_t::StringToInt(_foundation_::ScanChar::FindValueAndInc(&pBuffer, "SOF", "EOF"));
		}
		else
		{
			Time.tm_hour = _string_t::StringToInt(_string_t(&sTime[0], 2));
			Time.tm_min = _string_t::StringToInt(_string_t(&sTime[2], 2));
			Time.tm_sec = _string_t::StringToInt(_string_t(&sTime[4]));
		}

		return Timestamp_type::fromEpochTm(&Time);
	}

} //_foundation


#endif // __MODAFX_20160418173310_1438860785_H__
