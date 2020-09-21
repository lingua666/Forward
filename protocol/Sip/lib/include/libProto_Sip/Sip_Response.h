
#ifndef __SIP_SIP_RESPONSE_20160510104022_1462848022_H__
#define __SIP_SIP_RESPONSE_20160510104022_1462848022_H__

#include <libProto_Sip/Sip_Proto_Common.h>

namespace	_sip_ {

	class Sip_Response : public ISip_Proto
	{
	protected:
		Sip_Response(void);

		virtual ~Sip_Response(void)
		{}

		void SetStatusCode(sip_status_code Status);

		_string_big_t	Generate(void);

	private:
		sip_status_code	_Status;
	};

	class Sip_Response_Register : public Sip_Response
	{
	public:
		Sip_Response_Register( void )
		{}

		~Sip_Response_Register(void)
		{}

		int Init(sip_status_code Status)
		{
			SetStatusCode(Status);
			return 1;
		}
	};
	
}

#endif
