
#include <libThirdWrap_RapidJson/DArray.h>
#include <libThirdWrap_RapidJson/DObject.h>

namespace rapid {

	DArray::DArray(void)
		: _Node(rapidjson::kArrayType)
	{

	}

	DArray::~DArray(void)
	{

	}

	void DArray::Append(const DObject& Object)
	{
		_Node.PushBack(const_cast<DNode&>(Object.Get()), AllocatorInstance());
	}

	const DNode& DArray::Get(void) const
	{
		return _Node;
	}
}