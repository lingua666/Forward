
#include <dllFrameWork_Uart/Exports.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

void _CALLTYPE MUart_RecData( const char* c_szData,
								UInt32 u32Size )
{

}

int main(int argc, char* argv[])
{
	/*UInt32 u1 = 0x436F0C00;
	UInt32 u2 = 0x436F5021;
	UInt32 u3 = 0x436F71EC;
	UInt32 u4 = 0x42A50000;
	UInt32 u5 = 0x42924CCD;
	UInt32 u6 = 0x428A0A3D;

	float f1 = *((float*)&u1);
	float f2 = *((float*)&u2);
	float f3 = *((float*)&u3);
	float f4 = *((float*)&u4);
	float f5 = *((float*)&u5);
	float f6 = *((float*)&u6);*/

	/*float f3 = 239.0469;
	UInt32 u3 = *((UInt32*)&f3);*/

	char s32[] = "0C00436F";
	double d1 = MUart_String2Double(s32);

	char suy[10] = {0};
	MUart_Double2String(d1, suy, sizeof(suy));

	MUart_Init(MUart_RecData);

	MUart_Set(1, false, true);

	if( MUart_Open(3, 9600, 8) )
	{
		MUart_SentData("1234567890", 10);
	}

	std::cin.get();
	return 0;
}