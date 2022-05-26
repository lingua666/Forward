
#include "../include/RegcProc.hpp"
#include "../../common/include/PjsipUA.h"
#include "../include/UserManagement.h"
#include "../include/InviteProc.h"
#include "../include/ProxyServerApp.h"

#include "../../DataBase/include/DataBaseFactory.h"
#include "../../DataBase/include/MySqlDB.h"


namespace juli {

void ProxyServerApp::Proxy_onRegcCB(struct pjsip_regc_cbparam *param)
{
	PjsipRegisters* pRegisters = reinterpret_cast<PjsipRegisters*>(param->token);
	if(param->rdata == NULL)
	{
		OUTPUT_LOG(3,"rdata is NULL,PjsipRegisters TimeOut\r\n");
		return ;
	}

	OUTPUT_LOG(3,"%d\r\n",param->rdata->msg_info.msg->line.req.method.id);

	//Register request
	switch(param->rdata->msg_info.msg->line.req.method.id)
	{
	case PJSIP_REGISTER_METHOD://×¢²áÇëÇó

		break;
	case PJSIP_SC_UNAUTHORIZED:
	case PJSIP_SC_PROXY_AUTHENTICATION_REQUIRED:
		break;
	case PJSIP_SC_OK:
		{//Notify

		}
		break;
	case PJSIP_CANCEL_METHOD:

		break;
	case PJSIP_ACK_METHOD:

		break;
	case PJSIP_BYE_METHOD:

		break;
	case PJSIP_INVITE_METHOD:

		break;
	}

	return ;
}

pj_bool_t ProxyServerApp::Proxy_on_rx_Request( pjsip_rx_data *rdata )
{
	pjsip_transport *uas_tsx = rdata->tp_info.transport;

	OUTPUT_LOG(3,"Proxy_on_rx_Request: method %d\r\n",rdata->msg_info.msg->line.req.method.id);

	switch(rdata->msg_info.msg->line.req.method.id)
	{
	case PJSIP_REGISTER_METHOD:
		{
			int iType = UserManagement::TYPE_USER_AGENT;
			const char* szMediaIP = NULL;
			int			iMediaPort = 0;
			pjsip_user_agent_hdr* pUserAgent = (pjsip_user_agent_hdr*)
												pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_OTHER, NULL);

			if(pUserAgent != NULL && pj_strncmp2(&pUserAgent->hvalue,MEDIASERVER_STRING_DEF,strlen(MEDIASERVER_STRING_DEF)) == 0)
			{
				iType = UserManagement::TYPE_MEDIA_SERVER;
				if(rdata->msg_info.msg->body != NULL)
				{
					char szPort[10] = {0};
					pjsip_print_text_body(rdata->msg_info.msg->body,szPort,sizeof(szPort));
					ProxyServerApp::GetInstance()->SetMediaSendPort(atoi(szPort));

					OUTPUT_LOG(3,"MediaSendPort :%d\r\n",atoi(szPort));
				}
				//UserAgentAddress.sIP = std::string("192.168.1.231");//_sIP;
				//UserAgentAddress.sIP = std::string("58.67.136.34");//_sIP;
			}
			else
			{
				const stUserAgentAddress* pMeidaAddr = UserManagement::GetIntance()->GetMediaServer();
				if(pMeidaAddr == NULL)
				{
					PjsipUserAgent	ua(rdata->tp_info.transport->endpt);
					ua.SendEndptResponseStateless(rdata,PJSIP_SC_NOT_FOUND);
				}
				else
				{
					std::string sLocalIP;
					PjsipInit::GetHostIP(sLocalIP);
					if( strcmp(pMeidaAddr->sIP.c_str(),"127.0.0.1") == 0 ||
						strcmp(pMeidaAddr->sIP.c_str(),sLocalIP.c_str()) == 0
						|| strncmp(pMeidaAddr->sIP.c_str(),"192.168.",8) == 0 )
					{
						szMediaIP = ProxyServerApp::GetInstance()->_sIP.c_str();//pMeidaAddr->sIP.c_str();
					}
					else
						szMediaIP = pMeidaAddr->sIP.c_str();

					iMediaPort = ProxyServerApp::GetInstance()->GetMediaSendPort();//40000;
					OUTPUT_LOG(3,"szMediaIP:%s,iMediaPort  :%d\r\n",szMediaIP,iMediaPort);
				}
			}

			stUserAgentAddress	UserAgentAddress;
			int iStatusCode = PjsipRegisters::GetInstance()->SendRegistersResponse(rdata,&UserAgentAddress,on_auth_lookup_cred,szMediaIP,iMediaPort);

			switch(iStatusCode)
			{
			case PjsipRegisters::STATUS_REGISTER_OK:
				if(UserAgentAddress.iPort > 0)
				{//Registers Success
					if(UserManagement::GetIntance()->Insert(UserAgentAddress.sUserName.c_str(),
						UserAgentAddress.sIP.c_str(),
						UserAgentAddress.iPort,UserAgentAddress.sSpiUri.c_str(),iType) == EC_S_OK)
					{
						OUTPUT_LOG(3,"Receive Registers Quest:\r\n");
						OUTPUT_LOG(3,"	UserName:%s, IP:%s, Port:%d\r\n",UserAgentAddress.sUserName.c_str(),
							UserAgentAddress.sIP.c_str(),UserAgentAddress.iPort);
						
						//test
						char szRemoteHost[100] = {0};
						char szRecvHost[100] = {0};
						strncpy(szRemoteHost,rdata->tp_info.transport->remote_name.host.ptr,rdata->tp_info.transport->remote_name.host.slen);
						strncpy(szRecvHost,rdata->msg_info.via->recvd_param.ptr,rdata->msg_info.via->recvd_param.slen);
						OUTPUT_LOG(3,"	remote_addr:%s:%d, Revice:%s:%d\r\n",szRemoteHost,rdata->tp_info.transport->remote_name.port,
							szRecvHost,rdata->msg_info.via->rport_param);
						//test
					}
				}
				break;
			case PjsipRegisters::STATUS_UNREGISTERS:	//UnRegisters
				{
					UserManagement::GetIntance()->Remove(UserAgentAddress.sUserName.c_str(),iType);
					OUTPUT_LOG(3,"UserName:%s unRegisters\r\n",UserAgentAddress.sUserName.c_str());
				}
				break;
			}
		}
		break;
	case PJSIP_INVITE_METHOD:
		{
			InviteProc::GetIntance()->InviteResponse(rdata);
		}
		break;
	case PJSIP_ACK_METHOD:
		{
			/*printf("PJSIP_ACK_METHOD\r\n");
			PjsipUserAgent	ua(uas_tsx->endpt);
			pjsip_tx_data *tdata;	
			ua.CreateEndptRequest_fwd(rdata,NULL,&tdata);
			status = ua.SendEndptRequestStateless(tdata);

			PJ_ASSERT_RETURN(status == PJ_SUCCESS, PJ_FALSE);*/
			return PJ_FALSE;
		}
		break;
	case PJSIP_BYE_METHOD:
		{
			/*PjsipUserAgent	ua(uas_tsx->endpt);
			ua.SendEndptResponseStateless(rdata,PJSIP_SC_OK);*/
		}
		break;
	case PJSIP_OPTIONS_METHOD:
	case PJSIP_OTHER_METHOD:
		{
			PjsipUserAgent	ua(uas_tsx->endpt);

			if(pj_strcmp2(&rdata->msg_info.msg->line.req.method.name,"MESSAGE") == 0 || 
				pj_strcmp2(&rdata->msg_info.msg->line.req.method.name,"INFO") == 0)
			{//MESSAGE or INFO
				if(pjsip_rdata_get_dlg(rdata) != NULL)
				{
					InviteProc::Invite_on_rx_Request(rdata);
				}
				else
				{
					//ua.SendEndptResponseStateless(rdata,PJSIP_SC_TRYING);

					pjsip_sip_uri*	ContactUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.msg->line.req.uri);
					pjsip_sip_uri*	FromUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.from->uri);
					pjsip_sip_uri*	ToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);

					char szSrcName[100] = {0};
					char szDstName[100] = {0};
					strncpy(szSrcName,FromUri->user.ptr,FromUri->user.slen);
					strncpy(szDstName,ToUri->user.ptr,ToUri->user.slen);
					const stUserAgentAddress* pSUaDstAddress = UserManagement::GetIntance()->IsExist(szDstName);//FindMapList(string(szSrcName));
					if(pSUaDstAddress != NULL)
					{
						char szContact[100] = {0};
						char szFromUri[100] = {0};
						char szToUri[100] = {0};
						char szSvrUri[100] = {0};
						char szHost[100] = {0};
						strncpy(szHost,ContactUri->host.ptr,ContactUri->host.slen);
						
						if(pj_strcmp2(&rdata->msg_info.via->sent_by.host,pSUaDstAddress->sIP.c_str()) == 0
							&& rdata->msg_info.via->sent_by.port == pSUaDstAddress->iPort)
						{
							const stUserAgentAddress* pSUaSrcAddress = UserManagement::GetIntance()->IsExist(szSrcName);
							if(pSUaSrcAddress == NULL)
							{
								ua.SendEndptResponseStateless(rdata,PJSIP_SC_NOT_ACCEPTABLE);
								return PJ_FALSE;
							}
							sprintf(szSvrUri,"<sip:%s@%s:%d>",szSrcName,pSUaSrcAddress->sIP.c_str(),pSUaSrcAddress->iPort);
						}
						else
						{
							sprintf(szSvrUri,"<sip:%s@%s:%d>",szDstName,pSUaDstAddress->sIP.c_str(),pSUaDstAddress->iPort);
						}

						rdata->msg_info.via->sent_by.host = rdata->msg_info.via->recvd_param;
						rdata->msg_info.via->sent_by.port = rdata->msg_info.via->rport_param;

						sprintf(szContact,"<sip:%s@%s:%d>",szSrcName,szHost,ContactUri->port);
						pj_str_t psContactUri = pj_str(szContact);

						pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR,rdata->msg_info.from->uri,szFromUri,sizeof(szFromUri));
						pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR,rdata->msg_info.to->uri,szToUri,sizeof(szToUri));
						pj_str_t psFromUri = pj_str(szFromUri);
						pj_str_t psToUri = pj_str(szToUri);
						pj_str_t psSvrUri = pj_str(szSvrUri);
					
