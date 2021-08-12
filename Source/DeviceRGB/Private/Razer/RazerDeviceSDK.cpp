// Copyright(c) 2021 Viktor Pramberg
#include "RazerDeviceSDK.h"
#include "RazerDevice.h"
#include "IDevice.h"

#include <GeneralProjectSettings.h>

#define IMPORT_FUNC(InName) InName = reinterpret_cast<decltype(InName)>(FPlatformProcess::GetDllExport(SDKHandle, TEXT(#InName)))

FRazerDeviceSDK::FRazerDeviceSDK()
{
	constexpr auto LibraryPath = TEXT("RzChromaSDK64.dll");

	SDKHandle = FPlatformProcess::GetDllHandle(LibraryPath);
	if (!SDKHandle)
	{
		return;
	}

	const auto* ProjectSettings = GetDefault<UGeneralProjectSettings>();
	
	const auto StringOrDefault = [](const FString& InDesired, const TCHAR* InDefault)
	{
		return InDesired.IsEmpty() ? InDefault : *InDesired;
	};

	ChromaSDK::APPINFOTYPE AppInfo{};
	_tcscpy_s(AppInfo.Title, 256, StringOrDefault(ProjectSettings->ProjectName, *FApp::GetName()));
	_tcscpy_s(AppInfo.Description, 1024, StringOrDefault(ProjectSettings->Description, TEXT("Default Description - Please fill in 'Description' in Project Settings")));
	_tcscpy_s(AppInfo.Author.Name, 256, StringOrDefault(ProjectSettings->CompanyName, TEXT("Default Author - Please fill in 'CompanyName' in Project Settings")));
	_tcscpy_s(AppInfo.Author.Contact, 256, StringOrDefault(ProjectSettings->Homepage, TEXT("Contact - Please fill in 'Homepage' in Project Settings")));
	AppInfo.SupportedDevice = (0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20);
	AppInfo.Category = 1;

	IMPORT_FUNC(Init);
	IMPORT_FUNC(UnInit);
	IMPORT_FUNC(InitSDK);
	IMPORT_FUNC(CreateEffect);
	IMPORT_FUNC(CreateKeyboardEffect);
	IMPORT_FUNC(CreateMouseEffect);
	IMPORT_FUNC(CreateHeadsetEffect);
	IMPORT_FUNC(CreateMousepadEffect);
	IMPORT_FUNC(CreateKeypadEffect);
	IMPORT_FUNC(CreateChromaLinkEffect);
	IMPORT_FUNC(SetEffect);
	IMPORT_FUNC(DeleteEffect);
	IMPORT_FUNC(QueryDevice);

	VerifySDK();

	const auto Result = InitSDK(&AppInfo);
	if (Result != RZRESULT_SUCCESS)
	{
		UnInit();
		FPlatformProcess::FreeDllHandle(SDKHandle);
		SDKHandle = nullptr;
		return;
	}

	if (HasAnyKeyboard())
	{
		Devices.Add(MakeUnique<FRazerDevice<ERazerDeviceType::Keyboard>>(this));
	}
	if (HasAnyHeadset())
	{
		Devices.Add(MakeUnique<FRazerDevice<ERazerDeviceType::Headset>>(this));
	}
	if (HasAnyMousepad())
	{
		Devices.Add(MakeUnique<FRazerDevice<ERazerDeviceType::Mousepad>>(this));
	}
	if (HasAnyMouse())
	{
		Devices.Add(MakeUnique<FRazerDevice<ERazerDeviceType::Mouse>>(this));
	}
	if (HasAnyKeypad())
	{
		Devices.Add(MakeUnique<FRazerDevice<ERazerDeviceType::Keypad>>(this));
	}
	if (HasAnyChromaLink())
	{
		Devices.Add(MakeUnique<FRazerDevice<ERazerDeviceType::ChromaLink>>(this));
	}
}

FRazerDeviceSDK::~FRazerDeviceSDK()
{
	if (SDKHandle)
	{
		UnInit();
		FPlatformProcess::FreeDllHandle(SDKHandle);
		SDKHandle = nullptr;
	}
}

int32 FRazerDeviceSDK::GetNumberOfDevices() const
{
	return Devices.Num();
}

