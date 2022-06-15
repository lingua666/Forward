
#include "../include/PjsipUA.h"
#include "../include/PjsipInvite.h"

namespace juli {

PjsipUserAgent::PjsipUserAgent( int iPort, on_rx_Request OnRequest,
							   on_rx_Response Response )
				:_isBind(PJ_FALSE)
				,_pEndpt(NULL)
				,_pPool(NULL)
				,_iPort(iPort)
{
	pj_bzero(&_Module,sizeof(pjsip_module));
	Init(iPort,OnRequest,Response);
}

PjsipUserAgent::PjsipUserAgent( pjsip_endpoint*	_pEndpt )
				:_isBind(PJ_TRUE)
				,_pEndpt(_pEndpt)
				,_pPool(NULL)
				,_iPort(0)

{
	pj_bzero(&_Module,sizeof(pjsip_module));
	if(_pPool == NULL)
	{
		_pPool = pjsip_endpt_create_pool(_pEndpt,"PjsipEndptPool",1024 * 1024, 1024 * 1024);
		if(_pPool == NULL)
		{
			OUTPUT_ERROR_LOG("pjsip_endpt_create_pool is Failed!!");
		}
	}
}

PjsipUserAgent::~PjsipUserAgent( void )
{
	Release();
}

pj_bool_t	PjsipUserAgent::Init( int iPort, on_rx_Request OnRequest,
								 on_rx_Response Response )
{
	pj_status_t status;
	_pEndpt = PjsipInit::GetInstance()->GetEndpt();
	if(_pEndpt == NULL)
	{
		return PJ_FALSE;
	}

	if(_pPool == NULL)
	{
		_pPool = pjsip_endpt_create_pool(_pEndpt,"PjsipEndptPool",1024 * 1024, 1024 * 1024);
		PJ_ASSERT_RETURN(_pPool != NULL, PJ_FALSE);
	}

	status = RegisterModule(OnRequest,Response);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	pj_sockaddr_in tm_addr = {0};
	tm_addr.sin_family = PJ_AF_INET;
	tm_addr.sin_port = pj_htons(iPort);

	status = pjsip_udp_transport_start(_pEndpt,&tm_addr,NULL,1,NULL);
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJ_FALSE);

	return PJ_TRUE;
}

void		PjsipUserAgent::Release( void )
{
	if(_pEndpt != NULL)
	{
		if(!_isBind)
		{
			pj_status_t status = PJ_SUCCESS;
			status = UnRegisterModule();
			if(_pPool == NULL)
			{
				OUTPUT_ERROR_LOG("UnRegisterModule is Failed!!");
			}
			
		}

		if(_pPool != NULL)
		{
			pjsip_endpt_release_pool(_pEndpt,_pPool);
			_pPool = NULL;
		}
		_pEndpt = NULL;
	}
}

void		PjsipUserAgent::FlushEvents( pj_uint32_t puDuration )
{
	if(_pEndpt == NULL)
		return ;

	pj_time_val stop_time;

	pj_gettimeofday(&stop_time);
	stop_time.msec += puDuration;
	pj_time_val_normalize(&stop_time);

	/* Process all events for the specified duration. */
	for (;;) {
		pj_time_val timeout = {0, 1}, now;

		pjsip_endpt_handle_events(_pEndpt, &timeout);

		pj_gettimeofday(&now);
		if (PJ_TIME_VAL_GTE(now, stop_time))
			break;
	}
}

pj_status_t		PjsipUserAgent::SendEndptRequest( const pjsip_method *pMethod, const pj_str_t psSrvUri,
												 const pj_str_t psFromUri, const pj_str_t psToUrl,
												 const pj_str_t psContactUri, pjsip_msg_body* pMsgBody,
												 const char* szUseragent )
{
	if(_pEndpt == NULL || _pPool == NULL || pMethod == NULL || pMsgBody == NULL)
		return PJSIP_ENOTINITIALIZED;

	pjsip_tx_data *tdata = NULL;
	pj_status_t status = pjsip_endpt_create_request(_pEndpt, pMethod, &psSrvUri, 
		&psFromUri, &psToUrl,
		&psContactUri, NULL, -1, NULL, &tdata);
	tdata->msg->body = pMsgBody;
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(szUseragent,_pPool,tdata->msg)
	return SendEndptRequestStateless(tdata);
}

