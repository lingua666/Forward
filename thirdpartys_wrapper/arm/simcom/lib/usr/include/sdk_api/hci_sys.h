/** 
 * @file    hci_sys.h 
 * @brief   HCI_SYS ����ͷ�ļ�  
 */  

#ifndef _hci_sys_header_
#define _hci_sys_header_

#ifdef __ANDROID__
#include <jni.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup HCI_SYS ����ϵͳAPI */
/* @{ */

/* parameter remark definition */
#ifndef _IN_
#define _IN_
#endif
#ifndef _OUT_
#define _OUT_
#endif
#ifndef _IN_OUT_
#define _IN_OUT_
#endif
#ifndef _MUST_
#define _MUST_
#endif
#ifndef _OPT_
#define _OPT_
#endif

#ifdef _WIN32
#define HCIAPI _stdcall
#else
#define HCIAPI
#endif

/** @defgroup HCI_SYS_STRUCT  �ṹ�� */
/* @{ */

/**
*@brief	����64λint
*/
#if !defined _MSC_VER && !defined __BORLANDC__
#  include <stdint.h>
#endif

#if defined _MSC_VER || defined __BORLANDC__
typedef __int64 int64;
#else
typedef int64_t int64;
#endif

/**
*@brief	ģ������
*/
#define SYS_MODULE    "HCI_SYS"

