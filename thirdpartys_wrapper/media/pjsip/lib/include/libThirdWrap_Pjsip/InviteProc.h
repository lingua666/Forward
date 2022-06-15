
#include "../../common/include/CMyMap.hpp"
#include "../../common/include/PjsipInit.hpp"
#include "../../common/include/PjsipRegisters.h"

struct pjsip_inv_session;

namespace juli {

class	MediaStreamSession;

class	InviteProc
{
public:
	typedef	boost::shared_ptr<MediaStreamSession>	MediaSession_share_ptr;
	typedef	CMyMap<pjsip_inv_session*,MediaSession_share_ptr>	MediaStreamSessionMap;
	typedef	CMyMap<std::string,MediaStreamSession*>		MediaSenderMap;

public:
	static	InviteProc* GetIntance( void )
	{
		static	InviteProc InvProc;
		return &InvProc;
	}

	static	void	Invite_on_state_changed( pjsip_inv_session *inv, pjsip_event *e );
	static	void	Invite_on_new_session( pjsip_inv_session *inv, pjsip_event *e );
	static	void	Invite_on_media_update( pjsip_inv_session *inv_ses, pj_status_t status );
	static	void	Invite_on_send_ack( pjsip_inv_session *inv, pjsip_rx_data *rdata );
	static	void	Invite_on_tsx_state_changed( pjsip_inv_session *inv,
												pjsip_transaction *tsx,
												pjsip_event *e );

	static	void	Invite_on_rx_Request( pjsip_rx_data *rdata );
	static	void	Invite_on_rx_Response( pjsip_rx_data *rdata );

	static	inline void	SetModule( const pjsip_module *pModule )
	{
		_pModule = const_cast<pjsip_module *>(pModule);
	}

	static	inline pjsip_module* GetModule( void )
	{
		return _pModule;
	}

private:
	static	pjsip_module	*_pModule;

public:
	InviteProc( void );
	explicit InviteProc( pjsip_endpoint	*pEndpt, std::string &sHostIP, int iHostPort );
	~InviteProc( void );

	void Init( pjsip_endpoint	*pEndpt, const std::string &sHostIP, int iHostPort );
	void Release( void );

	pj_status_t	InviteResponse( pjsip_rx_data *rdata );

	inline MediaStreamSessionMap*	GetMediaSessionMap( void )
	{
		return &_MediaSessionMap;
	}

	inline MediaSenderMap*	GetMediaSenderMap( void )
	{
		return &_MediaSenderMap;
	}

private:
	pjsip_endpoint*	_pEndpt;
	MediaStreamSessionMap	_MediaSessionMap;
	MediaSenderMap			_MediaSenderMap;
	std::string		_sHostIP;
	int				_iHostPort;
};

} // namespace juli
