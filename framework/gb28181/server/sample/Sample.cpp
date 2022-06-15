// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_RtspSvr/RtspServer.h>
#include <libFrameWork_Rtsp/H265Info.h>

bool isPlay = false;

void ReadH265Data( const _string_type& sSrc, RtspServer* pRtspSvr, const RtspServer::HFNStreamProcessEvent& hEvent )
{
	_file_::CMyFile SrcFile;
	SrcFile.Open(sSrc.c_str(), "rb");

	const int iSize = 1024 * 1024;
	_io_net_::_buf_hdr* pHdr = (_io_net_::_buf_hdr*)malloc(iSize + 10);
	_string_type sData;
	char szHead[4] = {0};
	int iLen = 0;
	char szData[4] = {0};
	do 
	{
		int iSize = SrcFile.Read(szData, 1);
		if( iSize <= 0 )
		{
			break;
		}
		else
		{
			switch(iLen)
			{
			case 0:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					sData.append(szData, 1);
					iLen = 0;
				}
				break;
			case 1:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					sData.append(szHead, iLen);
					sData.append(szData, 1);
					iLen = 0;
				}
				break;
			case 2:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					sData.append(szHead, iLen);
					sData.append(szData, 1);
					iLen = 0;
				}
				break;
			case 3:
				if(szData[0] == 0x01)
				{
					szHead[iLen] = szData[0];
					if(sData.size() > 0)
					{
						memcpy(pHdr->data, sData.c_str(), sData.size());
						pHdr->payload = sData.size();
						StreamBuf_ptr	sptr(pHdr, false);
						hEvent(-1, sptr);

						//test
						Sleep(30);
						/*tagVideoPlayload_INFO PlayLoad;
						H265Info Info;
						Info.Parse((char*)sData.c_str(), sData.size(), &PlayLoad);*/
						//test

						sData.resize(0);
					}
				}
				else
				{
					sData.append(szHead, iLen);
					sData.append(szData, 1);
				}

				iLen = 0;
				break;
			}
		}

	} while (true);

	SrcFile.Close();
	if( pHdr != NULL )
	{
		free(pHdr);
	}
}

void ReadH264Data( const _string_type& sSrc, RtspServer* pRtspSvr, const RtspServer::HFNStreamProcessEvent& hEvent )
{
	_file_::CMyFile SrcFile;
	SrcFile.Open(sSrc.c_str(), "rb");

	const int iSize = 1024 * 1024;
	_io_net_::_buf_hdr* pHdr = (_io_net_::_buf_hdr*)malloc(iSize + 10);
	_string_type sData;
	char szHead[4] = {0};
	int iLen = 0;
	char szData[4] = {0};
	do 
	{
		int iSize = SrcFile.Read(szData, 1);
		if( iSize <= 0 )
		{
			break;
		}
		else
		{
			switch(iLen)
			{
			case 0:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					sData.append(szData, 1);
					iLen = 0;
				}
				break;
			case 1:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					sData.append(szHead, iLen);
					sData.append(szData, 1);
					iLen = 0;
				}
				break;
			case 2:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					sData.append(szHead, iLen);
					sData.append(szData, 1);
					iLen = 0;
				}
				break;
			case 3:
				if(szData[0] == 0x01)
				{
					szHead[iLen] = szData[0];
					if(sData.size() > 0)
					{
						memcpy(pHdr->data, sData.c_str(), sData.size());
						pHdr->payload = sData.size();
						StreamBuf_ptr	sptr(pHdr, false);
						hEvent(-1, sptr);

						sData.resize(0);

						//test
						Sleep(40);
						//test
					}
				}
				else
				{
					sData.append(szHead, iLen);
					sData.append(szData, 1);
				}

				iLen = 0;
				break;
			}
		}

	} while (true);

	SrcFile.Close();
	if( pHdr != NULL )
	{
		free(pHdr);
	}
}

