#ifdef MARVELL_EXTENDED
typedef struct {
	int mode;
	char *operLongStr;
	char *operShortStr;
	char *operNumStr;
	int act;
} RIL_OperInfo;

//add sim contact struct by yanglei
typedef struct {
	int index;
	char *number;
	int type;
	char *name;
} RIL_CPSimContact;

typedef struct {
    int cid;
    int state;
} RIL_CIDState;

typedef struct {
    int iptype;
    char *ipv4;
    char *ipv6;
    char *apn;
} RIL_PdpIpApn;

typedef struct {
    int fun;
    int config;
}RIL_CfunInfo;

typedef struct {
    int storage;
    char *password;
}RIL_CPBSsetInfo;

typedef struct {
    int smsready;
    int memfull;
}RIL_SMSStatus;

typedef struct {
    int cat;
    char *number;
}RIL_DialEmergency;

typedef struct {
    int time;
    int rtime;
    int intimer1;
    int intimer2;
}RIL_ECALLCfg;

typedef struct {
	int active;
	char *testnum;
	char *recfgnum;
} RIL_ECALLOnly;

typedef struct {
	int active;
    int simEcall;
	char *num1;
	char *num2;
} RIL_ECALLOnlyInfo;

typedef struct {
    int state;
    int cid;
    char *ip_type;
    char *ip;
    char *gateway;
    char *primary_dns;
    char *secondary_dns;
} RIL_IPDNSInfo;

typedef struct {
    int cid;
    char *ip_type;
    char *apn;
    int data_on_roaming;
    int connect_mode;
    int data_on_boot;
} RIL_ZGDCONTInfo;

typedef enum {
	NO_DOMAIN,
	CAMPED_DOMAIN,
	CS_DOMAIN,
	PS_DOMAIN,
	CS_PS_DOMAIN,
} RIL_REGDOMAIN;

typedef struct {
	char *mccmnc;
	/*ipv4:1,ipv6:2,ipv4v6:3*/
	int pdptype; 
} RIL_ZROAM;

typedef enum {
 	LOCKLEVEL_NEVER_LOCKED,
	LOCKLEVEL_LOCKED,
	LOCKLEVEL_ALREADY_UNLOCKED,
	LOCKLEVEL_UNKNOWN,
} RIL_LOCKLEVEL ;
/***********************************************************************/

#define RIL_REQUEST_EXT_BASE 500

/**
 * RIL_REQUEST_DIAL_VT
 *
 * Initiate VT call
 *
 * "data" is const RIL_Dial *
 * "response" is NULL
 *
 * This method is never used for supplementary service codes
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DIAL_VT (RIL_REQUEST_EXT_BASE + 0)

/**
 * RIL_REQUEST_HANGUP_VT
 *
 * Will only be sent while VT call is active or incoming.
 *
 * "data" is an int *
 * (int *)data)[0] is cause value of the reject
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_HANGUP_VT (RIL_REQUEST_EXT_BASE + 1)

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_EXT
 *
 * Manually select a specified network extension.
 *
 * "data" is a const RIL_OperInfo *
 * Please note that RIL_OperInfo have operLongStr field and operShortStr may be NULL
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  ILLEGAL_SIM_OR_ME
 *  GENERIC_FAILURE
 *
 * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
 *       no retries needed, such as illegal SIM or ME.
 *       Returns GENERIC_FAILURE for all other causes that might be
 *       fixed by retries.
 *
 */

#define RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_EXT    (RIL_REQUEST_EXT_BASE + 2)

