
#ifndef __APPS_CBC_MODBUS_TOOL_CONFIG_20200831193628_1598873788_H__
#define __APPS_CBC_MODBUS_TOOL_CONFIG_20200831193628_1598873788_H__

#include <libThirdWrap_Xml/XML.h>

namespace	_app_ {

	typedef _pugixml_::_string_t _app_string;
	typedef _pugixml_::_string_big_t _app_string_big_t;

	struct Config
	{
		struct CCommandInfo
		{
			_app_string CommandName;

			CCommandInfo(void)
			{}

			CCommandInfo(const CCommandInfo& t1)
			{
				this->CommandName = t1.CommandName;
			}

			CCommandInfo& operator=(const CCommandInfo& t1)
			{
				this->CommandName = t1.CommandName;
				return *this;
			}

			int Load(_pugixml_::CTiXml& CXml)
			{
				xml_node xNode;
				xNode = CXml.FindNode("CommandInfo", "CommandName");
				if (xNode)
				{
					this->CommandName = CXml.GetNodeText(xNode);
				}

				return 1;
			}

			void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent, "CommandName", this->CommandName);
			}
		};

		struct CLocal
		{
			_app_string IP;
			_app_string Port;
			_app_string Addr485;
			_app_string StartReg;
			_app_string RegCount;
			_app_string DeviceType;
			_app_string UsageType;

			CLocal(void)
			{}

			CLocal(const CLocal& t1)
			{
				this->IP = t1.IP;
				this->Port = t1.Port;
				this->Addr485 = t1.Addr485;
				this->StartReg = t1.StartReg;
				this->RegCount = t1.RegCount;
				this->DeviceType = t1.DeviceType;
				this->UsageType = t1.UsageType;
			}

			CLocal& operator=(const CLocal& t1)
			{
				this->IP = t1.IP;
				this->Port = t1.Port;
				this->Addr485 = t1.Addr485;
				this->StartReg = t1.StartReg;
				this->RegCount = t1.RegCount;
				this->DeviceType = t1.DeviceType;
				this->UsageType = t1.UsageType;
				return *this;
			}

			int Load(_pugixml_::CTiXml& CXml)
			{
				xml_node xNode;
				xNode = CXml.FindNode("Local", "IP");
				if (xNode)
				{
					this->IP = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Local", "Port");
				if (xNode)
				{
					this->Port = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Local", "Addr485");
				if (xNode)
				{
					this->Addr485 = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Local", "StartReg");
				if (xNode)
				{
					this->StartReg = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Local", "RegCount");
				if (xNode)
				{
					this->RegCount = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Local", "DeviceType");
				if (xNode)
				{
					this->DeviceType = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Local", "UsageType");
				if (xNode)
				{
					this->UsageType = CXml.GetNodeText(xNode);
				}

				return 1;
			}

			void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent, "IP", this->IP);
				xNode = CXml.InsertNode(Parent, "Port", this->Port);
				xNode = CXml.InsertNode(Parent, "Addr485", this->Addr485);
				xNode = CXml.InsertNode(Parent, "StartReg", this->StartReg);
				xNode = CXml.InsertNode(Parent, "RegCount", this->RegCount);
				xNode = CXml.InsertNode(Parent, "DeviceType", this->DeviceType);
				xNode = CXml.InsertNode(Parent, "UsageType", this->UsageType);
			}
		};

		CCommandInfo CommandInfo;
		CLocal Local;
		_pugixml_::CTiXml CXml;

		Config(bool isPrimary)
		{

		}

		Config(void)
		{

		}

		Config(const Config& t1)
		{
			this->CommandInfo = t1.CommandInfo;
			this->Local = t1.Local;
		}

		Config& operator=(const Config& t1)
		{
			this->CommandInfo = t1.CommandInfo;
			this->Local = t1.Local;
			return *this;
		}

		int Load(const char* Path)
		{
			if (CXml.LoadFile(Path) == -1)
				return -1;

			xml_node xNode;
			this->CommandInfo.Load(CXml);
			this->Local.Load(CXml);
			return 1;
		}

		_pugixml_::_string_big_t Save(void)
		{
			char c_szData[] = "<?xml version=\"1.0\" Encoding=\"UTF-8\"?>\
					<Protocol Version=\"V1.1\">\
					</Protocol>";

			_pugixml_::CTiXml CXml;
			if (CXml.Load(c_szData) == -1)
				return "";

			xml_node xNode;
			xNode = CXml.InsertNode(CXml.root(), "CommandInfo", "");
			CommandInfo.Save(CXml, xNode);
			xNode = CXml.InsertNode(CXml.root(), "Local", "");
			Local.Save(CXml, xNode);
			return CXml.GetXmlString();
		}
	};

}

#endif//__CONFIG_20200831193628_1598873788_H__

