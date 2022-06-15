
#ifndef __EXPORTS_20201229152241_1444199361_H__
#define __EXPORTS_20201229152241_1444199361_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(dllThirdWrap_Openssl)


/*****************************************************************
/*����˵����	AES����
/*����˵����	c_szKey: ��Կ
/*			c_szInData: ����
/*			uInSize: ���ĳ���
/*			isBase64: �����Ƿ���Ҫ��base64ת��
/*			pOutData: ת������������
/*			uOutSize: ����������
/*����ֵ��	<0:������   >0: ת�����ʵ�ʳ���
/*ע��:     ��pOutDataΪ0ʱ������ֵΪת�����ʵ�ʳ��ȣ��ɸ��ݸý��������ջ��������ڵ��øýӿ�
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MOpenssl_AESEncode(const char* c_szKey,
	const char* c_szInData, UInt32 uInSize, bool isBase64, char* pOutData, UInt32 uOutSize);


/*****************************************************************
/*����˵����	AES����
/*����˵����	c_szKey: ��Կ
/*			c_szInData: ����
/*			uInSize: ���ĳ���
/*			isBase64: �������Ƿ񾭹�base64ת��
/*			pOutData: ת������������
/*			uOutSize: ����������
/*����ֵ��	<0:������   >0: ת�����ʵ�ʳ���
/*ע��:     ��pOutDataΪ0ʱ������ֵΪת�����ʵ�ʳ��ȣ��ɸ��ݸý��������ջ��������ڵ��øýӿ�
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MOpenssl_AESDecode(const char* c_szKey,
		const char* c_szInData, UInt32 uInSize, bool isBase64, char* pOutData, UInt32 uOutSize);

#endif//__PLC_OMRON_20151228191441_1451301281_H__
