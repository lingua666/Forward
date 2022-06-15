
//#include <pjmedia.h>
#include <pjsip-ua/sip_100rel.h>
#include <pjmedia/endpoint.h>
#include <pjmedia/sdp.h>

#include "../../common/include/PjsipUA.h"
#include "../../common/include/PjsipRegisters.h"
#include "../include/UserManagement.h"
#include "../include/InviteProc.h"
#include "../include/MediaStreamSesion.h"


namespace juli {

MediaStreamSession::MediaStreamSession( std::string &sIP, int iPort, int iMode )
					:_pSrcSDP(NULL)
					,_iLocalPort(0)
					,_isTimeout(false)
					,_sIP(sIP)
					,_iPort(iPort)
					,_iMode(iMode)
					,_iLookerNumber(0)
					,_iStatus(STATUS_NULL)
{
	memset(&_SrcInvSession,0,sizeof(InvSessionInfo));
	memset(&_TargetInvSession,0,sizeof(InvSessionInfo));
	memset(&_SendMediaInvSession,0,sizeof(InvSessionInfo));
	memset(&_RecvMediaInvSession,0,sizeof(InvSessionInfo));
	memset(_szToUri,0,sizeof(_szToUri));
}

MediaStreamSession::~MediaStreamSession( void )
{
#ifdef USE_IOSERVRICEIOEX
	MediaStreamSession* pMediaStreamSession = NULL;
	if(InviteProc::GetIntance()->GetMediaSenderMap()->IsExistKey2(_szToUri,pMediaStreamSession))
	{			
		if( this != pMediaStreamSession)
		{
			pMediaStreamSession->LookerRefDec();
			if(pMediaStreamSession->GetLookerRef() <= 0 &&
				pMediaStreamSession->GetStatus() == STATUS_STOP_PENDING)
				pMediaStreamSession->Stop();
		}
	}

	if(pMediaStreamSession == this)
		InviteProc::GetIntance()->GetMediaSenderMap()->eraseKey(_szToUri);

#endif

	//test
	OUTPUT_LOG(3,"MediaStreamSession is Free\r\n");
	//test
	
}

void	MediaStreamSession::Invite_on_rx_Request( pjsip_rx_data *rdata )
{
	pjsip_transport *uas_tsx = rdata->tp_info.transport;
	pjsip_dialog* pDlg = pjsip_rdata_get_dlg(rdata);
	if(pDlg == NULL)
	{
		OUTPUT_LOG(3,"rdata Not Found in dialog instance\r\n");
		return ;
	}
	
	pjsip_inv_session* pInv = pjsip_dlg_get_inv_session(pDlg);
	if(pInv == NULL)
	{
		OUTPUT_LOG(3,"rdata Not Found in inv_session instance\r\n");
		return ;
	}

	pj_status_t status;

	switch(rdata->msg_info.msg->line.req.method.id)
	{
	case PJSIP_OPTIONS_METHOD:
	case PJSIP_OTHER_METHOD:
		{
			if(pj_strcmp2(&rdata->msg_info.msg->line.req.method.name,"MESSAGE") == 0 || 
				pj_strcmp2(&rdata->msg_info.msg->line.req.method.name,"INFO") == 0)
			{
				PjsipUserAgent	ua(uas_tsx->endpt);
				pjsip_transaction *tsx = pjsip_rdata_get_tsx(rdata);
				pjsip_tx_data *tdata = NULL;
				pjsip_tsx_recv_msg(tsx,rdata);
				status = pjsip_dlg_respond(pInv->dlg,rdata,PJSIP_SC_OK,NULL,NULL,NULL);
				
				if(pInv == _SrcInvSession.pInv)
				{
					status = pjsip_dlg_create_request(_TargetInvSession.pInv->dlg,&rdata->msg_info.msg->line.req.method,rdata->msg_info.cseq->cseq,&tdata);
					if(PJ_SUCCESS != status)
					{
						ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
					}

					tdata->msg->body = rdata->msg_info.msg->body;
					status = pjsip_dlg_send_request(_TargetInvSession.pInv->dlg,tdata, -1, NULL);
					if(PJ_SUCCESS != status)
					{
						ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
					}
				}
				else if(pInv == _TargetInvSession.pInv)
				{
					status = pjsip_dlg_create_request(_SrcInvSession.pInv->dlg,&rdata->msg_info.msg->line.req.method,rdata->msg_info.cseq->cseq,&tdata);
					if(PJ_SUCCESS != status)
					{
						ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
					}
					tdata->msg->body = rdata->msg_info.msg->body;
					status = pjsip_dlg_send_request(_SrcInvSession.pInv->dlg,tdata, -1, NULL);					
					if(PJ_SUCCESS != status)
					{
						ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
					}
				}
			}
		}
		break;
	}
}

void	MediaStreamSession::Invite_on_rx_Respond( pjsip_rx_data *rdata )
{
	OUTPUT_LOG(3,"Invite_on_rx_Respond");

	pjsip_transport *uas_tsx = rdata->tp_info.transport;
	pjsip_dialog* pDlg = pjsip_rdata_get_dlg(rdata);
	if(pDlg == NULL)
	{
		OUTPUT_LOG(3,"rdata Not Found in dialog instance\r\n");
		return ;
	}

	pjsip_inv_session* pInv = pjsip_dlg_get_inv_session(pDlg);
	if(pInv == NULL)
	{
		OUTPUT_LOG(3,"rdata Not Found in inv_session instance\r\n");
		return ;
	}

	pj_status_t status;

	switch(rdata->msg_info.msg->line.req.method.id)
	{
	case PJSIP_OPTIONS_METHOD:
	case PJSIP_OTHER_METHOD:
		{
			if(pj_strcmp2(&rdata->msg_info.msg->line.req.method.name,"MESSAGE") == 0 || 
				pj_strcmp2(&rdata->msg_info.msg->line.req.method.name,"INFO") == 0)
			{
				PjsipUserAgent	ua(uas_tsx->endpt);
				//pjsip_tx_data *tdata = NULL;
				//pjsip_tsx_recv_msg(tsx,rdata);
				//status = pjsip_dlg_respond(pInv->dlg,rdata,PJSIP_SC_TRYING,NULL,NULL,NULL);

				if(pInv == _SrcInvSession.pInv)
				{
					status = pjsip_dlg_respond(_TargetInvSession.pInv->dlg,rdata,rdata->msg_info.msg->line.status.code,NULL,NULL,NULL);
					if(PJ_SUCCESS != status)
					{
						ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
					}
				}
				else if(pInv == _TargetInvSession.pInv)
				{
					status = pjsip_dlg_respond(_SrcInvSession.pInv->dlg,rdata,rdata->msg_info.msg->line.status.code,NULL,NULL,NULL);
					if(PJ_SUCCESS != status)
					{
						ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
					}
				}
			}
		}
		break;
	}
}

pj_status_t	MediaStreamSession::Invite_on_state_changed( pjsip_inv_session *inv, pjsip_event *e,
														 pjsip_inv_session **p_newinv )
{
	pj_status_t status;
	
	OUTPUT_LOG(3,"Invite_on_state_changed:%d\r\n",inv->state);

	switch(inv->state)
	{
	case PJSIP_INV_STATE_NULL:	    /**< Before INVITE is sent or received  */
		OUTPUT_LOG(3,"PJSIP_INV_STATE_NULL\r\n");
		break;
	case PJSIP_INV_STATE_CALLING:	    /**< After INVITE is sent		    */
		OUTPUT_LOG(3,"PJSIP_INV_STATE_CALLING\r\n");
		break;
	case PJSIP_INV_STATE_INCOMING:	    /**< After INVITE is received.	    */
		OUTPUT_LOG(3,"PJSIP_INV_STATE_INCOMING\r\n");
		break;
	case PJSIP_INV_STATE_EARLY:	    /**< After response with To tag.	    */
		OUTPUT_LOG(3,"PJSIP_INV_STATE_EARLY\r\n");
		break;
	case PJSIP_INV_STATE_CONNECTING:	    /**< After 2xx is sent/received.	    */
		OUTPUT_LOG(3,"PJSIP_INV_STATE_CONNECTING,%d,%d\r\n",e->type,e->body.tsx_state.type);
		{
			if(_SrcInvSession.pInv == inv)
			{
				
			}
			else if(_TargetInvSession.pInv == inv)
			{
				pjsip_rx_data *rdata = e->body.rx_msg.rdata;
				
				pjsip_sip_uri*	pTagUri = (pjsip_sip_uri*)pjsip_uri_get_uri(_TargetInvSession.pInv->dlg->target);
				char szUri[100] = {0};
				strncpy(szUri,pTagUri->user.ptr,pTagUri->user.slen);
				const stUserAgentAddress* pSAddress = UserManagement::GetIntance()->IsExist(szUri);
				if(pSAddress != NULL)
				{
					pTagUri->host = pj_str(const_cast<char*>(pSAddress->sIP.c_str()));
					pTagUri->port = pSAddress->iPort;
				}

				PjsipInvite Invite(_TargetInvSession.pInv);
				status = Invite.SendAck(rdata->msg_info.cseq->cseq);

				pjsip_rdata_sdp_info* pSDPInfo = pjsip_rdata_get_sdp_info(rdata);
				if(_RecvMediaInvSession.pInv != NULL)
				{
					PjsipInvite Invite(_RecvMediaInvSession.pInv);
					pjsip_cseq_hdr* cseq = (pjsip_cseq_hdr*)pjsip_msg_find_hdr(_RecvMediaInvSession.pInv->invite_req->msg, PJSIP_H_CSEQ, NULL);
					if(cseq != NULL)
					{
						//test
						/*pjsip_sip_uri*	pTagUri = (pjsip_sip_uri*)pjsip_uri_get_uri(_RecvMediaInvSession.pInv->dlg->target);
						char szUri[100] = {0};
						strncpy(szUri,pTagUri->user.ptr,pTagUri->user.slen);
						const stUserAgentAddress* pSAddress = UserManagement::GetIntance()->IsExist(szUri);
						if(pSAddress != NULL)
						{
							pTagUri->host = pj_str(const_cast<char*>(pSAddress->sIP.c_str()));
							pTagUri->port = pSAddress->iPort;
						}*/

						//test
						status = Invite.SendAck(cseq->cseq,pSDPInfo->sdp);
					}
					else
					{
						OUTPUT_LOG(3,"CSEQ Not Found!!");
					}
				}
				
				OUTPUT_LOG(3,"_TargetInvSession \r\n");

				if(_TargetInvSession.iStatus == STATUS_WAIT_OK)
				{
					_TargetInvSession.iStatus = STATUS_WAIT_BYE;
					pjsip_sip_uri*	pFromUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.from->uri);
					pjsip_sip_uri*	pToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);
					/*std::string sLocalIP;
					PjsipInit::GetInstance()->GetHostIP(sLocalIP);*/

					const stUserAgentAddress* pUaAddr = UserManagement::GetIntance()->GetMediaServer();
					if(pUaAddr == NULL)
					{
						OUTPUT_LOG(3,"Media Server Not Found!!\r\n");
						return	PJSIP_ERRNO_START;
					}
			
					char szCallID[100] = {0};
					strncpy(szCallID,_RecvMediaInvSession.pInv->dlg->call_id->id.ptr,
							_RecvMediaInvSession.pInv->dlg->call_id->id.slen);
					status = SendInviteRequestToTarget(pUaAddr->sIP.c_str(),pUaAddr->iPort,pFromUri,pToUri,_pSrcSDP,&_SendMediaInvSession,szCallID);
					*p_newinv = _SendMediaInvSession.pInv;
				}
			}
			else if(_SendMediaInvSession.pInv == inv)
			{
				PjsipInvite SendInvite(_SendMediaInvSession.pInv);
				pjsip_cseq_hdr* cseq = (pjsip_cseq_hdr*)pjsip_msg_find_hdr(_SendMediaInvSession.pInv->invite_req->msg, PJSIP_H_CSEQ, NULL);
				if(cseq != NULL)
				{
					//test
					pjsip_sip_uri*	pTagUri = (pjsip_sip_uri*)pjsip_uri_get_uri(_SendMediaInvSession.pInv->dlg->target);
					char szUri[100] = {0};
					strncpy(szUri,pTagUri->user.ptr,pTagUri->user.slen);
					const stUserAgentAddress* pSAddress = UserManagement::GetIntance()->IsExist(szUri);
					if(pSAddress != NULL)
					{
						pTagUri->host = pj_str(const_cast<char*>(pSAddress->sIP.c_str()));
						pTagUri->port = pSAddress->iPort;
					}

					//test
					status = SendInvite.SendAck(cseq->cseq,_pSrcSDP);
				}
				else
				{
					OUTPUT_LOG(3,"CSEQ Not Found!!");
				}

				if(_SendMediaInvSession.iStatus == STATUS_WAIT_OK)
				{
					_SendMediaInvSession.iStatus = STATUS_WAIT_BYE;
					pjsip_rx_data *rdata = e->body.rx_msg.rdata;
					pjsip_rdata_sdp_info* pSDPInfo = pjsip_rdata_get_sdp_info(rdata);

					//test
					//pSDPInfo->sdp->conn->addr = pj_str("58.67.136.34");
					//test

					PjsipInvite SrcInvite(_SrcInvSession.pInv);
					status = SrcInvite.SendInvAnswer1(PJSIP_SC_OK,pSDPInfo->sdp);
				}
				
				OUTPUT_LOG(3,"_SendMediaInvSession \r\n");
			}
			else if( _RecvMediaInvSession.pInv == inv && _RecvMediaInvSession.iStatus == STATUS_WAIT_OK)
			{
				_RecvMediaInvSession.iStatus = STATUS_WAIT_BYE;
				pjsip_rx_data *rdata = e->body.rx_msg.rdata;
				pjsip_rdata_sdp_info* pSDPInfo = pjsip_rdata_get_sdp_info(rdata);

				pjsip_sip_uri*	pFromUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.from->uri);
				pjsip_sip_uri*	pToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);
				strncpy(_szToUri,pToUri->user.ptr,pToUri->user.slen);