void FRazerDeviceSDK::FlushBuffers()
{
	FlushBuffersImpl();
}

void FRazerDeviceSDK::FlushBuffersImpl()
{
}

void FRazerDeviceSDK::ForEachDevice(TFunctionRef<void(IDevice*)> InFunction)
{
	for (auto& Device : Devices)
	{
		InFunction(Device.Get());
	}
}

bool FRazerDeviceSDK::IsValid() const
{
	return SDKHandle != nullptr;
}

bool FRazerDeviceSDK::IsDeviceConnected(RZDEVICEID InDeviceID) const
{
#if DEVICERGB_ENABLE_DEVICE_CHECKS
	ChromaSDK::DEVICE_INFO_TYPE Info;
	QueryDevice(InDeviceID, Info);
	return Info.Connected == 1;
#else
	return true;
#endif
}

bool FRazerDeviceSDK::HasAnyKeyboard() const
{
	return (
		IsDeviceConnected(ChromaSDK::BLACKWIDOW_CHROMA) ||
		IsDeviceConnected(ChromaSDK::BLACKWIDOW_CHROMA_TE) ||
		IsDeviceConnected(ChromaSDK::DEATHSTALKER_CHROMA) ||
		IsDeviceConnected(ChromaSDK::OVERWATCH_KEYBOARD) ||
		IsDeviceConnected(ChromaSDK::BLACKWIDOW_X_CHROMA) ||
		IsDeviceConnected(ChromaSDK::BLACKWIDOW_X_TE_CHROMA) ||
		IsDeviceConnected(ChromaSDK::ORNATA_CHROMA) ||
		IsDeviceConnected(ChromaSDK::BLADE_STEALTH) ||
		IsDeviceConnected(ChromaSDK::BLADE) ||
		IsDeviceConnected(ChromaSDK::BLADE_PRO) ||
		IsDeviceConnected(ChromaSDK::HUNTSMAN) ||
		IsDeviceConnected(ChromaSDK::BLACKWIDOW_ELITE)
		);
}

bool FRazerDeviceSDK::HasAnyMouse() const
{
	return (
		IsDeviceConnected(ChromaSDK::DEATHADDER_CHROMA) ||
		IsDeviceConnected(ChromaSDK::MAMBA_CHROMA_TE) ||
		IsDeviceConnected(ChromaSDK::DIAMONDBACK_CHROMA) ||
		IsDeviceConnected(ChromaSDK::MAMBA_CHROMA) ||
		IsDeviceConnected(ChromaSDK::NAGA_EPIC_CHROMA) ||
		IsDeviceConnected(ChromaSDK::NAGA_CHROMA) ||
		IsDeviceConnected(ChromaSDK::OROCHI_CHROMA) ||
		IsDeviceConnected(ChromaSDK::NAGA_HEX_CHROMA) ||
		IsDeviceConnected(ChromaSDK::DEATHADDER_ELITE_CHROMA)
		);
}

bool FRazerDeviceSDK::HasAnyHeadset() const
{
	return (
		IsDeviceConnected(ChromaSDK::KRAKEN71_CHROMA) ||
		IsDeviceConnected(ChromaSDK::MANOWAR_CHROMA) ||
		IsDeviceConnected(ChromaSDK::KRAKEN71_REFRESH_CHROMA) ||
		IsDeviceConnected(ChromaSDK::KRAKEN_KITTY)
		);
}

bool FRazerDeviceSDK::HasAnyMousepad() const
{
	return (
		IsDeviceConnected(ChromaSDK::FIREFLY_CHROMA)
		);
}

bool FRazerDeviceSDK::HasAnyKeypad() const
{
	return (
		IsDeviceConnected(ChromaSDK::TARTARUS_CHROMA) ||
		IsDeviceConnected(ChromaSDK::ORBWEAVER_CHROMA)
		);
}

bool FRazerDeviceSDK::HasAnyChromaLink() const
{
	return (
		IsDeviceConnected(ChromaSDK::NOMMO_CHROMA) ||
		IsDeviceConnected(ChromaSDK::NOMMO_CHROMA_PRO)
		);
}
