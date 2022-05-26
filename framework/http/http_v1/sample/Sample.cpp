// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Http/Http.h>

void HandleHttp(const _http_::Http<_string_type>::self_sptr& sptr, _http_::Http<_string_type>::StatusCode Code, void* pParam)
{
	switch (Code)
	{
	case _http_::Http<_string_type>::StatusCode::EN_CODE_CONNECT_SUCCESS:
	{
		if (sptr->PostData("", 0, "/nkyt-gj-ms/ag/device/getTime") == -1)
		{//Ê§°Ü
			
		}
	}
	break;
	case _http_::Http<_string_type>::StatusCode::EN_CODE_REPONES_SUCCESS:
	{//³É¹¦
		printf("HandleHttp:%s\r\n", (char*)pParam);
		if (pParam != NULL)
		{
			
		}
	}
	break;
	case _http_::Http<_string_type>::StatusCode::EN_CODE_CONNECT_FAILED:
	case _http_::Http<_string_type>::StatusCode::EN_CODE_TIMEOUT:
	case _http_::Http<_string_type>::StatusCode::EN_CODE_EXCEPT:
	case _http_::Http<_string_type>::StatusCode::EN_CODE_REPONES_FAILED:
	{//Ê§°Ü
		int i = 0;
	}
	break;
	}
}

void HandleHttpChunk(const _http_::Http<_string_type>::self_sptr& sptr, _http_::Http<_string_type>::StatusCode Code, void* pParam)
{
	switch (Code)
	{
	case _http_::Http<_string_type>::StatusCode::EN_CODE_CONNECT_SUCCESS:
	{
		if (sptr->PostData("", 0, "/nkyt-gj-ms/ag/device/download") == -1)
		{//Ê§°Ü

		}
	}
	break;
	case _http_::Http<_string_type>::StatusCode::EN_CODE_REPONES_SUCCESS:
	{//³É¹¦
		printf("HandleHttp:%s\r\n", (char*)pParam);
		if (pParam != NULL)
		{

		}
	}
	break;
	case _http_::Http<_string_type>::StatusCode::EN_CODE_CONNECT_FAILED:
	case _http_::Http<_string_type>::StatusCode::EN_CODE_TIMEOUT:
	case _http_::Http<_string_type>::StatusCode::EN_CODE_EXCEPT:
	case _http_::Http<_string_type>::StatusCode::EN_CODE_REPONES_FAILED:
	{//Ê§°Ü
		int i = 0;
	}
	break;
	}
}

int main(int argc, char* argv[])
{
	LOG_Print_SetLog(libFrameWork_Http, 0);

	_http_::Http<_string_type>::InitEnvironment();

	while (true)
	{
		_http_::Http<_string_type>::self_sptr sptr = _http_::Http<_string_type>::Alloc();
		sptr->Init();
		//sptr->Open("192.168.1.45", 80, HandleHttp);
		sptr->Open("202.105.12.106", 8181, HandleHttpChunk);
		Sleep(5000000);
	}

	std::cin.get();
	return 0;
}