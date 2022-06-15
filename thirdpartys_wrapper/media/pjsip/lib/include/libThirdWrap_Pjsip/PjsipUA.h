
#ifndef  _JULI_PJSIPUA_H_DEF_
#define _JULI_PJSIPUA_H_DEF_

#include "PjsipInit.hpp"

namespace juli {		

class PjsipUserAgent : public boost::noncopyable
{
private:
	typedef	pj_bool_t	(*on_rx_Request)( pjsip_rx_data *rdata );
	typedef	pj_bool_t	(*on_rx_Response)( pjsip_rx_data *rdata );
	
public:
	explicit PjsipUserAgent( int iPort, on_rx_Request OnRequest = NULL,
							on_rx_Response Response = NULL );
	explicit PjsipUserAgent( pjsip_endpoint*	_pEndpt );
	~PjsipUserAgent( void );

	pj_bool_t	Init( int iPort, on_rx_Request OnRequest,
					on_rx_Response Response );
	void		Release( void );
	void		FlushEvents( pj_uint32_t puDuration );

	pj_status_t		SendEndptRequest( const pjsip_method *pMethod, const pj_str_t psSrvUri,
									const pj_str_t psFromUri, const pj_str_t psToUrl,
									const pj_str_t psContactUri, pjsip_msg_body* pMsgBody,
									const char* szUseragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t		SendEndptRequestStateless( pjsip_tx_data *tdata, const char* useragent = DEFAULT_FLAG_STRING_DEF );
	pj_status_t		TransmitEndptRequest( pjsip_rx_data *rdata, const pjsip_uri *uri, const char* c_HostIp, int iPort = 5060 );

	pj_status_t		CreateEndptRequest_fwd( pjsip_rx_data *rdata, const pjsip_uri *uri,
											pjsip_tx_data **tdata,
											const char* useragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t		SendAuthSrvResponse( pjsip_rx_data *rdata, pjsip_auth_srv* auth_srv,
											const char* useragent = DEFAULT_FLAG_STRING_DEF );
	pj_status_t		SendEndptResponse2( pjsip_rx_data *rdata,
										pjsip_tx_data *tdata,
										const char* useragent = DEFAULT_FLAG_STRING_DEF );


	pj_status_t		SendEndptResponse3(	pjsip_response_addr *res_addr, pjsip_tx_data *tdata,
										const char* useragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t		SendEndptResponseStateless( const pjsip_rx_data *rdata,
												int st_code,
												const char* useragent = DEFAULT_FLAG_STRING_DEF,
												const pj_str_t *st_text = NULL,
												const pjsip_hdr *hdr_list = NULL,
												const pjsip_msg_body *body = NULL);

	pj_status_t		TransmitEndptResponse( pjsip_rx_data *rdata );

	pj_status_t		CreateEndptResponse_fwd( pjsip_rx_data *rdata, 
											pjsip_tx_data **p_tdata,
											const char* useragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t		SendEndptACK( const pjsip_tx_data *tdata,
							const pjsip_rx_data *rdata,
							const char* useragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t		SendEndptCancel( const pjsip_tx_data *tdata,
									const pjsip_rx_data *rdata,
									const char* useragent = DEFAULT_FLAG_STRING_DEF );

	inline	const pjsip_endpoint*	GetEndPoint( void ) const
	{
		return _pEndpt;
	}
private:
	pj_status_t	RegisterModule( on_rx_Request OnRequest,
								on_rx_Response Response );

	pj_status_t	UnRegisterModule( void );
	
private:
	pj_bool_t		_isBind;
	pjsip_module	_Module;
	pjsip_endpoint	*_pEndpt;
	pj_pool_t		*_pPool;
	int				_iPort;

};

} // namespace juli

#endif
