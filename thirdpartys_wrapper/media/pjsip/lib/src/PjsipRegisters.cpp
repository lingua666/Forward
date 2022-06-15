
#include "../include/PjsipRegisters.h"
#include "../include/PjsipUA.h"

namespace _thirdwrap_ {

static pj_str_t	pjsip_USERNAME_STR =	{ "username", 8 },
				pjsip_REALM_STR =	    { "realm", 5},
				pjsip_NONCE_STR =	    { "nonce", 5},
				pjsip_URI_STR =		    { "uri", 3 },
				pjsip_RESPONSE_STR =	{ "response", 8 },
				pjsip_ALGORITHM_STR =	{ "algorithm", 9 },
				pjsip_CNONCE_STR =	    { "cnonce", 6},
				pjsip_OPAQUE_STR =	    { "opaque", 6},
				pjsip_QOP_STR =		    { "qop", 3},
				pjsip_NC_STR =		    { "nc", 2};


PjsipRegisters::PjsipRegisters( void )
				:_isProxy(PJ_FALSE)
				,_pPool(NULL)
				,_pRegc(NULL)
				,_pEndpt(NULL)
				,_isRegc(PJ_FALSE)
{
	
}

PjsipRegisters::~PjsipRegisters( void )
{
	Release();
}

pj_bool_t	PjsipRegisters::Init( pj_bool_t isProxy, pjsip_endpoint* pEndpt, on_regc_cb OnRegcCB )
{
	PJ_ASSERT_RETURN(pEndpt != NULL, PJ_FALSE);
	_pEndpt = pEndpt;
	if(!_isProxy && _pRegc == NULL)
	{
		_pPool = pjsip_endpt_create_pool(_pEndpt,"RegistersPool",1024 * 1024, 1024 * 1024);
		PJ_ASSERT_RETURN(_pPool != NULL, PJ_FALSE);

		pj_status_t status = pjsip_regc_create (_pEndpt, this, OnRegcCB, &_pRegc);
		if(status != PJ_SUCCESS)
		{
			return PJ_FALSE;
		}
	}

	return PJ_TRUE;
}

void		PjsipRegisters::Release( void )
{
	if(_pRegc != NULL)
	{
		pjsip_regc_destroy(_pRegc);
		_pRegc = NULL;
	}

	if(_pPool != NULL)
	{
		pjsip_endpt_release_pool(_pEndpt,_pPool);
		_pPool = NULL;
	}
}

pj_status_t	PjsipRegisters::ParseRxData( pjsip_rx_data *rdata )
{
	PJ_ASSERT_RETURN(rdata != NULL, NULL);

	if(_isProxy)
	{//Proxy Process

	}
	else
	{//User Agent Process

	}

	return PJ_SUCCESS;
}

pj_status_t	PjsipRegisters::SendRegistersRequest( pj_str_t psSvrUrl, pj_str_t psFromUrl, 
												pj_str_t psToUrl, int iCCnt,
												const pj_str_t psContact[],
												pj_uint32_t puExpires, const pj_str_t* pUserName,
												const pj_str_t* pPassword,
												int iSendPort,
												const char* useragent )
{
	PJ_ASSERT_RETURN(_pRegc != NULL, PJSIP_ENOTINITIALIZED);

	pj_status_t status = pjsip_regc_init(_pRegc,&psSvrUrl,&psFromUrl,
		&psToUrl,iCCnt,psContact,puExpires);
	if(status != PJ_SUCCESS)
	{
		LOG_Print_Error(ThirdWrap_Pjsip,"pjsip_regc_init() error\r\n");
		return status;
	}

	if(pUserName != NULL)
	{// Set UserName And Password
		pjsip_cred_info cred;
		pj_bzero(&cred,sizeof(pjsip_cred_info));
		pj_cstr(&cred.realm, "*");
		pj_cstr(&cred.scheme, "digest");
		pj_cstr(&cred.data, "");
		cred.data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
		cred.username = *pUserName;
		if(pPassword != NULL)
			cred.data = *pPassword;

		status = pjsip_regc_set_credentials(_pRegc, 1, &cred);
		if(status != PJ_SUCCESS)
			return status;
	}

	pjsip_tx_data * p_tdata;
	status = pjsip_regc_register(_pRegc,PJ_TRUE,&p_tdata);
	if(status != PJ_SUCCESS)
	{
		LOG_Print_Error(ThirdWrap_Pjsip,"pjsip_regc_register() error\r\n");
		return status;
	}

	if(iSendPort > 0)
	{
		pj_str_t type = pj_str("Application");
		pj_str_t subtype = pj_str("MANSCDP+xml");

		char szHost[100] = {0};
		sprintf(szHost,"%d",iSendPort);
		pj_str_t text = pj_str(szHost);
		pjsip_msg_body* msgBody = pjsip_msg_body_create(_pPool,&type,&subtype,&text);
		p_tdata->msg->body = msgBody;
	}

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,p_tdata->msg)

