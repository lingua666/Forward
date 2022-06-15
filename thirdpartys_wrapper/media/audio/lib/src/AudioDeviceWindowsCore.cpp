
#include <libThirdWrap_Audio/AudioDeviceWindowsCore.h>
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

	// Constructor for STA initialization.
	ScopedCOMInitializer::ScopedCOMInitializer()
	{
		Initialize(COINIT_APARTMENTTHREADED);
	}

	// Constructor for MTA initialization.
	ScopedCOMInitializer::ScopedCOMInitializer(SelectMTA mta)
	{
		Initialize(COINIT_MULTITHREADED);
	}

	ScopedCOMInitializer::~ScopedCOMInitializer() 
	{
		if (SUCCEEDED(hr_))
			CoUninitialize();
	}

	bool ScopedCOMInitializer::succeeded() const 
	{ 
		return SUCCEEDED(hr_);
	}

	void ScopedCOMInitializer::Initialize(COINIT init)
	{
		hr_ = CoInitializeEx(NULL, init);
	}

}
