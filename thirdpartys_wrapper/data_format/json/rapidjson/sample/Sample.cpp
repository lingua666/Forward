// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

#include <libThirdWrap_RapidJson/json.h>

typedef rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::CrtAllocator > Document;
typedef rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::CrtAllocator > Value;
Document::AllocatorType allocator;

void TraverseObject(std::string name, const Value& value);

void TraverseArray(std::string name, const Value& Value);


void TraverseObject(std::string name, const Value& value)
{
	printf("TraverseObject：%s\r\n", name.c_str());
	for (Value::MemberIterator itr = const_cast<Value&>(value).MemberBegin(); itr != value.MemberEnd(); itr ++)
	{
		Value& jKey = itr->name;
		Value& jValue = itr->value;
		if (jValue.IsObject())
		{//对象
			TraverseObject(jKey.GetString(), jValue);
		}
		else if (jValue.IsArray())
		{//数组
			TraverseArray(jKey.GetString(), jValue);
		}
		else if (jValue.IsString())
		{//内容
			printf("key:%s, value:%s\r\n", jKey.GetString(), jValue.GetString());
		}
		else
		{
			printf("unKnow\r\n");
		}
	}
}

void TraverseArray(std::string name, const Value& value)
{
	printf("TraverseArray：%s\r\n", name.c_str());
	for (int i = 0; i < const_cast<Value&>(value).Size(); i ++)
	{
		if (value[i].IsObject())
		{//对象
			TraverseObject(name, value[i]);
		}
		else if (value[i].IsArray())
		{//数组
			TraverseArray(name, value[i]);
		}
		else if (value[i].IsString())
		{//内容
			printf("i:%d, value:%s\r\n", i, value[i].GetString());
		}
		else
		{
			printf("unKnow\r\n");
		}
	}
}


void Test(void)
{
	char szData[1024] = { 0 };
	FILE* fp = fopen("G:\\Work\\ProjectCode\\MyTool\\trunk\\Python\\Protocol\\System_V3.0\\json\\proto3.0\\proto\\AccessSvr\\WebSocketJson.txt", "rb");
	if (fp != NULL)
	{
		int iSize = fread(szData, 1, sizeof(szData), fp);
		fclose(fp);

		Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(szData);

		if (!doc.HasParseError())
		{
			for (Value::MemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); itr++)
			{
				Value& jKey = itr->name;
				Value& jValue = itr->value;

				if (jKey.IsString())
				{
					std::string name = jKey.GetString();
					if (jValue.IsObject())
					{
						TraverseObject(name, jValue);
					}
					else if (jValue.IsArray())
					{
						TraverseArray(name, jValue);
					}
					else if (jValue.IsString())
					{
						printf("key:%s, value:%s\r\n", jKey.GetString(), jValue.GetString());
					}
					else
					{
						printf("unKnow!!\r\n");
					}
				}
			}
		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		_string_type s = buffer.GetString();
	}

	std::cin.get();
}

void TestJson(void)
{
	//读操作
	/*{
		char szData[1024] = { 0 };
		FILE* fp = fopen("G:\\Work\\ProjectCode\\MyTool\\trunk\\Python\\Protocol\\System_V3.0\\json\\proto3.0\\proto\\AccessSvr\\WebSocketJson.txt", "rb");
		if (fp != NULL)
		{
			int iSize = fread(szData, 1, sizeof(szData), fp);
			fclose(fp);

			
			rapid::json	json;
			if (json.Load(szData) == 1)
			{
				rapid::DNode& Node = json.FindNode("Gid");
				_string_type s = json.GetText(Node);
				printf("%s\r\n", s.c_str());
				std::cin.get();
				int i = 0;
			}
		}
	}*/

	//写操作
	{
		const char* sc = "{\"Protocol\":{\"888\":\"\u6211\u7231\u5317\u4eac\u5929\u5b89\u95e8\","
			"\"9999\":\"\u5de5\u63a7\u8bbe\u5907\"}}";

		rapid::json Write;
		int iRet1 = Write.Load(sc);
		if (iRet1 != 1)
			return;

		Write.InsertNode(Write.root(), "write", "test");
		rapid::DObject	Obj;
		{
			rapid::DObject Obj1;
			Obj1.Append("000", "111");
			Obj.Append("yes", Obj1);
		}
		Obj.Append("123", "456");
		Obj.Append("3333", "444");
		rapid::DNode& Node = Write.InsertObject(Write.root(), "obj", Obj);
		Write.InsertObject(Write.root(), "obj1", Obj);
		Write.InsertObject(Write.root(), "obj2", Obj);

		rapid::DObject Obj1;
		Obj1.Append("000", "111");

		rapid::DObject Obj2;
		Obj2.Append("000", "111");

		rapid::DArray	array;
		array.Append(Obj1);
		array.Append(Obj2);
		Write.InsertArray(Node, "write1", array);

		//Write.SaveFile("D:\\1.TXT");

		_string_type s1 = Write.GetString();
		printf("json:%s\r\n", s1.c_str());
	}

	std::cin.get();
}


