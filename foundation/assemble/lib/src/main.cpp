
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllCommon/DllCommon.h>
#include <libFoundation/modafx.h>

struct Init20160714
{
	Init20160714( void )
	{
		DllCommonInit();
	}
};

static Init20160714 g_Init20160714;

int main(int argc, char* argv[])
{
	return 0;
}