				if(_iMode == MediaStreamSession::MODE_PLAYBACK)
				{
					pSDPInfo->sdp->name = pj_str("Playback");
					pSDPInfo->sdp->time.start = _pSrcSDP->time.start;
					pSDPInfo->sdp->time.stop = _pSrcSDP->time.stop;
				}
#ifdef	USE_IOSERVRICEIOEX
				else
				{
					if(pj_strcmp2(&pSDPInfo->sdp->name,"PlayExist") == 0)
					{
						OUTPUT_LOG(3,"_RecvMediaInvSession PlayExist\r\n");
						
						pjsip_cseq_hdr* cseq = (pjsip_cseq_hdr*)pjsip_msg_find_hdr(_RecvMediaInvSession.pInv->invite_req->msg, PJSIP_H_CSEQ, NULL);
						if(cseq != NULL)
						{
							PjsipInvite Invite(_RecvMediaInvSession.pInv);
							status = pjmedia_sdp_neg_create_w_remote_offer(_RecvMediaInvSession.pInv->pool, NULL,
								_pSrcSDP,
								&_RecvMediaInvSession.pInv->neg);

							status = Invite.SendAck(cseq->cseq,_pSrcSDP);

							//RecvMediaInvSession.pInv->neg->state
						}

						pjsip_rdata_sdp_info* pSDPInfo = pjsip_rdata_get_sdp_info(rdata);
						PjsipInvite SrcInvite(_SrcInvSession.pInv);
						status = SrcInvite.SendInvAnswer1(PJSIP_SC_OK,pSDPInfo->sdp);

						MediaStreamSession* pMediaStreamSession = NULL;
						if(InviteProc::GetIntance()->GetMediaSenderMap()->IsExistKey2(_szToUri,pMediaStreamSession))
						{
							pMediaStreamSession->LookerRefInc();
						}

						/*PjsipInvite SrcInvite(_SrcInvSession.pInv);
						status = SrcInvite.SendInvAnswer1(PJSIP_SC_OK,pSDPInfo->sdp);*/
						return PJ_SUCCESS;
					}
				}
#endif
				//test
				//pSDPInfo->sdp->conn->addr = pj_str("58.67.136.34");
				//test

