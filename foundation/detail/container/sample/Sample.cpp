// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <libContainer/LinkList.h>
#include <libContainer/CycleList.hpp>
#include <libContainer/Queue.hpp>
#include <libContainer/CMyUnordered_map.hpp>
#include <map>
#include <list>
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define NUM1_DEC	10000000//30000000//100000000

using namespace _container_;

void MapTest( void )
{
	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<int,int>	m;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.insert(std::make_pair(i,i));
		}
		printf("unordered_map<int,int> insert timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("unordered_map<int,int> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<int,std::string>	m;
		char sz[100] = {0};

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			itoa(1000000000 + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(i,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<int,string> insert timer:%d\r\n", msecond);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("unordered_map<int,string> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<std::string,int>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(sz,i));
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<string,int> insert timer:%d\r\n", msecond);

		msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.find(sz);
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<string,int> find timer:%d\r\n", msecond);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<std::string,std::string>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			/*std::string s = Uid;
			hash<std::string>()(s);
			size_t _Val = 2166136261U;
			size_t _First = 0;
			size_t _Last = strlen(Uid);
			size_t _Stride = 1 + _Last / 10;

			for(; _First < _Last; _First += _Stride)
				_Val = 16777619U * _Val ^ (size_t)Uid[_First];
			printf("_Val:%lld\r\n",_Val);*/
			//return (_Val);


			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			//m.insert(std::make_pair(Uid,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<std::string,std::string> insert timer:%d\r\n", msecond);

		msecond = 0;
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			//m.find(Uid);
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<std::string,std::string> find timer:%d\r\n", msecond);
	}


	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<int,int>	m;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.insert(std::make_pair(i,i));
		}
		printf("map<int,int> insert timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("map<int,int> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<int,std::string>	m;
		char sz[100] = {0};

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(i,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("map<int,string> insert timer:%d\r\n", msecond);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("map<int,string> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<std::string,int>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(sz,i));
			msecond += GetTickCount() - iTimer;
		}
		printf("map<string,int> insert timer:%d\r\n", msecond);

		msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.find(sz);
			msecond += GetTickCount() - iTimer;
		}
		printf("map<string,int> find timer:%d\r\n", msecond);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<std::string,std::string>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(Uid,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("map<std::string,std::string> insert timer:%d\r\n", msecond);

		msecond = 0;
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			itoa(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.find(Uid);
			msecond += GetTickCount() - iTimer;
		}
		printf("map<std::string,std::string> find timer:%d\r\n", msecond);
	}
}

