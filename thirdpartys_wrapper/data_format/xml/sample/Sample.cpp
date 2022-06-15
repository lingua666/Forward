// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libCommon/os/Platform.h>
#include "common.h"

//#include "TestXml.h"

//using namespace _proto_;

#if defined(PLATFORM_OS_FAMILY_UNIX) && PLATFORM_OS != PLATFORM_OS_ANDROID
	int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)  
	{  
		iconv_t cd;  
		int rc;  
		char **pin = &inbuf;  
		char **pout = &outbuf;  

		cd = iconv_open(to_charset,from_charset);  
		if (cd==0)  
			return -1;  
		memset(outbuf,0,outlen);  
		if (iconv(cd,pin,&inlen,pout,&outlen) == -1)  
			return -1;  
		iconv_close(cd);  
		return 0;  
	}  

	int u2g(char *inbuf,int inlen,char *outbuf,int outlen)  
	{  
		return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);  
	}  

	int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)  
	{  
		return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);  
	}  
#elif defined(PLATFORM_OS_FAMILY_WINDOWS)

#endif

void XmlTest( void )
{
	_pugixml_::_string_t d11 = "<?xml version='1.0' encoding='UTF-8'?><CommandUID>车道指示器</CommandUID>";
	//_pugixml_::_string_t d33 = "<?xml version='1.0' encoding='UTF-8'?><Protocol Version='V1.1'><CommandInfo><CommandUID>1A00000Dd8x9201609200919290620301C00000I</CommandUID><ServerUID>1B000001510020160530000000000ZZZ00000000</ServerUID><ClientUID>1B000001510020160918000000000ZZZ00000001</ClientUID><ModuleUID></ModuleUID><CommandName>GetDevStatus</CommandName><SendTime>20160920091929062</SendTime></CommandInfo><Content><PLC_UID>1000000E510020151201401122001Y016zG4NlBs</PLC_UID><PLC_Index>0</PLC_Index><PLC_MemType>1</PLC_MemType><PLC_MemAdd>100</PLC_MemAdd><MemAddModule></MemAddModule><MemAddBitNum></MemAddBitNum><PLC_MemValueType>1</PLC_MemValueType><PLC_MemValue>0</PLC_MemValue><PLC_MemName>车道指示器</PLC_MemName><PLC_MemUID>10000000000020160919401122001ZZZPshL6aXY</PLC_MemUID><PLC_Number>0</PLC_Number></Content></Protocol>";
	_pugixml_::_string_t d33 = "<?xml version='1.0' encoding='UTF-8'?><Protocol Version='V1.1'><CommandInfo><CommandUID>1A00000Dolqf201609200929124090301C00000I</CommandUID><ServerUID>1B000001510020160530000000000ZZZ00000000</ServerUID><ClientUID>1B000001510020160918000000000ZZZ00000001</ClientUID><ModuleUID></ModuleUID><CommandName>GetDevStatus</CommandName><SendTime>20160920092912409</SendTime></CommandInfo><Content><PLC_UID>1000000E510020151201401122001Y016zG4NlBs</PLC_UID><PLC_Index>0</PLC_Index><PLC_MemType>1</PLC_MemType><PLC_MemAdd>100</PLC_MemAdd><MemAddModule></MemAddModule><MemAddBitNum></MemAddBitNum><PLC_MemValueType>1</PLC_MemValueType><PLC_MemValue>0</PLC_MemValue><PLC_MemName>车道指示器</PLC_MemName><PLC_MemUID>10000000000020160920401122001ZZZiFyYDlsJ</PLC_MemUID><PLC_Number>0</PLC_Number></Content></Protocol>";
	_pugixml_::_string_t d22 = d33.gbk_utf8();
	printf("gbk_utf8:%s\r\n", d22.c_str());

#if defined(PLATFORM_OS_FAMILY_UNIX) && PLATFORM_OS != PLATFORM_OS_ANDROID
	char szUtf8[500] = {0};
	g2u((char*)d33.c_str(), d33.size(), szUtf8, sizeof(szUtf8));
	printf("szUtf8:%s\r\n", szUtf8);
#endif

	const char* data1 = "<?xml version='1.0' encoding='UTF-8'?>\
						   <Protocol1 Version='V1.1'>\
						   <CommandInfo>\
						   <CommandUID></CommandUID>\
						   <ServerUID></ServerUID> \
						   <ClientUID></ClientUID> \
						   <CommandName>LoginIn</CommandName>\
						   <ModuleUID>\
						   <ServerUID></ServerUID> \
						   <ClientUID></ClientUID> \
						   <CommandUID>\
						   <ClientUID></ClientUID>\
						   </CommandUID> \
						   </ModuleUID> \
						   <SendTime></SendTime>\
						   </CommandInfo>\
						   <Content>\
						   <UpdataServerInfoResult>0/1</UpdataServerInfoResult> \
						   </Content>\
						   </Protocol1>";

	const char* data2 = "<?xml version='1.0' encoding='UTF-8'?>\
						<Protocol2 Version='V1.1'>\
						<CommandInfo>\
						<CommandUID></CommandUID>\
						<ServerUID></ServerUID> \
						<ClientUID></ClientUID> \
						<ModuleUID></ModuleUID> \
						<CommandName>UpdataServerInfoResult</CommandName>\
						<SendTime></SendTime>\
						</CommandInfo>\
						<Content>\
						<UpdataServerInfoResult1>0/1/2</UpdataServerInfoResult1> \
						</Content>\
						</Protocol2>";

	CComandType YWE;
	if( YWE.Parse(d22.c_str()) == -1 )
	{
		printf("XML Parse失败!!\r\n");
		return ;
	}
	_pugixml_::_string_t s989 = YWE.GetHandle().GetName(YWE.GetHandle().root());
	const char* sz23 = d22.c_str();
	_pugixml_::_string_t sssss = YWE.GetHandle().GetNodeText(YWE.GetHandle().FindNode("PLC_MemName"));
	printf("XmlTest:%s\r\n", sssss.c_str());

	/*CComandType	CType;
	if( CType.Parse(data1) == -1 )
		return ;

	std::string sx;

	if( CType == "LoginIn" )
	{
		LoginIn s;
		s.Load(CType);
		sx = s.Save();
		LoginIn s1;

		CComandType	CType1;
		if( CType1.Parse(sx.c_str()) == -1 )
			return ;

		s1.Load(CType1);
		int i = 0;
		i ++;
	}

	CComandType	CType2;
	if( CType2.Parse(data2) == -1 )
		return ;

	if( CType2 == "UpdataServerInfoResult" )
	{
		UpdataServerInfoResult s;
		s.Load(CType2);
		std::string sx = s.Save();
		int i = 0;
	}*/
}

