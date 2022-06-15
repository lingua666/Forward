
#ifndef __GB28181_RSP_20210310152316_1615360996_H__
#define __GB28181_RSP_20210310152316_1615360996_H__

#include "Apps_Protocol_XML.hpp"
#include <libTimestamp/Timestamp.h>

namespace _apps_proto_ {

	namespace _xml_ {
	
		namespace _gb28181_rsp_ {
		
			struct DeviceInfo
			{
				struct CResponse
				{
					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;
					_proto_string DeviceName;
					_proto_string Result;
					_proto_string DeviceType;
					_proto_string Manufacturer;
					_proto_string Model;
					_proto_string Firmware;
					_proto_string MaxCamera;
					_proto_string MaxAlarm;
					_proto_string Channel;
					_proto_string Port;
					_proto_string LoginName;
					_proto_string Password;
					
					CResponse( void )
					{}
					
					CResponse( const CResponse& t1 )
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->DeviceName = t1.DeviceName;
						this->Result = t1.Result;
						this->DeviceType = t1.DeviceType;
						this->Manufacturer = t1.Manufacturer;
						this->Model = t1.Model;
						this->Firmware = t1.Firmware;
						this->MaxCamera = t1.MaxCamera;
						this->MaxAlarm = t1.MaxAlarm;
						this->Channel = t1.Channel;
						this->Port = t1.Port;
						this->LoginName = t1.LoginName;
						this->Password = t1.Password;
					}
					
					CResponse& operator=( const CResponse& t1 )
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->DeviceName = t1.DeviceName;
						this->Result = t1.Result;
						this->DeviceType = t1.DeviceType;
						this->Manufacturer = t1.Manufacturer;
						this->Model = t1.Model;
						this->Firmware = t1.Firmware;
						this->MaxCamera = t1.MaxCamera;
						this->MaxAlarm = t1.MaxAlarm;
						this->Channel = t1.Channel;
						this->Port = t1.Port;
						this->LoginName = t1.LoginName;
						this->Password = t1.Password;
						return *this;
					}
					