pj_status_t		PjsipUserAgent::SendEndptRequestStateless( pjsip_tx_data *tdata, const char* useragent )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,tdata->msg)
	pj_status_t status = pjsip_endpt_send_request_stateless( _pEndpt, tdata, this, NULL);
	return PJ_SUCCESS;
}

pj_status_t		PjsipUserAgent::TransmitEndptRequest( pjsip_rx_data *rdata, const pjsip_uri *uri,
													const char* c_HostIp, int iPort )
{
	pjsip_tx_data *tdata = NULL;
	pj_status_t status = CreateEndptRequest_fwd(rdata,uri,&tdata);
	pjsip_via_hdr* pVia = (pjsip_via_hdr*)pjsip_msg_find_hdr(tdata->msg,PJSIP_H_VIA,NULL);
	std::string	sIP;
	PjsipInit::GetHostIP(sIP);
	pVia->sent_by.host = pj_str(const_cast<char*>(c_HostIp));
	pVia->sent_by.port = iPort;

	rdata->tp_info.transport->local_name.host = pj_str(const_cast<char*>(c_HostIp));
	rdata->tp_info.transport->local_name.port = iPort;

	OUTPUT_LOG(3,"TransmitEndptRequest pVia->sent_by.host:%s\r\n",c_HostIp);
	
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	status = SendEndptRequestStateless(tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	return PJ_SUCCESS;
}

pj_status_t		PjsipUserAgent::CreateEndptRequest_fwd( pjsip_rx_data *rdata, const pjsip_uri *uri,
													   pjsip_tx_data **tdata,
													   const char* useragent )
{
	pj_status_t status = pjsip_endpt_create_request_fwd(_pEndpt,rdata,uri,NULL,NULL,tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,(*tdata)->msg)

	return PJ_SUCCESS;
}

pj_status_t		PjsipUserAgent::SendAuthSrvResponse( pjsip_rx_data *rdata, pjsip_auth_srv* auth_srv, const char* useragent )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED);
	pjsip_tx_data *tdata;

	//创建一个发送响应
	pj_status_t status = pjsip_endpt_create_response(_pEndpt,rdata,
		PJSIP_SC_UNAUTHORIZED,NULL,&tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	//服务器端增加用户鉴权请求字段
	status = pjsip_auth_srv_challenge(auth_srv,NULL,NULL,NULL,0,tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	pjsip_response_addr res_addr = {0};
	//获取响应地址（UAC/UAS地址）
	status = pjsip_get_response_addr(_pPool,rdata,&res_addr);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,tdata->msg)

	//发送响应
	return pjsip_endpt_send_response(_pEndpt,&res_addr,tdata,NULL,NULL);
}

pj_status_t		PjsipUserAgent::SendEndptResponseStateless( const pjsip_rx_data *rdata,
														   int st_code,
														   const char* useragent,
														   const pj_str_t *st_text,
														   const pjsip_hdr *hdr_list,
														   const pjsip_msg_body *body )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED);

	if(useragent != NULL)
	{	//Add User-Agent
		const pj_str_t USER_AGENT = { "User-Agent", 10};
		pj_str_t	user_agent = pj_str(const_cast<char*>(useragent));
		pjsip_hdr *hdr = (pjsip_hdr*) pjsip_generic_string_hdr_create(rdata->tp_info.pool,
			&USER_AGENT,
			&user_agent);

		if(hdr_list == NULL)
		{
			pjsip_hdr res_hdr_list;
			pj_list_init(&res_hdr_list);
			pj_list_push_back(&res_hdr_list, hdr);
			return 		pjsip_endpt_respond_stateless(_pEndpt, const_cast<pjsip_rx_data *>(rdata),
				st_code, st_text,
				&res_hdr_list, body);
		}
		else
			pj_list_push_back(const_cast<pjsip_hdr*>(hdr_list),hdr);
	}
		
	return 		pjsip_endpt_respond_stateless(_pEndpt, const_cast<pjsip_rx_data *>(rdata),
		st_code, st_text,
		hdr_list, body);
}

