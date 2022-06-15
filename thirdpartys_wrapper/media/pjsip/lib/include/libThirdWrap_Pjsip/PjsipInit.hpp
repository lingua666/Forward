
#ifndef  _JULI_PJSIPINIT_H_DEF_
#define _JULI_PJSIPINIT_H_DEF_

#include "External.h"
#include <pj/log.h>
#include <pjlib.h>
#include <pjsip.h>
#include <pjlib-util.h>
#include <pjsip/sip_endpoint.h>
#include <pjmedia/errno.h>

#include <string>
using namespace std;

#define	COMPANY_STRING_DEF		"Personage V1.0"
#define USERAGENT_STRING_DEF	"User Agent"
#define PROXY_STRING_DEF		"Proxy"
#define MEDIASERVER_STRING_DEF	"Media Server"

#define	PROXY_FLAG_STRING_DEF		"Proxy Personage V1.0"//PROXY_STRING_DEF##COMPANY_STRING_DEF
#define	USERAGENT_FLAG_STRING_DEF	"User Agent Personage V1.0"//USERAGENT_STRING_DEF##COMPANY_STRING_DEF
#define MEDIASERVER_FLAG_STRING_DEF "Media Server Personage V1.0"//MEDIASERVER_STRING_DEF##COMPANY_STRING_DEF

#define DEFAULT_FLAG_STRING_DEF		MEDIASERVER_FLAG_STRING_DEF//PROXY_FLAG_STRING_DEF//

#define	USE_MEDIATRANSFER_MODE_DEF

//#define	ENABLE_MYSQL
#define	USE_IOSERVRICEIOEX
//#define USE_28181STANDARD


/* Settings */
#define AF		pj_AF_INET() /* Change to pj_AF_INET6() for IPv6.*/

namespace _thirdwrap_ {

struct pj_pool_t;
struct pjsip_endpoint;
struct pjsip_regc;
struct pjsip_regc_cbparam;

typedef enum ErrorCode
{
	EC_S_OK					= 0,
	EC_INVALID_PARAMETER	= 1,
	EC_MEMORY_NOENOUGH		= 3,
	EC_EXISTING				= 4,
	EC_OTHER_FAIL			= 5,
}emErrorCode;

#define MACRO_ADD_USER_AGENT(useragent,pPool,msg)\
	if(useragent != NULL)\
	{\
		const pj_str_t USER_AGENT = { "User-Agent", 10};\
		if(pjsip_msg_find_hdr_by_name(msg,&USER_AGENT,NULL) == NULL)\
		{\
			pj_str_t	user_agent = pj_str(const_cast<char*>(useragent));\
			pjsip_hdr *hdr = (pjsip_hdr*) pjsip_generic_string_hdr_create(pPool,\
											&USER_AGENT,\
											&user_agent);\
			pjsip_msg_add_hdr(msg, hdr);\
		}\
	}

class PjsipInit
{
public:
	static PjsipInit* GetInstance( void )
	{
		static PjsipInit _sPjsipInit;
		return &_sPjsipInit;
	}

	static inline pj_status_t GetHostIP( std::string& sIP )
	{
		pj_sockaddr HostAddr = {0};
		char szHostIP[30] = {0};
		pj_status_t status = pj_gethostip(AF, &HostAddr);
		if (status == PJ_SUCCESS) 
		{
			pj_sockaddr_print(&HostAddr, szHostIP, sizeof(szHostIP), 2);
			sIP = szHostIP;
		}

		return status;
	}

public:
	PjsipInit( void )
	{
		LOG_Print_SetLog(ThirdWrap_Pjsip, MLog_GetAllLevel())

		/* Must init PJLIB first: */
		pj_status_t status = pj_init();
		if(status != PJ_SUCCESS)
		{
			LOG_Print_Error(ThirdWrap_Pjsip, "pj_init is Failed,status:%d", status);
			return ;
		}
			
		/* Then init PJLIB-UTIL: */
		status = pjlib_util_init();
		if(status != PJ_SUCCESS)
		{
			LOG_Print_Error(ThirdWrap_Pjsip, "pjlib_util_init is Failed,status:%d", status);
			return ;
		}

		/*//初始化pjnath库
		status = pjnath_init(); 

		//初始化pjsua-lib库(使用标准协议(STUN,TURN,ICE)提供NAT设备穿越功能的中间件.);
		//(里面初始化了pjlib,pjlib-util,pjnath)
		status = pjsua_create();*/
		
		/* Must create a pool factory before we can allocate any memory. */
		pj_caching_pool_init(&_cp,&pj_pool_factory_default_policy,0);

		/* Create pool for the application */
		_pPool = pj_pool_create(&_cp.factory, "PjsipPool", 
								4000, 4000, NULL);
		if(status != PJ_SUCCESS)
		{
			LOG_Print_Error(ThirdWrap_Pjsip, "pj_pool_create is Failed,status:%d", status);
			return ;
		}

		/* Create the endpoint: */
		status = pjsip_endpt_create(&_cp.factory, "PjsipEndpt", &_pEndpt);
		if(status != PJ_SUCCESS)
		{
			LOG_Print_Error(ThirdWrap_Pjsip, "pjsip_endpt_create is Failed,status:%d", status);
			return ;
		}
		//PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

		/* Init transaction layer for stateful proxy only */
		status = pjsip_tsx_layer_init_module(_pEndpt);
		//PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
		if(status != PJ_SUCCESS)
		{
			LOG_Print_Error(ThirdWrap_Pjsip, "pjsip_tsx_layer_init_module is Failed,status:%d", status);
			return ;
		}

		/* 
		* Initialize UA layer module.
		* This will create/initialize dialog hash tables etc.
		*/
		status = pjsip_ua_init_module( _pEndpt, NULL );
		//PJ_ASSERT_RETURN(status == PJ_SUCCESS, 1);	
		if(status != PJ_SUCCESS)
		{
			LOG_Print_Error(ThirdWrap_Pjsip, "pjsip_ua_init_module is Failed,status:%d", status);
			return ;
		}
	}

	~PjsipInit( void )
	{
		//Destroy the user agent layer.
		pjsip_ua_destroy();

		if(_pEndpt != NULL)
		{
			pjsip_endpt_destroy (_pEndpt);
			_pEndpt = NULL;
		}

		if(_pPool != NULL)
		{
			pj_pool_release(_pPool);
			_pPool = NULL;
		}

		//Destroy pool factory
		pj_caching_pool_destroy(&_cp);

		//Stop pjlib
		pj_shutdown(); 
	}

	inline pj_pool_t* GetPool( void ) const
	{
		return _pPool;
	}

	inline	pjsip_endpoint*	GetEndpt( void ) const
	{
		return _pEndpt;
	}

	inline	pj_pool_factory* GetPoolFactory( void )
	{
		return &_cp.factory;
	}

private:
	pj_caching_pool _cp;
	pj_pool_t		*_pPool;
	pjsip_endpoint	*_pEndpt;
};

} // namespace juli

#endif