/**
 * RIL_REQUEST_QUERY_COLP
 *
 * Queries the status of the COLP supplementary service
 *
 * (for MMI code "*#76")
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "COLP provisioned"
 *                           and 0 for "CLIP not provisioned"
 *                           and 2 for "unknown, e.g. no network etc"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_QUERY_COLP (RIL_REQUEST_EXT_BASE + 3)

/**
 * RIL_REQUEST_SET_CLIP
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CLIP  (RIL_REQUEST_EXT_BASE + 4)

/**
 * RIL_REQUEST_SET_COLP
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_COLP  (RIL_REQUEST_EXT_BASE + 5)

/**
 * RIL_REQUEST_GET_CNAP
 *
 * Queries the status of the CNAP supplementary service
 *
 * (for MMI code "*#300#")
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "CNAP provisioned"
 *                    and 0 for "CNAP not provisioned"
 *                    and 2 for "unknown, e.g. no network etc"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_CNAP (RIL_REQUEST_EXT_BASE + 6)

/**
 * RIL_REQUEST_SET_CNAP
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CNAP  (RIL_REQUEST_EXT_BASE + 7)

/**
 * RIL_REQUEST_QUERY_COLR
 *
 * Queries the status of the COLR supplementary service
 *
 * (for MMI code "*#77#")
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "COLR provisioned"
 *                           and 0 for "COLR not provisioned"
 *                           and 2 for "unknown, e.g. no network etc"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_QUERY_COLR (RIL_REQUEST_EXT_BASE + 8)

/**
 * RIL_REQUEST_SET_COLR
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_COLR  (RIL_REQUEST_EXT_BASE + 9)

/**
 * RIL_REQUEST_LOCK_INFO
 *
 * "data" is NULL
 *
 * (int *)response[0] is "PIN1" retry-left times
 *                [1] is "PIN2" retry-left times
 *                [2] is "PUK1" retry-left times
 *                [3] is "PUK2" retry-left times
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_LOCK_INFO (RIL_REQUEST_EXT_BASE + 10)

/**
 * "data" in NULL
 * "response" is char*
 */
#define RIL_REQUEST_SIM_GET_ATR (RIL_REQUEST_EXT_BASE + 11)

/**
 * RIL_REQUEST_GET_CP_PHONEBOOK_STATUS
 *
 * Request CP phonebook state
 *
 * "data" is NULL
 *
 * "response" is int *
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_CP_PHONEBOOK_STATUS (RIL_REQUEST_EXT_BASE + 12)

/**
 *  	RIL_REQUEST_SETUP_DATA_CALL_DEDICATED_BEARER
 *
 * setup dedicted bearer
 *
 * "data" is NULL
 *
 * "response" is int *
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define  RIL_REQUEST_SETUP_DATA_CALL_DEDICATED_BEARER (RIL_REQUEST_EXT_BASE + 13)

/**
 *  	 	RIL_REQUEST_CONFIGURE_TFT
 *
 * setup dedicted bearer
 *
 * "data" is NULL
 *
 * "response" is int *
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define  RIL_REQUEST_CONFIGURE_TFT  (RIL_REQUEST_EXT_BASE + 14)

/**
 * RIL_REQUEST_SET_CSCS
 *
 * Set DTE character set
 *
 * "data" is char *
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CSCS (RIL_REQUEST_EXT_BASE + 15)

/**
 * RIL_REQUEST_GET_SIM_CONTACT
 *
 * Get contact from sim by index
 *
 * "data" is int *
 *
 * "response" is contact info
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_SIM_CONTACT (RIL_REQUEST_EXT_BASE + 16)

/**
 * RIL_REQUEST_ADD_CONTACT_IN_SIM
 *
 * Add new contact in sim
 *
 * "data" is contact info
 *
 * "response" is int (contact index in sim)
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ADD_CONTACT_IN_SIM (RIL_REQUEST_EXT_BASE + 17)

/**
 * RIL_REQUEST_DELETE_CONTACT_IN_SIM
 *
 * Delete contact in sim
 *
 * "data" is int
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DELETE_CONTACT_IN_SIM (RIL_REQUEST_EXT_BASE + 18)

/**
 * RIL_REQUEST_UPDATE_CONTACT_IN_SIM
 *
 * Delete contact in sim
 *
 * "data" is contact info
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_UPDATE_CONTACT_IN_SIM (RIL_REQUEST_EXT_BASE + 19)

/**
 * RIL_REQUEST_SET_PS_ATTACH_STATUS
 *
 * set  PS attach status
 *
 * "data" is int *
 * ((int *)data)[0] is 0 for "set to detach status"
 *                      and 1 for "set to attach status"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_PS_ATTACH_STATUS (RIL_REQUEST_EXT_BASE + 20)

/**
 * RIL_REQUEST_GET_PS_ATTACH_STATUS
 *
 * get  PS attach status
 *
 * "data" is NULL
 *
 * "response" is int*
 *       1    --   attached
 *       0    --   detached
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_PS_ATTACH_STATUS (RIL_REQUEST_EXT_BASE + 21)
/**
 *  RIL_REQUEST_SET_EMBMS
 *
 *  send embms command to modem
 *
 * "data" is string
 *
 * "response" is string
 *
 *  Valid errors:
 *
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SET_EMBMS (RIL_REQUEST_EXT_BASE + 22)
/**
 *  RIL_REQUEST_CLEAR_ETIFLAG
 *
 *  send embms command to modem
 *
 * "data" is void
 *
 * "response" is void
 *
 *  Valid errors:
 *
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */

