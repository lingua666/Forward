// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Http_V20/Http.h>

typedef _foundation_::_string_t		_app_string;
typedef _foundation_::_string_big_t	_app_string_big;

void HandleHttp(const _http_::Http_V20<_string_type>::self_sptr& sptr,
	_http_::Http_V20<_string_type>::StatusCode Code, const char* c_szData, UInt32 uSize)
{
	switch (Code)
	{
	case _http_::Http_V20<_app_string>::EN_Status_Data_Start:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Data:
	{
		
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Data_End:
	{
		
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Repones:
	{
		printf("%s\r\n", c_szData);
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Connect:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Encode:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Decode:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Send:
	{

	}
	break;
	}
}

static FILE* g_fp = fopen("D:\\11111.mp3", "wb");

void HandleHttpChunk(const _http_::Http_V20<_string_type>::self_sptr& sptr,
	_http_::Http_V20<_string_type>::StatusCode Code, const char* c_szData, UInt32 uSize)
{
	switch (Code)
	{
	case _http_::Http_V20<_app_string>::EN_Status_Data_Start:
	{
		
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Data:
	{
		/*fwrite(c_szData, 1, uSize, g_fp);
		fflush(g_fp);*/
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Data_End:
	{
		//fclose(g_fp);
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Repones:
	{
		
	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_UpLoad:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Connect:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Encode:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Decode:
	{

	}
	break;
	case _http_::Http_V20<_app_string>::EN_Status_Err_Send:
	{

	}
	break;
	}
}

int main(int argc, char* argv[])
{
	_http_::Http_V20<_string_type>::HttpClientsEnvironmentInit();

	while (true)
	{
		_http_::Http_V20<_string_type>::self_sptr sptr = _http_::Http_V20<_string_type>::Alloc();
		/*sptr->Init(function20_bind_Ex(HandleHttp));
		sptr->Post("http://36.137.225.202:8180/nkyt-lhkzq-ms/syn/getTime", "");*/
		sptr->Init(function20_bind_Ex(HandleHttpChunk));
		//sptr->Get("http://202.105.12.106:8181/resources/mp3/24ae4084-120b-405e-a9b1-625a4999479e.mp3", "");

		/*_http_::Http_V20<_string_type>::HdrList_type	List;
		_http_::Http_V20<_string_type>::tagHdr_INFO		Hdr_INFO;
		Hdr_INFO.Name = "uid";
		Hdr_INFO.Value = "44";
		List.push_back(Hdr_INFO);

		Hdr_INFO.Name = "rcode";
		Hdr_INFO.Value = "9S9DRYI1ZPVT672UVL6U6632THRXPGEPG288S487WI6HTWB27V";
		List.push_back(Hdr_INFO);

		Hdr_INFO.Name = "file\"; filename=\"sip.txt";
		Hdr_INFO.ContentType = "text/plain";
		Hdr_INFO.Value = "";
		List.push_back(Hdr_INFO);*/

		_app_string sPath = "E:\\key\\sys.dat";// "/data/sys.dat";
		typedef typename _http_::Http_V20<_string_type>	Https_type;
		Https_type::HdrList_type	List;
		Https_type::tagHdr_INFO	Hdr_INFO;
		Hdr_INFO.Name = "uid";
		Hdr_INFO.Value = "9999";
		List.push_back(Hdr_INFO);

		Hdr_INFO.Name = "rcode";
		Hdr_INFO.Value = "E8JOZBIZJVSIGWDK1O3LZI5LTU94XVU56CKH9J96X2QAR84AU9";
		List.push_back(Hdr_INFO);

		Hdr_INFO.Name = "path";
		_app_string sTmp = sPath;
		_app_string sDst = "/";

		Hdr_INFO.Value = sDst.c_str();

		List.push_back(Hdr_INFO);

		Hdr_INFO.Name = "device_id";
		Hdr_INFO.Value = "8888888888888888";
		List.push_back(Hdr_INFO);

		_uuid_::MD5Engine Md5;
		_app_string sFileMD5 = Md5.digestToFile(sPath.c_str()).c_str();

		Hdr_INFO.Name = "md5";
		Hdr_INFO.Value = sFileMD5.c_str();
		List.push_back(Hdr_INFO);

		char szFileInfo[1024] = { 0 };
		sprintf(szFileInfo, "file\"; filename=\"%s", "sys.dat");

		Hdr_INFO.Name = szFileInfo;
		Hdr_INFO.ContentType = "application/zip";
		Hdr_INFO.Value = "";
		List.push_back(Hdr_INFO);

		sptr->PostFile("http://192.168.1.188/nkytms/uploadDeviceFile", List, sPath.c_str());
		//Sleep(rand() % 3000);
		//Sleep(5000000);
		Sleep(10000000000);
	}

	std::cin.get();
	return 0;
}