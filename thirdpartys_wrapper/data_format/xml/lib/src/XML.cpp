
#include <libThirdWrap_Xml/XML.h>
#include <sstream>

namespace _pugixml_ {

	class allow_node_predicate
	{
	public:
		allow_node_predicate( const _string_t& name )
			: _name(name)
		{

		}

		bool operator()(pugi::xml_node node) const
		{
			return strcmp(node.name(), _name.c_str()) == 0;
		}

	private:
		const _string_t& _name;
	};

	class allow_parent_node_predicate
	{
	public:
		allow_parent_node_predicate( const _string_t& parent_name,
								const _string_t& node_name )
			: _parent_name(parent_name)
			, _node_name(node_name)
		{

		}

		bool operator()(pugi::xml_node node) const
		{
			return (strcmp(node.parent().name(), _parent_name.c_str()) == 0 &&
					strcmp(node.name(), _node_name.c_str()) == 0);
		}

	private:
		const _string_t& _parent_name;
		const _string_t& _node_name;
	};

	class allow_attribute_predicate
	{
	public:
		allow_attribute_predicate( const _string_t& name )
			: _name(name)
		{

		}

		bool operator()(pugi::xml_attribute attr) const
		{
			return strcmp(attr.name(), _name.c_str()) == 0;
		}

	private:
		const _string_t& _name;
	};

	template<typename Predicate>
	class allow_node_attribute_predicate
	{
	public:
		allow_node_attribute_predicate( const Predicate& pred,
									const _string_t& attr_name )
					: _pred(pred)
					, _attr_name(attr_name)
		{

		}

		bool operator()(pugi::xml_node node) const
		{
			if( _pred(node) )
			{
				allow_attribute_predicate	pred(_attr_name);
				if( !node.find_attribute(pred).empty() )
				{
					return true;
				}
			}

			return false;
		}

	private:
		const Predicate&	_pred;
		const _string_t& _attr_name;
	};

	CTiXml::CTiXml( UInt8 Enconding )
			: _CharacterEnc( Enconding )
	{

	}

	CTiXml::~CTiXml( void )
	{

	}

	pugi::xml_node CTiXml::InitHeader(const char* c_szVersion, const char* c_szEncoding)
	{
		pugi::xml_node pre = _Doc.prepend_child(pugi::node_declaration);
		if (pre)
		{
			if(c_szVersion != NULL)
				pre.append_attribute("version") = c_szVersion;

			if(c_szEncoding != NULL)
				pre.append_attribute("encoding") = c_szEncoding;
		}

		return pre;
	}

	int CTiXml::Load( const char* szContent )
	{
		xml_parse_result result = _Doc.load(szContent);
		if( !result )
		{
			printf("CTiXml::Load Error:%s\r\n", result.description());
			return -1;
		}

		return 1;
	}

	int CTiXml::LoadFile( const char* szFilePath )
	{
		xml_parse_result result = _Doc.load_file(szFilePath);
		if( !result )
		{
			return -1;
		}

		return 1;
	}

	xml_node CTiXml::root( void )
	{
		return _Doc.first_child();
		return _Doc.root();
	}

	_string_t CTiXml::GetName( const xml_node& Node )
	{
		return Node.name();
	}

	bool  CTiXml::SetNodeText( const xml_node& Node, const _string_t& Text )
	{
		switch( _CharacterEnc )
		{
		case EN_CCODE_GBK:
			{
				return Node.text().set(Text.c_str());
			}
			break;
		case EN_CCODE_UTF8:
			{
				_string_t utf_text = Text.gbk_utf8();
				return Node.text().set(utf_text.c_str());
			}
			break;
		}
		
		return "";
	}

	bool  CTiXml::SetNodeAttribute( const xml_node& Node,
								const _string_t& AttributeName,
								const _string_t& Text )
	{
		switch( _CharacterEnc )
		{
		case EN_CCODE_GBK:
			{
				return Node.attribute(AttributeName.c_str()).set_value(Text.c_str());
			}
			break;
		case EN_CCODE_UTF8:
			{
				_string_t utf_text = Text.gbk_utf8();
				return Node.attribute(AttributeName.c_str()).set_value(utf_text.c_str());
			}
			break;
		}

		return "";
	}

	bool  CTiXml::SetNodeAttribute( const xml_attribute& Attri , const _string_t& Text )
	{
		switch( _CharacterEnc )
		{
		case EN_CCODE_GBK:
			{
				return ((xml_attribute&)Attri).set_value(Text.c_str());
			}
			break;
		case EN_CCODE_UTF8:
			{
				_string_t utf_text = Text.gbk_utf8();
				return ((xml_attribute&)Attri).set_value(utf_text.c_str());
			}
			break;
		}

		return "";
	}

	xml_node	CTiXml::FindNode( const _string_t& NodeName )
	{
		allow_node_predicate pred(NodeName);
		return _Doc.find_node(pred);
	}

	xml_node	CTiXml::FindNode( const _string_t& NodeName,
							const xml_node& Node )
	{
		allow_node_predicate pred(NodeName);
		return Node.find_node(pred);
	}

