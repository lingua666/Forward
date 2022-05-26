
#ifndef __GB28181_20210309161822_1615277902_H__
#define __GB28181_20210309161822_1615277902_H__

#include "Apps_Protocol_XML.hpp"

namespace _gb28181_ {

	struct Catalog
	{
		struct CQuery
		{
			_gb_string CmdType;
			_gb_string SN;
			_gb_string DeviceID;

			CQuery(void)
			{}

			CQuery(const CQuery& t1)
			{
				this->CmdType = t1.CmdType;
				this->SN = t1.SN;
				this->DeviceID = t1.DeviceID;
			}

			CQuery& operator=(const CQuery& t1)
			{
				this->CmdType = t1.CmdType;
				this->SN = t1.SN;
				this->DeviceID = t1.DeviceID;
				return *this;
			}

			int Load(CComandType& CType)
			{
				_pugixml_::CTiXml& CXml = CType.GetHandle();

				xml_node xNode;
				xNode = CXml.FindNode("Query", "CmdType");
				if (xNode)
				{
					this->CmdType = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Query", "SN");
				if (xNode)
				{
					this->SN = CXml.GetNodeText(xNode);
				}

				xNode = CXml.FindNode("Query", "DeviceID");
				if (xNode)
				{
					this->DeviceID = CXml.GetNodeText(xNode);
				}

				return 1;
			}

			void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent, "CmdType", this->CmdType);
				xNode = CXml.InsertNode(Parent, "SN", this->SN);
				xNode = CXml.InsertNode(Parent, "DeviceID", this->DeviceID);
			}
		};

		CQuery Query;

		Catalog(bool isPrimary)
		{}

		Catalog(void)
		{
			this->Query.CmdType = "Catalog";
		}

		Catalog(const Catalog& t1)
		{
			this->Query = t1.Query;
		}

		Catalog& operator=(const Catalog& t1)
		{
			this->Query = t1.Query;
			return *this;
		}

		int Load(CComandType& CType)
		{
			_pugixml_::CTiXml& CXml = CType.GetHandle();

			xml_node xNode;
			this->Query.Load(CType);
			return 1;
		}

		_gb_string_big Save(void)
		{
			_pugixml_::CTiXml CXml;
			xml_node Per = CXml.InitHeader("1.0", "UTF-8");
			if (!Per)
				return "";

			xml_node xNode;
			xNode = CXml.Append("Query", "");
			Query.Save(CXml, xNode);
			return CXml.GetXmlString();
		}
	};

} //namespace _GB28181_

#endif//__GB28181_20210309161822_1615277902_H__

