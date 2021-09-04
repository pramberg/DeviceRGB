// Copyright(c) 2021 Viktor Pramberg
#include "CorsairController.h"
#include <Interfaces/IPluginManager.h>
#include "CorsairDevice.h"
#include "DeviceRGB.h"

#include <CUESDK.h>

FCorsairController::FCorsairController()
{
	const FString BaseDir = IPluginManager::Get().FindPlugin("DeviceRGB")->GetBaseDir();
	const FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/CUESDK/redist/x64/CUESDK.x64_2017.dll"));

	SDKHandle = FPlatformProcess::GetDllHandle(*LibraryPath);
}

FCorsairController::~FCorsairController()
{
	FPlatformProcess::FreeDllHandle(SDKHandle);
	SDKHandle = nullptr;
}

TUniquePtr<FCorsairController> FCorsairController::Construct()
{
	auto Controller = TUniquePtr<FCorsairController>(new FCorsairController());
	if (!Controller->GetDLLHandle())
	{
		return nullptr;
	}

	CorsairPerformProtocolHandshake();
	if (CorsairGetLastError())
	{
		UE_LOG(LogDeviceRGB, Display, TEXT("Failed to connect to Corsair CUE service."));
		return nullptr;
	}

	for (int32 DeviceIndex = 0; DeviceIndex < CorsairGetDeviceCount(); DeviceIndex++)
	{
		Controller->AddDevice<FCorsairDevice>(DeviceIndex);
	}

	return MoveTemp(Controller);
}

void FCorsairController::FlushBuffers()
{
	FlushBuffersImpl();
}

void FCorsairController::FlushBuffersImpl()
{
	CorsairSetLedsColorsFlushBufferAsync([](void* Context, bool Result, CorsairError Error)
	{
		if (Error != CE_Success)
		{
			UE_LOG(LogDeviceRGB, Error, TEXT("Failed to flush Corsair color buffer"));
		}
	}, nullptr);
}

void FCorsairController::SetEnabled(bool bEnabled)
{
	CorsairSetLayerPriority(bEnabled ? 255 : 0);
}
