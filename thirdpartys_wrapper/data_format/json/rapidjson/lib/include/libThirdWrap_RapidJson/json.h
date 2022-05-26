#ifndef __THIRDWRAP_RAPIDJSON_20170706154027_1499326827_H__
#define __THIRDWRAP_RAPIDJSON_20170706154027_1499326827_H__

#include "External.h"
#include "DArray.h"
#include "DObject.h"

namespace rapid {

	typedef _foundation_::_string_t				_string_t;
	typedef _foundation_::_string_big_t			_string_big_t;

	class json
	{
	public:
		json(void);

		~json(void);

		int Load(const char* c_szData);

		int  LoadFile(const char* szFilePath);

		DNode& root(void);

		_string_t GetText(const DNode& Node);

		bool  SetText(const _string_t& Name, const _string_t& Text);

		bool  SetText(DNode& Node, const _string_t& Text);

		bool GetInt(const DNode& Node, Int64& Value);

		bool  SetInt(const _string_t& Name, Int64 Value);

		bool  SetInt(DNode& Node, Int64 Value);

		bool GetDouble(const DNode& Node, double& Value);

		bool  SetDouble(const _string_t& Name, double Value);

		bool  SetDouble(DNode& Node, double Value);

		bool  SetBool(const _string_t& Name, bool Value);

		bool  SetBool(DNode& Node, bool Value);

		DNode&	FindNode(const _string_t& NodeName);

		DNode&	FindNode(const _string_t& NodeName, const DNode& Start);

		DNode&  FindNode(const _string_t& ParentName, const _string_t& NodeName);

		DNode&  FindNode(const _string_t& ParentName, const _string_t& NodeName, const DNode& Start);

		DNode&  GetChildNode(const DNode& Node);

		DNode&  InsertNode(const _string_t& ParentName,
			const _string_t& NodeName,
			const _string_t& NodeText);

		DNode&  InsertNode(const DNode& Parent,
			const _string_t& NodeName,
			const _string_t& NodeText);

		DNode&  InsertNode(const _string_t& ParentName,
			const _string_t& NodeName,
			Int64 Value);

		DNode&  InsertNode(const DNode& Parent,
			const _string_t& NodeName,
			Int64 Value);

		DNode&  InsertNode(const _string_t& ParentName,
			const _string_t& NodeName,
			double Value);

		DNode&  InsertNode(const DNode& Parent,
			const _string_t& NodeName,
			double Value);

		DNode&  InsertNode(const _string_t& ParentName,
			const _string_t& NodeName,
			bool Value);

		DNode&  InsertNode(const DNode& Parent,
			const _string_t& NodeName,
			bool Value);

		DNode&  InsertNode(const DNode& Parent,
			const _string_t& NodeName,
			const DNode& Node);

		DNode&  InsertNode(const _string_t& ParentName, const DNode& Node);

		DNode&  InsertNode(const DNode& Parent, const DNode& Node);

		bool  RemoveNode(const _string_t &ParentName, const _string_t &NodeName);

		bool  RemoveNode(DNode& Node);

		DNode&  InsertObject(const _string_t& ParentName, const _string_t& sName, const DObject& Obj);

		DNode&  InsertObject(const DNode& Parent, const _string_t& sName,  const DObject& Obj);

		DNode&  InsertArray(const _string_t& ParentName, const _string_t& sName, const DArray& Array);

		DNode&  InsertArray(const DNode& Parent, const _string_t& sName, const DArray& Array);

		bool SaveFile(const _string_t& Path);

		_string_big_t GetString(void);

	private:
		Document _Doc;
	};
}

#endif
