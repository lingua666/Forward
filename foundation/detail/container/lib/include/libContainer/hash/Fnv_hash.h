
#ifndef __FNV_HASH_20150918120440_1442549080_H__
#define __FNV_HASH_20150918120440_1442549080_H__

namespace	_hash_{

	template<std::size_t = sizeof(std::size_t)>
	struct Fnv_hash
	{
		static std::size_t hash(const char* first, std::size_t length)
		{
			std::size_t result = 0;
			for (; length > 0; --length)
				result = (result * 131) + *first++;
			return result;
		}
	};

	template<>
	struct Fnv_hash<4>
	{
		static std::size_t hash(const char* first, std::size_t length)
		{
			std::size_t result = static_cast<std::size_t>(2166136261UL);
			for (; length > 0; --length)
			{
				result ^= (std::size_t)*first++;
				result *= 16777619UL;
			}
			return result;
		}
	};

	template<>
	struct Fnv_hash<8>
	{
		static std::size_t hash(const char* first, std::size_t length)
		{
			std::size_t result = static_cast<std::size_t>(14695981039346656037ULL);
			for (; length > 0; --length)
			{
				result ^= (std::size_t)*first++;
				result *= 1099511628211ULL;
			}
			return result;
		}
	};
}	//namespace	_hash_

#endif//__FNV_HASH_20150918120440_1442549080_H__