int main(int argc, char* argv[])
{
	TestJson();
	//Test();

	/*typedef rapidjson::Document Document;
	typedef rapidjson::GenericValue<rapidjson::UTF8<> > Value;*/

	Document doc;

	/*char*  szData = "{\"serviceData\": [{\"serviceData\": {\"batteryLevel\": 1,\"dd\":{\"bb\":2}}, \"serviceId\"\
	: \"Battery\"\
	}, { \"serviceData\": {\"signalStrength\": -65,\"dd\":{\"bb\":2}}, \"serviceId\" : \"Meter\" }, { \"\
	serviceData\": {\"imsi\": \"460111173221563\", \"timestamp\": \"121527160 - 11 - 28 16:42 : 34\
	\", \"version\": -127, \"imei\": \"867726030082111\",\"dd\":{\"bb\":2}}, \"serviceId\": \"UserDataHead\"}, {\"\
	serviceData\": {\"Alarm2\": \"on\", \"GPSstate\": \"no\", \"Alarm4\": \"on\", \"Alarm1\": \"on\",\
	\"MainPowerInfo\" : \"no\", \"Alarm3\" : \"on\",\"dd\":{\"bb\":2} }, \"serviceId\":\"flag\"}], \"createTime\" : \"2\
	018 - 12 - 03 17:51 : 35\", \"lastMessageTime\": \"2018 - 12 - 15 06 : 37 : 53\", \"devSerial\": \"867\
	726030082111\", \"iotEventTime\": \"2018 - 12 - 15 06 : 37 : 52\"}";*/

	//char*  szData = "{\"serviceData\": \"123\"}";

	/*char*  szData = "{\"serviceData\": [{\"serviceData\": {\"batteryLevel\": 1}, \"serviceId\"\
	: \"Battery\"\
	}, { \"serviceData\": {\"signalStrength\": -65}, \"serviceId\" : \"Meter\" }, { \"\
	serviceData\": {\"imsi\": \"460111173221563\", \"timestamp\": \"121527160 - 11 - 28 16:42 : 34\
	\", \"version\": -127, \"imei\": \"867726030082111\"}, \"serviceId\": \"UserDataHead\"}, {\"\
	serviceData\": {\"Alarm2\": \"on\", \"GPSstate\": \"no\", \"Alarm4\": \"on\", \"Alarm1\": \"on\",\
	\"MainPowerInfo\" : \"no\", \"Alarm3\" : \"on\"}, \"serviceId\":\"flag\"}], \"createTime\" : \"2\
	018 - 12 - 03 17:51 : 35\", \"lastMessageTime\": \"2018 - 12 - 15 06 : 37 : 53\", \"devSerial\": \"867\
	726030082111\", \"iotEventTime\": \"2018 - 12 - 15 06 : 37 : 52\"}";*/

	const char* szData = "{\"s23\":{\"s13\":\"00\"}, \"project\":\"rapidjson\",\"stars\":101, \"arrary\":[{\"a11\":\"a00\"},{\"a11\":\"a11\"}]}";

	while (true)
	{
		//test
		/*rapid::DataNode::DTNode Value12;
		_string_type sr3 = "1233";
		Value12.SetString(sr3.c_str(), allocator);
		//test

		const char* sc = "{\"Protocol\":{\"888\":\"\u6211\u7231\u5317\u4eac\u5929\u5b89\u95e8\","
			"\"9999\":\"\u5de5\u63a7\u8bbe\u5907\"}}";
		rapid::json Write;
		int iRet1 = Write.Load(sc);
		rapid::DataNode Root;// = Write.root();
		Root.Append("test", "test1");
		rapid::DataObject::DataMap Map;
		Map.insert(std::make_pair("t1", "a1"));
		Map.insert(std::make_pair("t2", "a2"));
		Map.insert(std::make_pair("t3", "a3"));
		rapid::DataObject Object;
		Object.Append(Map);
		rapid::DataArray Array;
		Array.Append(Object);
		Array.Append(Object);
		Root.Append("ob", Array);

		rapid::DataObject Object1;
		Object1.Append("ob22", Object);
		Root.Append("ob", Object1);
		rapid::DataArray Array1;
		Object1.Append("ar1", Array1);
		Object1.Append("ar2", Array1);
		Root.Append("ob2", Array);

		_string_type s1 = Write.GetString();
		printf("json:%s\r\n", s1.c_str());

		rapid::json json;
		int iRet = json.Load(szData);*/

		/*rapid::DataNode Node;
		if ((Node = json.FindNode("project")))
		{
			json.WriteNodeText(Node, "123");
			printf("%s:%s\r\n", Node.GetName().c_str(), json.GetNodeText(Node).c_str());
			//Node.Append("s12" , "s03");

			//json.RemoveNode(Node);
			_string_type s = json.GetString();
			printf("json:%s\r\n", s.c_str());
		}

		//rapid::DataNode  Node = json.FindNode("iotEventTime");
		Sleep(100);*/
	}

	{
		// 1. 把 JSON 解析至 DOM。
		const char* json = "{\"project\":\"rapidjson\",\"stars\":101, \"s23\":{\"s13\":\"00\"},\"arrary\":[{\"a11\":\"a00\"},{\"a11\":\"a11\"}]}";
		/*const	char*  json = "{\"serviceData\": [{\"serviceData\": {\"batteryLevel\": 1,\"dd\":{\"bb\":2}}, \"serviceId\"\
		: \"Battery\"\
		}, { \"serviceData\": {\"signalStrength\": -65,\"dd\":{\"bb\":2}}, \"serviceId\" : \"Meter\" }, { \"\
		serviceData\": {\"imsi\": \"460111173221563\", \"timestamp\": \"121527160 - 11 - 28 16:42 : 34\
		\", \"version\": -127, \"imei\": \"867726030082111\",\"dd\":{\"bb\":2}}, \"serviceId\": \"UserDataHead\"}, {\"\
		serviceData\": {\"Alarm2\": \"on\", \"GPSstate\": \"no\", \"Alarm4\": \"on\", \"Alarm1\": \"on\",\
		\"MainPowerInfo\" : \"no\", \"Alarm3\" : \"on\",\"dd\":{\"bb\":2} }, \"serviceId\":\"flag\"}], \"createTime\" : \"2\
		018 - 12 - 03 17:51 : 35\", \"lastMessageTime\": \"2018 - 12 - 15 06 : 37 : 53\", \"devSerial\": \"867\
		726030082111\", \"iotEventTime\": \"2018 - 12 - 15 06 : 37 : 52\"}";*/

		Document d;
		d.Parse(json);

		// 2. 利用 DOM 作出修改。
		/*Value& s = d["stars"];
		s.SetInt(s.GetInt() + 1);
		d["stars"].SetString("123453");
		d.AddMember(rapidjson::StringRef("test"), rapidjson::StringRef("yyy"), allocator);
		d["s23"].AddMember(rapidjson::StringRef("56"), rapidjson::StringRef("777"), allocator);
		d.RemoveMember("s13");
		d["arrary"].Erase(&d["arrary"][0]);*/

		// 3. 把 DOM 转换（stringify）成 JSON。
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		// Output {"project":"rapidjson","stars":11}
		std::cout << buffer.GetString() << std::endl;
		return 0;
	}

	doc.Parse<rapidjson::kParseDefaultFlags>(szData);

	if (doc.HasParseError())
	{
		int i = 0;
	}

	int is = doc.MemberCount();

	if (doc.HasMember("batteryLevel"))
	{
		const char* sz = doc["batteryLevel"].GetString();
		int i = 0;
	}

	if (!doc.HasParseError())
	{
		for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); itr++)
		{
			Value jKey;
			Value jValue;
			jKey.CopyFrom(itr->name, allocator);
			jValue.CopyFrom(itr->value, allocator);
			jValue.SetString("11111111", allocator);
			/*rapidjson::Document::AllocatorType allocator;

			jValue.AddMember("storage.photo.count", storage_photo_count, allocator);
			jValue.AddMember("storage.music.count", storage_music_count, allocator);*/

			//jValue.AddMember();

			if (jKey.IsString())
			{
				std::string name = jKey.GetString();
				printf("\r\nname(%d): %s\r\n", jValue.GetType(), name.c_str());
				switch (jValue.GetType())
				{
				case rapidjson::Type::kArrayType:
				{
					for (int i = 0; i < jValue.Size(); i++)
					{
						switch (jValue[i].GetType())
						{
						case rapidjson::Type::kArrayType:
						{

						}
						break;
						case rapidjson::Type::kObjectType:
						{
							for (Value::ConstMemberIterator itr1 = jValue[i].MemberBegin(); itr1 != jValue[i].MemberEnd(); itr1++)
							{
								Value jKey1;
								Value jValue1;
								jKey1.CopyFrom(itr1->name, allocator);
								jValue1.CopyFrom(itr1->value, allocator);

								int i = 0;
							}
						}
						break;
						}
					}
				}
				break;
				case rapidjson::Type::kObjectType:
				{

				}
				break;
				}
			}
		}
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	_string_type s = buffer.GetString();

	std::cin.get();
	return 0;
}