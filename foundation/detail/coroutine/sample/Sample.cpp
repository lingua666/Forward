// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

typedef signed __int64     Int64;
typedef unsigned __int64   UInt64;
typedef unsigned char      UInt8;

#define	_CALLTYPE	__stdcall


/*****************************************************************
/*����˵��:	����������������
/*����˵��:	LowStr: ������������Ƿ����Сд��ĸ
/*			UpStr:  ������������Ƿ������д��ĸ
/*			NumStr: ������������Ƿ��������
/*			SignStr: ������������Ƿ��������
/*			TempLen: ���������������
/*����ֵ��	�����
*****************************************************************/
typedef const char* (_CALLTYPE *MStrRandomEx)(bool LowStr, bool UpStr, bool NumStr, bool SignStr, int TempLen);


/*****************************************************************
/*����˵��:	Сд��ĸ�����
/*����˵��:	TempLen: ���������������
/*����ֵ��	�����
*****************************************************************/
typedef const char* (_CALLTYPE *MLowStrRandom)(int TempLen);

/*****************************************************************
/*����˵��:	��д��ĸ�����
/*����˵��:	TempLen: ���������������
/*����ֵ��	�����
*****************************************************************/
typedef const char* (_CALLTYPE *MUpStrRandom)(int TempLen);

/*****************************************************************
/*����˵��:	���������
/*����˵��:	TempLen: ���������������
/*����ֵ��	�����
*****************************************************************/
typedef const char* (_CALLTYPE *MNumStrRandom)(int TempLen);


/*****************************************************************
/*����˵��:	��һ��������������������
/*����˵��:	TempMax: �����������
/*			TempMin: ��С��������
/*			TempLen: ������С���㱣��λ ��Χ: 1 - 15
/*����ֵ��	�����
*****************************************************************/
typedef const char* (_CALLTYPE *MFloatStrRandom)(int TempMax, int TempMin, int TempLen);


int MRandomTest( void )
{
	HINSTANCE hDllInst = LoadLibraryA("D:\\RandomModule\\Win32\\Release\\MRandom.dll");
	if (hDllInst == NULL)
		return -1;

	MStrRandomEx StrRandomEx = (MStrRandomEx)GetProcAddress(hDllInst, "StrRandomEx");
	MLowStrRandom LowStrRandom = (MLowStrRandom)GetProcAddress(hDllInst, "LowStrRandom");
	MUpStrRandom UpStrRandom = (MUpStrRandom)GetProcAddress(hDllInst, "UpStrRandom");
	MNumStrRandom NumStrRandom = (MNumStrRandom)GetProcAddress(hDllInst, "NumStrRandom");
	MFloatStrRandom FloatStrRandom = (MFloatStrRandom)GetProcAddress(hDllInst, "FloatStrRandom");

	char szData[100] = {0};
	strcpy(szData, StrRandomEx(true, true, true, true, 20));
	printf("StrRandomEx:%s\r\n", szData);

	strcpy(szData, LowStrRandom(20));
	printf("LowStrRandom:%s\r\n", szData);

	strcpy(szData, UpStrRandom(20));
	printf("UpStrRandom:%s\r\n", szData);

	strcpy(szData, NumStrRandom(20));
	printf("NumStrRandom:%s\r\n", szData);

	strcpy(szData, FloatStrRandom(10000, 1, 20));
	printf("FloatStrRandom:%s\r\n", szData);
}

struct TPicInfo
{
	bool Result;
	int PicTypeIndex;
	char* PicName;
	Int64 BufSize;
	char* PPicBuf;
};

/*****************************************************************
/*����˵��:	���ڴ��е�ͼ��������ʾ����
/*����˵��:	PTmpByte: �����������
/*			TmpSize: ��С��������
/*����ֵ��	
*****************************************************************/
typedef void (_CALLTYPE *MShowFrmEx)(const UInt8* PTmpByte, Int64 TmpSize);


/*****************************************************************
/*����˵��:	��·���ļ����ص��ڴ�
/*����˵��:	
/*			
/*����ֵ��	�ļ��ڴ�ռ���Ϣ
*****************************************************************/
typedef TPicInfo* (_CALLTYPE *MGetPicFormDllEx)(const char* PTmpPicPath);


int PicTest( void )
{
	//HINSTANCE hDllInst = LoadLibraryA("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\ͼƬ��ָ�����BufV4.0\\M_PicRes.dll");
	HINSTANCE hDllInst = LoadLibraryA("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\ͼƬ��ָ�����BufV4.0PChar\\M_PicRes.dll");
	if (hDllInst == NULL)
		return -1;

	MShowFrmEx ShowFrmEx = (MShowFrmEx)GetProcAddress(hDllInst, "ShowFrmEx");
	MGetPicFormDllEx GetPicFormDllEx = (MGetPicFormDllEx)GetProcAddress(hDllInst, "GetPicFormDllEx");

	int iLen = 0;
	/*UInt8* pData = new UInt8[1024 * 5000];
	memset(pData, 9, 1024 * 5000);
	FILE* rp = fopen("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\ͼƬ��ָ�����BufV4.0\\ClientBG.png", "rb");
	if(rp != NULL)
	{
		 iLen = fread(pData, 1, 1024 * 5000, rp);
		 fclose(rp);
	}

	ShowFrmEx(pData, iLen);*/

	TPicInfo* pInfo = GetPicFormDllEx("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\ͼƬ��ָ�����BufV4.0\\ClientBG.png");

	FILE* fp = fopen("D:\\ClientBG.png", "wb");
	int iOffset = 0;
	while( iOffset < pInfo->BufSize )
	{
		iLen = __min(2048, pInfo->BufSize - iOffset);
		fwrite(pInfo->PPicBuf, 1, iLen, fp);
		fflush(fp);
		iOffset += iLen;
		pInfo->PPicBuf += iLen;
	}

	fclose(fp);
}


int main(int argc, char* argv[])
{
	PicTest();

	std::cin.get();
	return 0;
}