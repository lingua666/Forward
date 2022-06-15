
#ifndef  _JULI_MEDIASTREAMSESSION_H_DEF_
#define _JULI_MEDIASTREAMSESSION_H_DEF_

#include "../../common/include/PjsipInvite.h"
//#include "../include/IOService.h"

namespace juli {

//#define PROXY_HOST_IP_DEF		"192.168.1.46"		
//#define PROXY_HOST_PORT_DEF		5060

//#define	MEDIA_URI_DEF			"34020000001180000002"
//#define	MEDIA_PORT_DEF			10010
//#define	MEDIA_SENDPORT_DEF		8888

class MediaStreamSession
{
public:
enum	enInvStatus
{
	STATUS_NULL,
	STATUS_PLAY,
	STATUS_PLAYBACK,
	STATUS_STOP_PENDING,
	STAUS_STOP,
	STATUS_WAIT_OK,
	STATUS_WAIT_ACK,
	STATUS_WAIT_BYE,
	STATUS_BYE
};

#pragma pack(push,1)
	struct	InvSessionInfo
	{
		pjsip_inv_session	*pInv;
		enInvStatus			iStatus;
	};
#pragma pack(pop)

public:
	enum	enMediaMode
	{
		MODE_PLAY,
		MODE_PLAYBACK
	};

	/*static	MediaStreamSession* GetInstance( void )
	{
		static	MediaStreamSession	MediaStream;
		return	&MediaStream; 
	}*/

public:
	MediaStreamSession( std::string &sIP, int iPort,
								int iMode = MODE_PLAY );
	~MediaStreamSession( void );

	void	Invite_on_rx_Request( pjsip_rx_data *rdata );
	void	Invite_on_rx_Respond( pjsip_rx_data *rdata );

	pj_status_t	Invite_on_state_changed( pjsip_inv_session *inv, pjsip_event *e,
										pjsip_inv_session **p_newinv );

	pj_status_t	Invite_on_media_update( pjsip_inv_session *inv_ses, pj_status_t status );
	pj_status_t	InviteResponse( pjsip_rx_data *rdata );

	pj_status_t	SendInviteRequestToTarget( const char* szTargetIP, int iTargetPort,
										pjsip_sip_uri* pFromUri,
										pjsip_sip_uri*	pToUri,
										const pjmedia_sdp_session *local_sdp,
										InvSessionInfo* pInvSessionInfo,
										const char* szSubject = NULL );

	pj_status_t	SendInviteRespond( pjsip_rx_data *rdata, int st_code, 
								const pjmedia_sdp_session *local_sdp,
								InvSessionInfo* pInvSessionInfo );

	pj_status_t	OnSendAck( pjsip_inv_session *inv, pjsip_rx_data *rdata );

	pj_status_t	Stop( void );

	void	InviteDisconnect( pjsip_inv_session *inv, pjsip_event *e );
	
	inline bool	IsOwns( pjsip_inv_session *inv )
	{
		if(_SrcInvSession.pInv == inv || 
			_TargetInvSession.pInv == inv || 
			_RecvMediaInvSession.pInv == inv ||
			_SendMediaInvSession.pInv == inv )
			return true;
		return false;
	}

	inline void SetMode( int iMode )
	{
		if(iMode > MODE_PLAYBACK)
			iMode = MODE_PLAYBACK;
		if(iMode < MODE_PLAY)
			iMode = MODE_PLAY;
		_iMode = iMode;
	}

	inline const int GetStatus( void ) const 
	{
		return _iStatus;
	}

	inline	pjmedia_sdp_session**	GetVedioSDP( void )
	{
		return &_pSrcSDP;
	}

	inline	MediaStreamSession::InvSessionInfo*	GetSrcInv( void )
	{
		return &_SrcInvSession;
	}

	inline	MediaStreamSession::InvSessionInfo*	GetTargetInv( void )
	{
		return &_TargetInvSession;
	}

	inline	MediaStreamSession::InvSessionInfo* GetSendMediaInv( void )
	{
		return &_SendMediaInvSession;
	}

	inline	MediaStreamSession::InvSessionInfo* GetRecvMediaInv( void )
	{
		return &_RecvMediaInvSession;
	}
	
	inline void	LookerRefInc( void )
	{
		_iLookerNumber ++;
	}

	inline void	LookerRefDec( void )
	{
		if(_iLookerNumber > 0)
			_iLookerNumber --;
	}

	inline const int	GetLookerRef( void ) const 
	{
		return _iLookerNumber;
	}
	
private:
	InvSessionInfo		_SrcInvSession;
	InvSessionInfo		_TargetInvSession;
	InvSessionInfo		_SendMediaInvSession;
	InvSessionInfo		_RecvMediaInvSession;
	int					_iLocalPort;
	int					_iStatus;
	int					_iLookerNumber;

	char				_szToUri[100];
	pjmedia_sdp_session	*_pSrcSDP;
	bool				_isTimeout;
	std::string			_sIP;
	int					_iPort;
	int					_iMode;
};

} // namespace juli

#endif
