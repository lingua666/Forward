
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

	//校时结构参数
	typedef struct
	{
		UInt32 dwYear;		//年
		UInt32 dwMonth;		//月
		UInt32 dwDay;		//日
		UInt32 dwHour;		//时
		UInt32 dwMinute;	//分
		UInt32 dwSecond;	//秒
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

	//设置校验
	int SetTimerCFG( NET_DVR_TIME& tagTimer );

	int GetTimerCFG( NET_DVR_TIME& tagTimer );

	//云台预置点
	int PTZPreset( void );

	//云台控制
	int PTZControl( UInt8 uPTZCommand, UInt8 uSpeed = 0 );

	int PTZControl_Stop( void );

	// dwSpeed: 取值范围[1,7] 
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
