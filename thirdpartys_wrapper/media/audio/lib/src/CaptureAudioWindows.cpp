
#include <libThirdWrap_Audio/CaptureAudioWindows.h>
#include <endpointvolume.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <dmo.h>
#include <uuids.h>
#include <atlstr.h>

namespace _audio_ {

	const float MAX_CORE_SPEAKER_VOLUME = 255.0f;
	const float MIN_CORE_SPEAKER_VOLUME = 0.0f;
	const float MAX_CORE_MICROPHONE_VOLUME = 255.0f;
	const float MIN_CORE_MICROPHONE_VOLUME = 0.0f;

	void _TraceCOMError(HRESULT hr)
	{
		TCHAR buf[MAXERRORLENGTH];
		TCHAR errorText[MAXERRORLENGTH];

		const DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS;
		const DWORD dwLangID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

		// Gets the system's human readable message string for this HRESULT.
		// All error message in English by default.
		DWORD messageLength = ::FormatMessageA(dwFlags,
			0,
			hr,
			dwLangID,
			errorText,
			MAXERRORLENGTH,
			NULL);

		// Trims tailing white space (FormatMessage() leaves a trailing cr-lf.).
		for (; messageLength && ::isspace(errorText[messageLength - 1]);
			--messageLength)
		{
			errorText[messageLength - 1] = '\0';
		}
	}

	MemPool_type* CaptureAudioWindows::GetPoolInstance(void)
	{
		static MemPool_type	s_Pool(sizeof(tagDevice_INFO));
		return &s_Pool;
	}

	Device_INFO_sptr CaptureAudioWindows::Alloc(void)
	{
		tagDevice_INFO* pInfo = GetPoolInstance()->AllocObj<tagDevice_INFO>();
		pInfo->pDevice = NULL;
		pInfo->pClient = NULL;
		pInfo->uIndex = 0;
		pInfo->isCaptureInit = false;
		return Device_INFO_sptr(pInfo,
			function20_bind(&CaptureAudioWindows::Free, pInfo));
	}

	void CaptureAudioWindows::Free(tagDevice_INFO* pInfo)
	{
		SAFE_RELEASE(pInfo->pClient);
		SAFE_RELEASE(pInfo->pDevice);
		GetPoolInstance()->FreeObj(pInfo);
	}


	CaptureAudioWindows::CaptureAudioWindows(void)
		: _ptrEnumerator(NULL)
		, _ptrDmo(NULL)
		, _ptrDmoPS(NULL)
		, _ptrCollection(NULL)
		, _hSamplesReadyEvent(NULL)
		, _ComInit(ScopedCOMInitializer::kMTA)
		, _avrtLibrary(NULL)
		, _hMmTask(NULL)
		, _winSupportAvrt(false)
		, _builtInAecEnabled(false)
	{

	}

	CaptureAudioWindows::~CaptureAudioWindows(void)
	{
		Release();
	}

	int32_t CaptureAudioWindows::Init(void)
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
				LOG_Print_Info(libThirdWrap_Audio, "CaptureAudioWindows::CaptureAudioWindows() The Avrt DLL module is now loaded");

				_PAvRevertMmThreadCharacteristics = (PAvRevertMmThreadCharacteristics)GetProcAddress(_avrtLibrary, "AvRevertMmThreadCharacteristics");
				_PAvSetMmThreadCharacteristicsA = (PAvSetMmThreadCharacteristicsA)GetProcAddress(_avrtLibrary, "AvSetMmThreadCharacteristicsA");
				_PAvSetMmThreadPriority = (PAvSetMmThreadPriority)GetProcAddress(_avrtLibrary, "AvSetMmThreadPriority");