/**
*HCI_ERR_CODE  ���ش����붨��
*/
typedef enum {
	HCI_ERR_UNKNOWN = -1,					///< -1: δ֪����ͨ���������
	HCI_ERR_NONE = 0,						///< 0: ��ȷ
	HCI_ERR_PARAM_INVALID,					///< 1: �����Ĵ����������
	HCI_ERR_OUT_OF_MEMORY,					///< 2: �����ڴ�ʧ��
	HCI_ERR_CONFIG_INVALID,					///< 3: ���ô���������
	HCI_ERR_CONFIG_CAPKEY_MISSING,			///< 4: ȱ�ٱ����capKey������
	HCI_ERR_CONFIG_CAPKEY_NOT_MATCH,		///< 5: CAPKEY�뵱ǰ���治ƥ��
	HCI_ERR_CONFIG_DATAPATH_MISSING,		///< 6: ȱ�ٱ����dataPath������
	HCI_ERR_CONFIG_UNSUPPORT,				///< 7: �����֧��
	HCI_ERR_SERVICE_CONNECT_FAILED,			///< 8: ���ӷ�����ʧ�ܣ�����������Ӧ
	HCI_ERR_SERVICE_TIMEOUT,				///< 9: ���������ʳ�ʱ
	HCI_ERR_SERVICE_DATA_INVALID,			///< 10: ���������ص����ݸ�ʽ����ȷ
	HCI_ERR_SERVICE_RESPONSE_FAILED,		///< 11: ���������ز���ʧ��
	HCI_ERR_CAPKEY_NOT_FOUND,				///< 12: û���ҵ�ָ��������
	HCI_ERR_NOT_LOCAL_CAPKEY,				///< 13: ���Ǳ���������KEY
	HCI_ERR_LOCAL_LIB_MISSING,				///< 14: ������������ȱʧ��Ҫ�Ŀ���Դ
	HCI_ERR_URL_MISSING,					///< 15: �Ҳ�����Ӧ����������ַ��������HCI���������ַ��������Դ���ַ�ȣ�
	HCI_ERR_SESSION_INVALID,				///< 16: ��Ч�ĻỰ
	HCI_ERR_TOO_MANY_SESSION,				///< 17: �����Ự����(Ŀǰÿ��HCI���������Ự��Ϊ256)
	HCI_ERR_ACTIVE_SESSION_EXIST,			///< 18: ���лỰû��ֹͣ
	HCI_ERR_START_LOG_FAILED,				///< 19: ������־����, ��������־���ò�������·�������ڻ���û��дȨ�޵����
	HCI_ERR_DATA_SIZE_TOO_LARGE,			///< 20: ����������������ɴ��������
	HCI_ERR_LOAD_CODEC_DLL,					///< 21: ����codec�����ʧ��
	HCI_ERR_UNSUPPORT,						///< 22: �ݲ�֧��
	HCI_ERR_LOAD_FUNCTION_FROM_DLL,			///< 23: ���ؿ�ʧ��
    HCI_ERR_TXACCOUNT_NOT_FOUND,            ///< 24: �����˺Ż�ȡʧ��

	//HCI_SYS 
	HCI_ERR_SYS_NOT_INIT = 100,				///< 100: HCI_SYSδ��ʼ��
	HCI_ERR_SYS_ALREADY_INIT,				///< 101: HCI_SYS��γ�ʼ��
	HCI_ERR_SYS_CONFIG_AUTHPATH_MISSING,	///< 102: ȱ�ٱ����authPath������
	HCI_ERR_SYS_CONFIG_CLOUDURL_MISSING,	///< 103: ȱ�ٱ����cloudUrl������
	HCI_ERR_SYS_CONFIG_USERID_MISSING,		///< 104: ȱ�ٱ����userId������
	HCI_ERR_SYS_CONFIG_PASSWORD_MISSING,	///< 105: ȱ�ٱ����password������
	HCI_ERR_SYS_CONFIG_PLATFORMID_MISSING,	///< 106: ȱ�ٱ����platformId������
	HCI_ERR_SYS_CONFIG_DEVELOPERID_MISSING,	///< 107: ȱ�ٱ����developerId������
	HCI_ERR_SYS_CONFIG_DEVELOPERKEY_MISSING,///< 108: ȱ�ٱ����developerKey������
	HCI_ERR_SYS_CONFIG_APPNO_MISSING,		///< 109: ȱ�ٱ����appNo������
	HCI_ERR_SYS_USERINFO_INVALID,			///< 110: ��д�û���Ϣ�ļ�����
	HCI_ERR_SYS_AUTHFILE_INVALID,			///< 111: ��ȡ��Ȩ�ļ�����
	HCI_ERR_SYS_CHECKAUTH_RESPONSE_FAILED,	///< 112: ���������ػ�ȡ�ƶ���Ȩʧ��
	HCI_ERR_SYS_REGISTER_RESPONSE_FAILED,	///< 113: ����������ע���û�ʧ�ܣ��ѷ����������汾3.0��
	HCI_ERR_SYS_USING,						///< 114: ��Ȼ��������ʹ�ã���δ����ʼ����
	HCI_ERR_SYS_CONFIG_APPKEY_MISSING,      ///< 115: ȱ�ٱ����appkey������

	/* HCI_ASR */
	HCI_ERR_ASR_NOT_INIT = 200,				///< 200: HCI_ASRû�г�ʼ��
	HCI_ERR_ASR_ALREADY_INIT,				///< 201: HCI_ASR��γ�ʼ��
	HCI_ERR_ASR_CONFIRM_NO_TASK,			///< 202: ʹ��confirm����û��ȷ������
	HCI_ERR_ASR_PARAM_CHECK_FLAG_INVALID,	///< 203: �����CheckFlag������Ѿ���⵽�˵���Ȼ�ٷ������ݣ�����δ�����˵���ͷ���flagΪ��CHECK_FLAG_END��CHECK_FLAG_PROGRESS��
	HCI_ERR_ASR_GRAMMAR_DATA_TOO_LARGE,		///< 204: �﷨����̫��
	HCI_ERR_ASR_ENGINE_NOT_INIT,			///< 205: ASR����������δ��ʼ��(�ѷ����������汾3.8)
	HCI_ERR_ASR_ENGINE_INIT_FAILED,			///< 206: ASR���������ʼ��ʧ��
	HCI_ERR_ASR_OPEN_GRAMMAR_FILE,			///< 207: ��ȡ�﷨�ļ�ʧ��
	HCI_ERR_ASR_LOAD_GRAMMAR_FAILED,		///< 208: �����﷨�ļ�ʧ��
	HCI_ERR_ASR_ENGINE_FAILED,				///< 209: ASR��������ʶ��ʧ��
	HCI_ERR_ASR_GRAMMAR_ID_INVALID,			///< 210: �﷨ID��Ч
	HCI_ERR_ASR_REALTIME_WAITING,			///< 211: ʵʱʶ��ʱδ��⵽��Ƶĩ�ˣ������ȴ�����
	HCI_ERR_ASR_GRAMMAR_OVERLOAD,			///< 212: �����﷨�����Ѵ�����
	HCI_ERR_ASR_GRAMMAR_USING,				///< 213: ���﷨����ʹ����
	HCI_ERR_ASR_REALTIME_END,				///< 214: ʵʱʶ��ʱ��⵽ĩ�ˣ����߻�����������Ҫʹ��NULL��ȡ���
	HCI_ERR_ASR_UPLOAD_NO_DATA,				///< 215: �ϴ���������ʱ���������ϴ������� 
	HCI_ERR_ASR_REALTIME_NO_VOICE_INPUT,	///< 216: ʵʱʶ��ʱδ�������
	HCI_ERR_ASR_VOICE_DATA_TOO_LARGE,		///< 217: ��ƵƬ��̫����Ӧ��(0,32K)
	
	/* HCI_HWR */
	HCI_ERR_HWR_NOT_INIT = 300,				///< 300: HCI_HWRû�г�ʼ��
	HCI_ERR_HWR_ALREADY_INIT,				///< 301: HCI_HWR��γ�ʼ��
	HCI_ERR_HWR_CONFIRM_NO_TASK,			///< 302: ʹ��confirm����û��ȷ������
	HCI_ERR_HWR_ENGINE_INIT_FAILED,			///< 303: HWR���������ʼ��ʧ��
	HCI_ERR_HWR_ENGINE_FAILED,				///< 304: HWR�������������ʶ�𡢻�ȡƴ������ȡ��������ʧ��
	HCI_ERR_HWR_UPLOAD_NO_DATA,				///< 305: û�п������ϴ�������
	HCI_ERR_HWR_ENGINE_SESSION_START_FAILED,///< 306: HWR�������濪���Ựʧ��
	HCI_ERR_HWR_ENGINE_NOT_INIT,			///< 307: SDK��ʼ��ʱδ���뱾������ȴ�ڴ����Ựʱʹ���˱�������(�ѷ����������汾3.8)
	HCI_ERR_HWR_CONFIG_SUBLANG_MISSING,		///< 308: �����������������ֵ�ʱ��δ����sublang
	HCI_ERR_HWR_TOO_MANY_DOMAIN,			///< 309: ������������Ŀ������4��

	/* HCI_OCR */
	HCI_ERR_OCR_NOT_INIT = 400,				///< 400: HCI_OCRû�г�ʼ��
	HCI_ERR_OCR_ALREADY_INIT,				///< 401: HCI_OCR��γ�ʼ��
	HCI_ERR_OCR_ENGINE_INIT_FAILED,			///< 402: OCR���������ʼ��ʧ��
	HCI_ERR_OCR_ENGINE_FAILED,				///< 403: OCR���������������бУ�������������ʶ��Ԥ����ѹ����ʧ��
	HCI_ERR_OCR_ENGINE_NOT_INIT,			///< 404: SDK��ʼ��ʱδ���뱾������ȴ�ڴ����Ựʱʹ���˱�������(�ѷ����������汾3.8)
	HCI_ERR_OCR_LOAD_IMAGE,					///< 405: ���뱾���ļ����߱���ͼƬ����ʧ��
	HCI_ERR_OCR_SAVE_IMAGE,					///< 406: ����OCR_IMAGE�������ļ�ʧ��
	HCI_ERR_OCR_IMAGE_NOT_SET,				///< 407: δ����Ҫ�����ͼƬ�ͽ�������бУ�������������ʶ��Ȳ���
	HCI_ERR_OCR_LOAD_TEMPLATE_FAILED,		///< 408: ����ģ���ļ�ʧ��
	HCI_ERR_OCR_TEMPLATE_OVERLOAD,		    ///< 409: ����ģ�������Ѵ����ޣ�1024��
	HCI_ERR_OCR_TEMPLATE_ID_INVALID,		///< 410: �����ģ��ID���ڼ��ص�ģ���б���
	HCI_ERR_OCR_TEMPLATE_USING,			    ///< 411: ���лỰ��ʹ��Ҫж�ص�ģ��
	HCI_ERR_OCR_DETECT_CORNER_FAILED,		///< 412: OCR��ȡ�б���Ϣʧ��
	HCI_ERR_OCR_NORMALIZE_FAILED,			///< 413: OCR�б�ʧ��
	HCI_ERR_OCR_RECOGNIZABLE_FAILED,		///< 414: OCRʶ���жϷ���ʧ��
	HCI_ERR_OCR_IMAGE_NOT_CLEAR,			///< 415: OCRʶ��ͼƬ������
	HCI_ERR_OCR_CONFIG_TEMPLATE_ID_MISSING,	///< 416: OCR����ģ�� templateid����ȱʧ
	HCI_ERR_OCR_CONFIG_TEMPLATE_INDEX_MISSING,	///< 417: OCRģ�� templateindex����ȱʧ
	HCI_ERR_OCR_CONFIG_TEMPLATE_PAGE_INDEX_MISSING,	///< 418: OCRģ�� templatepageindex����ȱʧ
	HCI_ERR_OCR_CONFIG_PROPERTY_MISSING,	///< 419: OCR�ƶ�ģ�� property����ȱʧ
	HCI_ERR_OCR_EDGE_IS_NOT_EXIST,			///< 420: OCRʶ��ͼƬ��Ե������

	/* HCI_TTS */
	HCI_ERR_TTS_NOT_INIT = 500,				///< 500: HCI_TTSû�г�ʼ��
	HCI_ERR_TTS_ALREADY_INIT,				///< 501: HCI_TTS��γ�ʼ��
	HCI_ERR_TTS_SESSION_BUSY,				///< 502: TTS�Ự��æ�������ںϳɻص��������ֵ����˺ϳ�
	HCI_ERR_TTS_ENGINE_SESSION_START_FAILED,///< 503: TTS�������濪���Ựʧ��
	HCI_ERR_TTS_ENGINE_FAILED,				///< 504: TTS��������ϳ�ʧ��
    HCI_ERR_TTS_ENGINE_INIT_FAILED,	        ///< 505: TTS(NU)���������ʼ��ʧ��
    HCI_ERR_TTS_ENGINE_NOT_INIT,			///< 506: TTS(NU)����������δ��ʼ��
	HCI_ERR_TTS_CONFIG_PROPERTY_MISSING,	///< 507: TTS �ƶ�property����ȱʧ

	/* HCI_MT */
	HCI_ERR_MT_NOT_INIT = 600,				///< 600: HCI_MTû�г�ʼ��
	HCI_ERR_MT_ALREADY_INIT,				///< 601: HCI_MT��γ�ʼ��

	/*HCI_NLU*/
	HCI_ERR_NLU_NOT_INIT = 700,				///< 700: HCI_NLUû�г�ʼ��
	HCI_ERR_NLU_ALREADY_INIT,				///< 701: HCI_NLU��γ�ʼ��
	HCI_ERR_NLU_ENGINE_SESSION_START_FAILED,///< 702: NLU�������濪���Ựʧ��
	HCI_ERR_NLU_ENGINE_FAILED,				///< 703: NLU��������ʶ��ʧ��
	
	/*HCI_KB*/
	HCI_ERR_KB_NOT_INIT = 800,				///< 800: HCI_KBû�г�ʼ��
	HCI_ERR_KB_ALREADY_INIT,				///< 801: HCI_KB��γ�ʼ��
	HCI_ERR_KB_ENGINE_SESSION_START_FAILED,	///< 802: KB�������濪���Ựʧ��
	HCI_ERR_KB_ENGINE_FAILED,				///< 803: KB��������ʶ��ʧ��
	HCI_ERR_KB_SYLLABLE_INVALID,			///< 804: �ݴ������޷��ж�����
	HCI_ERR_KB_UDB_WORD_EXIST,				///< 805: �Ѿ����û��ʿ��д���
	HCI_ERR_KB_CONFIRM_NO_TASK,				///< 806: ʹ��confirm����û��ȷ������

	/*HCI_VPR*/
	HCI_ERR_VPR_NOT_INIT = 900,				///< 900: HCI_VPRû�г�ʼ��
	HCI_ERR_VPR_ALREADY_INIT,				///< 901: HCI_VPR��γ�ʼ��
	HCI_ERR_VPR_ENGINE_INIT_FAILED,			///< 902: VPR���������ʼ��ʧ��
	HCI_ERR_VPR_ENGINE_FAILED,				///< 903: VPR�������洦��ʧ��
	HCI_ERR_VPR_USERID_NOT_EXIST,			///< 904: VPR�û�������
	HCI_ERR_VPR_ENGINE_SESSION_START_FAILED,///< 905: VPR�������濪���Ựʧ��
	HCI_ERR_VPR_CONFIG_USER_ID_MISSING,		///< 906: VPRȱ��userId������
    HCI_ERR_VPR_CONFIG_GROUP_ID_MISSING,	///< 907: VPRȱ��groupId������
	HCI_ERR_VPR_REALTIME_WAITING,           ///< 908: ʵʱʶ���⵽ĩ�ˣ��ȴ�
	HCI_ERR_VPR_VOICE_SHORT,			    ///< 909: ����̫��
	HCI_ERR_VPR_VOLUMN_HIGH,			    ///< 910: ����̫��
	HCI_ERR_VPR_VOLUMN_LOW,				    ///< 911: ����̫��
	HCI_ERR_VPR_NOISE_HIGH,				    ///< 912: ����̫��
	HCI_ERR_VPR_VAD_DETECT_RESULT_IS_EMPTY, ///< 913: �˵�����Ϊ��


	/*HCI_FPR*/
	HCI_ERR_FPR_NOT_INIT = 1000,			///< 1000: HCI_FPRû�г�ʼ��
	HCI_ERR_FPR_ALREADY_INIT,				///< 1001: HCI_FPR��γ�ʼ��
	HCI_ERR_FPR_ENGINE_INIT_FAILED,			///< 1002: FPR���������ʼ��ʧ��
	HCI_ERR_FPR_ENGINE_FAILED,				///< 1003: FPR�������洦��ʧ��
	HCI_ERR_FPR_USERID_NOT_EXIST,			///< 1004: FPR�û�������
	HCI_ERR_FPR_ENGINE_SESSION_START_FAILED,///< 1005: FPR�������濪���Ựʧ��
	HCI_ERR_FPR_CONFIG_USER_ID_MISSING,		///< 1006: FPRȱ��userId������
	HCI_ERR_FPR_CONFIG_DPI_MISSING,			///< 1007: FPRȱ��dpi������
	HCI_ERR_FPR_CONFIG_BGCOLOR_MISSING,		///< 1008: FPRȱ��bgColor������
	HCI_ERR_FPR_CONFIG_WIDTH_MISSING,		///< 1009: FPRȱ��width������
	HCI_ERR_FPR_CONFIG_HEIGHT_MISSING,		///< 1010: FPRȱ��height������
    HCI_ERR_FPR_CONFIG_GROUP_ID_MISSING,	///< 1011: FPRȱ��groupId������

	/*HCI_AFR*/
	HCI_ERR_AFR_NOT_INIT = 1100,				///< 1100: HCI_AFRû�г�ʼ��
	HCI_ERR_AFR_ALREADY_INIT,					///< 1101: HCI_AFR��γ�ʼ��
	HCI_ERR_AFR_ENGINE_INIT_FAILED,				///< 1102: AFR���������ʼ��ʧ��
	HCI_ERR_AFR_ENGINE_FAILED,					///< 1103: AFR�������洦��ʧ��
	HCI_ERR_AFR_USERID_NOT_EXIST,				///< 1104: AFR�û�������
	HCI_ERR_AFR_ENGINE_SESSION_START_FAILED,	///< 1105: AFR�������濪���Ựʧ��
	HCI_ERR_AFR_CONFIG_USER_ID_MISSING,			///< 1106: AFRȱ��userId������
	HCI_ERR_AFR_CONFIG_FACE_ID_MISSING,			///< 1107: AFRȱ��faceid������
	HCI_ERR_AFR_CONFIG_GROUP_ID_MISSING,		///< 1108: AFRȱ��groupId������
	HCI_ERR_AFR_ENGINE_SESSION_IMAGE_MISSING,	///< 1109: AFR̽��δ����ͼ����Ϣ
	HCI_ERR_AFR_LOAD_IMAGE,						///< 1110: AFR�޷����ļ�
	HCI_ERR_AFR_IMAGE_INVALID,					///< 1111: AFR����ͼ��Ƿ�
	HCI_ERR_AFR_MODEL_INVALID,					///< 1112: AFR���������ģ�ͷǷ�
	HCI_ERR_AFR_CONFIG_INVALID,					///< 1113: AFR��������ô����Ϸ�
	HCI_ERR_AFR_PARAM_KEY,						///< 1114: AFR����ȷ�Ĳ���Key
	HCI_ERR_AFR_SAVE_MODEL,						///< 1115: �����û�ģ�ʹ���
	HCI_ERR_AFR_ENGINE,							///< 1116: �������, һ�㲻�ᷢ��
	HCI_ERR_AFR_FACEID_NOT_EXIST,				///< 1117: AFR����������
	HCI_ERR_AFR_MODEL_NOT_EXIST,					///< 1118: AFR����ģ�Ͳ�����

	/*ASR Recorder*/
	HCI_ERR_RECORDER_NOT_INIT = 1200,				///< 1200: ASR Recorderδ��ʼ��
	HCI_ERR_RECORDER_ALREADY_INIT,					///< 1201: ASR Recorder�Ѿ���ʼ��
	HCI_ERR_RECORDER_NOT_BEGIN,						///< 1202: ASR Recorderδ��ʼ¼��
	HCI_ERR_RECORDER_ALREADY_BEGIN					///< 1203: ASR Recorder�Ѿ���ʼ¼��
	
} HCI_ERR_CODE;