	xml_node  CTiXml::FindChildNode(const _string_t& NodeName,
		const xml_node& Node)
	{
		allow_node_predicate pred(NodeName);
		return Node.find_child(pred);
	}

	xml_node  CTiXml::FindNode( const _string_t& ParentName,
							const _string_t& NodeName )
	{
		allow_parent_node_predicate	pred(ParentName, NodeName);
		return _Doc.find_node(pred);
	}

	xml_node  CTiXml::FindNode( const _string_t& ParentName,
							const _string_t& NodeName,
							const xml_node& Node )
	{
		allow_parent_node_predicate	pred(ParentName, NodeName);
		return Node.find_node(pred);
	}

	xml_node  CTiXml::FindNodeToAttribute( const _string_t& NodeName,
								const _string_t& AttributeName )
	{
		allow_node_predicate	p(NodeName);
		allow_node_attribute_predicate<allow_node_predicate>	pred(p, AttributeName);
		return _Doc.find_node(pred);
	}

	xml_node  CTiXml::FindNodeToAttribute( const _string_t& NodeName,
							const _string_t& AttributeName,
							const xml_node& Node )
	{
		allow_node_predicate	p(NodeName);
		allow_node_attribute_predicate<allow_node_predicate>	pred(p, AttributeName);
		return Node.find_node(pred);
	}

	xml_node  CTiXml::FindNodeToAttribute( const _string_t &ParentName,
							const _string_t& NodeName,
							const _string_t& AttributeName )
	{
		allow_parent_node_predicate	p(ParentName, NodeName);
		allow_node_attribute_predicate<allow_parent_node_predicate>	pred(p, AttributeName);
		return _Doc.find_node(pred);
	}

	xml_node  CTiXml::FindNodeToAttribute( const _string_t& ParentName,
								const _string_t& NodeName,
								const _string_t& AttributeName,
								const xml_node Node )
	{
		allow_parent_node_predicate	p(ParentName, NodeName);
		allow_node_attribute_predicate<allow_parent_node_predicate>	pred(p, AttributeName);
		return Node.find_node(pred);
	}

	//查找下一个节点
	xml_node  CTiXml::NextNode( const xml_node& Node )
	{
		return Node.next_sibling();
	}

	xml_node  CTiXml::GetChildNode( const xml_node& Node )
	{
		return Node.first_child();
	}

	xml_node  CTiXml::GetParentNode( const xml_node& Node )
	{
		return Node.parent();
	}

	//查找下一个字节点
	xml_node  CTiXml::FindNextNode( const xml_node& Node, const _string_t& NodeName )
	{
		return Node.next_sibling(NodeName.c_str());
	}

	xml_node  CTiXml::Append( const _string_t& NodeName,
			const _string_t& NodeText )
	{
		xml_node newNode = _Doc.root().append_child(NodeName.c_str());
		newNode.append_child(pugi::node_pcdata).set_value(NodeText.c_str());
		return newNode;
	}

	//插入节点
	xml_node  CTiXml::InsertNode_Prev( const _string_t& Parent,
		const _string_t& NodeName,
		const _string_t& NodeText )
	{
		xml_node Node = FindNode(NodeName);
		if( !Node.empty() )
		{
			xml_node newNode = Node.prepend_child(NodeName.c_str());
			SetNodeText(newNode.prepend_child(pugi::node_pcdata), NodeText);

			//newNode.append_child(pugi::node_pcdata).set_value(NodeText.c_str());
			return newNode;
		}
		return xml_node();
	}

	xml_node  CTiXml::InsertNode( const _string_t& Parent,
								const _string_t& NodeName,
								const _string_t& NodeText )
	{
		xml_node Node = FindNode(NodeName);
		if( !Node.empty() )
		{
			xml_node newNode = Node.append_child(NodeName.c_str());
			SetNodeText(newNode.append_child(pugi::node_pcdata), NodeText);

			//newNode.append_child(pugi::node_pcdata).set_value(NodeText.c_str());
			return newNode;
		}
		return xml_node();
	}

	xml_node  CTiXml::InsertNode_Prev( const xml_node& Parent,
		const _string_t& NodeName,
		const _string_t& NodeText )
	{
		xml_node newNode = ((xml_node&)Parent).prepend_child(NodeName.c_str());
		SetNodeText(newNode.prepend_child(pugi::node_pcdata), NodeText);

		//newNode.append_child(pugi::node_pcdata).set_value(NodeText.c_str());
		return newNode;
	}

	xml_node  CTiXml::InsertNode( const xml_node& Parent,
							const _string_t& NodeName,
							const _string_t& NodeText )
	{
		xml_node newNode = ((xml_node&)Parent).append_child(NodeName.c_str());
		SetNodeText(newNode.append_child(pugi::node_pcdata), NodeText);

		//newNode.append_child(pugi::node_pcdata).set_value(NodeText.c_str());
		return newNode;
	}
		