				if (_PAvRevertMmThreadCharacteristics &&
					_PAvSetMmThreadCharacteristicsA &&
					_PAvSetMmThreadPriority)
				{
					LOG_Print_Info(libThirdWrap_Audio, "CaptureAudioWindows::CaptureAudioWindows() AvRevertMmThreadCharacteristics() is OK");
					LOG_Print_Info(libThirdWrap_Audio, "CaptureAudioWindows::CaptureAudioWindows() AvSetMmThreadCharacteristicsA() is OK");
					LOG_Print_Info(libThirdWrap_Audio, "CaptureAudioWindows::CaptureAudioWindows() AvSetMmThreadPriority() is OK");
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
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::Init() MMDeviceEnumerator hr:%x\r\n", hr);
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
				LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::Init() CLSID_CWMAudioAEC hr:%x\r\n", hr);
				return -1;
			}
		}

		_builtInAecEnabled = false;

		return 1;
	}

	void CaptureAudioWindows::Release(void)
	{
		if (_hSamplesReadyEvent != NULL)
		{
			CloseHandle(_hSamplesReadyEvent);
			_hSamplesReadyEvent = NULL;
		}

		_Thread.join();

		SAFE_RELEASE(_ptrCollection);
		SAFE_RELEASE(_ptrDmoPS);
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

	int32_t CaptureAudioWindows::Enumerate(Device_INFO_List* pList)
	{
		return CaptureAudioWindows::EnumerateDevices(_ptrEnumerator, eCapture, pList);
	}

	Device_INFO_sptr CaptureAudioWindows::GetDefaultDevice(void)
	{
		if (_ptrEnumerator == NULL)
			return Device_INFO_sptr();

		return GetDefaultDevice(_ptrEnumerator, eCapture);
	}

	int32_t CaptureAudioWindows::GetVolume(const Device_INFO_sptr& Device_sptr, uint32_t& Volume)
	{
		if (!Device_sptr)
			return -1;

		HRESULT hr = S_OK;
		IAudioEndpointVolume*	ptrVolume;
		hr = Device_sptr->pDevice->Activate(__uuidof(IAudioEndpointVolume),
			CLSCTX_ALL,
			NULL,
			reinterpret_cast<void **>(&ptrVolume));
		if (hr != 0 || ptrVolume == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetCaptureVolume() failed to initialize the capture volume, hr:%x", hr);
			return -1;
		}

		float fLevel(0.0f);
		Volume = 0;
		hr = ptrVolume->GetMasterVolumeLevelScalar(&fLevel);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetCaptureVolume() GetMasterVolumeLevelScalar Failed, hr:%x", hr);
			SAFE_RELEASE(ptrVolume);
			return -1;
		}

		// scale input volume range [0.0,1.0] to valid output range
		Volume = static_cast<uint32_t> (fLevel*MAX_CORE_MICROPHONE_VOLUME);

		SAFE_RELEASE(ptrVolume);
		return 1;
	}

	//Volume： 0.0f-255.0f
	int32_t CaptureAudioWindows::SetVolume(const Device_INFO_sptr& Device_sptr, uint32_t Volume)
	{
		if (!Device_sptr)
			return -1;

		if (Volume < static_cast<uint32_t>(MIN_CORE_MICROPHONE_VOLUME) ||
			Volume > static_cast<uint32_t>(MAX_CORE_MICROPHONE_VOLUME))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetCaptureVolume() failed to initialize the set volume:%f", Volume);
			return -1;
		}

		IAudioEndpointVolume*	ptrVolume;
		HRESULT hr = Device_sptr->pDevice->Activate(__uuidof(IAudioEndpointVolume),
			CLSCTX_ALL,
			NULL,
			reinterpret_cast<void **>(&ptrVolume));
		if (hr != 0 || ptrVolume == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetCaptureVolume() failed to initialize the capture volume, hr:%x", hr);
			return -1;
		}

		// scale input volume to valid range (0.0 to 1.0)
		const float fLevel = static_cast<float>(Volume) / MAX_CORE_MICROPHONE_VOLUME;
		hr = ptrVolume->SetMasterVolumeLevelScalar(fLevel, NULL);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetCaptureVolume() SetMasterVolumeLevelScalar Failed, hr:%x", hr);
			SAFE_RELEASE(ptrVolume);
			return -1;
		}

		SAFE_RELEASE(ptrVolume);
		return 1;
	}

	//枚举音频设备
	int32_t CaptureAudioWindows::EnumerateDevices(IMMDeviceEnumerator*	ptrEnumerator, EDataFlow Type, Device_INFO_List* pList)
	{
		if (Type > eAll)
			return -1;

		HRESULT hr = S_OK;

		IMMDeviceCollection* pCollection = NULL;

		if (Type == eCapture)
		{
			SAFE_RELEASE(_ptrCollection);

			if (_ptrCollection == NULL)
			{
				//刷新输入设备列表
				// Create a fresh list of devices using the specified direction
				hr = ptrEnumerator->EnumAudioEndpoints(
					Type,
					DEVICE_STATE_ACTIVE,
					&_ptrCollection);
				if (FAILED(hr))
				{
					LOG_Print_Error(libThirdWrap_Audio,
						"CaptureAudioWindows::EnumerateAudioDevices() EnumAudioEndpoints() Failed, hr:%x!!", hr);
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
				LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::EnumerateAudioDevices() MMDeviceCollection::Item() Failed, hr:%x", hr);
				continue;
			}

			Device_INFO_sptr sptr = CaptureAudioWindows::Alloc();
			sptr->pDevice = ptrDeviceIn;
			sptr->Type = Type;
			sptr->uIndex = i;
			sptr->sDeviceID = GetDeviceID(sptr->pDevice);
			sptr->sName = GetDeviceName(sptr->pDevice);
			GetDeviceWAVEFORMATEX(sptr->pDevice, &sptr->Format);

			pList->push_back(sptr);
		}

		return 0;
	}

	_audio_string_t CaptureAudioWindows::GetDeviceID(IMMDevice*	ptrDevice)
	{
		if (ptrDevice == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetDeviceID() Device Object is Null");
			return "";
		}

		LPWSTR pwszID = NULL;
		HRESULT hr = ptrDevice->GetId(&pwszID);
		if (hr != S_OK)
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetDeviceID() IMMDevice::GetId failed, hr = 0x%08X", hr);
			return "";
		}

		return _audio_string_t((char*)pwszID, (wcslen(pwszID) + 1) * sizeof(WCHAR)).wchar2char().c_str();
	}

	_audio_string_t CaptureAudioWindows::GetDeviceName(IMMDevice*	ptrDevice)
	{
		_audio_string_t	sName;

		//获取设备名称
		IPropertyStore *pProps = NULL;
		HRESULT hr = ptrDevice->OpenPropertyStore(STGM_READ, &pProps);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetDeviceName() IMMDevice::OpenPropertyStore failed, hr = 0x%08X", hr);
			goto gt_end;
		}

		// Initialize container for property value.
		PROPVARIANT varName;
		PropVariantInit(&varName);

		// Get the endpoint device's friendly-name property.
		hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetDeviceName() IPropertyStore::GetValue failed, hr = 0x%08X\r\n", hr);
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

	int32_t	CaptureAudioWindows::GetDeviceWAVEFORMATEX(IMMDevice* ptrDevice, tWAVEFORMATEX* pOutWfx)
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
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetDeviceWAVEFORMATEX() IPropertyStore::GetValue failed, hr = 0x%08X\r\n", hr);
			return -1;
		}

		WAVEFORMATEX* pWfxIn = NULL;
		hr = ptrClient->GetMixFormat(&pWfxIn);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetDeviceWAVEFORMATEX() IPropertyStore::GetValue failed, hr = 0x%08X\r\n", hr);
			goto gt_end;
		}

		memcpy(pOutWfx, pWfxIn, sizeof(WAVEFORMATEX));
		CoTaskMemFree(pWfxIn);

	gt_end:
		SAFE_RELEASE(ptrClient);
		return 1;
	}

	Device_INFO_sptr CaptureAudioWindows::GetDefaultDevice(IMMDeviceEnumerator*	ptrEnumerator, EDataFlow Type)
	{
		Device_INFO_sptr sptr;
		IMMDevice*	ptrDevice = NULL;
		HRESULT hr = _ptrEnumerator->GetDefaultAudioEndpoint(
			Type,
			eConsole,
			&ptrDevice);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetDefaultDevice() GetDefaultAudioEndpoint Failed, hr = 0x%08X\r\n", hr);
		}
		else
		{
			sptr = CaptureAudioWindows::Alloc();
			sptr->pDevice = ptrDevice;
			sptr->Type = Type;
			sptr->uIndex = 0;
			sptr->sName = GetDeviceName(sptr->pDevice);
			GetDeviceWAVEFORMATEX(sptr->pDevice, &sptr->Format);
		}

		return sptr;
	}

	int32_t	CaptureAudioWindows::Start(const Device_INFO_sptr& Device_sptr,
		const tWAVEFORMATEX& Format, const HFNCapture& hCapture)
	{
		if (!Device_sptr)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::Start() Not Found Microphone!!");
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
				LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::StartCapture() AudioClient Faild!!");
				return -1;
			}
		}

		memcpy(&Device_sptr->Format, &Format, sizeof(tWAVEFORMATEX));

		if (_builtInAecEnabled)
		{//开启AEC
			if (SetDMOProperties(const_cast<Device_INFO_sptr&>(Device_sptr)) == -1)
			{
				LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::StartCapture() SetDMOProperties Faild!!");
				return -1;
			}
		}

		if (_hSamplesReadyEvent == NULL)
		{
			_hSamplesReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (_hSamplesReadyEvent == NULL)
			{
				LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::GetDefaultDevice() CreateEvent Failed!!");
				return -1;
			}
		}

		_Thread.exit();
		if(_builtInAecEnabled)
			_Thread.run(function20_bind(&CaptureAudioWindows::DoThreadDMO, this, Device_sptr, hCapture));
		else
			_Thread.run(function20_bind(&CaptureAudioWindows::DoThread, this, Device_sptr, hCapture));
		_Thread.wait_run();

		return 1;
	}

	void CaptureAudioWindows::DoThread(const Device_INFO_sptr& sptr, const HFNCapture& hCapture)
	{
		IAudioCaptureClient* ptrCaptureClient = GetClient(sptr, sptr->Format, _hSamplesReadyEvent);
		if (ptrCaptureClient == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoCaptureThread() Get Capture Client Failed!!");
			return;
		}

		LARGE_INTEGER	perfCounterFreq;
		if (QueryPerformanceFrequency(&perfCounterFreq) == 0)
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::DoCaptureThread() QueryPerformanceFrequency Failed, error:%d!!",
				GetLastError());
			return;
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
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoCaptureThread() Open MMCSS Service Failed, hr:%x!!", hr);
			goto gt_end;
		}

		UINT32 bufferLength = 0;
		hr = sptr->pClient->GetBufferSize(&bufferLength);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoCaptureThread() GetBufferSize() Failed!!");
			goto gt_end;
		}

		/*const UINT32 syncBufferSize = 2 * (bufferLength * _recAudioFrameSize);
		_syncBuffer.reserve(syncBufferSize + 10);*/

		// Get maximum latency for the current stream (will not change for the lifetime of the IAudioClient object).
		//
		REFERENCE_TIME latency;
		sptr->pClient->GetStreamLatency(&latency);
		printf("[CAPT] max stream latency   : %u (%3.2f ms)\r\n",
			(DWORD)latency, (double)(latency / 10000.0));

		// Get the length of the periodic interval separating successive processing passes by
		// the audio engine on the data in the endpoint buffer.
		//
		REFERENCE_TIME devPeriod = 0;
		REFERENCE_TIME devPeriodMin = 0;
		sptr->pClient->GetDevicePeriod(&devPeriod, &devPeriodMin);
		printf("[CAPT] device period        : %u (%3.2f ms)\r\n", (DWORD)devPeriod, (double)(devPeriod / 10000.0));

		double extraDelayMS = (double)((latency + devPeriod) / 10000.0);
		printf("[CAPT] extraDelayMS         : %3.2f\r\n", extraDelayMS);

		double endpointBufferSizeMS = 10.0 * ((double)bufferLength / (double)_recBlockSize);
		printf("[CAPT] endpointBufferSizeMS : %3.2f\r\n", endpointBufferSizeMS);

		// Start up the capturing stream.
		//
		hr = sptr->pClient->Start();
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoCaptureThread() AudioCapture Start() Failed!!");
			goto gt_end;
		}

		while (true)
		{
			//信号可以保证线程睡眠，降低cpu消耗
			DWORD waitResult = WaitForSingleObject(_hSamplesReadyEvent, INFINITE);
			if (waitResult == WAIT_OBJECT_0)
			{
				BYTE *pData = 0;
				UINT32 framesAvailable = 0;
				DWORD flags = 0;
				UINT64 recTime = 0;
				UINT64 recPos = 0;
				UINT32 syncBufIndex = 0;

				//  Find out how much capture data is available
				//
				hr = ptrCaptureClient->GetBuffer(&pData,           // packet which is ready to be read by used
					&framesAvailable, // #frames in the captured packet (can be zero)
					&flags,           // support flags (check)
					&recPos,          // device position of first audio frame in data packet
					&recTime);        // value of performance counter at the time of recording the first audio frame
				if (FAILED(hr))
				{//失败
					LOG_Print_Error(libThirdWrap_Audio,
						"CaptureAudioWindows::DoCaptureThread() CaptureMicrophone GetBuffer Failed:%x", hr);
				}
				else
				{
					if (framesAvailable != 0)
					{
						if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
						{//静音
							printf("静音\r\n");
						}
						else
						{
							if (pData != NULL)
							{
								if (hCapture)
									hCapture(sptr, pData, framesAvailable*_recAudioFrameSize);

								//CopyMemory(&_syncBuffer[syncBufIndex*_recAudioFrameSize], pData, framesAvailable*_recAudioFrameSize);
							}
							/*else
							{
							ZeroMemory(&_syncBuffer[syncBufIndex*_recAudioFrameSize], framesAvailable*_recAudioFrameSize);
							}

							if (syncBufferSize < (syncBufIndex*_recAudioFrameSize) + framesAvailable*_recAudioFrameSize)
							{
							LOG_Print_Error(libThirdWrap_Audio,
							"CaptureAudioWindows::DoCaptureThread(%d,%d) Capture Error!",
							syncBufferSize, (syncBufIndex*_recAudioFrameSize) + framesAvailable*_recAudioFrameSize);
							break;
							}*/

							// Release the capture buffer
							//
							hr = ptrCaptureClient->ReleaseBuffer(framesAvailable);
							if (FAILED(hr))
							{//失败
								LOG_Print_Error(libThirdWrap_Audio,
									"CaptureAudioWindows::DoCaptureThread() CaptureMicrophone ReleaseBuffer Failed:%x\r\n", hr);
							}

							/*_readSamples += framesAvailable;
							syncBufIndex += framesAvailable;

							QueryPerformanceCounter(&t1);

							// Get the current recording and playout delay.
							uint32_t sndCardRecDelay = (uint32_t)
							(((((UINT64)t1.QuadPart * _perfCounterFactor) - recTime)
							/ 10000) + (10 * syncBufIndex) / _recBlockSize - 10);

							while (syncBufIndex >= _recBlockSize)
							{
							// store remaining data which was not able to deliver as 10ms segment
							MoveMemory(&_syncBuffer[0], &_syncBuffer[_recBlockSize*_recAudioFrameSize], (syncBufIndex - _recBlockSize)*_recAudioFrameSize);
							syncBufIndex -= _recBlockSize;
							sndCardRecDelay -= 10;
							}*/
						}
					}
				}
			}
			else if (waitResult == WAIT_FAILED)
			{
				break;
			}
		}

	gt_end:
		RevertThreadPriority();
		sptr->pClient->Stop();
		SAFE_RELEASE(ptrCaptureClient);
		SAFE_RELEASE(sptr->pClient);

		LOG_Print_Info(libThirdWrap_Audio, "CaptureAudioWindows::DoCaptureThread() Exit");
	}

	//回音消除AEC
	void CaptureAudioWindows::DoThreadDMO(const Device_INFO_sptr& sptr, const HFNCapture& hCapture)
	{
		//开启 MMCSS服务
		HRESULT hr = InitThreadPriority();
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoCaptureThread() Open MMCSS Service Failed, hr:%x!!", hr);
			goto gt_end;
		}

		UINT _recAudioFrameSize = sptr->Format.nBlockAlign;
		UINT32 _recBlockSize = sptr->Format.nSamplesPerSec / 100;

		IMediaBuffer*	mediaBuffer = new MediaBufferImpl(_recBlockSize * _recAudioFrameSize);
		//IMediaBuffer*	mediaBuffer = new MediaBufferImpl(sptr->Format.nSamplesPerSec * sptr->Format.nBlockAlign);
		DMO_OUTPUT_DATA_BUFFER dmoBuffer = { 0 };
		dmoBuffer.pBuffer = mediaBuffer;

		while (true)
		{
			try
			{
				ThreadWrap::interruption_point();

				DWORD dwStatus = 0;
				dmoBuffer.dwStatus = 0;
				// Poll the DMO for AEC processed capture data. The DMO will
				// copy available data to |dmoBuffer|, and should only return
				// 10 ms frames. The value of |dwStatus| should be ignored.
				hr = _ptrDmo->ProcessOutput(0, 1, &dmoBuffer, &dwStatus);
				dwStatus = dmoBuffer.dwStatus;

				if (FAILED(hr))
				{
					LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoThreadDMO() ProcessOutput Failed, hr:%x!!", hr);
					break;
				}

				if (!(dwStatus & DMO_OUTPUT_DATA_BUFFERF_INCOMPLETE))
				{
					// The DMO cannot currently produce more data. This is the
					// normal case; otherwise it means the DMO had more than 10 ms
					// of data available and ProcessOutput should be called again.
					Sleep(10);
				}
				else
				{
					ULONG bytesProduced = 0;
					BYTE* pData = NULL;
					// Get a pointer to the data buffer. This should be valid until
					// the next call to ProcessOutput.
					hr = mediaBuffer->GetBufferAndLength(&pData, &bytesProduced);
					if (FAILED(hr))
					{
						LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoThreadDMO() GetBufferAndLength Failed, hr:%x!!", hr);
						break;
					}

					if (bytesProduced > 0)
					{
						if (pData != NULL)
						{
							//test
							static FILE* fil = fopen("D:\\out\\AEC_my.pcm", "wb");
							if (fil != NULL)
							{
								fwrite(pData, 1, bytesProduced, fil);
								fflush(fil);
							}
							//test

							if (hCapture)
								hCapture(sptr, pData, bytesProduced);
						}
					}

					// Reset length to indicate buffer availability.
					hr = mediaBuffer->SetLength(0);
					if (FAILED(hr))
					{
						LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::DoThreadDMO() SetLength Failed, hr:%x!!", hr);
						break;
					}
				}
			}
			catch (const thread_interrupted& e)
			{
				switch (e.get_code())
				{
				case ThreadData::en_INTERRUPTED_EXIT:	//线程退出
					goto gt_end;
					break;
				}
				break;
			}

			//信号可以保证线程睡眠，降低cpu消耗
			/*DWORD waitResult = WaitForSingleObject(_hSamplesReadyEvent, INFINITE);
			if (waitResult == WAIT_OBJECT_0)
			{*/
			

			/*if (!(dwStatus & DMO_OUTPUT_DATA_BUFFERF_INCOMPLETE))
			{
				// The DMO cannot currently produce more data. This is the
				// normal case; otherwise it means the DMO had more than 10 ms
				// of data available and ProcessOutput should be called again.
				break;
			}*/

			/*}
			else if (waitResult == WAIT_FAILED)
			{
				break;
			}*/
		}

	gt_end:
		RevertThreadPriority();
		if (mediaBuffer != NULL)
		{
			delete mediaBuffer; 
			mediaBuffer = NULL;
		}
	}

	IAudioCaptureClient* CaptureAudioWindows::GetClient(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format,
		HANDLE hCaptureSamplesReadyEvent)
	{
		if (!Device_sptr)
			return NULL;

		HRESULT hr;
		if (!IsFormatSupported(Device_sptr, Format))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetClient() IsFormatSupported nChannels=%d, nSamplesPerSec=%d is not supported!!",
				Format.nChannels, Format.nSamplesPerSec);
			return NULL;
		}

		if (!Device_sptr->isCaptureInit)
		{
			// Create a capturing stream.
			hr = Device_sptr->pClient->Initialize(
				AUDCLNT_SHAREMODE_SHARED,             // share Audio Engine with other applications
				AUDCLNT_STREAMFLAGS_EVENTCALLBACK |   // processing of the audio buffer by the client will be event driven
				AUDCLNT_STREAMFLAGS_NOPERSIST,        // volume and mute settings for an audio session will not persist across system restarts
				0,                                    // required for event-driven shared mode
				0,                                    // periodicity
				&Format,                              // selected wave format
				NULL);                                // session GUID

			if (FAILED(hr))
			{
				LOG_Print_Error(libThirdWrap_Audio,
					"CaptureAudioWindows::GetCaptureClient() Initialize() nChannels=%d, nSamplesPerSec=%d is not supported!!",
					Format.nChannels, Format.nSamplesPerSec);
				return NULL;
			}

			Device_sptr->isCaptureInit = true;
		}
		
		UINT bufferFrameCount(0);
		hr = Device_sptr->pClient->GetBufferSize(&bufferFrameCount);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetCaptureClient() GetBufferSize() Failed, hr:%d!!", hr);
			return NULL;
		}

		// Set the event handle that the system signals when an audio buffer is ready
		// to be processed by the client.
		hr = Device_sptr->pClient->SetEventHandle(hCaptureSamplesReadyEvent);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetCaptureClient() SetEventHandle() Failed, hr:%d!!", hr);
			return NULL;
		}

		IAudioCaptureClient* ptrCaptureClient = NULL;

		hr = Device_sptr->pClient->GetService(
			__uuidof(IAudioCaptureClient),
			(void**)&ptrCaptureClient);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio,
				"CaptureAudioWindows::GetCaptureClient() GetService() Failed, hr:%d!!", hr);
			return NULL;
		}

		return ptrCaptureClient;
	}

	DWORD CaptureAudioWindows::InitThreadPriority(void)
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

	void CaptureAudioWindows::RevertThreadPriority(void)
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

	int32_t	CaptureAudioWindows::Stop(void)
	{
		if (_hSamplesReadyEvent != NULL)
		{
			CloseHandle(_hSamplesReadyEvent);
			_hSamplesReadyEvent = NULL;
		}

		if (_builtInAecEnabled)
		{
			HRESULT hr = _ptrDmo->FreeStreamingResources();
			if (FAILED(hr))
			{
				LOG_Print_Error(libThirdWrap_Audio,
					"CaptureAudioWindows::Stop() FreeStreamingResources Failed, hr:%x!!", hr);
			}

			_Thread.exit();
		}

		_Thread.join();

		return 1;
	}

	bool CaptureAudioWindows::IsFormatSupported(const Device_INFO_sptr& Device_sptr, const tWAVEFORMATEX& Format)
	{
		if (!Device_sptr)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::IsFormatSupported() Not Found Microphone!!");
			return false;
		}

		bool isRet = true;
		HRESULT hr;

		if (Device_sptr->pClient == NULL)
		{
			hr = Device_sptr->pDevice->Activate(
				__uuidof(IAudioClient),
				CLSCTX_ALL,
				NULL,
				(void**)&Device_sptr->pClient);
			if (FAILED(hr) || Device_sptr->pClient == NULL)
			{
				LOG_Print_Error(libThirdWrap_Audio,
					"CaptureAudioWindows::IsFormatSupported() Activate() Failed, hr:%x!!", hr);
				return NULL;
			}
		}

		WAVEFORMATEX*	pWfxClosestMatch = NULL;

		hr = Device_sptr->pClient->IsFormatSupported(
			AUDCLNT_SHAREMODE_SHARED,
			&Format,
			&pWfxClosestMatch);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::IsFormatSupported() nChannels=%d, nSamplesPerSec=%d is not supported",
				Format.nChannels, Format.nSamplesPerSec);

			isRet = false;
			goto gt_end;
		}