						pjsip_uri* pUri = pjsip_parse_uri(rdata->tp_info.pool,szSvrUri,strlen(szSvrUri),NULL);
						char szHostIP[100] = {0};
						strncpy(szHostIP,ContactUri->host.ptr,ContactUri->host.slen);
						ua.TransmitEndptRequest(rdata,pUri,ProxyServerApp::GetInstance()->GetIP().c_str(),ContactUri->port);
						OUTPUT_LOG(3,"psSvrUri:%s\r\n",szSvrUri);
					}
				}
			}
			else
			{
				ua.SendEndptResponseStateless(rdata,PJSIP_SC_NOT_ACCEPTABLE);
			}
		}
		break;

	}

	return PJ_TRUE;
}

pj_bool_t ProxyServerApp::Proxy_on_rx_Response( pjsip_rx_data *rdata )
{
	if(pj_strcmp2(&rdata->msg_info.cseq->method.name,"MESSAGE") == 0 
		|| pj_strcmp2(&rdata->msg_info.cseq->method.name,"INFO") == 0)
	{
		if(pjsip_rdata_get_dlg(rdata) != NULL)
		{
			InviteProc::Invite_on_rx_Response(rdata);
		}
		else
		{
			pjsip_sip_uri*	FromUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.from->uri);
			pjsip_sip_uri*	ToUri = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);
			char szSrcName[100] = {0};
			char szDstName[100] = {0};
			strncpy(szSrcName,FromUri->user.ptr,FromUri->user.slen);
			strncpy(szDstName,ToUri->user.ptr,ToUri->user.slen);
			const stUserAgentAddress* pSUaDstAddress = UserManagement::GetIntance()->IsExist(szDstName);
			const stUserAgentAddress* pSUaSrcAddress = UserManagement::GetIntance()->IsExist(szSrcName);

			if(pSUaDstAddress != NULL && pSUaSrcAddress != NULL)
			{
				PjsipUserAgent	ua(rdata->tp_info.transport->endpt);
				pj_status_t status = ua.TransmitEndptResponse(rdata);
			}
		}
	}

	return PJ_TRUE;
}

