
#include <libThirdWrap_Audio/RenderAudioWindows.h>
#include <endpointvolume.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <dmo.h>
#include <uuids.h>
#include <atlstr.h>

#define ROUND(x) ((x) >=0 ? (int)((x) + 0.5) : (int)((x) - 0.5))

namespace _audio_ {

	const float MAX_CORE_SPEAKER_VOLUME = 255.0f;
	const float MIN_CORE_SPEAKER_VOLUME = 0.0f;
	const float MAX_CORE_MICROPHONE_VOLUME = 255.0f;
	const float MIN_CORE_MICROPHONE_VOLUME = 0.0f;


	MemPool_type* RenderAudioWindows::GetPoolInstance(void)
	{
		static MemPool_type	s_Pool(sizeof(tagDevice_INFO));
		return &s_Pool;
	}

	Device_INFO_sptr RenderAudioWindows::Alloc(void)
	{
		tagDevice_INFO* pInfo = GetPoolInstance()->AllocObj<tagDevice_INFO>();
		pInfo->pDevice = NULL;
		pInfo->pClient = NULL;
		pInfo->uIndex = 0;
		return Device_INFO_sptr(pInfo,
			function20_bind(&RenderAudioWindows::Free, pInfo));
	}

	void RenderAudioWindows::Free(tagDevice_INFO* pInfo)
	{
		SAFE_RELEASE(pInfo->pClient);
		SAFE_RELEASE(pInfo->pDevice);
		GetPoolInstance()->FreeObj(pInfo);
	}


	RenderAudioWindows::RenderAudioWindows(void)
		: _ptrEnumerator(NULL)
		, _ptrDmo(NULL)
		, _ptrCollection(NULL)
		, _hSamplesReadyEvent(NULL)
		, _ComInit(ScopedCOMInitializer::kMTA)
		, _avrtLibrary(NULL)
		, _hMmTask(NULL)
		, _winSupportAvrt(false)
	{

	}

	RenderAudioWindows::~RenderAudioWindows(void)
	{
		Release();
	}

	int32_t RenderAudioWindows::Init(void)
	{
		//test
		LOG_Print_SetLog(libThirdWrap_Audio, MLog_GetAllLevel());
		//test

		// Try to load the Avrt DLL
		if (!_avrtLibrary)
		{
			// Get handle to the Avrt DLL module.
			_avrtLibrary = LoadLibrary(TEXT("Avrt.dll"));
			if (_avrtLibrary)
			{
				// Handle is valid (should only happen if OS larger than vista & win7).
				// Try to get the function addresses.
				LOG_Print_Info(libThirdWrap_Audio, "RenderAudioWindows::RenderAudioWindows() The Avrt DLL module is now loaded");

				_PAvRevertMmThreadCharacteristics = (PAvRevertMmThreadCharacteristics)GetProcAddress(_avrtLibrary, "AvRevertMmThreadCharacteristics");
				_PAvSetMmThreadCharacteristicsA = (PAvSetMmThreadCharacteristicsA)GetProcAddress(_avrtLibrary, "AvSetMmThreadCharacteristicsA");
				_PAvSetMmThreadPriority = (PAvSetMmThreadPriority)GetProcAddress(_avrtLibrary, "AvSetMmThreadPriority");

				if (_PAvRevertMmThreadCharacteristics &&
					_PAvSetMmThreadCharacteristicsA &&
					_PAvSetMmThreadPriority)
				{
					LOG_Print_Info(libThirdWrap_Audio, "RenderAudioWindows::RenderAudioWindows() AvRevertMmThreadCharacteristics() is OK");
					LOG_Print_Info(libThirdWrap_Audio, "RenderAudioWindows::RenderAudioWindows() AvSetMmThreadCharacteristicsA() is OK");
					LOG_Print_Info(libThirdWrap_Audio, "RenderAudioWindows::RenderAudioWindows() AvSetMmThreadPriority() is OK");
					_winSupportAvrt = true;
				}
			}
		}

		if (_ptrEnumerator != NULL)
			return -1;

		// We know that this API will work since it has already been verified in
		// CoreAudioIsSupported, hence no need to check for errors here as well.

		// Retrive the IMMDeviceEnumerator API (should load the MMDevAPI.dll)
		// TODO(henrika): we should probably move this allocation to Init() instead
		// and deallocate in Terminate() to make the implementation more symmetric.
		HRESULT hr = CoCreateInstance(
			__uuidof(MMDeviceEnumerator),
			NULL,
			CLSCTX_ALL,
			__uuidof(IMMDeviceEnumerator),
			reinterpret_cast<void**>(&_ptrEnumerator));
		if (FAILED(hr) || NULL == _ptrEnumerator)
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::Init() MMDeviceEnumerator hr:%x\r\n", hr);
			return -1;
		}

