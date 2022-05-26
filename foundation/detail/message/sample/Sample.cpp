// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllMessage/Exports.h>
#include <libMessage/MessageFIFO.h>
#include <libMessage/PriorityEvent.h>
#include <libMessage/PriorityQueue.h>
#include <libMessage/PriorityQueueV2.h>
#include <libContainer/LinkList.h>
#include <libContainer/Queue.hpp>
#include <libContainer/CycleList.hpp>
#include <libMessage/TimerEvent.h>
#include <libMessage/TimerEventKM.h>
#include <libMessage/TimerEventSync.h>
#include <list>
#include <deque>
#include <map>

using namespace _container_;


TimerEvent		g_Timer;
TimerEventKM	g_TimerKM;
TimerEventKM	g_TimerKM_int;
TimerEventSync<void>	g_TimerV20;
TimerEventSync<int>	g_TimerV20_int;
int			g_Num = 0;

void TimeCallBack( void )
{
	printf("TimeCallBack\r\n");
	g_Num ++;
	g_Timer.push_back(function20_bind_Ex(TimeCallBack), 1);
	if( g_Num > 1 )
		g_Timer.CloseAll();
}

void TimeCallBackV20( void )
{
	printf("TimeCallBackV20\r\n");
	g_Num ++;
	/*g_TimerV20.push_back(function20_bind_Ex(TimeCallBackV20), 1);
	if( g_Num > 1 )
		g_TimerV20.CloseAll();*/
}

int TimeCallBackV20_int( void )
{
	g_Num ++;
	printf("TimeCallBackV20_int g_Num:%d\r\n", g_Num);
	/*g_TimerV20_int.push_back(function20_bind_Ex(TimeCallBackV20_int), 1);
	if( g_Num > 1 )
		g_TimerV20_int.CloseAll();*/

	return g_Num;
}

class CTest //: public TR1::enable_shared_from_this<CTest>
{
public:
	CTest( void )
	{
		//printf("CTest constructor\r\n");
	}

	~CTest( void )
	{
		//printf("CTest destructor\r\n");
	}

	int yst( void )
	{
		return 500;
	}
};


void TimeEventTest( void )
{
	while(true)
	{
		printf("开启计时器\r\n");
		g_Timer.Init(1);

		g_Timer.push_back(function20_bind_Ex(TimeCallBack), 1);
		//std::cin.get();
		Sleep(100);
	}
}

void TimeEventV20Test( void )
{
	while(true)
	{
		g_TimerV20.Init(1);
		g_TimerV20_int.Init(1);

		SmartPTR<CTest> Ptr(new CTest);

		{
			g_Timer.Init(1);
			g_Timer.push_back(function20_bind_Ex(&CTest::yst, Ptr), 10000);
			g_Timer.Clear();
			int i = 0;
		}

		printf("开启V20计时器 begin\r\n");
		g_TimerV20.push_back(function20_bind_Ex(TimeCallBackV20), 0);
		int i = g_TimerV20_int.push_back(function20_bind_Ex(TimeCallBackV20_int), 0);
		printf("开启V20计时器 i:%d end\r\n", i);
		//std::cin.get();
		Sleep(100);
	}
}

void TimeEventKMTest(void)
{
	while (true)
	{
		g_TimerKM.Init(1);
		g_TimerKM_int.Init(1);

		/*SmartPTR<CTest> Ptr(new CTest);

		{
			g_TimerKM.Init(1);
			g_TimerKM.push_back(function20_bind_Ex(&CTest::yst, Ptr), 10000);
			g_TimerKM.Clear();
			int i = 0;
		}*/



		printf("开启KM计时器 begin\r\n");
		while(true)
		{
			printf("11111\r\n");
			TimerEventKM::_string_ sUid1 = g_TimerKM.push_back(function20_bind_Ex(TimeCallBackV20), 3000);
			TimerEventKM::_string_ sUid2 = g_TimerKM_int.push_back(function20_bind_Ex(TimeCallBackV20_int), 3000);
			printf("2222\r\n");
			//test
			g_TimerKM.Lock();
			Timestamp_type Local;
			system("time 12:23:20"); // 修改时间
			Timestamp_type New;
			g_TimerKM.UpdateTime(Local, New);
			g_TimerKM.UnLock();
			printf("333\r\n");
			Sleep(10000000);
			//test

			g_TimerKM.Remove(sUid1);
			g_TimerKM_int.Remove(sUid2);
			Sleep(10);
		}
		printf("开启KM计时器 end\r\n");
		//std::cin.get();
		Sleep(100);
	}
}

void ContainerTest( void )
{
	int iNum = 100000000;

	std::string szwer("1234");
	
	//while(true)
	{
		UInt32 _u32Size;
		int iTimer = 0;
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
		}

		{
			std::deque< std::string > deq;
			iTimer = GetTickCount();
			for( int i = 0 ; i < iNum; i ++)
			{
				deq.push_back(szwer);
			}
			printf("deque push timer:%d\r\n", GetTickCount() - iTimer);

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

		{
			CycleList< std::string, CNoneLock > cyc;
			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				//cyc.push_back(i);
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
			printf("CycleList pop timer:%d\r\n", GetTickCount() - iTimer);
		}

		{
			Queue< std::string, CNoneLock, MemPool_macro<0, CNoneLock> >	que;
			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				que.push_back(szwer);
			}
			printf("Queue push timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				que.front();
			}
			printf("Queue front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				que.pop_front();
			}
			printf("Queue pop timer:%d\r\n", GetTickCount() - iTimer);
		}

		{
			PriorityQueue<std::string>		MesQue;
			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				MesQue.push_back(szwer, rand() % 5);
			}
			printf("PriorityQueue push timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				MesQue.front();
			}
			printf("PriorityQueue front timer:%d\r\n", GetTickCount() - iTimer);

			iTimer = GetTickCount();
			for(int i = 0 ; i < iNum; i ++)
			{
				MesQue.pop_front();
			}
			printf("PriorityQueue pop timer:%d\r\n", GetTickCount() - iTimer);
		}
	}
}

void EventTestFunc( void )
{
	int i = 0;
	i ++;
}

struct EventTest : public IEvent
{
	EventTest( void )
	{

	}

	~EventTest( void )
	{

	}

	void Invoke( void )
	{
		EventTestFunc();
	}
};

EventTest	g_EventTest;
void EventTestDll_Test()
{
	MMessage_TimerEvent_Init();
	MMessage_TimerEvent_push_back(&g_EventTest, 0);
	std::cin.get();
};

int main(int argc, char* argv[])
{
	TimeEventKMTest();

	//EventTestDll_Test();

	TimeEventV20Test();

	PriorityQueue<int>		MesQue;
	for(int i = 0 ; i < 10; i ++)
	{
		MesQue.push_back(i, rand() % 5);
	}

	printf("PriorityQueue value:%d\r\n", MesQue[0]);
	printf("PriorityQueue value:%d\r\n", MesQue[5]);
	printf("PriorityQueue value:%d\r\n", MesQue[9]);

	 //MesQue.pop();
	PriorityQueue<int>::iterator iter;
	for( iter = MesQue.begin(); iter; iter ++ )
	{
		printf("PriorityQueue iter:%d\r\n", *iter);
	}

	for(int i = 0 ; i < 10; i ++)
	{
		printf("PriorityQueue pop value:%d\r\n", MesQue.front());
		 MesQue.pop();
	}

	//ContainerTest();
	TimeEventTest();

	std::cin.get();
	return 0;
}