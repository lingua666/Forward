#ifndef __THIRDWRAP_RAPIDJSON_DARRAY_20170706154027_1499326827_H__
#define __THIRDWRAP_RAPIDJSON_DARRAY_20170706154027_1499326827_H__

#include "External.h"

namespace rapid {

	class DObject;
	class DArray;

	class DArray
	{
	public:
		typedef std::map<_string_type, _string_type>	DataMap;

	public:
		DArray(void);

		~DArray(void);

		void Append(const DObject& Object);

		const DNode& Get(void) const;

	private:
		//! Copy constructor is not permitted.
		DArray(const DArray& rhs) /* = delete */;

		//! Copy assignment operator is not permitted.
		DArray& operator=(const DArray& rhs) /* = delete */;

	private:
		DNode		 _Node;
	};
}

#endif
