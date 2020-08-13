
#ifndef __EXPORTS_20180119161941_1451301281_H__
#define __EXPORTS_20180119161941_1451301281_H__

#include "External.h"

#ifndef DLL_INIT_FLAG_DEF
	#define DLL_INIT_FLAG_DEF
	DLL_INIT_DECL_DEF()
#endif

DLL_LOG_DECL_DEF(NetFrame_UDP_Stream_Module)

/*****************************************************************
/*函数说明：	初始化
/*参数说明：	LocalPort: 本地端口
/*				c_szDestIP: 广播系统软件ip地址
/*				DestPort: 广播系统软件端口号
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Init( int LocalPort,
											const char* c_szDestIP, int iDestPort );


/*****************************************************************
/*函数说明：	释放资源
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Free( void );


/*****************************************************************
/*函数说明：	呼叫所有喇叭
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Call( void );


/*****************************************************************
/*函数说明：	挂断所有喇叭
/*参数说明：	
/*				
/*返回值：		1:成功  <0:错误码
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MEmergency_Telephone_Hangup( void );

#endif//__PLC_OMRON_20151228191441_1451301281_H__