	status = pjsip_regc_send (_pRegc, p_tdata);
	if(status != PJ_SUCCESS)
	{
		LOG_Print_Error(ThirdWrap_Pjsip,"pjsip_regc_send() error\r\n");
	}
	return status;
}

pj_status_t	PjsipRegisters::SendUnRegistersRequest( const char* useragent )
{
	PJ_ASSERT_RETURN(_pRegc != NULL, PJSIP_ENOTINITIALIZED);
	pjsip_tx_data *p_tdata;

	pj_status_t status = pjsip_regc_unregister(_pRegc,&p_tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	
	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,p_tdata->msg)
	return pjsip_regc_send (_pRegc, p_tdata);
}

int	PjsipRegisters::SendRegistersResponse( pjsip_rx_data *rdata,
										  stUserAgentAddress* pUserAgentAddress,
										  on_auth_lookup_cred	OnAuthLookupCred,
										  const char* c_sz_MediaServerIP, int iMediaPort,
										  const char* useragent )
{
	if(rdata == NULL)
		return STATUS_FAILED;

	pjsip_contact_hdr* contact_hdr = (pjsip_contact_hdr*)
		pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_CONTACT,
		NULL);
	pjsip_sip_uri*	pContactUri = (pjsip_sip_uri*)pjsip_uri_get_uri(contact_hdr->uri);
	if(pContactUri->user.slen <= 0 || pContactUri->port < 0 || pContactUri->host.slen <= 0)
		return STATUS_FAILED;

	pjsip_sip_uri*	pToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);
	if(pToUri->user.slen <= 0)
		return STATUS_FAILED;

	char szDstName[100] = {0};
	strncpy(szDstName,pToUri->user.ptr,pToUri->user.slen);

	//pjsip_tx_data *tdata;
	pjsip_auth_srv auth_srv = {0};
	int pjstate = 0;
	pjsip_transport *uas_tsx = rdata->tp_info.transport;//pjsip_rdata_get_tsx(rdata);
	PJ_ASSERT_RETURN(uas_tsx != NULL, PJ_FALSE);

	pj_str_t H_RR = pj_str("Personage Sip Proxy");
	const pjsip_authorization_hdr* h_auth = FindAuthHdr(rdata);
	if(h_auth != NULL)
		H_RR = h_auth->credential.digest.realm;

	PjsipUserAgent	UserAgent(uas_tsx->endpt);
	pjsip_auth_srv_init(uas_tsx->pool,&auth_srv,&H_RR,OnAuthLookupCred,PJSIP_AUTH_SRV_IS_PROXY);
	pj_status_t status = pjsip_auth_srv_verify(&auth_srv,rdata,&pjstate);
	if(PJ_SUCCESS == status)
	{
		//200 0K
		pjsip_msg_body* msgBody = NULL;
		if(c_sz_MediaServerIP != NULL)
		{
			pj_str_t type = pj_str("Application");
			pj_str_t subtype = pj_str("MANSCDP+xml");

			char szHost[100] = {0};
			sprintf(szHost,"%s:%d",c_sz_MediaServerIP,iMediaPort);
			pj_str_t text = pj_str(szHost);
			msgBody = pjsip_msg_body_create(uas_tsx->pool,&type,&subtype,&text);
			
		}

		UserAgent.SendEndptResponseStateless(rdata,PJSIP_SC_OK,useragent,NULL,NULL,msgBody);
		PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);
		
		const pjsip_expires_hdr* pExpires = FindExpiresHdr(rdata);
		if(pExpires->ivalue == 0)
		{//unRegisters
			//UserManagement::GetIntance()->Remove(szDstName);
			LOG_Print_Error(ThirdWrap_Pjsip,"UserName:%s unRegisters\r\n",szDstName);
			if(pjstate == PJSIP_SC_OK)
			{
				pjstate = STATUS_UNREGISTERS;
			}
		}
		
		if(pUserAgentAddress != NULL)
		{
			pjsip_contact_hdr* contact_hdr = (pjsip_contact_hdr*)
				pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_CONTACT,
				NULL);
			pUserAgentAddress->sUserName = std::string(szDstName);
			
			//test
			char szUri[100] = {0};
			strncpy(szUri,contact_hdr->name.ptr,contact_hdr->name.slen);

			char szRemoteHost[100] = {0};
			pUserAgentAddress->iPort = rdata->msg_info.via->rport_param;
			strncpy(szRemoteHost,rdata->msg_info.via->recvd_param.ptr,rdata->msg_info.via->recvd_param.slen);
			pUserAgentAddress->sIP = std::string(szRemoteHost);
			pUserAgentAddress->sSpiUri = std::string(szDstName);
			//test
		}
	}
	else if(PJSIP_EAUTHNOAUTH == status && pjstate == PJSIP_SC_PROXY_AUTHENTICATION_REQUIRED)
	{
		status = UserAgent.SendAuthSrvResponse(rdata,&auth_srv,useragent);
		PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);
	}
	else
	{//ÆäËû´íÎó
		status = UserAgent.SendEndptResponseStateless(rdata,pjstate,useragent);
		PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);
	}

	return pjstate;
}

