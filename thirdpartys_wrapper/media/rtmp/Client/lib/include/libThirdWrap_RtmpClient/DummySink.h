#ifndef __DUMMYSINK_20170213155004_1444199361_H__
#define __DUMMYSINK_20170213155004_1444199361_H__

/*#include "External.h"
#include <liveMedia.hh>

//虽然我们不会做任何传入的数据,我们仍然需要得到它。
//定义缓冲区的大小,我们将使用:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 300000

// 如果你不想看到调试输出为每个接收帧,然后注释掉以下行:
#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1

class MyRTSPClient;

class DummyDataPool
{
public:
	DummyDataPool( void )
		: _Pool(DUMMY_SINK_RECEIVE_BUFFER_SIZE + 4)
	{

	}

	UInt8* Alloc( void )
	{
		return reinterpret_cast<UInt8*>(_Pool.Alloc());
	}

	void Free( UInt8* pBuf )
	{
		if( pBuf != NULL )
			_Pool.Free(pBuf);
	}

private:
	MemPool_type	_Pool;
};

//数据的消费者
class DummySink: public MediaSink 
{
public:
	friend class MyRTSPClient;

	typedef function20_handle<void (UInt8*, UInt32)>	HFNRealData;

	static UInt8* Alloc( void );

	static void Free( UInt8* pBuf );

private:
  DummySink( UsageEnvironment& env, MediaSubsession& subsession, char const* streamId );
    // called only by "createNew()"

  virtual ~DummySink( void );

  void SetRealDataCallBack( const HFNRealData& hRealData );

  static void afterGettingFrame( void* clientData,
			unsigned frameSize,unsigned numTruncatedBytes,
			struct timeval presentationTime,
			unsigned durationInMicroseconds );

  void afterGettingFrame( unsigned frameSize,
			unsigned numTruncatedBytes,
			struct timeval presentationTime,
			unsigned durationInMicroseconds );

private:
  // redefined virtual functions:
  virtual Boolean continuePlaying( void );

private:
  HFNRealData	_hRealData;
  MediaSubsession& _fSubsession;
  unsigned char* _fReceiveBuffer;
  char* _fStreamId;
  int	_offset;
};*/

#endif




