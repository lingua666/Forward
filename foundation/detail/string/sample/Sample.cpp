// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libString/string.hpp>
#include <libTimestamp/Timestamp.h>

typedef _string_::string	string_type;

void str_Trim( const string_type &str, const string_type& value ) 
{
	string_type s(str);
	s.erase(0,s.find_first_not_of(value.c_str()));
	s.erase(s.find_last_not_of(value.c_str()) + 1);
}

void str_Trim_s( const std::string& str, const std::string& value ) 
{
	std::string s(str);
	s.erase(0,s.find_first_not_of(value.c_str()));
	s.erase(s.find_last_not_of(value.c_str()) + 1);
}

int main(int argc, char* argv[])
{
	_string_type stype = "车道指示器";
	_string_type sey = stype.gbk_utf8();
	const char* ps = sey.c_str();
	sey.append(1,'<');

	_string_type sey1 = sey.utf8_gbk();

	/*HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hdl, FOREGROUND_YELLOW | FOREGROUND_INTENSITY);*/
	
	/*printf("Hello ");
	SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("world!\n");*/

	//int com = string_type::stringCompareIgnoreCase("abcdrewrewrwerwrsdfsdf","AbCDwrewrwerwrsdfsdf");

	/*std::string ss1;
	ss1.append(1,'\0');
	ss1.append("a");
	ss1.append(1,'\0');
	str_Trim_s(ss1,"");*/

	//void* pp = std::memchr(ss1.data(),'\0',1);

	/*string_type s;
	s.append(1,'\0');
	s.append("a");
	s.append(1,'\0');
	string_type se2 = s.strim("");
	s.append("12");

	string_type s21(1,'a');

	std::string sss1("1234");
	sss1.append("12");
	sss1.append("888");
	sss1.replace(0, 2, "888888");*/

	int num = 100000000;

	while(true)
	{
		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s;
				s.resize(100);
				//s.resize(50);
			}
			printf("string_type12 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s;
				s.resize(100);
				//s.resize(50);
			}
			printf("std::string12 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		//while(true)
		{
			{
				string_type s;
				string_type s1("123456789");
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					s = "123465679897987985646546321321";
				}
				printf("string_type6 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}

			{
				std::string s;
				std::string s1("123456789");
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					s = "123465679897987985646546321321";
				}
				printf("std::string6 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s("123456789123456789789789");
			}
			printf("string_type time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s("123456789123456789789789");
			}
			printf("std::string time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s1 = s;
				s1.append("1234567894324234234234234234");
			}
			printf("string_type1 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s1 = s;
				s1.append("1234567894324234234234234234");
			}
			printf("std::string1 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s1 = s;
			}
			printf("string_type2 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s1 = s;
			}
			printf("std::string2 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s.c_str();
			}
			printf("string_type3 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s.c_str();
			}
			printf("std::string3 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}


		{
			string_type s1;
			string_type s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s1 = s;
			}
			printf("string_type4 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s1;
			std::string s("12345678923456789456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s1 = s;
			}
			printf("std::string4 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		//while(true)
		{
			{
				string_type s;
				string_type s1("123456789");
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					s = string_type("123465679897987985646546321321");
				}
				printf("string_type5 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}

			{
				std::string s;
				std::string s1("123456789");
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					s = std::string("123465679897987985646546321321");
				}
				printf("std::string5 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s("12346557989621313132165465432131313123");
				s.append("123465579896");
				s.append("123465579896");
			}
			printf("string_type7 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{	
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s("12346557989621313132165465432131313123");
				s.append("123465579896");
				s.append("123465579896");
			}
			printf("std::string7 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
			}
			printf("string_type8 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
				s.append("12346557989621313132165465432131313123");
			}
			printf("std::string8 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			//string_type s1("121323241412412341241243");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s;
				s += "123465679897987985646546321321";
			}
			printf("string_type9 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s1("121323241412412341241243");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s;
				s += "123465679897987985646546321321";
			}
			printf("std::string9 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s;
				s += string_type("123465679897987985646546321321");
			}
			printf("string_type10 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s;
				s += std::string("123465679897987985646546321321");
			}
			printf("std::string10 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		//while(true)
		{
			{
				
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					string_type s;
					s.reserve(100);
				}
				printf("string_type11 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}

			{
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					std::string s;
					s.reserve(100);
				}
				printf("std::string11 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}
		}

		{
			string_type s;
			string_type s1("12346557989621313132165465432131313123");
			string_type s2("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = s1 + s2;
			}
			printf("string_type13 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s;
			std::string s1("12346557989621313132165465432131313123");
			std::string s2("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = s1 + s2;
			}
			printf("std::string13 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		//while(true)
		{
			{
				string_type s;
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					s = string_type("12346557989621313132165465432131313123") + 
						"12346557989621313132165465432131313123";
				}
				printf("string_type14 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}

			{
				std::string s;
				_timestamp_::Timestamp	time;
				for(int i = 0; i < num; i ++)
				{
					s = std::string("12346557989621313132165465432131313123") +
						"12346557989621313132165465432131313123";
				}
				printf("std::string14 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
			}
		}

		{
			string_type s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == "12346557989621313132165465432131313123" )
				{
					int i = 0;
				}
			}
			printf("string_type15 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == "12346557989621313132165465432131313123" )
				{
					int i = 0;
				}
			}
			printf("std::string15 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == string_type("12346557989621313132165465432131313123") )
				{

				}
			}
			printf("string_type16 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s == std::string("12346557989621313132165465432131313123") )
				{

				}
			}
			printf("std::string16 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s != string_type("123465579896213131321654654321313131231") )
				{

				}
			}
			printf("string_type17 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				if( s != std::string("123465579896213131321654654321313131231") )
				{

				}
			}
			printf("std::string17 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.find("131");
				int y = 0;
			}
			printf("string_type18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("12346557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.find("131");
				int y = 0;
			}
			printf("std::string18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s("88846557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.rfind("888");
				int y = 0;
			}
			printf("string_type18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s("88846557989621313132165465432131313123");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				int iPos = s.rfind("888");
				int y = 0;
			}
			printf("std::string18 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, 6, "888888");
			}
			printf("string_type19 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, 6, "888888");
			}
			printf("std::string19 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			char* sz = "888888888888888888888888888888";
			string_type ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, strlen(sz), string_type(sz));
			}
			printf("string_type20 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			char* sz = "888888888888888888888888888888";
			std::string ss = "123456798794654643311";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.replace(0, strlen(sz), std::string(sz));
			}
			printf("std::string20 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type ss = "123456798794654643311123456789645674878978";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.substr(10, 30);
			}
			printf("string_type21 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string ss = "123456798794654643311123456789645674878978";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				ss.substr(10, 30);
			}
			printf("std::string21 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type ss = "1234567890";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s = ss;
			}
			printf("string_type22 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string ss = "1234567890";
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s = ss;
			}
			printf("std::string22 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type s;
			string_type s1("123456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = s1;
			}
			printf("string_type23 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string s;
			std::string s1("123456789");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				s = s1;
			}
			printf("std::string23 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				str_Trim("\\12321312\\","\\");
			}
			printf("string_type24 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				str_Trim_s("\\12321312\\","\\");
			}
			printf("std::string24 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				str_Trim("\\12321312343242342345432524234\\","\\");
			}
			printf("string_type25 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				str_Trim_s("\\12321312343242342345432524234\\","\\");
			}
			printf("std::string25 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			string_type str("\\12321312343242342345432524234\\");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s(str);
				s.erase(0,1);
			}
			printf("string_type26 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			std::string str("\\12321312343242342345432524234\\");
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s(str);
				s.erase(0,1);
			}
			printf("std::string26 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s("12321312343242342345432524234");
				s.insert(10,"\\\\\\\\\\\\\\\\\\\\\\");
			}
			printf("string_type27 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				std::string s("12321312343242342345432524234");
				s.insert(10,"\\\\\\\\\\\\\\\\\\\\\\");
			}
			printf("std::string27 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s("12321312343242342345432524234");
				s.insert(0,"\\\\\\\\\\\\\\\\\\\\\\");
			}
			printf("string_type28 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}

		{
			_timestamp_::Timestamp	time;
			for(int i = 0; i < num; i ++)
			{
				string_type s("12321312343242342345432524234");
				s.insert(0,"\\\\\\\\\\\\\\\\\\\\\\");
			}
			printf("std::string28 time:%d\r\n", (_timestamp_::Timestamp().epochMicroseconds() - time.epochMicroseconds()) / 1000);
		}
	}

	std::cin.get();
	return 0;
}