// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libFrameWork_Http/Http.h>

#include <libThirdWrap_RapidJson/json.h>

typedef _foundation_::_string_t		_proto_string;
typedef _foundation_::_string_big_t	_proto_string_big;

typedef rapid::json				DFormat_type;
typedef rapid::DNode			DNode;
typedef rapid::DObject			DObject;
typedef rapid::DArray			DArray;

struct Report
{
	struct CProtocol
	{
		_proto_string midId;
		_proto_string id;
		_proto_string state;
		_proto_string value;
		Int64 sendTime;

		CProtocol(void)
		{}

		CProtocol(const CProtocol& t1)
		{
			this->midId = t1.midId;
			this->id = t1.id;
			this->state = t1.state;
			this->value = t1.value;
			this->sendTime = t1.sendTime;
		}

		CProtocol& operator=(const CProtocol& t1)
		{
			this->midId = t1.midId;
			this->id = t1.id;
			this->state = t1.state;
			this->value = t1.value;
			this->sendTime = t1.sendTime;
			return *this;
		}

		int Load(DFormat_type& Handle, const DNode& xNode)
		{
			if (xNode.HasMember("midId"))
			{
				this->midId = Handle.GetText(xNode["midId"]);
			}

			if (xNode.HasMember("id"))
			{
				this->id = Handle.GetText(xNode["id"]);
			}

			if (xNode.HasMember("state"))
			{
				this->state = Handle.GetText(xNode["state"]);
			}

			if (xNode.HasMember("value"))
			{
				this->value = Handle.GetText(xNode["value"]);
			}

			if (xNode.HasMember("sendTime"))
			{
				Handle.GetInt(xNode["sendTime"], this->sendTime);
			}

			return 1;
		}

		void Save(DFormat_type& Hanle, const DNode& Parent)
		{
			DObject Object;
			Hanle.InsertNode(Parent, "midId", this->midId);
			Hanle.InsertNode(Parent, "id", this->id);
			Hanle.InsertNode(Parent, "state", this->state);
			Hanle.InsertNode(Parent, "value", this->value);
			Hanle.InsertNode(Parent, "sendTime", this->sendTime);
		}
	};

	CProtocol Protocol;

	Report(bool isPrimary)
	{

	}

	Report(void)
	{

	}

	Report(const Report& t1)
	{
		this->Protocol = t1.Protocol;
	}

	Report& operator=(const Report& t1)
	{
		this->Protocol = t1.Protocol;
		return *this;
	}

	int Load(const char* c_szData)
	{
		DFormat_type Hanle;
		if (Hanle.Load(c_szData) == -1)
			return -1;

		this->Protocol.Load(Hanle, Hanle.root());
		return 1;
	}

	_proto_string_big Save(void)
	{
		char c_szData[] = "{}";

		DFormat_type Hanle;
		if (Hanle.Load(c_szData) != 1)
			return "";

		Protocol.Save(Hanle, Hanle.root());
		return Hanle.GetString();
	}
};

struct ReportACK
{
	struct CProtocol
	{
		Int64 code;
		Int64 success;
		_proto_string msg;

		CProtocol(void)
		{}

		CProtocol(const CProtocol& t1)
		{
			this->code = t1.code;
			this->success = t1.success;
			this->msg = t1.msg;
		}

		CProtocol& operator=(const CProtocol& t1)
		{
			this->code = t1.code;
			this->success = t1.success;
			this->msg = t1.msg;
			return *this;
		}

		int Load(DFormat_type& Handle, const DNode& xNode)
		{
			if (xNode.HasMember("code"))
			{
				Handle.GetInt(xNode["code"], this->code);
			}

			if (xNode.HasMember("success"))
			{
				Handle.GetInt(xNode["success"], this->success);
			}

			if (xNode.HasMember("msg"))
			{
				this->msg = Handle.GetText(xNode["msg"]).utf8_gbk();
			}

			return 1;
		}

