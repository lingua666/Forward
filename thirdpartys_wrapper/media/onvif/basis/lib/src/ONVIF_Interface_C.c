//��Դ�ļ��Ƕ�onvif�ӿں����ĵķ�װ����Ϊ��main�ļ�Ϊcpp��Ҫ����cpp��ʹ��c�Ĵ��룬��Ҫʹ��extern C ����������װc�ĺ����ӿ�
//�����ڸ��ļ��ڲ��ܰ����й�cpp�Ĵ������ͷ�ļ�����cpp��Դ�ļ�����Ҳ���ܰ����й�c�Ĵ������ͷ�ļ����÷�װc�ӿڵ�ͷ�ļ����⣨ONVIF_Interface.h��
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

#define	MULTICAST_ADDRESS	"soap.udp://239.255.255.250:3702"		//�ú궨��ಥ��ַ
#define MAX_64_LEN			20                             
#define INFO_LENGTH			40
#define MAX_PROF_TOKEN		20


//****************************************************************onvif�ӿڣ�begin*****************************

#ifdef __cplusplus
extern "C" {
#endif

HSOAP	ONVIF_malloc( const char* c_pUserName, const char* c_pPassword )
{
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return 0;

	soap_set_namespaces(soap, namespaces);   //���������ռ�

	if( c_pUserName != NULL && c_pPassword != NULL )
		soap_wsse_add_UsernameTokenDigest(soap, NULL, c_pUserName, c_pPassword);         //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��

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
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	if( c_pUserName == NULL || c_pPassword == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, c_pUserName, c_pPassword);         //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
	return 1;
}

//��ȡ�豸��Ϣ������Ľ�������ֻ�����ں������豸��
int ONVIF_GetDevInformation_C( const char* pDevice_t, const char* pService_Addr,
							const char* pUserName, const char* pPassword,
							tagOnvif_DevInfo* _OUT pDevInfo )
{
	struct _tds__GetDeviceInformationResponse tds__GetDeviceInformationResponse;  
	struct _tds__GetDeviceInformation tds__GetDeviceInformation;  
	const char* soap_endpoint = pService_Addr;  //�������ĵ�ַ
	
	int iResult = -1;
	int ret = 0;

	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap, namespaces);   //���������ռ�

	if(soap_register_plugin(soap, http_da))
		soap_print_fault(soap, stderr); // failed to register  �Ǽ�ʧ��
	
	ret = soap_call___tds__GetDeviceInformation(soap,soap_endpoint,NULL,&tds__GetDeviceInformation,&tds__GetDeviceInformationResponse);  //��ȡ�豸��Ϣ

	memset(pDevInfo, 0, sizeof(tagOnvif_DevInfo));

	if (ret != SOAP_OK)  
	{  
		if (ret == 401) // challenge: HTTP authentication required  ��֤ʧ��
		{  
			if (!strcmp(soap->authrealm, pDevice_t)) //�Ա��豸�ͺ�
			{  
				struct http_da_info info; //�����洢�û���������
				http_da_save(soap, &info, pDevice_t, pUserName, pPassword); // �����豸�û���������
				
				//���ּ�Ȩ��ʽ������
				//soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);         //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
				if (soap_call___tds__GetDeviceInformation_(soap, soap_endpoint, NULL, &tds__GetDeviceInformation,&tds__GetDeviceInformationResponse)== SOAP_OK) // ���»�ȡ�豸��Ϣ
				{  
					strcpy(pDevInfo->Manufacturer, tds__GetDeviceInformationResponse.Manufacturer);
					strcpy(pDevInfo->HardwareId, tds__GetDeviceInformationResponse.HardwareId);
					strcpy(pDevInfo->FirmwareVersion, tds__GetDeviceInformationResponse.FirmwareVersion);
					strcpy(pDevInfo->SerialNumber, tds__GetDeviceInformationResponse.SerialNumber);
			
					http_da_restore(soap, &info); // ��մ洢���û��������� 
					http_da_release(soap, &info); // ɾ���û���������

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

//RecvTimeout�� ��ʱʱ�䣬 ��λ��
//fpnOnvif_DiscoveryCallBack ֪ͨ�ص�����
int ONVIF_ClientDiscovery_C( int RecvTimeout,
							fpnOnvif_DiscoveryCallBack fpnDiscovery,
							void* pUser,
							const char* szIP )
{  
	unsigned char macaddr[6];     //�����ַ    
	char _HwId[1024];             //��������guid��  
	unsigned int Flagrand;        //����ı�־  

	int iRet = -1, result = 0, i = 0;                                                //�����ж��Ƿ�ִ�гɹ���
	wsdd__ProbeType req;                                                             //�ͻ��˷��͵�Probe
	struct __wsdd__ProbeMatches resp;                                                //����˻ص�Probematchs  
	wsdd__ScopesType sScope;                                                         //Probe��������Է�Χ
	struct SOAP_ENV__Header header;                                                  //SOAP��ͷ��Ϣ 
	struct in_addr if_req;
	struct soap soap;																 //soap��������
	soap_init(&soap);

	if( szIP != NULL )
	{
		if_req.s_addr = inet_addr(szIP);  // ��󶨵�IP��ַ
		soap.ipv4_multicast_if = (char*)soap_malloc(&soap, sizeof(struct in_addr));
		memset(soap.ipv4_multicast_if, 0, sizeof(struct in_addr));
		memcpy(soap.ipv4_multicast_if, (char*)&if_req, sizeof(if_req));
	}

	soap.recv_timeout = RecvTimeout;                        //���ó�ʱʱ��
	soap_set_namespaces(&soap, namespaces);                   //��������ʱ��
	soap_default_SOAP_ENV__Header(&soap, &header);            //��header����Ϊsoap����Ϣͷ

	//��ȡ��guid��Ψһ��ʶ��(windows�½�guid,Linux�½�uuid),��ʽΪurn:uuid:8-4-4-4-12,��ϵͳ�������
	// ��ȡһ�������������Ϊ��ʶ��
	srand((int)time(0));   //ϵͳ�ṩ������α��������е����Ӻ�������ʼ��������
	Flagrand = rand()%9000 + 8888;   //�����������
	macaddr[0] = 0x1;                //�����ַ
	macaddr[1] = 0x2; 
	macaddr[2] = 0x3; 
	macaddr[3] = 0x4; 
	macaddr[4] = 0x5; 
	macaddr[5] = 0x6;
	//_HwId�����洢uuid�ģ��õ�������룬�����������ַ��
	sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X", 
			Flagrand, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

    ///////////����Ϣͷ��ֵ//////////
	header.wsa__MessageID = _HwId;                     //��Ϣͷ��id�������ʶ�������
	header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";               //��ϢҪ������ URI 
	header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";  //�����˷����д�����Ϣ��������WSDL�ж���Ĳ����� Action
	soap.header = &header;                
	
	//��������Ѱ���豸�����ͺͷ�Χ,���������趨һ��,��������յ���ONVIF�豸,�����쳣
	soap_default_wsdd__ScopesType(&soap, &sScope);  
	sScope.__item = "";  
	soap_default_wsdd__ProbeType(&soap, &req);  
	req.Scopes = &sScope;  
	req.Types = ""; //"dn:NetworkVideoTransmitter"  
	
	//ͨ���鲥����Probe̽��,���ͳɹ�����0,����-1   soap_call___tds__GetSystemSupportInformation
    result = soap_send___wsdd__Probe(&soap, MULTICAST_ADDRESS, NULL, &req);  
	
	//ѭ����������˵Ļ�Ӧ
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
							strcpy(DiscoveryInfo.Service_Addr, resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);      //ȫ�ִ�ŷ�������ַ

							if( strlen( DiscoveryInfo.Service_Addr ) > strlen("http://") )
							{
								char* p = strchr(&DiscoveryInfo.Service_Addr[strlen("http://")], '/');
								strncpy(DiscoveryInfo.IP, &DiscoveryInfo.Service_Addr[strlen("http://")], p - (char*)&DiscoveryInfo.Service_Addr[strlen("http://")]);
							}

							//�������ص��豸��Ϣ
							if(resp.wsdd__ProbeMatches->ProbeMatch->Scopes != NULL)  
							{  
								char *temp = strstr(resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item, "name") + 5;
								strncpy(DiscoveryInfo.Manufacturer, temp, 9);

								temp = strstr(resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item, "hardware") + 9;
								for(i = 0; i < 20; i++)
								{
									if(isspace(temp[i]))            //�жϸ��ַ��Ƿ��ǿո�����ǿո��ֹͣ����
										strncpy(DiscoveryInfo.Device_t, temp, i);      //���ո�ǰ���ַ������Ƹ�str1
								}

							}  
						}
					}

					if( strcmp(DiscoveryInfo.Device_t, "") )//�豸�ͺ�Ϊ�������
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


//��ȡ�豸������ֵ��ʹ�õ����豸��ַ����̨��ַ��Ҳ���Ƿ���������ȡý����Ϣ��ַ�ṩ��GetProfiles�ӿ�ʹ��,ret_data��ŷ������ݵĽṹ��
int ONVIF_GetCapabilities_C( HSOAP hSoap, const char *pService_Addr,
						const char* pUserName,
						const char* pPassword,
						ONVIF_GetCapabilities_C_DATE* _OUT Ret_Data )
{                                          
	struct _tds__GetCapabilities tds__GetCapabilities;                         //GetCapabilities
	struct _tds__GetCapabilitiesResponse tds__GetCapabilitiesResponse;         //GetCapabilities
	enum tt__CapabilityCategory	tagCategory = {0};	
	int iRet = -1;

	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	tds__GetCapabilities.Category = &tagCategory;
	tds__GetCapabilities.__sizeCategory = 1; 
	*(tds__GetCapabilities.Category) = (enum tt__CapabilityCategory)(tt__CapabilityCategory__All);  //����������û�ȡ�ĵ������ݻ����ǣ�All��Ϊȫ����ȡ

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

				//����Ϳ����ж����ǲ�������ˣ�����������ִ������Ĵ����ȡ�豸����̨��ַ
				if(tds__GetCapabilitiesResponse.Capabilities->PTZ != NULL)
				{
					strcpy(Ret_Data->PTZ_addr, tds__GetCapabilitiesResponse.Capabilities->PTZ->XAddr);     
					Ret_Data->isPTZ = 1;                            //�ж��ǲ�������ı�־λ��0Ϊ���ǣ�1����
				}
				else
					Ret_Data->isPTZ = 0;                            //�ж��ǲ�������ı�־λ��0Ϊ���ǣ�1����
			}

			iRet = 1;
		} 
	}

	return iRet;
}

//��ȡ�豸����ʹ�õ����豸�����ַ
int ONVIF_GetService_C( HSOAP hSoap, 
					const char *pService_Addr,
					const char* pUserName,
					const char* pPassword )
{
	struct _tds__GetServices tds__GetServices;
	struct _tds__GetServicesResponse tds__GetServicesResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡý�����Ϣ�ļ���ʹ�õ���ý����Ϣ��ַ��ʶ��������ͨ���ı���ֱ��ʣ��ṩ��GetStreamuri�ӿ�ʹ��, ret_data����GetProfiles�����ݽṹ��
int ONVIF_GetProfiles_C( HSOAP hSoap, const char* Media_Addr,
						const char* pUserName,
						const char* pPassword,
						unsigned char isMain,
						ONVIF_GetProfiles_C_DATE* _OUT Ret_Data)
{ 
	struct _trt__GetProfiles trt__GetProfiles;
	struct _trt__GetProfilesResponse trt__GetProfilesResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;			//��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

		strcpy(Ret_Data->VideoSourcesToken, trt__GetProfilesResponse.Profiles[iType].VideoSourceConfiguration->token);   //���token��Configuration��token������ONVIF_GetVideoSourceConfiguration�����Ĳ�����sourcetoken��Configuration token��һ��
		strcpy(Ret_Data->ProfilesToken, trt__GetProfilesResponse.Profiles[iType].token);
	} 

	return iRet;
 }

//��ȡ��ý��ĵ�ַ��ʹ�õ���ý����Ϣ��ַ���ṩ��RTSP��ȡ��Ƶ��������Ҫ��uri��ַ�ϼ����û��������룬ret_MediaUri��ŷ��ص���ý��ĵ�ַ
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

	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL || Media_Addr == NULL || Profiles_token == NULL )
		return -1;

	trt__GetStreamUri.StreamSetup = &tagSetup;            //��ʼ��������ռ� 
	trt__GetStreamUri.StreamSetup->Stream = tt__StreamType__RTP_Unicast;                                                  //�������� 
	trt__GetStreamUri.StreamSetup->Transport = &tagTransport;   //��ʼ��������ռ� 
	trt__GetStreamUri.StreamSetup->Transport->Protocol = tt__TransportProtocol__UDP;                                      //Э������
	trt__GetStreamUri.StreamSetup->Transport->Tunnel = 0; 
	trt__GetStreamUri.ProfileToken = Profiles_token ; 

	sprintf(UserAndPassword,"rtsp://%s:%s@", pUserName, pPassword); //�豸�û���������

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword);            //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
	soap_call___trt__GetStreamUri(soap, Media_Addr, NULL, &trt__GetStreamUri, &trt__GetStreamUriResponse); 
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 		
		//�����û���������: "rtsp://admin:abcd-1234@192.168.8.23:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";
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
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	tse__GetRecordingInformation.RecordingToken = NULL; //ҪRecordingToken������������
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡ��ƵԴ��ʹ�õ���ý����Ϣ��ַMedia_Addr
int ONVIF_GetVideoSources_C( HSOAP hSoap, const char *Media_Addr )
{
	struct _trt__GetVideoSources trt__GetVideoSources = {0};
	struct _trt__GetVideoSourcesResponse trt__GetVideoSourcesResponse = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
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

	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
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

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
	iRet=  soap_call___tds__SetSystemDateAndTime(soap, pService_Addr, NULL, &SetSystemDateAndTime, &SetSystemDateAndTimeResponse) ;
	//�ɹ���ȡ�豸�����ں�ʱ����Ϣ
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

	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
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

//��ȡ��ƵԴ����Ϣ�� ʹ�õ���ý���ַ����ƵԴ��token
int ONVIF_GetVideoSourceConfiguration_C(const char *Media_Addr,
									const char *VideoSourcesToken,
									const char* pUserName,
									const char* pPassword)
{
	struct _trt__GetVideoSourceConfiguration trt__GetVideoSourceConfiguration = {0};
	struct _trt__GetVideoSourceConfigurationResponse trt__GetVideoSourceConfigurationResponse = {0};

	int iRet = -1;
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if(soap != NULL)
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	trt__GetVideoSourceConfiguration.ConfigurationToken = VideoSourcesToken;
	//trt__GetVideoSourceConfigurationResponse.Configuration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
	//memset(trt__GetVideoSourceConfigurationResponse.Configuration, 0, sizeof(struct tt__VideoSourceConfiguration));

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡ��Ƶ���������ʹ�õ�����ý��ĵ�ַ
int ONVIF_GetVideoOutputs_C(const char *Media_Addr,
							const char* pUserName,
							const char* pPassword )
{
	struct _tmd__GetVideoOutputs tmd__GetVideoOutputs = {0};
	struct _tmd__GetVideoOutputsResponse tmd__GetVideoOutputsResponse = {0};

	int iRet = -1;
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡ��̨���豸��״̬����ַ�õ��Ǿ߱���̨���ܵķ�������ַ ��ʹ�õ���ProfilesToken ���豸����̨�����ַ��ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_PTZ_GetStatus_C( const char *ProfilesToken,
						const char *pService_Addr,
						const char* pUserName,
						const char* pPassword )
{
	struct _tptz__GetStatus tptz__GetStatus;
	struct _tptz__GetStatusResponse tptz__GetStatusResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	tptz__GetStatus.ProfileToken = ProfilesToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡ��̨�豸��nodesֵ����ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ",ret_GetNodeToken��ŷ��ص�NodeToken
int ONVIF_PTZ_GetNodes_C( HSOAP hSoap, const char *pService_Addr,
						const char* pUserName,
						const char* pPassword,
						char* _OUT Ret_GetNodeToken )
{
	struct _tptz__GetNodes tptz__GetNodes;
	struct _tptz__GetNodesResponse tptz__GetNodesResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡ��̨�豸��nodeֵ, ��ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ"
int ONVIF_PTZ_GetNode_C( HSOAP hSoap, const char *Token,
						const char *pService_Addr,
						const char* pUserName,
						const char* pPassword )
{
	struct _tptz__GetNode tptz__GetNode;
	struct _tptz__GetNodeResponse tptz__GetNodeResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	tptz__GetNode.NodeToken = Token;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
	soap_call___tptz__GetNode(soap, pService_Addr, NULL, &tptz__GetNode, &tptz__GetNodeResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		printf("\n-------------------PTZ_GetNode OK--------------\n");	
		printf("AuxiliaryCommands = %s\n", tptz__GetNodeResponse.PTZNode->AuxiliaryCommands);              //ΪNULL˵����֧�ָ�������
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


//��ȡ�豸��̨��Presetsֵ,��ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ"  ret_data���ڷ��ص����ݵĽṹ��
int ONVIF_PTZ_GetPresets_C( HSOAP hSoap, const char *ProfileToken,
							const char *pService_Addr,
							const char* pUserName,
							const char* pPassword,
							ONVIF_PTZ_GetPresets_C_DATE* _OUT Ret_Data)
{
	struct _tptz__GetPresets tptz__GetPresets;
	struct _tptz__GetPresetsResponse tptz__GetPresetsResponse;

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	tptz__GetPresets.ProfileToken = ProfileToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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


//�����豸��̨��Presetֵ����ַ�õ��Ǿ߱���̨���ܵķ�������ַ  ps:"http://192.168.8.23/onvif/PTZ"  ret_PresetToken���ڷ��ص�����
//����ӿ��õ���GetPresets���ص���Ϣ�����ṹ��ģ�
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
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	tptz__SetPreset.ProfileToken = ProfileToken;
	tptz__SetPreset.PresetToken = PresetTokenm;
	tptz__SetPreset.PresetName = PresetName;

	printf("---------------Now PTZ_SetPreset----------------\n");
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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


//�ص�Ԥ��㣬����ӿ���GetPresets ������SetPreset ���ص���Ϣ�����ṹ��  ,�Ƴ�Ԥ�裨RemovePreset��Ҳ�Ǻ�GotoPresetһ��
//ͨ��GetPresets ������SetPreset����ȡ������Ԥ���֮������GotoPreset�ص�Ԥ��㣬ǰ���ǣ��������ú�Ԥ���֮�����Ҫ�仯��ǰλ�ã����øýӿڲŻ���Ч��֮��Ż�ص�Ԥ���
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

	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	tptz__GotoPreset.ProfileToken = ProfileToken;
	tptz__GotoPreset.PresetToken = PresetToken;
	tptz__GotoPreset.Speed = &tagPTZSpeed;
	tptz__GotoPreset.Speed->PanTilt = &tagVector2D;
	tptz__GotoPreset.Speed->PanTilt->x = x;
	tptz__GotoPreset.Speed->PanTilt->y = y;

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��ȡ�豸��̨��ȫ����Ϣ��ʹ�õ����豸��̨�ķ����ַ,ret_data ���ڷ������ݵĽṹ��
int ONVIF_PTZ_GetConfigurations_C( const char *pService_Addr, 
								const char* pUserName,
								const char* pPassword,
								ONVIF_PTZ_GetConfigurations_C_DATE *Ret_Data )
{
	struct _tptz__GetConfigurations tptz__GetConfigurations;
	struct _tptz__GetConfigurationsResponse tptz__GetConfigurationsResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
	soap_call___tptz__GetConfigurations(soap, pService_Addr, NULL, &tptz__GetConfigurations, &tptz__GetConfigurationsResponse);
	if (soap->error) 
	{ 
		printf("soap error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
	} 
	else
	{ 
		strcpy(Ret_Data->ConfigurationToken, tptz__GetConfigurationsResponse.PTZConfiguration->token);   //���ص�����
		strcpy(Ret_Data->TimeOut, (char *)tptz__GetConfigurationsResponse.PTZConfiguration->DefaultPTZTimeout);   //���ص�����
		strcpy(Ret_Data->spcae, tptz__GetConfigurationsResponse.PTZConfiguration->DefaultAbsolutePantTiltPositionSpace);     //���ص�����
		iRet = 1;
	}

	soap_end(soap);
	free(soap);
	return iRet;
}

//��ȡ�豸��̨�ĵ������Ϣ��ʹ�õ�ConfigurationToken ���豸��̨�ķ����ַ
int ONVIF_PTZ_GetConfigurationOptions_C( const char *ConfigurationToken,
										const char *pService_Addr,
										const char* pUserName,
										const char* pPassword )
{
	struct _tptz__GetConfigurationOptions tptz__GetConfigurationOptions;
	struct _tptz__GetConfigurationOptionsResponse tptz__GetConfigurationOptionsResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	tptz__GetConfigurationOptions.ConfigurationToken = ConfigurationToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��̨�豸�ľ����ƶ��� ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ
int ONVIF_PTZ_AbsoluteMove_C( HSOAP hSoap, const char *ProfileToken,
							const char *pService_Addr,
							const char* pUserName,
							const char* pPassword )
{
	float x = -1.0;     //λ��
	float y = 1.0;     //λ��
	float zoom_x = 0.1;
	struct _tptz__AbsoluteMove tptz__AbsoluteMove = {0};
	struct _tptz__AbsoluteMoveResponse tptz__AbsoluteMoveResponse = {0};
	struct tt__PTZVector	tagPTZVector = {0};
	struct tt__Vector2D		tagVector2D = {0};
	struct tt__Vector1D		tagVector1D = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	tptz__AbsoluteMove.ProfileToken = ProfileToken;
	tptz__AbsoluteMove.Position = &tagPTZVector;
	tptz__AbsoluteMove.Position->PanTilt = &tagVector2D;
	tptz__AbsoluteMove.Position->Zoom = &tagVector1D;
	tptz__AbsoluteMove.Position->PanTilt->x = x;
	tptz__AbsoluteMove.Position->PanTilt->y = y;
	tptz__AbsoluteMove.Position->Zoom->x = zoom_x;     //�佹

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��̨�豸�������ƶ���ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ
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
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
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
	case PTZ_TILT_UP:		//��̨����
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_TILT_DOWN:		//��̨�¸�
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed * -1;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_PAN_LEFT:		//��̨��ת
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed * -1;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_PAN_RIGHT:		//��̨��ת
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_UP_LEFT:		//��̨��������ת
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed * -1;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_UP_RIGHT:		//��̨��������ת
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_DOWN_LEFT:		//��̨�¸�����ת
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed * -1;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed * -1;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_DOWN_RIGHT:	//��̨�¸�����ת
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = fSpeed;
			tptz__ContinuousMove.Velocity->PanTilt->y = fSpeed * -1;
			tptz__ContinuousMove.Velocity->Zoom->x = 0.0;       //�佹��������������������
		}
		break;
	case PTZ_ZOOM_IN:		//����
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = fSpeed * -1;       //�佹��������������������
		}
		break;
	case PTZ_ZOOM_OUT:		//Զ��
		{
			tptz__ContinuousMove.Velocity->PanTilt->x = 0.0;
			tptz__ContinuousMove.Velocity->PanTilt->y = 0.0;
			tptz__ContinuousMove.Velocity->Zoom->x = fSpeed;       //�佹��������������������
		}
		break;
	}

	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//��̨�豸��ֹͣ�ƶ�ָ�ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ
int ONVIF_PTZ_Stop_C( HSOAP hSoap, const char *ProfileToken, 
					const char *pService_Addr,
					const char* pUserName,
					const char* pPassword )
{
	struct _tptz__Stop tptz__Stop = {0};
	struct _tptz__StopResponse tptz__StopResponse = {0};

	int iRet = -1;
	struct soap *soap = (struct soap *)hSoap;  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	tptz__Stop.ProfileToken = ProfileToken;
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//������̨�豸�ļ�λ�õ㣬 ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ
int ONVIF_PTZ_SetHomePosition_C( const char *ProfileToken,
								const char *pService_Addr,
								const char* pUserName,
								const char* pPassword )
{
	struct _tptz__SetHomePosition tptz__SetHomePosition;
	struct _tptz__SetHomePositionResponse tptz__SetHomePositionResponse;

	int iRet = -1;
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap == NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	tptz__SetHomePosition.ProfileToken = ProfileToken;

	printf("---------------Now PTZ_SetHomePosition---------------\n");
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

	soap_end(soap);     //����soapҪ�����ͷŵ�
	free(soap);
	return iRet;
}

//�ص���̨�豸�ļ�λ�õ㣬 ʹ�õ��豸��ProfileToken ���豸��̨�ķ����ַ
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
	struct soap *soap = soap_new();  //��ʼ��һ��soap����
	if( soap != NULL )
		return -1;

	soap_set_namespaces(soap,namespaces);   //���������ռ�

	tptz__GotoHomePosition.ProfileToken = ProfileToken;
	tptz__GotoHomePosition.Speed = &tagPTZSpeed;
	tptz__GotoHomePosition.Speed->PanTilt = &tagVector2D;
	tptz__GotoHomePosition.Speed->PanTilt->x = 1;   //��ѡ������Ҫʵ��ͬһ��
	tptz__GotoHomePosition.Speed->PanTilt->y = 1;

	printf("---------------Now PTZ_GotoHomePosition----------------\n");
	soap_wsse_add_UsernameTokenDigest(soap, NULL, pUserName, pPassword); //����ÿ���ӿڶ�ʹ���µ�һ��soap���������Ƕ�Ҫ�������м�Ȩ�ȣ���ʹ��
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

//****************************************************************onvif�ӿڣ�begin**************************** 
