#ifndef _LBSEAT_H
#define _LBSEAT_H
#include "simcom_sdk_common.h"

#define SIMCOM_LBS_DETAILADDR_LEN 150
#define SIMCOM_LBS_DATAANDTIME_LEN 40
#define SIMCOM_LBS_SERVER_ADDR "lbs-simcom.com"
#define SIMCOM_LBS_SERVER_PORT 3002



/**
    Define CellID select type, sCellID or pCellID. 
*/

typedef enum
{
    sCellID = 0x00,
    pCellID = 0x01,
} LBS_CellID_select;

/**
    Define radio_if_type_enum that should agree with radio_if_type_enum. 
*/
typedef enum
{
    LBS_RADIO_IF_NO_SVC = 0x00,
    LBS_RADIO_IF_CDMA_1X = 0x01,
    LBS_RADIO_IF_CDMA_1XEVDO = 0x02,
    LBS_RADIO_IF_AMPS = 0x03,
    LBS_RADIO_IF_GSM = 0x04,
    LBS_RADIO_IF_UMTS = 0x05,
    LBS_RADIO_IF_LTE = 0x08,
    LBS_RADIO_IF_TDSCDMA = 0x09,
    LBS_RADIO_IF_1XLTE = 0x0A
} LBS_radio_if_type_enum;

/**
    Define LBS node message ID enum for communicating with lbsServer 
*/
typedef enum
{
    SIMCOM_LBS_MSG_NODE_SEQUENCENUM = 0x01,
    SIMCOM_LBS_MSG_NODE_IMSI = 0x02,
    SIMCOM_LBS_MSG_NODE_NETMODE = 0x03,
    SIMCOM_LBS_MSG_NODE_MCC_MNC = 0x04,
    SIMCOM_LBS_MSG_NODE_CELL_INFO = 0x05,
    SIMCOM_LBS_MSG_NODE_CLIENT_ID = 0x06,
    SIMCOM_LBS_MSG_NODE_SOFTWARE_VER = 0x07,
    SIMCOM_LBS_MSG_NODE_REQ_SVR_TYPE = 0x08,
    SIMCOM_LBS_MSG_NODE_CTRL_SRV = 0x09,
    SIMCOM_LBS_MSG_NODE_CELL32_INFO = 0x0a,
    SIMCOM_LBS_MSG_NODE_CELL_INFO_CDMA = 0x0b,

    SIMCOM_LBS_MSG_NODE_ERROR = 0x80,
    SIMCOM_LBS_MSG_NODE_LONGITUDE_LATITUDE = 0x81,
    SIMCOM_LBS_MSG_NODE_ACCURACY = 0x82,
    SIMCOM_LBS_MSG_NODE_LOC_ADDRESS = 0x83,
    SIMCOM_LBS_MSG_NODE_SERVER_TIME = 0x84,
    SIMCOM_LBS_MSG_NODE_LONGITUDE_LATITUDE_TYPE = 0x85,
    SIMCOM_LBS_MSG_NODE_QUERY_AMOUNT = 0x86
} SIMCOM_LBS_MSG_NODE_TYPE;

/**
    Define LBS Errorcode enum 
*/
typedef enum
{
    SIMCOM_LBS_CODE_SUCC_ERROR = 0,
    SIMCOM_LBS_CODE_PARA_ERROR = 1,
    SIMCOM_LBS_CODE_SERVICE_OUT_TIME_ERROR = 2,
    SIMCOM_LBS_CODE_NOT_FIND_ERROR = 3,
    SIMCOM_LBS_CODE_TIME_OUT_ERROR = 4,
    SIMCOM_LBS_CODE_CERTIFICATION_FAILED_ERROR = 5,
    SIMCOM_LBS_CODE_SERVER_LBS_SUCCESS = 6,
    SIMCOM_LBS_CODE_SERVER_LBS_FAIL = 7,

    /*ADD FOR USER*/
    SIMCOM_LBS_CODE_BUSING = 8,
    SIMCOM_LBS_CODE_NETWORK_OPEN_FAILED = 9,
    SIMCOM_LBS_CODE_NETWORK_CLOSE_FAILED = 10,
    SIMCOM_LBS_CODE_TIMEOUT = 11,
    SIMCOM_LBS_CODE_DNS_ERROR = 12,
    SIMCOM_LBS_CODE_SOCKET_CREAT_FAILED = 13,
    SIMCOM_LBS_CODE_SOCKET_CONNECT_FAILED = 14,
    SIMCOM_LBS_CODE_SOCKET_CLOSE_FAILED = 15,
    SIMCOM_LBS_CODE_GET_CELLID_FAILED = 16,
    SIMCOM_LBS_CODE_GET_IMEI_FAILED = 17,
    SIMCOM_LBS_CODE_SEND_DATA_FAILED = 18,
    SIMCOM_LBS_CODE_RECEIVE_DATA_FAILED = 19,
    SIMCOM_LBS_CODE_NONET = 20,
    SIMCOM_LBS_CODE_NET_NOTOPEN = 21,
    /*END*/

    SIMCOM_LBS_CODE_LBS_ERROR_SUCCESS = 80,
    SIMCOM_LBS_CODE_LBS_PARAMETER_ERR = 81,
    SIMCOM_LBS_CODE_LBS_ERROR_FAILED = 82,

    SIMCOM_LBS_CODE_OTHER_ERROR = 110,
    SIMCOM_LBS_CODE_DEFUALT_VALUE = 0xFF
} SIMCOM_LBS_CODE_TYPE;

