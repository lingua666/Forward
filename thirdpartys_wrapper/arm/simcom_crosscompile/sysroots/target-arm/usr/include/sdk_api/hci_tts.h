/** 
 * @file    hci_tts.h 
 * @brief   HCI_TTS SDK ͷ�ļ�  
 */  

#ifndef __HCI_TTS_HEADER__
#define __HCI_TTS_HEADER__

#include "hci_sys.h"

#ifdef __cplusplus
extern "C"		
{
#endif

/** @defgroup HCI_TTS ����TTS����API */
/* @{ */


    /** @defgroup HCI_TTS_STRUCT  �ṹ�� */
    /* @{ */
    //////////////////////////////////////////////////////////////////////////
    
    /**
	*@brief	ģ������
	*/
    #define TTS_MODULE	"hci_tts"

	/**
	 * @brief mark ���
	 */
	typedef struct _tag_TTS_SYNTH_MARK
	{
		/// mark��ǵ�����
		char * pszName;
		///mark�ںϳ���Ƶ�е�ʱ��㡣
		unsigned int time;
	} TTS_SYNTH_MARK;

    /**
     * @brief	TTS �ϳɽ��
     */
    typedef struct _tag_TTS_SYNTH_RESULT
    {
	    /// ���ݻ�����ָ�룬���ݸ�ʽ�ɺϳ�ʱ������þ���
	    void * pvVoiceData;

	    /// ���ݻ��������ȣ����ֽ�Ϊ��λ
	    unsigned int uiVoiceSize;

	    /// �������ݶ�Ӧ���ı����ݵ���ʼָ��
	    char * pszText;

	    /// �������ݶ�Ӧ���ı����ȣ��ֽ�Ϊ��λ
	    unsigned int uiTextLen;
    	
	    /// �Ƿ������ݵȴ��ϳ�
	    bool bMore;

		/// mark ���
		TTS_SYNTH_MARK *pMark;

		/// mark������
		unsigned int	nMarkCount;
    } TTS_SYNTH_RESULT;

    /* @} */

    /** @defgroup HCI_TTS_CALLBACK  �ص����� */
    /* @{ */


    /**  
     * @brief	�����ϳɻص�����
     * @param	pvUserParam			�û��Զ���ص�ָ��
     * @param	psTtsSynthResult	�ϳɽ�������ݽṹָ��
     * @param	hciErrCode			������
     * @return	�Ƿ�����ϳɣ�����true��ʾ�����ϳɣ�����false��ʾ��ֹ�ϳ�
     *
     * @note
     * psTtsSythResult�е������ڱ��ص��������غ󣬲���֤������Ч������û������Ҫ�������ݽ��д���
     * ��Ҫ���н����ݿ�����ȥ��
     */ 
    typedef bool (HCIAPI * Callback_TtsSynth)(
		    _OPT_ _IN_ void * pvUserParam,
		    _MUST_ _IN_ TTS_SYNTH_RESULT * psTtsSynthResult,
		    _MUST_ _IN_ HCI_ERR_CODE  hciErrCode
		    );

     /* @} */

    /** @defgroup HCI_TTS_FUNC  ���� */
    /* @{ */

/**  
 * @brief	����TTS���� ��ʼ��
 * @param	pszConfig		��ʼ�����ô���ASCII�ַ�������ΪNULL����'\0'����
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS ��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ALREADY_INIT</td><td>�Ѿ���ʼ������</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>���ò����������趨ֵ�Ƿ������ʽ�����</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>����������Դ·��δ����</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>δ�ҵ���Ȩ������</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>������Key��ƥ��</td></tr>
 *	</table>
 *
 * @par ���ô����壺
 * ���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>dataPath</td>
 *			<td>opt/myapp/tts_data/</td>
 *			<td>��</td>
 *			<td>�����ϳɱ�����Դ����·��</td>
 *			<td>�����ʹ�ñ��������ϳ����������Բ�������</td>
 *		</tr>
 *		<tr>
 *			<td>initCapKeys</td>
 *			<td>tts.local.xixi;tts.local.haobo</td>
 *			<td>��</td>
 *			<td>��ʼ������ı�������</td>
 *			<td>���������';'��������������ƶ�����key���򵥺��ԡ������ʹ�ñ��������ϳ����������Բ�������</td>
 *		</tr>
 *		<tr>
 *			<td>fileFlag</td>
 *			<td>none, android_so</td>
 *			<td>none</td>
 *			<td>��ȡ�����ļ�����������</td>
 *			<td>�μ������ע��</td>
 *		</tr>
 *	</table>
 *
 *  <b>Android��������</b>
 *  @n
 *  ��fileFlagΪandroid_soʱ�����ر�����Դ�ļ�ʱ�Ὣ�����Ŀ��ļ�������Ϊso�ļ������м��ء�
 *  ���磬��ʹ�õĿ�Ϊfile.datʱ����ʵ�ʴ򿪵��ļ���Ϊlibfile.dat.so��������Androidϵͳ�£�
 *  �����߿��԰��մ˹��򽫱�����Դ������, ����libsĿ¼�´����apk���ڰ�װ����Щ��Դ�ļ�
 *  �ͻ������/data/data/����/libĿ¼�¡������ֱ�ӽ�dataPath������ָΪ��Ŀ¼���ɡ�
 */

HCI_ERR_CODE HCIAPI hci_tts_init(
		_MUST_ _IN_ const char * pszConfig
		);

/**  
 * @brief	��ʼ�Ự
 * @param	pszConfig		�Ự���ô���ASCII�ַ�������'\0'����
 * @param	pnSessionId		�ɹ�ʱ���ػỰID
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>����Ĳ������Ϸ�</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>���ò����������趨ֵ�Ƿ������ʽ�����</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_MISSING</td><td>ȱ�ٱ����capKey������</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>������Key��ƥ��</td></tr>
 *		<tr><td>@ref HCI_ERR_TOO_MANY_SESSION</td><td>������Session������������(256),����������Ҫ�򿪱�����Դ�ļ��������ܹ��ɹ��򿪵ı���Session�����뱾��ϵͳ��ͬʱ���ļ������������йء�</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>δ�ҵ���Ȩ������</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>����������Դ·��δ����</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ENGINE_SESSION_START_FAILED</td><td>TTS�������濪���Ựʧ��</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>���ò��������治֧��</td></tr>
 *		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>������������ȱʧ��Ҫ�Ŀ���Դ</td></tr>
 *	</table>
 *
 *
 * @par ���ô����壺
 * ���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>capKey</td>
 *			<td>�ַ������ο� @ref hci_tts_page </td>
 *			<td>��</td>
 *			<td>�����ϳ�����key</td>
 *			<td>������ÿ��sessionֻ�ܶ���һ�����������ҹ����в��ܸı䡣</td>
 *		</tr>
 *		<tr>
 *			<td>resPrefix</td>
 *			<td></td>
 *			<td>��</td>
 *			<td>��Դ����ǰ׺</td>
 *			<td>�����ͬ�Ự��Ҫʹ��ͬһ·������Դʱ����ʹ�ø��ֶζ�ͳһ·���µ���Դ��������</td>
 *		</tr>
 *	</table>
 * @n@n
 * ���⣬���ﻹ���Դ���ϳɵ��������ΪĬ��������μ� hci_tts_synth() ��
 */
HCI_ERR_CODE HCIAPI hci_tts_session_start(
		_MUST_ _IN_ const char * pszConfig,
		_MUST_ _OUT_ int * pnSessionId
		);

/**  
 * @brief	���������ϳɲ���
 * @param	nSessionId		�ỰID
 * @param	pvText			Ҫ�ϳɵ��ı����ݣ�UTF-8���룬��󳤶�Ϊ1024�ֽڣ����ظ質����֧����󳤶�Ϊ8192�ֽڣ����ظ質�����ݲ�֧�ִ����޸��������ı�������'\0'����
 * @param	pszConfig		�ϳɲ������ô���ASCII�ַ�������ΪNULL����'\0'����
 * @param	pfnTtsSynth		�����ϳɽ���ص�����
 * @param	pvUserParam		�û��Զ���ص�����ָ��
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>����Ĳ������Ϸ�</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>���ò����������趨ֵ�Ƿ������ʽ�����</td></tr>
 *		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>�����Session��Ч</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_SESSION_BUSY</td><td>TTS��ǰ�Ự��æ</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>δ�ҵ���Ȩ������</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>���ò��������治֧��</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ENGINE_FAILED</td><td>��������ϳ�ʧ��</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>���ӷ�����ʧ�ܣ�����������Ӧ</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>���������ʳ�ʱ</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>���������ص����ݸ�ʽ����ȷ</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_RESPONSE_FAILED</td><td>���������غϳ�ʧ��</td></tr>
 *		<tr><td>@ref HCI_ERR_LOAD_CODEC_DLL</td><td>����codec�����ʧ��</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_CONFIG_PROPERTY_MISSING</td><td>TTS �ƶ�property����ȱʧ</td></tr>
 *	</table>
 */

#ifdef PRIVATE_CLOUD__

 /**
 * @par ���ô����壺
 * ���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
 * @n@n
 * @par ��������
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>pitch</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>�ϳ������Ļ�Ƶ</td>
 *			<td>����Խ���ƵԽ�ߣ���Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>volume</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>�ϳ�����������</td>
 *			<td>����Խ������Խ����Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>speed</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>�ϳ�����������</td>
 *			<td>����Խ������Խ�죬��Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>audioFormat</td>
 *			<td>�ַ�������Чֵ����ϸ����</td> 
 *			<td>pcm16k16bit</td>
 *			<td>�����Ƶ��ʽ</td>
 *			<td>�ƶ�����֧�ֵ���Ƶ��ʽ:<br/>
 *				auto<br/>
 *				vox6k4bit, vox8k4bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				mp3   ��mp3 64kbps ��ʽ��<br/>
 *				mp3_24��mp3 24kbps ��ʽ��<br/>
 *				mp3_16��mp3 16kbps ��ʽ��<br/>
 *				��������֧�ֵ���Ƶ��ʽ:<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/></td> 
 *		</tr>
 *		<tr>
 *			<td>puncMode</td>
 *			<td>�ַ�������Чֵ{off, on}</td>
 *			<td>off</td>
 *			<td>�����Ŷ���</td>
 *			<td>off: ���ϳɱ�����<br/>
 *				on: �ϳɱ�����<br/>
 *		</tr>
 *		<tr>
 *			<td>engMode</td>
 *			<td>�ַ�������Чֵ{auto, english, letter}</td>
 *			<td>auto</td>
 *			<td>Ӣ���Ķ���ʽ</td>
 *			<td>auto: �Զ��жϡ� <br /> 
 *				english: ǿ������Ӣ������ĸ����¼���ʻ�ϳɡ�<br /> 
 *				letter: ǿ������Ӣ�İ��յ���ĸ��ʽ�ϳɡ� </td> 
 *		</tr>
 *		<tr>
 *			<td>digitMode</td>
 *			<td>�ַ�������Чֵ{auto_number, auto_telegram, number, telegram}</td>
 *			<td>auto_number</td>
 *			<td>�����Ķ���ʽ</td>
 *			<td>auto_number: �Զ���ʽ���������������Զ�ѡ��籨��ʽ����Ŀ��ʽ�ϳɣ�����޷��жϣ���ʹ����Ŀ��ʽ��<br /> 
 *				auto_telegram: �Զ���ʽ���������������Զ�ѡ��籨��ʽ����Ŀ��ʽ�ϳɣ�����޷��жϣ���ʹ�õ籨��ʽ��<br />
 *				number: ��Ŀ��ʽ�������������ֺϳɡ����磺1997���ɡ�һǧ�Űپ�ʮ�ߡ��� <br />
 *				telegram: �籨��ʽ����һ������һ�����ֺϳɡ����磺1997���ɡ�һ�ž��ߡ��� </td>
 *		</tr>
 *		<tr>
 *			<td>tagmode</td>
 *			<td>�ַ�������Чֵ{none,auto,vtml,ssml,s3ml,s3ml_sing}</td>
 *			<td>none</td>
 *			<td>��Ǵ���ʽ</td>
 *			<td>none: ���б�ǽ��ᱻ��Ϊ�ı�������ȱʡֵ<br/>
 *				auto: �Զ��жϱ������<br/>
 *				vtml: ����VTML��� <br/>
 *				ssml: ����SSML��� <br/> 
 *				ע��: ���ض˽�֧�� none, s3ml�� s3ml_sing(����s3ml��ʱֻ֧�֣�segment��prosody��mark) <br/> 
 *					  �ƶ˲�֧��s3ml_sing
 *				����S3ML����ϸ���ܣ�����գ�����ͨ����iHear7.0_Embedded S3ML �������ʹ��˵���� </td> 
 *		</tr>
 *	</table>
 */
#else
 /**
 * @par ���ô����壺
 * ���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
 * @n@n
 * @par ��������
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>pitch</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>�ϳ������Ļ�Ƶ</td>
 *			<td>����Խ���ƵԽ�ߣ���Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>volume</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>�ϳ�����������</td>
 *			<td>����Խ������Խ����Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>speed</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>�ϳ�����������</td>
 *			<td>����Խ������Խ�죬��Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>audioFormat</td>
 *			<td>�ַ�������Чֵ����ϸ����</td> 
 *			<td>pcm16k16bit</td>
 *			<td>�����Ƶ��ʽ</td>
 *			<td>�ƶ�����֧�ֵ���Ƶ��ʽ:<br/>
 *				auto<br/>
 *				vox6k4bit, vox8k4bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				mp3   ��mp3 64kbps ��ʽ��<br/>
 *				mp3_24��mp3 24kbps ��ʽ��<br/>
 *				mp3_16��mp3 16kbps ��ʽ��<br/>
 *				��������֧�ֵ���Ƶ��ʽ:<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/></td> 
 *		</tr>
 *		<tr>
 *			<td>puncMode</td>
 *			<td>off, on, off_rtn, on_rtn</td>
 *			<td>off</td>
 *			<td>�����Ŷ���</td>
 *			<td>off: ���ϳɱ����ţ��Զ��жϻس������Ƿ�ָ���<br/>
 *				on: �ϳɱ����ţ��Զ��жϻس������Ƿ�ָ���<br/>
 *				off_rtn: �������ţ�ǿ�ƽ��س�������Ϊ�ָ���(���ƶ�֧��)<br/>
 *				on_rtn: �����ţ�ǿ�ƽ��س�������Ϊ�ָ���(���ƶ�֧��)</td>
 *		</tr>
 *		<tr>
 *			<td>engMode</td>
 *			<td>�ַ�������Чֵ{auto, english, letter}</td>
 *			<td>auto</td>
 *			<td>Ӣ���Ķ���ʽ</td>
 *			<td>auto: �Զ��жϡ� <br /> 
 *				english: ǿ������Ӣ������ĸ����¼���ʻ�ϳɡ�<br /> 
 *				letter: ǿ������Ӣ�İ��յ���ĸ��ʽ�ϳɡ� </td> 
 *		</tr>
 *		<tr>
 *			<td>digitMode</td>
 *			<td>�ַ�������Чֵ{auto_number, auto_telegram, number, telegram}</td>
 *			<td>auto_number</td>
 *			<td>�����Ķ���ʽ</td>
 *			<td>auto_number: �Զ���ʽ���������������Զ�ѡ��籨��ʽ����Ŀ��ʽ�ϳɣ�����޷��жϣ���ʹ����Ŀ��ʽ��<br /> 
 *				auto_telegram: �Զ���ʽ���������������Զ�ѡ��籨��ʽ����Ŀ��ʽ�ϳɣ�����޷��жϣ���ʹ�õ籨��ʽ��<br />
 *				number: ��Ŀ��ʽ�������������ֺϳɡ����磺1997���ɡ�һǧ�Űپ�ʮ�ߡ��� <br />
 *				telegram: �籨��ʽ����һ������һ�����ֺϳɡ����磺1997���ɡ�һ�ž��ߡ��� </td>
 *		</tr>
 *		<tr>
 *			<td>tagmode��������֧�֣�</td>
 *			<td>�ַ�������Чֵ{none,s3ml,s3ml_sing}</td>
 *			<td>none</td>
 *			<td>��Ǵ���ʽ</td>
 *			<td>none: ���б�ǽ��ᱻ��Ϊ�ı�����<br/>
 *				s3ml_sing:���б�ǽ�����Ϊ���׳���������ĺϳ��ı������ϸ���ѭs3ml��ʽ<br/>
 *				s3ml��ʱֻ֧�֣�segment��prosody��mark <br/> 
 *				����S3ML����ϸ���ܣ�����գ�����ͨ����iHear7.0_Embedded S3ML �������ʹ��˵���� </td>
 *		</tr>
 *		<tr>
 *			<td>tempo����tts.local.synth.sing֧�֣�</td>
 *			<td>������25~1275</td>
 *			<td>380</td>
 *			<td>����</td>
 *			<td>ÿ�ĳ�������</td>
 *		</tr>
 *		<tr>
 *			<td>interval����tts.local.synth.sing֧�֣�</td>
 *			<td>������-16~16</td>
 *			<td>0</td>
 *			<td>���߱仯����</td>
 *			<td>��Ĭ��C�����߻򽵵ͼ�������</td>
 *		</tr>
 *	</table>
 */
#endif

#ifdef PRIVATE_CLOUD__
 /**
 * @n@n
 * @par �ƶ�����
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>encode</td>
 *			<td>�ַ�������Чֵ{none, opus, speex}</td>
 *			<td>opus</td>
 *			<td>ʹ�õı����ʽ��ֻ���ƶ˺ϳ���Ч�����غϳɺ��Ը�����</td>
 *			<td>��ʾ�ƶ˺ϳɴ����ʱ����õı����ʽ����audioFormatΪvox6k4bit/vox8k4bit/mp3ʱ��<br/>
 *              ��֧��ʹ��opus,speex��ֱ�Ӻ������ã���������ɲ���opus,speex����ѹ�����䣬�Լ�����������</td>
 *		</tr> 
 *		<tr>
 *			<td>encLevel</td>
 *			<td>��������Χ[0,10]</td>
 *			<td>7</td>
 *			<td>ѹ���ȼ���ֻ���ƶ˺ϳ���Ч�����غϳɺ��Ը����á���ֵ��ѹ���ʳɷ���</td>
 *			<td></td>
 *		</tr>
 *		<tr>
 *			<td>property</td>
 *			<td>�ַ��������磺cn_xiaokun_common</td>
 *			<td>��</td>
 *			<td>���������ȡֵ������lang��������speaker������domain��ϡ�</td>
 *			<td>��ѡ����Ч��ϲμ��ĵ�ĩβ��˵��<br/>
 *			</td>
 *		</tr>
 *	</table>
 */
#else
  /**
 * @n@n
 * @par �ƶ�����
 *	<table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>encode</td>
 *			<td>�ַ�������Чֵ{none, opus, speex}</td>
 *			<td>opus</td>
 *			<td>ʹ�õı����ʽ��ֻ���ƶ˺ϳ���Ч�����غϳɺ��Ը�����</td>
 *			<td>��ʾ�ƶ˺ϳɴ����ʱ����õı����ʽ����audioFormatΪvox6k4bit/vox8k4bit/mp3ʱ��<br/>
 *              ��֧��ʹ��opus,speex��ֱ�Ӻ������ã���������ɲ���opus,speex����ѹ�����䣬�Լ�����������</td>
 *		</tr> 
 *		<tr>
 *			<td>encLevel</td>
 *			<td>��������Χ[0,10]</td>
 *			<td>7</td>
 *			<td>ѹ���ȼ���ֻ���ƶ˺ϳ���Ч�����غϳɺ��Ը����á���ֵ��ѹ���ʳɷ���</td>
 *			<td></td>
 *		</tr>
 *		<tr>
 *			<td>domain</td>
 *			<td>finance</td>
 *			<td>��</td>
 *			<td>�ϳ�ʹ�õ�����</td>
 *			<td>ʹ��ָ�������������кϳɡ����capKey������δָ���ض�����
 *				�����ʹ��domain���н�һ��ָ���������capKey�������Ѿ�ָ�����ض�����
 *				�����������Ч��<br/>
 *				common      ͨ����������<br/>
 *				finance     ����֤ȯ<br/>
 *				weather     ����Ԥ��<br/>
 *				sports      ��������<br/>
 *				traffic     ������Ϣ<br/>
 *				travel      ���β���<br/>
 *				carborne    ����<br/>
 *				queue       �Ŷ�<br/>
 *				song        ���<br/>
 *				insurance	����<br/>
 *				voyage      ����<br/>
 *				revenue     ˰��<br/>
 *				elecpower   ����<br/>
 *				message     ����<br/>
 *				finainsu    ���ڱ���<br/>
 *				telecom     ����<br/>
 *				custom      �Զ���<br/>
 *				�����ѡ��������õ���������ڿ���ʱ��ѯ��ͨ������</td>
 *		</tr>
 *	</table>
 */
#endif
 /**
 * @n@n
 * @par ���ظ߼�����
 *  <table>
 *		<tr>
 *			<td><b>�ֶ�</b></td>
 *			<td><b>ȡֵ��ʾ��</b></td>
 *			<td><b>ȱʡֵ</b></td>
 *			<td><b>����</b></td>
 *			<td><b>��ϸ˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td>soundEffect</td>
 *			<td>�ַ�������Чֵ{base,reverb,echo,chorus,nearfar,robot}</td>
 *			<td>base</td>
 *			<td>��Ч����</td>
 *			<td>base: ����Ч<br/>
 *				reverb: ����<br/>
 *				echo: ����<br/>
 *				chorus: �ϳ�<br/>
 *				nearfar: ��Զ����<br/>
 *				robot: ������</td> 
 *		</tr>
 *		<tr>
 *			<td>mixSound</td>
 *			<td>��������Χ[1,9]</td>
 *			<td>5</td>
 *			<td>����ʱ��</td>
 *			<td>����Խ�����ʱ��Խ��,������ֻ����soundEffect����Ϊechoʱ��Ч</td>
 *		</tr>
 *		<tr>
 *			<td>namePolyphone</td>
 *			<td>�ַ�������Чֵ{off, on}</td>
 *			<td>off</td>
 *			<td>���϶����ֿ���</td>
 *			<td>off: �ر�<br/>
 *				on: ��</td>
 *		</tr>
 *		<tr>
 *			<td>symbolFilter</td>
 *			<td>�ַ�������Чֵ{off, on}</td>
 *			<td>on</td>
 *			<td>�����ſ��Ʋ���</td>
 *			<td>off: �رշ��ź��ԣ����Է��Ž��й��ˣ����������ĶϾ�����Ŵ���ʽ���д���<br/>
 *				on: �������ź��ԣ��������������������ķ��ţ�����ȫ��Ǿ�š����š��ʺš���̾�š��ո񣬾����չ��˺�����һ�����Ŵ������������򲻻���ˣ���ΪĬ������</td>
 *		</tr>
 *		<tr>
 *			<td>specialOne</td>
 *			<td>�ַ�������Чֵ{yi1,yao1}</td>
 *			<td>��</td>
 *			<td>'1'�Ķ�������v6����֧��</td>
 *			<td>�����ø������Զ��жϣ�'1'�Ķ������ֱ�Ϊyi1��yao1; ������ڵ籨��������Ч</td>
 *		</tr>
 *		<tr>
 *			<td>specialTwo</td>
 *			<td>�ַ�������Чֵ{er4,liang3}</td>
 *			<td>��</td>
 *			<td>'2'�Ķ�������v6����֧��</td>
 *			<td>�����ø������Զ��жϣ�'2'�Ķ������ֱ�Ϊer4��liang3; ���������Ŀ��������Ч</td>
 *		</tr>
 *		<tr>
 *			<td>gainFactor</td>
 *			<td>��������Χ[2,8]</td>
 *			<td>2</td>
 *			<td>����ϵ������v6����֧��</td>
 *			<td>����Խ������Ч��Խ���ԣ�ͬʱ��������ϵ����������������Ч��������ϳɵ������������</td>
 *		</tr>
 *		<tr>
 *			<td>speedUp</td>
 *			<td>�ַ�������Чֵ{0,1,2,3}</td>
 *			<td>0</td>
 *			<td>�����������</td>
 *			<td>
 *				0: �����м���<br/>
 *				1: ���٣����Ż�<br/>
 *				2: ���٣��Ż�%30<br/>
 *				3: ���٣��Ż�%50<br/>
 *				ע�⣺�������ټ�ʹ�ü򻯹��������㷨��cpu���½�����Ӱ�����ʣ�ֻ����ϵͳ�����ر�͵�ʱ��Ž��鿪����</td>
 *		</tr>
 *	</table>
 * @n@n
 * @par ���ر������������
 *  <table>
 *		<tr>
 *			<td>backaudiopath</td>
 *			<td>�ַ������磺./tts/backaudio.wav </td>
 *			<td>��</td>
 *			<td>�������ļ�����·��</td>
 *			<td>Ŀǰ��������֧��Ϊ16k16Bit PCM �� WAV�ļ��������˲�������ʹ�ñ��������ܡ�</td>
 *		</tr>
 *		<tr>
 *			<td>backaudiovolume</td>
 *			<td>����������Χ[0,9.99]</td>
 *			<td>5</td>
 *			<td>��������</td>
 *			<td>����Խ������Խ����Ҫ��֤���ô�С��λ��������λ���ݲ�֧��exponent�����ʾ�ĸ������ַ���</td>
 *		</tr>
 *		<tr>
 *			<td>backaudiorepeat</td>
 *			<td>�ַ�������Чֵ{yes��no}</td>
 *			<td>yes</td>
 *			<td>�Ƿ�ѭ�����ű�����</td>
 *			<td>yes��ѭ������\n no,��ѭ������</td>
 *		</tr>
 *	</table>
 * @n@n
 * ����û�ж�����������ʹ�� session_start ʱ�Ķ��塣��� session_start ʱҲû�ж��壬��ʹ��ȱʡֵ
 *
 * @note
 * 1. ������ͨ���ص������ķ�ʽ�������ݣ�����Ȼ��ͬ����ʽ��Ҳ��ֻ���������ݶ�ͨ���ص�����������
 * �������Ż᷵��;@n
 * 2. �������Ƿֶκϳɣ�TTS_SYNTH_RESULT�е�pszTextΪ��ǰ���ںϳ��ı��ε��׵�ַ��
 * �����ǰ�ı��εĺϳ�������Ҫ��λص�����ֻ�ڵ�һ�λص�ʱ�������ı��ε���Ϣ��
 * ����ص��У��õ�ַΪNULL;@n
 * 3. �ϳɵĹ����У������ǰ�ı���Ϊ���е�������ص�����û���������ݷ��أ���pvVoiceData����ΪNULL��<br/>

  */ 

 #ifdef PRIVATE_CLOUD__
 /**
 @n@n
 *  ���֣������߼��������£�
 *	<table>
 *		<tr>
 *			<td><b>������</b></td>
 *			<td><b>����</b></td>
 *			<td><b>˵��</b></td>
 *			<td><b>֧��tagmode</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>xiaokun</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����Ů��</b></td>
 *			<td><b>none,auto,s3ml,vtml,ssml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>wangjing</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����Ů��</b></td>
 *			<td><b>none,auto,s3ml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>haobo</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>��������</b></td>
 *			<td><b>none,auto,s3ml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>uyghur</b></td>
 *			<td><b>uyghur</b></td>
 *			<td><b>ά��Ů��</b></td>
 *			<td><b>none,s3ml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>baisong</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����������Ƕ��ʽ���⣩</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>shenxue</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����������Ƕ��ʽ���⣩</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>xiaokune</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����Ů����Ƕ��ʽ���⣩</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>zhangnane</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����Ů����Ƕ��ʽ���⣩</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>xixie</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>����ͯ����Ƕ��ʽ���⣩</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *	</table>
 * domain�����򣩣�ȡֵ��Χ�����壨��xiaokun�⣬����������ֻ��ȡĬ��ֵcommon��
 *	<table>
 *		<tr>
 *			<td><b>��Чdomain����</b></td>
 *			<td><b>����˵��</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>common</b></td>
 *			<td><b>Ĭ��ֵ</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>finance</b></td>
 *			<td><b>����֤ȯ</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>weather</b></td>
 *			<td><b>����Ԥ��</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>queue</b></td>
 *			<td><b>�Ŷӽк�</b></td>
 *		</tr> 
 *		<tr>
 *			<td><b>insurance</b></td>
 *			<td><b>����</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>voyage</b></td>
 *			<td><b>����</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>revenue</b></td>
 *			<td><b>˰��</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>elecpower</b></td>
 *			<td><b>����</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>message</b></td>
 *			<td><b>����</b></td>
 *		</tr>
 *	</table>
 */
#endif

HCI_ERR_CODE HCIAPI hci_tts_synth(
		_MUST_ _IN_ int nSessionId,
		_MUST_ _IN_ char * pvText,			
		_OPT_ _IN_ const char * pszConfig,
		_MUST_ _IN_ Callback_TtsSynth pfnTtsSynth,
		_OPT_ _IN_ void * pvUserParam
		);

/**  
 * @brief	�����Ự
 * @param	nSessionId		�ỰID
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>�����Session�Ƿ�</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_SESSION_BUSY</td><td>HCI TTSæ������hci_tts_session_synthִ����</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_tts_session_stop(
		_MUST_ _IN_ int nSessionId
		);

/**  
 * @brief	����TTS���� ����ʼ��
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>�����ɹ�</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS��δ��ʼ��</td></tr>
 *		<tr><td>@ref HCI_ERR_ACTIVE_SESSION_EXIST</td><td>����δstop��Sesssion���޷�����</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_tts_release();

/* @} */

/* @} */
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
};
#endif

#endif // _hci_tts_header_
