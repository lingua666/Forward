
#ifndef __SINGLETON_20151127152350_1448609030_H__
#define __SINGLETON_20151127152350_1448609030_H__

#include <libLock/Lock.hpp>

template <typename T>
class Singleton
{    
public:    
	//获取类的唯一实例    
	static inline T* instance( void )
	{
		if(_instance ==  NULL)
		{
			_Lock.Lock();
			if(_instance == NULL)
			{
				_instance = (void*)new T(); 
			}
			_Lock.UnLock();
		}
		return (T*)_instance;
	}

	//获取类的唯一实例
	template<typename P1>
	static inline T* instance( const P1& p1 )
	{
		if(_instance ==  NULL)
		{
			_Lock.Lock();
			if(_instance == NULL)
			{
				_instance = (void*)new T(p1);
			}
			_Lock.UnLock();
		}
		return (T*)_instance;
	}

	//释放类的唯一实例
	static inline void release( void )
	{
		if(_instance !=  NULL)
		{
			_Lock.Lock();
			if(_instance != NULL)
			{
				delete (T*)_instance;
				_instance = NULL;
			}
			_Lock.UnLock();
		}
	}

protected:
	Singleton( void ){}    
	~Singleton( void ){}

	static CLock _Lock;
	static void* _instance;

private:
	Singleton( const Singleton& );
	Singleton& operator=( const Singleton& );
};

template<typename T> CLock Singleton<T>::_Lock;
template<typename T> void* Singleton<T>::_instance = NULL;

#endif
