//该源文件是对onvif接口函数的的封装，因为在main文件为cpp，要想在cpp中使用c的代码，就要使用extern C 进行声明封装c的函数接口
//并且在该文件内不能包含有关cpp的代码或者头文件，在cpp的源文件中我也不能包含有关c的代码或者头文件，该封装c接口的头文件除外（ONVIF_Interface.h）
#include <string.h>
#include <stdio.h>

#include <libThirdWrap_Onvif/onvif/soapH.h>
#include <libThirdWrap_Onvif/onvif/soapStub.h>
#include <libThirdWrap_Onvif/onvif/httpda.h>
#include <libThirdWrap_Onvif/onvif/wsdd.h>
#include <libThirdWrap_Onvif/onvif/wsseapi.h>
#include <libThirdWrap_Onvif/ONVIF_Interface_C.h>

#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */

#define	MULTICAST_ADDRESS	"soap.udp://239.255.255.250:3702"		//用宏定义多播地址
#define MAX_64_LEN			20                             
#define INFO_LENGTH			40
#define MAX_PROF_TOKEN		20


//****************************************************************onvif接口：begin*****************************

#ifdef __cplusplus
extern "C" {
#endif

HSOAP	ONVIF_malloc( const char* c_pUserName, const char* c_pPassword )
{
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return 0;

	soap_set_namespaces(soap, namespaces);   //设置命名空间

	if( c_pUserName != NULL && c_pPassword != NULL )
		soap_wsse_add_UsernameTokenDigest(soap, NULL, c_pUserName, c_pPassword);         //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用

	return (HSOAP)soap;
}

void	ONVIF_free( HSOAP hSoap )
{
	struct soap *soap = (struct soap*)hSoap;
	soap_end(soap);
	free(soap);
}

int	ONVIF_SetUserPwd( HSOAP hSoap, const char* c_pUserName, const char* c_pPassword )
{
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	if( c_pUserName == NULL || c_pPassword == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, c_pUserName, c_pPassword);         //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	return 1;
}

//获取设备信息，这里的解析数据只适用于海康的设备。
int ONVIF_GetDevInformation_C( const char* pDevice_t, const char* pService_Addr,
							const char* pUserName, const char* pPassword,
							tagOnvif_DevInfo* _OUT pDevInfo )
{
	struct _tds__GetDeviceInformationResponse tds__GetDeviceInformationResponse;  
	struct _tds__GetDeviceInformation tds__GetDeviceInformation;  
	const char* soap_endpoint = pService_Addr;  //服务器的地址
	
	int iResult = -1;
	int ret = 0;

	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap, namespaces);   //设置命名空间

	if(soap_register_plugin(soap, http_da))
		soap_print_fault(soap, stderr); // failed to register  登记失败
	
	ret = soap_call___tds__GetDeviceInformation(soap,soap_endpoint,NULL,&tds__GetDeviceInformation,&tds__GetDeviceInformationResponse);  //获取设备信息

	memset(pDevInfo, 0, sizeof(tagOnvif_DevInfo));

	if (ret != SOAP_OK)  
	{  
		if (ret == 401) // challenge: HTTP authentication required  验证失败
		{  
			if (!strcmp(soap->authrealm, pDevice_t)) //对比设备型号
			{  
				struct http_da_info info; //用来存储用户名和密码
				http_da_save(soap, &info, pDevice_t, pUserName, pPassword); // 加上设备用户名和密码
				
				//两种鉴权方式都可以
				//soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);         //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
				if (soap_call___tds__GetDeviceInformation_(soap, soap_endpoint, NULL, &tds__GetDeviceInformation,&tds__GetDeviceInformationResponse)== SOAP_OK) // 重新获取设备信息
				{  
					strcpy(pDevInfo->Manufacturer, tds__GetDeviceInformationResponse.Manufacturer);
					strcpy(pDevInfo->HardwareId, tds__GetDeviceInformationResponse.HardwareId);
					strcpy(pDevInfo->FirmwareVersion, tds__GetDeviceInformationResponse.FirmwareVersion);
					strcpy(pDevInfo->SerialNumber, tds__GetDeviceInformationResponse.SerialNumber);
			
					http_da_restore(soap, &info); // 清空存储的用户名和密码 
					http_da_release(soap, &info); // 删除用户名和密码

					iResult = 1;
				}  
				else  
					soap_print_fault(soap, stderr); // failed toregister  
			}   
		}  
		else  
		{  
			soap_print_fault(soap,stderr); // failed to register  
		}  
	}

	soap_end(soap);
	free(soap); 
	return iResult;
}

