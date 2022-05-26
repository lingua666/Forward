/** 
 * @file    hci_tts.h 
 * @brief   HCI_TTS SDK 头文件  
 */  

#ifndef __HCI_TTS_HEADER__
#define __HCI_TTS_HEADER__

#include "hci_sys.h"

#ifdef __cplusplus
extern "C"		
{
#endif

/** @defgroup HCI_TTS 灵云TTS能力API */
/* @{ */


    /** @defgroup HCI_TTS_STRUCT  结构体 */
    /* @{ */
    //////////////////////////////////////////////////////////////////////////
    
    /**
	*@brief	模块名称
	*/
    #define TTS_MODULE	"hci_tts"

	/**
	 * @brief mark 标记
	 */
	typedef struct _tag_TTS_SYNTH_MARK
	{
		/// mark标记的名称
		char * pszName;
		///mark在合成音频中的时间点。
		unsigned int time;
	} TTS_SYNTH_MARK;

    /**
     * @brief	TTS 合成结果
     */
    typedef struct _tag_TTS_SYNTH_RESULT
    {
	    /// 数据缓冲区指针，数据格式由合成时候的配置决定
	    void * pvVoiceData;

	    /// 数据缓冲区长度，以字节为单位
	    unsigned int uiVoiceSize;

	    /// 本段数据对应的文本内容的起始指针
	    char * pszText;

	    /// 本段数据对应的文本长度，字节为单位
	    unsigned int uiTextLen;
    	
	    /// 是否还有数据等待合成
	    bool bMore;

		/// mark 标记
		TTS_SYNTH_MARK *pMark;

		/// mark数量。
		unsigned int	nMarkCount;
    } TTS_SYNTH_RESULT;

    /* @} */

    /** @defgroup HCI_TTS_CALLBACK  回调函数 */
    /* @{ */


    /**  
     * @brief	语音合成回调函数
     * @param	pvUserParam			用户自定义回调指针
     * @param	psTtsSynthResult	合成结果的数据结构指针
     * @param	hciErrCode			错误码
     * @return	是否继续合成，返回true表示继续合成，返回false表示中止合成
     *
     * @note
     * psTtsSythResult中的数据在本回调函数返回后，不保证继续有效，因此用户如果需要保留数据进行处理，
     * 需要自行将数据拷贝出去。
     */ 
    typedef bool (HCIAPI * Callback_TtsSynth)(
		    _OPT_ _IN_ void * pvUserParam,
		    _MUST_ _IN_ TTS_SYNTH_RESULT * psTtsSynthResult,
		    _MUST_ _IN_ HCI_ERR_CODE  hciErrCode
		    );

     /* @} */

    /** @defgroup HCI_TTS_FUNC  函数 */
    /* @{ */

/**  
 * @brief	灵云TTS能力 初始化
 * @param	pszConfig		初始化配置串，ASCII字符串，可为NULL或以'\0'结束
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS 尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ALREADY_INIT</td><td>已经初始化过了</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置参数有误，如设定值非法、或格式错误等</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>本地引擎资源路径未配置</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>未找到授权能力项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>能力与Key不匹配</td></tr>
 *	</table>
 *
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>dataPath</td>
 *			<td>opt/myapp/tts_data/</td>
 *			<td>无</td>
 *			<td>语音合成本地资源所在路径</td>
 *			<td>如果不使用本地语音合成能力，可以不传此项</td>
 *		</tr>
 *		<tr>
 *			<td>initCapKeys</td>
 *			<td>tts.local.xixi;tts.local.haobo</td>
 *			<td>无</td>
 *			<td>初始化所需的本地能力</td>
 *			<td>多个能力以';'隔开，如果传入云端能力key，简单忽略。如果不使用本地语音合成能力，可以不传此项</td>
 *		</tr>
 *		<tr>
 *			<td>fileFlag</td>
 *			<td>none, android_so</td>
 *			<td>none</td>
 *			<td>获取本地文件名的特殊标记</td>
 *			<td>参见下面的注释</td>
 *		</tr>
 *	</table>
 *
 *  <b>Android特殊配置</b>
 *  @n
 *  当fileFlag为android_so时，加载本地资源文件时会将正常的库文件名更改为so文件名进行加载。
 *  例如，当使用的库为file.dat时，则实际打开的文件名为libfile.dat.so，这样在Android系统下，
 *  开发者可以按照此规则将本地资源改名后, 放在libs目录下打包入apk。在安装后，这些资源文件
 *  就会放置在/data/data/包名/lib目录下。则可以直接将dataPath配置项指为此目录即可。
 */

HCI_ERR_CODE HCIAPI hci_tts_init(
		_MUST_ _IN_ const char * pszConfig
		);

/**  
 * @brief	开始会话
 * @param	pszConfig		会话配置串，ASCII字符串，以'\0'结束
 * @param	pnSessionId		成功时返回会话ID
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置参数有误，如设定值非法、或格式错误等</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_MISSING</td><td>缺少必需的capKey配置项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>能力与Key不匹配</td></tr>
 *		<tr><td>@ref HCI_ERR_TOO_MANY_SESSION</td><td>创建的Session数量超出限制(256),本地能力需要打开本地资源文件，最终能够成功打开的本地Session个数与本地系统对同时打开文件个数的限制有关。</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>未找到授权能力项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>本地引擎资源路径未配置</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ENGINE_SESSION_START_FAILED</td><td>TTS本地引擎开启会话失败</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>配置参数，引擎不支持</td></tr>
 *		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>本地能力引擎缺失必要的库资源</td></tr>
 *	</table>
 *
 *
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>capKey</td>
 *			<td>字符串，参考 @ref hci_tts_page </td>
 *			<td>无</td>
 *			<td>语音合成能力key</td>
 *			<td>此项必填。每个session只能定义一种能力，并且过程中不能改变。</td>
 *		</tr>
 *		<tr>
 *			<td>resPrefix</td>
 *			<td></td>
 *			<td>无</td>
 *			<td>资源加载前缀</td>
 *			<td>如果不同会话需要使用同一路径下资源时可以使用该字段对统一路径下的资源进行区分</td>
 *		</tr>
 *	</table>
 * @n@n
 * 另外，这里还可以传入合成的配置项，作为默认配置项。参见 hci_tts_synth() 。
 */
HCI_ERR_CODE HCIAPI hci_tts_session_start(
		_MUST_ _IN_ const char * pszConfig,
		_MUST_ _OUT_ int * pnSessionId
		);

/**  
 * @brief	进行语音合成操作
 * @param	nSessionId		会话ID
 * @param	pvText			要合成的文本数据，UTF-8编码，最大长度为1024字节（本地歌唱能力支持最大长度为8192字节，本地歌唱能力暂不支持传入无歌曲内容文本），以'\0'结束
 * @param	pszConfig		合成参数配置串，ASCII字符串，可为NULL或以'\0'结束
 * @param	pfnTtsSynth		语音合成结果回调函数
 * @param	pvUserParam		用户自定义回调数据指针
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置参数有误，如设定值非法、或格式错误等</td></tr>
 *		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session无效</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_SESSION_BUSY</td><td>TTS当前会话正忙</td></tr>
 *		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>未找到授权能力项</td></tr>
 *		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>配置参数，引擎不支持</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_ENGINE_FAILED</td><td>本地引擎合成失败</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>连接服务器失败，服务器无响应</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>服务器访问超时</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>服务器返回的数据格式不正确</td></tr>
 *		<tr><td>@ref HCI_ERR_SERVICE_RESPONSE_FAILED</td><td>服务器返回合成失败</td></tr>
 *		<tr><td>@ref HCI_ERR_LOAD_CODEC_DLL</td><td>加载codec编码库失败</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_CONFIG_PROPERTY_MISSING</td><td>TTS 云端property参数缺失</td></tr>
 *	</table>
 */

#ifdef PRIVATE_CLOUD__

 /**
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 * @par 基本配置
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>pitch</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>合成语音的基频</td>
 *			<td>数字越大基频越高，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>volume</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>合成语音的音量</td>
 *			<td>数字越大音量越大，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>speed</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>合成语音的语速</td>
 *			<td>数字越大语速越快，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>audioFormat</td>
 *			<td>字符串，有效值见详细描述</td> 
 *			<td>pcm16k16bit</td>
 *			<td>输出音频格式</td>
 *			<td>云端引擎支持的音频格式:<br/>
 *				auto<br/>
 *				vox6k4bit, vox8k4bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				mp3   （mp3 64kbps 格式）<br/>
 *				mp3_24（mp3 24kbps 格式）<br/>
 *				mp3_16（mp3 16kbps 格式）<br/>
 *				本地引擎支持的音频格式:<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/></td> 
 *		</tr>
 *		<tr>
 *			<td>puncMode</td>
 *			<td>字符串，有效值{off, on}</td>
 *			<td>off</td>
 *			<td>标点符号读法</td>
 *			<td>off: 不合成标点符号<br/>
 *				on: 合成标点符号<br/>
 *		</tr>
 *		<tr>
 *			<td>engMode</td>
 *			<td>字符串，有效值{auto, english, letter}</td>
 *			<td>auto</td>
 *			<td>英文阅读方式</td>
 *			<td>auto: 自动判断。 <br /> 
 *				english: 强制所有英文用字母或自录音词汇合成。<br /> 
 *				letter: 强制所有英文按照单字母方式合成。 </td> 
 *		</tr>
 *		<tr>
 *			<td>digitMode</td>
 *			<td>字符串，有效值{auto_number, auto_telegram, number, telegram}</td>
 *			<td>auto_number</td>
 *			<td>数字阅读方式</td>
 *			<td>auto_number: 自动方式，即根据上下文自动选择电报方式或数目方式合成，如果无法判断，则使用数目方式。<br /> 
 *				auto_telegram: 自动方式，即根据上下文自动选择电报方式或数目方式合成，如果无法判断，则使用电报方式。<br />
 *				number: 数目方式，即连续的数字合成。例如：1997读成“一千九百九十七”。 <br />
 *				telegram: 电报方式，即一个数字一个数字合成。例如：1997读成“一九九七”。 </td>
 *		</tr>
 *		<tr>
 *			<td>tagmode</td>
 *			<td>字符串，有效值{none,auto,vtml,ssml,s3ml,s3ml_sing}</td>
 *			<td>none</td>
 *			<td>标记处理方式</td>
 *			<td>none: 所有标记将会被视为文本读出，缺省值<br/>
 *				auto: 自动判断标记类型<br/>
 *				vtml: 处理VTML标记 <br/>
 *				ssml: 处理SSML标记 <br/> 
 *				注意: 本地端仅支持 none, s3ml和 s3ml_sing(本地s3ml暂时只支持：segment、prosody、mark) <br/> 
 *					  云端不支持s3ml_sing
 *				更多S3ML的详细介绍，请参照：《捷通华声iHear7.0_Embedded S3ML 标记语言使用说明》 </td> 
 *		</tr>
 *	</table>
 */
#else
 /**
 * @par 配置串定义：
 * 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
 * @n@n
 * @par 基本配置
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>pitch</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>合成语音的基频</td>
 *			<td>数字越大基频越高，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>volume</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>合成语音的音量</td>
 *			<td>数字越大音量越大，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>speed</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>合成语音的语速</td>
 *			<td>数字越大语速越快，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>audioFormat</td>
 *			<td>字符串，有效值见详细描述</td> 
 *			<td>pcm16k16bit</td>
 *			<td>输出音频格式</td>
 *			<td>云端引擎支持的音频格式:<br/>
 *				auto<br/>
 *				vox6k4bit, vox8k4bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				mp3   （mp3 64kbps 格式）<br/>
 *				mp3_24（mp3 24kbps 格式）<br/>
 *				mp3_16（mp3 16kbps 格式）<br/>
 *				本地引擎支持的音频格式:<br/>
 *				pcm8k8bit, pcm8k16bit<br/>
 *				pcm11k8bit, pcm11k16bit<br/>
 *				pcm16k8bit, pcm16k16bit<br/>
 *				alaw8k8bit, ulaw8k8bit<br/></td> 
 *		</tr>
 *		<tr>
 *			<td>puncMode</td>
 *			<td>off, on, off_rtn, on_rtn</td>
 *			<td>off</td>
 *			<td>标点符号读法</td>
 *			<td>off: 不合成标点符号，自动判断回车换行是否分隔符<br/>
 *				on: 合成标点符号，自动判断回车换行是否分隔符<br/>
 *				off_rtn: 不读符号，强制将回车换行作为分隔符(仅云端支持)<br/>
 *				on_rtn: 读符号，强制将回车换行作为分隔符(仅云端支持)</td>
 *		</tr>
 *		<tr>
 *			<td>engMode</td>
 *			<td>字符串，有效值{auto, english, letter}</td>
 *			<td>auto</td>
 *			<td>英文阅读方式</td>
 *			<td>auto: 自动判断。 <br /> 
 *				english: 强制所有英文用字母或自录音词汇合成。<br /> 
 *				letter: 强制所有英文按照单字母方式合成。 </td> 
 *		</tr>
 *		<tr>
 *			<td>digitMode</td>
 *			<td>字符串，有效值{auto_number, auto_telegram, number, telegram}</td>
 *			<td>auto_number</td>
 *			<td>数字阅读方式</td>
 *			<td>auto_number: 自动方式，即根据上下文自动选择电报方式或数目方式合成，如果无法判断，则使用数目方式。<br /> 
 *				auto_telegram: 自动方式，即根据上下文自动选择电报方式或数目方式合成，如果无法判断，则使用电报方式。<br />
 *				number: 数目方式，即连续的数字合成。例如：1997读成“一千九百九十七”。 <br />
 *				telegram: 电报方式，即一个数字一个数字合成。例如：1997读成“一九九七”。 </td>
 *		</tr>
 *		<tr>
 *			<td>tagmode（仅本地支持）</td>
 *			<td>字符串，有效值{none,s3ml,s3ml_sing}</td>
 *			<td>none</td>
 *			<td>标记处理方式</td>
 *			<td>none: 所有标记将会被视为文本读出<br/>
 *				s3ml_sing:所有标记将被视为乐谱唱出，传入的合成文本必须严格遵循s3ml格式<br/>
 *				s3ml暂时只支持：segment、prosody、mark <br/> 
 *				更多S3ML的详细介绍，请参照：《捷通华声iHear7.0_Embedded S3ML 标记语言使用说明》 </td>
 *		</tr>
 *		<tr>
 *			<td>tempo（仅tts.local.synth.sing支持）</td>
 *			<td>整数，25~1275</td>
 *			<td>380</td>
 *			<td>语速</td>
 *			<td>每拍唱几毫秒</td>
 *		</tr>
 *		<tr>
 *			<td>interval（仅tts.local.synth.sing支持）</td>
 *			<td>整数，-16~16</td>
 *			<td>0</td>
 *			<td>音高变化区间</td>
 *			<td>比默认C调升高或降低几个半音</td>
 *		</tr>
 *	</table>
 */
#endif

#ifdef PRIVATE_CLOUD__
 /**
 * @n@n
 * @par 云端配置
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>encode</td>
 *			<td>字符串，有效值{none, opus, speex}</td>
 *			<td>opus</td>
 *			<td>使用的编码格式，只对云端合成有效，本地合成忽略该配置</td>
 *			<td>表示云端合成传输的时候采用的编码格式，当audioFormat为vox6k4bit/vox8k4bit/mp3时，<br/>
 *              不支持使用opus,speex（直接忽略配置），其余均可采用opus,speex进行压缩传输，以减少数据流量</td>
 *		</tr> 
 *		<tr>
 *			<td>encLevel</td>
 *			<td>整数，范围[0,10]</td>
 *			<td>7</td>
 *			<td>压缩等级，只对云端合成有效，本地合成忽略该配置。数值与压缩率成反比</td>
 *			<td></td>
 *		</tr>
 *		<tr>
 *			<td>property</td>
 *			<td>字符串，例如：cn_xiaokun_common</td>
 *			<td>无</td>
 *			<td>引擎参数，取值是语种lang、发音者speaker和领域domain组合。</td>
 *			<td>必选，有效组合参见文档末尾的说明<br/>
 *			</td>
 *		</tr>
 *	</table>
 */
#else
  /**
 * @n@n
 * @par 云端配置
 *	<table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>encode</td>
 *			<td>字符串，有效值{none, opus, speex}</td>
 *			<td>opus</td>
 *			<td>使用的编码格式，只对云端合成有效，本地合成忽略该配置</td>
 *			<td>表示云端合成传输的时候采用的编码格式，当audioFormat为vox6k4bit/vox8k4bit/mp3时，<br/>
 *              不支持使用opus,speex（直接忽略配置），其余均可采用opus,speex进行压缩传输，以减少数据流量</td>
 *		</tr> 
 *		<tr>
 *			<td>encLevel</td>
 *			<td>整数，范围[0,10]</td>
 *			<td>7</td>
 *			<td>压缩等级，只对云端合成有效，本地合成忽略该配置。数值与压缩率成反比</td>
 *			<td></td>
 *		</tr>
 *		<tr>
 *			<td>domain</td>
 *			<td>finance</td>
 *			<td>无</td>
 *			<td>合成使用的领域</td>
 *			<td>使用指定领域的音库进行合成。如果capKey的能力未指定特定领域，
 *				则可以使用domain进行进一步指定领域。如果capKey的能力已经指定了特定领域，
 *				则该配置项无效。<br/>
 *				common      通用领域，新闻<br/>
 *				finance     金融证券<br/>
 *				weather     天气预报<br/>
 *				sports      体育赛事<br/>
 *				traffic     公交信息<br/>
 *				travel      旅游餐饮<br/>
 *				carborne    汽运<br/>
 *				queue       排队<br/>
 *				song        点歌<br/>
 *				insurance	保险<br/>
 *				voyage      航空<br/>
 *				revenue     税务<br/>
 *				elecpower   电力<br/>
 *				message     短信<br/>
 *				finainsu    金融保险<br/>
 *				telecom     电信<br/>
 *				custom      自定义<br/>
 *				此项可选。具体可用的领域可以在开发时咨询捷通华声。</td>
 *		</tr>
 *	</table>
 */
#endif
 /**
 * @n@n
 * @par 本地高级配置
 *  <table>
 *		<tr>
 *			<td><b>字段</b></td>
 *			<td><b>取值或示例</b></td>
 *			<td><b>缺省值</b></td>
 *			<td><b>含义</b></td>
 *			<td><b>详细说明</b></td>
 *		</tr>
 *		<tr>
 *			<td>soundEffect</td>
 *			<td>字符串，有效值{base,reverb,echo,chorus,nearfar,robot}</td>
 *			<td>base</td>
 *			<td>音效设置</td>
 *			<td>base: 无音效<br/>
 *				reverb: 混响<br/>
 *				echo: 回声<br/>
 *				chorus: 合唱<br/>
 *				nearfar: 忽远忽近<br/>
 *				robot: 机器人</td> 
 *		</tr>
 *		<tr>
 *			<td>mixSound</td>
 *			<td>整数，范围[1,9]</td>
 *			<td>5</td>
 *			<td>混响时长</td>
 *			<td>数字越大混音时间越长,该配置只有在soundEffect设置为echo时生效</td>
 *		</tr>
 *		<tr>
 *			<td>namePolyphone</td>
 *			<td>字符串，有效值{off, on}</td>
 *			<td>off</td>
 *			<td>姓氏多音字开关</td>
 *			<td>off: 关闭<br/>
 *				on: 打开</td>
 *		</tr>
 *		<tr>
 *			<td>symbolFilter</td>
 *			<td>字符串，有效值{off, on}</td>
 *			<td>on</td>
 *			<td>标点符号控制参数</td>
 *			<td>off: 关闭符号忽略：不对符号进行过滤，按照正常的断句与符号处理方式进行处理<br/>
 *				on: 开启符号忽略：六个或六个以上连续的符号，包括全半角句号、逗号、问号、感叹号、空格，均按照过滤后保留第一个符号处理，不足六个则不会过滤；此为默认设置</td>
 *		</tr>
 *		<tr>
 *			<td>specialOne</td>
 *			<td>字符串，有效值{yi1,yao1}</td>
 *			<td>无</td>
 *			<td>'1'的读法，仅v6能力支持</td>
 *			<td>不设置该项则自动判断，'1'的读法，分别为yi1和yao1; 此项仅在电报读法下有效</td>
 *		</tr>
 *		<tr>
 *			<td>specialTwo</td>
 *			<td>字符串，有效值{er4,liang3}</td>
 *			<td>无</td>
 *			<td>'2'的读法，仅v6能力支持</td>
 *			<td>不设置该项则自动判断，'2'的读法，分别为er4和liang3; 此项仅在数目读法下有效</td>
 *		</tr>
 *		<tr>
 *			<td>gainFactor</td>
 *			<td>整数，范围[2,8]</td>
 *			<td>2</td>
 *			<td>增益系数，仅v6能力支持</td>
 *			<td>数字越大增益效果越明显，同时调大增益系数与音量，可以有效提升引擎合成的整体音量输出</td>
 *		</tr>
 *		<tr>
 *			<td>speedUp</td>
 *			<td>字符串，有效值{0,1,2,3}</td>
 *			<td>0</td>
 *			<td>引擎加速设置</td>
 *			<td>
 *				0: 不进行加速<br/>
 *				1: 加速，不优化<br/>
 *				2: 加速，优化%30<br/>
 *				3: 加速，优化%50<br/>
 *				注意：开启加速即使用简化过的引擎算法，cpu会下降，但影响音质，只有在系统配置特别低的时候才建议开启。</td>
 *		</tr>
 *	</table>
 * @n@n
 * @par 本地背景音相关配置
 *  <table>
 *		<tr>
 *			<td>backaudiopath</td>
 *			<td>字符串，如：./tts/backaudio.wav </td>
 *			<td>无</td>
 *			<td>背景音文件完整路径</td>
 *			<td>目前背景音仅支持为16k16Bit PCM 或 WAV文件。不传此参数，则不使用背景音功能。</td>
 *		</tr>
 *		<tr>
 *			<td>backaudiovolume</td>
 *			<td>浮点数，范围[0,9.99]</td>
 *			<td>5</td>
 *			<td>背景音量</td>
 *			<td>数字越大音量越大，需要保证配置串小数位不超过两位，暂不支持exponent阶码表示的浮点数字符串</td>
 *		</tr>
 *		<tr>
 *			<td>backaudiorepeat</td>
 *			<td>字符串，有效值{yes，no}</td>
 *			<td>yes</td>
 *			<td>是否循环播放背景音</td>
 *			<td>yes，循环播放\n no,不循环播放</td>
 *		</tr>
 *	</table>
 * @n@n
 * 这里没有定义的配置项，会使用 session_start 时的定义。如果 session_start 时也没有定义，则使用缺省值
 *
 * @note
 * 1. 本函数通过回调函数的方式给出数据，但仍然是同步方式，也即只有所有数据都通过回调函数给出后，
 * 本函数才会返回;@n
 * 2. 本函数是分段合成，TTS_SYNTH_RESULT中的pszText为当前正在合成文本段的首地址，
 * 如果当前文本段的合成数据需要多次回调，则只在第一次回调时给出该文本段的信息，
 * 其余回调中，该地址为NULL;@n
 * 3. 合成的过程中，如果当前文本段为空行等情况，回调可能没有声音数据返回，即pvVoiceData可能为NULL。<br/>

  */ 

 #ifdef PRIVATE_CLOUD__
 /**
 @n@n
 *  语种，发音者及含义如下：
 *	<table>
 *		<tr>
 *			<td><b>发音人</b></td>
 *			<td><b>语种</b></td>
 *			<td><b>说明</b></td>
 *			<td><b>支持tagmode</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>xiaokun</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文女声</b></td>
 *			<td><b>none,auto,s3ml,vtml,ssml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>wangjing</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文女声</b></td>
 *			<td><b>none,auto,s3ml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>haobo</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文男声</b></td>
 *			<td><b>none,auto,s3ml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>uyghur</b></td>
 *			<td><b>uyghur</b></td>
 *			<td><b>维语女声</b></td>
 *			<td><b>none,s3ml</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>baisong</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文男声（嵌入式音库）</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>shenxue</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文男声（嵌入式音库）</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>xiaokune</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文女声（嵌入式音库）</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>zhangnane</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文女声（嵌入式音库）</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>xixie</b></td>
 *			<td><b>cn</b></td>
 *			<td><b>中文童声（嵌入式音库）</b></td>
 *			<td><b>none</b></td>
 *		</tr>
 *	</table>
 * domain（领域），取值范围及含义（除xiaokun外，其他发音者只能取默认值common）
 *	<table>
 *		<tr>
 *			<td><b>有效domain参数</b></td>
 *			<td><b>参数说明</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>common</b></td>
 *			<td><b>默认值</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>finance</b></td>
 *			<td><b>金融证券</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>weather</b></td>
 *			<td><b>天气预报</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>queue</b></td>
 *			<td><b>排队叫号</b></td>
 *		</tr> 
 *		<tr>
 *			<td><b>insurance</b></td>
 *			<td><b>保险</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>voyage</b></td>
 *			<td><b>航空</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>revenue</b></td>
 *			<td><b>税务</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>elecpower</b></td>
 *			<td><b>电力</b></td>
 *		</tr>
 *		<tr>
 *			<td><b>message</b></td>
 *			<td><b>短信</b></td>
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
 * @brief	结束会话
 * @param	nSessionId		会话ID
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_SESSION_BUSY</td><td>HCI TTS忙，尚有hci_tts_session_synth执行中</td></tr>
 *	</table>
 */ 
HCI_ERR_CODE HCIAPI hci_tts_session_stop(
		_MUST_ _IN_ int nSessionId
		);

/**  
 * @brief	灵云TTS能力 反初始化
 * @return
 * @n
 *	<table>
 *		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
 *		<tr><td>@ref HCI_ERR_TTS_NOT_INIT</td><td>HCI TTS尚未初始化</td></tr>
 *		<tr><td>@ref HCI_ERR_ACTIVE_SESSION_EXIST</td><td>尚有未stop的Sesssion，无法结束</td></tr>
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
