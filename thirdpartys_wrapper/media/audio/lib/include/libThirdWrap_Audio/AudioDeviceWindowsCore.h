
#ifndef __AUDIO_DEVICE_WINDOWS_CORE_20201229104504_1444199361_H__
#define __AUDIO_DEVICE_WINDOWS_CORE_20201229104504_1444199361_H__

// MediaServerSample.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#if defined(PLATFORM_OS_FAMILY_WINDOWS)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

// Macro that calls a COM method returning HRESULT value.
#define EXIT_ON_ERROR(hres)    do { if (FAILED(hres)) goto Exit; } while(0)

// Macro that continues to a COM error.
#define CONTINUE_ON_ERROR(hres) do { if (FAILED(hres)) goto Next; } while(0)

#include <wmcodecdsp.h>			//该头文件必须在<AudioClient.h>和<AudioPolicy.h>，否则编译报错
#include <MMDeviceAPI.h>		//该头文件必须在<AudioClient.h>和<AudioPolicy.h>，否则编译报错
#include <AudioClient.h>
#include <AudioPolicy.h>
#include <avrt.h>

#include "External.h"

// Use Multimedia Class Scheduler Service (MMCSS) to boost the thread priority
#pragma comment( lib, "avrt.lib" )
#pragma comment( lib, "wmcodecdspuuid.lib" )
#pragma comment( lib, "dmoguids.lib" )
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "msdmo.lib" )
// AVRT function pointers

namespace _audio_ {

	typedef _foundation_::_string_t		_audio_string_t;
	typedef _foundation_::_string_big_t	_audio_string_big_t;

	typedef BOOL(WINAPI *PAvRevertMmThreadCharacteristics)(HANDLE);
	typedef HANDLE(WINAPI *PAvSetMmThreadCharacteristicsA)(LPCSTR, LPDWORD);
	typedef BOOL(WINAPI *PAvSetMmThreadPriority)(HANDLE, AVRT_PRIORITY);

	class ScopedCOMInitializer {
	public:
		// Enum value provided to initialize the thread as an MTA instead of STA.
		enum SelectMTA { kMTA };

		// Constructor for STA initialization.
		ScopedCOMInitializer();

		// Constructor for MTA initialization.
		explicit ScopedCOMInitializer(SelectMTA mta);

		ScopedCOMInitializer::~ScopedCOMInitializer();

		bool succeeded() const;

	private:
		void Initialize(COINIT init);

		HRESULT hr_;

		ScopedCOMInitializer(const ScopedCOMInitializer&);
		void operator=(const ScopedCOMInitializer&);
	};

	struct AudioDeviceModule
	{
	public:
		enum ErrorCode {
			kAdmErrNone = 0,
			kAdmErrArgument = 1
		};

		enum AudioLayer {
			kPlatformDefaultAudio = 0,
			kWindowsWaveAudio = 1,
			kWindowsCoreAudio = 2,
			kLinuxAlsaAudio = 3,
			kLinuxPulseAudio = 4,
			kAndroidJavaAudio = 5,
			kAndroidOpenSLESAudio = 6,
			kAndroidJavaInputAndOpenSLESOutputAudio = 7,
			kDummyAudio = 8
		};

		enum WindowsDeviceType {
			kDefaultCommunicationDevice = -1,
			kDefaultDevice = -2
		};

		enum BufferType {
			kFixedBufferSize = 0,
			kAdaptiveBufferSize = 1
		};

		enum ChannelType {
			kChannelLeft = 0,
			kChannelRight = 1,
			kChannelBoth = 2
		};
	};

	enum
	{
		kAecCaptureStreamIndex = 0,
		kAecRenderStreamIndex = 1
	};

	typedef struct tagDevice_INFO
	{
		_audio_string_t	sDeviceID;
		_audio_string_t	sName;
		UInt32			uIndex;
		EDataFlow		Type;
		IMMDevice*		pDevice;
		IAudioClient*	pClient;
		tWAVEFORMATEX	Format;
		bool			isCaptureInit;
	}tagDevice_INFO;

	typedef SmartPTR<tagDevice_INFO>		Device_INFO_sptr;
	typedef LinkList_type<Device_INFO_sptr>	Device_INFO_List;

	// An implementation of IMediaBuffer, as required for
	// IMediaObject::ProcessOutput(). After consuming data provided by
	// ProcessOutput(), call SetLength() to update the buffer availability.
	//
	// Example implementation:
	// http://msdn.microsoft.com/en-us/library/dd376684(v=vs.85).aspx
	class MediaBufferImpl : public IMediaBuffer
	{
	public:
		explicit MediaBufferImpl(DWORD maxLength)
			: _data(new BYTE[maxLength]),
			_length(0),
			_maxLength(maxLength),
			_refCount(0)
		{}

		// IMediaBuffer methods.
		STDMETHOD(GetBufferAndLength(BYTE** ppBuffer, DWORD* pcbLength))
		{
			if (!ppBuffer || !pcbLength)
			{
				return E_POINTER;
			}

			*ppBuffer = _data;
			*pcbLength = _length;

			return S_OK;
		}

		STDMETHOD(GetMaxLength(DWORD* pcbMaxLength))
		{
			if (!pcbMaxLength)
			{
				return E_POINTER;
			}

			*pcbMaxLength = _maxLength;
			return S_OK;
		}

		STDMETHOD(SetLength(DWORD cbLength))
		{
			if (cbLength > _maxLength)
			{
				return E_INVALIDARG;
			}

			_length = cbLength;
			return S_OK;
		}

		// IUnknown methods.
		STDMETHOD_(ULONG, AddRef())
		{
			return InterlockedIncrement(&_refCount);
		}

		STDMETHOD(QueryInterface(REFIID riid, void** ppv))
		{
			if (!ppv)
			{
				return E_POINTER;
			}
			else if (riid != IID_IMediaBuffer && riid != IID_IUnknown)
			{
				return E_NOINTERFACE;
			}

			*ppv = static_cast<IMediaBuffer*>(this);
			AddRef();
			return S_OK;
		}

		STDMETHOD_(ULONG, Release())
		{
			LONG refCount = InterlockedDecrement(&_refCount);
			if (refCount == 0)
			{
				delete this;
			}

			return refCount;
		}

	private:
		~MediaBufferImpl()
		{
			delete[] _data;
		}

		BYTE* _data;
		DWORD _length;
		const DWORD _maxLength;
		LONG _refCount;
	};
}

//#endif

#endif
