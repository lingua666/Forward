#ifndef __THIRDWRAP_RAPIDJSON_DOBJECT_20170706154027_1499326827_H__
#define __THIRDWRAP_RAPIDJSON_DOBJECT_20170706154027_1499326827_H__

#include "External.h"

namespace rapid {

	class DObject;
	class DArray;

	class DObject
	{
	public:
		typedef std::map<_string_type, _string_type>	DataMap;

	public:
		DObject( void );

		~DObject(void);

		void Append(DataMap& Map);

		void Append(const _string_type& sName, const _string_type& sValue);

		void Append(const _string_type& sName, const DObject& Object);

		void Append(const _string_type& sName, const DArray& Array);

		const DNode& Get(void) const;

	private:
		//! Copy constructor is not permitted.
		DObject(const DObject& rhs) /* = delete */;
		//! Copy assignment operator is not permitted.
		DObject& operator=(const DObject& rhs) /* = delete */;

	private:
		DNode		 _Node;
	};
}

#endif
