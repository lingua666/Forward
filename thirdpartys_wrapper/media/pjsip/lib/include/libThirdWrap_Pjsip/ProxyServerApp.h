
#ifndef  _JULI_PROXYSERVERAPP_H_DEF_
#define _JULI_PROXYSERVERAPP_H_DEF_

#include "../../common/include/PjsipProxy.h"

namespace juli {

class ProxyServerApp
{
public:
	static	void Proxy_onRegcCB(struct pjsip_regc_cbparam *param);
	static	pj_bool_t Proxy_on_rx_Request( pjsip_rx_data *rdata );
	static	pj_bool_t Proxy_on_rx_Response( pjsip_rx_data *rdata );

	static	ProxyServerApp*	GetInstance( void )
	{
		static	ProxyServerApp	s_ServerApp;
		return	&s_ServerApp;
	}

public:
	ProxyServerApp( void );
	~ProxyServerApp( void );

	pj_bool_t	Init( const std::string& sIP, int iPort );
	void	Release( void );


	inline std::string& GetIP( void )
	{
		return _sIP;
	}

	inline void SetMediaSendPort( int iMediaSendPort )
	{
		_iMediaSendPort = iMediaSendPort;
	}

	inline const int GetMediaSendPort( void ) const
	{
		return _iMediaSendPort;
	}

private:
	PjsipProxy	_Proxy;
	std::string	_sIP;
	int			_iPort;
	int			_iMediaSendPort;
};

} // namespace juli

#endif