//RecvTimeout： 超时时间， 单位秒
//fpnOnvif_DiscoveryCallBack 通知回调函数
int ONVIF_ClientDiscovery_C( int RecvTimeout,
							fpnOnvif_DiscoveryCallBack fpnDiscovery,
							void* pUser,
							const char* szIP )
{  
	unsigned char macaddr[6];     //物理地址    
	char _HwId[1024];             //用来保存guid号  
	unsigned int Flagrand;        //随机的标志  

	int iRet = -1, result = 0, i = 0;                                                //用来判断是否执行成功的
	wsdd__ProbeType req;                                                             //客户端发送的Probe
	struct __wsdd__ProbeMatches resp;                                                //服务端回的Probematchs  
	wsdd__ScopesType sScope;                                                         //Probe里面的属性范围
	struct SOAP_ENV__Header header;                                                  //SOAP的头消息 
	struct in_addr if_req;
	struct soap soap;																 //soap环境变量
	soap_init(&soap);

	if( szIP != NULL )
	{
		if_req.s_addr = inet_addr(szIP);  // 想绑定的IP地址
		soap.ipv4_multicast_if = (char*)soap_malloc(&soap, sizeof(struct in_addr));
		memset(soap.ipv4_multicast_if, 0, sizeof(struct in_addr));
		memcpy(soap.ipv4_multicast_if, (char*)&if_req, sizeof(if_req));
	}

	soap.recv_timeout = RecvTimeout;                        //设置超时时间
	soap_set_namespaces(&soap, namespaces);                   //设置命名时间
	soap_default_SOAP_ENV__Header(&soap, &header);            //将header设置为soap的消息头

	//获取类guid的唯一标识符(windows下叫guid,Linux下叫uuid),格式为urn:uuid:8-4-4-4-12,由系统随机产生
	// 获取一个随机的数字作为标识符
	srand((int)time(0));   //系统提供的生成伪随机数序列的种子函数（初始化函数）
	Flagrand = rand()%9000 + 8888;   //生成随机数字
	macaddr[0] = 0x1;                //物理地址
	macaddr[1] = 0x2; 
	macaddr[2] = 0x3; 
	macaddr[3] = 0x4; 
	macaddr[4] = 0x5; 
	macaddr[5] = 0x6;
	//_HwId用来存储uuid的，用到了随机码，本机的物理地址，
	sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X", 
			Flagrand, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

    ///////////给消息头赋值//////////
	header.wsa__MessageID = _HwId;                     //消息头的id由随机标识符来填充
	header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";               //消息要发往的 URI 
	header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";  //定义了服务中处理消息方法，即WSDL中定义的操作的 Action
	soap.header = &header;                
	
	//设置所需寻找设备的类型和范围,二者至少设定一个,否则可能收到非ONVIF设备,出现异常
	soap_default_wsdd__ScopesType(&soap, &sScope);  
	sScope.__item = "";  
	soap_default_wsdd__ProbeType(&soap, &req);  
	req.Scopes = &sScope;  
	req.Types = ""; //"dn:NetworkVideoTransmitter"  
	
	//通过组播发送Probe探针,发送成功返回0,否则-1   soap_call___tds__GetSystemSupportInformation
    result = soap_send___wsdd__Probe(&soap, MULTICAST_ADDRESS, NULL, &req);  
	
	//循环监听服务端的回应
    while(result == SOAP_OK)
    {  
		int i = 0; 
		result = soap_recv___wsdd__ProbeMatches(&soap, &resp);  

		if(result == SOAP_OK)  
		{  
			if(soap.error)  
			{  
				printf("soap error 1: %d, %s, %s\n", soap.error, *soap_faultcode(&soap), *soap_faultstring(&soap));  
				result = soap.error;
			}  
			else  
			{  
				iRet = 0;
				if( fpnDiscovery != NULL )
				{
					tagOnvif_DiscoveryInfo	DiscoveryInfo = {0};

					if( soap.header != NULL && soap.header->wsa__MessageID != NULL)  
					{
						strcpy(DiscoveryInfo.UUID, soap.header->wsa__MessageID);
					}  

					if( resp.wsdd__ProbeMatches != NULL )
					{
						for(i = 0; i < resp.wsdd__ProbeMatches->__sizeProbeMatch; i++)  
						{  
							strcpy(DiscoveryInfo.EP_Address, resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);
							strcpy(DiscoveryInfo.Service_Addr, resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);      //全局存放服务器地址

							if( strlen( DiscoveryInfo.Service_Addr ) > strlen("http://") )
							{
								char* p = strchr(&DiscoveryInfo.Service_Addr[strlen("http://")], '/');
								strncpy(DiscoveryInfo.IP, &DiscoveryInfo.Service_Addr[strlen("http://")], p - (char*)&DiscoveryInfo.Service_Addr[strlen("http://")]);
							}

							//解析返回的设备信息
							if(resp.wsdd__ProbeMatches->ProbeMatch->Scopes != NULL)  
							{  
								char *temp = strstr(resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item, "name") + 5;
								strncpy(DiscoveryInfo.Manufacturer, temp, 9);

								temp = strstr(resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item, "hardware") + 9;
								for(i = 0; i < 20; i++)
								{
									if(isspace(temp[i]))            //判断该字符是否是空格，如果是空格就停止遍历
										strncpy(DiscoveryInfo.Device_t, temp, i);      //将空格前的字符串复制给str1
								}

							}  
						}
					}

					if( strcmp(DiscoveryInfo.Device_t, "") )//设备型号为空则过滤
					{
						iRet = fpnDiscovery(&DiscoveryInfo, pUser);
						if( iRet == -1 )
						{
							break;
						}
					}
				}
			}
		}  
		else if (soap.error)  
		{  
			printf("[%d] soap error 2: %d, %s, %s\n", __LINE__, soap.error, *soap_faultcode(&soap), *soap_faultstring(&soap));
			result = soap.error;
		}
    }

	soap_end(&soap);
	return iRet;
}  


