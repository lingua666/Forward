
#include "../../common/include/PjsipInit.hpp"
#include "../../common/include/PjsipRegisters.h"

#include "../../DataBase/include/DataBaseFactory.h"
#include "../../DataBase/include/MySqlDB.h"


namespace juli {

inline	pj_status_t	on_auth_lookup_cred( pj_pool_t *pool,
										   const pj_str_t *realm,
										   const pj_str_t *acc_name,
										   pjsip_cred_info *cred_info )
{
	const pj_str_t pjsip_USERNAME_STR =	{ "username", 8 };

	pj_scanner scanner;
	pjsip_digest_credential cred;
	pj_bzero(&cred,sizeof(pjsip_digest_credential));
	char* c_szbuf = acc_name->ptr - (pjsip_USERNAME_STR.slen + 2);
	pj_scan_init( &scanner, c_szbuf, strlen(c_szbuf), 0,NULL);
	PjsipRegisters::ParseDigestCredential( &scanner, pool, &cred);

	cred_info->realm = *realm;
	cred_info->username = cred.username;
	cred_info->data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	cred_info->data = pj_str("12345678");

#ifdef ENABLE_MYSQL
	char szSql[100] = {0};
	char szUserName[100] = {0};
	strncpy(szUserName,cred_info->username.ptr,cred_info->username.slen);
	sprintf(szSql,"select password from testdb where username='%s' LIMIT 1",szUserName);
	RecordSet Set(MySqlDB::GetInstance()->GetMysqlHandle(),szSql);
	if(Set.GetRow() > 0)
	{
		cred_info->data = pj_str(const_cast<char*>(Set.GetValue(0)));
	}
#endif

	return PJ_SUCCESS;
}

} // namespace juli
