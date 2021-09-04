// Copyright(c) 2021 Viktor Pramberg
#include "WootingController.h"
#include <Interfaces/IPluginManager.h>
#include "WootingDevice.h"
#include "DeviceRGB.h"

#include <wooting-rgb-sdk.h>

FWootingController::FWootingController()
{
	const FString BaseDir = IPluginManager::Get().FindPlugin("DeviceRGB")->GetBaseDir();
	const FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/Wooting/redist/x64/wooting-rgb-sdk64.dll"));

	SDKHandle = FPlatformProcess::GetDllHandle(*LibraryPath);
}

FWootingController::~FWootingController()
{
	if (SDKHandle)
	{
		wooting_rgb_close();
	}

	FPlatformProcess::FreeDllHandle(SDKHandle);
	SDKHandle = nullptr;
}

TUniquePtr<FWootingController> FWootingController::Construct()
{
	auto Controller = TUniquePtr<FWootingController>(new FWootingController());

	if (!Controller->SDKHandle || !wooting_rgb_kbd_connected())
	{
		return nullptr;
	}

    Controller->AddDevice<FWootingDevice>();

    return MoveTemp(Controller);
}

void FWootingController::FlushBuffers()
{
	FlushBuffersImpl();
}

void FWootingController::FlushBuffersImpl()
{
	wooting_rgb_array_update_keyboard();
}

void FWootingController::SetEnabled(bool bEnabled)
{
	if (!bEnabled)
	{
		wooting_rgb_reset_rgb();
	}
}