		// DMO initialization for built-in WASAPI AEC.
		{
			hr = CoCreateInstance(CLSID_CWMAudioAEC,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IMediaObject,
				reinterpret_cast<void**>(&_ptrDmo));
			if (FAILED(hr) || _ptrDmo == NULL)
			{
				// Since we check that _dmo is non-NULL in EnableBuiltInAEC(), the
				// feature is prevented from being enabled.
				bool _builtInAecEnabled = false;
				LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::Init() CLSID_CWMAudioAEC hr:%x\r\n", hr);
				return -1;
			}
		}

		return 1;
	}

	void RenderAudioWindows::Release(void)
	{
		if (_hSamplesReadyEvent != NULL)
		{
			CloseHandle(_hSamplesReadyEvent);
			_hSamplesReadyEvent = NULL;
		}

		_Thread.join();

		SAFE_RELEASE(_ptrCollection);
		SAFE_RELEASE(_ptrDmo);
		SAFE_RELEASE(_ptrEnumerator);

		if (_avrtLibrary)
		{
			FreeLibrary(_avrtLibrary);
			_avrtLibrary = NULL;
			_PAvRevertMmThreadCharacteristics = NULL;
			_PAvSetMmThreadCharacteristicsA = NULL;
			_PAvSetMmThreadPriority = NULL;
		}
	}

	int32_t RenderAudioWindows::Enumerate(Device_INFO_List* pList)
	{
		return EnumerateDevices(_ptrEnumerator, eRender, pList);
	}

	Device_INFO_sptr RenderAudioWindows::GetDefaultDevice(void)
	{
		if (_ptrEnumerator == NULL)
			return Device_INFO_sptr();

		return GetDefaultDevice(_ptrEnumerator, eRender);
	}

	int32_t RenderAudioWindows::GetVolume(const Device_INFO_sptr& Device_sptr, uint32_t& Volume)
	{
		if (!Device_sptr)
			return -1;

		HRESULT hr = S_OK;
		IAudioEndpointVolume*	ptrVolume = NULL;
		hr = Device_sptr->pDevice->Activate(__uuidof(IAudioEndpointVolume),
			CLSCTX_ALL,
			NULL,
			reinterpret_cast<void **>(&ptrVolume));
		if (hr != 0 || ptrVolume == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::GetCaptureVolume() failed to initialize the capture volume, hr:%x", hr);
			return -1;
		}

		float fLevel(0.0f);
		Volume = 0;
		hr = ptrVolume->GetMasterVolumeLevelScalar(&fLevel);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::GetCaptureVolume() GetMasterVolumeLevelScalar Failed, hr:%x", hr);
			SAFE_RELEASE(ptrVolume);
			return -1;
		}

		// scale input volume range [0.0,1.0] to valid output range
		Volume = static_cast<uint32_t> (fLevel*MAX_CORE_MICROPHONE_VOLUME);

		SAFE_RELEASE(ptrVolume);
		return 1;
	}

	//Volume： 0.0f-255.0f
	int32_t RenderAudioWindows::SetVolume(const Device_INFO_sptr& Device_sptr, uint32_t Volume)
	{
		if (!Device_sptr)
			return -1;

		if (Volume < static_cast<uint32_t>(MIN_CORE_MICROPHONE_VOLUME) ||
			Volume > static_cast<uint32_t>(MAX_CORE_MICROPHONE_VOLUME))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::SetCaptureVolume() failed to initialize the set volume:%f", Volume);
			return -1;
		}

		IAudioEndpointVolume*	ptrVolume;
		HRESULT hr = Device_sptr->pDevice->Activate(__uuidof(IAudioEndpointVolume),
			CLSCTX_ALL,
			NULL,
			reinterpret_cast<void **>(&ptrVolume));
		if (hr != 0 || ptrVolume == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::SetCaptureVolume() failed to initialize the capture volume, hr:%x", hr);
			return -1;
		}

		// scale input volume to valid range (0.0 to 1.0)
		const float fLevel = static_cast<float>(Volume) / MAX_CORE_MICROPHONE_VOLUME;
		hr = ptrVolume->SetMasterVolumeLevelScalar(fLevel, NULL);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::SetCaptureVolume() SetMasterVolumeLevelScalar Failed, hr:%x", hr);
			SAFE_RELEASE(ptrVolume);
			return -1;
		}

		SAFE_RELEASE(ptrVolume);
		return 1;
	}

	//枚举音频设备
	int32_t RenderAudioWindows::EnumerateDevices(IMMDeviceEnumerator*	ptrEnumerator, EDataFlow Type, Device_INFO_List* pList)
	{
		if (Type > eAll)
			return -1;

		HRESULT hr = S_OK;

		IMMDeviceCollection* pCollection = NULL;

		if (Type == eRender)
		{
			if (_ptrCollection == NULL)
			{
				//刷新输出设备列表
				// Create a fresh list of devices using the specified direction
				hr = ptrEnumerator->EnumAudioEndpoints(
					Type,
					DEVICE_STATE_ACTIVE,
					&_ptrCollection);
				if (FAILED(hr))
				{
					LOG_Print_Error(libThirdWrap_Audio,
						"RenderAudioWindows::EnumerateAudioDevices() EnumAudioEndpoints() Failed, hr:%x!!", hr);
					return -1;
				}
			}

			pCollection = _ptrCollection;
		}
		else
		{
			return -1;
		}

		//获取设备数量
		UINT count = 0;
		pCollection->GetCount(&count);

		for (int i = 0; i < count; i++)
		{
			//获取设备名称
			IMMDevice*	ptrDeviceIn = NULL;
			hr = pCollection->Item(i, &ptrDeviceIn);
			if (FAILED(hr))
			{
				LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::EnumerateAudioDevices() MMDeviceCollection::Item() Failed, hr:%x", hr);
				continue;
			}

			Device_INFO_sptr sptr = RenderAudioWindows::Alloc();
			sptr->pDevice = ptrDeviceIn;
			sptr->Type = Type;
			sptr->uIndex = i;
			sptr->sName = GetDeviceName(sptr->pDevice);
			GetDeviceWAVEFORMATEX(sptr->pDevice, &sptr->Format);

			pList->push_back(sptr);
		}

		return 0;
	}

	_audio_string_t RenderAudioWindows::GetDeviceName(IMMDevice*	ptrDevice)
	{
		_audio_string_t	sName;

		//获取设备名称
		IPropertyStore *pProps = NULL;
		HRESULT hr = ptrDevice->OpenPropertyStore(STGM_READ, &pProps);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::GetDeviceName() IMMDevice::OpenPropertyStore failed, hr = 0x%08X", hr);
			goto gt_end;
		}

		// Initialize container for property value.
		PROPVARIANT varName;
		PropVariantInit(&varName);

		// Get the endpoint device's friendly-name property.
		hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::GetDeviceName() IPropertyStore::GetValue failed, hr = 0x%08X\r\n", hr);
			goto gt_end;
		}

		if (varName.vt == VT_LPWSTR)
		{
			sName = _audio_string_t((char*)varName.pwszVal, (wcslen(varName.pwszVal) + 1) * sizeof(WCHAR)).wchar2char().c_str();
		}
		else
		{
			sName = varName.pszVal;
		}

		PropVariantClear(&varName);

	gt_end:
		SAFE_RELEASE(pProps);

		return sName;
	}

	int32_t	RenderAudioWindows::GetDeviceWAVEFORMATEX(IMMDevice* ptrDevice, tWAVEFORMATEX* pOutWfx)
	{
		if (ptrDevice == NULL)
			return -1;

		IAudioClient*	ptrClient = NULL;
		HRESULT hr = ptrDevice->Activate(
			__uuidof(IAudioClient),
			CLSCTX_ALL,
			NULL,
			(void**)&ptrClient);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::GetDeviceWAVEFORMATEX() IPropertyStore::GetValue failed, hr = 0x%08X\r\n", hr);
			return -1;
		}

		WAVEFORMATEX* pWfxIn = NULL;
		hr = ptrClient->GetMixFormat(&pWfxIn);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::GetDeviceWAVEFORMATEX() IPropertyStore::GetValue failed, hr = 0x%08X\r\n", hr);
			goto gt_end;
		}

		memcpy(pOutWfx, pWfxIn, sizeof(WAVEFORMATEX));
		CoTaskMemFree(pWfxIn);

	gt_end:
		SAFE_RELEASE(ptrClient);
		return 1;
	}

	Device_INFO_sptr RenderAudioWindows::GetDefaultDevice(IMMDeviceEnumerator*	ptrEnumerator, EDataFlow Type)
	{
		Device_INFO_sptr sptr;
		IMMDevice*	ptrDevice = NULL;
		HRESULT hr = _ptrEnumerator->GetDefaultAudioEndpoint(
			Type,
			eConsole,
			&ptrDevice);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::GetDefaultDevice() GetDefaultAudioEndpoint Failed, hr = 0x%08X\r\n", hr);
		}
		else
		{
			sptr = RenderAudioWindows::Alloc();
			sptr->pDevice = ptrDevice;
			sptr->Type = Type;
			sptr->uIndex = 0;
			sptr->sName = GetDeviceName(sptr->pDevice);
			GetDeviceWAVEFORMATEX(sptr->pDevice, &sptr->Format);
		}

		return sptr;
	}

	int32_t	RenderAudioWindows::Start(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format,
		const HFNRender& hRender)
	{
		if (!Device_sptr)
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::Start() Not Found Microphone!!");
			return -1;
		}

		if (Device_sptr->pClient == NULL)
		{
			HRESULT hr = Device_sptr->pDevice->Activate(
				__uuidof(IAudioClient),
				CLSCTX_ALL,
				NULL,
				(void**)&Device_sptr->pClient);
			if (FAILED(hr))
			{
				LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::Start() AudioClient Faild!!");
				return -1;
			}
		}

		memcpy(&Device_sptr->Format, &Format, sizeof(tWAVEFORMATEX));

		if (_hSamplesReadyEvent == NULL)
		{
			_hSamplesReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (_hSamplesReadyEvent == NULL)
			{
				LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::Start() CreateEvent Failed!!");
				return -1;
			}
		}

		_Thread.exit();
		_Thread.run(function20_bind(&RenderAudioWindows::DoThread, this, Device_sptr, hRender));

		return 1;
	}

	int RenderAudioWindows::Play(const Device_INFO_sptr& Device_sptr, const char* c_szData, UInt32 uSize)
	{
		if (!Device_sptr)
			return -1;

		_Datas.append(c_szData, uSize);
		UINT32 _recBlockSize = Device_sptr->Format.nSamplesPerSec / 100 * Device_sptr->Format.nBlockAlign;

		UInt32 uRemain = _Datas.size();
		while (uRemain >= _recBlockSize)
		{
			_Lock.Lock();
			_DataList.push_back(_audio_string_t(&_Datas[_Datas.size() - uRemain], _recBlockSize));
			_Lock.UnLock();

			uRemain -= _recBlockSize;
		};

		if (uRemain > 0)
			std::memmove(&_Datas[0], &_Datas[_Datas.size() - uRemain], uRemain);

		_Datas.update_size(uRemain);
		return 1;
	}

	IAudioRenderClient* RenderAudioWindows::GetClient(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format,
		HANDLE hSamplesReadyEvent)
	{
		if (!Device_sptr)
			return NULL;

		if (Device_sptr->pClient == NULL)
		{
			HRESULT hr = Device_sptr->pDevice->Activate(
				__uuidof(IAudioClient),
				CLSCTX_ALL,
				NULL,
				(void**)&Device_sptr->pClient);
			if (FAILED(hr) || Device_sptr->pClient == NULL)
			{
				LOG_Print_Error(libThirdWrap_Audio,
					"RenderAudioWindows::GetCaptureClient() Activate() Failed, hr:%x!!", hr);
				return NULL;
			}
		}

		WAVEFORMATEX*	pWfxClosestMatch = NULL;

		HRESULT hr = Device_sptr->pClient->IsFormatSupported(
			AUDCLNT_SHAREMODE_SHARED,
			&Format,
			&pWfxClosestMatch);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::GetCaptureClient() IsFormatSupported nChannels=%d, nSamplesPerSec=%d is not supported!!",
				Format.nChannels, Format.nSamplesPerSec);
			return NULL;
		}

		// Create a rendering stream.
		//
		// ****************************************************************************
		// For a shared-mode stream that uses event-driven buffering, the caller must
		// set both hnsPeriodicity and hnsBufferDuration to 0. The Initialize method
		// determines how large a buffer to allocate based on the scheduling period
		// of the audio engine. Although the client's buffer processing thread is
		// event driven, the basic buffer management process, as described previously,
		// is unaltered.
		// Each time the thread awakens, it should call IAudioClient::GetCurrentPadding
		// to determine how much data to write to a rendering buffer or read from a capture
		// buffer. In contrast to the two buffers that the Initialize method allocates
		// for an exclusive-mode stream that uses event-driven buffering, a shared-mode
		// stream requires a single buffer.
		// ****************************************************************************
		//
		REFERENCE_TIME hnsBufferDuration = 0;  // ask for minimum buffer size (default)
		if (Format.nSamplesPerSec == 44100)
		{
			// Ask for a larger buffer size (30ms) when using 44.1kHz as render rate.
			// There seems to be a larger risk of underruns for 44.1 compared
			// with the default rate (48kHz). When using default, we set the requested
			// buffer duration to 0, which sets the buffer to the minimum size
			// required by the engine thread. The actual buffer size can then be
			// read by GetBufferSize() and it is 20ms on most machines.
			hnsBufferDuration = 30 * 10000;
		}

		// Create a capturing stream.
		hr = Device_sptr->pClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,             // share Audio Engine with other applications
			AUDCLNT_STREAMFLAGS_EVENTCALLBACK |   // processing of the audio buffer by the client will be event driven
			AUDCLNT_STREAMFLAGS_NOPERSIST,        // volume and mute settings for an audio session will not persist across system restarts
			hnsBufferDuration,                    // required for event-driven shared mode
			0,                                    // periodicity
			&Format,                              // selected wave format
			NULL);                                // session GUID

		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::GetCaptureClient() Initialize() nChannels=%d, nSamplesPerSec=%d is not supported!!",
				Format.nChannels, Format.nSamplesPerSec);
			return NULL;
		}

		UINT bufferFrameCount(0);
		hr = Device_sptr->pClient->GetBufferSize(&bufferFrameCount);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::GetCaptureClient() GetBufferSize() Failed, hr:%d!!", hr);
			return NULL;
		}

		// Set the event handle that the system signals when an audio buffer is ready
		// to be processed by the client.
		hr = Device_sptr->pClient->SetEventHandle(hSamplesReadyEvent);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::GetCaptureClient() SetEventHandle() Failed, hr:%d!!", hr);
			return NULL;
		}

		IAudioRenderClient* ptrClient = NULL;

		hr = Device_sptr->pClient->GetService(
			__uuidof(IAudioRenderClient),
			(void**)&ptrClient);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::GetCaptureClient() GetService() Failed, hr:%d!!", hr);
			return NULL;
		}

		CoTaskMemFree(pWfxClosestMatch);

		return ptrClient;
	}

	DWORD RenderAudioWindows::InitThreadPriority(void)
	{
		_hMmTask = NULL;

		// Use Multimedia Class Scheduler Service (MMCSS) to boost the thread
		// priority.
		if (_winSupportAvrt)
		{
			DWORD taskIndex(0);
			_hMmTask = _PAvSetMmThreadCharacteristicsA("Pro Audio", &taskIndex);
			if (_hMmTask)
			{
				if (!_PAvSetMmThreadPriority(_hMmTask, AVRT_PRIORITY_CRITICAL))
				{
					LOG_Print_Error(libThirdWrap_Audio, "failed to boost rec-thread using MMCSS");
				}
				LOG_Print_Info(libThirdWrap_Audio,
					"capture thread is now registered with MMCSS (taskIndex=%d)",
					taskIndex);
			}
			else
			{
				LOG_Print_Error(libThirdWrap_Audio, "failed to enable MMCSS on capture thread (err=%d)",
					GetLastError());
			}
		}

		return S_OK;
	}

	void RenderAudioWindows::RevertThreadPriority(void)
	{
		if (_winSupportAvrt)
		{
			if (NULL != _hMmTask)
			{
				if(_PAvRevertMmThreadCharacteristics != NULL)
					_PAvRevertMmThreadCharacteristics(_hMmTask);

				_hMmTask = NULL;
			}
		}
	}

	int32_t	RenderAudioWindows::Stop(void)
	{
		if (_hSamplesReadyEvent != NULL)
		{
			CloseHandle(_hSamplesReadyEvent);
			_hSamplesReadyEvent = NULL;

			_Lock.Lock();
			_DataList.clear();
			_Lock.UnLock();
			
		}

		return 1;
	}

	void RenderAudioWindows::DoThread(const Device_INFO_sptr& sptr, const HFNRender& hRender)
	{
		IAudioRenderClient* ptrRenderClient = GetClient(sptr, sptr->Format, _hSamplesReadyEvent);
		if (ptrRenderClient == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() Get Capture Client Failed!!");
			return ;
		}

		LARGE_INTEGER	perfCounterFreq;
		if (QueryPerformanceFrequency(&perfCounterFreq) == 0)
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"RenderAudioWindows::DoCaptureThread() QueryPerformanceFrequency Failed, error:%d!!",
				GetLastError());
			return ;
		}

		double _perfCounterFactor = 10000000.0 / (double)perfCounterFreq.QuadPart;
		UINT _recAudioFrameSize = sptr->Format.nBlockAlign;
		UINT32 _recBlockSize = sptr->Format.nSamplesPerSec / 100;
		UINT64	_readSamples = 0;

		LARGE_INTEGER t1;
		LARGE_INTEGER t2;

		printf("[WAVEFORMATEX] AvgBytesPerSec : %d\r\n", sptr->Format.nAvgBytesPerSec);
		printf("[WAVEFORMATEX] BlockAlign : %d\r\n", sptr->Format.nBlockAlign);
		printf("[WAVEFORMATEX] Channels : %d\r\n", sptr->Format.nChannels);
		printf("[WAVEFORMATEX] SamplesPerSec : %d\r\n", sptr->Format.nSamplesPerSec);
		printf("[WAVEFORMATEX] BitsPerSample : %d\r\n", sptr->Format.wBitsPerSample);
		printf("[WAVEFORMATEX] FormatTag : %d\r\n", sptr->Format.wFormatTag);

		//开启 MMCSS服务
		HRESULT hr = InitThreadPriority();
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() Open MMCSS Service Failed, hr:%x!!", hr);
			goto gt_end;
		}

		IAudioClock* clock = NULL;

		UINT32 bufferLength = 0;
		hr = sptr->pClient->GetBufferSize(&bufferLength);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() GetBufferSize() Failed!!");
			goto gt_end;
		}

		// Get maximum latency for the current stream (will not change for the lifetime of the IAudioClient object).
		//
		REFERENCE_TIME latency = 0;
		sptr->pClient->GetStreamLatency(&latency);
		printf("[REND] max stream latency   : %u (%3.2f ms)\r\n",
			(DWORD)latency, (double)(latency / 10000.0));

		// Get the length of the periodic interval separating successive processing passes by
		// the audio engine on the data in the endpoint buffer.
		//
		REFERENCE_TIME devPeriod = 0;
		REFERENCE_TIME devPeriodMin = 0;
		sptr->pClient->GetDevicePeriod(&devPeriod, &devPeriodMin);
		printf("[REND] device period        : %u (%3.2f ms)\r\n", (DWORD)devPeriod, (double)(devPeriod / 10000.0));

		UINT64 writtenSamples = 0;
		LONGLONG	playAcc = 0;
		int32_t time = 0;

		// Derive initial rendering delay.
		// Example: 10*(960/480) + 15 = 20 + 15 = 35ms
		//
		int playout_delay = 10 * (bufferLength / _recBlockSize) +
			(int)((latency + devPeriod) / 10000);

		writtenSamples = 0;
		printf("[REND] initial delay        : %u", playout_delay);

		double endpointBufferSizeMS = 10.0 * ((double)bufferLength / (double)_recBlockSize);
		printf("[REND] endpointBufferSizeMS : %3.2f", endpointBufferSizeMS);


		// Before starting the stream, fill the rendering buffer with silence.
		//
		BYTE *pData = NULL;
		hr = ptrRenderClient->GetBuffer(bufferLength, &pData);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() RenderClient GetBuffer() Failed!!");
			goto gt_end;
		}

		hr = ptrRenderClient->ReleaseBuffer(bufferLength, AUDCLNT_BUFFERFLAGS_SILENT);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() RenderClient ReleaseBuffer() Failed!!");
			goto gt_end;
		}

		writtenSamples += bufferLength;

		hr = sptr->pClient->GetService(__uuidof(IAudioClock), (void**)&clock);

		if (FAILED(hr)) 
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() failed to get IAudioClock interface from the IAudioClient!!");
			goto gt_end;
		}

		// Start up the capturing stream.
		//
		hr = sptr->pClient->Start();
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() AudioCapture Start() Failed!!");
			goto gt_end;
		}

		while (true)
		{
			//信号可以保证线程睡眠，降低cpu消耗
			DWORD waitResult = WaitForSingleObject(_hSamplesReadyEvent, INFINITE);
			if (waitResult == WAIT_OBJECT_0)
			{
				BYTE *pData = 0;
				DWORD flags = 0;
				UINT64 recTime = 0;
				UINT64 recPos = 0;
				UINT32 syncBufIndex = 0;

				// Get the number of frames of padding (queued up to play) in the endpoint buffer.
				UINT32 padding = 0;
				hr = sptr->pClient->GetCurrentPadding(&padding);
				if (FAILED(hr))
				{
					LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() GetCurrentPadding Failed!!");
					goto gt_end;
				}

				// Derive the amount of available space in the output buffer
				uint32_t framesAvailable = bufferLength - padding;
				// WEBRTC_TRACE(kTraceStream, kTraceAudioDevice, _id, "#avaliable audio frames = %u", framesAvailable);

				// Do we have 10 ms available in the render buffer?
				if (framesAvailable < _recBlockSize || _DataList.size() <= 0)
				{
					// Not enough space in render buffer to store next render packet.
					continue;
				}

				// Write n*10ms buffers to the render buffer
				UInt32 uLength = _recBlockSize * _recAudioFrameSize;
				const uint32_t n10msBuffers = (framesAvailable / _recBlockSize);
				for (uint32_t n = 0; n < n10msBuffers; n++)
				{
					// Get pointer (i.e., grab the buffer) to next space in the shared render buffer.
					hr = ptrRenderClient->GetBuffer(_recBlockSize, &pData);
					if (FAILED(hr))
					{
						LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() Render Client GetBuffer Failed!!");
						goto gt_end;
					}

					QueryPerformanceCounter(&t1);    // measure time: START

					_audio_string_t sData;
					//Get Play Data
					_Lock.Lock();
					sData = _DataList.pop_front();
					_Lock.UnLock();

					if(sData.size() > 0)
						memcpy(pData, sData.c_str(), uLength);
					//Get Play Data

					QueryPerformanceCounter(&t2);    // measure time: STOP
					time = (int)(t2.QuadPart - t1.QuadPart);
					playAcc += time;

					DWORD dwFlags(0);
					hr = ptrRenderClient->ReleaseBuffer(_recBlockSize, dwFlags);
					if (FAILED(hr))
					{
						// See http://msdn.microsoft.com/en-us/library/dd316605(VS.85).aspx
						// for more details regarding AUDCLNT_E_DEVICE_INVALIDATED.
						LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::DoThread() Render Client ReleaseBuffer Failed!!");
						goto gt_end;
					}

					writtenSamples += _recBlockSize;
				}

				// Check the current delay on the playout side.
				if (clock) 
				{
					UINT64 pos = 0;
					UINT64 freq = 1;
					clock->GetPosition(&pos, NULL);
					clock->GetFrequency(&freq);
					playout_delay = ROUND((double(writtenSamples) /
						sptr->Format.nSamplesPerSec - double(pos) / freq) * 1000.0);
				}
			}
			else if(waitResult == WAIT_FAILED)
			{
				break;
			}
		}

