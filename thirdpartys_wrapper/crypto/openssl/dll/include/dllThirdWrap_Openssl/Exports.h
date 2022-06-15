
#ifndef __EXPORTS_20201229152241_1444199361_H__
#define __EXPORTS_20201229152241_1444199361_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(dllThirdWrap_Openssl)


/*****************************************************************
/*函数说明：	AES加密
/*参数说明：	c_szKey: 秘钥
/*			c_szInData: 明文
/*			uInSize: 明文长度
/*			isBase64: 密文是否需要经base64转换
/*			pOutData: 转换后结果缓冲区
/*			uOutSize: 缓冲区长度
/*返回值：	<0:错误码   >0: 转换后的实际长度
/*注意:     当pOutData为0时，返回值为转换后的实际长度，可根据该结果分配接收缓冲区，在调用该接口
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MOpenssl_AESEncode(const char* c_szKey,
	const char* c_szInData, UInt32 uInSize, bool isBase64, char* pOutData, UInt32 uOutSize);


/*****************************************************************
/*函数说明：	AES解密
/*参数说明：	c_szKey: 秘钥
/*			c_szInData: 密文
/*			uInSize: 密文长度
/*			isBase64: 该密文是否经过base64转换
/*			pOutData: 转换后结果缓冲区
/*			uOutSize: 缓冲区长度
/*返回值：	<0:错误码   >0: 转换后的实际长度
/*注意:     当pOutData为0时，返回值为转换后的实际长度，可根据该结果分配接收缓冲区，在调用该接口
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MOpenssl_AESDecode(const char* c_szKey,
		const char* c_szInData, UInt32 uInSize, bool isBase64, char* pOutData, UInt32 uOutSize);

#endif//__PLC_OMRON_20151228191441_1451301281_H__
