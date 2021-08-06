// Copyright(c) 2021 Viktor Pramberg
#pragma once
#include "CoreMinimal.h"
#include "IDeviceSDK.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

// Device queries fail when testing with Razer's emulator, so this macro makes it possible to override those checks.
#define DEVICERGB_ENABLE_DEVICE_CHECKS 0

class IDevice;

// Because Razer's API is kind of silly in combination with DeviceRGB's design; we create one device of each type, and just call its "Create*Device*Effect()" in SetColors
enum class ERazerDeviceType : uint8
{
	None,
	Keyboard,
	Headset,
	Mousepad,
	Mouse,
	Keypad,
	ChromaLink,
};

class FRazerDeviceSDK : public IDeviceSDK
{
public:
	FRazerDeviceSDK();
	virtual ~FRazerDeviceSDK() override;

	virtual int32 GetNumberOfDevices() const override;
	virtual void FlushBuffers() override;

	static void FlushBuffersImpl();

	virtual void ForEachDevice(TFunctionRef<void(IDevice*)> InFunction) override;

	virtual bool IsValid() const override;
private:

#if PLATFORM_WINDOWS
	using INIT = RZRESULT(*)(void);
	using INITSDK = RZRESULT(*)(ChromaSDK::APPINFOTYPE*);
	using UNINIT = RZRESULT(*)(void);
	using CREATEEFFECT = RZRESULT(*)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using CREATEKEYBOARDEFFECT = RZRESULT(*)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using CREATEHEADSETEFFECT = RZRESULT(*)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using CREATEMOUSEPADEFFECT = RZRESULT(*)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using CREATEMOUSEEFFECT = RZRESULT(*)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using CREATEKEYPADEFFECT = RZRESULT(*)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using CREATECHROMALINKEFFECT = RZRESULT(*)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	using SETEFFECT = RZRESULT(*)(RZEFFECTID EffectId);
	using DELETEEFFECT =  RZRESULT(*)(RZEFFECTID EffectId);
	using REGISTEREVENTNOTIFICATION = RZRESULT(*)(HWND hWnd);
	using UNREGISTEREVENTNOTIFICATION = RZRESULT(*)(void);
	using QUERYDEVICE = RZRESULT(*)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);

	INIT Init = nullptr;
	INITSDK InitSDK = nullptr;
	UNINIT UnInit = nullptr;
	CREATEEFFECT CreateEffect = nullptr;
	CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
	CREATEMOUSEEFFECT CreateMouseEffect = nullptr;
	CREATEHEADSETEFFECT CreateHeadsetEffect = nullptr;
	CREATEMOUSEPADEFFECT CreateMousepadEffect = nullptr;
	CREATEKEYPADEFFECT CreateKeypadEffect = nullptr;
	CREATECHROMALINKEFFECT CreateChromaLinkEffect = nullptr;
	SETEFFECT SetEffect = nullptr;
	DELETEEFFECT DeleteEffect = nullptr;
	QUERYDEVICE QueryDevice = nullptr;

	bool VerifySDK() const 
	{
		return ensure(
			Init &&
			InitSDK &&
			UnInit &&
			CreateEffect &&
			CreateKeypadEffect &&
			CreateMouseEffect &&
			CreateHeadsetEffect &&
			CreateMousepadEffect &&
			CreateKeypadEffect &&
			SetEffect &&
			DeleteEffect &&
			QueryDevice
		);
	}

	// Used to check whether to create devices
	bool IsDeviceConnected(RZDEVICEID InDeviceID) const;
	bool HasAnyKeyboard() const;
	bool HasAnyMouse() const;
	bool HasAnyHeadset() const;
	bool HasAnyMousepad() const;
	bool HasAnyKeypad() const;
	bool HasAnyChromaLink() const;

	template<ERazerDeviceType DeviceType> friend class FRazerDevice;
#endif

	void* SDKHandle = nullptr;
	TArray<TUniquePtr<IDevice>> Devices;
};