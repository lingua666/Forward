#ifndef __XML_20160426173004_1444199361_H__
#define __XML_20160426173004_1444199361_H__

#include "External.h"

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#pragma comment(linker,"/nodefaultlib:libc.lib")
	#pragma warning(disable:4996)
	#include <Windows.h>
#elif defined(PLATFORM_OS_FAMILY_UNIX)

#endif

#include <pugiconfig.hpp>
#include <pugixml.hpp>
#include <algorithm>

using namespace pugi;

namespace	_pugixml_{

	typedef _foundation_::_string_t				_string_t;
	typedef _foundation_::_string_big_t			_string_big_t;

	enum CharacterEnc
	{
		EN_CCODE_GBK,
		EN_CCODE_UTF8
	};

	class CTiXml
	{
	public:
		explicit CTiXml( UInt8 Enconding = EN_CCODE_GBK );

		~CTiXml( void );

		pugi::xml_node InitHeader(const char* c_szVersion, const char* c_szEncodin);

		int Load( const char* szContent );

		int LoadFile( const char* szFilePath );

		xml_node root( void );

		_string_t GetName( const xml_node& Node );

		bool  SetNodeText( const xml_node& Node, const _string_t& Text );
		
		bool  SetNodeAttribute( const xml_node& Node,
							const _string_t& AttributeName,
							const _string_t& Text );

		bool  SetNodeAttribute( const xml_attribute& Attri,
							const _string_t& Text );

		xml_node	FindNode( const _string_t& NodeName );

		xml_node	FindNode( const _string_t& NodeName,
							const xml_node& Node );

		xml_node  FindChildNode(const _string_t& NodeName,
			const xml_node& Node);

		xml_node  FindNode( const _string_t& ParentName,
						const _string_t& NodeName );

		xml_node  FindNode( const _string_t& ParentName,
				const _string_t& NodeName,
				const xml_node& Node );

		xml_node  FindNodeToAttribute( const _string_t& NodeName,
				const _string_t& AttributeName );

		xml_node  FindNodeToAttribute( const _string_t& NodeName,
				const _string_t& AttributeName,
				const xml_node& Node );

		xml_node FindNodeToAttribute( const _string_t &ParentName,
				const _string_t& NodeName,
				const _string_t& AttributeName );

		xml_node  FindNodeToAttribute( const _string_t& ParentName,
				const _string_t& NodeName,
				const _string_t& AttributeName,
				const xml_node Node );

		xml_node  NextNode( const xml_node& Node );

		xml_node  GetChildNode( const xml_node& Node );

		xml_node  GetParentNode( const xml_node& Node );

		xml_node  FindNextNode( const xml_node& Node, const _string_t& NodeName );

		xml_node  Append( const _string_t& NodeName,
				const _string_t& NodeText );

		xml_node  InsertNode_Prev( const _string_t& Parent,
			const _string_t& NodeName,
			const _string_t& NodeText );

		xml_node  InsertNode( const _string_t& Parent,
				const _string_t& NodeName,
				const _string_t& NodeText );

		xml_node  InsertNode_Prev( const xml_node& Parent,
			const _string_t& NodeName,
			const _string_t& NodeText );

		xml_node  InsertNode( const xml_node& Parent,
				const _string_t& NodeName,
				const _string_t& NodeText );

		xml_node  InsertNodeAttribute( const _string_t& NodeName,
				const _string_t& AttributeName,
				const _string_t& Attributevalue );

		xml_node  InsertNodeAttribute( const xml_node& Node,
				const _string_t& AttributeName,
				const _string_t& Attributevalue );

		bool  RemoveNode( const _string_t &parentnode,
				const _string_t &NodeName );

		bool  RemoveNode( const xml_node& Node );

		bool  RemoveNodeAttribute( const _string_t &NodeName,
				const _string_t &AttributeName );

		bool  RemoveNodeAttribute( const xml_node& Node,
				const _string_t &AttributeName );

		bool GetNodeAttributeValue( const xml_node& Node,
				const _string_t& AttributeName,
			_string_t& AttributeValue );

		_string_t GetNodeText( const xml_node& Node );

		xml_node WriteNodeAttributeValue( const _string_t& NodeName,
				const _string_t& AttributeName,
				const _string_t& AttributeValue );

		bool WriteNodeAttributeValue( const xml_node& Node,
				const _string_t& AttributeName,
				const _string_t& AttributeValue );

		xml_node WriteNodeText( const _string_t& NodeName,
				const _string_t& Text );

		bool WriteNodeText( const xml_node& Node,
			const _string_t& Text );

		bool XmlSaveFile( const _string_t& Path );

		_string_big_t GetXmlString( bool isHeader = true );

	private:
		xml_document	_Doc;			//XMLÎÄµµ
		UInt8			_CharacterEnc;
	};

} // namespace _pugixml_

#endif