pj_status_t	PjsipRegisters::GenerateResponseCode( pj_str_t psUserName, pj_str_t psPassword,
												 pjsip_digest_credential* pDigCred,
												 pj_str_t* outResponseCode )
{
	PJ_ASSERT_RETURN(pDigCred != NULL, PJ_EINVAL);
	PJ_ASSERT_RETURN(outResponseCode != NULL, PJ_EINVAL);
	PJ_ASSERT_RETURN(outResponseCode->slen >= PJSIP_MD5STRLEN, PJ_ENOMEM);

	pj_str_t	pjmethod = pj_str("REGISTER");

	//Init UserName And Password
	pjsip_cred_info credinfo;
	pj_bzero(&credinfo,sizeof(pjsip_cred_info));
	credinfo.username = psUserName;
	credinfo.data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	credinfo.data = psPassword;

	//Generate Code
	pjsip_auth_create_digest(outResponseCode,&pDigCred->nonce,&pDigCred->nc,
							&pDigCred->cnonce,&pDigCred->qop,
							&pDigCred->uri,&pDigCred->realm,&credinfo,&pjmethod);

	return PJ_SUCCESS;
}

pj_status_t	PjsipRegisters::GenerateResponseCode( pj_str_t psUserName, pj_str_t psPassword,
												 pj_str_t psNonce, pj_str_t psNc,
												 pj_str_t psCNonce, pj_str_t psQop,
												 pj_str_t psUri, pj_str_t psRealm,
												 pj_str_t* outResponseCode )
{
	PJ_ASSERT_RETURN(outResponseCode != NULL, PJ_EINVAL);
	PJ_ASSERT_RETURN(outResponseCode->slen >= PJSIP_MD5STRLEN, PJ_ENOMEM);

	pj_str_t	pjmethod = pj_str("REGISTER");

	//Init UserName And Password
	pjsip_cred_info credinfo;
	pj_bzero(&credinfo,sizeof(pjsip_cred_info));
	credinfo.username = psUserName;
	credinfo.data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	credinfo.data = psPassword;

	//Generate Code
	pjsip_auth_create_digest(outResponseCode,&psNonce,&psNc,
		&psCNonce,&psQop,&psUri,&psRealm,&credinfo,&pjmethod);

	return PJ_SUCCESS;
}

