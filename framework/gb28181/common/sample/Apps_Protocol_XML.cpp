
#include "Apps_Protocol_XML.hpp"

namespace _apps_proto_ {

	namespace _xml_ {

		_proto_string CComandType::GenerateUIDIndex(const _proto_string& sUid, int iIndex)
		{
			return sUid + "_" + _proto_string::NumberToStr(iIndex);
		}

		_proto_string CComandType::GetUidToInfo(const _proto_string& Info)
		{
			int iPos = Info.find("_");
			if (iPos == -1)
				return "";

			return Info.substr(0, iPos);
		}

		_proto_string CComandType::GetIndexToInfo(const _proto_string& Info)
		{
			int iPos = Info.find("_");
			if (iPos == -1)
				return "";

			return Info.substr(iPos + 1);
		}

		CComandType::CComandType(void)
		{

		}

		int	CComandType::Parse(const _proto_string& sData)
		{
			return _CXml.Load(sData.c_str());
		}

		bool CComandType::Equal(const _proto_string& sType)
		{
			if (strcmp(GetCommandType().c_str(), sType.c_str()) == 0)
				return true;
			return false;
		}

		bool CComandType::Change(const _proto_string& sName, const _proto_string& Content)
		{
			if (_CXml.WriteNodeText(sName, Content))
				return true;

			return false;
		}

		_proto_string CComandType::GetCommandType(void)
		{
			xml_node xNode = _CXml.FindNode("CmdType");
			return _CXml.GetNodeText(xNode);
		}

		_pugixml_::CTiXml&	CComandType::GetHandle(void)
		{
			return _CXml;
		}

		_proto_string_big CComandType::GetString(void)
		{
			return _CXml.GetXmlString();
		}

	}

}