		void Save(DFormat_type& Hanle, const DNode& Parent)
		{
			DObject Object;
			Hanle.InsertNode(Parent, "code", this->code);
			Hanle.InsertNode(Parent, "success", this->success);
			Hanle.InsertNode(Parent, "msg", this->msg);
		}
	};

	CProtocol Protocol;

	ReportACK(bool isPrimary)
	{

	}

	ReportACK(void)
	{

	}

	ReportACK(const ReportACK& t1)
	{
		this->Protocol = t1.Protocol;
	}

	ReportACK& operator=(const ReportACK& t1)
	{
		this->Protocol = t1.Protocol;
		return *this;
	}

	int Load(const char* c_szData)
	{
		DFormat_type Hanle;
		if (Hanle.Load(c_szData) != 1)
			return -1;

		this->Protocol.Load(Hanle, Hanle.root());
		return 1;
	}

	_proto_string_big Save(void)
	{
		char c_szData[] = "{}";

		DFormat_type Hanle;
		if (Hanle.Load(c_szData) == -1)
			return "";

		Protocol.Save(Hanle, Hanle.root());
		return Hanle.GetString();
	}
};

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

void HandleHttpChunk(const _http_::Http<_string_type>::self_sptr& sptr,
	_http_::Http<_string_type>::StatusCode Code, void* pParam)
{
	switch (Code)
	{
	case _http_::Http<_string_type>::StatusCode::EN_CODE_CONNECT_SUCCESS:
	{
		_string_type s = _file_::RWIni::GetCurrentPath();
		s.append("\\Config\\App.ini");

		Report Req;
		Req.Protocol.midId = _file_::RWIni::Read("Config", "midId", s.c_str()).c_str();//"Mid001";
		Req.Protocol.id = _file_::RWIni::Read("Config", "id", s.c_str()).c_str();// "Device0072";
		Req.Protocol.state = "1";

		/*Req.Protocol.value = "ksk33";
		Req.Protocol.sendTime = 1654132425484;*/

		int Number = _file_::RWIni::ReadInt("Config", "SendCount", s.c_str());

		LOG_Print_Info(libFrameWork_Http, "===========Send Begin===========",
			Req.Protocol.midId.c_str(), Req.Protocol.id, Number);

		LOG_Print_Info(libFrameWork_Http, "midId:%s, id:%s, SendCount:%d",
			Req.Protocol.midId.c_str(), Req.Protocol.id.c_str(), Number);

		for (int i = 0; i < Number; i++)
		{
			Req.Protocol.value = _string_type::NumberToStr(i).c_str();
			Req.Protocol.sendTime = Timestamp_type().epochMicroseconds() / 1000;

			_proto_string_big sRet = Req.Save();
			if (sptr->PostData(sRet.c_str(), sRet.size(), "/device-data/receive") == -1)
			{//Ê§°Ü
				printf("(%d)Send Data Faild22222!!!\r\n", i);
			}
		}

		/*if (sptr->PostData("", 0, "/nkyt-gj-ms/ag/device/download") == -1)
		{//Ê§°Ü

		}*/
	}
	break;
	case _http_::Http<_string_type>::StatusCode::EN_CODE_REPONES_SUCCESS:
	{//³É¹¦
		printf("HandleHttp(%d):%s\r\n", Code, (char*)pParam);
		if (pParam != NULL)
		{
			ReportACK Ack;
			int iRet = Ack.Load((char*)pParam);

			if (pParam != NULL)
			{

			}
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
		sptr->SetMode(false);

		_http_::Http<_string_type>::tagHost_INFO HostInfo;
		_http_::Http<_string_type>::GetHostInfo("http://trosent.net:53003/device-data/receive", &HostInfo);
		sptr->Open(HostInfo.IP.c_str(), HostInfo.uPort, HandleHttpChunk);

		//sptr->Open("192.168.1.45", 80, HandleHttp);
		//sptr->Open("192.168.1.58", 8181, HandleHttpChunk);
		//sptr->Open("127.0.0.1", 8181, HandleHttpChunk);
		
		Sleep(5000000);
	}

	std::cin.get();
	return 0;
}