				status = SendInviteRequestToTarget(NULL,0,pFromUri,pToUri,pSDPInfo->sdp,&_TargetInvSession);
				*p_newinv = _TargetInvSession.pInv;
				
				OUTPUT_LOG(3,"_RecvMediaInvSession \r\n");
			}
		}
		break;
	case PJSIP_INV_STATE_CONFIRMED:	    /**< After ACK is sent/received.	    */
		OUTPUT_LOG(3,"Proxy PJSIP_INV_STATE_CONFIRMED,%d,%d\r\n",e->type,e->body.tsx_state.type);
		break;
	case PJSIP_INV_STATE_DISCONNECTED:   /**< Session is terminated.		    */
		{
			InviteDisconnect(inv,e);
		}
		break;
	}

	return PJ_SUCCESS;
}

pj_status_t	MediaStreamSession::Invite_on_media_update( pjsip_inv_session *inv_ses, pj_status_t status )
{
	const pjmedia_sdp_session *local_sdp;
    const pjmedia_sdp_session *remote_sdp;

    if (status != PJ_SUCCESS) 
	{
		PjsipInvite	Inv(inv_ses);
		Inv.SendEndSession();
		/* Here we should disconnect call if we're not in the middle 
		* of initializing an UAS dialog and if this is not a re-INVITE.
		*/
		return status;
    }

	if(inv_ses == _TargetInvSession.pInv)
	{
		OUTPUT_LOG(3,"Invite_on_media_update _TargetInvSession\r\n");
	}
	else if(inv_ses == _RecvMediaInvSession.pInv)
	{
		OUTPUT_LOG(3,"Invite_on_media_update _RecvMediaInvSession\r\n");
	}
	else if(inv_ses == _SrcInvSession.pInv)
	{
		OUTPUT_LOG(3,"Invite_on_media_update _SrcInvSession\r\n");
	}
	else if(inv_ses == _SendMediaInvSession.pInv)
	{
		OUTPUT_LOG(3,"Invite_on_media_update _SendMediaInvSession\r\n");
		return PJ_SUCCESS;
	}

	status = pjmedia_sdp_neg_get_active_local(inv_ses->neg, &local_sdp);
	status = pjmedia_sdp_neg_get_active_remote(inv_ses->neg, &remote_sdp);

	OUTPUT_LOG(3,"local_sdp->media[0]->desc.port:%d\r\n",local_sdp->media[0]->desc.port);
	OUTPUT_LOG(3,"remote_sdp->media[0]->desc.port:%d\r\n",remote_sdp->media[0]->desc.port);

	if(inv_ses == _SendMediaInvSession.pInv)
	{
		char szRemoteIP[50] = {0};
		strncpy(szRemoteIP,local_sdp->conn->addr.ptr,local_sdp->conn->addr.slen);
		//IOServrice::GetInstance()->AddEndpointToOnLine(local_sdp->media[0]->desc.port,szRemoteIP,remote_sdp->media[0]->desc.port);
	}

	return	PJ_ETOOSMALL;
}