//获取设备的能力值，使用的是设备地址和云台地址，也就是服务器，获取媒体信息地址提供给GetProfiles接口使用,ret_data存放返回数据的结构体
int ONVIF_GetCapabilities_C( HSOAP hSoap, const char *pService_Addr,
						const char* pUserName,
						const char* pPassword,
						ONVIF_GetCapabilities_C_DATE* _OUT Ret_Data )
{                                          
	struct _tds__GetCapabilities tds__GetCapabilities;                         //GetCapabilities
	struct _tds__GetCapabilitiesResponse tds__GetCapabilitiesResponse;         //GetCapabilities
	enum tt__CapabilityCategory	tagCategory = {0};	
	int iRet = -1;

	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	tds__GetCapabilities.Category = &tagCategory;
	tds__GetCapabilities.__sizeCategory = 1; 
	*(tds__GetCapabilities.Category) = (enum tt__CapabilityCategory)(tt__CapabilityCategory__All);  //这里可以设置获取的单个内容或者是（All）为全部获取

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tds__GetCapabilities(soap, pService_Addr, NULL, &tds__GetCapabilities, &tds__GetCapabilitiesResponse);
	if (soap->error) 
	{ 
		printf("[%d]--->>> soap error: %d, %s, %s\n",__LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
		return iRet;
	} 
	else 
	{
		if(tds__GetCapabilitiesResponse.Capabilities==NULL) 
		{ 
			printf(" GetCapabilities failed!\n");
			return iRet;
		} 
		else 
		{ 
			if( Ret_Data != NULL )
			{
				strcpy(Ret_Data->Media_addr, tds__GetCapabilitiesResponse.Capabilities->Media->XAddr);

				//这里就可以判断他是不是球机了，如果是球机就执行下面的代码获取设备额云台地址
				if(tds__GetCapabilitiesResponse.Capabilities->PTZ != NULL)
				{
					strcpy(Ret_Data->PTZ_addr, tds__GetCapabilitiesResponse.Capabilities->PTZ->XAddr);     
					Ret_Data->isPTZ = 1;                            //判断是不是球机的标志位，0为不是，1就是
				}
				else
					Ret_Data->isPTZ = 0;                            //判断是不是球机的标志位，0为不是，1就是
			}

			iRet = 1;
		} 
	}

	return iRet;
}

//获取设备服务，使用的是设备服务地址
int ONVIF_GetService_C( HSOAP hSoap, 
					const char *pService_Addr,
					const char* pUserName,
					const char* pPassword )
{
	struct _tds__GetServices tds__GetServices;
	struct _tds__GetServicesResponse tds__GetServicesResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tds__GetServices(soap, pService_Addr, NULL, &tds__GetServices, &tds__GetServicesResponse); 

	if (soap->error)  
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		iRet = 1;	 
	} 
	
	return iRet;
}

//获取媒体的信息文件，使用的是媒体信息地址，识别主，子通道的编码分辨率，提供给GetStreamuri接口使用, ret_data返回GetProfiles的数据结构体
int ONVIF_GetProfiles_C( HSOAP hSoap, const char* Media_Addr,
						const char* pUserName,
						const char* pPassword,
						unsigned char isMain,
						ONVIF_GetProfiles_C_DATE* _OUT Ret_Data)
{ 
	struct _trt__GetProfiles trt__GetProfiles;
	struct _trt__GetProfilesResponse trt__GetProfilesResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;			//初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___trt__GetProfiles(soap, Media_Addr, NULL, &trt__GetProfiles, &trt__GetProfilesResponse); 

	if (soap->error)  
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		int iType = 0;
		iRet = 1;
		if( isMain == 0 )
		{
			iType = 1;
		}

		strcpy(Ret_Data->VideoSourcesToken, trt__GetProfilesResponse.Profiles[iType].VideoSourceConfiguration->token);   //这个token是Configuration的token，用于ONVIF_GetVideoSourceConfiguration函数的参数，sourcetoken和Configuration token不一样
		strcpy(Ret_Data->ProfilesToken, trt__GetProfilesResponse.Profiles[iType].token);
	} 

	return iRet;
 }

//获取流媒体的地址，使用的是媒体信息地址，提供给RTSP获取视频流，但是要在uri地址上加上用户名和密码，ret_MediaUri存放返回的流媒体的地址
int ONVIF_GetUri_C( HSOAP hSoap, const char *Media_Addr,
				const char* Profiles_token,
				const char* pUserName,
				const char* pPassword,
				char* _OUT Ret_MediaUri ) 
{ 
	char UserAndPassword[1024] = {0};
	char temp[1024] = {0};
	 
	struct _trt__GetStreamUri trt__GetStreamUri;
	struct _trt__GetStreamUriResponse trt__GetStreamUriResponse; 
	struct tt__StreamSetup	tagSetup = {0};
	struct tt__Transport	tagTransport = {0};

	int iRet = -1;

	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL || Media_Addr == NULL || Profiles_token == NULL )
		return -1;

	trt__GetStreamUri.StreamSetup = &tagSetup;            //初始化，分配空间 
	trt__GetStreamUri.StreamSetup->Stream = tt__StreamType__RTP_Unicast;                                                  //流的类型 
	trt__GetStreamUri.StreamSetup->Transport = &tagTransport;   //初始化，分配空间 
	trt__GetStreamUri.StreamSetup->Transport->Protocol = tt__TransportProtocol__UDP;                                      //协议类型
	trt__GetStreamUri.StreamSetup->Transport->Tunnel = 0; 
	trt__GetStreamUri.ProfileToken = Profiles_token ; 

	sprintf(UserAndPassword,"rtsp://%s:%s@", pUserName, pPassword); //设备用户名和密码

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___trt__GetStreamUri(soap, Media_Addr, NULL, &trt__GetStreamUri, &trt__GetStreamUriResponse); 
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 		
		//加上用户名和密码: "rtsp://admin:abcd-1234@192.168.8.23:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";
		strncpy(temp, trt__GetStreamUriResponse.MediaUri->Uri+7, 80);  
		strcat(UserAndPassword, temp);
		strcpy(Ret_MediaUri, UserAndPassword);
		iRet = 1;
	} 

	return iRet;
}