void RemoveH264Header( const _string_type& sSrc, const _string_type& sDest )
{
	_file_::CMyFile SrcFile;
	SrcFile.Open(sSrc.c_str(), "rb");

	_file_::CMyFile DestFile;
	DestFile.Open(sDest.c_str(), "wb");

	char szHead[4] = {0};
	int iLen = 0;
	char szData[4] = {0};
	do 
	{
		int iSize = SrcFile.Read(szData, 1);
		if( iSize <= 0 )
		{
			break;
		}
		else
		{
			switch(iLen)
			{
			case 0:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					DestFile.Write(szData, 1);
					iLen = 0;
				}
				break;
			case 1:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					DestFile.Write(szHead, iLen);
					DestFile.Write(szData, 1);
					iLen = 0;
				}
				break;
			case 2:
				if(szData[0] == 0x00)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					DestFile.Write(szHead, iLen);
					DestFile.Write(szData, 1);
					iLen = 0;
				}
				break;
			case 3:
				if(szData[0] == 0x01)
				{
					szHead[iLen] = szData[0];
					iLen ++;
				}
				else
				{
					DestFile.Write(szHead, iLen);
					DestFile.Write(szData, 1);
				}

				iLen = 0;
				break;
			}
		}

	} while (true);

	SrcFile.Close();
	DestFile.Close();

}

void HandleClose( void )
{

}

void WorkThread( RtspServer* pRtspSvr )
{
	/*char da = 0x65;
	tagNALU_HEADER* ppp = ((tagNALU_HEADER*)&da);
	ppp->TYPE = 28;*/

	_file_::CMyFile File;
	//File.Open("C:\\Users\\Tranwit\\Desktop\\rtsp\\海康样本_不带头.h264", "rb");
	File.Open("C:\\Users\\Tranwit\\Desktop\\rtsp\\128x128.264", "rb");

	/*RtspServer::HFNStreamProcessEvent hEvent = pRtspSvr->AddMediaStream("Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
		function20_bind(HandleClose), IVideoInfo::ENUM_ENC_H265);*/

	RtspServer::HFNStreamProcessEvent hEvent = pRtspSvr->AddMediaStream("Streaming/Channels/101?transportmode=unicast&profile=Profile_1",
		function20_bind(HandleClose), IVideoInfo::ENUM_ENC_H264);

	//test
	//ReadH265Data("C:\\Users\\Tranwit\\Desktop\\rtsp\\test3.h265", pRtspSvr, hEvent);
	//RemoveH264Header("C:\\Users\\Tranwit\\Desktop\\rtsp\\128x128.264", "C:\\Users\\Tranwit\\Desktop\\rtsp\\128x128_no_head.264");
	//test

	//test
	ReadH264Data("C:\\Users\\Tranwit\\Desktop\\rtsp\\rtsp(192.168.8.166).H264", pRtspSvr, hEvent);
	//ReadH265Data("C:\\Users\\Tranwit\\Desktop\\rtsp\\test3.h265", pRtspSvr, hEvent);
	printf("is Run\r\n");
	while(true)
	{
		//ReadH264Data("C:\\Users\\Tranwit\\Desktop\\rtsp\\128x128.264", pRtspSvr, hEvent);
		if( isPlay )
		{
			ReadH264Data("C:\\Users\\Tranwit\\Desktop\\rtsp\\rtsp(192.168.8.166).H264", pRtspSvr, hEvent);
			//ReadH265Data("C:\\Users\\Tranwit\\Desktop\\rtsp\\test3.h265", pRtspSvr, hEvent);
			isPlay = false;
		}
		Sleep(10);
	}
	//test

	const int iSize = 1024 * 2;
	_io_net_::_buf_hdr* pHdr = (_io_net_::_buf_hdr*)malloc(iSize + 10);
	char szData[iSize] = {0};
	int iLen = 0;
	while(true)
	{
		try
		{
			iLen = File.Read(szData, sizeof(szData));
			if( iLen <= 0 )
			{
				File.Seek(0, SEEK_SET);
			}
			else
			{
				memcpy(pHdr->data, szData, iLen);
				pHdr->payload = iLen;
				StreamBuf_ptr	sptr(pHdr, false);
				hEvent(-1, sptr);
			}

			ThreadWrap::interruption_point();
		}
		catch (const thread_interrupted& e)
		{
			switch( e.get_code() )
			{
			case ThreadData::en_INTERRUPTED_EXIT:	//线程退出
				goto exit;
				break;
			}
			break;
		}
	}
exit:
	return ;
}

