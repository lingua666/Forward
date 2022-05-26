#ifndef __COMMON_TOOL_H__
#define __COMMON_TOOL_H__

namespace HciExampleComon{
    //���ÿ���̨��ӡ��ɫ
    void SetSpecialConsoleTextAttribute();
    //�ָ�����̨��ӡ��ɫ
    void SetOriginalConsoleTextAttribute();
    //����̨��ӡUTF8�ַ���windowsת��ΪGBK��
    void PrintUtf8String(char *pUTF8Str);

    //ת�뺯��
    int UTF8ToGBK(unsigned char * pUTF8Str,unsigned char ** pGBKStr);
	int GBKToUTF8(unsigned char * pGBKStr,unsigned char ** pUTF8Str);
    //ת���ڴ��ͷź���
    void FreeConvertResult(unsigned char * pConvertResult);

}

#endif // __FILE_UTIL_H__