/**
 * @brief	��������Ŀ
 */
typedef struct _tag_CAPABILITY_ITEM
{
	/// ������Ψһ��ʶ
	char* pszCapKey;
} CAPABILITY_ITEM;

/**
 * @brief	�����б�
 */
typedef struct _tag_CAPABILITY_LIST 
{
	/// ������Ŀ�б�
	CAPABILITY_ITEM *	pItemList;

	/// ��������
	unsigned int	uiItemCount;
} CAPABILITY_LIST;

/* @} */
/** @defgroup HCI_SYS_FUNC ���� */
/* @{ */

/**
 * @brief	����ϵͳSDK ������ʼ�� 
 * @param	pszConfig	��ʼ�����ô���ASCII�ַ�������'\0'����
 * @return	
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_ALREADY_INIT</td><td>HCI SYS �Ѿ���ʼ������</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>����������Ϸ�,�����pszConfig����NULL���ַ�������""</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>���ò����������趨ֵ�Ƿ����ʽ�����</td></tr>
 *		<tr><td>@ref HCI_ERR_START_LOG_FAILED</td><td>������־����, һ��Ϊ��־·�������ڻ���û��дȨ�޵����</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_CONFIG_AUTHPATH_MISSING</td><td>ȱ�ٱ����authPath������</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_CONFIG_CLOUDURL_MISSING</td><td>ȱ�ٱ����cloudUrl������</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_CONFIG_DEVELOPERKEY_MISSING</td><td>ȱ�ٱ����developerKey������</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_CONFIG_APPKEY_MISSING</td><td>ȱ�ٱ����appKey������</td></tr>
 *	</table>
 *
 * @par ���ô����壺
 * ���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
 * @n@n
 * �����˺����ã���ѡ��
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>developerKey</td>
 *			<td>�ַ���</td>
 *			<td>��</td>
 *			<td>��������Կ</td>
 *			<td>�������ɽ�ͨ�����ṩ��</td>
 *		</tr>
 *		<tr>
 *			<td>appKey</td>
 *			<td>�ַ���</td>
 *			<td>��</td>
 *			<td>Ӧ����Կ</td>
 *			<td>�������ɽ�ͨ�����ṩ��</td>
 *		</tr>
 *		<tr>
 *			<td>cloudUrl</td>
 *			<td>�ַ�����url��ַ���磺http://test.api.hcicloud.com:8888</td>
 *			<td>��</td>
 *			<td>���Ʒ����ַ</td>
 *			<td>�������,�ɽ�ͨ�����ṩ��</td>
 *		</tr>
 *		<tr>
 *			<td>authPath</td>
 *			<td>�ַ������磺./auth/</td>
 *			<td>��</td>
 *			<td>��Ȩ�ļ�����·��</td>
 *			<td>��������Ŀ¼�½�����<b>��Ȩ�ļ�</b>��<br/>
 *				SDK����Ҫ�ڴ�Ŀ¼��дһЩ�����ļ�����˴�Ŀ¼�����пɶ�дȨ��, 
 *              ��ǿ�ҽ��鲻����һ�����Ƴ��Ľ����ϣ�����SD���ϣ���ֹSD�����γ���<br/>
 *              Androidϵͳ�½���ʹ��/data/data/����/file ·��
 *			</td>
 *		</tr>
 *		<tr>
 *			<td>appChannel</td>
 *			<td>���������ο�@subpage app_channel</td>
 *			<td>1</td>
 *			<td>Ӧ������</td>
 *			<td>��ѡ���ã�������ͨ��ָ����ͬ��ֵ�����벻ͬ�İ汾�����������ƿ������������Ի�ȡ��ͬ�������ذ�װʹ�õ�һ��ͳ�ơ�</td>
 *		</tr>
 *		<tr>
 *			<td>udidType</td>
 *			<td>
 *				�ַ���������ƽ̨��ͬ����ѡȡֵΪ
 *				windows��{"unknown","macadress","uuid"}
 *				linux��{"unknown","macadress","uuid"}
 *				ios��{"unknown","macadress","uuid","openudid"}
 *				android��{"unknown","androidid","imei","serial","wifiid","randomnumber"}
 *			</td>
 *			<td>unknown</td>
 *			<td>��ȡudid����</td>
 *			<td>
 *				��ѡ���á���Ϊunknownʱ����ƽ̨Ĭ����Ȩ��ʽΪ
 *				windows��"macadress"
 *				linux��"macadress"
 *				ios��"macadress"
 *				android��"androidid"
 *			</td>
 *		</tr>
 *	</table>
 * @n@n
 * ��־���ã���ѡ��
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>logFilePath</td>
 *			<td>�ַ������磺./log/</td>
 *			<td>��</td>
 *			<td>��־�ļ����·��</td>
 *			<td>��ѡ�������������Ϊ����������־����־����Ϊhci.log</td>
 *		</tr>
 *		<tr>
 *			<td>logFileSize</td>
 *			<td>����������Χ[1,1048576]</td>
 *			<td>32</td>
 *			<td>��־��С</td>
 *			<td>Ĭ��һ����־�ļ�д��󣬵�λΪK</td>
 *		</tr>
 *		<tr>
 *			<td>logFileCount</td>
 *			<td>��������Χ[0,1024]</td>
 *			<td>5</td>
 *			<td>��־��Ŀ</td>
 *			<td>Ĭ�ϱ������ٸ����ݵ���־�ļ��������򸲸���ɵ���־��Ϊ0��ʾ��ǰ��־���������κα���</td>
 *		</tr>
 * 		<tr>
 *			<td>logLevel</td>
 *			<td>������[0��5]</td>
 *			<td>1</td>
 *			<td>��־�ȼ�</td>
 *			<td>SDK�����С�ڵ���logLevel����־��Ϣ,��־����Ϊ��<br/>
 *              0=��<br/>
 *              1=����<br/>
 *              2=����<br/>
 *              3=��Ϣ<br/>
 *              4=ϸ��<br/>
 *              5=����</td>
 *		</tr>
 *	</table>
 * HTTP�������ã���ѡ��
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 * 		<tr>
 *			<td>netProxy</td>
 *			<td>�ַ�����һ��ΪIP��ַ���磺192.168.10.2</td>
 *			<td>��</td>
 *			<td>HTTP���������IP��ַ</td>
 *			<td>��ѡ</td>
 *		</tr>
 * 		<tr>
 *			<td>netProxyPort</td>
 *			<td>����������Ч�Ķ˿ںţ��磺1080</td>
 *			<td>1080</td>
 *			<td>HTTP����������˿�</td>
 *			<td>��ѡ,���netPorxyδ���������Ҳ��������</td>
 *		</tr>
 * 		<tr>
 *			<td>netProxyUser</td>
 *			<td>�ַ������磺user</td>
 *			<td>��</td>
 *			<td>HTTP��������������û���</td>
 *			<td>��ѡ,���netPorxyδ���������Ҳ��������</td>
 *		</tr>
 * 		<tr>
 *			<td>netProxyPwd</td>
 *			<td>�ַ������磺pwd</td>
 *			<td>��</td>
 *			<td>HTTP�����������������</td>
 *			<td>��ѡ,���netPorxyδ���������Ҳ��������</td>
 *		</tr>
 *	</table>
 * @note
 * @n@n			
 * <b>Ӧ����Ϣ</b>
 * @n@n
 * ��������developerKey, appKey���������ɽ�ͨ�����ṩ�����ڱ�ʶ��ͬ�Ŀ����ߺ�Ӧ�ó��򣬸��費ͬ����Ȩ�ͷ���<br/>
 * ������ṩ�����ṩ���ݺͽ�ͨ���������Ĳ�һ�£��ᵼ�·���ʧ�ܡ�
 * @n@n
 * <b>��Ȩ</b>
 * @n@n
 * ��Ȩ������ȷ��Ӧ�ÿ���ʹ�õ��ƶ˻򱾵ص�HCI��������CapKey�б����й���ʱ�䣬����ʱ�����Ϊ���ÿ��á�<br/>
 * ��Ȩ�������б����ͨ���ӿ�\ref hci_get_capability_list ��ȡ������ʱ�����ͨ���ӿ�\ref hci_get_auth_expire_time ��ȡ��
 * @n@n
 * <b>CapKey</b>
 * @n@n
 * ����SDK������ʶ������������asr.cloud.freetalkΪ�ƶ�����˵����ʶ��Ӧ��ֻ��ʹ�ð�������Ȩ�ļ��е�CapKey��
 * @n@n
 */ 
