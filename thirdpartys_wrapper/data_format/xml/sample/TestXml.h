
#include "common.h"

namespace _proto_ {

	struct LoginIn
	{
		struct CCommandInfo
		{
			struct CModuleUID
			{
				struct CCommandUID
				{
					std::string ClientUID;
					
					int Load( CComandType& CType )
					{
						_pugixml_::CTiXml& CXml = CType.GetHandle();
						
						xml_node xNode;
						xNode = CXml.FindNode("CommandUID","ClientUID");
						if( !xNode )
							return -1;
						
						this->ClientUID = CXml.GetNodeText(xNode);
						
						return 1;
					}
					
					void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
					{
						xml_node xNode;
						xNode = CXml.InsertNode(Parent,"ClientUID",this->ClientUID);
					}
				};

				std::string ServerUID;
				std::string ClientUID;
				CCommandUID CommandUID;
				
				int Load( CComandType& CType )
				{
					_pugixml_::CTiXml& CXml = CType.GetHandle();
					
					xml_node xNode;
					xNode = CXml.FindNode("ModuleUID","ServerUID");
					if( !xNode )
						return -1;
					
					this->ServerUID = CXml.GetNodeText(xNode);
					
					xNode = CXml.FindNode("ModuleUID","ClientUID");
					if( !xNode )
						return -1;
					
					this->ClientUID = CXml.GetNodeText(xNode);
					
					this->CommandUID.Load(CType);
					return 1;
				}
				
				void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
				{
					xml_node xNode;
					xNode = CXml.InsertNode(Parent,"ServerUID",this->ServerUID);
					xNode = CXml.InsertNode(Parent,"ClientUID",this->ClientUID);
					xNode = CXml.InsertNode(Parent,"CommandUID","");
					CommandUID.Save(CXml, xNode);
				}
			};

			std::string CommandUID;
			std::string ServerUID;
			std::string ClientUID;
			std::string CommandName;
			CModuleUID ModuleUID;
			std::string SendTime;
			
			int Load( CComandType& CType )
			{
				_pugixml_::CTiXml& CXml = CType.GetHandle();
				
				xml_node xNode;
				xNode = CXml.FindNode("CommandInfo","CommandUID");
				if( !xNode )
					return -1;
				
				this->CommandUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","ServerUID");
				if( !xNode )
					return -1;
				
				this->ServerUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","ClientUID");
				if( !xNode )
					return -1;
				
				this->ClientUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","CommandName");
				if( !xNode )
					return -1;
				
				this->CommandName = CXml.GetNodeText(xNode);
				
				this->ModuleUID.Load(CType);
				xNode = CXml.FindNode("CommandInfo","SendTime");
				if( !xNode )
					return -1;
				
				this->SendTime = CXml.GetNodeText(xNode);
				
				return 1;
			}
			
