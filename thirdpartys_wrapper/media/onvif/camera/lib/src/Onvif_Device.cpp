
#include <libThirdWrap_Onvif_Camera/Onvif_Device.h>
#include <libThirdWrap_RtspClient/DummySink.h>
#include <dllThirdWrap_FFmpeg/Exports.h>


Onvif_Device::Onvif_Device( void )
	: _pRtsp( 0 )
	, _hDec( 0 )
{
	memset(&_DecData, 0, sizeof(_DecData));
	MDecoder_Video_Init();
	GetRtspClientManagerInstance()->Init();
}

Onvif_Device::~Onvif_Device( void )
{
	Close();
}

int Onvif_Device::Connect( const _string_type& sService_Addr,
	const _string_type& sUserName,
	const _string_type& sPassword )
{
	if( _Onvif.Init(sService_Addr.c_str(),
					sUserName.c_str(),
					sPassword.c_str(), 0) == -1 )
		return -1;

	_MediaUrl = _Onvif.GetMediaUri();
	return 1;
}

void Onvif_Device::Close( void )
{
	_Onvif.Relase();
}

int Onvif_Device::SetRealDataCallBack( const HFNRealData& hRealData )
{
	_hRealData = hRealData;
	return -1;
}

int	Onvif_Device::Start_Preview( HWND hPlayWnd )
{
	if( !_Onvif.is_open() )
		return -1;

	Stop_Preview();
	
	_pRtsp = GetRtspClientManagerInstance()->Open(_MediaUrl.c_str());
	if( _pRtsp != NULL )
		_pRtsp->SetRealDataCallBack( function20_bind(&Onvif_Device::HandleRealData, this,
									hPlayWnd, _function_::_1, _function_::_2) );

	return _pRtsp != NULL ? 1 : -1;
}

int	Onvif_Device::Stop_Preview( void )
{
	if( _pRtsp == NULL )
		return -1;

	GetRtspClientManagerInstance()->Close(_pRtsp);
	_pRtsp = NULL;
	return 1;
}

//设置校验
int Onvif_Device::SetTimerCFG( NET_DVR_TIME& tagTimer )
{
	tagOnvif_TimeAndDate	Date;
	Date.Year = tagTimer.dwYear;
	Date.Month = tagTimer.dwMonth;
	Date.Day = tagTimer.dwDay;
	Date.Hour = tagTimer.dwHour;
	Date.Minute = tagTimer.dwMinute;
	Date.Second = tagTimer.dwSecond;
	if( _Onvif.SetSystemTimeAndDate(&Date) == -1 )
		return -1;

	return 1;
}

int Onvif_Device::GetTimerCFG( NET_DVR_TIME& tagTimer )
{
	tagOnvif_TimeAndDate	Date;
	if( _Onvif.GetSystemTimeAndDate(&Date) == -1 )
		return -1;

	tagTimer.dwYear = Date.Year;
	tagTimer.dwMonth = Date.Month;
	tagTimer.dwDay = Date.Day;
	tagTimer.dwHour = Date.Hour;
	tagTimer.dwMinute = Date.Minute;
	tagTimer.dwSecond = Date.Second;
	return 1;
}

//云台预置点
int Onvif_Device::PTZPreset( void )
{
	return _Onvif.PTZ_GotoPreset();
}

//云台
int Onvif_Device::PTZControl( UInt8 uPTZCommand, UInt8 uSpeed )
{
	return _Onvif.PTZ_ContinuousMove(uPTZCommand, uSpeed);
}

int Onvif_Device::PTZControl_Stop( void )
{
	return _Onvif.PTZ_Stop();
}

// dwSpeed: 取值范围[1,7] 
int Onvif_Device::PTZControlWithSpeed( DWORD dwPTZCommand, DWORD dwSpeed )
{
	return -1;
}

int Onvif_Device::PTZControlWithSpeed_Stop( DWORD dwPTZCommand, DWORD dwSpeed )
{
	return -1;
}

void	Onvif_Device::HandleRealData( HWND hWnd, UInt8* pData, UInt32 uSize )
{
	if( hWnd != NULL )
	{//显示到句柄地方
		if( _hDec == NULL )
		{
			if( _DecData.pData == NULL )
			{
				_DecData.pData = DummySink::Alloc();
				_DecData.uSize = 0;
			}

			if( _DecData.uSize + uSize > DUMMY_SINK_RECEIVE_BUFFER_SIZE )
				_DecData.uSize = 0;

			memcpy(&_DecData.pData[_DecData.uSize], pData, uSize);
			_DecData.uSize += uSize;

			_hDec = MDecoder_Parse((const char*)_DecData.pData, _DecData.uSize);
			if( _hDec != NULL )
			{
				MDecoder_Set(_hDec, hWnd);
				DummySink::Free(_DecData.pData);
			}
		}
		else
		{
			MDecoder_Decode(_hDec, (const char*)pData, uSize);
		}
	}

	if( _hRealData )
		_hRealData(pData, uSize);
}