gt_end:
		RevertThreadPriority();
		SAFE_RELEASE(clock);
		sptr->pClient->Stop();
		sptr->pClient->Reset();
		SAFE_RELEASE(ptrRenderClient);
		SAFE_RELEASE(sptr->pClient);

		LOG_Print_Info(libThirdWrap_Audio, "RenderAudioWindows::DoThread() Exit");
	}

	bool RenderAudioWindows::IsFormatSupported(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format)
	{
		bool isRet = true;
		IAudioClient* ptrClient = NULL;
		if (!Device_sptr)
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::StartCapture() Not Found Microphone!!");
			return false;
		}

		HRESULT hr = Device_sptr->pDevice->Activate(
			__uuidof(IAudioClient),
			CLSCTX_ALL,
			NULL,
			(void**)&ptrClient);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "RenderAudioWindows::IsCaptureFormatSupported() AudioClient Faild!!");
			return false;
		}

		WAVEFORMATEX*	pWfxClosestMatch = NULL;

		hr = ptrClient->IsFormatSupported(
			AUDCLNT_SHAREMODE_SHARED,
			&Format,
			&pWfxClosestMatch);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "nChannels=%d, nSamplesPerSec=%d is not supported",
				Format.nChannels, Format.nSamplesPerSec);

			isRet = false;
			goto gt_end;
		}

		// Create a rendering stream.
		//
		// ****************************************************************************
		// For a shared-mode stream that uses event-driven buffering, the caller must
		// set both hnsPeriodicity and hnsBufferDuration to 0. The Initialize method
		// determines how large a buffer to allocate based on the scheduling period
		// of the audio engine. Although the client's buffer processing thread is
		// event driven, the basic buffer management process, as described previously,
		// is unaltered.
		// Each time the thread awakens, it should call IAudioClient::GetCurrentPadding
		// to determine how much data to write to a rendering buffer or read from a capture
		// buffer. In contrast to the two buffers that the Initialize method allocates
		// for an exclusive-mode stream that uses event-driven buffering, a shared-mode
		// stream requires a single buffer.
		// ****************************************************************************
		//
		REFERENCE_TIME hnsBufferDuration = 0;  // ask for minimum buffer size (default)
		if (Format.nSamplesPerSec == 44100)
		{
			// Ask for a larger buffer size (30ms) when using 44.1kHz as render rate.
			// There seems to be a larger risk of underruns for 44.1 compared
			// with the default rate (48kHz). When using default, we set the requested
			// buffer duration to 0, which sets the buffer to the minimum size
			// required by the engine thread. The actual buffer size can then be
			// read by GetBufferSize() and it is 20ms on most machines.
			hnsBufferDuration = 30 * 10000;
		}

		// Create a capturing stream.
		hr = ptrClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,             // share Audio Engine with other applications
			AUDCLNT_STREAMFLAGS_EVENTCALLBACK |   // processing of the audio buffer by the client will be event driven
			AUDCLNT_STREAMFLAGS_NOPERSIST,        // volume and mute settings for an audio session will not persist across system restarts
			hnsBufferDuration,                    // required for event-driven shared mode
			0,                                    // periodicity
			&Format,                              // selected wave format
			NULL);                                // session GUID

		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "Initialize() nChannels=%d, nSamplesPerSec=%d is not supported",
				Format.nChannels, Format.nSamplesPerSec);

			isRet = false;
			goto gt_end;
		}

		//test
		LOG_Print_Info(libThirdWrap_Audio, "Initialize() supported -----  nChannels=%d, nSamplesPerSec=%d",
			Format.nChannels, Format.nSamplesPerSec);
		//test

gt_end:
		CoTaskMemFree(pWfxClosestMatch);
		SAFE_RELEASE(ptrClient);

		return isRet;
	}
}