#define RIL_REQUEST_CLEAR_ETIFLAG  (RIL_REQUEST_EXT_BASE + 23)

/**
 * RIL_REQUEST_MEP_LOCK_INFO
 *
 * "data" is NULL
 *
 * (int *)response[0]  is "MEP SIM" retry-left times
 *		     [1] is "MEP Network" retry-left times
 *		     [2] is "MEP SubNetwork" retry-left times
 *		     [3] is "MEP Service Provide" retry-left times
 *		     [4] is "MEP Corporate" retry-left times
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_MEP_LOCK_INFO (RIL_REQUEST_EXT_BASE + 24)

/**
 * RIL_REQUEST_GET_UPLMN
 *
 * Queries the UPLMN list
 *
 * "response" is char **
 * ((char **)response)[i] indicate for "index"
 * ((char **)response)[i+1] indicate for "mccmnc"
 * ((char **)response)[i+1] indicate for "act"
 * ((char **)response)[n-1] indicate for "LTE/non-LTE mode"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_UPLMN (RIL_REQUEST_EXT_BASE + 25)

/**
 *  RIL_REQUEST_CANCEL_PLMN_SEARCH
 *
 *  Tells the modem to cancel the plmn search
 *
 * "data" is NULL
 *
 * "response" is NULL
 *
 *  Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CANCEL_PLMN_SEARCH (RIL_REQUEST_EXT_BASE + 26)

/**
 * RIL_REQUEST_SET_LTE_BGSCAN_TIME
 *
 * "data" is char**
 *      data[0] is interval
 *      data[1] is state
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_LTE_BGSCAN_TIME (RIL_REQUEST_EXT_BASE + 27)

/**
 * RIL_REQUEST_GET_LTE_BGSCAN_TIME
 *
 * "data" is NULL
 *
 * " (int *)response[0] is sate
 *                            [1] is interval
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_LTE_BGSCAN_TIME (RIL_REQUEST_EXT_BASE + 28)

/*
 *RIL_REQUEST_SEND_ATCMD
 *Send AT cmd by this ril request
 *"data" is char *
 *response is char *
 */
#define RIL_REQUEST_SEND_ATCMD  (RIL_REQUEST_EXT_BASE + 29)

#define RIL_REQUEST_SET_EEMOPT  (RIL_REQUEST_EXT_BASE + 30)

#define RIL_REQUEST_GET_EEMGINFO (RIL_REQUEST_EXT_BASE + 31)

#define RIL_REQUEST_SET_MODEM_VERSION (RIL_REQUEST_EXT_BASE + 32)

#define RIL_REQUEST_SET_CPPOWERIND  (RIL_REQUEST_EXT_BASE + 33)

#define RIL_REQUEST_SET_PROACTIVECMD    (RIL_REQUEST_EXT_BASE + 34)

