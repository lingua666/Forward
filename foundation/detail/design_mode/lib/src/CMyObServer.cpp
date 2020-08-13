
#include <libDesignMode/CMyObServer.h>

//±ªπ€≤Ï’ﬂ
CMyObServable::CMyObServable(void)
{

}

CMyObServable::~CMyObServable( void )
{
	m_ObSeverListMap.clear();
}

void CMyObServable::Attach(  IMyObSever *const c_CObSever )
{
	m_ObSeverListMap.insert1(std::make_pair(*((unsigned int*)(&c_CObSever)), 
						const_cast<IMyObSever*>(c_CObSever)));
}

void CMyObServable::Detach( IMyObSever *const c_CObSever )
{
	OBSEVERLISTLIST_MAP_ITER iter = m_ObSeverListMap.find(*((unsigned int*)(&c_CObSever)));
	if(iter != m_ObSeverListMap.end())
	{
		m_ObSeverListMap.erase(iter);
	}
}

void CMyObServable::Notify( MYWPARAM wParam, MYLPARAM lParam )
{
	OBSEVERLISTLIST_MAP_ITER iter;
	for(iter = m_ObSeverListMap.begin(); iter != m_ObSeverListMap.end();)
	{
		if(iter->second != NULL)
		{
			(reinterpret_cast<IMyObSever*>(iter->second))->Update(wParam,lParam);
		}
		iter ++;
	}
}
