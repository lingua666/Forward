#include <libThirdWrap_RtspClient/DummySink.h>              //live555只要包含这两个头文件就可以了

#define DUMYSINK_BUFFER_SIZE 5120

// 实现 "DummySink":

UInt8* DummySink::Alloc( void )
{
	return Singleton<DummyDataPool>::instance()->Alloc();
}

void DummySink::Free( UInt8* pBuf )
{
	Singleton<DummyDataPool>::instance()->Free(pBuf);
}

//DummySink构造函数
DummySink::DummySink( UsageEnvironment& env,
				MediaSubsession& subsession,
				char const* streamId )
	: MediaSink(env)
	, _fSubsession(subsession) 
	, _offset(4)
{
	_fStreamId = strDup(streamId);
	_fReceiveBuffer = DummySink::Alloc();//new unsigned char[DUMMY_SINK_RECEIVE_BUFFER_SIZE + 4];

	_fReceiveBuffer[0] = 0x00;
	_fReceiveBuffer[1] = 0x00;
	_fReceiveBuffer[2] = 0x00;
	_fReceiveBuffer[3] = 0x01;
}

//DummySink析构函数
DummySink::~DummySink( void ) 
{
  DummySink::Free(_fReceiveBuffer);
  //delete[] _fReceiveBuffer;
  delete[] _fStreamId;
}

void DummySink::SetRealDataCallBack( const HFNRealData& hRealData )
{
	_hRealData = hRealData;
}

void DummySink::afterGettingFrame( void* clientData,
							unsigned frameSize,
							unsigned numTruncatedBytes,
							struct timeval presentationTime,
							unsigned durationInMicroseconds ) 
{
  DummySink* sink = (DummySink*)clientData;
  sink->afterGettingFrame(frameSize, numTruncatedBytes,
						presentationTime, durationInMicroseconds);
}


void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,struct timeval presentationTime, unsigned /*durationInMicroseconds*/)
{

  bool firstFrame = true;
  if (!strcmp(_fSubsession.mediumName(), "video"))
  {
	  unsigned int num;
	  //SPropRecord *sps = parseSPropParameterSets(_fSubsession.fmtp_spropparametersets(), num);
	  // For H.264 video stream, we use a special sink that insert start_codes:

	  _offset += frameSize;

	  if( _offset > DUMYSINK_BUFFER_SIZE )
	  {
		/*if( g_hDec == NULL )
		{
			g_hDec = MDecoder_Parse((const char*)fReceiveBuffer, _offset);
			if( g_hDec != NULL )
			{
				MDecoder_Set(g_hDec, GetConsoleWindow());
			}
		}
		else
		{
			MDecoder_Decode(g_hDec, (const char*)fReceiveBuffer, _offset);
			_offset = 4;
		}*/

		  if( _hRealData )
			  _hRealData(_fReceiveBuffer, _offset);

		  _offset = 4;
	  }
	  else
	  {
			_fReceiveBuffer[_offset] = 0x00;
			_fReceiveBuffer[_offset + 1] = 0x00;
			_fReceiveBuffer[_offset + 2] = 0x00;
			_fReceiveBuffer[_offset + 3] = 0x01;
			_offset += 4;
	  }

  }

  // Then continue, to request the next frame of data:
  continuePlaying();
}

//这个是循环继续获取流数据函数，用的也还是afterGettingFrame()函数，其实获取流数据最重要的就是这个函数
Boolean DummySink::continuePlaying( void ) 
{
	if (fSource == NULL) 
		return False; // 完整性检查(不应发生)

	// 请求下一帧数据从输入源。“afterGettingFrame()”将被调用后,当它到达:
	//fReceiveBuffer 是流数据的存储内存
	//fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE, afterGettingFrame, this, onSourceClosure, this);
	fSource->getNextFrame(&_fReceiveBuffer[_offset], DUMMY_SINK_RECEIVE_BUFFER_SIZE - _offset, afterGettingFrame, this, onSourceClosure, this);
	return True;
}

//****************************************************************建立rtsp连接和收流部分接口：End*****************************