const pjsip_authorization_hdr*	PjsipRegisters::FindAuthHdr( const pjsip_rx_data *rdata ) const
{
	PJ_ASSERT_RETURN(rdata != NULL, NULL);
	PJ_ASSERT_RETURN(rdata->msg_info.msg != NULL, NULL);
	
	pjsip_authorization_hdr* h_auth = (pjsip_authorization_hdr*) pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_PROXY_AUTHORIZATION, NULL);
	if(h_auth == NULL)
	{
		h_auth = (pjsip_authorization_hdr*) pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_AUTHORIZATION, NULL);
		if(h_auth == NULL)
		{
			h_auth = (pjsip_authorization_hdr*) pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_PROXY_AUTHENTICATE, NULL);
		}
	}

	return h_auth;
}

const pjsip_expires_hdr*	PjsipRegisters::FindExpiresHdr( const pjsip_rx_data *rdata ) const
{
	PJ_ASSERT_RETURN(rdata != NULL, NULL);
	PJ_ASSERT_RETURN(rdata->msg_info.msg != NULL, NULL);
	/* Get Expires header */
	return (const pjsip_expires_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_EXPIRES, NULL);

}

void		PjsipRegisters::ParseDigestCredential( pj_scanner *pScanner, pj_pool_t *pPool, 
												pjsip_digest_credential *pCred )
{
	pj_list_init(&pCred->other_param);

	for (;;) {
		pj_str_t name, value;
		
		pjsip_parse_param_imp(pScanner, pPool, &name, &value,
			PJSIP_PARSE_REMOVE_QUOTE);

		if (!pj_stricmp(&name, &pjsip_USERNAME_STR)) {
			pCred->username = value;

		} else if (!pj_stricmp(&name, &pjsip_REALM_STR)) {
			pCred->realm = value;

		} else if (!pj_stricmp(&name, &pjsip_NONCE_STR)) {
			pCred->nonce = value;

		} else if (!pj_stricmp(&name, &pjsip_URI_STR)) {
			pCred->uri = value;

		} else if (!pj_stricmp(&name, &pjsip_RESPONSE_STR)) {
			pCred->response = value;

		} else if (!pj_stricmp(&name, &pjsip_ALGORITHM_STR)) {
			pCred->algorithm = value;

		} else if (!pj_stricmp(&name, &pjsip_CNONCE_STR)) {
			pCred->cnonce = value;

		} else if (!pj_stricmp(&name, &pjsip_OPAQUE_STR)) {
			pCred->opaque = value;

		} else if (!pj_stricmp(&name, &pjsip_QOP_STR)) {
			pCred->qop = value;

		} else if (!pj_stricmp(&name, &pjsip_NC_STR)) {
			pCred->nc = value;

		} else {
			pjsip_param *p = PJ_POOL_ALLOC_T(pPool, pjsip_param);
			p->name = name;
			p->value = value;
			pj_list_insert_before(&pCred->other_param, p);
		}

		/* Eat comma */
		if (!pj_scan_is_eof(pScanner) && *pScanner->curptr == ',')
		{
			pj_scan_get_char(pScanner);
			//Filtering the blank space
			while(!pj_scan_is_eof(pScanner) && *pScanner->curptr == ' ')
				pj_scan_get_char(pScanner);
		}
		else
			break;
	}
}

} // namespace _thirdwrap_
