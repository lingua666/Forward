
#include <dllString/Exports.h>
#include <libFoundation/modafx.h>

char g_IOTPen_Proto_Test_FilePath[500] = {0};
bool g_IOTPen_Proto_Test_isWork = false;
fpnMTPen_Proto_Test_RecData g_g_IOTPen_Proto_Test_fnRec = NULL;
fpnMTPen_Proto_Test_ParseData g_g_IOTPen_Proto_Test_fnParse = NULL;

void MIOTPen_Proto_Test_WorkThread( void )
{
	_file_::CMyFile File;
	_string_type sData;
	int iStart = 0, iEnd = 0;
	while( true )
	{
		if( g_IOTPen_Proto_Test_isWork )
		{
			if( !File.is_open() )
			{
				if( File.Open(g_IOTPen_Proto_Test_FilePath, "rb") == -1 )
				{
					g_IOTPen_Proto_Test_isWork = false;
				}
				else
				{
					while( !File.isEof() )
					{
						sData = File.GetLineData();

						iStart = sData.find("iValid|") + strlen("iValid|");
						iEnd = sData.find("|", iStart);
						_string_type siValid = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("iType|") + strlen("iType|");
						iEnd = sData.find("|", iStart);
						_string_type siType = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("iSAR|") + strlen("iSAR|");
						iEnd = sData.find("|", iStart);
						_string_type siSAR = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("rX|") + 3;
						iEnd = sData.find("|", iStart);
						_string_type srX = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("rY|") + 3;
						iEnd = sData.find("|", iStart);
						_string_type srY = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("iIndex|") + strlen("iIndex|");
						iEnd = sData.find("|", iStart);
						_string_type siIndex = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("pointZ|") + strlen("pointZ|");
						iEnd = sData.find("|", iStart);
						_string_type spointZ = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("fX|") + 3;
						iEnd = sData.find("|", iStart);
						_string_type sfX = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("fY|") + 3;
						iEnd = sData.find("|", iStart);
						_string_type sfY = sData.substr( iStart, iEnd - iStart);

						iStart = sData.find("Page|") + 5;
						iEnd = sData.find("|", iStart);
						if( iEnd == -1 )
							iEnd = sData.size();

						_string_type sPage = sData.substr( iStart, iEnd - iStart);

						IOTPenParse_INFO RetData = {0};

						RetData.iValid = _string_type::StringToInt(siValid);
						RetData.iType = _string_type::StringToInt(siType);
						RetData.iSAR = _string_type::StringToInt(siSAR);
						RetData.rX = _string_type::StringToInt(srX);
						RetData.rY = _string_type::StringToInt(srY);
						RetData.iIndex = _string_type::StringToInt(siIndex);
						RetData.pointZ = _string_type::StringToInt(spointZ);
						RetData.fX = _string_type::StringToFloat(sfX);
						RetData.fY = _string_type::StringToFloat(sfY);
						RetData.iPage = _string_type::StringToInt(sPage);

						/*int iValid = _string_type::StringToInt(siValid);
						int iType = _string_type::StringToInt(siType);
						int iSAR = _string_type::StringToInt(siSAR);
						int rX = _string_type::StringToInt(srX);
						int rY = _string_type::StringToInt(srY);
						int iIndex = _string_type::StringToInt(siIndex);
						int pointZ = _string_type::StringToInt(spointZ);

						UInt8 uData[11] = {0};
						uData[0] = 0x12;
						if( iValid )
						{
							uData[1] = 0x80;
						}
						else
						{
							uData[1] = 0x00;
						}

						uData[1] |= ((UInt8)iType << 5) & 0x60;
						uData[1] |= ((UInt8)iSAR << 2) & 0x1C;
						uData[7] = (UInt8)iIndex;
						uData[8] = (UInt8)(pointZ >> 8 & 0x03);
						uData[9] = (UInt8)(pointZ & 0xFF);

						//
						uData[1] |= (UInt8(rY >> 19)) & 0x03;
						uData[2] = (UInt8(rY >> 11)) & 0xFF;
						uData[3] = (UInt8(rY >> 3)) & 0xFF;
						uData[4] = (UInt8(rY << 5)) & 0xE0;
						uData[4] |= (UInt8(rX >> 16)) & 0x1F;
						uData[5] = (UInt8(rX >> 8)) & 0xFF;
						uData[6] = (UInt8(rX)) & 0xFF;

						if( g_g_IOTPen_Proto_Test_fnRec )
						{
							g_g_IOTPen_Proto_Test_fnRec((const char*)uData, 10);
						}*/
					};

					File.Close();
					g_IOTPen_Proto_Test_isWork = false;
				}
			}
		}
		else
		{
			Sleep(100);
		}
	}
}

static _thread_::ThreadWrap* MIOTPen_Proto_Test_GetThread( void )
{
	static _thread_::ThreadWrap s_Thread(function20_bind(MIOTPen_Proto_Test_WorkThread));
	return &s_Thread;
}

/*****************************************************************
/*函数说明：	解析数据
/*参数说明：	fnRec: 数据接收回调函数
/*返回值：		1:成功(解析出有效数据)  <0: 错误码			
*****************************************************************/
/*EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MIOTPen_Proto_Test_SetRecv( fpnMTPen_Proto_Test_RecData fnRec )
{
	g_g_IOTPen_Proto_Test_fnRec = fnRec;
	return 1;
}*/


/*****************************************************************
/*函数说明：	解析数据
/*参数说明：	fnRec: 数据接收回调函数
/*返回值：		1:成功(解析出有效数据)  <0: 错误码			
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MIOTPen_Proto_Test_SetParse( fpnMTPen_Proto_Test_ParseData fnParse )
{
	g_g_IOTPen_Proto_Test_fnParse = fnParse;
	return 1;
}


/*****************************************************************
/*函数说明：	解析数据
/*参数说明：	c_szFilePath: 测试文件路径
/*返回值：		1:成功(解析出有效数据)  <0: 错误码			
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MIOTPen_Proto_Test_Parse( const char* c_szFilePath )
{
	if( !g_IOTPen_Proto_Test_isWork ) 
	{
		g_IOTPen_Proto_Test_isWork = true;
		memset(g_IOTPen_Proto_Test_FilePath, 0, sizeof(g_IOTPen_Proto_Test_FilePath));
		strcpy(g_IOTPen_Proto_Test_FilePath, c_szFilePath);
		MIOTPen_Proto_Test_GetThread();
		return 1;
	}

	return -1;
}