pj_status_t	MediaStreamSession::InviteResponse( pjsip_rx_data *rdata )
{
	pj_status_t		status;
	pjsip_tx_data	*tdata = NULL;

	pjsip_sip_uri*	pFromUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.from->uri);
	PJ_ASSERT_RETURN(pFromUri != NULL, PJSIP_ENOTINITIALIZED);

	PjsipUserAgent	ua(rdata->tp_info.transport->endpt);
	char szSrcName[100] = {0};
	strncpy(szSrcName,pFromUri->user.ptr,pFromUri->user.slen);

	const stUserAgentAddress* pSUaSrcAddress = UserManagement::GetIntance()->IsExist(szSrcName);
	if(pSUaSrcAddress != NULL)
	{
		//Find target 
		pjsip_sip_uri*	ToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);
		char szDstName[100] = {0};
		strncpy(szDstName,ToUri->user.ptr,ToUri->user.slen);

		const stUserAgentAddress* pSUaDstAddress = UserManagement::GetIntance()->IsExist(szDstName);
		if(pSUaDstAddress != NULL)
		{//Exist,Send TRYING message
			if(_SrcInvSession.pInv != NULL)
			{
				OUTPUT_LOG(3,"_pSrcInv Is Not NULL");
			}

			status = SendInviteRespond(rdata,PJSIP_SC_TRYING,NULL,&_SrcInvSession);
			pjsip_rdata_sdp_info* pSDPInfo = pjsip_rdata_get_sdp_info(rdata);
			_pSrcSDP = pjmedia_sdp_session_clone( _SrcInvSession.pInv->dlg->pool,
												pSDPInfo->sdp );
			/*std::string sLocalIP;
			PjsipInit::GetInstance()->GetHostIP(sLocalIP);*/
			const stUserAgentAddress* pUaAddr = UserManagement::GetIntance()->GetMediaServer();
			if(pUaAddr == NULL)
			{
				OUTPUT_LOG(3,"Media Server Not Found!!\r\n");
				return	PJSIP_ERRNO_START;
			}

			status = SendInviteRequestToTarget(pUaAddr->sIP.c_str()/*sLocalIP.c_str()*/,pUaAddr->iPort,pFromUri,ToUri,NULL,&_RecvMediaInvSession);
		}
		else
		{//Not exist
			ua.SendEndptResponseStateless(rdata,PJSIP_SC_NOT_FOUND);
		}
	}
	else
	{//不是本区域设备
		
	}

	return PJ_SUCCESS;
}