int main(int argc, char* argv[])
{
	XmlTest();
	/*{
		char sz[] = "<?xml version=\"1.0\" encoding=\"GBK\"?>\
					<root>\
					<ip at = \"v\">192.168.1.1</ip>\
					<ip ct=\"1\">192.168.2.1</ip>\
					<test>\
					<ip at1 = \"v\">192.168.3.1</ip>\
					<ip ct1 =\"1\">192.168.4.1</ip>\
					</test>\
					</root>";
		int iNum = 100000000;
		int iTimer = GetTickCount();
		for(int i = 0; i < iNum; i ++)
		{
			_pugixml_::CTiXml	xml;
			int iResult = xml.Load(sz);
		}
		printf("CTiXml Load timer:%d\r\n", GetTickCount() - iTimer);
	}*/

	//XmlTest();

	/*{
		char szContent[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
						   <Protocol Version=\"V1.1\">\
						   <CommandInfo>\
						   <CommandUID></CommandUID>\
						   <ServerUID></ServerUID>\
						   <ClientUID></ClientUID>\
						   <ModuleUID></ModuleUID>\
						   <CommandName>LoginIn</CommandName>";

		_pugixml_::CTiXml	xml;
		std::string sText;
		int iResult = xml.Load(szContent);
		xml_node xNode = xml.FindNode("CommandInfo","CommandName");
		if( xNode )
		{
			std::string s = xml.GetNodeText(xNode);
			int y = 0;
			y ++;
		}
	}

	{
		char sz[] = "<?xml version=\"1.0\" encoding=\"GBK\"?>\
					<root>\
					<ip at = \"v\">192.168.1.1</ip>\
					<ip ct=\"1\">192.168.2.1</ip>\
					<port pt=\"2\">88888</port>\
					<test>\
					<ip at1 = \"v\">192.168.3.1</ip>\
					<ip ct1 =\"1\">192.168.4.1</ip>\
					</test>\
					</root>";

		_pugixml_::CTiXml	xml;
		std::string sText;
		int iResult = xml.Load(sz);

		std::string s123;

		xml_node nd123 = xml.FindNodeToAttribute("test","ip","ct1");
		xml.GetNodeAttributeValue(xml.FindNode("ip"),"at",s123);

		xml_node nd = xml.FindNodeToAttribute("ip","ct");
		xml.GetNodeAttributeValue(xml.FindNode("ip"),"at",s123);

		xml.SetNodeText(xml.FindNode("ip"),"11123");
		xml.SetNodeAttribute(xml.FindNode("ip"),"atrr","value");

		xml.WriteNodeText("port","979797");
		xml.WriteNodeAttributeValue("port","pt","55555");

		sText = xml.GetXmlString();

		//xml.GetNodeText(xml.FindNode("ip"),sText);
	}*/

	std::cin.get();
	return 0;
}