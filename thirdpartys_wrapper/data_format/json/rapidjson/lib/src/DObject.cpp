
#include <libThirdWrap_RapidJson/DObject.h>
#include <libThirdWrap_RapidJson/DArray.h>

namespace rapid {

	DObject::DObject( void )
		: _Node(rapidjson::kObjectType)
	{

	}

	DObject::~DObject(void)
	{

	}

	void DObject::Append(DObject::DataMap& Map)
	{
		if (Map.size() <= 0)
			return;

		for (DataMap::iterator iter = Map.begin(); iter != Map.end(); iter++)
		{
			//_Node.AddMember(rapidjson::StringRef(iter->first.c_str()), rapidjson::StringRef(iter->second.c_str()), AllocatorInstance());
			_Node.AddMember(DNode().SetString(iter->first.c_str(), AllocatorInstance()),
				DNode().SetString(iter->second.c_str(), AllocatorInstance()), AllocatorInstance());
		}
	}

	void DObject::Append(const _string_type& sName, const _string_type& sValue)
	{
		//_Node.AddMember(rapidjson::StringRef(sName.c_str()), rapidjson::StringRef(sValue.c_str()), AllocatorInstance());
		_Node.AddMember(DNode().SetString(sName.c_str(), AllocatorInstance()),
			DNode().SetString(sValue.c_str(), AllocatorInstance()), AllocatorInstance());
	}

	void DObject::Append(const _string_type& sName, const DObject& Object)
	{
		//_Node.AddMember(rapidjson::StringRef(sName.c_str()), const_cast<DNode&>(Object.Get()), AllocatorInstance());
		_Node.AddMember(DNode().SetString(sName.c_str(), AllocatorInstance()), const_cast<DNode&>(Object.Get()), AllocatorInstance());

		/*DNode tmObject;
		tmObject.CopyFrom(Object.Get(), AllocatorInstance());
		_Node.AddMember(rapidjson::StringRef(sName.c_str()), tmObject, AllocatorInstance());*/

		/*_Node.AddMember(DObject::DTNode().SetString(sName.c_str(), sName.size(), AllocatorInstance()),
			tmObject, AllocatorInstance());*/
	}

	void DObject::Append(const _string_type& sName, const DArray& Array)
	{
		_Node.AddMember(DNode().SetString(sName.c_str(), AllocatorInstance()), const_cast<DNode&>(Array.Get()), AllocatorInstance());

		/*DNode tmArray;
		tmArray.CopyFrom(Array.Get(), AllocatorInstance());	
		_Node.AddMember(DNode().SetString(sName.c_str(), AllocatorInstance()), tmArray, AllocatorInstance());*/

		//_Node.AddMember(rapidjson::StringRef(sName.c_str()), tmArray, AllocatorInstance());
		/*_Node.AddMember(DObject::DTNode().SetString(sName.c_str(), sName.size(), AllocatorInstance()),
			tmArray, AllocatorInstance());*/
	}

	const DNode& DObject::Get(void) const
	{
		return _Node;
	}

}