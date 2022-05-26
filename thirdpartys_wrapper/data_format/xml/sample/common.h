
#include <libThirdWrap_Xml/XML.h>

class CComandType
{
public:
	CComandType( void )
	{

	}

	int	Parse( const std::string& sData )
	{
		return _CXml.Load(sData.c_str());
	}

	bool operator==( const char* c_szType )
	{
		xml_node xNode = _CXml.FindNode("CommandInfo","CommandName");
		if( strcmp( _CXml.GetNodeText(xNode).c_str(), c_szType ) == 0 )
			return true;
		return false;
	}

	_pugixml_::CTiXml&	GetHandle( void ) 
	{
		return _CXml;
	}

private:

	_pugixml_::CTiXml _CXml;
};