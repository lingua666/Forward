
#include <dllThirdWrap_Openssl/Exports.h>
#include <libThirdWrap_Openssl/Aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172205,
	"45000001000V2016021700000000VZZZ10000000",
	"系统通信指令模块",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(dllThirdWrap_Openssl)

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
	const char* c_szInData, UInt32 uInSize, bool isBase64, char* pOutData, UInt32 uOutSize)
{
	if (c_szInData == NULL || uInSize <= 0 || uOutSize <= 0)
		return -1;

	int iLen = 0;
	_openssl_::Aes	AES;
	AES.Init(c_szKey);
	_openssl_::_openssl_string sEnc = AES.Encrypt((unsigned char*)c_szInData, uInSize);
	if (isBase64)
	{//base64转换
		std::string sRet = _common_::base64_encode((unsigned char*)sEnc.c_str(), sEnc.size());
		if (pOutData != NULL)
		{
			iLen = __min(uOutSize, sRet.size());
			memcpy(pOutData, sRet.c_str(), iLen);
		}
		else
			iLen = sRet.size();
	}
	else
	{
		if (pOutData != NULL)
		{
			iLen = __min(uOutSize, sEnc.size());
			memcpy(pOutData, sEnc.c_str(), iLen);
		}
		else
			iLen = sEnc.size();
	}

	return iLen;
}


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
	const char* c_szInData, UInt32 uInSize, bool isBase64, char* pOutData, UInt32 uOutSize)
{
	if (c_szInData == NULL || uInSize <= 0 || uOutSize <= 0)
		return -1;

	std::string sRet;
	if (isBase64)
		sRet = _common_::base64_decode((const unsigned char*)c_szInData, uInSize);
	else
		sRet.append(c_szInData, uInSize);

	_openssl_::Aes	AES;
	AES.Init(c_szKey);
	_openssl_::_openssl_string sData = AES.Decrypt((unsigned char*)sRet.c_str(), sRet.size());

	int iLen = 0;
	int iOffset = sData.size();
	while (iOffset > 0)
	{
		if (sData[iOffset - 1] != 0)
			break;

		iOffset--;
	}

	sData.update_size(iOffset);

	if (pOutData != NULL)
	{
		iLen = __min(uOutSize, sData.size());
		memcpy(pOutData, sData.c_str(), iLen);
	}
	else
		iLen = sRet.size();

	return iLen;
}

