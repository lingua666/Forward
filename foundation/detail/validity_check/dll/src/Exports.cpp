
#include <dllValidityCheck/Exports.h>
#include <libValidityCheck/FileCheck.h>


bool g_MValidity_Failed201611101928( fpnMValidity_CheckFaild fpnFaild,
									const _string_type& sPath,
									int iValue )
{
	if( fpnFaild )
	{
		return fpnFaild(sPath.c_str(), iValue);
	}

	return true;
}

/*****************************************************************
/*函数说明：	文件合法性验证
/*参数说明：	fpnFaild: 非法文件回调函数
/*				
/*返回值：		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS void _CALLTYPE MValidity_Check( fpnMValidity_CheckFaild fpnFaild )
{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	char szPath[1024] = {0};
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	::VirtualQuery( fpnFaild, &mbi, sizeof(mbi) );
	::GetModuleFileNameA(reinterpret_cast<HMODULE>(mbi.AllocationBase),
	szPath, sizeof(szPath));
	_validity_::CheckValidity Check(szPath,
						function20_bind(g_MValidity_Failed201611101928, 
						fpnFaild, _string_type(szPath), _function_::_1));
#endif
}