void	HandleSessionOPStatusEvent( const RtspServer::RtspSession_sptr& sptr, const _string_type& sSessionUid,
	UInt8 uStatus )
{
	isPlay = true;
}

void	HandleAccept( const _string_type& sSessionUid,
	const _string_type& sChildUrl,
	const RtspServer::RtspSession_sptr& sptr )
{

}

void	HandleDestroy( const _string_type& sSessionUid,
	const RtspServer::RtspSession_sptr& sptr )
{

}

int main(int argc, char* argv[])
{
	//test
	/*UInt8 pp[] = {
	0x80,0x60,0x00,0x04,0x29,0x52,0x01,0x38,0xB0,0x19,
	0xB3,0xB0,0x62,0x01,0x93,0xAF,0x11,0x68,0x5C,0x6B,
	0x5A,0xB1,0x4B,0x34,0xC7,0xFA,0xA7,0xDF,0x03,0xC5
	};

	UInt8 pH265[] = {
		0x26,0x01,0xB1,0xAF,0x11,0x68,0x5C,0x6B,0x5A,0xB1,
		0x4B,0x34,0xC7,0xFA,0xA7,0xC90,0xB30,0x71,0xB9,0xEB
	};

	UInt8 pH2651[] = {
		0x01,0x26,0xB1,0xAF,0x11,0x68,0x5C,0x6B,0x5A,0xB1,
		0x4B,0x34,0xC7,0xFA,0xA7,0xC90,0xB30,0x71,0xB9,0xEB
	};

	UInt8 pH2652[] = {
		0x26,0x00,0xB1,0xAF,0x11,0x68,0x5C,0x6B,0x5A,0xB1,
		0x4B,0x34,0xC7,0xFA,0xA7,0xC90,0xB30,0x71,0xB9,0xEB
	};

	tagNALU265_HEADER ls = {0};
	ls.F = 0;
	ls.hLayerID = 2;
	ls.lLayerID = 1;
	ls.TYPE = 49;

	UInt8* uu = (UInt8*)&ls;

	char sz[] = {0x63,0x10};

	tagFU265_A_HEADER* ppy = (tagFU265_A_HEADER*)pH265;
	tagNALU265_HEADER* ppy1 = (tagNALU265_HEADER*)pH2651;
	tagNALU265_HEADER* ppy2 = (tagNALU265_HEADER*)pH2652;
	tagNALU265_HEADER* ppy3 = (tagNALU265_HEADER*)sz;

	char ys[2] = {0};
	memcpy(ys, ppy3, sizeof(ppy3));

	

	RtpPacket po;
	tagVideoPlayload_INFO PlayloadInfo;
	po.Init(0,0,90000,IVideoInfo::ENUM_ENC_H265);
	po.Parse((char*)pp, sizeof(pp), &PlayloadInfo);*/
	//test



	RtspServer	RtspSvr;
	int iPort = 554;
	RtspSvr.Init("admin", "abcd-1234", "192.168.8.43", _string_type::NumberToStr(iPort));
	RtspSvr.Start(iPort);
	RtspSvr.SetSessionEvent(HandleAccept, HandleDestroy, HandleSessionOPStatusEvent);

	ThreadWrap	Wrap(function20_bind(WorkThread, &RtspSvr));
	Wrap.wait_run();


	std::cin.get();
	return 0;
}