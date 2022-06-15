
#include <libProto_Sip/Sip_Response.h>

namespace	_sip_ {

	Sip_Response::Sip_Response(void)
	{

	}

	void Sip_Response::SetStatusCode(sip_status_code Status)
	{
		_Status = Status;
	}

	_string_big_t	Sip_Response::Generate(void)
	{
		_string_t Head = "SIP/2.0 ";
		Head += _string_t::NumberToStr(_Status) + " ";
		Head += Sip_Proto_Common::GetStatusCodeString(_Status);
		return GenerateString(Head);
	}

}