gt_end:
		CoTaskMemFree(pWfxClosestMatch);
		return isRet;
	}

	int CaptureAudioWindows::SetDMOProperties(Device_INFO_sptr Device_sptr)
	{
		if (_ptrDmo == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetDMOProperties() AEC Object is Null");
			return -1;
		}

		SAFE_RELEASE(_ptrDmoPS);

		HRESULT hr = S_OK;
		hr = _ptrDmo->QueryInterface(IID_IPropertyStore,
			reinterpret_cast<void**>(&_ptrDmoPS));
		if (FAILED(hr) || _ptrDmoPS == NULL)
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetDMOProperties() failed (hr=0x%x)",
				hr);
			return -1;
		}

		// Set the AEC system mode.
		// SINGLE_CHANNEL_AEC - AEC processing only.
		if (SetVtI4Property(_ptrDmoPS,
			MFPKEY_WMAAECMA_SYSTEM_MODE,
			SINGLE_CHANNEL_NSAGC/*SINGLE_CHANNEL_AEC*/) == -1)
		{
			return -1;
		}

		// Set the AEC source mode.
		// VARIANT_TRUE - Source mode (we poll the AEC for captured data).
		if (SetBoolProperty(_ptrDmoPS,
			MFPKEY_WMAAECMA_DMO_SOURCE_MODE,
			VARIANT_TRUE) == -1)
		{
			return -1;
		}

		// Enable the feature mode.
		// This lets us override all the default processing settings below.
		if (SetBoolProperty(_ptrDmoPS,
			MFPKEY_WMAAECMA_FEATURE_MODE,
			VARIANT_TRUE) == -1)
		{
			return -1;
		}

		// Disable analog AGC (default enabled).
		if (SetBoolProperty(_ptrDmoPS,
			MFPKEY_WMAAECMA_MIC_GAIN_BOUNDER,
			VARIANT_FALSE) == -1)
		{
			return -1;
		}

		// Disable noise suppression (default enabled).
		// 0 - Disabled, 1 - Enabled
		if (SetVtI4Property(_ptrDmoPS,
			MFPKEY_WMAAECMA_FEATR_NS,
			0) == -1)
		{
			return -1;
		}

		// Relevant parameters to leave at default settings:
		// MFPKEY_WMAAECMA_FEATR_AGC - Digital AGC (disabled).
		// MFPKEY_WMAAECMA_FEATR_CENTER_CLIP - AEC center clipping (enabled).
		// MFPKEY_WMAAECMA_FEATR_ECHO_LENGTH - Filter length (256 ms).
		//   TODO(andrew): investigate decresing the length to 128 ms.
		// MFPKEY_WMAAECMA_FEATR_FRAME_SIZE - Frame size (0).
		//   0 is automatic; defaults to 160 samples (or 10 ms frames at the
		//   selected 16 kHz) as long as mic array processing is disabled.
		// MFPKEY_WMAAECMA_FEATR_NOISE_FILL - Comfort noise (enabled).
		// MFPKEY_WMAAECMA_FEATR_VAD - VAD (disabled).

		// Set the devices selected by VoE. If using a default device, we need to
		// search for the device index.
		int inDevIndex = Device_sptr->uIndex;
		int outDevIndex = 0;

		DWORD devIndex = static_cast<uint32_t>(outDevIndex << 16) +
			static_cast<uint32_t>(0x0000ffff & inDevIndex);
		printf("Capture device index: %d, render device index: %d",
			inDevIndex, outDevIndex);
		if (SetVtI4Property(_ptrDmoPS,
			MFPKEY_WMAAECMA_DEVICE_INDEXES,
			devIndex) == -1)
		{
			return -1;
		}

		DMO_MEDIA_TYPE mt = { 0 };
		hr = MoInitMediaType(&mt, sizeof(WAVEFORMATEX));
		if (FAILED(hr))
		{
			MoFreeMediaType(&mt);
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetDMOProperties() MoInitMediaType() failed (hr=0x%x)",
				hr);
			return -1;
		}
		mt.majortype = MEDIATYPE_Audio;
		mt.subtype = MEDIASUBTYPE_PCM;
		mt.formattype = FORMAT_WaveFormatEx;

		// Supported formats
		// nChannels: 1 (in AEC-only mode)
		// nSamplesPerSec: 8000, 11025, 16000, 22050
		// wBitsPerSample: 16
		WAVEFORMATEX* ptrWav = reinterpret_cast<WAVEFORMATEX*>(mt.pbFormat);
		ptrWav->wFormatTag = WAVE_FORMAT_PCM;
		ptrWav->nChannels = 1;
		// 16000 is the highest we can support with our resampler.
		ptrWav->nSamplesPerSec = 16000;
		ptrWav->nAvgBytesPerSec = 32000;
		ptrWav->nBlockAlign = 2;
		ptrWav->wBitsPerSample = 16;
		ptrWav->cbSize = 0;

		//拷贝支持格式
		memcpy(&Device_sptr->Format, ptrWav, sizeof(WAVEFORMATEX));

		// Set the DMO output format parameters.
		hr = _ptrDmo->SetOutputType(kAecCaptureStreamIndex, &mt, 0);
		MoFreeMediaType(&mt);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetDMOProperties() SetOutputType() failed (hr=0x%x)",
				hr);
			return -1;
		}

		// Optional, but if called, must be after media types are set.
		hr = _ptrDmo->AllocateStreamingResources();
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetDMOProperties() AllocateStreamingResources() failed (hr=0x%x)",
				hr);
			return -1;
		}

		return 1;
	}

	int CaptureAudioWindows::SetBoolProperty(IPropertyStore* ptrPS,
		REFPROPERTYKEY key,
		VARIANT_BOOL value)
	{
		PROPVARIANT pv;
		PropVariantInit(&pv);
		pv.vt = VT_BOOL;
		pv.boolVal = value;
		HRESULT hr = ptrPS->SetValue(key, pv);
		PropVariantClear(&pv);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetBoolProperty() failed (hr=0x%x)",
				hr);
			return -1;
		}
		return 1;
	}

	int CaptureAudioWindows::SetVtI4Property(IPropertyStore* ptrPS,
		REFPROPERTYKEY key,
		LONG value)
	{
		PROPVARIANT pv;
		PropVariantInit(&pv);
		pv.vt = VT_I4;
		pv.lVal = value;
		HRESULT hr = ptrPS->SetValue(key, pv);
		PropVariantClear(&pv);
		if (FAILED(hr))
		{
			LOG_Print_Error(libThirdWrap_Audio, "CaptureAudioWindows::SetVtI4Property() failed (hr=0x%x)",
				hr);
			return -1;
		}
		return 1;
	}
}
