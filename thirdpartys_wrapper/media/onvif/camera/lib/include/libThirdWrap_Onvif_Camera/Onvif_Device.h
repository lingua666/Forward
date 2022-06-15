
#ifndef __ONVIF_DEVICE_20151023151138_1445584298_H__
#define __ONVIF_DEVICE_20151023151138_1445584298_H__

#include "External.h"

/*!
* @class  Onvif_Device
* @brief  Onvif_Device
* @{
*/

class Onvif_Device
{
public:
	typedef	MyRTSPClient::HFNRealData	HFNRealData;

#pragma pack(push, 1)
	struct tagDecoderData
	{
		UInt8*	pData;
		UInt32	uSize;
	};

	//Уʱ�ṹ����
	typedef struct
	{
		UInt32 dwYear;		//��
		UInt32 dwMonth;		//��
		UInt32 dwDay;		//��
		UInt32 dwHour;		//ʱ
		UInt32 dwMinute;	//��
		UInt32 dwSecond;	//��
	}NET_DVR_TIME, *LPNET_DVR_TIME;
#pragma pack(pop)

public:
	/*!
	* @function   Onvif_Device
	* @brief    
	* @return  
	*/
	Onvif_Device( void );

	/*!
	* @function   ~Onvif_Device
	* @brief    
	* @return  
	*/
	~Onvif_Device( void );

	int Connect( const _string_type& sService_Addr,
				const _string_type& sUserName,
				const _string_type& sPassword );

	void Close( void );

	int SetRealDataCallBack( const HFNRealData& hRealData );

	int	Start_Preview( HWND hPlayWnd );

	int	Stop_Preview( void );

	//����У��
	int SetTimerCFG( NET_DVR_TIME& tagTimer );

	int GetTimerCFG( NET_DVR_TIME& tagTimer );

	//��̨Ԥ�õ�
	int PTZPreset( void );

	//��̨����
	int PTZControl( UInt8 uPTZCommand, UInt8 uSpeed = 0 );

	int PTZControl_Stop( void );

	// dwSpeed: ȡֵ��Χ[1,7] 
	int PTZControlWithSpeed( DWORD dwPTZCommand, DWORD dwSpeed );

	int PTZControlWithSpeed_Stop( DWORD dwPTZCommand, DWORD dwSpeed );

	bool	is_connect( void ) const
	{
		
	}
protected:
	void	HandleRealData( HWND hWnd, UInt8* pData, UInt32 uSize );

private:
	ONVIF_INTERFACE	_Onvif;
	HFNRealData		_hRealData;
	MyRTSPClient*	_pRtsp;
	_string_type	_MediaUrl;
	THandle			_hDec;
	tagDecoderData	_DecData;
};
/** @} end Onvif_Device */

inline RTSPClientManager* GetRtspClientManagerInstance( void )
{
	return Singleton<RTSPClientManager>::instance();
}

#endif//__HAID_DEVICE_20151023151138_1445584298_H__
