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
/*函数说明:	产生多种组合随机数
/*参数说明:	LowStr: 产生的随机数是否包含小写字母
/*			UpStr:  产生的随机数是否包含大写字母
/*			NumStr: 产生的随机数是否包含数字
/*			SignStr: 产生的随机数是否包含符号
/*			TempLen: 产生的随机数长度
/*返回值：	随机数
*****************************************************************/
typedef const char* (_CALLTYPE *MStrRandomEx)(bool LowStr, bool UpStr, bool NumStr, bool SignStr, int TempLen);


/*****************************************************************
/*函数说明:	小写字母随机数
/*参数说明:	TempLen: 产生的随机数长度
/*返回值：	随机数
*****************************************************************/
typedef const char* (_CALLTYPE *MLowStrRandom)(int TempLen);

/*****************************************************************
/*函数说明:	大写字母随机数
/*参数说明:	TempLen: 产生的随机数长度
/*返回值：	随机数
*****************************************************************/
typedef const char* (_CALLTYPE *MUpStrRandom)(int TempLen);

/*****************************************************************
/*函数说明:	数字随机数
/*参数说明:	TempLen: 产生的随机数长度
/*返回值：	随机数
*****************************************************************/
typedef const char* (_CALLTYPE *MNumStrRandom)(int TempLen);


/*****************************************************************
/*函数说明:	在一个区间产生浮点数随机数
/*参数说明:	TempMax: 最大区间数字
/*			TempMin: 最小区间数字
/*			TempLen: 浮点数小数点保留位 范围: 1 - 15
/*返回值：	随机数
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
/*函数说明:	将内存中的图像数据显示出来
/*参数说明:	PTmpByte: 最大区间数字
/*			TmpSize: 最小区间数字
/*返回值：	
*****************************************************************/
typedef void (_CALLTYPE *MShowFrmEx)(const UInt8* PTmpByte, Int64 TmpSize);


/*****************************************************************
/*函数说明:	将路径文件加载到内存
/*参数说明:	
/*			
/*返回值：	文件内存空间信息
*****************************************************************/
typedef TPicInfo* (_CALLTYPE *MGetPicFormDllEx)(const char* PTmpPicPath);


int PicTest( void )
{
	//HINSTANCE hDllInst = LoadLibraryA("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\图片流指针测试BufV4.0\\M_PicRes.dll");
	HINSTANCE hDllInst = LoadLibraryA("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\图片流指针测试BufV4.0PChar\\M_PicRes.dll");
	if (hDllInst == NULL)
		return -1;

	MShowFrmEx ShowFrmEx = (MShowFrmEx)GetProcAddress(hDllInst, "ShowFrmEx");
	MGetPicFormDllEx GetPicFormDllEx = (MGetPicFormDllEx)GetProcAddress(hDllInst, "GetPicFormDllEx");

	int iLen = 0;
	/*UInt8* pData = new UInt8[1024 * 5000];
	memset(pData, 9, 1024 * 5000);
	FILE* rp = fopen("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\图片流指针测试BufV4.0\\ClientBG.png", "rb");
	if(rp != NULL)
	{
		 iLen = fread(pData, 1, 1024 * 5000, rp);
		 fclose(rp);
	}

	ShowFrmEx(pData, iLen);*/

	TPicInfo* pInfo = GetPicFormDllEx("D:\\WeChat Files\\wxid_izobr64bgqqv21\\FileStorage\\File\\2019-02\\图片流指针测试BufV4.0\\ClientBG.png");

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