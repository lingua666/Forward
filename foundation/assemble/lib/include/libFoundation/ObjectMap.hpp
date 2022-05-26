
#ifndef __OBJECTMAP_20160418173310_1438860785_H__
#define __OBJECTMAP_20160418173310_1438860785_H__

#include <libContainer/CMyUnordered_map.hpp>
#include <libLock/modafx.h>

namespace	_foundation_{
	
	template<typename _Kty, typename _Ty>
	class ObjectHaseMap
	{
	public:
		typedef Container_Map_type<_Kty, _Ty> Map_type;

	public:
		ObjectHaseMap(void)
		{}

		~ObjectHaseMap(void)
		{
			Release();
		}

		int Init(void)
		{}

		void Release(void)
		{
			_Lock.Lock();
			_Map.clear();
			_Lock.UnLock();
		}

		int Insert(const _Kty& Key, const _Ty& Value)
		{
			int iRet = -1;
			_Lock.Lock();
			if (_Map.find(Key) == _Map.end())
			{
				_Map.insert(std::make_pair(Key, Value));
				iRet = 1;
			}
			_Lock.UnLock();
			
			return iRet;
		}

		const _Ty& Find(const _Kty& Key)
		{
			_Ty Value;
			_Lock.Lock();
			typename Map_type::iterator iter = _Map.find(Key);
			if (iter != _Map.end())
				Value = iter->second;
			_Lock.UnLock();
			return Value;
		}

		_Ty	Remove(const _Kty& Key)
		{
			_Ty Value = Find(Key);
			_Lock.Lock();
			_Map.erase(Key);
			_Lock.UnLock();
			return Value;
		}

		int size(void)
		{
			return _Map.size();
		}

		Map_type*	Get(void)
		{
			return &_Map;
		}

	private:
		Map_type	_Map;
		CLock		_Lock;
	};


} //_foundation


#endif // __MODAFX_20160418173310_1438860785_H__
