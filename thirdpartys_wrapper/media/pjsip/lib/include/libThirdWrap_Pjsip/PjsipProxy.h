
#ifndef  _JULI_PJSIPPROXY_H_DEF_
#define _JULI_PJSIPPROXY_H_DEF_

#include "PjsipInit.hpp"

namespace juli {

/* Common proxy functions */
#define STATEFUL    1

class PjsipProxy : public boost::noncopyable
{
public:
	typedef	pj_bool_t	(*on_rx_Request)( pjsip_rx_data *rdata );
	typedef	pj_bool_t	(*on_rx_Response)( pjsip_rx_data *rdata );

#if PJ_HAS_THREADS
	static int WorkerThread(void *p);
#endif

public:
	PjsipProxy( void );
	PjsipProxy( std::string &sIP, int iPort, std::string sRealm, on_rx_Request	OnRequest = NULL,
				on_rx_Response OnResponse = NULL );
	~PjsipProxy( void );

	pj_bool_t	Init( const std::string &sIP, int iPort, on_rx_Request	OnRequest = NULL,
					on_rx_Response OnResponse = NULL );

	void		Release( void );

	inline	const pjsip_endpoint*	GetEndPoint( void ) const
	{
		return _pEndpt;
	}

	inline const pjsip_module* GetRegisterModule( void )
	{
		return &_Module;
	}

private:
	pj_status_t	RegisterModule(	on_rx_Request	OnRequest,
								on_rx_Response OnResponse );
	pj_status_t	UnRegisterModule( void );

	static pj_status_t	ProcessRouting( pjsip_tx_data *tdata );

	static pj_status_t	CalculateTarget( pjsip_rx_data *rdata, pjsip_tx_data *tdata );
	static void			AddRouteHeader(pjsip_tx_data *tdata);

private:
	pjsip_module	_Module;
	pjsip_endpoint	*_pEndpt;
	pjsip_transport *_pTransport;
	pj_thread_t		*_pThread;
	pj_pool_t		*_pPool;
	pj_bool_t		_isInit;
	const string	_sRealm;
	std::string		_sIP;
	int				_iPort;
};

} // namespace juli

#endif