pj_status_t	MediaStreamSession::SendInviteRequestToTarget( const char* szTargetIP, int iTargetPort,
														  pjsip_sip_uri* pFromUri,
														  pjsip_sip_uri*	pToUri,
														  const pjmedia_sdp_session *local_sdp,
														  InvSessionInfo* pInvSessionInfo,
														  const char* szSubject )
{
	char szSrcUriBuf[100] = {0};
	char szDstUriBuf[100] = {0};
	char szSrcName[100] = {0};
	strncpy(szSrcName,pFromUri->user.ptr,pFromUri->user.slen);
	sprintf(szSrcUriBuf,"<sip:%s@%s:%d>",szSrcName,_sIP.c_str(),_iPort);

	pj_str_t src_uri = pj_str(const_cast<char*>(szSrcUriBuf));
	pj_str_t dst_uri = pj_str(const_cast<char*>(szDstUriBuf));

	char szTargetBuf[100] = {0};
	char szFromBuf[100] = {0};
	char szToBuf[100] = {0};
	if(szTargetIP == NULL)
	{
		char szToUser[100] = {0};
		strncpy(szToUser,pToUri->user.ptr,pToUri->user.slen);
		const stUserAgentAddress* pSUaToAddress = UserManagement::GetIntance()->IsExist(szToUser);
		if(pSUaToAddress != NULL)
		{
			sprintf(szTargetBuf,"<sip:%s@%s:%d>",szToUser,pSUaToAddress->sIP.c_str(),pSUaToAddress->iPort);
		}
		else
		{//Not exist
			return PJSIP_SC_NOT_FOUND;
		}
	}
	else
	{
		const stUserAgentAddress* pUaAddr = UserManagement::GetIntance()->GetMediaServer();
		if(pUaAddr == NULL)
		{
			OUTPUT_LOG(3,"Media Server Not Found!!\r\n");
			return	PJSIP_ERRNO_START;
		}
		sprintf(szTargetBuf,"<sip:%s@%s:%d>",pUaAddr->sSpiUri.c_str(),szTargetIP,iTargetPort);
	}

	pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR,pFromUri,szFromBuf,sizeof(szFromBuf));
	pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR,pToUri,szToBuf,sizeof(szToBuf));

	pj_str_t target_uri = pj_str(szTargetBuf);
	pj_str_t from_uri = pj_str(szFromBuf);
	pj_str_t to_uri = pj_str(szToBuf);

	//Send Invite Message to MediaServer (No With SDP)
	pjsip_dialog	*pDlg = NULL;
	pj_status_t status = PjsipInvite::CreateUacDialog(&from_uri,&src_uri,&to_uri, &target_uri,&pDlg);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	status = PjsipInvite::CreateUacInv(pDlg,local_sdp,&pInvSessionInfo->pInv);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, status);
	if(_iMode == MODE_PLAYBACK)
	{
		status = pjsip_dlg_add_usage(pDlg, InviteProc::GetModule(), NULL);
		if (status != PJ_SUCCESS) {
			OUTPUT_LOG(3,"pjsip_dlg_add_usage Is Failed!!\r\n");
			return status;
		}
	}

	PjsipInvite	InvUac(pInvSessionInfo->pInv);
	status = InvUac.SendInvRequest(szSubject);
	if(status == PJ_SUCCESS)
		pInvSessionInfo->iStatus = STATUS_WAIT_OK;
	return status;
}