					int Load( CComandType& CType )
					{
						_pugixml_::CTiXml& CXml = CType.GetHandle();
						
						xml_node xNode;
						xNode = CXml.FindNode("Response","CmdType");
						if( xNode )
						{
							this->CmdType = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","SN");
						if( xNode )
						{
							this->SN = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","DeviceID");
						if( xNode )
						{
							this->DeviceID = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","DeviceName");
						if( xNode )
						{
							this->DeviceName = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Result");
						if( xNode )
						{
							this->Result = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","DeviceType");
						if( xNode )
						{
							this->DeviceType = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Manufacturer");
						if( xNode )
						{
							this->Manufacturer = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Model");
						if( xNode )
						{
							this->Model = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Firmware");
						if( xNode )
						{
							this->Firmware = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","MaxCamera");
						if( xNode )
						{
							this->MaxCamera = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","MaxAlarm");
						if( xNode )
						{
							this->MaxAlarm = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Channel");
						if( xNode )
						{
							this->Channel = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Port");
						if( xNode )
						{
							this->Port = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","LoginName");
						if( xNode )
						{
							this->LoginName = CXml.GetNodeText(xNode);
						}
						
						xNode = CXml.FindNode("Response","Password");
						if( xNode )
						{
							this->Password = CXml.GetNodeText(xNode);
						}
						
						return 1;
					}
					
					void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
					{
						xml_node xNode;
						xNode = CXml.InsertNode(Parent,"CmdType",this->CmdType);
						xNode = CXml.InsertNode(Parent,"SN",this->SN);
						xNode = CXml.InsertNode(Parent,"DeviceID",this->DeviceID);
						xNode = CXml.InsertNode(Parent,"DeviceName",this->DeviceName);
						xNode = CXml.InsertNode(Parent,"Result",this->Result);
						xNode = CXml.InsertNode(Parent,"DeviceType",this->DeviceType);
						xNode = CXml.InsertNode(Parent,"Manufacturer",this->Manufacturer);
						xNode = CXml.InsertNode(Parent,"Model",this->Model);
						xNode = CXml.InsertNode(Parent,"Firmware",this->Firmware);
						xNode = CXml.InsertNode(Parent,"MaxCamera",this->MaxCamera);
						xNode = CXml.InsertNode(Parent,"MaxAlarm",this->MaxAlarm);
						xNode = CXml.InsertNode(Parent,"Channel",this->Channel);
						xNode = CXml.InsertNode(Parent,"Port",this->Port);
						xNode = CXml.InsertNode(Parent,"LoginName",this->LoginName);
						xNode = CXml.InsertNode(Parent,"Password",this->Password);
					}
				};
				
				CResponse Response;
				
				DeviceInfo( bool isPrimary )
				{}
				
				DeviceInfo( void )
				{
					this->Response.CmdType = "DeviceInfo";
				}
				
				
				DeviceInfo( const DeviceInfo& t1 )
				{
					this->Response = t1.Response;
				}
				
				DeviceInfo& operator=( const DeviceInfo& t1 )
				{
					this->Response = t1.Response;
					return *this;
				}
				
				int Load( CComandType& CType )
				{
					_pugixml_::CTiXml& CXml = CType.GetHandle();
					
					xml_node xNode;
					this->Response.Load(CType);
					return 1;
				}
				
				_proto_string_big Save( void )
				{
					_pugixml_::CTiXml CXml;
					xml_node Per = CXml.InitHeader("1.0", "UTF-8");
					if (!Per)
						return "";
					
					xml_node xNode;
					xNode = CXml.Append("Response","");
					Response.Save(CXml, xNode);
					return CXml.GetXmlString(false);
				}
			};

			struct DeviceStatus
			{
				struct CResponse
				{
					_proto_string CmdType;
					_proto_string SN;
					_proto_string DeviceID;
					_proto_string Result;
					_proto_string Online;
					_proto_string Status;
					_proto_string DeviceTime;
					_proto_string Encode;
					_proto_string Record;

					CResponse(void)
					{}

					CResponse(const CResponse& t1)
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->Result = t1.Result;
						this->Online = t1.Online;
						this->Status = t1.Status;
						this->DeviceTime = t1.DeviceTime;
						this->Encode = t1.Encode;
						this->Record = t1.Record;
					}

					CResponse& operator=(const CResponse& t1)
					{
						this->CmdType = t1.CmdType;
						this->SN = t1.SN;
						this->DeviceID = t1.DeviceID;
						this->Result = t1.Result;
						this->Online = t1.Online;
						this->Status = t1.Status;
						this->DeviceTime = t1.DeviceTime;
						this->Encode = t1.Encode;
						this->Record = t1.Record;
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

						xNode = CXml.FindNode("Response", "Result");
						if (xNode)
						{
							this->Result = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "Online");
						if (xNode)
						{
							this->Online = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "Status");
						if (xNode)
						{
							this->Status = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "DeviceTime");
						if (xNode)
						{
							this->DeviceTime = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "Encode");
						if (xNode)
						{
							this->Encode = CXml.GetNodeText(xNode);
						}

						xNode = CXml.FindNode("Response", "Record");
						if (xNode)
						{
							this->Record = CXml.GetNodeText(xNode);
						}

						return 1;
					}

					void Save(_pugixml_::CTiXml& CXml, xml_node& Parent)
					{
						xml_node xNode;
						xNode = CXml.InsertNode(Parent, "CmdType", this->CmdType);
						xNode = CXml.InsertNode(Parent, "SN", this->SN);
						xNode = CXml.InsertNode(Parent, "DeviceID", this->DeviceID);
						xNode = CXml.InsertNode(Parent, "Result", this->Result);
						xNode = CXml.InsertNode(Parent, "Online", this->Online);
						xNode = CXml.InsertNode(Parent, "Status", this->Status);
						xNode = CXml.InsertNode(Parent, "DeviceTime", this->DeviceTime);
						xNode = CXml.InsertNode(Parent, "Encode", this->Encode);
						xNode = CXml.InsertNode(Parent, "Record", this->Record);
					}
				};

				CResponse Response;

				DeviceStatus(bool isPrimary)
				{}

				DeviceStatus(void)
				{
					this->Response.CmdType = "DeviceStatus";
				}


				DeviceStatus(const DeviceStatus& t1)
				{
					this->Response = t1.Response;
				}

				DeviceStatus& operator=(const DeviceStatus& t1)
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
					return CXml.GetXmlString(false);
				}
			};

			struct Catalog
			{
				struct CResponse
				{
					struct LisDeviceList
					{
						struct CItem
						{
							_proto_string DeviceID;
							_proto_string Name;
							_proto_string Manufacturer;
							_proto_string Model;
							_proto_string Owner;
							_proto_string CivilCode;
							_proto_string Address;
							_proto_string Parental;
							_proto_string ParentID;
							_proto_string SafetyWay;
							_proto_string RegisterWay;
							_proto_string Secrecy;
							_proto_string Status;
							_proto_string Longitude;
							_proto_string Latitude;

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
								this->SafetyWay = t1.SafetyWay;
								this->RegisterWay = t1.RegisterWay;
								this->Secrecy = t1.Secrecy;
								this->Status = t1.Status;
								this->Longitude = t1.Longitude;
								this->Latitude = t1.Latitude;
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
								this->SafetyWay = t1.SafetyWay;
								this->RegisterWay = t1.RegisterWay;
								this->Secrecy = t1.Secrecy;
								this->Status = t1.Status;
								this->Longitude = t1.Longitude;
								this->Latitude = t1.Latitude;
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
									tmNode = CXml.FindNode("SafetyWay", xNode);
									if (tmNode)
									{
										this->SafetyWay = CXml.GetNodeText(tmNode);
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
									tmNode = CXml.FindNode("Status", xNode);
									if (tmNode)
									{
										this->Status = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Longitude", xNode);
									if (tmNode)
									{
										this->Longitude = CXml.GetNodeText(tmNode);
									}
									tmNode = CXml.FindNode("Latitude", xNode);
									if (tmNode)
									{
										this->Latitude = CXml.GetNodeText(tmNode);
									}
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
								xNode = CXml.InsertNode(Parent, "SafetyWay", this->SafetyWay);
								xNode = CXml.InsertNode(Parent, "RegisterWay", this->RegisterWay);
								xNode = CXml.InsertNode(Parent, "Secrecy", this->Secrecy);
								xNode = CXml.InsertNode(Parent, "Status", this->Status);
								xNode = CXml.InsertNode(Parent, "Longitude", this->Longitude);
								xNode = CXml.InsertNode(Parent, "Latitude", this->Latitude);
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
					xml_node Per = CXml.InitHeader("1.0", "");
					if (!Per)
						return "";

					xml_node xNode;
					xNode = CXml.Append("Response", "");
					Response.Save(CXml, xNode);
					return CXml.GetXmlString(false);
				}
			};
			
		} //namespace _gb28181_rsp_
		
	} //namespace _xml_
	
} //namespace _apps_proto_

#endif//__GB28181_RSP_20210310152316_1615360996_H__