#ifdef __ANDROID__
HCI_ERR_CODE HCIAPI hci_init( 
		_MUST_ _IN_ const char * pszConfig ,
		_MUST_ _IN_ JavaVM *vm, 
		_MUST_ _IN_ jobject context);
#else
HCI_ERR_CODE HCIAPI hci_init(
		_MUST_ _IN_ const char * pszConfig
		);
#endif

/**  
 * @brief	�ֶ���������Ȩ
 * @details	����������ʱ������ͨ�� hci_get_auth_expire_time()�õ�����Ȩ�Ĺ���ʱ�䣬
 *			ͨ�� hci_get_capability() �õ�����Ȩ�Ŀ�ʹ�õ�HCI������
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS ��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>���ӷ�����ʧ�ܣ���Ҫ����豸����</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>���������ʳ�ʱ</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>���������ص����ݸ�ʽ����ȷ�������쳣����ϵ��ͨ����</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_CHECKAUTH_RESPONSE_FAILED</td><td>��ȡ�ƶ���Ȩʧ��</td></tr>
 *	</table>
 */
HCI_ERR_CODE HCIAPI hci_check_auth();

/**  
 * @brief	�����Ȩ����ʱ��
 * @details	�õ���ʱ��� time() ���ظ���һ�£�ָ 1970-01-01 00:00:00 UTC ֮���������
 * @param	pnExpireTime		���������ָ�룬��Ź���ʱ��
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS ��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>����������Ϸ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_AUTHFILE_INVALID</td><td>��ȡ��Ȩ�ļ�����򱾵�����Ȩ</td></tr>
 *	</table>
 */
