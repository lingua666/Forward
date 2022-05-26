
//#include <pjmedia.h>
#include <pjsip-ua/sip_100rel.h>
#include <pjmedia/endpoint.h>
#include <pjmedia/sdp.h>


#include "../include/PjsipInvite.h"

/*
 * Transport manager.
 */
namespace juli {

pj_bool_t	InviteRegc::_isInit = PJ_FALSE;

InviteRegc::InviteRegc( pjsip_endpoint	*pEndpt, pjsip_inv_callback inv_cb, int iPort )
			:_pMediaEndpt(NULL)
{
	if(!_isInit)
	{
		Init(pEndpt,inv_cb,iPort);
		_isInit = true;
	}
}

InviteRegc::~InviteRegc( void )
{

}

pj_bool_t InviteRegc::Init( pjsip_endpoint	*pEndpt,  pjsip_inv_callback inv_cb, int iPort )
{
	PJ_ASSERT_RETURN(pEndpt != NULL, PJ_FALSE);

	/* Init the callback for INVITE session: */
	pj_status_t status;
	/* Initialize invite session module:  */
	status = pjsip_inv_usage_init(pEndpt, &inv_cb);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);

	/* Initialize 100rel support */
	status = pjsip_100rel_init_module(pEndpt);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

/*#if PJ_HAS_THREADS
	status = pjmedia_endpt_create(PjsipInit::GetInstance()->GetPoolFactory(), NULL, 1, &_pMediaEndpt);
#else
	status = pjmedia_endpt_create(PjsipInit::GetInstance()->GetPoolFactory(), 
		pjsip_endpt_get_ioqueue(pEndpt), 
		0, &_pMediaEndpt);
#endif*/

	PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);
	return PJ_TRUE;
}

void InviteRegc::Release( void )
{

}

const pjsip_uri*	PjsipInvite::GetReceiverUri( pjsip_msg	*msg )
{
	if(msg == NULL)
		return NULL;
	pjsip_uri* pUri = NULL;

	pjsip_routing_hdr* hdr = (pjsip_routing_hdr*) pjsip_msg_find_hdr( msg, PJSIP_H_ROUTE, NULL);
	if(hdr != NULL)
	{
		pUri = hdr->name_addr.uri;

		//目标为本机 Remove ROUTE Header
		/*std::string sHostIP;
		GetHostIP(sHostIP);
		pjsip_sip_uri*	ToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(pUri);
		if(pj_strcmp2(&ToUri->host,sHostIP.c_str()) == 0 && ToUri->port == HOST_PORT_DEF)
		{
			pj_list_erase(hdr);
			pUri = msg->line.req.uri;
		}*/
		//目标为本机
	}
	else
	{
		pUri = msg->line.req.uri;
	}

	return pUri;
}

PjsipInvite::PjsipInvite( pjsip_inv_session *pInv, pj_bool_t isProxy )
			:_pInv(pInv)
			,_isProxy(isProxy)
{
	
}

PjsipInvite::~PjsipInvite( void )
{
	
}

pj_status_t PjsipInvite::CreateMediaBaseSDP( string sUserName,pjmedia_endpt *endpt,
											pj_pool_t *pool,
											const pj_str_t *sess_name,
											const pj_sockaddr *origin,
											pjmedia_sdp_session **p_sdp)
{
	pj_time_val tv;
	pjmedia_sdp_session *sdp;

	static const pj_str_t STR_AUDIO = { "audio", 5};
	static const pj_str_t STR_VIDEO = { "video", 5};
	static const pj_str_t STR_IN = { "IN", 2 };
	static const pj_str_t STR_IP4 = { "IP4", 3};
	static const pj_str_t STR_IP6 = { "IP6", 3};
	static const pj_str_t STR_RTP_AVP = { "RTP/AVP", 7 };
	static const pj_str_t STR_SDP_NAME = { "pjmedia", 7 };
	static const pj_str_t STR_SENDRECV = { "sendrecv", 8 };

	/* Sanity check arguments */
	PJ_ASSERT_RETURN(endpt && pool && p_sdp, PJ_EINVAL);

	sdp = PJ_POOL_ZALLOC_T(pool, pjmedia_sdp_session);

	pj_gettimeofday(&tv);
	/*if(UserName == NULL)
		sdp->origin.user = pj_str("-");
	else*/
		pj_strdup2(pool, &sdp->origin.user, sUserName.c_str());
	
	sdp->origin.version = sdp->origin.id = 0;//tv.sec + 2208988800UL;
	sdp->origin.net_type = STR_IN;

	if (origin->addr.sa_family == pj_AF_INET()) {
		sdp->origin.addr_type = STR_IP4;
		pj_strdup2(pool, &sdp->origin.addr,
			pj_inet_ntoa(origin->ipv4.sin_addr));
	} else if (origin->addr.sa_family == pj_AF_INET6()) {
		char tmp_addr[PJ_INET6_ADDRSTRLEN];

		sdp->origin.addr_type = STR_IP6;
		pj_strdup2(pool, &sdp->origin.addr,
			pj_sockaddr_print(origin, tmp_addr, sizeof(tmp_addr), 0));

	} else {
		pj_assert(!"Invalid address family");
		return PJ_EAFNOTSUP;
	}

	if (sess_name)
		pj_strdup(pool, &sdp->name, sess_name);
	else
		sdp->name = STR_SDP_NAME;

	/* SDP time and attributes. */
	sdp->time.start = sdp->time.stop = 0;
	sdp->attr_count = 0;

	/* Done */
	*p_sdp = sdp;

	return PJ_SUCCESS;
}