#define RIL_REQUEST_GET_CID_STATE   (RIL_REQUEST_EXT_BASE + 35)

#define RIL_REQUEST_GET_PDPIPAPN    (RIL_REQUEST_EXT_BASE + 36)

#define RIL_REQUEST_GET_CFUN    (RIL_REQUEST_EXT_BASE + 37)

#define RIL_REQUEST_SET_CFUN    (RIL_REQUEST_EXT_BASE + 38)

#define RIL_REQUEST_SET_CPBS    (RIL_REQUEST_EXT_BASE + 39)

#define RIL_REQUEST_GET_MPBK    (RIL_REQUEST_EXT_BASE + 40)

#define RIL_REQUEST_SET_PARKMODE    (RIL_REQUEST_EXT_BASE + 41)

#define RIL_REQUEST_CHECK_EMERGENCYNUM  (RIL_REQUEST_EXT_BASE + 42)

#define RIL_REQUEST_SET_CNMI  (RIL_REQUEST_EXT_BASE + 43)

#define RIL_REQUEST_DIAL_EMERGENCY  (RIL_REQUEST_EXT_BASE + 44)

#define RIL_REQUEST_GET_CGMR  (RIL_REQUEST_EXT_BASE + 45)

#define RIL_REQUEST_HOLD_ACTIVE_CALL  (RIL_REQUEST_EXT_BASE + 46)

#define RIL_REQUEST_RESET_MODEM  (RIL_REQUEST_EXT_BASE + 47)

#define RIL_REQUEST_SET_CECALL  (RIL_REQUEST_EXT_BASE + 48) 

#define RIL_REQUEST_GET_CECALL  (RIL_REQUEST_EXT_BASE + 49)

