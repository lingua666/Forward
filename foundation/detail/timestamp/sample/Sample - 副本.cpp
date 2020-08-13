// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libTimestamp/Timestamp.h>

class HighTime
{
public:
	HighTime( void )
		: mTimerMask( 0 )
	{
		reset();
	}

	~HighTime( void )
	{

	}

	template<typename T>
	static bool isPO2(T n)
	{
		return (n & (n-1)) == 0;
	}

	bool setOption( const _string_type & key, const void * val )
	{
		if ( key == "QueryAffinityMask" )
		{
			// Telling timer what core to use for a timer read
			DWORD newTimerMask = * static_cast < const DWORD * > ( val );

			// Get the current process core mask
			DWORD_PTR procMask;
			DWORD_PTR sysMask;
			GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

			// If new mask is 0, then set to default behavior, otherwise check
			// to make sure new timer core mask overlaps with process core mask
			// and that new timer core mask is a power of 2 (i.e. a single core)
			if( ( newTimerMask == 0 ) ||
				( ( ( newTimerMask & procMask ) != 0 ) && isPO2( newTimerMask ) ) )
			{
				mTimerMask = newTimerMask;
				return true;
			}
		}

		return false;
	}


	//-------------------------------------------------------------------------
	void reset()
	{
		// Get the current process core mask
		DWORD_PTR procMask;
		DWORD_PTR sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// If procMask is 0, consider there is only one core available
		// (using 0 as procMask will cause an infinite loop below)
		if (procMask == 0)
			procMask = 1;

		// Find the lowest core that this process uses
		if( mTimerMask == 0 )
		{
			mTimerMask = 1;
			while( ( mTimerMask & procMask ) == 0 )
			{
				mTimerMask <<= 1;
			}
		}

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Get the constant frequency
		QueryPerformanceFrequency(&mFrequency);

		// Query the timer
		QueryPerformanceCounter(&mStartTime);
		mStartTick = GetTickCount();

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		mLastTime = 0;
	}

	//-------------------------------------------------------------------------
	unsigned long getMilliseconds()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		// scale by 1000 for milliseconds
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;

			// Re-calculate milliseconds
			newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
		}

		// Record last time for adjust
		mLastTime = newTime;

		return newTicks;
	}

	//-------------------------------------------------------------------------
	unsigned long getMicroseconds()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;

		// get milliseconds to check against GetTickCount
		unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - mStartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;
		}

		// Record last time for adjust
		mLastTime = newTime;

		// scale by 1000000 for microseconds
		unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

		return newMicro;
	}

	UInt64 Utc( void )
	{
		LARGE_INTEGER tickNow;

		QueryPerformanceCounter(&tickNow);

		// adjust our tick count so that we get a proper epoch:
		tickNow.QuadPart += mStartTime.QuadPart;

		return (UInt64)((double)tickNow.QuadPart * 1000000) / mFrequency.QuadPart;
	}

private:
	DWORD mStartTick;
	LONGLONG mLastTime;
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mFrequency;
	DWORD_PTR mTimerMask;
};

void Test( void )
{
	int iTimer = 0, iNum = 100000000;
	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		_timestamp_::Timestamp	st;
		int iys = _timestamp_::Timestamp().epochMicroseconds() - st.epochMicroseconds();
	}
	printf("Timestamp epochMicroseconds timer:%d\r\n", GetTickCount() - iTimer);

	HighTime	st1;
	HighTime	st2;

	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		int iys = st1.Utc() - st2.Utc();
	}
	printf("HighTime Utc timer:%d\r\n", GetTickCount() - iTimer);


	
	iTimer = GetTickCount();
	for(int i = 0 ; i < iNum; i ++)
	{
		
		int iys = st1.getMicroseconds() - st2.getMicroseconds();
	}
	printf("HighTime getMicroseconds timer:%d\r\n", GetTickCount() - iTimer);
}

int main(int argc, char* argv[])
{
	Test();
	std::cin.get();
	return 0;
}


/*#include <icrsint.h>
#include<iostream>
#include<iomanip>
#include <string>
using namespace std;
#import "c:\program files\common files\system\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

void main()
{
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr=sqlSp.CreateInstance(_uuidof(Connection));
	if(FAILED(hr))
	{
		cout<<"_ConnectionPtr对象指针实例化失败！！！"<<endl;
		return ;
	}
	else {
		try {
			_bstr_t strConnect="Driver={sql server};server=192.168.8.153,1433;uid=sa;pwd=P@ssw0rd;database=equipmenttable;";
			//_bstr_t strConnect="Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=PBMS;uid=sa;pwd=pp471006459;";
			//_bstr_t strConnect="Provider=SQLOLEDB.1;Password=pp471006459;Persist Security Info=True;User ID=sa;"
			//"Initial Catalog=PBMS;Data Source=127.0.0.1,1433";
			//以上这三条strConnect语句都可以用！！看自己喜好选择用吧
			//要注意修改用户名uid、密码pwd、数据库名database，这三个参数均是你自己sql server的数据库信息
			sqlSp->Open(strConnect,"","",adModeUnknown);
		}
		catch(_com_error &e) {
			cout<<e.Description()<<endl;
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if(FAILED(m_pRecordset.CreateInstance( _uuidof( Recordset ))))
		{
			cout<<"记录集对象指针实例化失败！"<<endl;
			return;
		}
		//char strSQL[]="SELECT * FROM  Books";
		try {
			m_pRecordset->Open("SELECT * FROM  equipmenttable",(IDispatch*)sqlSp,adOpenDynamic,adLockOptimistic, adCmdText);//打开数据库，执行SQL语句

		}
		catch (_com_error &e)
		{
			cout << e.Description()<<endl;
		}

		try {
			m_pRecordset->MoveFirst();
			while(!m_pRecordset->adoEOF) 
			{
				string bookname=(char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("MyUIDCode"))->Value);
				cout<<bookname<<endl;
				m_pRecordset->MoveNext();
			}
			 m_pRecordset->Close(); 

		}
		catch (_com_error &e)
		{
			cout << e.Description()<<endl;
		}

		char *sqlCommand = "INSERT INTO equipmenttable VALUES ('888888', 'aaa', 'bbb', 'ccc', 'ddd', 'eee', 'fff') WHERE not exists (select * from equipmenttable where equipmenttable.[MyUIDCode] = '888888'); ";
		sqlSp->Execute(sqlCommand, NULL, 1);//执行SQL语句 

		 char *sqlUpdate = "UPDATE equipmenttable SET [A相电流] = 'Fred', [B相电流] = 'Blue' WHERE [MyUIDCode] = '888888'; ";
		 sqlSp->Execute(sqlUpdate, NULL, 1);//执行SQL语句 

	}
	system("pause");
}*/