/**
    Define LBS operation code enum 
*/
typedef enum
{
    SIMCOM_LBS_OP_LON_LAT = 1,
    SIMCOM_LBS_OP_DETAIL_ADDR,
    SIMCOM_LBS_OP_ACCESS_TIMES,
    SIMCOM_LBS_OP_LON_LAT_DATA_TIME,
    SIMCOM_LBS_OP_GET_SERVER_CONFIG,
    SIMCOM_LBS_OP_SET_SERVER_CONFIG,
    SIMCOM_LBS_OP_REPOT_POSITION_ERROR = 9
} SIMCOM_LBS_OP;

/**
    Define REQUEST type enum
*/
typedef enum
{
    SIMCOM_LBS_REQUEST_LON_LAC_ACC = 0x01,
    SIMCOM_LBS_REQUEST_ADDRESS = 0x02,
    SIMCOM_LBS_REQUEST_SERVERTIME = 0x04,
    SIMCOM_LBS_REQUEST_QUERY_AMOUNT = 0x08
} SIMCOM_LBS_REQUEST_TYPE;

/**
    Define net mode enum  
*/
typedef enum
{
    SIMCOM_LBS_NET_MODE_GSM = 0,
    //SIMCOM_LBS_NET_MODE_CDMA                           = 1,
    SIMCOM_LBS_NET_MODE_WCDMA = 2,
    // SIMCOM_LBS_NET_MODE_TD                           = 3,
    //SIMCOM_LBS_NET_MODE_LTE                          = 4,
    SIMCOM_LBS_NET_MODE_MAX
} SIMCOM_LBS_NET_MODE_TYPE;

#pragma pack(1)
/**
    Define struct simcom_lbs_head_t  
*/
typedef struct
{
    uint32_t u32TotalLen; ///<Total_Length - uint32_t
    uint8_t u8Version[6]; ///<ProtocolVersion - 6 byte
} simcom_lbs_head_t;

/**
    Define struct simcom_lbs_sequ_id_t  
*/
typedef struct
{
    uint8_t u8Len;      ///<length - 1 byte
    uint8_t u8Type;     ///<type - 1 byte
    uint32_t u32SequId; ///<sequence_id - 4 byte
} simcom_lbs_sequ_id_t;

/**
    Define struct simcom_lbs_imsi_t  
*/
typedef struct
{
    uint8_t u8Len;      ///<length - 1 byte
    uint8_t u8Type;     ///<type - 1 byte
    uint8_t u8Imsi[15]; ///<imsi - 15 byte
} simcom_lbs_imsi_t;

/**
    Define struct simcom_lbs_network_type_t  
*/
typedef struct
{
    uint8_t u8Len;     ///<length - 1 byte
    uint8_t u8Type;    ///<type - 1 byte
    uint8_t u8NetType; ///<rt - 1 byte
} simcom_lbs_network_type_t;

/**
    Define struct simcom_lbs_mcc_mnc_t  
*/
typedef struct
{
    uint8_t u8Len;    ///<length - 1 byte
    uint8_t u8Type;   ///<type - 1 byte
    uint8_t u8Mcc[3]; ///<mcc - 3 byte
    uint8_t u8Mnc[3]; ///<mnc - 3 byte
} simcom_lbs_mcc_mnc_t;

/**
    Define struct simcom_lbs_request_type_t  
*/
typedef struct
{
    uint8_t u8Len;           ///<length - 1 byte
    uint8_t u8Type;          ///<type - 1 byte
    uint8_t u8RequestAction; ///<action - 1 byte
} simcom_lbs_request_type_t;

/**
    Define struct simcom_lbs_ReSer_Action_t  
*/
typedef struct
{
    uint8_t u8Len;    ///<length - 1 byte
    uint8_t u8Type;   ///<type - 1 byte
    uint8_t u8Action; ///<action - 1 byte
} simcom_lbs_ReSer_Action_t;

/**
    Define struct simcom_lbs_cell_id_t  
*/
typedef struct
{
    uint8_t u8Len;   ///<length - 1 byte
    uint8_t u8Type;  ///<type - 1 byte
    uint16_t u16Lac; ///<lac - 2 byte
    //uint16_t u16CellId;                     ///<cellid - 2 byte
    uint32_t u32CellId;  ///<cellid - 4 byte   for LTE, WCDMA
    signed short u16Dbm; ///<dbm - 2 byte
} simcom_lbs_cell_id_t;

