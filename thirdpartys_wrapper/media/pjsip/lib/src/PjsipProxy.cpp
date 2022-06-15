
#include "../include/PjsipProxy.h"
#include "../include/PjsipRegisters.h"
#include "../include/PjsipInvite.h"
#include "../include/PjsipUA.h"

namespace juli {

#if PJ_HAS_THREADS
int PjsipProxy::WorkerThread(void *p)
{
	PjsipProxy* pProxy = reinterpret_cast<PjsipProxy*>(p);
	pj_time_val delay = {0, 10};
	PJ_UNUSED_ARG(p);

	while(pProxy->_isInit)
	{
		pjsip_endpt_handle_events(pProxy->_pEndpt, &delay);
	}

	return 0;
}
#endif

PjsipProxy::PjsipProxy( void )
			:_pEndpt(NULL)
			,_pPool(NULL)
			,_pTransport(NULL)
			,_pThread(NULL)
			,_iPort(0)
			,_sRealm("MyPjsipProxy")
			,_isInit(PJ_FALSE)
{
	pj_bzero(&_Module,sizeof(pjsip_module));
}

PjsipProxy::PjsipProxy( std::string &sIP, int iPort, std::string sRealm,
					   on_rx_Request	OnRequest,
					   on_rx_Response OnResponse )
			:_pEndpt(NULL)
			,_pPool(NULL)
			,_pTransport(NULL)
			,_pThread(NULL)
			,_iPort(iPort)
			,_sRealm(sRealm)
			,_isInit(PJ_FALSE)
{
	pj_bzero(&_Module,sizeof(pjsip_module));
	Init(sIP,iPort,OnRequest,OnResponse);
}

PjsipProxy::~PjsipProxy( void )
{
	Release();
}

pj_bool_t	PjsipProxy::Init( const std::string &sIP,	int iPort,
							 on_rx_Request	OnRequest,
							 on_rx_Response OnResponse )
{
	if(_isInit)
		return PJ_SUCCESS;

	/* Get the endpoint: */
	_pEndpt = PjsipInit::GetInstance()->GetEndpt();

	/* Get pool for the application */
	_pPool = pjsip_endpt_create_pool(_pEndpt, "ProxyEndptPool", 1024 * 1024, 4 * 1024);
	PJ_ASSERT_RETURN(_pPool != NULL, PJ_FALSE);

	RegisterModule(OnRequest,OnResponse);

	/* Create listening transport */
	pj_sockaddr_in addr = {0};
	addr.sin_family = pj_AF_INET();
	addr.sin_port = pj_htons((pj_uint16_t)iPort);

	pj_status_t status ;
	status = pjsip_udp_transport_start( _pEndpt, &addr, 
		NULL, 32, &_pTransport);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);

	_isInit = PJ_TRUE;

#if PJ_HAS_THREADS
	status = pj_thread_create(_pPool, "PjsipProxyThread", &WorkerThread, 
							this, 0, 0, &_pThread);

	PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);
#endif

	_sIP = sIP;
	_iPort = iPort;
	return PJ_TRUE;
}

void	PjsipProxy::Release( void )
{
	if(!_isInit)
		return ;

	_isInit = PJ_FALSE;
	UnRegisterModule();

	if(_pThread != NULL)
	{
		pj_thread_join(_pThread);
		pj_thread_destroy(_pThread);
		_pThread = NULL;
	}
	
	if(_pTransport != NULL)
	{
		//pjsip_transport_shutdown(_pTransport);
		pjsip_udp_transport_pause(_pTransport,PJSIP_UDP_TRANSPORT_DESTROY_SOCKET);
		_pTransport = NULL;
	}

	if(_pEndpt != NULL && _pPool != NULL)
	{
		pjsip_endpt_release_pool(_pEndpt,_pPool);
		_pPool = NULL;
	}

	_pEndpt = NULL;
}

pj_status_t	PjsipProxy::ProcessRouting(pjsip_tx_data *tdata)
{
	pjsip_sip_uri *target;
    pjsip_route_hdr *hroute;

    /* RFC 3261 Section 16.4 Route Information Preprocessing */

    target = (pjsip_sip_uri*) tdata->msg->line.req.uri;

    /* The proxy MUST inspect the Request-URI of the request.  If the
     * Request-URI of the request contains a value this proxy previously
     * placed into a Record-Route header field (see Section 16.6 item 4),
     * the proxy MUST replace the Request-URI in the request with the last
     * value from the Route header field, and remove that value from the
     * Route header field.  The proxy MUST then proceed as if it received
     * this modified request.
     */
    if (PJ_TRUE/*is_uri_local(target)*/) {
	pjsip_route_hdr *r;
	pjsip_sip_uri *uri;

	/* Find the first Route header */
	r = hroute = (pjsip_route_hdr*)
		     pjsip_msg_find_hdr(tdata->msg, PJSIP_H_ROUTE, NULL);
	if (r == NULL) {
	    /* No Route header. This request is destined for this proxy. */
	    return PJ_SUCCESS;
	}

	/* Find the last Route header */
	while ( (r=(pjsip_route_hdr*)pjsip_msg_find_hdr(tdata->msg, 
						        PJSIP_H_ROUTE, 
							r->next)) != NULL )
	{
	    hroute = r;
	}

	/* If the last Route header doesn't have ";lr" parameter, then
	 * this is a strict-routed request indeed, and we follow the steps
	 * in processing strict-route requests above.
	 *
	 * But if it does contain ";lr" parameter, skip the strict-route
	 * processing.
	 */
	uri = (pjsip_sip_uri*)
	      pjsip_uri_get_uri(&hroute->name_addr);
	if (uri->lr_param == 0) {
	    /* Yes this is strict route, so:
	     * - replace req URI with the URI in Route header,
	     * - remove the Route header,
	     * - proceed as if it received this modified request. 
	    */
	    tdata->msg->line.req.uri = hroute->name_addr.uri;
	    target = (pjsip_sip_uri*) tdata->msg->line.req.uri;
	    pj_list_erase(hroute);
	}
    }

    /* If the Request-URI contains a maddr parameter, the proxy MUST check
     * to see if its value is in the set of addresses or domains the proxy
     * is configured to be responsible for.  If the Request-URI has a maddr
     * parameter with a value the proxy is responsible for, and the request
     * was received using the port and transport indicated (explicitly or by
     * default) in the Request-URI, the proxy MUST strip the maddr and any
     * non-default port or transport parameter and continue processing as if
     * those values had not been present in the request.
     */
    if (target->maddr_param.slen != 0) {
	pjsip_sip_uri maddr_uri;

	maddr_uri.host = target->maddr_param;
	maddr_uri.port = 5060;

	if (PJ_TRUE/*is_uri_local(&maddr_uri)*/) {
	    target->maddr_param.slen = 0;
	    target->port = 0;
	    target->transport_param.slen = 0;
	}
    }

    /* If the first value in the Route header field indicates this proxy,
     * the proxy MUST remove that value from the request.
     */
    hroute = (pjsip_route_hdr*) 
	      pjsip_msg_find_hdr(tdata->msg, PJSIP_H_ROUTE, NULL);
    if (hroute && PJ_TRUE/*is_uri_local((pjsip_sip_uri*)hroute->name_addr.uri)*/) {
	pj_list_erase(hroute);
    }

    return PJ_SUCCESS;
}