pj_status_t PjsipInvite::dlg_set_route_set( pjsip_dialog *dlg,
								const pjsip_route_hdr *route_set )
{
	pjsip_route_hdr *r;

	PJ_ASSERT_RETURN(dlg, PJ_EINVAL);

	pjsip_dlg_inc_lock(dlg);

	/* Clear route set. */
	pj_list_init(&dlg->route_set);

	if (!route_set) {
		pjsip_dlg_dec_lock(dlg);
		return PJ_SUCCESS;
	}

	r = route_set->next;
	do 
	{
		pjsip_route_hdr *new_r;
		new_r = (pjsip_route_hdr*) pjsip_hdr_clone(dlg->pool, r);
		pj_list_push_back(&dlg->route_set, new_r);

		r = r->next;
	} while (r != route_set);

	pjsip_dlg_dec_lock(dlg);
	return PJ_SUCCESS;
}

pj_status_t	PjsipInvite::CreateUasDialog( pjsip_rx_data *rdata,  pj_str_t& psContactUri, pjsip_dialog **pDlg )
{
	pj_status_t  status = pjsip_dlg_create_uas( pjsip_ua_instance(),rdata,&psContactUri,pDlg);

	if(status == PJ_SUCCESS)
		(*pDlg)->local.cseq = 1;
	return status;
}

pj_status_t	PjsipInvite::CreateUasInv( pjsip_dialog *dlg,
						 pjsip_rx_data *rdata,
						 const pjmedia_sdp_session *local_sdp,
						 pjsip_inv_session **p_inv)
{
	return pjsip_inv_create_uas(dlg,rdata,local_sdp,0,p_inv);
}

pj_status_t	PjsipInvite::CreateUacDialog( const pj_str_t *local_uri, const pj_str_t *local_contact,
							const pj_str_t *remote_uri, const pj_str_t *target, pjsip_dialog **pDlg )
{
	pj_status_t  status = pjsip_dlg_create_uac( pjsip_ua_instance(),local_uri, local_contact,
								remote_uri, target,pDlg);

	if(status == PJ_SUCCESS)
		(*pDlg)->local.cseq = 1;
	return status;

}

pj_status_t	PjsipInvite::CreateUacInv( pjsip_dialog *dlg,
						 const pjmedia_sdp_session *local_sdp,
						 pjsip_inv_session **p_inv )
{
	return pjsip_inv_create_uac(dlg,local_sdp,0,p_inv);
}

pj_status_t PjsipInvite::SendAck( int iCSeq, pjmedia_sdp_session *pSDPSession, const char* useragent )
{
	MACRO_IS_VALIED(_pInv)

	if(pSDPSession != NULL)
		pjsip_inv_set_local_sdp(_pInv,pSDPSession);

	pj_status_t status = pjsip_inv_create_ack(_pInv, iCSeq, &_pInv->last_ack);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pInv->dlg->pool,_pInv->last_ack->msg)
	
	/* Send ACK */
	status = pjsip_dlg_send_request(_pInv->dlg, _pInv->last_ack, -1, NULL);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);

	return status;
}

pj_status_t	PjsipInvite::SendInvAnswer1( int st_code, const pjmedia_sdp_session *sdp, const char* useragent )
{
	MACRO_IS_VALIED(_pInv)
	
	pjsip_tx_data *pTData = NULL;
	pj_status_t status = pjsip_inv_answer( _pInv, 
										st_code, NULL,	/* st_code and st_text */
										sdp,		/* SDP already specified */
										&pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pInv->dlg->pool,pTData->msg)
	
	status = pjsip_inv_send_msg(_pInv, pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);
	return status;
}

