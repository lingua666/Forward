
#ifndef __APPS_PROTOCOL_XML_20200517181513_1445757319_H__
#define __APPS_PROTOCOL_XML_20200517181513_1445757319_H__

#include <libFoundation/modafx.h>
#include <libThirdWrap_Xml/XML.h>
#include <libProto_Sip_V20/modafx.h>
#include <string>

namespace _apps_proto_ {

	typedef _foundation_::_string_t		_proto_string;
	typedef _foundation_::_string_big_t	_proto_string_big;

	namespace _xml_ {

		class CComandType
		{
		public:
			static _proto_string GenerateUIDIndex(const _proto_string& sUid, int iIndex);

			static _proto_string GetUidToInfo(const _proto_string& Info);

			static _proto_string GetIndexToInfo(const _proto_string& Info);

			template<typename T1, typename T2>
			static void CloneRouteInfo(T1& Dest, const T2& Src)
			{
				Dest.DestLV0Info = Src.DestLV0Info;
				Dest.DestLV1Info = Src.DestLV1Info;
				Dest.DestLV2Info = Src.DestLV2Info;
				Dest.DestLV3Info = Src.DestLV3Info;
				Dest.SourceLV0Info = Src.SourceLV0Info;
				Dest.SourceLV1Info = Src.SourceLV1Info;
				Dest.SourceLV2Info = Src.SourceLV2Info;
				Dest.SourceLV3Info = Src.SourceLV3Info;
			}

			template<typename T1, typename T2>
			static void ReversalRouteInfo(T1& Dest, const T2& Src)
			{
				Dest.DestLV0Info = Src.SourceLV0Info;
				Dest.DestLV1Info = Src.SourceLV1Info;
				Dest.DestLV2Info = Src.SourceLV2Info;
				Dest.DestLV3Info = Src.SourceLV3Info;
				Dest.SourceLV0Info = Src.DestLV0Info;
				Dest.SourceLV1Info = Src.DestLV1Info;
				Dest.SourceLV2Info = Src.DestLV2Info;
				Dest.SourceLV3Info = Src.DestLV3Info;
			}

		public:
			CComandType(void);

			int	Parse(const _proto_string& sData);

			bool Equal(const _proto_string& sType);

			bool Change(const _proto_string& sName, const _proto_string& Content);

			_proto_string GetCommandType(void);

			_proto_string GetSourceUID(void);

			_proto_string GetCommandName(void);

			_proto_string GetCommandUID(void);

			_proto_string GetResult(void);

			_pugixml_::CTiXml&	GetHandle(void);

			_proto_string_big GetString(void);

		private:
			_pugixml_::CTiXml _CXml;
		};

	}

}

#endif//__APPS_PROTOCOL_20151025151519_1445757319_H__
