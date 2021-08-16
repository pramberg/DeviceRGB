// Copyright(c) 2021 Viktor Pramberg
#include "RazerController.h"
#include "RazerDevice.h"
#include "IDeviceRGB.h"

#include <GeneralProjectSettings.h>

FRazerController::FRazerController()
{
	constexpr auto LibraryPath = TEXT("RzChromaSDK64.dll");

	SDKHandle = FPlatformProcess::GetDllHandle(LibraryPath);
	if (!SDKHandle)
	{
		return;
	}

#define DEVICERGB_IMPORT_FUNC(InName) InName = reinterpret_cast<decltype(InName)>(FPlatformProcess::GetDllExport(SDKHandle, TEXT(#InName)))

	DEVICERGB_IMPORT_FUNC(Init);
	DEVICERGB_IMPORT_FUNC(UnInit);
	DEVICERGB_IMPORT_FUNC(InitSDK);
	DEVICERGB_IMPORT_FUNC(CreateEffect);
	DEVICERGB_IMPORT_FUNC(CreateKeyboardEffect);
	DEVICERGB_IMPORT_FUNC(CreateMouseEffect);
	DEVICERGB_IMPORT_FUNC(CreateHeadsetEffect);
	DEVICERGB_IMPORT_FUNC(CreateMousepadEffect);
	DEVICERGB_IMPORT_FUNC(CreateKeypadEffect);
	DEVICERGB_IMPORT_FUNC(CreateChromaLinkEffect);
	DEVICERGB_IMPORT_FUNC(SetEffect);
	DEVICERGB_IMPORT_FUNC(DeleteEffect);
	DEVICERGB_IMPORT_FUNC(QueryDevice);

#undef DEVICERGB_IMPORT_FUNC
}

FRazerController::~FRazerController()
{
	if (UnInit)
	{
		UnInit();
	}
	FPlatformProcess::FreeDllHandle(SDKHandle);
	SDKHandle = nullptr;
}

TUniquePtr<FRazerController> FRazerController::Construct()
{
	auto Controller = TUniquePtr<FRazerController>(new FRazerController());
	if (!Controller->GetDLLHandle())
		return nullptr;

	/* InitSDK is a new and nicer way of initializing, but it may not be supported by older clients */
	if (Controller->InitSDK)
	{
		const auto* ProjectSettings = GetDefault<UGeneralProjectSettings>();

		const auto StringOrDefault = [](const FString& InDesired, const TCHAR* InDefault)
		{
			return InDesired.IsEmpty() ? InDefault : *InDesired;
		};

		#define DEVICERGB_SET_APPINFO_PROPERTY(InPropertyName, InValue) _tcscpy_s(InPropertyName, UE_ARRAY_COUNT(InPropertyName), InValue);

		ChromaSDK::APPINFOTYPE AppInfo{};
		DEVICERGB_SET_APPINFO_PROPERTY(AppInfo.Title, StringOrDefault(ProjectSettings->ProjectName, *FApp::GetName()));
		DEVICERGB_SET_APPINFO_PROPERTY(AppInfo.Description, StringOrDefault(ProjectSettings->Description, TEXT("Default Description - Please fill in 'Description' in Project Settings")));
		DEVICERGB_SET_APPINFO_PROPERTY(AppInfo.Author.Name, StringOrDefault(ProjectSettings->CompanyName, TEXT("Default Author - Please fill in 'CompanyName' in Project Settings")));
		DEVICERGB_SET_APPINFO_PROPERTY(AppInfo.Author.Contact, StringOrDefault(ProjectSettings->Homepage, TEXT("Contact - Please fill in 'Homepage' in Project Settings")));
		AppInfo.SupportedDevice = (0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20);
		AppInfo.Category = 1;

		#undef DEVICERGB_SET_APPINFO_PROPERTY

		const auto Result = Controller->InitSDK(&AppInfo);
		if (Result != RZRESULT_SUCCESS)
		{
			return nullptr;
		}
	}
	else if (Controller->Init)
	{
		const auto Result = Controller->Init();
		if (Result != RZRESULT_SUCCESS)
		{
			return nullptr;
		}
	}
	else
	{
		checkNoEntry();
	}

	Controller->TryAddDevices();
	return MoveTemp(Controller);
}

int32 FRazerController::GetNumberOfDevices() const
{
	return Devices.Num();
}

void FRazerController::ForEachDevice(TFunctionRef<void(IDeviceRGB*)> InFunction)
{
	for (auto& Device : Devices)
	{
		InFunction(Device.Get());
	}
}

bool FRazerController::IsDeviceConnected(RZDEVICEID InDeviceID) const
{
#if DEVICERGB_ENABLE_DEVICE_CHECKS
	ChromaSDK::DEVICE_INFO_TYPE Info;
	QueryDevice(InDeviceID, Info);
	return Info.Connected == 1;
#else
	return true;
#endif
}

bool FRazerController::HasAnyKeyboard() const
{
	return CreateKeyboardEffect && (
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

bool FRazerController::HasAnyMouse() const
{
	return CreateMouseEffect && (
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

bool FRazerController::HasAnyHeadset() const
{
	return CreateHeadsetEffect && (
		IsDeviceConnected(ChromaSDK::KRAKEN71_CHROMA) ||
		IsDeviceConnected(ChromaSDK::MANOWAR_CHROMA) ||
		IsDeviceConnected(ChromaSDK::KRAKEN71_REFRESH_CHROMA) ||
		IsDeviceConnected(ChromaSDK::KRAKEN_KITTY)
		);
}

bool FRazerController::HasAnyMousepad() const
{
	return CreateMousepadEffect && (
		IsDeviceConnected(ChromaSDK::FIREFLY_CHROMA)
		);
}

bool FRazerController::HasAnyKeypad() const
{
	return CreateKeypadEffect && (
		IsDeviceConnected(ChromaSDK::TARTARUS_CHROMA) ||
		IsDeviceConnected(ChromaSDK::ORBWEAVER_CHROMA)
		);
}

bool FRazerController::HasAnyChromaLink() const
{
	return CreateChromaLinkEffect && (
		IsDeviceConnected(ChromaSDK::NOMMO_CHROMA) ||
		IsDeviceConnected(ChromaSDK::NOMMO_CHROMA_PRO)
		);
}

void FRazerController::TryAddDevices()
{
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