	//插入节点属性
	xml_node  CTiXml::InsertNodeAttribute( const _string_t& NodeName,
											const _string_t& AttributeName,
											const _string_t& Attributevalue )
	{		
		xml_node Node = FindNode(NodeName);
		if( !Node.empty() )
		{
			SetNodeAttribute(Node.append_attribute(AttributeName.c_str()), Attributevalue);

			//Node.append_attribute(AttributeName.c_str()).set_value(Attributevalue.c_str());
			return Node;
		}
		return xml_node();
	}

	xml_node  CTiXml::InsertNodeAttribute( const xml_node& Node,
										const _string_t& AttributeName,
										const _string_t& Attributevalue )
	{
		SetNodeAttribute(((xml_node&)Node).append_attribute(AttributeName.c_str()), Attributevalue);

		//Node.append_attribute(AttributeName.c_str()).set_value(Attributevalue.c_str());
		return Node;
	}
		
	//删除节点
	bool  CTiXml::RemoveNode( const _string_t &parentnode,
							const _string_t &NodeName )
	{
		xml_node Node = FindNode(parentnode);
		if( Node.empty() )
		{
			return false;
		}
		return Node.remove_child(NodeName.c_str());
	}

	bool  CTiXml::RemoveNode( const xml_node& Node )
	{
		return Node.parent().remove_child(Node);
	}

	//删除节点属性
	bool  CTiXml::RemoveNodeAttribute( const _string_t &NodeName,
									const _string_t &AttributeName )
	{
		xml_node Node = FindNode(NodeName.c_str());
		if( Node.empty() )
			return false;

		return Node.remove_attribute(AttributeName.c_str());
	}


	bool  CTiXml::RemoveNodeAttribute( const xml_node& Node,
									const _string_t &AttributeName )
	{
		return ((xml_node&)Node).remove_attribute(AttributeName.c_str());
	}

	bool CTiXml::GetNodeAttributeValue( const xml_node& Node,
										const _string_t& AttributeName,
										_string_t& AttributeValue )
	{
		xml_attribute attr = Node.attribute(AttributeName.c_str());
		if( attr.empty() )
			return false;

		const char_t* v = attr.value();
		if( v == NULL )
			return false;

		switch( _CharacterEnc )
		{
		case EN_CCODE_GBK:
			{
				AttributeValue = v;
			}
			break;
		case EN_CCODE_UTF8:
			{
				AttributeValue = _string_t(v).utf8_gbk();
			}
			break;
		}
		
		return true;
	}

	_string_t CTiXml::GetNodeText( const xml_node& Node )
	{
		const char* p = Node.text().get();
		if( p == NULL )
			return "";

		switch( _CharacterEnc )
		{
		case EN_CCODE_GBK:
			{
				return p;
			}
			break;
		case EN_CCODE_UTF8:
			{
				return _string_t(p).utf8_gbk();
			}
			break;
		}

		return "";
	}

	xml_node CTiXml::WriteNodeAttributeValue( const _string_t& NodeName,
										const _string_t& AttributeName,
										const _string_t& AttributeValue )
	{
		xml_node Node = FindNodeToAttribute( NodeName, AttributeName.c_str() );
		if( !Node.empty() )
		{
			SetNodeAttribute(Node, AttributeName, AttributeValue);

			//Node.attribute(AttributeName.c_str()).set_value(AttributeValue.c_str());
			return Node;
		}

		return xml_node();
	}

	bool CTiXml::WriteNodeAttributeValue( const xml_node& Node,
										const _string_t& AttributeName,
										const _string_t& AttributeValue )
	{
		return SetNodeAttribute(Node, AttributeName, AttributeValue);
		//return Node.attribute(AttributeName.c_str()).set_value(AttributeValue.c_str());
	}

	xml_node CTiXml::WriteNodeText( const _string_t& NodeName,
								const _string_t& Text )
	{
		xml_node Node = FindNode( NodeName );
		if( !Node.empty() )
		{
			SetNodeText(Node, Text);

			//Node.set_value(Text.c_str());
			return Node;
		}

		return xml_node();
	}

	bool CTiXml::WriteNodeText( const xml_node& Node,
							const _string_t& Text )
	{
		return SetNodeText(Node, Text);

		//return Node.set_value(Text.c_str());
	}

	//保存数据
	bool CTiXml::XmlSaveFile( const _string_t& Path )
	{
		return _Doc.save_file(Path.c_str());
	}

	_string_big_t CTiXml::GetXmlString( bool isHeader )
	{
		std::ostringstream out;
		std::ostream& stream = out;

		if (isHeader)
		{//增加头部
			pugi::xml_node decl = _Doc.prepend_child(pugi::node_declaration);
			decl.append_attribute("version") = "1.0";

			switch (_CharacterEnc)
			{
			case EN_CCODE_GBK:
			{
				decl.append_attribute("encoding") = "GBK";
			}
			break;
			case EN_CCODE_UTF8:
			{
				decl.append_attribute("encoding") = "UTF-8";
			}
			break;
			}
		}

		//decl.append_attribute("standalone") = "no";
		
		//_Doc.save(stream);//带格式输出

		_Doc.save(stream,"",format_raw);
		return _string_big_t(out.str().c_str(), out.str().length());
	}

} // namespace TiXml