pj_status_t		PjsipUserAgent::SendEndptResponse2( pjsip_rx_data *rdata,
												   pjsip_tx_data *tdata,
												   const char* useragent )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED);
	return pjsip_endpt_send_response2(_pEndpt,rdata,tdata,this,NULL);
}

pj_status_t		PjsipUserAgent::SendEndptResponse3(	pjsip_response_addr *res_addr, pjsip_tx_data *tdata,
													const char* useragent )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED );
	
	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,tdata->msg)
	return pjsip_endpt_send_response(_pEndpt,res_addr,tdata,this,NULL);
}

pj_status_t		PjsipUserAgent::TransmitEndptResponse( pjsip_rx_data *rdata )
{
	pjsip_tx_data *tdata = NULL;
	pjsip_via_hdr *hvia = NULL;
	pjsip_response_addr res_addr;

	pj_status_t status = CreateEndptResponse_fwd(rdata,&tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	/* Get topmost Via header */
    hvia = (pjsip_via_hdr*) pjsip_msg_find_hdr(tdata->msg, PJSIP_H_VIA, NULL);
    if (hvia == NULL) {
	/* Invalid response! Just drop it */
	pjsip_tx_data_dec_ref(tdata);
	return PJ_TRUE;
    }

    /* Calculate the address to forward the response */
    pj_bzero(&res_addr, sizeof(res_addr));
    res_addr.dst_host.type = PJSIP_TRANSPORT_UDP;
    res_addr.dst_host.flag = pjsip_transport_get_flag_from_type(PJSIP_TRANSPORT_UDP);

    /* Destination address is Via's received param */
    res_addr.dst_host.addr.host = hvia->recvd_param;
    if (res_addr.dst_host.addr.host.slen == 0) {
	/* Someone has messed up our Via header! */
	res_addr.dst_host.addr.host = hvia->sent_by.host;
    }

    /* Destination port is the rpot */
    if (hvia->rport_param != 0 && hvia->rport_param != -1)
	res_addr.dst_host.addr.port = hvia->rport_param;

    if (res_addr.dst_host.addr.port == 0) {
	/* Ugh, original sender didn't put rport!
	 * At best, can only send the response to the port in Via.
	 */
	res_addr.dst_host.addr.port = hvia->sent_by.port;
    }

    /* Forward response */
    status = pjsip_endpt_send_response(_pEndpt, &res_addr, tdata, 
				       NULL, NULL);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	return PJ_TRUE;
}

pj_status_t		PjsipUserAgent::CreateEndptResponse_fwd( pjsip_rx_data *rdata, 
													  pjsip_tx_data **p_tdata,
													  const char* useragent )
{
	pj_status_t status = pjsip_endpt_create_response_fwd( _pEndpt,rdata,0,p_tdata);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);

	//Add User-Agent
	MACRO_ADD_USER_AGENT(useragent,_pPool,(*p_tdata)->msg)
	return PJ_SUCCESS;
}

pj_status_t		PjsipUserAgent::SendEndptACK( const pjsip_tx_data *tdata,
											const pjsip_rx_data *rdata,
											const char* useragent )
{
	return PJ_SUCCESS;
}

pj_status_t		PjsipUserAgent::SendEndptCancel( const pjsip_tx_data *tdata,
												const pjsip_rx_data *rdata,
												const char* useragent )
{
	return PJ_SUCCESS;
}

pj_status_t	PjsipUserAgent::RegisterModule( on_rx_Request OnRequest,
										   on_rx_Response Response )
{
	PJ_ASSERT_RETURN(_pEndpt != NULL, PJSIP_ENOTINITIALIZED);

	_Module.name = pj_str("ModEndpt");
	_Module.id = -1;
	_Module.priority = PJSIP_MOD_PRIORITY_APPLICATION;
	_Module.on_rx_request = OnRequest;
	_Module.on_rx_response = Response;

	pj_status_t status;
	status = pjsip_endpt_register_module( _pEndpt, &_Module );
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	return status;
}

pj_status_t	PjsipUserAgent::UnRegisterModule( void )
{
	if(_pEndpt != NULL)
	{
		return pjsip_endpt_unregister_module (_pEndpt, &_Module);
	}
	return PJ_SUCCESS;
}

} // namespace juli