int ONVIF_GetRecordingSummary_C( const char *MediaUri, 
								const char* pUserName,
								const char* pPassword )
{
	struct _tse__GetRecordingSummary tse__GetRecordingSummary;
	struct _tse__GetRecordingSummaryResponse tse__GetRecordingSummaryResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tse__GetRecordingSummary(soap, MediaUri, NULL, &tse__GetRecordingSummary, &tse__GetRecordingSummaryResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------GetRecordingSummary OK--------------\n");
		printf("DataFrom is :%s \n",tse__GetRecordingSummaryResponse.Summary->DataFrom); 
		printf("DataUntil is :%s \n",tse__GetRecordingSummaryResponse.Summary->DataUntil);
		printf("NumberRecordings is :%d \n",tse__GetRecordingSummaryResponse.Summary->NumberRecordings);
		
		printf("--------------------GetRecordingSummary Done----------------\n\n");

		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return iRet;
}

int ONVIF_GetRecordingInformation_C( const char *MediaUri,
									const char* pUserName,
									const char* pPassword )
{
	struct _tse__GetRecordingInformation tse__GetRecordingInformation;
	struct _tse__GetRecordingInformationResponse tse__GetRecordingInformationResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	tse__GetRecordingInformation.RecordingToken = NULL; //要RecordingToken才能正常调用
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tse__GetRecordingInformation(soap, MediaUri, NULL, &tse__GetRecordingInformation, &tse__GetRecordingInformationResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------GetRecordingInformation OK--------------\n");
		printf("sizeVideoSources is :%s \n",tse__GetRecordingInformationResponse.RecordingInformation->RecordingToken); 
		printf("token is :%s \n",tse__GetRecordingInformationResponse.RecordingInformation->Source->SourceId);	
		printf("token is :%s \n",tse__GetRecordingInformationResponse.RecordingInformation->Source->Name);	
		printf("token is :%s \n",tse__GetRecordingInformationResponse.RecordingInformation->Source->Location);	
		printf("token is :%s \n",tse__GetRecordingInformationResponse.RecordingInformation->Source->Description);	
		printf("token is :%s \n",tse__GetRecordingInformationResponse.RecordingInformation->Source->Address);	
		printf("--------------------GetRecordingInformation Done----------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return iRet;
}

//获取视频源，使用的是媒体信息地址Media_Addr
int ONVIF_GetVideoSources_C( HSOAP hSoap, const char *Media_Addr )
{
	struct _trt__GetVideoSources trt__GetVideoSources = {0};
	struct _trt__GetVideoSourcesResponse trt__GetVideoSourcesResponse = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_call___trt__GetVideoSources(soap, Media_Addr, NULL, &trt__GetVideoSources, &trt__GetVideoSourcesResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------GetVideoSources OK--------------\n");
		printf("sizeVideoSources is :%d \n",trt__GetVideoSourcesResponse.__sizeVideoSources); 
		printf("token is :%s \n",trt__GetVideoSourcesResponse.VideoSources->token);	
		printf("--------------------GetVideoSources Done----------------\n\n");
		iRet = 1;
	} 

	return iRet;
}

int ONVIF_SetSystemTimeAndDate_C(HSOAP hSoap, const char *pService_Addr,
	const char* pUserName, const char* pPassword,
	const ONVIF_C_TimeAndDate* pTimeAndDate )
{
	struct _tds__SetSystemDateAndTime SetSystemDateAndTime = {0} ;
	struct _tds__SetSystemDateAndTimeResponse SetSystemDateAndTimeResponse = {0};
	int iRet = -1;

	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	SetSystemDateAndTime.TimeZone = NULL;
	SetSystemDateAndTime.UTCDateTime = (struct tt__DateTime *)soap_malloc(soap,sizeof(struct tt__DateTime));
	SetSystemDateAndTime.UTCDateTime->Date = (struct tt__Date *)soap_malloc(soap,sizeof(struct tt__Date));
	SetSystemDateAndTime.UTCDateTime->Time = (struct tt__Time *)soap_malloc(soap,sizeof(struct tt__Time));

	SetSystemDateAndTime.DaylightSavings = xsd__boolean__false_;
	SetSystemDateAndTime.DateTimeType = tt__SetDateTimeType__Manual;
	SetSystemDateAndTime.UTCDateTime->Date->Year = pTimeAndDate->Year;
	SetSystemDateAndTime.UTCDateTime->Date->Month = pTimeAndDate->Month;
	SetSystemDateAndTime.UTCDateTime->Date->Day = pTimeAndDate->Day;
	SetSystemDateAndTime.UTCDateTime->Time->Hour = pTimeAndDate->Hour;
	SetSystemDateAndTime.UTCDateTime->Time->Minute = pTimeAndDate->Minute;
	SetSystemDateAndTime.UTCDateTime->Time->Second = pTimeAndDate->Second;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	iRet=  soap_call___tds__SetSystemDateAndTime(soap, pService_Addr, NULL, &SetSystemDateAndTime, &SetSystemDateAndTimeResponse) ;
	//成功获取设备的日期和时间信息
	if(iRet != SOAP_OK)
	{
		fprintf(stderr,"soap error: %s, %s\n", *soap_faultcode(soap), *soap_faultstring(soap));
	}
	return iRet;
}

int  ONVIF_GetSystemTimeAndDate_C( HSOAP hSoap, const char *pService_Addr,
	ONVIF_C_TimeAndDate* pTimeAndDate )
{
	struct _tds__GetSystemDateAndTime  GetSystemDateAndTime ;
	struct _tds__GetSystemDateAndTimeResponse GetSystemDateAndTimeResponse = {0};
	int iRet = -1;

	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	iRet = soap_call___tds__GetSystemDateAndTime(soap, pService_Addr, NULL, &GetSystemDateAndTime, &GetSystemDateAndTimeResponse) ;
	if(iRet == SOAP_OK)
	{
		pTimeAndDate->Year = GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Date->Year;
		pTimeAndDate->Month = GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Date->Month;
		pTimeAndDate->Day = GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Date->Day;

		pTimeAndDate->Hour = GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Time->Hour;
		pTimeAndDate->Minute = GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Time->Minute;
		pTimeAndDate->Second = GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Time->Second;
	}

	return iRet;
}

//获取视频源的信息， 使用的是媒体地址和视频源的token
int ONVIF_GetVideoSourceConfiguration_C(const char *Media_Addr,
									const char *VideoSourcesToken,
									const char* pUserName,
									const char* pPassword)
{
	struct _trt__GetVideoSourceConfiguration trt__GetVideoSourceConfiguration = {0};
	struct _trt__GetVideoSourceConfigurationResponse trt__GetVideoSourceConfigurationResponse = {0};

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if(soap != NULL)
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	trt__GetVideoSourceConfiguration.ConfigurationToken = VideoSourcesToken;
	//trt__GetVideoSourceConfigurationResponse.Configuration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
	//memset(trt__GetVideoSourceConfigurationResponse.Configuration, 0, sizeof(struct tt__VideoSourceConfiguration));

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___trt__GetVideoSourceConfiguration(soap, Media_Addr, NULL, &trt__GetVideoSourceConfiguration, &trt__GetVideoSourceConfigurationResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n------------------- GetVideoSourceConfiguration OK--------------\n");
		printf("UseCount is :%d \n",trt__GetVideoSourceConfigurationResponse.Configuration->UseCount); 
		printf("token is :%s \n",trt__GetVideoSourceConfigurationResponse.Configuration->token);	
		printf("SourceToken is :%s \n",trt__GetVideoSourceConfigurationResponse.Configuration->SourceToken);
		printf("-------------------- GetVideoSourceConfiguration Done----------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return iRet;
}

//获取视频的输出集，使用的是流媒体的地址
int ONVIF_GetVideoOutputs_C(const char *Media_Addr,
							const char* pUserName,
							const char* pPassword )
{
	struct _tmd__GetVideoOutputs tmd__GetVideoOutputs = {0};
	struct _tmd__GetVideoOutputsResponse tmd__GetVideoOutputsResponse = {0};

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tmd__GetVideoOutputs(soap, Media_Addr, NULL, &tmd__GetVideoOutputs, &tmd__GetVideoOutputsResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------GetVideoOutputs OK--------------\n");
		printf("sizeVideoSources is :%d \n",tmd__GetVideoOutputsResponse.__sizeVideoOutputs); 
		printf("token is :%s \n",tmd__GetVideoOutputsResponse.VideoOutputs->token);		
		printf("--------------------GetVideoOutputs Done----------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return iRet;
}

//获取云台的设备的状态，地址用的是具备云台功能的服务器地址 ，使用的是ProfilesToken 和设备的云台服务地址，ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_PTZ_GetStatus_C( const char *ProfilesToken,
						const char *pService_Addr,
						const char* pUserName,
						const char* pPassword )
{
	struct _tptz__GetStatus tptz__GetStatus;
	struct _tptz__GetStatusResponse tptz__GetStatusResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	tptz__GetStatus.ProfileToken = ProfilesToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GetStatus(soap, pService_Addr, NULL, &tptz__GetStatus, &tptz__GetStatusResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_GetStatus OK--------------\n");	
		printf("x = %f\n", tptz__GetStatusResponse.PTZStatus->Position->PanTilt->x);
		printf("y = %f\n", tptz__GetStatusResponse.PTZStatus->Position->PanTilt->y);
		printf("space = %s\n", tptz__GetStatusResponse.PTZStatus->Position->PanTilt->space);
		printf("--------------------PTZ_GetStatus Done--------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return iRet;
}

//获取云台设备的nodes值，地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ",ret_GetNodeToken存放返回的NodeToken
int ONVIF_PTZ_GetNodes_C( HSOAP hSoap, const char *pService_Addr,
						const char* pUserName,
						const char* pPassword,
						char* _OUT Ret_GetNodeToken )
{
	struct _tptz__GetNodes tptz__GetNodes;
	struct _tptz__GetNodesResponse tptz__GetNodesResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GetNodes(soap, pService_Addr, NULL, &tptz__GetNodes, &tptz__GetNodesResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		strcpy(Ret_GetNodeToken, tptz__GetNodesResponse.PTZNode->token);
		iRet = 1;
	} 

	return iRet;
}

//获取云台设备的node值, 地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_PTZ_GetNode_C( HSOAP hSoap, const char *Token,
						const char *pService_Addr,
						const char* pUserName,
						const char* pPassword )
{
	struct _tptz__GetNode tptz__GetNode;
	struct _tptz__GetNodeResponse tptz__GetNodeResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	tptz__GetNode.NodeToken = Token;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GetNode(soap, pService_Addr, NULL, &tptz__GetNode, &tptz__GetNodeResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_GetNode OK--------------\n");	
		printf("AuxiliaryCommands = %s\n", tptz__GetNodeResponse.PTZNode->AuxiliaryCommands);              //为NULL说明不支持辅助操作
		printf("name = %s\n", tptz__GetNodeResponse.PTZNode->Name);
		printf("token = %s\n", tptz__GetNodeResponse.PTZNode->token);
		printf("__sizeAuxiliaryCommands = %d\n", tptz__GetNodeResponse.PTZNode->__sizeAuxiliaryCommands);
		printf("MaximumNumberOfPresets = %d\n", tptz__GetNodeResponse.PTZNode->MaximumNumberOfPresets);
		printf("AbsolutePanTiltPositionSpace = %d\n", tptz__GetNodeResponse.PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace);
		printf("ContinuousPanTiltVelocitySpace = %d\n", tptz__GetNodeResponse.PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace);
		printf("--------------------PTZ_GetNode Done--------------\n\n");
		iRet = 1;
	} 

	return -1;
}


//获取设备云台的Presets值,地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ"  ret_data用于返回的数据的结构体
int ONVIF_PTZ_GetPresets_C( HSOAP hSoap, const char *ProfileToken,
							const char *pService_Addr,
							const char* pUserName,
							const char* pPassword,
							ONVIF_PTZ_GetPresets_C_DATE* _OUT Ret_Data)
{
	struct _tptz__GetPresets tptz__GetPresets;
	struct _tptz__GetPresetsResponse tptz__GetPresetsResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	tptz__GetPresets.ProfileToken = ProfileToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GetPresets(soap, pService_Addr, NULL, &tptz__GetPresets, &tptz__GetPresetsResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		/*printf("\n-------------------PTZ_GetPresets OK--------------\n");	
		printf("__sizePreset = %d\n", tptz__GetPresetsResponse.__sizePreset);
		printf("name = %s\n", tptz__GetPresetsResponse.Preset->Name);
		printf("token = %s\n", tptz__GetPresetsResponse.Preset->token);*/
		strcpy(Ret_Data->PresetToken, tptz__GetPresetsResponse.Preset->token);
		strcpy(Ret_Data->PresetName, tptz__GetPresetsResponse.Preset->Name);
		/*printf("space = %s\n", tptz__GetPresetsResponse.Preset->PTZPosition->PanTilt->space);
		printf("space = %s\n", tptz__GetPresetsResponse.Preset->PTZPosition->Zoom->space);
		printf("x = %d\n", tptz__GetPresetsResponse.Preset->PTZPosition->PanTilt->x);
		printf("y = %d\n", tptz__GetPresetsResponse.Preset->PTZPosition->PanTilt->y);
		printf("--------------------PTZ_GetPresets Done--------------\n\n");*/
		iRet = 1;
	} 

	return iRet;
}


//设置设备云台的Preset值，地址用的是具备云台功能的服务器地址  ps:"http://192.168.8.23/onvif/PTZ"  ret_PresetToken用于返回的数据
//这个接口用的是GetPresets返回的信息来填充结构体的，
int ONVIF_PTZ_SetPreset_C( const char *ProfileToken, 
						const char *PresetTokenm,
						const char *PresetName, 
						const char *pService_Addr,
						const char* pUserName,
						const char* pPassword,
						char* _OUT Ret_PresetToken )
{
	struct _tptz__SetPreset tptz__SetPreset;
	struct _tptz__SetPresetResponse tptz__SetPresetResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	tptz__SetPreset.ProfileToken = ProfileToken;
	tptz__SetPreset.PresetToken = PresetTokenm;
	tptz__SetPreset.PresetName = PresetName;

	printf("---------------Now PTZ_SetPreset----------------\n");
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__SetPreset(soap, pService_Addr, NULL, &tptz__SetPreset, &tptz__SetPresetResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_SetPreset OK--------------\n");	
		printf("PresetToken = %s\n", tptz__SetPresetResponse.PresetToken);
		strcpy(Ret_PresetToken, tptz__SetPresetResponse.PresetToken);
		printf("--------------------PTZ_SetPreset Done--------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return -1;
}


//回到预设点，这个接口用GetPresets 或者是SetPreset 返回的信息来填充结构体  ,移除预设（RemovePreset）也是和GotoPreset一样
//通过GetPresets 或者是SetPreset来获取和设置预设点之后再用GotoPreset回到预设点，前提是，在你设置好预设点之后，球机要变化当前位置，调用该接口才会生效，之后才会回到预设点
int ONVIF_PTZ_GotoPreset_C( HSOAP hSoap, const char *ProfileToken,
							const char *PresetToken, 
							const char *pService_Addr,
							const char* pUserName,
							const char* pPassword )
{
	float x = -0.5;
	float y =0;

	struct _tptz__GotoPreset tptz__GotoPreset;
	struct _tptz__GotoPresetResponse tptz__GotoPresetResponse;
	struct tt__Vector2D	tagVector2D = {0};
	struct tt__PTZSpeed tagPTZSpeed = {0};

	int iRet = -1;

	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	tptz__GotoPreset.ProfileToken = ProfileToken;
	tptz__GotoPreset.PresetToken = PresetToken;
	tptz__GotoPreset.Speed = &tagPTZSpeed;
	tptz__GotoPreset.Speed->PanTilt = &tagVector2D;
	tptz__GotoPreset.Speed->PanTilt->x = x;
	tptz__GotoPreset.Speed->PanTilt->y = y;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GotoPreset(soap, pService_Addr, NULL, &tptz__GotoPreset, &tptz__GotoPresetResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_GotoPreset OK--------------\n");	

		printf("--------------------PTZ_GotoPreset Done--------------\n\n");
		iRet = 1;
	} 

	return iRet;
}

//获取设备云台的全部信息，使用的是设备云台的服务地址,ret_data 用于返回数据的结构体
int ONVIF_PTZ_GetConfigurations_C( const char *pService_Addr, 
								const char* pUserName,
								const char* pPassword,
								ONVIF_PTZ_GetConfigurations_C_DATE *Ret_Data )
{
	struct _tptz__GetConfigurations tptz__GetConfigurations;
	struct _tptz__GetConfigurationsResponse tptz__GetConfigurationsResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GetConfigurations(soap, pService_Addr, NULL, &tptz__GetConfigurations, &tptz__GetConfigurationsResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		strcpy(Ret_Data->ConfigurationToken, tptz__GetConfigurationsResponse.PTZConfiguration->token);   //返回的数据
		strcpy(Ret_Data->TimeOut, (char *)tptz__GetConfigurationsResponse.PTZConfiguration->DefaultPTZTimeout);   //返回的数据
		strcpy(Ret_Data->spcae, tptz__GetConfigurationsResponse.PTZConfiguration->DefaultAbsolutePantTiltPositionSpace);     //返回的数据
		iRet = 1;
	}

	soap_end(soap);
	free(soap);
	return iRet;
}

//获取设备云台的的输出信息，使用的ConfigurationToken 和设备云台的服务地址
int ONVIF_PTZ_GetConfigurationOptions_C( const char *ConfigurationToken,
										const char *pService_Addr,
										const char* pUserName,
										const char* pPassword )
{
	struct _tptz__GetConfigurationOptions tptz__GetConfigurationOptions;
	struct _tptz__GetConfigurationOptionsResponse tptz__GetConfigurationOptionsResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	tptz__GetConfigurationOptions.ConfigurationToken = ConfigurationToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GetConfigurationOptions(soap, pService_Addr, NULL, &tptz__GetConfigurationOptions, &tptz__GetConfigurationOptionsResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_GetConfigurationOptions OK--------------\n");	
		printf("__sizePTZConfiguration = %s\n", tptz__GetConfigurationOptionsResponse.PTZConfigurationOptions->PTZRamps);
		printf("--------------------PTZ_GetConfigurationOptions Done--------------\n\n");
		iRet = 1;
	}

	soap_end(soap);
	free(soap);
	return iRet;
}

//云台设备的绝对移动， 使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_AbsoluteMove_C( HSOAP hSoap, const char *ProfileToken,
							const char *pService_Addr,
							const char* pUserName,
							const char* pPassword )
{
	float x = -1.0;     //位移
	float y = 1.0;     //位移
	float zoom_x = 0.1;
	struct _tptz__AbsoluteMove tptz__AbsoluteMove = {0};
	struct _tptz__AbsoluteMoveResponse tptz__AbsoluteMoveResponse = {0};
	struct tt__PTZVector	tagPTZVector = {0};
	struct tt__Vector2D		tagVector2D = {0};
	struct tt__Vector1D		tagVector1D = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	tptz__AbsoluteMove.ProfileToken = ProfileToken;
	tptz__AbsoluteMove.Position = &tagPTZVector;
	tptz__AbsoluteMove.Position->PanTilt = &tagVector2D;
	tptz__AbsoluteMove.Position->Zoom = &tagVector1D;
	tptz__AbsoluteMove.Position->PanTilt->x = x;
	tptz__AbsoluteMove.Position->PanTilt->y = y;
	tptz__AbsoluteMove.Position->Zoom->x = zoom_x;     //变焦

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__AbsoluteMove(soap, pService_Addr, NULL, &tptz__AbsoluteMove, &tptz__AbsoluteMoveResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_AbsoluteMove OK--------------\n");	
		printf("--------------------PTZ_AbsoluteMove Done--------------\n\n");
		iRet = 1;
	} 

	return iRet;
}

//云台设备的连续移动，使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_ContinuousMove_C( HSOAP hSoap, const char *ProfileToken,
							const char *pService_Addr,
							const char* pUserName,
							const char* pPassword, 
							unsigned char uType, unsigned char uSpeed )
{
	float fSpeed = 0.5;
	struct _tptz__ContinuousMove tptz__ContinuousMove = {0};
	struct _tptz__ContinuousMoveResponse tptz__ContinuousMoveResponse = {0};
	struct tt__PTZSpeed	tagPTZSpeed = {0};
	struct tt__Vector2D	tagVector2D = {0};
	struct tt__Vector1D	tagVector1D = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	if( uSpeed > 0 )
		fSpeed = 1.0 * uSpeed / 7;

	tptz__ContinuousMove.ProfileToken = ProfileToken;
	tptz__ContinuousMove.Velocity = &tagPTZSpeed;
	tptz__ContinuousMove.Velocity->PanTilt = &tagVector2D;
	tptz__ContinuousMove.Velocity->Zoom = &tagVector1D;

	switch( uType )
	{
	case PTZ_TILT_UP:		//云台上仰
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_TILT_DOWN:		//云台下俯
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed * -1;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_PAN_LEFT:		//云台左转
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed * -1;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_PAN_RIGHT:		//云台右转
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_UP_LEFT:		//云台上仰和左转
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed * -1;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_UP_RIGHT:		//云台上仰和右转
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_DOWN_LEFT:		//云台下俯和左转
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed * -1;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed * -1;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_DOWN_RIGHT:	//云台下俯和右转
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed * -1;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_ZOOM_IN:		//近焦
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = fSpeed * -1;       //变焦，正数是缩，负数是伸
		}
		break;
	case PTZ_ZOOM_OUT:		//远焦
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = fSpeed;       //变焦，正数是缩，负数是伸
		}
		break;
	}

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__ContinuousMove(soap, pService_Addr, NULL, &tptz__ContinuousMove, &tptz__ContinuousMoveResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_ContinuousMove OK--------------\n");	
		printf("--------------------PTZ_ContinuousMove Done--------------\n\n");
		iRet = 1;
	} 

	return iRet;
}

//云台设备的停止移动指令，使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_Stop_C( HSOAP hSoap, const char *ProfileToken, 
					const char *pService_Addr,
					const char* pUserName,
					const char* pPassword )
{
	struct _tptz__Stop tptz__Stop = {0};
	struct _tptz__StopResponse tptz__StopResponse = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	tptz__Stop.ProfileToken = ProfileToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__Stop(soap, pService_Addr, NULL, &tptz__Stop, &tptz__StopResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_Stop OK--------------\n");	
		printf("--------------------PTZ_Stop Done--------------\n\n");
		iRet = 1;
	} 

	return iRet;
}

//设置云台设备的家位置点， 使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_SetHomePosition_C( const char *ProfileToken,
								const char *pService_Addr,
								const char* pUserName,
								const char* pPassword )
{
	struct _tptz__SetHomePosition tptz__SetHomePosition;
	struct _tptz__SetHomePositionResponse tptz__SetHomePositionResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	tptz__SetHomePosition.ProfileToken = ProfileToken;

	printf("---------------Now PTZ_SetHomePosition---------------\n");
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__SetHomePosition(soap, pService_Addr, NULL, &tptz__SetHomePosition, &tptz__SetHomePositionResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_SetHomePosition OK--------------\n");	
		printf("--------------------PTZ_SetHomePosition Done--------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);     //用完soap要结束释放掉
	free(soap);
	return iRet;
}

//回到云台设备的家位置点， 使用的设备的ProfileToken 和设备云台的服务地址
int ONVIF_PTZ_GotoHomePosition_C( const char *ProfileToken,
								const char *pService_Addr,
								const char* pUserName,
								const char* pPassword )
{
	struct _tptz__GotoHomePosition tptz__GotoHomePosition;
	struct _tptz__GotoHomePositionResponse tptz__GotoHomePositionResponse;
	struct tt__PTZSpeed tagPTZSpeed = {0};
	struct tt__Vector2D	tagVector2D = {0};

	int iRet = -1;
	struct soap *soap = soap_new();  //初始化一个soap变量
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //设置命名空间

	tptz__GotoHomePosition.ProfileToken = ProfileToken;
	tptz__GotoHomePosition.Speed = &tagPTZSpeed;
	tptz__GotoHomePosition.Speed->PanTilt = &tagVector2D;
	tptz__GotoHomePosition.Speed->PanTilt->x = 1;   //可选的至少要实现同一个
	tptz__GotoHomePosition.Speed->PanTilt->y = 1;

	printf("---------------Now PTZ_GotoHomePosition----------------\n");
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //可以每个接口都使用新的一个soap变量，但是都要对他进行鉴权先，再使用
	soap_call___tptz__GotoHomePosition(soap, pService_Addr, NULL, &tptz__GotoHomePosition, &tptz__GotoHomePositionResponse);

	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_GotoHomePosition OK--------------\n");	
		printf("--------------------PTZ_GotoHomePosition Done--------------\n\n");
		iRet = 1;
	} 

	soap_end(soap);
	free(soap);
	return iRet;
}


#ifdef __cplusplus
}
#endif

//****************************************************************onvif接口：begin**************************** 
