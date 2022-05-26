
#ifndef  _JULI_PJSIPINVITE_H_DEF_
#define _JULI_PJSIPINVITE_H_DEF_

#include "PjsipInit.hpp"
#include <pjsip-ua/sip_inv.h>
#include <pjmedia/transport.h>

namespace juli {

class	InviteRegc
{
public:
	InviteRegc( pjsip_endpoint	*pEndpt, pjsip_inv_callback inv_cb, int iPort = 0 );
	~InviteRegc( void );

	static	pj_bool_t	_isInit;
private:
	pj_bool_t Init( pjsip_endpoint	*pEndpt, pjsip_inv_callback inv_cb, int iPort = 0 );
	void Release( void );

	pjmedia_endpt *_pMediaEndpt;
};


class PjsipInvite : public boost::noncopyable
{
public:
#define		MACRO_IS_VALIED(t)	\
			if(t == NULL)\
				return PJSIP_ENOTINITIALIZED;

public:
	static	pj_status_t	CreateUasDialog( pjsip_rx_data *rdata,  pj_str_t& psContactUri, pjsip_dialog **pDlg );

	static	pj_status_t	CreateUasInv( pjsip_dialog *dlg,
									pjsip_rx_data *rdata,
									const pjmedia_sdp_session *local_sdp,
									pjsip_inv_session **p_inv );

	static	pj_status_t	CreateUacDialog( const pj_str_t *local_uri, const pj_str_t *local_contact,
										const pj_str_t *remote_uri, const pj_str_t *target, pjsip_dialog **pDlg );
	static	pj_status_t	CreateUacInv( pjsip_dialog *dlg,
									const pjmedia_sdp_session *local_sdp,
									pjsip_inv_session **p_inv );

	static const pjsip_uri*	GetReceiverUri( pjsip_msg	*msg ) ;

public:
	explicit PjsipInvite( pjsip_inv_session *pInv, pj_bool_t isProxy = PJ_TRUE );
	~PjsipInvite( void );

	pj_status_t SendAck( int iCSeq, pjmedia_sdp_session *pSDPSession = NULL, const char* useragent = DEFAULT_FLAG_STRING_DEF );
	pj_status_t	SendInvAnswer1(  int st_code, const pjmedia_sdp_session *sdp = NULL, const char* useragent = DEFAULT_FLAG_STRING_DEF );
	pj_status_t	SendInvAnswer2( pjsip_rx_data *rdata, int st_code,
							const pjmedia_sdp_session *sdp = NULL, const char* useragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t SendEndSession( int st_code = PJSIP_SC_OK, const char* useragent = DEFAULT_FLAG_STRING_DEF );
	pj_status_t SendTerminateSession( int st_code );
	pj_status_t	CreateVideoSDPSession( const char* c_szID,
									const char* c_szSDPName ,
									const char* c_szIP, int iPort,
									pjmedia_sdp_session **pSDPSession );

	pj_status_t	SendInvRequest( const char* szSubject = NULL, const char* useragent = DEFAULT_FLAG_STRING_DEF );
	pj_status_t	SendInvRespond( pjsip_rx_data *rdata, pj_str_t& psContactUri,
								const pjmedia_sdp_session *pLocalSDP, const char* useragent = DEFAULT_FLAG_STRING_DEF );

private:
	pj_status_t CreateMediaBaseSDP( string sUserName,pjmedia_endpt *endpt,
												pj_pool_t *pool,
												const pj_str_t *sess_name,
												const pj_sockaddr *origin,
												pjmedia_sdp_session **p_sdp);

	pj_status_t dlg_set_route_set( pjsip_dialog *dlg,
									const pjsip_route_hdr *route_set );

private:
	pjsip_inv_session	*_pInv;
	pj_bool_t			_isProxy;
};

} // namespace juli

#endif