pj_status_t	MediaStreamSession::SendInviteRespond( pjsip_rx_data *rdata, int st_code,
												const pjmedia_sdp_session *local_sdp, 
												InvSessionInfo* pInvSessionInfo )
{
	/*
	* Create UAS dialog.
	*/

	char szContact[100] = {0};
	const pjsip_uri* pUri = PjsipInvite::GetReceiverUri(rdata->msg_info.msg);
	
	pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR,pUri,szContact,sizeof(szContact));
	pj_str_t psContact = pj_str(szContact);

	pjsip_dialog	*pDlg = NULL;
	pj_status_t status = PjsipInvite::CreateUasDialog(rdata,psContact,&pDlg);
	if (status != PJ_SUCCESS) 
	{
		PjsipUserAgent	ua(rdata->tp_info.transport->endpt);
		ua.SendEndptResponseStateless(rdata,PJSIP_SC_INTERNAL_SERVER_ERROR);
		return PJ_TRUE;
	}

	/* 
	* Create invite session, and pass both the UAS dialog and the SDP
	* capability to the session.
	*/
	status = PjsipInvite::CreateUasInv(pDlg,rdata,local_sdp,&pInvSessionInfo->pInv);
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, pjsip_dlg_terminate(pDlg) && PJ_TRUE);

	if(_iMode == MODE_PLAYBACK)
	{
		status = pjsip_dlg_add_usage(pDlg, InviteProc::GetModule(), NULL);
		if (status != PJ_SUCCESS) {
			OUTPUT_LOG(3,"pjsip_dlg_add_usage Is Failed!!\r\n");
			return status;
		}
	}

	/*
	* Initially send 100 response.
	*
	* The very first response to an INVITE must be created with
	* pjsip_inv_initial_answer(). Subsequent responses to the same
	* transaction MUST use pjsip_inv_answer().
	*/

	PjsipInvite	UasInv(pInvSessionInfo->pInv);
	return UasInv.SendInvAnswer2(rdata,st_code);
}