HCI_ERR_CODE HCIAPI hci_get_auth_expire_time(
		_MUST_ _OUT_ int64 * pnExpireTime
		);


/**
 * @brief	��ȡĳ��HCI���Ϳ��õ������б�
 * @param	pszHciType				����ѯ��HCI���������� hwr, tts, ocr, asr)������������';'������
 *									���ΪNULL���߿մ�����ʾȫ����ѯ
 * @param	psCapabilityList		���������ָ�룬��ŷ��ص������б�ʹ����Ϻ�Ӧ��ʹ�� hci_free_capability_list() �ͷ�
 * @return	
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS ��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>����������Ϸ�</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_get_capability_list(
	    _OPT_ _IN_ const char * pszHciType,
		_MUST_ _OUT_ CAPABILITY_LIST	* psCapabilityList
		);

/**
 * @brief	�ͷ������б���ڴ�
 * @param	psCapabilityList		��Ҫ�ͷŵ������б��ڴ�
 * @return	
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>����������Ϸ�</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_free_capability_list(
		_MUST_ _IN_ CAPABILITY_LIST	* psCapabilityList
		);


/**  
* @brief	�ϴ������û�¼������
* @return	������
* @return	
* @n
*	<table>
*		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
*		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS��δ��ʼ��</td></tr>
*		<tr><td>@ref HCI_ERR_URL_MISSING</td><td>δ�ҵ���Ч���Ʒ����ַ</td></tr>
*		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>���ӷ�����ʧ�ܣ���Ҫ����豸����</td></tr>
*		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>���������ʳ�ʱ</td></tr>
*		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>���������ص����ݸ�ʽ����ȷ�������쳣����ϵ��ͨ����</td></tr>
*		<tr><td>@ref HCI_ERR_SERVICE_RESPONSE_FAILED</td><td>�����������ϴ�ʧ��</td></tr>
*	</table>
*/ 
HCI_ERR_CODE HCIAPI hci_upload_user_history();

/**  
 * @brief	����ϵͳSDK ����ʼ��
 * @return	������
 * @return	
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS ��δ��ʼ��</td></tr>
  *		<tr><td>@ref HCI_ERR_SYS_USING</td><td>��Ȼ��������ʹ�ã���δ����ʼ����</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_release();


/**  
 * @brief	��ȡ�������Ӧ����ϸ������Ϣ
 * @return	������Ϣ
 * @note
 * ������Ϣ��utf8�����Ӣ���ַ���,�������Ĵ�����û��������sdk�ж���
 * �᷵�ء�Unkown Error��
 */ 
const char * HCIAPI hci_get_error_info(HCI_ERR_CODE nErrorCode);

/**  
 * @brief	��ȡsdk�汾��Ϣ
 * @return	sdk�汾��
 * 
 */ 
const char * HCIAPI hci_get_sdk_version();

/* @} */
/* @} */
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
};
#endif

#endif
