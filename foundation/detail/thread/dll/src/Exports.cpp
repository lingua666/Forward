
#include <dllThread/Exports.h>
#include <libCommon/os/Platform.h>

EXTERN_C _SYMBOL_DLL_EXPORTS char* _CALLTYPE GetMemory( void )
{
	_string_type d33  = "<?xml version='1.0' encoding='UTF-8'?><Protocol Version='V1.1'><CommandInfo><CommandUID>1A00000Dolqf201609200929124090301C00000I</CommandUID><ServerUID>1B000001510020160530000000000ZZZ00000000</ServerUID><ClientUID>1B000001510020160918000000000ZZZ00000001</ClientUID><ModuleUID></ModuleUID><CommandName>GetDevStatus</CommandName><SendTime>20160920092912409</SendTime></CommandInfo><Content><PLC_UID>1000000E510020151201401122001Y016zG4NlBs</PLC_UID><PLC_Index>0</PLC_Index><PLC_MemType>1</PLC_MemType><PLC_MemAdd>100</PLC_MemAdd><MemAddModule></MemAddModule><MemAddBitNum></MemAddBitNum><PLC_MemValueType>1</PLC_MemValueType><PLC_MemValue>0</PLC_MemValue><PLC_MemName>����ָʾ��</PLC_MemName><PLC_MemUID>10000000000020160920401122001ZZZiFyYDlsJ</PLC_MemUID><PLC_Number>0</PLC_Number></Content></Protocol>";
	_string_type d22 = d33.gbk_utf8();

	//����ռ�
	char* pBuf = (char*)malloc(d22.size() + 1);
	memcpy( pBuf, d22.c_str(), d22.size() );
	pBuf[d22.size()] = 0;
	return pBuf;
}


EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE ParseMemory( const char* Raw, const char* Delphi )
{
	int y = 0;
	y ++;
}

/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_1( unsigned char* sz, int uSize )
{
	unsigned char pData[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
	memcpy(sz, pData, sizeof(pData));
	return 1;
}


/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
/*				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_2( char* sz, int uSize )
{
	strcpy(sz, "zhongguo�й�");
	return 1;
}

/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_3( int* uSize )
{
	*uSize = 100;
	return 1;
}

/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_4( double* uSize )
{
	*uSize = 10.123;
	return 1;
}

/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_5( Test_stu* stu )
{
	strcpy(stu->buffer,"012struct���Խṹ��");
	stu->dl = 88.12;
	stu->val = 666;
	return 1;
}

/*****************************************************************
/*����˵����	�����־��Ϣ����־����4��
/*����˵����	hLog : ��־���
				c_szMessage�� ��־��Ϣ
/*				
/*����ֵ��		1:�ɹ�  <0: �������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MCSHARP_Test_6( Test_stu* stu, TDataEvent Event )
{
	Test_stu Tmp;
	Tmp.buffer = new char[100];
	memset(Tmp.buffer, 0, 100);
	strcpy(Tmp.buffer, "01234�ص�����");
	Tmp.dl = 99.88;
	Tmp.val = 500;

	char sz[] = {0x11,0x12,0x13,0x14,0x15,0x16,0x17};
	Event(sz, sizeof(sz), &Tmp);
	return 1;
}