/**
 * RIL_REQUEST_DATA_REGISTRATION_STATE
 *
 * Request current DATA registration state
 *
 * "data" is NULL
 * "response" is a "char **"
 * ((const char **)response)[0] is registration state 0-5 from TS 27.007 10.1.20 AT+CGREG
 * ((const char **)response)[1] is LAC if registered or NULL if not
 * ((const char **)response)[2] is CID if registered or NULL if not
 * ((const char **)response)[3] indicates the available data radio technology,
 *                              valid values as defined by RIL_RadioTechnology.
 * ((const char **)response)[4] if registration state is 3 (Registration
 *                               denied) this is an enumerated reason why
 *                               registration was denied.  See 3GPP TS 24.008,
 *                               Annex G.6 "Additonal cause codes for GMM".
 *      7 == GPRS services not allowed
 *      8 == GPRS services and non-GPRS services not allowed
 *      9 == MS identity cannot be derived by the network
 *      10 == Implicitly detached
 *      14 == GPRS services not allowed in this PLMN
 *      16 == MSC temporarily not reachable
 *      40 == No PDP context activated
 * ((const char **)response)[5] The maximum number of simultaneous Data Calls that can be
 *                              established using RIL_REQUEST_SETUP_DATA_CALL.
 *
 * The values at offsets 6..10 are optional LTE location information in decimal.
 * If a value is unknown that value may be NULL. If all values are NULL,
 * none need to be present.
 *  ((const char **)response)[6] is TAC, a 16-bit Tracking Area Code.
 *  ((const char **)response)[7] is CID, a 0-503 Physical Cell Identifier.
 *  ((const char **)response)[8] is ECI, a 28-bit E-UTRAN Cell Identifier.
 *  ((const char **)response)[9] is CSGID, a 27-bit Closed Subscriber Group Identity.
 *  ((const char **)response)[10] is TADV, a 6-bit timing advance value.
 *
 * LAC and CID are in hexadecimal format.
 * valid LAC are 0x0000 - 0xffff
 * valid CID are 0x00000000 - 0x0fffffff
 *
 * Please note that registration state 4 ("unknown") is treated
 * as "out of service" in the Android telephony system
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DATA_REGISTRATION_STATE_NOCACHE (RIL_REQUEST_EXT_BASE + 50)

#define RIL_REQUEST_GET_PARKMODE (RIL_REQUEST_EXT_BASE + 51)

#define RIL_REQUEST_SET_ECALLCFG (RIL_REQUEST_EXT_BASE + 52)

#define RIL_REQUEST_GET_ECALLCFG (RIL_REQUEST_EXT_BASE + 53)

#define RIL_REQUEST_SET_ECALLONLY (RIL_REQUEST_EXT_BASE + 54)

#define RIL_REQUEST_GET_ECALLONLY (RIL_REQUEST_EXT_BASE + 55)

#define RIL_REQUEST_SET_ECALLREG (RIL_REQUEST_EXT_BASE + 56)

/**
 * RIL_REQUEST_RELEASE_CALL
 *
 * call ATH
 *
 * "data" is NULL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_RELEASE_CALL (RIL_REQUEST_EXT_BASE + 57)

/**
 * RIL_REQUEST_CALL_HANDUP
 *
 * call AT+CHUP
 *
 * "data" is NULL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CALL_HANDUP (RIL_REQUEST_EXT_BASE + 58)

#define RIL_REQUEST_IMS_FUNC_ENABLE (RIL_REQUEST_EXT_BASE + 59)

#define RIL_REQUEST_IMS_FUNC_STAT_QUERY (RIL_REQUEST_EXT_BASE + 60)

#define RIL_REQUEST_ZFACTORY (RIL_REQUEST_EXT_BASE + 61)

#define RIL_REQUEST_REPORT_IP_DNS (RIL_REQUEST_EXT_BASE + 62)

#define RIL_REQUEST_GET_ZDOGREF (RIL_REQUEST_EXT_BASE + 63)

#define RIL_REQUEST_GET_ZATNUM (RIL_REQUEST_EXT_BASE + 64)

#define RIL_REQUEST_GET_REG_STATE (RIL_REQUEST_EXT_BASE + 65)

#define RIL_REQUEST_GET_OPERATOR (RIL_REQUEST_EXT_BASE + 66)

#define RIL_REQUEST_SET_NWSELMODE (RIL_REQUEST_EXT_BASE + 67)

#define RIL_REQUEST_GET_NWSELMODE (RIL_REQUEST_EXT_BASE + 68)

#define RIL_REQUEST_GET_NW_DOMAIN (RIL_REQUEST_EXT_BASE + 69)

#define RIL_REQUEST_GET_CELLINFO (RIL_REQUEST_EXT_BASE + 70)

#define RIL_REQUEST_GET_ZRSSI (RIL_REQUEST_EXT_BASE + 71)

#define RIL_REQUEST_GET_LTECAINFO (RIL_REQUEST_EXT_BASE + 72)

#define RIL_REQUEST_GET_BANDMODE (RIL_REQUEST_EXT_BASE + 73)

#define RIL_REQUEST_SET_BANDMODE (RIL_REQUEST_EXT_BASE + 74)

#define RIL_REQUEST_GET_NITZ (RIL_REQUEST_EXT_BASE + 75)

#define RIL_REQUEST_GET_ZSEC (RIL_REQUEST_EXT_BASE + 76)

#define RIL_REQUEST_GET_ZNCK (RIL_REQUEST_EXT_BASE + 77)

#define RIL_REQUEST_SET_ZNCK (RIL_REQUEST_EXT_BASE + 78)

#define RIL_REQUEST_SET_IMEISV (RIL_REQUEST_EXT_BASE + 79)

#define RIL_REQUEST_SET_CPMS (RIL_REQUEST_EXT_BASE + 80)

#define RIL_REQUEST_GET_CPMS (RIL_REQUEST_EXT_BASE + 81)

#define RIL_REQUEST_GET_CMGD (RIL_REQUEST_EXT_BASE + 82)

#define RIL_REQUEST_GET_CMGR (RIL_REQUEST_EXT_BASE + 83)

#define RIL_REQUEST_GET_CPBC (RIL_REQUEST_EXT_BASE + 84)

#define RIL_REQUEST_GET_ZROAM (RIL_REQUEST_EXT_BASE + 85)

#define RIL_REQUEST_SET_ZCFG (RIL_REQUEST_EXT_BASE + 86)

#define RIL_REQUEST_GET_ZCFGBYID (RIL_REQUEST_EXT_BASE + 87)

#define RIL_REQUEST_GET_CNUM (RIL_REQUEST_EXT_BASE + 88)

#define RIL_REQUEST_GET_ETIFLAG (RIL_REQUEST_EXT_BASE + 89)

#define RIL_REQUEST_GET_MRDIMEI (RIL_REQUEST_EXT_BASE + 90)

#define RIL_REQUEST_SET_MRDIMEI (RIL_REQUEST_EXT_BASE + 91)

#define RIL_REQUEST_GET_MRDWIFIMAC (RIL_REQUEST_EXT_BASE + 92)

#define RIL_REQUEST_SET_MRDWIFIMAC (RIL_REQUEST_EXT_BASE + 93)

#define RIL_REQUEST_DEL_ALLSIMPB (RIL_REQUEST_EXT_BASE + 94)

#define RIL_REQUEST_ZFACTORY_EXT (RIL_REQUEST_EXT_BASE + 95)

#define RIL_REQUEST_SETUP_DATA_CALL_BYCID (RIL_REQUEST_EXT_BASE + 96)

#define RIL_REQUEST_GET_LOCKLEVEL	(RIL_REQUEST_EXT_BASE + 97)

#define RIL_REQUEST_SET_NETWORK_DEREGISTER (RIL_REQUEST_EXT_BASE + 98)

#define RIL_REQUEST_GET_SMSSTATUS (RIL_REQUEST_EXT_BASE + 99)

#define RIL_REQUEST_REPORT_ZGDCONT (RIL_REQUEST_EXT_BASE + 100)
// change this definitions when adding more external ril requestes
#define RIL_REQUEST_EXT_LAST  (RIL_REQUEST_REPORT_ZGDCONT)

/***********************************************************************/