ProxyServerApp::ProxyServerApp( void )
				:_iPort(0)
{
	
}

ProxyServerApp::~ProxyServerApp( void )
{
	Release();
}

pj_bool_t	ProxyServerApp::Init( const  std::string& sIP, int iPort )
{
	pj_bool_t bResult = PJ_FALSE;
	bResult = _Proxy.Init(sIP,iPort,Proxy_on_rx_Request,Proxy_on_rx_Response);
	if(bResult)
	{
		PjsipRegisters::GetInstance()->Init(PJ_TRUE,const_cast<pjsip_endpoint*>(_Proxy.GetEndPoint()),
											&ProxyServerApp::Proxy_onRegcCB);
		InviteProc::SetModule(_Proxy.GetRegisterModule());
		InviteProc::GetIntance()->Init(const_cast<pjsip_endpoint*>(_Proxy.GetEndPoint()),sIP,iPort);
		_sIP = sIP;
		_iPort = iPort;

		//Mysql Init
#ifdef ENABLE_MYSQL
		CDataBaseFactory::GetInstance()->BindDataBases(MySqlDB::GetInstance());
		CDataBaseFactory::GetInstance()->Init();
		CDataBaseFactory::GetInstance()->Connect("testdb","127.0.0.1","root","",3306);
#endif
	}
	return bResult;
}

void	ProxyServerApp::Release( void )
{
	_Proxy.Release();
}

} // namespace juli