/**
    Define struct simcom_lbs_cellid_lac_t  
*/
typedef struct
{
    uint8_t u8Len;      ///<length - 1 byte
    uint8_t u8Type;     ///<type - 1 byte
    uint16_t u16Lac;    ///<lac - 2 byte
    uint16_t u16CellId; ///<cellid - 2 byte
    uint16_t u16Dbm;    ///<dbm - 2 byte
} simcom_lbs_cellid_lac_t;

/**
    Define struct simcom_lbs_cell_id_cdma_t  
*/
typedef struct
{
    uint8_t u8Len;       ///<length - 1 byte
    uint8_t u8Type;      ///<type - 1 byte
    uint16_t u16SID;     ///<SID - 2 byte
    uint16_t u16NID;     ///<NID - 2 byte
    uint16_t u16BID;     ///<BID - 2 byte
    signed short u16Dbm; ///<dbm - 2 byte
} simcom_lbs_cell_id_cdma_t;

/**
    Define struct simcom_lbs_gid_t  
*/
typedef struct
{
    uint8_t u8Len;    ///<length - 1 byte
    uint8_t u8Type;   ///<type - 1 byte
    uint8_t u8Gid[6]; ///<gid - 6 byte
} simcom_lbs_gid_t;

/**
    Define struct simcom_lbs_LngLatType_t  
*/
typedef struct
{
    uint8_t u8Len;         // length - 1 byte
    uint8_t u8Type;        // type - 1 byte
    uint8_t u32LngLatType; //lng - 1 byte
} simcom_lbs_LngLatType_t;

typedef struct
{
    uint8_t u8Len;  // length - 1 byte
    uint8_t u8Type; // type - 1 byte
    //double u32Lng;     //lng - 4 byte
    //double u32Lat;     //lat - 4 byte
    int32_t u32Lng; //lng - 4 byte
    int32_t u32Lat; //lat - 4 byte
} simcom_lbs_LngLat_t;

typedef struct
{
    uint8_t ver[6];                                    ///<version -6 byte
    uint32_t u32SequId;                                ///<sequence_id - 4 byte
    uint8_t u8ErrorCode;                               ///<ret - 1 byte
    uint32_t u32Lng;                                   ///<lng - 4 byte
    uint32_t u32Lat;                                   ///<lat - 4 byte
    uint16_t u16Acc;                                   ///<acc - 2 byte
    uint8_t u8LocAddress[SIMCOM_LBS_DETAILADDR_LEN];   ///<addr - SIMCOM_LBS_DETAILADDR_LEN
    uint32_t u32AddrLen;                               ///<addrlen - 4 byte
    uint8_t u8DateAndTime[SIMCOM_LBS_DATAANDTIME_LEN]; ///<dataandtime - SIMCOM_LBS_DATAANDTIME_LEN
    uint32_t u32QueryAmount;                           ///<amout - 4 byte
} simcom_lbs_receive_info_t;

typedef struct
{
    char type;
    char cid;
    boolean lonLatvalid;
    double longitude;
    double latitude;
    boolean lontypevalid;
    char lon_type;
} simcom_lbs_input_info_t;

typedef enum CIDEV_ENGMODE_NETWORK_TAG
{
    CI_DEV_EM_NETWORK_GSM = 0, /**< GSM network */
    CI_DEV_EM_NETWORK_UMTS,    /**< UMTS network */
    CI_DEV_EM_NETWORK_LTE,     /**< LTE Network */

    /* This must be the last entry */
    CI_DEV_NUM_EM_NETWORKS
} _CiDevEngModeNetwork;

typedef struct
{
    uint8_t network_valid;
    _CiDevEngModeNetwork network;
    uint8_t current_plmn_valid;
    uint32_t mnc;
    uint32_t mcc;
    uint8_t lac_valid;
    uint16_t lac;
    uint8_t cell_id_valid;
    uint32_t CellID;
    uint8_t tac_valid;
    uint32_t tac;
    uint8_t rssi_valid;
    uint16_t rssi;
    uint8_t imei_valid;
    char imei[16];
} simcom_get_nas_info_t;

typedef struct
{
    float    fLongitude;                                //fLongitude
    float    fLatitude;                                 //fLatitude
    uint16_t u16accuracy;                               //positioning accuracy
    uint8_t  u8LocAddress[SIMCOM_LBS_DETAILADDR_LEN * 2];   //<addr
    uint32_t u32AddrLen;                                //<addrlen
    uint8_t  u8DateTime[SIMCOM_LBS_DATAANDTIME_LEN];    //<dataandtime
} simcom_lbs_info_t;


#pragma pack()


/*****************************************************************************
 * FUNCTION
 *  simcom_lbs_get_info
 *
 * DESCRIPTION
 *  simcom_lbs_get_info
 *  
 * PARAMETERS
 *  pLbsRevInfo: pointer of simcom_lbs_info_t
 *
 * RETURNS
 *  @SIMCOM_LBS_CODE_TYPE  
 *
 * NOTE
 *  
 *****************************************************************************/
int simcom_lbs_get_info(simcom_lbs_info_t *lbs_info);

#endif