#define RIL_UNSOL_RESPONSE_EXT_BASE 1500
/**
 * RIL_UNSOL_STK_CALL_SETUP_STATUS
 *
 * Indicate When SIM wants application to know the call setup status
 *
 * "data" is a const char * containing SAT/USAT Call Setup Status
 * in hexadecimal format string
 */
#define RIL_UNSOL_STK_CALL_SETUP_STATUS (RIL_UNSOL_RESPONSE_EXT_BASE + 0)

/**
 * RIL_UNSOL_STK_CALL_SETUP_RESULT
 *
 * Indicate When CP wants application to know the call setup result
 *
 * "data" is a const char * containing SAT/USAT Call Setup Result
 * in hexadecimal format string
 */
#define RIL_UNSOL_STK_CALL_SETUP_RESULT (RIL_UNSOL_RESPONSE_EXT_BASE + 1)

/**
 * RIL_UNSOL_STK_SEND_SM_STATUS
 *
 * Indicate When SIM wants application to know the send sm status
 *
 * "data" is const char * containing SAT/USAT Send Sm Status
 * in hexadecimal format string
 */
#define RIL_UNSOL_STK_SEND_SM_STATUS (RIL_UNSOL_RESPONSE_EXT_BASE + 2)

/**
 * RIL_UNSOL_STK_SEND_SM_RESULT
 *
 * Indicate When CP wants application to know the send sm result
 *
 * "data" is const char * containing SAT/USAT Send Sm Result
 * in hexadecimal format string
 */
#define RIL_UNSOL_STK_SEND_SM_RESULT (RIL_UNSOL_RESPONSE_EXT_BASE + 3)

/**
 * RIL_UNSOL_STK_SEND_USSD__RESULT
 *
 * Indicate When CP wants application to know the send ussd result
 *
 * "data" is const  char * containing SAT/USAT Send Ussd Result
 * in hexadecimal format string
 */
