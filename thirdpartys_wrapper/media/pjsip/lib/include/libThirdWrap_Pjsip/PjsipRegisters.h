
#ifndef  _JULI_PJSIPREGISTERS_H_DEF_
#define _JULI_PJSIPREGISTERS_H_DEF_

#include "PjsipInit.hpp"
#include <pjsip-ua/sip_regc.h>

namespace _thirdwrap_ {

#pragma  pack(push,1)
	struct stUserAgentAddress
	{
		std::string	sSpiUri;
		std::string	sUserName;
		std::string	sIP;
		pjsip_tx_data *tdata;
		int			iPort;
	};
#pragma  pack(pop)

class PjsipRegisters
{
public:
	enum enRegcStatus
	{
		STATUS_FAILED,
		STATUS_UNREGISTERS,
		STATUS_REGISTER_OK = PJSIP_SC_OK,
	};

	typedef	void	(*on_regc_cb)(struct pjsip_regc_cbparam *param);
	typedef	pj_status_t	(*on_auth_lookup_cred)( pj_pool_t *pool,
												const pj_str_t *realm,
												const pj_str_t *acc_name,
												pjsip_cred_info *cred_info );

public:
	static	PjsipRegisters*	GetInstance( void)
	{
		static PjsipRegisters s_Registers;
		return &s_Registers;
	}

	static	void ParseDigestCredential( pj_scanner *pScanner, pj_pool_t *pPool, 
										pjsip_digest_credential *pCred );

public:
	PjsipRegisters( void );

	~PjsipRegisters( void );

	pj_bool_t	Init( pj_bool_t isProxy, pjsip_endpoint* pEndpt, on_regc_cb OnRegcCB );

	void		Release( void );

	pj_status_t	ParseRxData( pjsip_rx_data *rdata );

	pj_status_t	SendRegistersRequest( pj_str_t psSvrUrl, pj_str_t psFromUrl, 
									pj_str_t psToUrl, int iCCnt,
									const pj_str_t psContact[],
									pj_uint32_t puExpires, const pj_str_t* pUserName,
									const pj_str_t* pPassword,
									int iSendPort = 0,
									const char* useragent = DEFAULT_FLAG_STRING_DEF );

	pj_status_t	SendUnRegistersRequest( const char* useragent = DEFAULT_FLAG_STRING_DEF );

	int			SendRegistersResponse( pjsip_rx_data *rdata,
									stUserAgentAddress* pUserAgentAddress = NULL,
									on_auth_lookup_cred	OnAuthLookupCred = NULL,
									const char* c_sz_MediaServerIP = NULL, int iMediaPort = 0,
									const char* useragent = DEFAULT_FLAG_STRING_DEF );



	pj_status_t	GenerateResponseCode( pj_str_t psUserName, pj_str_t psPassword,
									pjsip_digest_credential* pDigCred,
									pj_str_t* outResponseCode );

	pj_status_t	GenerateResponseCode( pj_str_t psUserName, pj_str_t psPassword,
									pj_str_t psNonce, pj_str_t psNc,
									pj_str_t psCNonce, pj_str_t psQop,
									pj_str_t psUri, pj_str_t psRealm,
									pj_str_t* outResponseCode );

	const pjsip_authorization_hdr*	FindAuthHdr( const pjsip_rx_data *rdata ) const;

	const pjsip_expires_hdr*		FindExpiresHdr( const pjsip_rx_data *rdata ) const;

	inline	const pjsip_endpoint*	GetEndPoint( void ) const
	{
		return _pEndpt;
	}

private:
	pj_bool_t				_isRegc;
	pj_bool_t				_isProxy;
	pjsip_endpoint			*_pEndpt;
	pjsip_regc				*_pRegc;
	pj_pool_t				*_pPool;

};

} // namespace _thirdwrap_

#endif
