
#ifndef __FRAMEWORK_GB28181_PROTOCOL_20200517181513_1445757319_H__
#define __FRAMEWORK_GB28181_PROTOCOL_20200517181513_1445757319_H__

#include "External.h"

namespace _gb28181_ {

	typedef _gb_string	_proto_string;
	typedef _gb_string_big	_proto_string_big;

	class CComandType
	{
	public:
		CComandType(void)
		{

		}

		int	Parse(const _proto_string& sData)
		{
			return _CXml.Load(sData.c_str());
		}

		bool Equal(const _proto_string& sType)
		{
			if (strcmp(GetCommandType().c_str(), sType.c_str()) == 0)
				return true;
			return false;
		}

		bool Change(const _proto_string& sName, const _proto_string& Content)
		{
			if (_CXml.WriteNodeText(sName, Content))
				return true;

			return false;
		}

		_proto_string GetCommandType(void)
		{
			xml_node xNode = _CXml.FindNode("CmdType");
			return _CXml.GetNodeText(xNode);
		}

		_pugixml_::CTiXml&	GetHandle(void)
		{
			return _CXml;
		}

		_proto_string_big GetString(void)
		{
			return _CXml.GetXmlString();
		}

	private:
		_pugixml_::CTiXml _CXml;
	};

}

#endif//__APPS_PROTOCOL_20151025151519_1445757319_H__
