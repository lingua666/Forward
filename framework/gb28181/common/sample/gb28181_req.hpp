
#ifndef __GB28181_REQ_20210310152316_1615360996_H__
#define __GB28181_REQ_20210310152316_1615360996_H__

#include "Apps_Protocol_XML.hpp"
#include <libTimestamp/Timestamp.h>

namespace _apps_proto_ {

	namespace _xml_ {
	
		namespace _gb28181_req_ {
		
			struct Keepalive
			{
				struct CNotify
				{
					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;
					_proto_string Status;
					
					CNotify( void )
					{}
					
					CNotify( const CNotify& t1 )
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->Status = t1.Status;
					}
					
					CNotify& operator=( const CNotify& t1 )
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->Status = t1.Status;
						return *this;
					}
					
					int Load( CComandType& CType )
					{
						_pugixml_::CTiXml& CXml = CType.GetHandle();
						
						xml_node xNode;
						xNode = CXml.FindNode("Notify","CmdType");
						if( xNode )
						{
							this->CmdType = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Notify","SN");
						if( xNode )
						{
							this->SN = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Notify","DeviceID");
						if( xNode )
						{
							this->DeviceID = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Notify","Status");
						if( xNode )
						{
							this->Status = CXml.GetNodeText(xNode);
						}
						
						return 1;
					}
					
					void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
					{
						xml_node xNode;
						xNode = CXml.InsertNode(Parent,"CmdType",this->CmdType);
						xNode = CXml.InsertNode(Parent,"SN",this->SN);
						xNode = CXml.InsertNode(Parent,"DeviceID",this->DeviceID);
						xNode = CXml.InsertNode(Parent,"Status",this->Status);
					}
				};
				
				CNotify Notify;
				
				Keepalive( bool isPrimary )
				{}
				
				Keepalive( void )
				{
					this->Notify.CmdType = "Keepalive";
				}
				
				
				Keepalive( const Keepalive& t1 )
				{
					this->Notify = t1.Notify;
				}
				
				Keepalive& operator=( const Keepalive& t1 )
				{
					this->Notify = t1.Notify;
					return *this;
				}
				
				int Load( CComandType& CType )
				{
					_pugixml_::CTiXml& CXml = CType.GetHandle();
					
					xml_node xNode;
					this->Notify.Load(CType);
					return 1;
				}
				
				_proto_string_big Save( void )
				{
					_pugixml_::CTiXml CXml;
					xml_node Per = CXml.InitHeader("1.0", "UTF-8");
					if (!Per)
						return "";
					
					xml_node xNode;
					xNode = CXml.Append("Notify","");
					Notify.Save(CXml, xNode);
					return CXml.GetXmlString(false);
				}
			};
			
			struct DeviceInfo
			{
				struct CQuery
				{
					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;
					
					CQuery( void )
					{}
					
					CQuery( const CQuery& t1 )
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
					}
					
					CQuery& operator=( const CQuery& t1 )
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						return *this;
					}
					
					int Load( CComandType& CType )
					{
						_pugixml_::CTiXml& CXml = CType.GetHandle();
						
						xml_node xNode;
						xNode = CXml.FindNode("Query","CmdType");
						if( xNode )
						{
							this->CmdType = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Query","SN");
						if( xNode )
						{
							this->SN = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Query","DeviceID");
						if( xNode )
						{
							this->DeviceID = CXml.GetNodeText(xNode);
						}
						
						return 1;
					}
					
					void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
					{
						xml_node xNode;
						xNode = CXml.InsertNode(Parent,"CmdType",this->CmdType);
						xNode = CXml.InsertNode(Parent,"SN",this->SN);
						xNode = CXml.InsertNode(Parent,"DeviceID",this->DeviceID);
					}
				};
				
				CQuery Query;
				
				DeviceInfo( bool isPrimary )
				{}
				
				DeviceInfo( void )
				{
					this->Query.CmdType = "DeviceInfo";
				}
				
				
				DeviceInfo( const DeviceInfo& t1 )
				{
					this->Query = t1.Query;
				}
				
				DeviceInfo& operator=( const DeviceInfo& t1 )
				{
					this->Query = t1.Query;
					return *this;
				}
				
				int Load( CComandType& CType )
				{
					_pugixml_::CTiXml& CXml = CType.GetHandle();
					
					xml_node xNode;
					this->Query.Load(CType);
					return 1;
				}
				
				_proto_string_big Save( void )
				{
					_pugixml_::CTiXml CXml;
					xml_node Per = CXml.InitHeader("1.0", "UTF-8");
					if (!Per)
						return "";
					
					xml_node xNode;
					xNode = CXml.Append("Query","");
					Query.Save(CXml, xNode);
					return CXml.GetXmlString(false);
				}
			};
			
			struct DeviceStatus
			{
				struct CQuery
				{
					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;

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

				DeviceStatus(bool isPrimary)
				{}

				DeviceStatus(void)
				{
					this->Query.CmdType = "DeviceStatus";
				}


				DeviceStatus(const DeviceStatus& t1)
				{
					this->Query = t1.Query;
				}

				DeviceStatus& operator=(const DeviceStatus& t1)
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

				_proto_string_big Save(void)
				{
					_pugixml_::CTiXml CXml;
					xml_node Per = CXml.InitHeader("1.0", "UTF-8");
					if (!Per)
						return "";

					xml_node xNode;
					xNode = CXml.Append("Query", "");
					Query.Save(CXml, xNode);
					return CXml.GetXmlString(false);
				}
			};

			struct Catalog
			{
				struct CQuery
				{
					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;

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

				_proto_string_big Save(void)
				{
					_pugixml_::CTiXml CXml;
					xml_node Per = CXml.InitHeader("1.0", "UTF-8");
					if (!Per)
						return "";

					xml_node xNode;
					xNode = CXml.Append("Query", "");
					Query.Save(CXml, xNode);
					return CXml.GetXmlString(false);
				}
			};

		} //namespace _gb28181_req_
		
	} //namespace _xml_
	
} //namespace _apps_proto_

#endif//__GB28181_REQ_20210310152316_1615360996_H__

