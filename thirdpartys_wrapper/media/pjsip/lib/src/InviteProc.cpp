
#include <pjmedia/sdp.h>
#include <pjmedia/port.h>
#include <pjsip-ua/sip_inv.h>

#include "../../common/include/PjsipInvite.h"
#include "../../common/include/PjsipUA.h"
#include "../include/UserManagement.h"
#include "../include/MediaStreamSesion.h"
#include "../include/InviteProc.h"

namespace juli {

pjsip_module*	InviteProc::_pModule = NULL;

InviteProc::InviteProc( void )
			:_pEndpt(NULL)
{
	
}

InviteProc::InviteProc( pjsip_endpoint	*pEndpt, std::string &sHostIP, int iHostPort )
			:_pEndpt(pEndpt)
{
	Init(pEndpt,sHostIP,iHostPort);
}

InviteProc::~InviteProc( void )
{
	Release();
}

void InviteProc::Init( pjsip_endpoint	*pEndpt, const std::string &sHostIP, int iHostPort )
{
	_MediaSessionMap.clear();
	_MediaSenderMap.clear();

	pjsip_inv_callback inv_cb;
	pj_bzero(&inv_cb, sizeof(inv_cb));
	inv_cb.on_state_changed = &InviteProc::Invite_on_state_changed;
	inv_cb.on_new_session = &InviteProc::Invite_on_new_session;
	inv_cb.on_media_update = &InviteProc::Invite_on_media_update;
	inv_cb.on_send_ack = &InviteProc::Invite_on_send_ack;
	inv_cb.on_tsx_state_changed = &InviteProc::Invite_on_tsx_state_changed;
	_sHostIP = sHostIP;
	_iHostPort = iHostPort;

	static	InviteRegc	InvRegc(pEndpt,inv_cb);
}

void InviteProc::Release( void )
{
	
}

pj_status_t	InviteProc::InviteResponse( pjsip_rx_data *rdata )
{
	const stUserAgentAddress* pUaAddr = UserManagement::GetIntance()->GetMediaServer();
	if(pUaAddr == NULL)
	{
		PjsipUserAgent	ua(rdata->tp_info.transport->endpt);
		ua.SendEndptResponseStateless(rdata,PJSIP_SC_SERVICE_UNAVAILABLE);
		OUTPUT_LOG(3,"Media Server Not Found!!\r\n");
		return	PJSIP_ERRNO_START;
	}

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
			/*pjsip_dialog* pDlg = pjsip_rdata_get_dlg(rdata);
			PJ_ASSERT_RETURN(pDlg != NULL, PJSIP_ENOTINITIALIZED);
			pjsip_inv_session* pInv = pjsip_dlg_get_inv_session(pDlg);
			PJ_ASSERT_RETURN(pInv != NULL, PJSIP_ENOTINITIALIZED);*/
			if(pjsip_rdata_get_dlg(rdata) == NULL)
			{
				pjsip_rdata_sdp_info* pSDPInfo = pjsip_rdata_get_sdp_info(rdata);
				char szSubject[100] = {0};
				strncpy(szSubject,pSDPInfo->sdp->name.ptr,pSDPInfo->sdp->name.slen);

				MediaSession_share_ptr	MediaSessionPtr( new MediaStreamSession(_sHostIP,_iHostPort) );
				if(pj_strcmp2(&pSDPInfo->sdp->name,"Playback") == 0)
				{
					MediaSessionPtr->SetMode(MediaStreamSession::MODE_PLAYBACK);
				}

				status = MediaSessionPtr->SendInviteRespond(rdata,PJSIP_SC_TRYING,NULL,MediaSessionPtr->GetSrcInv());
				
				char szTagIP[100] = {0};
				strncpy(szTagIP,rdata->msg_info.via->recvd_param.ptr,rdata->msg_info.via->recvd_param.slen);
				pSDPInfo->sdp->conn->addr = pj_str(szTagIP);
					
				pjmedia_sdp_session** pSDP = MediaSessionPtr->GetVedioSDP();
				*pSDP = pjmedia_sdp_session_clone( MediaSessionPtr->GetSrcInv()->pInv->dlg->pool,pSDPInfo->sdp );
				if((*pSDP)->conn != NULL && (*pSDP)->media_count > 0)
				{
					//add hdr
					char szID[100] = {0};
					char szIP[50] = {0};
					strncpy(szID,(*pSDP)->origin.user.ptr,(*pSDP)->origin.user.slen);
					strncpy(szIP,(*pSDP)->origin.addr.ptr,(*pSDP)->origin.addr.slen);

					/*char szUri[100] = {0};
					sprintf(szUri,"<sip:%s@%s:%d>",szID,szIP,(*pSDP)->media[0]->desc.port);*/

					/*std::string sLocalIP;
					PjsipInit::GetInstance()->GetHostIP(sLocalIP);*/
					MediaStreamSession::InvSessionInfo* pSessionInfo = MediaSessionPtr->GetRecvMediaInv();

					MediaSessionPtr->SendInviteRequestToTarget(pUaAddr->sIP.c_str(),pUaAddr->iPort,pFromUri,ToUri,NULL,pSessionInfo,szSubject/*szUri*/);
					_MediaSessionMap.insert1(std::make_pair(pSessionInfo->pInv, MediaSessionPtr));

					_MediaSessionMap.insert1(std::make_pair(MediaSessionPtr->GetSrcInv()->pInv, MediaSessionPtr));
				}
				else
				{
					OUTPUT_LOG(3,"media_sdp_session Format is Error\r\n");
				}
			}
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

void	InviteProc::Invite_on_state_changed( pjsip_inv_session *inv, pjsip_event *e )
{
	MediaSession_share_ptr	MediaSessionPtr;
	if(InviteProc::GetIntance()->GetMediaSessionMap()->IsExistKey2(inv,MediaSessionPtr))
	{
		pjsip_inv_session *pNewInv = NULL;
		MediaSessionPtr->Invite_on_state_changed(inv,e,&pNewInv);
		switch(inv->state)
		{
		case PJSIP_INV_STATE_DISCONNECTED:   /**< Session is terminated.		    */
			{
				InviteProc::GetIntance()->GetMediaSessionMap()->eraseKey(inv);
			}
			break;
		default:
			if(pNewInv != NULL)
			{
				InviteProc::GetIntance()->GetMediaSessionMap()->insert1(std::make_pair(pNewInv,MediaSessionPtr));
			}
			break;
		}
	}
}

void	InviteProc::Invite_on_new_session( pjsip_inv_session *inv, pjsip_event *e )
{
	
}

void	InviteProc::Invite_on_media_update( pjsip_inv_session *inv_ses, pj_status_t status )
{
	MediaSession_share_ptr	MediaSessionPtr;
	if(InviteProc::GetIntance()->GetMediaSessionMap()->IsExistKey2(inv_ses,MediaSessionPtr))
	{
		pjsip_inv_session *pNewInv = NULL;
		if(MediaSessionPtr->Invite_on_media_update(inv_ses,status) == PJ_SUCCESS)
		{
#ifdef USE_IOSERVRICEIOEX
			pjsip_to_hdr* to = (pjsip_to_hdr*) pjsip_msg_find_hdr(inv_ses->invite_req->msg, PJSIP_H_TO, NULL);
			if(to != NULL)
			{//push 
				pjsip_sip_uri*	pToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(to->uri);
				char szToUri[100] = {0};
				strncpy(szToUri,pToUri->user.ptr,pToUri->user.slen);
				InviteProc::GetIntance()->GetMediaSenderMap()->insert1(std::make_pair(szToUri,MediaSessionPtr.get()));
			}
#endif
		}
	}
}

void	InviteProc::Invite_on_send_ack( pjsip_inv_session *inv, pjsip_rx_data *rdata )
{
	OUTPUT_LOG(3,"InviteProc::Invite_on_send_ack\r\n");
}

void	InviteProc::Invite_on_tsx_state_changed( pjsip_inv_session *inv,
											pjsip_transaction *tsx,
											pjsip_event *e )
{

}

void	InviteProc::Invite_on_rx_Request( pjsip_rx_data *rdata )
{
	if(rdata != NULL)
	{
		pjsip_dialog* pDlg = pjsip_rdata_get_dlg(rdata);
		if(pDlg == NULL)
		{
			OUTPUT_LOG(3,"pjsip_rdata_get_dlg Is Failed!\r\n");
		}
		pjsip_inv_session* pInv = pjsip_dlg_get_inv_session(pDlg);
		if(pInv == NULL)
		{
			OUTPUT_LOG(3,"pjsip_dlg_get_inv_session Is Failed!\r\n");
		}

		MediaSession_share_ptr	MediaSessionPtr;
		if(InviteProc::GetIntance()->GetMediaSessionMap()->IsExistKey2(pInv,MediaSessionPtr))
		{
			MediaSessionPtr->Invite_on_rx_Request(rdata);
		}
		else
		{
			OUTPUT_LOG(3,"Invite_on_rx_Request Not Found Invite!\r\n");
		}
	}
}

void	InviteProc::Invite_on_rx_Response( pjsip_rx_data *rdata )
{
	if(rdata != NULL)
	{
		pjsip_dialog* pDlg = pjsip_rdata_get_dlg(rdata);
		if(pDlg == NULL)
		{
			OUTPUT_LOG(3,"pjsip_rdata_get_dlg Is Failed!\r\n");
			return ;
		}

		pjsip_inv_session* pInv = pjsip_dlg_get_inv_session(pDlg);
		if(pInv == NULL)
		{
			OUTPUT_LOG(3,"pjsip_dlg_get_inv_session Is Failed!\r\n");
			return ;
		}

		MediaSession_share_ptr	MediaSessionPtr;
		if(InviteProc::GetIntance()->GetMediaSessionMap()->IsExistKey2(pInv,MediaSessionPtr))
		{
			MediaSessionPtr->Invite_on_rx_Request(rdata);
		}
		else
		{
			OUTPUT_LOG(3,"Invite_on_rx_Response Not Found Invite!\r\n");
		}
	}
}

} // namespace juli