pj_status_t	PjsipInvite::SendInvAnswer2( pjsip_rx_data *rdata, int st_code,
										const pjmedia_sdp_session *sdp, 
										const char* useragent )
{
	MACRO_IS_VALIED(_pInv)

	pjsip_tx_data *pTData = NULL;
	pj_status_t 	status = pjsip_inv_initial_answer(_pInv, rdata, 
													st_code, 
													NULL, sdp, &pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pInv->dlg->pool,pTData->msg)

	status = pjsip_inv_send_msg(_pInv, pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);
	return status;
}

pj_status_t PjsipInvite::SendEndSession( int st_code, const char* useragent )
{
	MACRO_IS_VALIED(_pInv)

	pjsip_tx_data *pBye;
	pj_status_t status = pjsip_inv_end_session(_pInv, st_code, 
												NULL, &pBye);
	
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);

	//Add User-Agent
	if(pBye != NULL)
	{
		if(pBye->msg != NULL)
			MACRO_ADD_USER_AGENT(useragent,_pInv->dlg->pool,pBye->msg)

			status = pjsip_inv_send_msg(_pInv, pBye);
		PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);
	}
	return status;
}

pj_status_t PjsipInvite::SendTerminateSession( int st_code )
{
	MACRO_IS_VALIED(_pInv)
	pj_status_t status = pjsip_inv_terminate(_pInv, st_code, PJ_FALSE);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);
	return status;
}

pj_status_t	PjsipInvite::CreateVideoSDPSession( const char* c_szID,
										const char* c_szSDPName ,
										const char* c_szIP, int iPort,
										pjmedia_sdp_session **pSDPSession )
{
	char szSDP[500] = {0};
	int iLen = sprintf(szSDP,
		"v=0\r\n"
		"o=%s 0 0 IN IP4 %s\r\n"
		"s=%s\r\n"
		"c=IN IP4 %s\r\n"
		"t=0 0\r\n"
		"m=video %d RTP/AVP 96 98 97\r\n"
		"a=recvonly\r\n"
		"a=rtpmap:96 PS/90000\r\n"
		"a=rtpmap:98 H264/90000\r\n"
		"a=rtpmap:97 MPEG4/90000\r\n",
		c_szID, c_szIP, c_szSDPName, c_szIP, iPort);
	
	return pjmedia_sdp_parse(_pInv->dlg->pool,szSDP,iLen,pSDPSession);
}

pj_status_t	PjsipInvite::SendInvRequest( const char* szSubject, const char* useragent )
{
	pjsip_tx_data *pTData = NULL;
	/* Create initial INVITE request.
	* This INVITE request will contain a perfectly good request and 
	* an SDP body as well.
	*/

	pj_status_t status = pjsip_inv_invite(_pInv, &pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, 1);

	if(szSubject != NULL)
	{
		const pj_str_t SUBJECT = { "Subject", 7};
		pj_str_t	Subject = pj_str(const_cast<char*>(szSubject));
		pjsip_hdr *hdr = (pjsip_hdr*) pjsip_generic_string_hdr_create(_pInv->dlg->pool,
									&SUBJECT,
									&Subject);
		PJ_ASSERT_RETURN(hdr != NULL, PJSIP_ESHUTDOWN);
		pjsip_msg_add_hdr(pTData->msg, hdr);
	}

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pInv->dlg->pool,pTData->msg)

	/* Send initial INVITE request. 
	* From now on, the invite session's state will be reported to us
	* via the invite session callbacks.
	*/
	status = pjsip_inv_send_msg(_pInv, pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);
	return status;
}

pj_status_t	PjsipInvite::SendInvRespond( pjsip_rx_data *rdata, pj_str_t& psContactUri,
										const pjmedia_sdp_session *pLocalSDP, const char* useragent )
{
	pjsip_tx_data	*pTData = NULL;

	/*
	* The very first response to an INVITE must be created with
	* pjsip_inv_initial_answer(). Subsequent responses to the same
	* transaction MUST use pjsip_inv_answer().
	*/
	pj_status_t status = pjsip_inv_initial_answer(_pInv, rdata, 
		PJSIP_SC_TRYING, 
		NULL, NULL, &pTData);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pInv->dlg->pool,pTData->msg)

	/* Send response. */  
	status = pjsip_inv_send_msg(_pInv, pTData); 
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(_pInv->dlg) && PJ_TRUE);
	return status;
}


} // namespace juli