			void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent,"CommandUID",this->CommandUID);
				xNode = CXml.InsertNode(Parent,"ServerUID",this->ServerUID);
				xNode = CXml.InsertNode(Parent,"ClientUID",this->ClientUID);
				xNode = CXml.InsertNode(Parent,"CommandName",this->CommandName);
				xNode = CXml.InsertNode(Parent,"ModuleUID","");
				ModuleUID.Save(CXml, xNode);
				xNode = CXml.InsertNode(Parent,"SendTime",this->SendTime);
			}
		};

		struct CContent
		{
			std::string UpdataServerInfoResult;
			
			int Load( CComandType& CType )
			{
				_pugixml_::CTiXml& CXml = CType.GetHandle();
				
				xml_node xNode;
				xNode = CXml.FindNode("Content","UpdataServerInfoResult");
				if( !xNode )
					return -1;
				
				this->UpdataServerInfoResult = CXml.GetNodeText(xNode);
				
				return 1;
			}
			
			void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent,"UpdataServerInfoResult",this->UpdataServerInfoResult);
			}
		};

		CCommandInfo CommandInfo;
		CContent Content;
		
		int Load( CComandType& CType )
		{
			_pugixml_::CTiXml& CXml = CType.GetHandle();
			
			xml_node xNode;
			this->CommandInfo.Load(CType);
			this->Content.Load(CType);
			return 1;
		}
		
		std::string Save( void )
		{
			char c_szData[] = "<?xml version=\"1.0\" ncoding=\"UTF-8\"?>\
					<Protocol Version=\"V1.1\">\
					</Protocol>";
			
			_pugixml_::CTiXml CXml;
			if( CXml.Load(c_szData) == -1 )
				return "";
			
			xml_node xNode;
			xNode = CXml.InsertNode(CXml.root(),"CommandInfo","");
			CommandInfo.Save(CXml, xNode);
			xNode = CXml.InsertNode(CXml.root(),"Content","");
			Content.Save(CXml, xNode);
			return CXml.GetXmlString();
		}
	};
	
	struct UpdataServerInfoResult
	{
		struct CCommandInfo
		{
			std::string CommandUID;
			std::string ServerUID;
			std::string ClientUID;
			std::string ModuleUID;
			std::string CommandName;
			std::string SendTime;
			
			int Load( CComandType& CType )
			{
				_pugixml_::CTiXml& CXml = CType.GetHandle();
				
				xml_node xNode;
				xNode = CXml.FindNode("CommandInfo","CommandUID");
				if( !xNode )
					return -1;
				
				this->CommandUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","ServerUID");
				if( !xNode )
					return -1;
				
				this->ServerUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","ClientUID");
				if( !xNode )
					return -1;
				
				this->ClientUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","ModuleUID");
				if( !xNode )
					return -1;
				
				this->ModuleUID = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","CommandName");
				if( !xNode )
					return -1;
				
				this->CommandName = CXml.GetNodeText(xNode);
				
				xNode = CXml.FindNode("CommandInfo","SendTime");
				if( !xNode )
					return -1;
				
				this->SendTime = CXml.GetNodeText(xNode);
				
				return 1;
			}
			
			void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent,"CommandUID",this->CommandUID);
				xNode = CXml.InsertNode(Parent,"ServerUID",this->ServerUID);
				xNode = CXml.InsertNode(Parent,"ClientUID",this->ClientUID);
				xNode = CXml.InsertNode(Parent,"ModuleUID",this->ModuleUID);
				xNode = CXml.InsertNode(Parent,"CommandName",this->CommandName);
				xNode = CXml.InsertNode(Parent,"SendTime",this->SendTime);
			}
		};

		struct CContent
		{
			std::string UpdataServerInfoResult1;
			
			int Load( CComandType& CType )
			{
				_pugixml_::CTiXml& CXml = CType.GetHandle();
				
				xml_node xNode;
				xNode = CXml.FindNode("Content","UpdataServerInfoResult1");
				if( !xNode )
					return -1;
				
				this->UpdataServerInfoResult1 = CXml.GetNodeText(xNode);
				
				return 1;
			}
			
			void Save( _pugixml_::CTiXml& CXml, xml_node& Parent )
			{
				xml_node xNode;
				xNode = CXml.InsertNode(Parent,"UpdataServerInfoResult1",this->UpdataServerInfoResult1);
			}
		};

		CCommandInfo CommandInfo;
		CContent Content;
		
		int Load( CComandType& CType )
		{
			_pugixml_::CTiXml& CXml = CType.GetHandle();
			
			xml_node xNode;
			this->CommandInfo.Load(CType);
			this->Content.Load(CType);
			return 1;
		}
		
		std::string Save( void )
		{
			char c_szData[] = "<?xml version=\"1.0\" ncoding=\"UTF-8\"?>\
					<Protocol Version=\"V1.1\">\
					</Protocol>";
			
			_pugixml_::CTiXml CXml;
			if( CXml.Load(c_szData) == -1 )
				return "";
			
			xml_node xNode;
			xNode = CXml.InsertNode(CXml.root(),"CommandInfo","");
			CommandInfo.Save(CXml, xNode);
			xNode = CXml.InsertNode(CXml.root(),"Content","");
			Content.Save(CXml, xNode);
			return CXml.GetXmlString();
		}
	};
	
} //namespace _proto_
