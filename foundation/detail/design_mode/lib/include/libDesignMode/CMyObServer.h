#ifndef  __CUSTOM_CMYOBSERVER_HEAD_DEF__
#define  __CUSTOM_CMYOBSERVER_HEAD_DEF__

#include <libContainer/CMyMap.hpp>

typedef long			MYLPARAM;
typedef unsigned int	MYWPARAM;

//观察者接口
class IMyObSever
{
public:
	virtual ~IMyObSever( void ) {};
	virtual void Update( MYWPARAM wParam, MYLPARAM lParam ) = 0;
};

class CMyObSever : public IMyObSever
{
public:
	CMyObSever( void ) {}
	virtual ~CMyObSever( void ) {}
	virtual void Update( MYWPARAM wParam, MYLPARAM lParam ) {}
};

//被观察者接口
class IMyObServable 
{
public:
	virtual ~IMyObServable( void ) {};
	virtual void Attach( IMyObSever *const CObSever ) = 0;
	virtual void Detach( IMyObSever *const CObSever ) = 0;
	virtual void Notify( MYWPARAM wParam, MYLPARAM lParam ) = 0;
};

//被观察者模式
typedef _container_::CMyMap<unsigned int, IMyObSever*>				OBSEVERLIST_MAP;
typedef _container_::CMyMap<unsigned int, IMyObSever*>::iterator	OBSEVERLISTLIST_MAP_ITER;

class CMyObServable : public IMyObServable
{
public:
	CMyObServable(void);
	virtual ~CMyObServable( void );
	virtual void Attach( IMyObSever *const c_CObSever );
	virtual void Detach( IMyObSever *const c_CObSever );
	virtual void Notify( MYWPARAM wParam, MYLPARAM lParam );
private:
	OBSEVERLIST_MAP  m_ObSeverListMap;
};

#endif