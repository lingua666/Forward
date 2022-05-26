
#ifndef __EXTERNAL_20181220100004_1444199361_H__
#define __EXTERNAL_20181220100004_1444199361_H__

#include <libFoundation/modafx.h> 
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <map>

namespace rapid {

	/*typedef rapidjson::Document Document;
	typedef rapidjson::GenericValue<rapidjson::UTF8<> > Value;*/

	typedef rapidjson::GenericDocument< rapidjson::UTF8<>, rapidjson::CrtAllocator > Document;
	typedef rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::CrtAllocator > Value;
	typedef	Value	DNode;

	inline Document::AllocatorType& AllocatorInstance(void)
	{
		static Document::AllocatorType allocator;
		return allocator;
	}
}

#endif//__EXTERNAL_20181220100004_1444199361_H__