#define RIL_UNSOL_STK_SEND_USSD_RESULT (RIL_UNSOL_RESPONSE_EXT_BASE + 4)

/**
 * RIL_UNSOL_CP_PHONEBOOK_INITED
 *
 * Indicate When phone book has been loaded by CP
 *
 * "data" is NULL
 *
 */
#define RIL_UNSOL_CP_PHONEBOOK_INITED (RIL_UNSOL_RESPONSE_EXT_BASE + 5)
/**
 * RIL_UNSOL_VZW_PCO_ACTION
 *
 * VZW is reserving to use "Container Identifier - FF00H" for subscription status
 *
 * "data" is "data" is int *
 * ((int *)data)[0] is action
 */
#define RIL_UNSOL_VZW_PCO_ACTION (RIL_UNSOL_RESPONSE_EXT_BASE + 6)

/**
 * RIL_UNSOL_SIM_HOT_PLUG_EVENT
 *
 * Indicate When detect SIM hot plug event
 *
 * "data" is int *
 * ((int *)data)[0] is indicate plug event type
 *                        0: sim plugin
 *                        1: sim plugout
 *                        2: sim tray plugin
 *                        3: sim tray plugout
 */
#define RIL_UNSOL_SIM_HOT_PLUG_EVENT (RIL_UNSOL_RESPONSE_EXT_BASE + 7)
/**
 * RIL_UNSOL_PDP_REJECT_CAUSE
 *
 * Indicate When pdp is rejected by network
 *
 * "data" is int *
 * ((int *)data)[0] is smcause

 *
 */

#define RIL_UNSOL_PDP_REJECT_CAUSE (RIL_UNSOL_RESPONSE_EXT_BASE + 8)

/**
 * RIL_UNSOL_PS_ATTACH_STATUS
 *
 * Indicate When packet service is attached or detached
 *
 * "data" is const char * containing attach/detach status
 * in decimal format string 

 *
 */
#define RIL_UNSOL_PS_ATTACH_STATUS (RIL_UNSOL_RESPONSE_EXT_BASE + 9)
/**
 * RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED_EXT
 *
 * Indicates a call status changed with informaition
 * on new call status
 *
 * "data" is const RIL_Call * containing call information
 *
 *
 */
#define RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED_EXT (RIL_UNSOL_RESPONSE_EXT_BASE + 10)

/**
 * RIL_UNSOL_CALL_NO_CARRIER_EXT
 *
 * Indicate when call status is NO CARRIER (e.g. when call ends)
 *
 * "data" is NULL
 *
 */
#define RIL_UNSOL_CALL_NO_CARRIER_EXT (RIL_UNSOL_RESPONSE_EXT_BASE + 11)

#define RIL_UNSOL_EEM_MODESTATE (RIL_UNSOL_RESPONSE_EXT_BASE + 12)

#define RIL_UNSOL_EEM_INFOSVC   (RIL_UNSOL_RESPONSE_EXT_BASE + 13)

#define RIL_UNSOL_EEM_INFOPS    (RIL_UNSOL_RESPONSE_EXT_BASE + 14)

#define RIL_UNSOL_EEM_INFONC    (RIL_UNSOL_RESPONSE_EXT_BASE + 15)

#define RIL_UNSOL_EEM_INBFTM    (RIL_UNSOL_RESPONSE_EXT_BASE + 16)

#define RIL_UNSOL_EEM_INFOGMM   (RIL_UNSOL_RESPONSE_EXT_BASE + 17)

#define RIL_UNSOL_EEM_LTESVC    (RIL_UNSOL_RESPONSE_EXT_BASE + 18)

#define RIL_UNSOL_EEM_LTEINTRA  (RIL_UNSOL_RESPONSE_EXT_BASE + 19)

#define RIL_UNSOL_EEM_LTEINTER  (RIL_UNSOL_RESPONSE_EXT_BASE + 20)

#define RIL_UNSOL_EEM_UMTSINTER  (RIL_UNSOL_RESPONSE_EXT_BASE + 21)

