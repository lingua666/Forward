
#ifndef __CAPTURE_AUDIO_DEVICE_WINDOWS_20201229104504_1444199361_H__
#define __CAPTURE_AUDIO_DEVICE_WINDOWS_20201229104504_1444199361_H__

#include <libThirdWrap_Audio/AudioDeviceWindowsCore.h>

namespace _audio_ {

	class CaptureAudioWindows
	{
	public:
		typedef function20_handle<void (const Device_INFO_sptr&, BYTE*, UInt32)>	HFNCapture;

		static MemPool_type* GetPoolInstance(void);

		static Device_INFO_sptr Alloc(void);

		static void Free(tagDevice_INFO* pInfo);

	public:
		CaptureAudioWindows(void);

		~CaptureAudioWindows(void);

		int32_t Init(void);

		void Release(void);

		int32_t Enumerate(Device_INFO_List* pList);

		Device_INFO_sptr GetDefaultDevice(void);

		int32_t GetVolume(const Device_INFO_sptr& Device_sptr, uint32_t& Volume);

		//Volume： 0.0f-255.0f
		int32_t SetVolume(const Device_INFO_sptr& Device_sptr, uint32_t Volume);

		int32_t	Start(const Device_INFO_sptr& Device_sptr,
			const tWAVEFORMATEX& Format, const HFNCapture& hCapture);

		int32_t	Stop(void);

		bool IsFormatSupported(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format);

	protected:

		//枚举音频设备
		int32_t EnumerateDevices(IMMDeviceEnumerator* ptrEnumerator, EDataFlow Type, Device_INFO_List* pList);

		_audio_string_t GetDeviceID(IMMDevice*	ptrDevice);

		_audio_string_t GetDeviceName(IMMDevice* ptrDevice);

		int32_t	GetDeviceWAVEFORMATEX(IMMDevice* ptrDevice, tWAVEFORMATEX* pOutWfx);

		Device_INFO_sptr GetDefaultDevice(IMMDeviceEnumerator*	ptrEnumerator, EDataFlow Type);

		//获取音频设备对象
		IAudioCaptureClient* GetClient(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format,
			HANDLE hCaptureSamplesReadyEvent);

		void RevertThreadPriority(void);

		//调用该接口的线程则为 MMCSS服务工作线程
		DWORD InitThreadPriority(void);

		void DoThread(const Device_INFO_sptr& sptr, const HFNCapture& hCapture);

		//回音消除AEC
		void DoThreadDMO(const Device_INFO_sptr& sptr, const HFNCapture& hCapture);

		//回音消除AEC
		int CaptureAudioWindows::SetDMOProperties(Device_INFO_sptr Device_sptr);

		//AEC回音消除
		int SetBoolProperty(IPropertyStore* ptrPS,
			REFPROPERTYKEY key,
			VARIANT_BOOL value);

		int SetVtI4Property(IPropertyStore* ptrPS,
			REFPROPERTYKEY key,
			LONG value);

	private:
		ScopedCOMInitializer	_ComInit;
		IMMDeviceEnumerator*	_ptrEnumerator;
		IMediaObject*			_ptrDmo;			//AEC
		IPropertyStore*			_ptrDmoPS;
		IMMDeviceCollection*	_ptrCollection;
		HANDLE					_hSamplesReadyEvent;
		ThreadWrap				_Thread;

		PAvRevertMmThreadCharacteristics    _PAvRevertMmThreadCharacteristics;
		PAvSetMmThreadCharacteristicsA      _PAvSetMmThreadCharacteristicsA;
		PAvSetMmThreadPriority              _PAvSetMmThreadPriority;
		HMODULE                             _avrtLibrary;
		bool                                _winSupportAvrt;
		HANDLE                              _hMmTask;
		bool								_builtInAecEnabled;
	};

}

//#endif

#endif
