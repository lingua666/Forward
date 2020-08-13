
#ifndef  __CUSTOM_CMESSAGEMAP_HEAD_DEF__
#define  __CUSTOM_CMESSAGEMAP_HEAD_DEF__

#include <Foundation/platform/Shared_ptr.h>
#include <Foundation/platform/CMyFunction.hpp>
#include <Foundation/CMyMap.hpp>

typedef	_function_::CMyFunction<void (int,const void*)>	MesMapFunc;

class CMessageMap
{
public:
	typedef	SmartPTR<MesMapFunc>	MesMapFunc_sptr;
	typedef	_container_::CMyMap<int,MesMapFunc_sptr> MesMap_map;

public:
	CMessageMap( void )
	{
		
	}

	virtual ~CMessageMap( void )
	{
		_MesMap_map.clear();
	}
	
	inline void push( int iMessageID, const MesMapFunc& Func )
	{
		MesMapFunc_sptr ptr(new MesMapFunc(Func));
		_MesMap_map.insert1(std::make_pair(iMessageID,ptr));
	}

	inline void invoke( int iMessagID, int wParam, const void* lParam )
	{
		MesMapFunc_sptr	Func_ptr;
		if(_MesMap_map.IsExistKey2(iMessagID,Func_ptr))
		{
			(*(Func_ptr.get()))(wParam,lParam);
		}
	}

protected:
	inline void pop( int iMessageID )
	{
		_MesMap_map.eraseKey(iMessageID);
	}

private:
	MesMap_map	_MesMap_map;
};

#define DECLARE_MESSAGE_MAP()\
public:\
			CMessageMap* GetMessageMap( void );\
private:\
			void RegisterMessageMap( void );\
			CMessageMap	_CMessageMap;
		

#define BEGIN_MESSAGE_MAP(theClass) \
		CMessageMap* theClass::GetMessageMap( void )\
		{\
			return &_CMessageMap;\
		}\
		void theClass::RegisterMessageMap( void )\
		{

#define ON_MESSAGE(id,func)\
		_CMessageMap.push(id,MesMapFunc(func,this));

#define END_MESSAGE_MAP() \
		}

#define REGISTER_MESSAGE_MAP()\
		RegisterMessageMap();

#define SEND_MESSAGE(handle,id,wparam,lparam)\
		handle->GetMessageMap()->invoke(id,wparam,lparam);

#endif