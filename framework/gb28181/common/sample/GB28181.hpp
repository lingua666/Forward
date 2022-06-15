
#ifndef __GB28181_20210309161822_1615277902_H__
#define __GB28181_20210309161822_1615277902_H__

#include "Apps_Protocol_XML.hpp"
#include <libTimestamp/Timestamp.h>

namespace _apps_proto_ {

	namespace _xml_ {

		namespace _GB28181_ {

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
					return CXml.GetXmlString();
				}
			};

			/*struct Catalog
			{
				struct CResponse
				{
					struct LisDeviceList
					{
						struct CItem
						{
							struct CInfo
							{
								_proto_string PTZType;
								_proto_string DownloadSpeed;

								CInfo(const CInfo& t1)
								{
									this->PTZType = t1.PTZType;
									this->DownloadSpeed = t1.DownloadSpeed;
								}

								CInfo& operator=(const CInfo& t1)
								{
									this->PTZType = t1.PTZType;
									this->DownloadSpeed = t1.DownloadSpeed;
									return *this;
								}

								CInfo(void)
								{

								}

								int Load(CComandType& CType, xml_node xNode)
								{
									_pugixml_::CTiXml& CXml = CType.GetHandle();
									xml_node tmNode;

									if (xNode)
									{
										tmNode = CXml.FindNode("PTZType", xNode);
										if (tmNode)
										{
											this->PTZType = CXml.GetNodeText(tmNode);
										}
										tmNode = CXml.FindNode("DownloadSpeed", xNode);
										if (tmNode)
										{
											this->DownloadSpeed = CXml.GetNodeText(tmNode);
										}
									}

									return 1;
								}

								void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
								{
									xml_node xNode;
									xNode = CXml.InsertNode(Parent, "PTZType", this->PTZType);
									xNode = CXml.InsertNode(Parent, "DownloadSpeed", this->DownloadSpeed);
								}
							};

							_proto_string DeviceID;
							_proto_string Name;
							_proto_string Manufacturer;
							_proto_string Model;
							_proto_string Owner;
							_proto_string CivilCode;
							_proto_string Address;
							_proto_string Parental;
							_proto_string ParentID;
							_proto_string RegisterWay;
							_proto_string Secrecy;
							_proto_string StreamNum;
							_proto_string Status;
							CInfo Info;

							CItem(const CItem& t1)
							{
								this->DeviceID = t1.DeviceID;
								this->Name = t1.Name;
								this->Manufacturer = t1.Manufacturer;
								this->Model = t1.Model;
								this->Owner = t1.Owner;
								this->CivilCode = t1.CivilCode;
								this->Address = t1.Address;
								this->Parental = t1.Parental;
								this->ParentID = t1.ParentID;
								this->RegisterWay = t1.RegisterWay;
								this->Secrecy = t1.Secrecy;
								this->StreamNum = t1.StreamNum;
								this->Status = t1.Status;
								this->Info = t1.Info;
							}

							CItem& operator=(const CItem& t1)
							{
								this->DeviceID = t1.DeviceID;
								this->Name = t1.Name;
								this->Manufacturer = t1.Manufacturer;
								this->Model = t1.Model;
								this->Owner = t1.Owner;
								this->CivilCode = t1.CivilCode;
								this->Address = t1.Address;
								this->Parental = t1.Parental;
								this->ParentID = t1.ParentID;
								this->RegisterWay = t1.RegisterWay;
								this->Secrecy = t1.Secrecy;
								this->StreamNum = t1.StreamNum;
								this->Status = t1.Status;
								this->Info = t1.Info;
								return *this;
							}

							CItem(void)
							{

							}

							int Load(CComandType& CType, xml_node xNode)
							{
								_pugixml_::CTiXml& CXml = CType.GetHandle();
								xml_node tmNode;

								if (xNode)
								{
									tmNode = CXml.FindNode("DeviceID", xNode);
									if (tmNode)
									{
										this->DeviceID = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Name", xNode);
									if (tmNode)
									{
										this->Name = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Manufacturer", xNode);
									if (tmNode)
									{
										this->Manufacturer = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Model", xNode);
									if (tmNode)
									{
										this->Model = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Owner", xNode);
									if (tmNode)
									{
										this->Owner = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("CivilCode", xNode);
									if (tmNode)
									{
										this->CivilCode = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Address", xNode);
									if (tmNode)
									{
										this->Address = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Parental", xNode);
									if (tmNode)
									{
										this->Parental = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("ParentID", xNode);
									if (tmNode)
									{
										this->ParentID = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("RegisterWay", xNode);
									if (tmNode)
									{
										this->RegisterWay = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Secrecy", xNode);
									if (tmNode)
									{
										this->Secrecy = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("StreamNum", xNode);
									if (tmNode)
									{
										this->StreamNum = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Status", xNode);
									if (tmNode)
									{
										this->Status = CXml.GetNodeText(tmNode);
									}
									this->Info.Load(CType, xNode);
								}

								return 1;
							}

							void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
							{
								xml_node xNode;
								xNode = CXml.InsertNode(Parent, "DeviceID", this->DeviceID);
								xNode = CXml.InsertNode(Parent, "Name", this->Name);
								xNode = CXml.InsertNode(Parent, "Manufacturer", this->Manufacturer);
								xNode = CXml.InsertNode(Parent, "Model", this->Model);
								xNode = CXml.InsertNode(Parent, "Owner", this->Owner);
								xNode = CXml.InsertNode(Parent, "CivilCode", this->CivilCode);
								xNode = CXml.InsertNode(Parent, "Address", this->Address);
								xNode = CXml.InsertNode(Parent, "Parental", this->Parental);
								xNode = CXml.InsertNode(Parent, "ParentID", this->ParentID);
								xNode = CXml.InsertNode(Parent, "RegisterWay", this->RegisterWay);
								xNode = CXml.InsertNode(Parent, "Secrecy", this->Secrecy);
								xNode = CXml.InsertNode(Parent, "StreamNum", this->StreamNum);
								xNode = CXml.InsertNode(Parent, "Status", this->Status);
								xNode = CXml.InsertNode(Parent, "Info", "");
								Info.Save(CXml, xNode);
							}
						};

						typedef _container_::LinkList<CItem, CNoneLock> LList_type;
						typedef SmartPTR<LList_type> LList_type_sptr;
						LList_type_sptr DeviceList;

						LisDeviceList(const LisDeviceList& t1)
						{
							this->DeviceList = t1.DeviceList;
						}

						LisDeviceList& operator=(const LisDeviceList& t1)
						{
							this->DeviceList = t1.DeviceList;
							return *this;
						}

						LisDeviceList(void)
						{
							DeviceList = LList_type_sptr(new LList_type());
						}

						int push_back(const CItem& Item)
						{
							DeviceList->push_back(Item);
							return 1;
						}

						int Load(CComandType& CType)
						{
							_pugixml_::CTiXml& CXml = CType.GetHandle();

							xml_node xNode;
							xNode = CXml.FindNode("DeviceList");
							if (xNode)
							{
								xNode = CXml.GetChildNode(xNode);
								if (xNode)
								{
									do
									{
										CItem Item;
										Item.Load(CType, xNode);
										DeviceList->push_back(Item);
										xNode = CXml.NextNode(xNode);
									} while (xNode);
								}
							}

							return 1;
						}

						void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
						{
							xml_node xNode;
							char szName[100] = "Item";
							CItem Item;
							LList_type::iterator iter;
							for (iter = DeviceList->begin(); iter != DeviceList->end(); iter++)
							{
								Item = *iter;
								xNode = CXml.InsertNode(Parent, szName, "");
								Item.Save(CXml, xNode);
							};
						}
					};

					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;
					_proto_string SumNum;
					LisDeviceList DeviceList;

					CResponse(void)
					{}

					CResponse(const CResponse& t1)
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->SumNum = t1.SumNum;
						this->DeviceList = t1.DeviceList;
					}

					CResponse& operator=(const CResponse& t1)
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->SumNum = t1.SumNum;
						this->DeviceList = t1.DeviceList;
						return *this;
					}

					int Load(CComandType& CType)
					{
						_pugixml_::CTiXml& CXml = CType.GetHandle();

						xml_node xNode;
						xNode = CXml.FindNode("Response", "CmdType");
						if (xNode)
						{
							this->CmdType = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "SN");
						if (xNode)
						{
							this->SN = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "DeviceID");
						if (xNode)
						{
							this->DeviceID = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "SumNum");
						if (xNode)
						{
							this->SumNum = CXml.GetNodeText(xNode);
						}

						this->DeviceList.Load(CType);
						return 1;
					}

					void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
					{
						xml_node xNode;
						xNode = CXml.InsertNode(Parent, "CmdType", this->CmdType);
						xNode = CXml.InsertNode(Parent, "SN", this->SN);
						xNode = CXml.InsertNode(Parent, "DeviceID", this->DeviceID);
						xNode = CXml.InsertNode(Parent, "SumNum", this->SumNum);
						xNode = CXml.InsertNode(Parent, "DeviceList", "");
						DeviceList.Save(CXml, xNode);
					}
				};

				CResponse Response;

				Catalog(bool isPrimary)
				{}

				Catalog(void)
				{
					this->Response.CmdType = "Catalog";
				}


				Catalog(const Catalog& t1)
				{
					this->Response = t1.Response;
				}

				Catalog& operator=(const Catalog& t1)
				{
					this->Response = t1.Response;
					return *this;
				}

				int Load(CComandType& CType)
				{
					_pugixml_::CTiXml& CXml = CType.GetHandle();

					xml_node xNode;
					this->Response.Load(CType);
					return 1;
				}

				_proto_string_big Save(void)
				{
					_pugixml_::CTiXml CXml;
					xml_node Per = CXml.InitHeader("1.0", "UTF-8");
					if (!Per)
						return "";

					xml_node xNode;
					xNode = CXml.Append("Response", "");
					Response.Save(CXml, xNode);
					return CXml.GetXmlString();
				}
			};*/

		} //namespace _GB28181_

	} //namespace _xml_

} //namespace _apps_proto_

#endif//__GB28181_20210309161822_1615277902_H__