void LinkListTest( void )
{
	int iNum = NUM1_DEC;
	int iTimer = 0;
	std::string szwer("1234");

	
	{
		{
			LinkList< std::string, CNoneLock >	yc;
			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				yc.push_back(szwer);
			}
			printf("LinkList push timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				yc.front();
			}
			printf("LinkList front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				yc.pop();
			}
			printf("LinkList pop timer:%d\r\n", GetTickCount() - iTimer);

			for( int i = 0 ; i < iNum; i ++)
			{
				yc.push_back(szwer);
			}

			LinkList< std::string, CNoneLock >::iterator iter ;

			iTimer = GetTickCount();
			for( iter = yc.begin(); iter; )
			{
				iter = yc.erase(iter);
			}
			printf("LinkList erase timer:%d\r\n", GetTickCount() - iTimer);
		}

		{
			std::list< std::string > yc1;
			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				yc1.push_back(szwer);
			}
			printf("list push timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				yc1.front();
			}
			printf("list front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				yc1.pop_front();
			}
			printf("list pop timer:%d\r\n", GetTickCount() - iTimer);

			std::list< std::string >::iterator iter ;

			for( int i = 0 ; i < iNum; i ++)
			{
				yc1.push_back(szwer);
			}

			iTimer = GetTickCount();
			for( iter = yc1.begin(); iter != yc1.end(); )
			{
				iter = yc1.erase(iter);
			}
			printf("list erase timer:%d\r\n", GetTickCount() - iTimer);
		}

		{
			CycleList< std::string, CNoneLock > cyc;
			
			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc.push_back(szwer);
			}
			printf("CycleList push timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc.current();
			}
			printf("CycleList front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc.pop();
			}

			/*cyc.push_back(100);
			cyc.current();*/

			printf("CycleList pop timer:%d\r\n", GetTickCount() - iTimer);
		}

		{
			Queue< std::string, CNoneLock, MemPool_macro<0, CNoneLock> > cyc;
			
			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc.push_back(szwer);
			}
			printf("Queue push timer:%d\r\n", GetTickCount() - iTimer);

			Queue< std::string, CNoneLock, MemPool_macro<0, CNoneLock> >::iterator iter;
			iTimer = GetTickCount();
			for(iter = cyc.begin(); iter != cyc.end(); iter ++)
			{
				//printf("iter:%d\r\n", *iter);
			}
			printf("Queue iter timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc[10];
			}
			printf("Queue[] timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc.front();
			}
			printf("Queue front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				cyc.pop_front();
			}

			/*cyc.push_back(100);
			cyc.current();*/

			printf("Queue pop timer:%d\r\n", GetTickCount() - iTimer);
		}

		{
			std::deque< std::string > deq;
			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				deq.push_back(szwer);
			}
			printf("deque push timer:%d\r\n", GetTickCount() - iTimer);

			std::deque< std::string >::iterator iter;
			iTimer = GetTickCount();
			for(iter = deq.begin(); iter != deq.end(); iter ++)
			{
				//printf("iter:%d\r\n", *iter);
			}
			printf("deque iter timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				deq[10];
			}
			printf("deque[] timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				deq.front();
			}
			printf("deque front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				deq.pop_front();
			}
			printf("deque pop timer:%d\r\n", GetTickCount() - iTimer);

		}
	}	
}

int main(int argc, char* argv[])
{
	/*std::vector<int>	vec;
	for(int i = 0; i < 10; i++)
		vec.push_back(i);

	vec.erase(vec.begin() + 3);

	for(int i = 0; i < 9; i++)
		printf("vec:%d\r\n", vec[i]);
	std::cin.get();*/

	LinkList< int, CNoneLock >	yc;

	while(true)
	{
		LinkList< int, CNoneLock >::iterator iter ;

		for(int i = 0; i < 10; i++)
			yc.push_back(i);

		iter = yc.erase(5);
		printf("yc:%d\r\n", *iter);
		printf("yc:%d\r\n", yc[5]);

		for(int i = 0 ; i < yc.size(); i ++)
		{
			printf("yc:%d\r\n", yc[i]);
		}
		
		for( iter = yc.begin(); iter != yc.end(); )
		{
			if(*iter == 9)
			{
				iter = yc.erase(iter);
				if( iter )
					printf("yc:%d\r\n", *iter);
				iter ++;
			}
			else
			{
				if( iter )
					printf("yc:%d\r\n", *iter);
				iter ++;
			}

			
		}

		yc.clear();

		for( iter = yc.begin(); iter != yc.end();  )
		{
			iter = yc.erase(iter);
		}
		Sleep(10);
	};
	

	/*Queue< int, CNoneLock >	que;

	while(true)
	{
		for(int i = 0; i < 50; i++)
			que.push_back(i);

		Queue< int, CNoneLock >::iterator iter1;
		for(iter1 = que.begin(); iter1 != que.end(); iter1 ++)
		{
			printf("iter:%d\r\n", *iter1);
		}

		printf("que:%d\r\n", que[0]);
		printf("que:%d\r\n", que[5]);
		printf("que:%d\r\n", que[9]);

		for(int i = 0; i < 50; i++)
		{
			printf("front:%d\r\n", que.front());
			que.pop();
		}
		Sleep(10);
	}*/
	

	//MapTest();
	LinkListTest();

	std::cin.get();
	return 0;
}