pj_status_t	PjsipProxy::CalculateTarget( pjsip_rx_data *rdata, pjsip_tx_data *tdata )
{
   pjsip_sip_uri *target;

    /* RFC 3261 Section 16.5 Determining Request Targets */

    target = (pjsip_sip_uri*) tdata->msg->line.req.uri;

    /* If the Request-URI of the request contains an maddr parameter, the
     * Request-URI MUST be placed into the target set as the only target
     * URI, and the proxy MUST proceed to Section 16.6.
     */
    if (target->maddr_param.slen) {
	//proxy_postprocess(tdata);
	return PJ_SUCCESS;
    }


    /* If the domain of the Request-URI indicates a domain this element is
     * not responsible for, the Request-URI MUST be placed into the target
     * set as the only target, and the element MUST proceed to the task of
     * Request Forwarding (Section 16.6).
     */
    if (PJ_TRUE/*!is_uri_local(target)*/) {
	//proxy_postprocess(tdata);
	return PJ_SUCCESS;
    }

    /* If the target set for the request has not been predetermined as
     * described above, this implies that the element is responsible for the
     * domain in the Request-URI, and the element MAY use whatever mechanism
     * it desires to determine where to send the request. 
     */

    /* We're not interested to receive request destined to us, so
     * respond with 404/Not Found (only if request is not ACK!).
     */
    if (rdata->msg_info.msg->line.req.method.id != PJSIP_ACK_METHOD) {
	/*pjsip_endpt_respond_stateless(global.endpt, rdata,
				      PJSIP_SC_NOT_FOUND, NULL,
				      NULL, NULL);*/
    }

    /* Delete the request since we're not forwarding it */
    pjsip_tx_data_dec_ref(tdata);

    return PJSIP_ERRNO_FROM_SIP_STATUS(PJSIP_SC_NOT_FOUND);
}

void			PjsipProxy::AddRouteHeader(pjsip_tx_data *tdata)
{
	 /*pjsip_route_hdr routeSet;
	 pj_list_init(&routeSet);
	 pj_str_t psRoute = pj_str("Route");
	 pjsip_route_hdr *route = reinterpret_cast<pjsip_route_hdr *>(pjsip_parse_hdr(dlg->pool, 
		 &psRoute, "<sip:34020000001310000051@192.168.1.46:57935;lr>", strlen("<sip:34020000001310000051@192.168.1.46:57935;lr>"), NULL));
	 
	 pj_list_push_back(&routeSet, route);
	 pjsip_dlg_set_route_set(dlg,&routeSet);*/

	/* Optionally record-route */
	/*char uribuf[128];
	pj_str_t uri;
	pj_str_t H_RR = pj_str("Route");//{ "Record-Route", 12 };
	pjsip_generic_string_hdr *rr;

	pj_ansi_snprintf(uribuf, sizeof(uribuf), "<sip:%.*s:%d;lr>",
					(int)strlen("192.168.1.46"),
					"192.168.1.46",
					5060);

	uri = pj_str(uribuf);
	rr = pjsip_generic_string_hdr_create(tdata->pool,
		&H_RR, &uri);
	pjsip_msg_insert_first_hdr(tdata->msg, (pjsip_hdr*)rr);*/
}

pj_status_t	PjsipProxy::RegisterModule( on_rx_Request	OnRequest,
									   on_rx_Response OnResponse )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED);

	_Module.name = pj_str("ModStatefuProxy");
	_Module.id = -1;
	_Module.priority = PJSIP_MOD_PRIORITY_UA_PROXY_LAYER;
	_Module.on_rx_request = OnRequest;
	_Module.on_rx_response = OnResponse;
	
	pj_status_t status;
	status = pjsip_endpt_register_module( _pEndpt, &_Module);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, 1);
	
	return status;
}

pj_status_t	PjsipProxy::UnRegisterModule( void )
{
	if(_pEndpt != NULL)
	{
		return pjsip_endpt_unregister_module (_pEndpt, &_Module);
	}
	return PJ_SUCCESS;
}

} // namespace juli