pj_status_t	MediaStreamSession::OnSendAck( pjsip_inv_session *inv, pjsip_rx_data *rdata )
{
	if(_SrcInvSession.pInv == inv)
	{

	}
	else if(_RecvMediaInvSession.pInv == inv)
	{

	}
	else if(_TargetInvSession.pInv == inv)
	{

	}
	else if(_SendMediaInvSession.pInv == inv)
	{

	}
	return PJ_SUCCESS;
}

pj_status_t	MediaStreamSession::Stop( void )
{
	pj_status_t status;
	if(_TargetInvSession.pInv != NULL)
	{
		PjsipInvite	Bye(_TargetInvSession.pInv);
		status = Bye.SendEndSession(PJSIP_SC_OK);
		_TargetInvSession.iStatus = STATUS_BYE;
		if(status != PJ_SUCCESS)
		{
			status = Bye.SendTerminateSession(PJSIP_SC_INTERNAL_SERVER_ERROR);
			_TargetInvSession.pInv = NULL;
		}
	}

	return status;
}

void	MediaStreamSession::InviteDisconnect( pjsip_inv_session *inv, pjsip_event *e )
{
	pj_status_t status;
	_iStatus = STATUS_STOP_PENDING;
start:
	if(inv == _SrcInvSession.pInv)
	{
		_SrcInvSession.iStatus = STATUS_BYE;
		_SrcInvSession.pInv = NULL;

		if(_RecvMediaInvSession.pInv != NULL)
		{
			goto _RecvMediaInvSessiongoto;
		}
		else if(_TargetInvSession.pInv != NULL && GetLookerRef() <= 0)
		{
			goto _TargetInvSessiongoto;
		}
		else if(_SendMediaInvSession.pInv != NULL && GetLookerRef() <= 0)
		{
			goto _SendMediaInvSessiongoto;
		}
		else 
			goto end;
	}
	else if(inv == _RecvMediaInvSession.pInv)
	{
		_RecvMediaInvSession.iStatus = STATUS_BYE;
		_RecvMediaInvSession.pInv = NULL;

		if(_TargetInvSession.pInv != NULL && GetLookerRef() <= 0)
		{
			goto _TargetInvSessiongoto;
		}
		else if(_SendMediaInvSession.pInv != NULL && GetLookerRef() <= 0)
		{
			goto _SendMediaInvSessiongoto;
		}
		else if(_SrcInvSession.pInv != NULL && GetLookerRef() <= 0)
		{
			goto _SrcInvSessiongoto;
		}
		else 
			goto end;
	}
	else if(inv == _TargetInvSession.pInv)
	{
		_TargetInvSession.iStatus = STATUS_BYE;
		_TargetInvSession.pInv = NULL;

		if(_SendMediaInvSession.pInv != NULL)
		{
			goto _SendMediaInvSessiongoto;
		}
		else if(_RecvMediaInvSession.pInv != NULL)
		{
			goto _RecvMediaInvSessiongoto;
		}
		else if(_SrcInvSession.pInv != NULL)
		{
			goto _SrcInvSessiongoto;
		}
		else 
			goto end;
	}
	else if(inv == _SendMediaInvSession.pInv)
	{
		_SendMediaInvSession.iStatus = STATUS_BYE;
		_SendMediaInvSession.pInv = NULL;

		if(_TargetInvSession.pInv != NULL)
		{
			goto _TargetInvSessiongoto;
		}
		else if(_RecvMediaInvSession.pInv != NULL)
		{
			goto _RecvMediaInvSessiongoto;
		}
		else if(_SrcInvSession.pInv != NULL)
		{
			goto _SrcInvSessiongoto;
		}
		else 
			goto end;
	}

#define	MACRO_GOTO_DEF(InvSession)\
InvSession##goto:\
	{\
		PjsipInvite	Bye(InvSession.pInv);\
		status = Bye.SendEndSession(inv->cause);\
		InvSession.iStatus = STATUS_BYE;\
		if(status != PJ_SUCCESS)\
		{\
			Bye.SendTerminateSession(PJSIP_SC_INTERNAL_SERVER_ERROR);\
			InvSession.pInv = NULL;\
			goto start;\
		}	\
		else\
			goto end;\
	}

MACRO_GOTO_DEF(_SrcInvSession)
MACRO_GOTO_DEF(_RecvMediaInvSession)
MACRO_GOTO_DEF(_SendMediaInvSession)
MACRO_GOTO_DEF(_TargetInvSession)

#undef MACRO_GOTO_DEF

end:
	PjsipInvite	Terminate(inv);
	Terminate.SendTerminateSession(PJSIP_SC_INTERNAL_SERVER_ERROR);
	
	if(_RecvMediaInvSession.pInv == NULL && _SendMediaInvSession.pInv == NULL
		&& _SrcInvSession.pInv == NULL && _SrcInvSession.pInv == NULL)
	{
		OUTPUT_LOG(3,"InviteDisconnect Is Complete!!\r\n");
	}
}

} // namespace juli