#define RIL_UNSOL_EEM_UMTSINTRA  (RIL_UNSOL_RESPONSE_EXT_BASE + 22)

#define RIL_UNSOL_EEM_UMTSINTERRAT  (RIL_UNSOL_RESPONSE_EXT_BASE + 23)

#define RIL_UNSOL_EEM_UMTSSVC   (RIL_UNSOL_RESPONSE_EXT_BASE + 24)

#define RIL_UNSOL_PDP_DEACTIVE_CID   (RIL_UNSOL_RESPONSE_EXT_BASE + 25)

#define RIL_UNSOL_SMS_MEM_STATUS   (RIL_UNSOL_RESPONSE_EXT_BASE + 26)

/**
 * RIL_UNSOL_SUSPEND_RESUME
 *
 * Indicate data status: 1 is suspended, 0 is resume
 *
 * "data" is int *
 * ((int *)data)[0] is suspend/resume flag
*/
#define RIL_UNSOL_SUSPEND_RESUME   (RIL_UNSOL_RESPONSE_EXT_BASE + 27)

#define RIL_UNSOL_ECALLDATA   (RIL_UNSOL_RESPONSE_EXT_BASE + 28)

#define RIL_UNSOL_ECALLVOICE   (RIL_UNSOL_RESPONSE_EXT_BASE + 29)

#define RIL_UNSOL_CALL_STATUS_INFO   (RIL_UNSOL_RESPONSE_EXT_BASE + 30)

#define RIL_UNSOL_CON_STAT   (RIL_UNSOL_RESPONSE_EXT_BASE + 31)

#define RIL_UNSOL_ZDOGREF   (RIL_UNSOL_RESPONSE_EXT_BASE + 32)

#define RIL_UNSOL_BANDIND   (RIL_UNSOL_RESPONSE_EXT_BASE + 33)

#define RIL_UNSOL_CPAS   (RIL_UNSOL_RESPONSE_EXT_BASE + 34)

#define RIL_UNSOL_ZNITZ   (RIL_UNSOL_RESPONSE_EXT_BASE + 35)

#define RIL_UNSOL_NEW_SMS_EXT	(RIL_UNSOL_RESPONSE_EXT_BASE + 36)

#define RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT_EXT (RIL_UNSOL_RESPONSE_EXT_BASE + 37)

#define RIL_UNSOL_DISCONNECT_CALLID (RIL_UNSOL_RESPONSE_EXT_BASE + 38)

#define RIL_UNSOL_RSAP_CONN_REQ_IND 		(RIL_UNSOL_RESPONSE_EXT_BASE + 39)
#define RIL_UNSOL_RSAP_DISCONN_REQ_IND 		(RIL_UNSOL_RESPONSE_EXT_BASE + 40)
#define RIL_UNSOL_RSAP_GET_ATR_IND 			(RIL_UNSOL_RESPONSE_EXT_BASE + 41)
#define RIL_UNSOL_RSAP_GET_STATUS_REQ_IND 	(RIL_UNSOL_RESPONSE_EXT_BASE + 42)
#define RIL_UNSOL_RSAP_SET_TRAN_P_REQ_IND 	(RIL_UNSOL_RESPONSE_EXT_BASE + 43)
#define RIL_UNSOL_RSAP_SIM_CONTROL_REQ_IND 	(RIL_UNSOL_RESPONSE_EXT_BASE + 44)
#define RIL_UNSOL_RSAP_STATUS_IND 			(RIL_UNSOL_RESPONSE_EXT_BASE + 45)
#define RIL_UNSOL_RSAP_TRANSFER_APDU_IND 	(RIL_UNSOL_RESPONSE_EXT_BASE + 46)

/**
 * RIL_UNSOL_CP_ASSERT
 *
 * Indicate when cp assert
 *
 * "data" is NULL
 */
#define RIL_UNSOL_CP_ASSERT				    (RIL_UNSOL_RESPONSE_EXT_BASE + 47)

#endif
